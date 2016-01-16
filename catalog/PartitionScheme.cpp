/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "catalog/PartitionScheme.hpp"

#include <cstddef>
#include <limits>
#include <unordered_set>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "types/TypedValue.pb.h"

#include "glog/logging.h"

namespace quickstep {

class Type;

PartitionScheme::PartitionScheme(const PartitionType type,
                                 const std::size_t num_partitions,
                                 const attribute_id attr_id)
    : num_partitions_(num_partitions),
      partition_attribute_id_(attr_id),
      partition_type_(type),
      blocks_in_partition_(num_partitions_),
      blocks_in_partition_mutexes_(num_partitions_) {
  DCHECK_GT(num_partitions, 0u);
  DCHECK_GE(attr_id, 0);
}

bool PartitionScheme::ProtoIsValid(
    const serialization::PartitionScheme &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }
  // Check if the number of partitions is non-zero.
  if (proto.num_partitions() <= 0) {
    return false;
  }
  // Check if the partitioning attribute exists in the relation.
  // TODO(gerald): Figure out how to find the max_attr_id of the
  // relation so that we can check that partitioning attribute
  // is not greater than the max_attr_id.

  // Check that the proto has a valid partition type.
  switch (proto.partition_type()) {
    case serialization::PartitionScheme_PartitionType_HASH_PARTITION:
      // Check if a hash partition scheme has a partition range boundary.
      // If so, then the proto is not valid since hash partition scheme
      // shouldn't contain range boundaries.
      if (proto.partition_range_boundary_size() > 0) {
        return false;
      }
      return true;
    case serialization::PartitionScheme_PartitionType_RANGE_PARTITION:
      if (static_cast<std::size_t>(proto.partition_range_boundary_size()) !=
          proto.num_partitions() - 1) {
        return false;
      }
      return true;
    default:
      // Partition type is unknown.
      return false;
  }
}

PartitionScheme* PartitionScheme::DeserializePartitionScheme(
    const serialization::PartitionScheme &proto, const Type &attr_type) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create PartitionScheme from an invalid proto description:\n"
      << proto.DebugString();

  PartitionScheme *partition_scheme;
  switch (proto.partition_type()) {
    // Deserialize a Hash Partition Scheme protobuf to a Hash Partition Scheme
    // C++ object.
    case serialization::PartitionScheme_PartitionType_HASH_PARTITION: {
      partition_scheme = new HashPartitionScheme(
          proto.num_partitions(), proto.partition_attribute_id());
      break;
    }
    // Deserialize a Range Partition Scheme protobuf to a Range Partition Scheme
    // C++ object.
    case serialization::PartitionScheme_PartitionType_RANGE_PARTITION: {
      std::vector<TypedValue> partition_range(proto.num_partitions() - 1);
      for (std::size_t partition_num = 0;
           partition_num <
           static_cast<std::size_t>(proto.partition_range_boundary_size());
           ++partition_num) {
        // Deserialize the partition range boundaries.
        partition_range[partition_num] = TypedValue::ReconstructFromProto(
            proto.partition_range_boundary(partition_num));
      }
      partition_scheme =
          new RangePartitionScheme(attr_type,
                                   proto.num_partitions(),
                                   proto.partition_attribute_id(),
                                   partition_range);
      break;
    }
    default:
      FATAL_ERROR("Invalid partition scheme.");
  }
  // Deserialize and add blocks to the corresponding partitions.
  for (std::size_t partition_num = 0;
       partition_num < static_cast<std::size_t>(proto.partitions_size());
       ++partition_num) {
    for (std::size_t block_num = 0;
         block_num < static_cast<std::size_t>(
                         proto.partitions(partition_num).blocks_size());
         ++block_num) {
      partition_scheme->addBlockToPartition(
          proto.partitions(partition_num).blocks(block_num), partition_num);
    }
  }
  return partition_scheme;
}

serialization::PartitionScheme PartitionScheme::getProto() const {
  serialization::PartitionScheme proto;
  proto.set_num_partitions(getNumPartitions());
  proto.set_partition_attribute_id(getPartitionAttributeId());

  // Set the partition type.
  switch (getPartitionType()) {
    case PartitionType::kHash:
      proto.set_partition_type(
          serialization::PartitionScheme_PartitionType_HASH_PARTITION);
      break;
    case PartitionType::kRange:
      proto.set_partition_type(
          serialization::PartitionScheme_PartitionType_RANGE_PARTITION);
      break;
    default:
      FATAL_ERROR("Invalid Partition Type.");
  }

  // Add blocks to the corresponding partitions.
  for (std::size_t partition_num = 0;
       partition_num < proto.num_partitions();
       ++partition_num) {
    proto.add_partitions();
    const std::vector<block_id> blocks_in_partition =
        getBlocksInPartition(partition_num);
    for (std::vector<block_id>::const_iterator blocks_in_partition_iter =
             blocks_in_partition.begin();
         blocks_in_partition_iter != blocks_in_partition.end();
         ++blocks_in_partition_iter) {
      proto.mutable_partitions(partition_num)
          ->add_blocks(*blocks_in_partition_iter);
    }
  }

  return proto;
}

serialization::PartitionScheme RangePartitionScheme::getProto() const {
  serialization::PartitionScheme proto = PartitionScheme::getProto();

  // Fill in range boundaries for Range Partition Scheme.
  for (std::size_t partition_num = 0;
       partition_num < getNumPartitions() - 1;
       ++partition_num) {
    proto.add_partition_range_boundary();
    proto.mutable_partition_range_boundary(partition_num)
        ->CopyFrom(partition_range_boundary_[partition_num].getProto());
  }

  return proto;
}

const partition_id PartitionScheme::getPartitionForBlock(
    block_id block) const {
  // Check if the block is present in the available partitions.
  // If so, return the partition id for the block.
  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    SpinSharedMutexSharedLock<false> lock(
        blocks_in_partition_mutexes_[part_id]);
    if (blocks_in_partition_[part_id].find(block) !=
        blocks_in_partition_[part_id].end()) {
      return part_id;
    }
  }
  // Block was not found in any partitions.
  return std::numeric_limits<std::size_t>::max();
}

}  // namespace quickstep
