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

#include "relational_operators/InitializeAggregationOperator.hpp"

#include <cstddef>

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/AggregationOperationState.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool InitializeAggregationOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (started_) {
    return true;
  }

  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    AggregationOperationState *agg_state =
        query_context->getAggregationState(aggr_state_index_, part_id);
    DCHECK(agg_state != nullptr);

    for (std::size_t state_part_id = 0;
         state_part_id < aggr_state_num_init_partitions_;
         ++state_part_id) {
      container->addNormalWorkOrder(
          new InitializeAggregationWorkOrder(query_id_,
                                             state_part_id,
                                             agg_state),
          op_index_);
    }
  }

  started_ = true;
  return true;
}

// TODO(quickstep-team) : Think about how the number of partitions could be
// accessed in this function. Until then, we can't use partitioned aggregation
// initialization with the distributed version.
bool InitializeAggregationOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  LOG(FATAL) << "Not supported";

  if (started_) {
    return true;
  }

  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    serialization::WorkOrder *proto = new serialization::WorkOrder;
    proto->set_work_order_type(serialization::INITIALIZE_AGGREGATION);
    proto->set_query_id(query_id_);

    proto->SetExtension(serialization::InitializeAggregationWorkOrder::aggr_state_index, aggr_state_index_);
    proto->SetExtension(serialization::InitializeAggregationWorkOrder::partition_id, part_id);
    proto->SetExtension(serialization::InitializeAggregationWorkOrder::state_partition_id, 0u);

    container->addWorkOrderProto(proto, op_index_);
  }
  started_ = true;
  return true;
}

void InitializeAggregationWorkOrder::execute() {
  state_->initialize(state_partition_id_);
}

}  // namespace quickstep
