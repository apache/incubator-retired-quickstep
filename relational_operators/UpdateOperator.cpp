/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/UpdateOperator.hpp"

#include <ostream>
#include <utility>

#include "catalog/CatalogDatabase.hpp"
#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

bool UpdateOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (blocking_dependencies_met_ && !started_) {
    for (const block_id input_block_id : input_blocks_) {
      container->addNormalWorkOrder(
          new UpdateWorkOrder(relation_.getID(),
                              relocation_destination_index_,
                              predicate_index_,
                              update_group_index_,
                              input_block_id,
                              op_index_,
                              foreman_client_id_,
                              bus_),
          op_index_);
    }
    started_ = true;
  }
  return started_;
}

void UpdateWorkOrder::execute(QueryContext *query_context,
                              CatalogDatabase *database,
                              StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  MutableBlockReference block(
      storage_manager->getBlockMutable(input_block_id_,
                                       *database->getRelationById(rel_id_)));

  InsertDestination *relocation_destination =
      query_context->getInsertDestination(relocation_destination_index_);
  DCHECK(relocation_destination != nullptr);

  StorageBlock::UpdateResult result =
      block->update(query_context->getUpdateGroup(update_group_index_),
                    query_context->getPredicate(predicate_index_),
                    relocation_destination);

  if (!result.indices_consistent) {
    LOG_WARNING("An UPDATE caused one or more IndexSubBlocks in StorageBlock "
                << BlockIdUtil::ToString(input_block_id_) << " to become inconsistent "
                << "(not enough space to index all tuples in block).");
  }

  // TODO(harshad): Stream the block ID only if the predicate returned at least
  // one match in the StorageBlock.
  ForemanMessage message(ForemanMessage::DataPipelineMessage(update_operator_index_,
                                                             input_block_id_,
                                                             rel_id_));
  TaggedMessage foreman_tagged_msg;
  foreman_tagged_msg.set_message(
      &message, sizeof(message), kDataPipelineMessage);

  QueryExecutionUtil::SendTMBMessage(
      bus_,
      ClientIDMap::Instance()->getValue(),
      foreman_client_id_,
      std::move(foreman_tagged_msg));
}

}  // namespace quickstep
