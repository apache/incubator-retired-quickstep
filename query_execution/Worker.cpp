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

#include <cstddef>
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
    const AnnotatedMessage annotated_msg = bus_->Receive(worker_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_msg.tagged_message;
    switch (tagged_message.message_type()) {
      case kWorkOrderMessage:  // Fall through.
      case kRebuildWorkOrderMessage: {
        WorkerMessage message(*static_cast<const WorkerMessage*>(tagged_message.message()));
        DCHECK(message.getWorkOrder() != nullptr);
        message.getWorkOrder()->execute(
            query_context_.get(), catalog_database_, storage_manager_);

        sendWorkOrderCompleteMessage(annotated_msg.sender,
                                     message.getRelationalOpIndex(),
                                     tagged_message.message_type() == kRebuildWorkOrderMessage);
        delete message.getWorkOrder();
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
                                          const size_t op_index,
                                          const bool isRebuildWorkOrder) {
  serialization::WorkOrderCompletionMessage proto;
  proto.set_operator_index(op_index);
  proto.set_worker_id(worker_id_);

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        isRebuildWorkOrder ? kRebuildWorkOrderCompleteMessage
                                           : kWorkOrderCompleteMessage);
  std::free(proto_bytes);

  QueryExecutionUtil::SendTMBMessage(bus_,
                                     worker_client_id_,
                                     receiver,
                                     std::move(message));
}

}  // namespace quickstep
