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

#include "relational_operators/DeleteOperator.hpp"

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {

bool DeleteOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  const Predicate *predicate = query_context->getPredicate(predicate_index_);

  if (relation_is_stored_) {
    // If relation_ is stored, iterate over the list of blocks in relation_.
    if (started_) {
      return true;
    }

    for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
      for (const block_id input_block_id : relation_block_ids_[part_id]) {
        container->addNormalWorkOrder(
            new DeleteWorkOrder(query_id_,
                                relation_,
                                part_id,
                                input_block_id,
                                predicate,
                                storage_manager,
                                op_index_,
                                scheduler_client_id,
                                bus),
            op_index_);
      }
    }
    started_ = true;
    return true;
  }

  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    while (num_workorders_generated_[part_id] < relation_block_ids_[part_id].size()) {
      container->addNormalWorkOrder(
          new DeleteWorkOrder(query_id_,
                              relation_,
                              part_id,
                              relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                              predicate,
                              storage_manager,
                              op_index_,
                              scheduler_client_id,
                              bus),
          op_index_);
      ++num_workorders_generated_[part_id];
    }
  }
  return done_feeding_input_relation_;
}

bool DeleteOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (relation_is_stored_) {
    // If relation_ is stored, iterate over the list of blocks in relation_.
    if (started_) {
      return true;
    }

    for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
      for (const block_id input_block_id : relation_block_ids_[part_id]) {
        container->addWorkOrderProto(createWorkOrderProto(part_id, input_block_id), op_index_);
      }
    }
    started_ = true;
    return true;
  }

  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    while (num_workorders_generated_[part_id] < relation_block_ids_[part_id].size()) {
      container->addWorkOrderProto(
          createWorkOrderProto(part_id, relation_block_ids_[part_id][num_workorders_generated_[part_id]]),
          op_index_);
      ++num_workorders_generated_[part_id];
    }
  }
  return done_feeding_input_relation_;
}

serialization::WorkOrder* DeleteOperator::createWorkOrderProto(const partition_id part_id, const block_id block) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::DELETE);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::DeleteWorkOrder::operator_index, op_index_);
  proto->SetExtension(serialization::DeleteWorkOrder::relation_id, relation_.getID());
  proto->SetExtension(serialization::DeleteWorkOrder::predicate_index, predicate_index_);
  proto->SetExtension(serialization::DeleteWorkOrder::block_id, block);
  proto->SetExtension(serialization::DeleteWorkOrder::partition_id, part_id);

  return proto;
}


void DeleteWorkOrder::execute() {
  MutableBlockReference block(
      storage_manager_->getBlockMutable(input_block_id_, input_relation_));
  block->deleteTuples(predicate_);

  // TODO(harshad): Stream the block ID only if the predicate returned at least
  // one match in the StorageBlock.
  serialization::DataPipelineMessage proto;
  proto.set_operator_index(delete_operator_index_);
  proto.set_block_id(input_block_id_);
  proto.set_relation_id(input_relation_.getID());
  proto.set_query_id(query_id_);
  proto.set_partition_id(partition_id_);

  // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
  const std::size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  tmb::TaggedMessage tagged_message(static_cast<const void *>(proto_bytes),
                                    proto_length,
                                    kDataPipelineMessage);
  std::free(proto_bytes);

  DLOG(INFO) << "DeleteWorkOrder sent DataPipelineMessage to Scheduler with Client " << scheduler_client_id_;
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(
          bus_,
          ClientIDMap::Instance()->getValue(),
          scheduler_client_id_,
          std::move(tagged_message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK);
}

}  // namespace quickstep
