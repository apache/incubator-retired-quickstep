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

#include "catalog/PartitionScheme.hpp"

#include <cstddef>
#include <limits>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/PartitionSchemeHeader.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "glog/logging.h"

using std::move;
using std::unordered_set;
using std::vector;

namespace quickstep {

bool PartitionScheme::ProtoIsValid(
    const serialization::PartitionScheme &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  if (!PartitionSchemeHeader::ProtoIsValid(proto.header())) {
    return false;
  }

  if (static_cast<std::size_t>(proto.partitions_size()) != proto.header().num_partitions()) {
    return false;
  }

  return true;
}

PartitionScheme* PartitionScheme::ReconstructFromProto(const serialization::PartitionScheme &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create PartitionScheme from an invalid proto description:\n"
      << proto.DebugString();

  vector<unordered_set<block_id>> blocks_in_partition;
  for (int i = 0; i < proto.partitions_size(); ++i) {
    unordered_set<block_id> blocks;

    const serialization::Partition &proto_blocks = proto.partitions(i);
    for (int j = 0; j < proto_blocks.blocks_size(); ++j) {
      blocks.insert(proto_blocks.blocks(j));
    }

    blocks_in_partition.push_back(move(blocks));
  }

  return new PartitionScheme(
      PartitionSchemeHeader::ReconstructFromProto(proto.header()),
      move(blocks_in_partition));
}

serialization::PartitionScheme PartitionScheme::getProto() const {
  serialization::PartitionScheme proto;

  proto.mutable_header()->MergeFrom(header_->getProto());

  // Add blocks to the corresponding partitions.
  for (std::size_t i = 0; i < blocks_in_partition_.size(); ++i) {
    serialization::Partition *proto_blocks = proto.add_partitions();

    SpinSharedMutexSharedLock<false> lock(blocks_in_partition_mutexes_[i]);
    const std::unordered_set<block_id> &partition = blocks_in_partition_[i];
    for (const block_id block : partition) {
      proto_blocks->add_blocks(block);
    }
  }

  return proto;
}

partition_id PartitionScheme::getPartitionForBlock(const block_id block) const {
  // Check if the block is present in the available partitions.
  // If so, return the partition id for the block.
  for (partition_id part_id = 0; part_id < header_->getNumPartitions(); ++part_id) {
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
