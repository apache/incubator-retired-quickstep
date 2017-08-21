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

#include "catalog/CatalogTypedefs.hpp"
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
  if (work_generated_) {
    return true;
  }

  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    container->addNormalWorkOrder(
        new DestroyAggregationStateWorkOrder(query_id_, aggr_state_index_, part_id, query_context),
        op_index_);
  }
  work_generated_ = true;
  return true;
}

bool DestroyAggregationStateOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (work_generated_) {
    return true;
  }

  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    serialization::WorkOrder *proto = new serialization::WorkOrder;
    proto->set_work_order_type(serialization::DESTROY_AGGREGATION_STATE);
    proto->set_query_id(query_id_);
    proto->SetExtension(serialization::DestroyAggregationStateWorkOrder::aggr_state_index, aggr_state_index_);
    proto->SetExtension(serialization::DestroyAggregationStateWorkOrder::partition_id, part_id);

    container->addWorkOrderProto(proto, op_index_);
  }
  work_generated_ = true;
  return true;
}

void DestroyAggregationStateWorkOrder::execute() {
  query_context_->destroyAggregationState(aggr_state_index_, part_id_);
}

}  // namespace quickstep
