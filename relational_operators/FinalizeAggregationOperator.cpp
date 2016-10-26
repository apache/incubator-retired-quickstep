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

#include "relational_operators/FinalizeAggregationOperator.hpp"

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/AggregationOperationState.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool FinalizeAggregationOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  if (blocking_dependencies_met_ && !started_) {
    started_ = true;
    AggregationOperationState *agg_state =
        query_context->getAggregationState(aggr_state_index_);
    DCHECK(agg_state != nullptr);
    for (int part_id = 0;
         part_id < static_cast<int>(agg_state->getNumPartitions());
         ++part_id) {
      container->addNormalWorkOrder(
          new FinalizeAggregationWorkOrder(
              query_id_,
              agg_state,
              query_context->getInsertDestination(output_destination_index_),
              part_id),
          op_index_);
    }
  }
  return started_;
}

// TODO(quickstep-team) : Think about how the number of partitions could be
// accessed in this function. Until then, we can't use partitioned aggregation
// with the distributed version.
bool FinalizeAggregationOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (blocking_dependencies_met_ && !started_) {
    started_ = true;

    serialization::WorkOrder *proto = new serialization::WorkOrder;
    proto->set_work_order_type(serialization::FINALIZE_AGGREGATION);
    proto->set_query_id(query_id_);
    proto->SetExtension(serialization::FinalizeAggregationWorkOrder::aggr_state_index,
                        aggr_state_index_);
    proto->SetExtension(serialization::FinalizeAggregationWorkOrder::insert_destination_index,
                        output_destination_index_);

    container->addWorkOrderProto(proto, op_index_);
  }
  return started_;
}

void FinalizeAggregationWorkOrder::execute() {
  if (state_->isAggregatePartitioned()) {
    state_->finalizeAggregatePartitioned(part_id_, output_destination_);
  } else {
    state_->finalizeAggregate(output_destination_);
  }
}

}  // namespace quickstep
