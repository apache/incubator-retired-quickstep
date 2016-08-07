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

#ifndef QUICKSTEP_CATALOG_PARTITION_PLACEMENT_SCHEME_HPP_
#define QUICKSTEP_CATALOG_PARTITION_PLACEMENT_SCHEME_HPP_

#include <numa.h>

#include <cstddef>
#include <ostream>
#include <unordered_map>
#include <utility>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief A NUMAPlacementScheme object which stores the mapping between the
 * blocks in a partition and the NUMA nodes corresponding to those partitions.
 *
 * @note NUMA stands for Non-Uniform Memory Access.
 *
 * @note There is always only a one-on-one mapping between the partitions and
 * the NUMA nodes. This means that the blocks of a relation that belong to a
 * particular partition can be placed on ONLY one NUMA node. Earlier we had a
 * design where we can place the blocks in a single partition on multiple
 * NUMA nodes but we later decided that there wouldn't be any scenario where
 * we would want to place the blocks belonging to a single partition on more
 * than one NUMA node and hence that earlier design has been discarded.
 *
 * @note The terms node and socket would be used interchangeably in the
 * context of NUMA. A NUMA node or a NUMA socket would essentially mean
 * the same.
 **/
class NUMAPlacementScheme {
 public:
  /**
   * @brief Constructor.
   *
   * @param num_partitions The number of partitions of the Catalog Relation.
   *                       This would be the same as the number of partitions
   *                       in the Partition Scheme of the relation.
   **/
  explicit NUMAPlacementScheme(const std::size_t num_partitions)
      : num_numa_nodes_(numa_num_configured_nodes()),
        num_partitions_(num_partitions) {
    // Assign each partition to exactly one NUMA node.
    // Partitions are assigned in a round robin way to NUMA nodes.
    for (std::size_t part_id = 0;
         part_id < num_partitions_;
         ++part_id) {
      partition_to_numa_node_map_[part_id] = part_id % num_numa_nodes_;
    }
  }

  /**
   * @brief Destructor.
   **/
  ~NUMAPlacementScheme() {}

  /**
   * @brief Get the number of partitions.
   *
   * @return Number of partitions in the Catalog Relation.
   **/
  inline std::size_t getNumPartitions() const {
    return num_partitions_;
  }

  /**
   * @brief Get the total number of NUMA nodes available in the system.
   *
   * @return Total number of NUMA nodes available in the system.
   **/
  inline std::size_t getNumNUMANodes() const {
    return num_numa_nodes_;
  }

  /**
   * @brief Get the NUMA node on which a partition is placed.
   *
   * @param part_id The partition id for which to find the NUMA node.
   *
   * @return The NUMA node on which the partition is placed.
   **/
  inline numa_node_id getNUMANodeForPartition(const partition_id part_id)
      const {
    auto partition_to_numa_node_map_iter =
        partition_to_numa_node_map_.find(part_id);
    DCHECK(partition_to_numa_node_map_iter != partition_to_numa_node_map_.end());
    return partition_to_numa_node_map_iter->second;
  }

  /**
   * @brief Add a mapping between a block and the NUMA node on which the block
   *        is placed.
   *
   * @param block The block_id of the block for which the NUMA node mapping is
   *              added.
   * @param numa_node The numa node id on which the block is placed.
   **/
  void addBlockToNUMANodeMap(const block_id block, const int numa_node) {
    // Make sure that the block doesn't have a mapping already.
    // A block will be mapped to only one NUMA node.
    // A NUMA node will be associated with a block only once.
    DCHECK(block_to_numa_node_map_.find(block) == block_to_numa_node_map_.end());
    DCHECK_GT(numa_num_configured_nodes(), numa_node)
        << "NUMA node above the valid value.";
    block_to_numa_node_map_[block] = numa_node;
  }

  /**
   * @brief Get the NUMA node associated with a block.
   *
   * @param block The block_id of the block for which the associated NUMA node
   *              id is to be found.
   *
   * @return The NUMA node id associated with the block.
   **/
  inline numa_node_id getNUMANodeForBlock(const block_id block) const {
    auto block_to_numa_node_map_iter = block_to_numa_node_map_.find(block);
    DCHECK(block_to_numa_node_map_iter != block_to_numa_node_map_.end());
    return block_to_numa_node_map_iter->second;
  }

  /**
   * @brief Check whether a serialization::NUMAPlacementScheme is fully-formed
   *        and all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a
   *              NUMAPlacementScheme, originally generated by getProto().
   *
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::NUMAPlacementScheme &proto);

  /**
   * @brief Serialize the NUMA Placement Scheme as Protocol Buffer.
   *
   * @return The Protocol Buffer representation of NUMA Placement Scheme.
   **/
  serialization::NUMAPlacementScheme getProto() const;

  /**
   * @brief Reconstruct a NUMA Placement Scheme from its serialized
   *        Protocol Buffer form.
   *
   * @param proto The Protocol Buffer serialization of a NUMA Placement Scheme,
   *              previously produced by getProto().
   * @param partition_proto The Protocol Buffer serialization of a
   *                        ParitionScheme. This is needed for number of
   *                        partitions.
   *
   * @return The deserialized NUMA Placement Scheme object.
   **/
  static NUMAPlacementScheme* ReconstructFromProto(
      const serialization::NUMAPlacementScheme &proto,
      const std::size_t num_partitions);

 private:
  // Number of NUMA nodes available in the system.
  const std::size_t num_numa_nodes_;

  // Number of partitions in the Catalog Relation.
  const std::size_t num_partitions_;

  // A map between the partition ids and NUMA node ids.
  // All the blocks that belong to a particular partition are placed on only
  // one NUMA node.
  std::unordered_map<partition_id, numa_node_id> partition_to_numa_node_map_;

  // A map between the block ids and the NUMA node ids.
  std::unordered_map<block_id, numa_node_id> block_to_numa_node_map_;

  DISALLOW_COPY_AND_ASSIGN(NUMAPlacementScheme);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_PARTITION_PLACEMENT_SCHEME_HPP_
