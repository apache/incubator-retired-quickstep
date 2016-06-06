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

#include "relational_operators/DeleteOperator.hpp"

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
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
    if (!started_) {
      for (const block_id input_block_id : relation_block_ids_) {
        container->addNormalWorkOrder(
            new DeleteWorkOrder(relation_,
                                input_block_id,
                                predicate,
                                storage_manager,
                                op_index_,
                                scheduler_client_id,
                                query_id_,
                                bus),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new DeleteWorkOrder(relation_,
                              relation_block_ids_[num_workorders_generated_],
                              predicate,
                              storage_manager,
                              op_index_,
                              scheduler_client_id,
                              query_id_,
                              bus),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
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

  // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
  const std::size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  tmb::TaggedMessage tagged_message(static_cast<const void *>(proto_bytes),
                                    proto_length,
                                    kDataPipelineMessage);
  std::free(proto_bytes);

  const tmb::client_id worker_thread_client_id = ClientIDMap::Instance()->getValue();
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(
          bus_,
          worker_thread_client_id,
          scheduler_client_id_,
          std::move(tagged_message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK) << "Message could not"
      " be sent from thread with TMB client ID " <<
      worker_thread_client_id << " to Foreman with TMB client ID "
      << scheduler_client_id_;
}

}  // namespace quickstep
