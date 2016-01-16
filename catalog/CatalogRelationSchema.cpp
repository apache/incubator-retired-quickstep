/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "catalog/CatalogRelationSchema.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogErrors.hpp"
#include "types/Type.hpp"
#include "utility/PtrVector.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

using std::size_t;
using std::string;
using std::unordered_map;

namespace quickstep {

CatalogRelationSchema::~CatalogRelationSchema() {
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
      variable_length_attribute_indices_.push_back(-1);

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
      nullable_attribute_indices_.push_back(-1);
    }

    return new_attr->getID();
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
