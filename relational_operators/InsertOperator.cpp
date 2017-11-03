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

#include "relational_operators/InsertOperator.hpp"

#include <memory>
#include <vector>

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
  if (work_generated_) {
    return true;
  }

  std::vector<std::unique_ptr<Tuple>> tuples;

  for (const QueryContext::tuple_id tuple_index : tuple_indexes_) {
    std::unique_ptr<Tuple> newTuple(query_context->releaseTuple(tuple_index));
    tuples.push_back(std::move(newTuple));
  }

  DCHECK(query_context != nullptr);
  container->addNormalWorkOrder(
      new InsertWorkOrder(
          query_id_,
          query_context->getInsertDestination(output_destination_index_),
          std::move(tuples)),
      op_index_);

  work_generated_ = true;
  return true;
}

bool InsertOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (work_generated_) {
    return true;
  }

  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::INSERT);
  proto->set_query_id(query_id_);
  proto->SetExtension(serialization::InsertWorkOrder::insert_destination_index, output_destination_index_);
  for (const QueryContext::tuple_id tuple_index : tuple_indexes_) {
    proto->AddExtension(serialization::InsertWorkOrder::tuple_indexes, tuple_index);
  }

  container->addWorkOrderProto(proto, op_index_);

  work_generated_ = true;
  return true;
}


void InsertWorkOrder::execute() {
  for (const auto &tuple : tuples_) {
    output_destination_->insertTuple(*tuple);
  }
}

}  // namespace quickstep
