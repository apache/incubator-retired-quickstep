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
    work_generated_ = true;
    container->addNormalWorkOrder(new DestroyHashWorkOrder(hash_table_index_, query_context),
                                  op_index_);
  }
  return work_generated_;
}

bool DestroyHashOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;

    serialization::WorkOrder *proto = new serialization::WorkOrder;
    proto->set_work_order_type(serialization::DESTROY_HASH);
    proto->SetExtension(serialization::DestroyHashWorkOrder::join_hash_table_index, hash_table_index_);

    container->addWorkOrderProto(proto, op_index_);
  }
  return work_generated_;
}


void DestroyHashWorkOrder::execute() {
  query_context_->destroyJoinHashTable(hash_table_index_);
}

}  // namespace quickstep
