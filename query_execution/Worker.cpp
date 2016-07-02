/**
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

#include "query_execution/Worker.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <utility>

#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "threading/ThreadUtil.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::size_t;
using std::uint64_t;

using tmb::TaggedMessage;

namespace quickstep {

void Worker::run() {
  if (cpu_id_ >= 0) {
    ThreadUtil::BindToCPU(cpu_id_);
  }
  ClientIDMap *thread_id_map = ClientIDMap::Instance();
  thread_id_map->addValue(worker_client_id_);
  for (;;) {
    // Receive() is a blocking call, causing this thread to sleep until next
    // message is received.
    const AnnotatedMessage annotated_msg =
        bus_->Receive(worker_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_msg.tagged_message;
    switch (tagged_message.message_type()) {
      case kWorkOrderMessage: {
        serialization::NormalWorkOrderCompletionMessage proto;
        executeWorkOrderHelper<serialization::NormalWorkOrderCompletionMessage>(
            tagged_message, &proto);
        sendWorkOrderCompleteMessage<
            serialization::NormalWorkOrderCompletionMessage>(
            annotated_msg.sender, proto, kWorkOrderCompleteMessage);
        break;
      }
      case kRebuildWorkOrderMessage: {
        serialization::RebuildWorkOrderCompletionMessage proto;
        executeWorkOrderHelper<
            serialization::RebuildWorkOrderCompletionMessage>(tagged_message,
                                                              &proto);
        sendWorkOrderCompleteMessage<
            serialization::RebuildWorkOrderCompletionMessage>(
            annotated_msg.sender, proto, kRebuildWorkOrderCompleteMessage);
        break;
      }
      case kPoisonMessage: {
        // Remove the entry from the thread ID based map for this worker thread.
        thread_id_map->removeValue();
        return;
      }
    }
  }
}

template <typename CompletionMessageProtoT>
void Worker::sendWorkOrderCompleteMessage(const tmb::client_id receiver,
                                          const CompletionMessageProtoT &proto,
                                          const message_type_id message_type) {
  // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char *>(std::malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_message(
      static_cast<const void *>(proto_bytes), proto_length, message_type);
  std::free(proto_bytes);

  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(
          bus_, worker_client_id_, receiver, std::move(tagged_message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
      << "Message could not be sent from worker with TMB client ID "
      << worker_client_id_ << " to Foreman with TMB client ID " << receiver;
}

template <typename CompletionMessageProtoT>
void Worker::executeWorkOrderHelper(const TaggedMessage &tagged_message,
                                    CompletionMessageProtoT *proto) {
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  WorkerMessage worker_message(
      *static_cast<const WorkerMessage *>(tagged_message.message()));
  DCHECK(worker_message.getWorkOrder() != nullptr);
  const size_t query_id_for_workorder = worker_message.getWorkOrder()->getQueryID();

  // Start measuring the execution time.
  start = std::chrono::steady_clock::now();
  worker_message.getWorkOrder()->execute();
  end = std::chrono::steady_clock::now();
  delete worker_message.getWorkOrder();
  const uint64_t execution_time_microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();
  // Construct the proto message.
  proto->set_operator_index(worker_message.getRelationalOpIndex());
  proto->set_query_id(query_id_for_workorder);
  proto->set_worker_thread_index(worker_thread_index_);
  proto->set_execution_time_in_microseconds(execution_time_microseconds);
}

}  // namespace quickstep
