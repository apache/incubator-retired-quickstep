/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/DeleteOperator.hpp"

#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"

#include "glog/logging.h"

namespace quickstep {

bool DeleteOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (relation_is_stored_) {
    // If relation_ is stored, iterate over the list of blocks in relation_.
    if (!started_) {
      for (const block_id input_block_id : relation_block_ids_) {
        container->addNormalWorkOrder(
            new DeleteWorkOrder(relation_.getID(),
                                predicate_index_,
                                input_block_id,
                                op_index_,
                                foreman_client_id_,
                                bus_),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new DeleteWorkOrder(relation_.getID(),
                              predicate_index_,
                              relation_block_ids_[num_workorders_generated_],
                              op_index_,
                              foreman_client_id_,
                              bus_),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

void DeleteWorkOrder::execute(QueryContext *query_context,
                              CatalogDatabase *database,
                              StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  MutableBlockReference block(
      storage_manager->getBlockMutable(input_block_id_,
                                       *database->getRelationById(rel_id_)));
  block->deleteTuples(query_context->getPredicate(predicate_index_));

  // TODO(harshad): Stream the block ID only if the predicate returned at least
  // one match in the StorageBlock.
  ForemanMessage message(ForemanMessage::DataPipelineMessage(delete_operator_index_,
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
