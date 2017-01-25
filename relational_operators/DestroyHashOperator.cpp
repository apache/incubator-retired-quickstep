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

#include "relational_operators/DestroyHashOperator.hpp"

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool DestroyHashOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (blocking_dependencies_met_ && !work_generated_) {
    for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
      container->addNormalWorkOrder(
          new DestroyHashWorkOrder(query_id_, hash_table_index_, part_id, query_context),
          op_index_);
    }
    work_generated_ = true;
  }
  return work_generated_;
}

bool DestroyHashOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
      serialization::WorkOrder *proto = new serialization::WorkOrder;
      proto->set_work_order_type(serialization::DESTROY_HASH);
      proto->set_query_id(query_id_);
      proto->SetExtension(serialization::DestroyHashWorkOrder::join_hash_table_index, hash_table_index_);
      proto->SetExtension(serialization::DestroyHashWorkOrder::partition_id, part_id);

      container->addWorkOrderProto(proto, op_index_);
    }

    work_generated_ = true;
  }
  return work_generated_;
}


void DestroyHashWorkOrder::execute() {
  query_context_->destroyJoinHashTable(hash_table_index_, part_id_);
}

}  // namespace quickstep
