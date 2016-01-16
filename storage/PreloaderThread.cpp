/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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

  for (const CatalogRelation &relation : database_) {
    std::vector<block_id> blocks = relation.getBlocksSnapshot();
    for (block_id current_block_id : blocks) {
      BlockReference current_block = storage_manager_->getBlock(current_block_id, relation);
    }
  }
}

}  // namespace quickstep
