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

#include "relational_operators/WindowAggregationOperator.hpp"

#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "storage/WindowAggregationOperationState.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool WindowAggregationOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (generated_) {
    return true;
  }

  std::vector<block_id> relation_blocks =
      input_relation_.getBlocksSnapshot();

  DCHECK(query_context != nullptr);
  container->addNormalWorkOrder(
      new WindowAggregationWorkOrder(
          query_id_,
          query_context->releaseWindowAggregationState(window_aggregation_state_index_),
          std::move(relation_blocks),
          query_context->getInsertDestination(output_destination_index_)),
      op_index_);
  generated_ = true;
  return true;
}

bool WindowAggregationOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (generated_) {
    return true;
  }

  container->addWorkOrderProto(createWorkOrderProto(), op_index_);
  generated_ = true;
  return true;
}

serialization::WorkOrder* WindowAggregationOperator::createWorkOrderProto() {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::WINDOW_AGGREGATION);
  proto->set_query_id(query_id_);
  proto->SetExtension(serialization::WindowAggregationWorkOrder::window_aggr_state_index,
                      window_aggregation_state_index_);

  const std::vector<block_id> relation_blocks = input_relation_.getBlocksSnapshot();
  for (const block_id bid : relation_blocks) {
    proto->AddExtension(serialization::WindowAggregationWorkOrder::block_ids, bid);
  }
  proto->SetExtension(serialization::WindowAggregationWorkOrder::insert_destination_index,
                      output_destination_index_);

  return proto;
}


void WindowAggregationWorkOrder::execute() {
  state_->windowAggregateBlocks(output_destination_,
                                block_ids_);
}

}  // namespace quickstep
