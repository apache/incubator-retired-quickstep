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

#include "relational_operators/DestroyAggregationStateOperator.hpp"

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool DestroyAggregationStateOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;
    container->addNormalWorkOrder(
        new DestroyAggregationStateWorkOrder(query_id_, aggr_state_index_, query_context),
        op_index_);
  }
  return work_generated_;
}

bool DestroyAggregationStateOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;

    serialization::WorkOrder *proto = new serialization::WorkOrder;
    proto->set_work_order_type(serialization::DESTROY_AGGREGATION_STATE);
    proto->set_query_id(query_id_);
    proto->SetExtension(serialization::DestroyAggregationStateWorkOrder::aggr_state_index, aggr_state_index_);

    container->addWorkOrderProto(proto, op_index_);
  }
  return work_generated_;
}

void DestroyAggregationStateWorkOrder::execute() {
  // NOTE(harshad) : The destroyAggregationHashTablePayload call is separate
  // from the destroyAggregationState call. The reason is that the aggregation
  // hash tables don't own the AggregationHandle objects. However the hash table
  // class requires the handles for destroying the payload (see the
  // destroyPayload methods in AggregationHandle classes). Therefore, we first
  // destroy the payloads in the hash table and then destroy the hash table.
  query_context_->destroyAggregationHashTablePayload(aggr_state_index_);
  query_context_->destroyAggregationState(aggr_state_index_);
}

}  // namespace quickstep
