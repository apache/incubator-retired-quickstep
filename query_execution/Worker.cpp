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

#include "query_execution/Worker.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <utility>

#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "query_optimizer/QueryOptimizerConfig.h"  // For QUICKSTEP_DISTRIBUTED.
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

using serialization::WorkOrderCompletionMessage;

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
#ifdef QUICKSTEP_DISTRIBUTED
      case kShiftbossRegistrationResponseMessage: {
        serialization::ShiftbossRegistrationResponseMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        shiftboss_index_ = proto.shiftboss_index();
        break;
      }
#endif  // QUICKSTEP_DISTRIBUTED
      case kWorkOrderMessage: {
        WorkOrderCompletionMessage proto;
        executeWorkOrderHelper(tagged_message, &proto);
        sendWorkOrderCompleteMessage(
            annotated_msg.sender, proto, kWorkOrderCompleteMessage);
        break;
      }
      case kRebuildWorkOrderMessage: {
        WorkOrderCompletionMessage proto;
        executeWorkOrderHelper(tagged_message, &proto, true /* is_rebuild */);
        sendWorkOrderCompleteMessage(
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

void Worker::sendWorkOrderCompleteMessage(const tmb::client_id receiver,
                                          const WorkOrderCompletionMessage &proto,
                                          const message_type_id message_type) {
  // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char *>(std::malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_message(
      static_cast<const void *>(proto_bytes), proto_length, message_type);
  std::free(proto_bytes);

  DLOG(INFO) << "Worker " << worker_thread_index_
#ifdef QUICKSTEP_DISTRIBUTED
             << " in Shiftboss " << shiftboss_index_
#endif  // QUICKSTEP_DISTRIBUTED
             << " sent " << QueryExecutionUtil::MessageTypeToString(message_type)
             << " to Scheduler with Client " << receiver;
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(
          bus_, worker_client_id_, receiver, std::move(tagged_message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK);
}

void Worker::executeWorkOrderHelper(const TaggedMessage &tagged_message,
                                    WorkOrderCompletionMessage *proto,
                                    const bool is_rebuild_work_order) {
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  WorkerMessage worker_message(
      *static_cast<const WorkerMessage *>(tagged_message.message()));
  std::unique_ptr<WorkOrder> work_order(worker_message.getWorkOrder());
  DCHECK(work_order);

  const size_t query_id_for_workorder = work_order->getQueryID();
  const partition_id part_id = work_order->getPartitionId();

  // Start measuring the execution time.
  start = std::chrono::steady_clock::now();
  work_order->execute();
  end = std::chrono::steady_clock::now();
  work_order.reset();

  // Convert the measured timestamps to epoch times in microseconds.
  const uint64_t execution_start_time =
      std::chrono::duration_cast<std::chrono::microseconds>(
          start.time_since_epoch()).count();
  const uint64_t execution_end_time =
      std::chrono::duration_cast<std::chrono::microseconds>(
          end.time_since_epoch()).count();

  // Construct the proto message.
  proto->set_work_order_type(is_rebuild_work_order ? WorkOrderCompletionMessage::REBUILD
                                                   : WorkOrderCompletionMessage::NORMAL);
  proto->set_operator_index(worker_message.getRelationalOpIndex());
  proto->set_query_id(query_id_for_workorder);
  proto->set_partition_id(part_id);
  proto->set_worker_thread_index(worker_thread_index_);
  proto->set_execution_start_time(execution_start_time);
  proto->set_execution_end_time(execution_end_time);

#ifdef QUICKSTEP_DISTRIBUTED
  proto->set_shiftboss_index(shiftboss_index_);
#endif  // QUICKSTEP_DISTRIBUTED
}

}  // namespace quickstep
