/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/FinalizeAggregationOperator.hpp"

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/AggregationOperationState.hpp"

#include "glog/logging.h"

namespace quickstep {

class InsertDestination;

bool FinalizeAggregationOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (blocking_dependencies_met_ && !started_) {
    started_ = true;
    container->addNormalWorkOrder(
        new FinalizeAggregationWorkOrder(aggr_state_index_, output_destination_index_),
        op_index_);
  }
  return started_;
}

void FinalizeAggregationWorkOrder::execute(
    QueryContext *query_context,
    CatalogDatabase *catalog_database,
    StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  AggregationOperationState *state = query_context->getAggregationState(aggr_state_index_);
  DCHECK(state != nullptr);

  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);
  DCHECK(output_destination != nullptr);

  state->finalizeAggregate(output_destination);

  // Now that the final results are materialized, destroy the
  // AggregationOperationState to free up memory ASAP.
  query_context->destroyAggregationState(aggr_state_index_);
}

}  // namespace quickstep
