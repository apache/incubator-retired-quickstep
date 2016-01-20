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

#include "relational_operators/DropTableOperator.hpp"

#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"

#include "glog/logging.h"

namespace quickstep {

bool DropTableOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;
    container->addNormalWorkOrder(
        new DropTableWorkOrder(rel_id_, only_drop_blocks_),
        op_index_);
  }
  return work_generated_;
}

void DropTableWorkOrder::execute(QueryContext *query_context,
                                 CatalogDatabase *database,
                                 StorageManager *storage_manager) {
  DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  CatalogRelation *relation = database->getRelationByIdMutable(rel_id_);
  DCHECK(relation != nullptr);

  std::vector<block_id> relation_blocks(relation->getBlocksSnapshot());

  for (const block_id relation_block_id : relation_blocks) {
    storage_manager->deleteBlockOrBlobFile(relation_block_id);
  }

  if (only_drop_blocks_) {
    relation->clearBlocks();
  } else {
    database->dropRelationById(rel_id_);
  }
}

}  // namespace quickstep
