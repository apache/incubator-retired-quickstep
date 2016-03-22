/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include <numa.h>

#include <cstddef>
#include <memory>

#include "catalog/NUMAPlacementScheme.hpp"
#include "catalog/PartitionScheme.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "gtest/gtest.h"

namespace quickstep {

// Test to check if the number of NUMA nodes in the system
// is the same as the number of NUMA nodes as reported by
// the NUMAPlacementScheme.
TEST(NUMAPlacementSchemeTest, CheckNumNUMANodes) {
  // Number of partitions in the Catalog Relation.
  std::size_t num_partitions = 4;

  // Create a NUMAPlacementScheme object with the num_partitions.
  std::unique_ptr<NUMAPlacementScheme> placement_scheme(
      new NUMAPlacementScheme(num_partitions));

  // Get the number of NUMA nodes in the system.
  const std::size_t num_numa_nodes = numa_num_configured_nodes();

  // Check if the number of NUMA nodes in NUMAPlacementScheme and that returned
  // from the system are the same.
  EXPECT_EQ(num_numa_nodes, placement_scheme->getNumNUMANodes());
}

// Test to check if the partitions are assigned the available NUMA nodes in a
// round-robin manner.
TEST(NUMAPlacementSchemeTest, CheckNUMANodesAssignment) {
  // Number of partitions in the Catalog Relation.
  std::size_t num_partitions = 64;

  // Create a NUMAPlacementScheme object with the num_partitions.
  std::unique_ptr<NUMAPlacementScheme> placement_scheme(
      new NUMAPlacementScheme(num_partitions));

  // Check that the partitions are assigned to the NUMA nodes in a
  // round-robbin manner. Partition 0 should be assigned to NUMA node 0,
  // partition 1 should be assigned to NUMA node 1, and so on. Partition n
  // should be assigned to NUMA node n modulus number_of_NUMA_nodes.
  for (std::size_t part_num = 0;
       part_num < num_partitions;
       ++part_num) {
    EXPECT_EQ(static_cast<int>(part_num % placement_scheme->getNumNUMANodes()),
              placement_scheme->getNUMANodeForPartition(part_num));
  }
}

// Test the serialization and deserialization of NUMAPlacementScheme object.
TEST(NUMAPlacementSchemeTest, NUMAPlacementSchemeSerializationTest) {
  // Number of blocks in the Catalog Relation.
  constexpr int kNumBlocks = 10;

  // Number of partitions in the Catalog Relation.
  std::size_t num_partitions = 64;

  // Create a HashPartitionScheme object with 64 partitions and attribute 0 as
  // the partitioning attribute.
  std::unique_ptr<PartitionScheme> partition_scheme(
      new HashPartitionScheme(num_partitions, 0));

  // Create a NUMAPlacementScheme object with the num_partitions.
  std::unique_ptr<NUMAPlacementScheme> placement_scheme(
      new NUMAPlacementScheme(num_partitions));

  // Add some entries into block to NUMA node map.
  for (block_id block = 0; block < kNumBlocks; ++block) {
    placement_scheme->addBlockToNUMANodeMap(
        block, block % placement_scheme->getNumNUMANodes());
  }

  // Check if the BlockToNUMANodeMap has exactly the same entries that we just
  // inserted..
  for (block_id block = 0; block < kNumBlocks; ++block) {
    EXPECT_EQ(static_cast<int>(block % placement_scheme->getNumNUMANodes()),
              placement_scheme->getNUMANodeForBlock(block));
  }

  // Create a new placement scheme object from the protobuf version of the
  // previously created placement scheme object. Note that serialization is done
  // using getProto() and deserialization is done using
  // ReconstructFromProto() methods, respectively.
  std::unique_ptr<NUMAPlacementScheme> placement_scheme_from_proto;
  placement_scheme_from_proto.reset(
      NUMAPlacementScheme::ReconstructFromProto(
          placement_scheme->getProto(), partition_scheme->getNumPartitions()));

  // Check if the number of NUMA nodes is the same in both the placement scheme
  // objects.
  EXPECT_EQ(placement_scheme->getNumNUMANodes(),
            placement_scheme_from_proto->getNumNUMANodes());

  // Check if the mapping between the partitions to the NUMA nodes is the same
  // in both the placement scheme objects.
  for (std::size_t part_id = 0; part_id < num_partitions; ++part_id) {
    EXPECT_EQ(placement_scheme->getNUMANodeForPartition(part_id),
              placement_scheme_from_proto->getNUMANodeForPartition(part_id));
  }

  // Check if the entries in the block to NUMA node map are the same in both the
  // placement scheme objects.
  for (block_id block = 0; block < kNumBlocks; ++block) {
    EXPECT_EQ(placement_scheme->getNUMANodeForBlock(block),
              placement_scheme_from_proto->getNUMANodeForBlock(block));
  }
}

}  // namespace quickstep
