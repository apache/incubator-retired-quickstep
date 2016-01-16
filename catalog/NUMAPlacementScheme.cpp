/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "catalog/NUMAPlacementScheme.hpp"

#include <unordered_map>
#include <utility>

#include "catalog/Catalog.pb.h"
#include "catalog/PartitionScheme.hpp"

#include "glog/logging.h"

namespace quickstep {

bool NUMAPlacementScheme::ProtoIsValid(
    const serialization::NUMAPlacementScheme &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }
  // Check if the number of numa nodes is non-zero.
  if (proto.num_numa_nodes() <= 0) {
    return false;
  }
  // Protobuf for NUMAPlacementScheme is valid.
  return true;
}

serialization::NUMAPlacementScheme NUMAPlacementScheme::getProto() const {
  serialization::NUMAPlacementScheme proto;

  // Serialize the number of numa nodes.
  proto.set_num_numa_nodes(getNumNUMANodes());

  // Serialize the block to NUMA node map.
  for (std::unordered_map<block_id, int>::const_iterator block_to_node_iter =
           block_to_numa_node_map_.begin();
       block_to_node_iter != block_to_numa_node_map_.end();
       ++block_to_node_iter) {
    serialization::NUMAPlacementScheme_BlockToNUMANodeEntry *entry =
        proto.add_block_to_numa_node_map();
    entry->set_block_id(block_to_node_iter->first);
    entry->set_numa_node(block_to_node_iter->second);
  }

  return proto;
}

NUMAPlacementScheme* NUMAPlacementScheme::DeserializeNUMAPlacementScheme(
    const serialization::NUMAPlacementScheme &proto,
    const serialization::PartitionScheme &partition_proto) {
  DCHECK(PartitionScheme::ProtoIsValid(partition_proto))
      << "Attempted to create NUMAPlacementScheme from an invalid partition proto description:\n"
      << proto.DebugString();

  NUMAPlacementScheme *placement_scheme;
  // This call to the constructor will populate the values of partition-to-NUMA
  // node map.
  placement_scheme = new NUMAPlacementScheme(partition_proto.num_partitions());

  DCHECK(ProtoIsValid(proto))
      << "Attempted to create NUMAPlacementScheme from an invalid proto description:\n"
      << proto.DebugString();

  // Deserialize block to NUMA node mapping.
  for (int i = 0; i < proto.block_to_numa_node_map_size(); ++i) {
    placement_scheme->addBlockToNUMANodeMap(
        proto.block_to_numa_node_map(i).block_id(),
        proto.block_to_numa_node_map(i).numa_node());
  }
  return placement_scheme;
}

}  // namespace quickstep
