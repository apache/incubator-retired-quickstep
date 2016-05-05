/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "storage/PreloaderThread.hpp"

#include <cstddef>
#include <vector>

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include <unordered_map>
#endif

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConfig.h"
#include "storage/StorageManager.hpp"
#include "threading/ThreadUtil.hpp"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include "catalog/NUMAPlacementScheme.hpp"
#endif

#include "glog/logging.h"

namespace quickstep {

void PreloaderThread::run() {
  if (cpu_id_ >= 0) {
    ThreadUtil::BindToCPU(cpu_id_);
  }

  const std::size_t num_slots = storage_manager_->getMaxBufferPoolSlots();
  std::size_t blocks_loaded = 0;

  for (const CatalogRelation &relation : database_) {
    if (relation.hasPartitionScheme()) {
      blocks_loaded += preloadNUMAAware(relation, blocks_loaded, num_slots);
    } else {
      std::vector<block_id> blocks = relation.getBlocksSnapshot();
      for (block_id current_block_id : blocks) {
        try {
          BlockReference current_block =
              storage_manager_->getBlock(current_block_id, relation);
        } catch (...) {
          LOG(ERROR) << "Error after loading " << blocks_loaded << "blocks\n";
          throw;
        }
        ++blocks_loaded;
        if (blocks_loaded == num_slots) {
          // The buffer pool has filled up. But, some database blocks are not
          // loaded.
          printf(
              " The database is larger than the buffer pool. Only %lu blocks "
              "were loaded", blocks_loaded);
          return;
        }
      }
      LOG(INFO) << "Relation " << relation.getName()
                << " completely preloaded in buffer pool\n";
    }
  }
  printf("Loaded %lu blocks", blocks_loaded);
}

std::size_t PreloaderThread::preloadNUMAAware(
    const CatalogRelation &relation,
    const std::size_t num_previously_loaded_blocks,
    const std::size_t num_slots) {
#ifdef QUICKSTEP_HAVE_LIBNUMA
  std::size_t blocks_loaded = 0;
  const NUMAPlacementScheme *placement_scheme =
      relation.getNUMAPlacementSchemePtr();
  DCHECK(placement_scheme != nullptr);
  DCHECK(relation.hasPartitionScheme());
  const PartitionScheme &part_scheme = relation.getPartitionScheme();
  const PartitionSchemeHeader &part_scheme_header =
      part_scheme.getPartitionSchemeHeader();
  const std::size_t num_partitions = part_scheme_header.getNumPartitions();
  // Key = NUMA node ID, value = number of blocks loaded from that NUMA node.
  std::unordered_map<numa_node_id, std::size_t> num_blocks_loaded;
  for (std::size_t part_id = 0; part_id < num_partitions; ++part_id) {
    const numa_node_id partition_numa_node_id =
        placement_scheme->getNUMANodeForPartition(part_id);
    for (block_id curr_block_id : part_scheme.getBlocksInPartition(part_id)) {
      try {
        BlockReference current_block = storage_manager_->getBlock(
            curr_block_id, relation, partition_numa_node_id);
      } catch (...) {
        LOG(ERROR) << "Error while preloading: " << " After loading total "
                   << blocks_loaded + num_previously_loaded_blocks
                   << " blocks and " << blocks_loaded
                   << " blocks of relation " << relation.getName() << "\n";
        throw;
      }
      ++blocks_loaded;
      num_blocks_loaded[partition_numa_node_id]++;
      if ((blocks_loaded + num_previously_loaded_blocks) == num_slots) {
        // The buffer pool has filled up. But, some database blocks are not
        // loaded.
        printf(
            " The database is larger than the buffer pool. Only %lu blocks "
            "were loaded ",
            blocks_loaded + num_previously_loaded_blocks);
        return blocks_loaded;
      }
    }
  }
  LOG(INFO) << "For relation: " << relation.getName();
  for (auto numa_block_loaded_info : num_blocks_loaded) {
    LOG(INFO) << "NUMA node: " << numa_block_loaded_info.first
              << " Number of loaded blocks: "
              << numa_block_loaded_info.second;
  }
  LOG(INFO) << "Relation " << relation.getName()
            << " completely preloaded in buffer pool in a NUMA aware fashion";
  return blocks_loaded;
#else
  LOG(INFO) << "Relation: " << relation.getName()
            << " has partition scheme but the system doesn't support NUMA";
  return 0;
#endif
}

}  // namespace quickstep
