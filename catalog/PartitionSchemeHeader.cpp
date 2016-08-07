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
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "types/TypedValue.hpp"
#include "types/TypedValue.pb.h"

#include "glog/logging.h"

namespace quickstep {

PartitionSchemeHeader::PartitionSchemeHeader(const PartitionType type,
                                             const std::size_t num_partitions,
                                             const attribute_id attr_id)
    : partition_type_(type),
      num_partitions_(num_partitions),
      partition_attribute_id_(attr_id) {
  DCHECK_GT(num_partitions, 0u);
  DCHECK_GE(attr_id, 0);
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
    case serialization::PartitionSchemeHeader::HASH:
      return true;
    case serialization::PartitionSchemeHeader::RANGE: {
      const std::size_t num_ranges =
          proto.ExtensionSize(serialization::RangePartitionSchemeHeader::partition_range_boundaries);
      return num_ranges == proto.num_partitions() - 1;
    }
    default:
      // Partition type is unknown.
      return false;
  }
}

PartitionSchemeHeader* PartitionSchemeHeader::ReconstructFromProto(
    const serialization::PartitionSchemeHeader &proto, const Type &attr_type) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create PartitionSchemeHeader from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.partition_type()) {
    case serialization::PartitionSchemeHeader::HASH: {
      return new HashPartitionSchemeHeader(proto.num_partitions(), proto.partition_attribute_id());
    }
    case serialization::PartitionSchemeHeader::RANGE: {
      std::vector<TypedValue> partition_ranges;
      for (int i = 0;
           i < proto.ExtensionSize(serialization::RangePartitionSchemeHeader::partition_range_boundaries);
           ++i) {
        partition_ranges.emplace_back(
            TypedValue::ReconstructFromProto(
                proto.GetExtension(serialization::RangePartitionSchemeHeader::partition_range_boundaries, i)));
      }

      return new RangePartitionSchemeHeader(attr_type,
                                            proto.num_partitions(),
                                            proto.partition_attribute_id(),
                                            std::move(partition_ranges));
    }
    default:
      LOG(FATAL) << "Invalid partition scheme header.";
  }
}

serialization::PartitionSchemeHeader PartitionSchemeHeader::getProto() const {
  serialization::PartitionSchemeHeader proto;

  switch (partition_type_) {
    case PartitionType::kHash:
      proto.set_partition_type(serialization::PartitionSchemeHeader::HASH);
      break;
    case PartitionType::kRange:
      LOG(FATAL) << "RangePartitionSchemeHeader should call the overridden method.";
    default:
      LOG(FATAL) << "Invalid Partition Type.";
  }

  proto.set_num_partitions(num_partitions_);
  proto.set_partition_attribute_id(partition_attribute_id_);

  return proto;
}

serialization::PartitionSchemeHeader RangePartitionSchemeHeader::getProto() const {
  serialization::PartitionSchemeHeader proto;

  proto.set_partition_type(serialization::PartitionSchemeHeader::RANGE);
  proto.set_num_partitions(num_partitions_);
  proto.set_partition_attribute_id(partition_attribute_id_);

  for (std::size_t i = 0; i < partition_range_boundaries_.size(); ++i) {
    proto.AddExtension(serialization::RangePartitionSchemeHeader::partition_range_boundaries)
        ->MergeFrom(partition_range_boundaries_[i].getProto());
  }

  return proto;
}

}  // namespace quickstep
