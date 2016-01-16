/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
