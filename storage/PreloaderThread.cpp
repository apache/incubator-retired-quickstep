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

#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadUtil.hpp"

namespace quickstep {

void PreloaderThread::run() {
  if (cpu_id_ >= 0) {
    ThreadUtil::BindToCPU(cpu_id_);
  }

  const std::size_t numBufferPoolSlots = storage_manager_->getMaxBufferPoolSlots();
  std::size_t blocksLoaded = 0;

  for (const CatalogRelation &relation : database_) {
    std::vector<block_id> blocks = relation.getBlocksSnapshot();
    for (block_id current_block_id : blocks) {
      try {
        BlockReference current_block = storage_manager_->getBlock(current_block_id, relation);
      } catch (...) {
        LOG(INFO) << "Error after loading " << blocksLoaded << "blocks\n";
        throw;
      }
      blocksLoaded++;
      if (blocksLoaded == numBufferPoolSlots) {
        // The buffer pool has filled up. But, some database blocks are not loaded.
        printf(" The database is larger than the buffer pool. Only %lu blocks were loaded ",
               blocksLoaded);
        return;
      }
    }
  }
  printf(" Loaded %lu blocks ", blocksLoaded);
}

}  // namespace quickstep
