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
