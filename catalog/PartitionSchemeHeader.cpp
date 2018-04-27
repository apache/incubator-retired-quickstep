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

#include "catalog/PartitionSchemeHeader.hpp"

#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/TypedValue.pb.h"

#include "glog/logging.h"

using std::move;
using std::size_t;
using std::vector;

namespace quickstep {

namespace S = serialization;

PartitionSchemeHeader::PartitionSchemeHeader(const PartitionType type,
                                             const std::size_t num_partitions,
                                             PartitionAttributeIds &&attr_ids)  // NOLINT(whitespace/operators)
    : partition_type_(type),
      num_partitions_(num_partitions),
      partition_attribute_ids_(move(attr_ids)) {
  DCHECK_GT(num_partitions, 0u);

#ifdef QUICKSTEP_DEBUG
  // Ensure that no duplicated partition attributes exist.
  std::unordered_set<attribute_id> partition_attribute_ids;
  for (const attribute_id attr_id : partition_attribute_ids_) {
    DCHECK_NE(attr_id, kInvalidCatalogId);

    CHECK_EQ(0u, partition_attribute_ids.count(attr_id));
    partition_attribute_ids.insert(attr_id);
  }
#endif
}

bool PartitionSchemeHeader::ProtoIsValid(
    const serialization::PartitionSchemeHeader &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }
  // Check if the partitioning attribute exists in the relation.
  // TODO(gerald): Figure out how to find the max_attr_id of the
  // relation so that we can check that partitioning attribute
  // is not greater than the max_attr_id.

  // Check that the proto has a valid partition type.
  switch (proto.partition_type()) {
    case S::PartitionSchemeHeader::HASH: {
      for (int i = 0; i < proto.ExtensionSize(S::HashPartitionSchemeHeader::partition_attr_types); ++i) {
        if (!TypeFactory::ProtoIsValid(proto.GetExtension(S::HashPartitionSchemeHeader::partition_attr_types, i))) {
          return false;
        }
      }
      return proto.ExtensionSize(S::HashPartitionSchemeHeader::partition_attr_types) ==
                 proto.partition_attribute_ids_size();
    }
    case serialization::PartitionSchemeHeader::RANDOM:
      return true;
    case serialization::PartitionSchemeHeader::RANGE: {
      return proto.HasExtension(serialization::RangePartitionSchemeHeader::partition_range_boundaries) &&
             proto.ExtensionSize(serialization::RangePartitionSchemeHeader::partition_range_boundaries) ==
                 static_cast<int>(proto.num_partitions() - 1) &&
             proto.HasExtension(serialization::RangePartitionSchemeHeader::partition_attr_types) &&
             proto.ExtensionSize(serialization::RangePartitionSchemeHeader::partition_attr_types) ==
                 proto.partition_attribute_ids_size();
    }
    default:
      // Partition type is unknown.
      return false;
  }
}

PartitionSchemeHeader* PartitionSchemeHeader::ReconstructFromProto(
    const serialization::PartitionSchemeHeader &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create PartitionSchemeHeader from an invalid proto description:\n"
      << proto.DebugString();

  PartitionAttributeIds partition_attribute_ids;
  for (int i = 0; i < proto.partition_attribute_ids_size(); ++i) {
    partition_attribute_ids.push_back(proto.partition_attribute_ids(i));
  }

  switch (proto.partition_type()) {
    case serialization::PartitionSchemeHeader::HASH: {
      vector<TypeID> attr_types;
      vector<bool> attr_type_nullables;
      vector<size_t> char_type_lengths;
      for (int i = 0; i < proto.ExtensionSize(S::HashPartitionSchemeHeader::partition_attr_types); ++i) {
        const S::Type &type_proto = proto.GetExtension(S::HashPartitionSchemeHeader::partition_attr_types, i);
        attr_types.push_back(DeserializeTypeID(type_proto.type_id()));
        attr_type_nullables.push_back(type_proto.nullable());
        if (type_proto.type_id() == S::Type::CHAR) {
          char_type_lengths.push_back(type_proto.GetExtension(S::CharType::length));
        }
      }

      return new HashPartitionSchemeHeader(proto.num_partitions(), move(partition_attribute_ids),
                                           move(attr_types), move(attr_type_nullables), move(char_type_lengths));
    }
    case serialization::PartitionSchemeHeader::RANDOM: {
      return new RandomPartitionSchemeHeader(proto.num_partitions());
    }
    case serialization::PartitionSchemeHeader::RANGE: {
      std::vector<const Type*> attr_types;
      for (int i = 0; i < proto.ExtensionSize(serialization::RangePartitionSchemeHeader::partition_attr_types); ++i) {
        attr_types.push_back(&TypeFactory::ReconstructFromProto(
            proto.GetExtension(serialization::RangePartitionSchemeHeader::partition_attr_types, i)));
      }

      const int partition_ranges_size =
          proto.ExtensionSize(serialization::RangePartitionSchemeHeader::partition_range_boundaries);
      std::vector<PartitionValues> partition_ranges(partition_ranges_size);
      for (int i = 0; i < partition_ranges_size; ++i) {
        const auto &proto_partition_values =
            proto.GetExtension(serialization::RangePartitionSchemeHeader::partition_range_boundaries, i);
        for (int j = 0; j < proto_partition_values.partition_values_size(); ++j) {
          partition_ranges[i].emplace_back(TypedValue::ReconstructFromProto(
              proto_partition_values.partition_values(j)));
        }
      }

      return new RangePartitionSchemeHeader(proto.num_partitions(),
                                            move(partition_attribute_ids),
                                            move(attr_types),
                                            move(partition_ranges));
    }
    default:
      LOG(FATAL) << "Invalid partition scheme header.";
      // Avoid '-Werror=return-type' using GCC.
      return nullptr;
  }
}

serialization::PartitionSchemeHeader PartitionSchemeHeader::getProto() const {
  serialization::PartitionSchemeHeader proto;

  switch (partition_type_) {
    case PartitionType::kHash:
      proto.set_partition_type(serialization::PartitionSchemeHeader::HASH);
      break;
    case PartitionType::kRandom:
      proto.set_partition_type(serialization::PartitionSchemeHeader::RANDOM);
      break;
    case PartitionType::kRange:
      proto.set_partition_type(serialization::PartitionSchemeHeader::RANGE);
      break;
    default:
      LOG(FATAL) << "Invalid Partition Type.";
  }

  proto.set_num_partitions(num_partitions_);

  for (const attribute_id attr_id : partition_attribute_ids_) {
    proto.add_partition_attribute_ids(attr_id);
  }

  return proto;
}

std::string PartitionSchemeHeader::toString(const CatalogRelationSchema &relation_schema) const {
  std::ostringstream oss;
  oss << "PARTITION BY ";
  switch (partition_type_) {
    case PartitionType::kHash:
      oss << "HASH";
      break;
    case PartitionType::kRandom:
      oss << "RANDOM";
      break;
    case PartitionType::kRange:
      oss << "RANGE";
      break;
    default:
      LOG(FATAL) << "Invalid Partition Type.";
  }

  oss << " ( ";
  if (!partition_attribute_ids_.empty()) {
    const CatalogAttribute *attr = relation_schema.getAttributeById(partition_attribute_ids_[0]);
    DCHECK(attr);
    oss << attr->getName();

    for (size_t i = 1; i < partition_attribute_ids_.size(); ++i) {
      attr = relation_schema.getAttributeById(partition_attribute_ids_[i]);
      DCHECK(attr);
      oss << ", " << attr->getName();
    }
  }
  oss << " ) PARTITIONS " << num_partitions_ << '\n';

  return oss.str();
}

partition_id HashPartitionSchemeHeader::getPartitionId(
    const PartitionValues &value_of_attributes) const {
  DCHECK_EQ(partition_attribute_ids_.size(), value_of_attributes.size());
  if (is_power_of_two_) {
    return getPartitionIdImpl<true>(HashCompositeKey(value_of_attributes));
  } else {
    return getPartitionIdImpl<false>(HashCompositeKey(value_of_attributes));
  }
}

void HashPartitionSchemeHeader::setPartitionMembership(
    std::vector<std::unique_ptr<TupleIdSequence>> *partition_membership,
    ValueAccessor *accessor) const {
  CHECK_EQ(1u, partition_attribute_ids_.size())
      << "TODO(quickstep-team): support multi-attribute repartition";

  InvokeOnAnyValueAccessor(
      accessor,
      [this,
       &partition_membership](auto *accessor) -> void {
    const attribute_id attr_id = partition_attribute_ids_.front();

    accessor->beginIteration();
    switch (attr_type_ids_.front()) {
      case kInt:
      case kFloat:
        if (attr_type_nullables_.front()) {
          return this->setPartitionMembershipImpl<kInt, true>(accessor, attr_id, partition_membership);
        } else {
          return this->setPartitionMembershipImpl<kInt, false>(accessor, attr_id, partition_membership);
        }
      case kLong:
      case kDouble:
      case kDate:
      case kDatetime:
      case kDatetimeInterval:
      case kYearMonthInterval:
        if (attr_type_nullables_.front()) {
          return this->setPartitionMembershipImpl<kLong, true>(accessor, attr_id, partition_membership);
        } else {
          return this->setPartitionMembershipImpl<kLong, false>(accessor, attr_id, partition_membership);
        }
      case kChar: {
        const std::size_t length = char_type_lengths_.front();
        if (attr_type_nullables_.front()) {
          return this->setPartitionMembershipImpl<kChar, true>(accessor, attr_id, partition_membership, length);
        } else {
          return this->setPartitionMembershipImpl<kChar, false>(accessor, attr_id, partition_membership, length);
        }
      }
      case kVarChar:
        if (attr_type_nullables_.front()) {
          return this->setPartitionMembershipImpl<kVarChar, true>(accessor, attr_id, partition_membership);
        } else {
          return this->setPartitionMembershipImpl<kVarChar, false>(accessor, attr_id, partition_membership);
        }
      default:
        LOG(FATAL) << "Unsupported type.";
    }
  });
}

S::PartitionSchemeHeader HashPartitionSchemeHeader::getProto() const {
  S::PartitionSchemeHeader proto = PartitionSchemeHeader::getProto();

  size_t char_type_lengths_index = 0;
  for (size_t i = 0; i < attr_type_ids_.size(); ++i) {
    const TypeID type_id = attr_type_ids_[i];

    S::Type *type_proto =
        proto.AddExtension(S::HashPartitionSchemeHeader::partition_attr_types);
    type_proto->set_type_id(SerializeTypeID(type_id));
    type_proto->set_nullable(attr_type_nullables_[i]);

    switch (type_id) {
      case kChar:
        type_proto->SetExtension(S::CharType::length, char_type_lengths_[char_type_lengths_index++]);
        break;
      case kVarChar:
        type_proto->SetExtension(S::VarCharType::length, 0u);
        break;
      default:
        break;
    }
  }

  DCHECK_EQ(char_type_lengths_.size(), char_type_lengths_index);

  return proto;
}

serialization::PartitionSchemeHeader RangePartitionSchemeHeader::getProto() const {
  serialization::PartitionSchemeHeader proto = PartitionSchemeHeader::getProto();

  for (const Type *type : partition_attr_types_) {
    proto.AddExtension(serialization::RangePartitionSchemeHeader::partition_attr_types)
        ->MergeFrom(type->getProto());
  }

  for (const PartitionValues &partition_range_boundary : partition_range_boundaries_) {
    serialization::PartitionValues *proto_range_boundary =
        proto.AddExtension(serialization::RangePartitionSchemeHeader::partition_range_boundaries);

    for (const TypedValue &value : partition_range_boundary) {
      proto_range_boundary->add_partition_values()->MergeFrom(value.getProto());
    }
  }

  return proto;
}

}  // namespace quickstep
