/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include "tmb/id_typedefs.h"

namespace quickstep {

class InsertDestination;

bool FinalizeAggregationOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    CatalogDatabase *catalog_database,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  if (blocking_dependencies_met_ && !started_) {
    started_ = true;
    container->addNormalWorkOrder(
        new FinalizeAggregationWorkOrder(aggr_state_index_,
                                         query_context->getInsertDestination(output_destination_index_),
                                         query_context),
        op_index_);
  }
  return started_;
}

void FinalizeAggregationWorkOrder::execute(
    QueryContext *query_context,
    CatalogDatabase *catalog_database,
    StorageManager *storage_manager) {
  AggregationOperationState *state = query_context_->getAggregationState(aggr_state_index_);
  DCHECK(state != nullptr);

  state->finalizeAggregate(output_destination_);

  // Now that the final results are materialized, destroy the
  // AggregationOperationState to free up memory ASAP.
  query_context_->destroyAggregationState(aggr_state_index_);
}

}  // namespace quickstep
