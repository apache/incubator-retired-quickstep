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

#include "relational_operators/DropTableOperator.hpp"

#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogDatabaseLite.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool DropTableOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;

    std::vector<block_id> relation_blocks(relation_.getBlocksSnapshot());

    // DropTableWorkOrder only drops blocks, if any.
    container->addNormalWorkOrder(
        new DropTableWorkOrder(
            query_id_, std::move(relation_blocks), storage_manager),
        op_index_);

    database_->setStatus(CatalogDatabase::Status::kPendingBlockDeletions);
  }

  return work_generated_;
}

bool DropTableOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;

    serialization::WorkOrder *proto = new serialization::WorkOrder;
    proto->set_work_order_type(serialization::DROP_TABLE);
    proto->set_query_id(query_id_);

    std::vector<block_id> relation_blocks(relation_.getBlocksSnapshot());
    for (const block_id relation_block : relation_blocks) {
      proto->AddExtension(serialization::DropTableWorkOrder::block_ids, relation_block);
    }

    container->addWorkOrderProto(proto, op_index_);

    database_->setStatus(CatalogDatabase::Status::kPendingBlockDeletions);
  }

  return work_generated_;
}

void DropTableOperator::actionOnCompletion() {
  const relation_id rel_id = relation_.getID();
  if (only_drop_blocks_) {
    database_->getRelationByIdMutable(rel_id)->clearBlocks();
  } else {
    database_->dropRelationById(rel_id);
  }

  database_->setStatus(CatalogDatabase::Status::kConsistent);
}


void DropTableWorkOrder::execute() {
  for (const block_id block : blocks_) {
    storage_manager_->deleteBlockOrBlobFile(block);
  }

  // Drop the relation in the cache in the distributed version, if any.
  if (catalog_database_cache_ != nullptr && rel_id_ != kInvalidCatalogId) {
    catalog_database_cache_->dropRelationById(rel_id_);
  }
}

}  // namespace quickstep
