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

#include "relational_operators/InsertOperator.hpp"

#include <memory>

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/InsertDestination.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool InsertOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (blocking_dependencies_met_ && !work_generated_) {
    DCHECK(query_context != nullptr);

    work_generated_ = true;
    container->addNormalWorkOrder(
        new InsertWorkOrder(
            query_id_,
            query_context->getInsertDestination(output_destination_index_),
            query_context->releaseTuple(tuple_index_)),
        op_index_);
  }
  return work_generated_;
}

bool InsertOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;

    serialization::WorkOrder *proto = new serialization::WorkOrder;
    proto->set_work_order_type(serialization::INSERT);
    proto->set_query_id(query_id_);
    proto->SetExtension(serialization::InsertWorkOrder::insert_destination_index, output_destination_index_);
    proto->SetExtension(serialization::InsertWorkOrder::tuple_index, tuple_index_);

    container->addWorkOrderProto(proto, op_index_);
  }

  return work_generated_;
}


void InsertWorkOrder::execute() {
  output_destination_->insertTuple(*tuple_);
}

}  // namespace quickstep
