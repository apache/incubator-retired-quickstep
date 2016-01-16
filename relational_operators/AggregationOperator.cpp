/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/AggregationOperator.hpp"

#include <vector>

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/AggregationOperationState.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "glog/logging.h"

namespace quickstep {

bool AggregationOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addNormalWorkOrder(
            new AggregationWorkOrder(input_block_id, aggr_state_index_),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new AggregationWorkOrder(input_relation_block_ids_[num_workorders_generated_], aggr_state_index_),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

void AggregationWorkOrder::execute(QueryContext *query_context,
                                   CatalogDatabase *catalog_database,
                                   StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  AggregationOperationState *state = query_context->getAggregationState(aggr_state_index_);
  DCHECK(state != nullptr);

  state->aggregateBlock(input_block_id_);
}

}  // namespace quickstep
