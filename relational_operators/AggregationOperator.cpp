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

#include "relational_operators/AggregationOperator.hpp"

#include <vector>

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/AggregationOperationState.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool AggregationOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    CatalogDatabase *catalog_database,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addNormalWorkOrder(
            new AggregationWorkOrder(input_block_id,
                                     query_context->getAggregationState(aggr_state_index_)),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new AggregationWorkOrder(input_relation_block_ids_[num_workorders_generated_],
                                   query_context->getAggregationState(aggr_state_index_)),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

void AggregationWorkOrder::execute() {
  state_->aggregateBlock(input_block_id_);
}

}  // namespace quickstep
