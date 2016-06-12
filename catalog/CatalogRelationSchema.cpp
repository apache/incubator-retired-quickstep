/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "catalog/CatalogRelationSchema.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogErrors.hpp"
#include "catalog/CatalogRelationConstraints.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "types/Type.hpp"
#include "utility/PtrVector.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

using std::size_t;
using std::string;
using std::unordered_map;

namespace quickstep {

CatalogRelationSchema::CatalogRelationSchema(const serialization::CatalogRelationSchema &proto)
    : parent_(nullptr),
      id_(proto.relation_id()),
      name_(proto.name()),
      temporary_(proto.temporary()),
      num_nullable_attributes_(0),
      num_variable_length_attributes_(0),
      max_byte_length_(0),
      min_byte_length_(0),
      estimated_byte_length_(0),
      fixed_byte_length_(0),
      max_variable_byte_length_(0),
      min_variable_byte_length_(0),
      min_variable_byte_length_excluding_nullable_(0),
      estimated_variable_byte_length_(0),
      max_byte_lengths_(proto.attributes_size()),
      current_nullable_attribute_index_(-1),
      current_variable_length_attribute_index_(-1) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create CatalogRelationSchema from an invalid proto description:\n"
      << proto.DebugString();

  // Deserializing the attributes for the relation schema.
  for (int i = 0; i < proto.attributes_size(); ++i) {
    if (proto.attributes(i).IsInitialized()) {
      addAttribute(new CatalogAttribute(proto.attributes(i)));
    } else {
      attr_vec_.push_back(nullptr);
    }
  }

  if (proto.has_constraints()) {
    constraints_.reset(new CatalogRelationConstraints(proto.constraints()));
  } else {
    constraints_.reset(new CatalogRelationConstraints());
  }
}

bool CatalogRelationSchema::ProtoIsValid(const serialization::CatalogRelationSchema &proto) {
  if (!proto.IsInitialized()
      || proto.relation_id() == static_cast<relation_id>(-1)) {
    return false;
  }

  for (int i = 0; i < proto.attributes_size(); ++i) {
    if (!CatalogAttribute::ProtoIsValid(proto.attributes(i))) {
      return false;
    }
  }

  if (proto.has_constraints()
      && !CatalogRelationConstraints::ProtoIsValid(proto.constraints(),
                                                   proto.attributes_size())) {
    return false;
  }

  return true;
}

serialization::CatalogRelationSchema CatalogRelationSchema::getProto() const {
  serialization::CatalogRelationSchema proto;

  proto.set_relation_id(id_);
  proto.set_name(name_);
  proto.set_temporary(temporary_);

  for (PtrVector<CatalogAttribute, true>::const_iterator it = attr_vec_.begin();
       it != attr_vec_.end();
       ++it) {
    if (it.isNull()) {
      proto.add_attributes();
    } else {
      proto.add_attributes()->MergeFrom(it->getProto());
    }
  }

  proto.mutable_constraints()->CopyFrom(constraints_->getProto());

  return proto;
}

const CatalogAttribute* CatalogRelationSchema::getAttributeByName(
    const string &attr_name) const {
  unordered_map<string, CatalogAttribute*>::const_iterator it
      = attr_map_.find(ToLower(attr_name));
  if (it == attr_map_.end()) {
    return nullptr;
  } else {
    return it->second;
  }
}

CatalogAttribute* CatalogRelationSchema::getAttributeByNameMutable(
    const string &attr_name) {
  unordered_map<string, CatalogAttribute*>::const_iterator it
      = attr_map_.find(ToLower(attr_name));
  if (it == attr_map_.end()) {
    return nullptr;
  } else {
    return it->second;
  }
}

attribute_id CatalogRelationSchema::addAttribute(CatalogAttribute *new_attr) {
  const string lower_attr_name = ToLower(new_attr->getName());
  DCHECK(!hasAttributeWithName(lower_attr_name));

  if (attr_vec_.size() > static_cast<size_t>(kCatalogMaxID)) {
    throw CatalogIDOverflow("attribute");
  } else {
    attr_map_[lower_attr_name] = new_attr;
    attr_vec_.push_back(new_attr);
    new_attr->setParent(this);
    new_attr->setID(static_cast<attribute_id>(attr_vec_.size() - 1));

    const Type &attr_type = new_attr->getType();

    if (attr_type.isVariableLength()) {
      ++num_variable_length_attributes_;
      variable_length_attribute_indices_.push_back(++current_variable_length_attribute_index_);

      max_variable_byte_length_ += attr_type.maximumByteLength();
      min_variable_byte_length_ += attr_type.minimumByteLength();
      if (!attr_type.isNullable()) {
        min_variable_byte_length_excluding_nullable_ += attr_type.minimumByteLength();
      }
      estimated_variable_byte_length_ += attr_type.estimateAverageByteLength();
    } else {
      variable_length_attribute_indices_.push_back(kInvalidCatalogId);

      fixed_length_attribute_offsets_.resize(new_attr->getID() + 1, fixed_byte_length_);
      fixed_byte_length_ += attr_type.maximumByteLength();
    }
    max_byte_length_ += attr_type.maximumByteLength();
    min_byte_length_ += attr_type.minimumByteLength();
    estimated_byte_length_ += attr_type.estimateAverageByteLength();

    if (attr_type.isNullable()) {
      ++num_nullable_attributes_;
      nullable_attribute_indices_.push_back(++current_nullable_attribute_index_);
    } else {
      nullable_attribute_indices_.push_back(kInvalidCatalogId);
    }

    const attribute_id attr_id = new_attr->getID();
    // Ensure that we will not overrun the vector by resizing and initializing
    // new entries to zero.
    max_byte_lengths_.resize(attr_id + 1, 0);
    max_byte_lengths_[attr_id] = attr_type.maximumByteLength();

    return attr_id;
  }
}

attribute_id CatalogRelationSchema::getMaxAttributeId() const {
  if (size() > 0) {
    return attr_vec_.back().getID();
  } else {
    return -1;
  }
}

}  // namespace quickstep
