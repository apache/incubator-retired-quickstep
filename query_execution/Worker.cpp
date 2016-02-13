/**
 *   Copyright 2015 Pivotal Software, Inc.
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

#include <utility>

#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/Macros.hpp"

#include "tmb/address.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

#include "glog/logging.h"

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
    AnnotatedMessage annotated_msg = bus_->Receive(worker_client_id_, 0, true);
    switch (annotated_msg.tagged_message.message_type()) {
      case kWorkOrderMessage: {
        WorkerMessage message(*static_cast<const WorkerMessage *>(
                                  annotated_msg.tagged_message.message()));
        DEBUG_ASSERT(message.getWorkOrder() != nullptr);
        message.getWorkOrder()->execute(
            query_context_.get(), catalog_database_, storage_manager_);
        TaggedMessage foreman_tagged_msg;
        ForemanMessage foreman_message(
            ForemanMessage::WorkOrderCompletionMessage(
                message.getRelationalOpIndex(),
                worker_id_));
        foreman_tagged_msg.set_message(&foreman_message,
                                       sizeof(foreman_message),
                                       kWorkOrderCompleteMessage);
        tmb::MessageBus::SendStatus send_status =
            QueryExecutionUtil::SendTMBMessage(bus_,
                                               worker_client_id_,
                                               annotated_msg.sender,
                                               std::move(foreman_tagged_msg));
        if (send_status != tmb::MessageBus::SendStatus::kOK) {
          LOG(FATAL) << "Message could not be sent from Worker with TMB client "
            "ID " << worker_client_id_ << " to Foreman";
        }
        delete message.getWorkOrder();
        break;
      }
      case kRebuildWorkOrderMessage: {
        WorkerMessage message(*static_cast<const WorkerMessage *>(
                                  annotated_msg.tagged_message.message()));
        DEBUG_ASSERT(message.getWorkOrder() != nullptr);
        message.getWorkOrder()->execute(
            query_context_.get(), catalog_database_, storage_manager_);
        TaggedMessage foreman_tagged_msg;
        ForemanMessage foreman_message(
            ForemanMessage::RebuildCompletionMessage(
                message.getRelationalOpIndex(), worker_id_));
        foreman_tagged_msg.set_message(&foreman_message,
                                       sizeof(foreman_message),
                                       kRebuildWorkOrderCompleteMessage);
        tmb::MessageBus::SendStatus send_status =
            QueryExecutionUtil::SendTMBMessage(bus_,
                                               worker_client_id_,
                                               annotated_msg.sender,
                                               std::move(foreman_tagged_msg));
        if (send_status != tmb::MessageBus::SendStatus::kOK) {
          LOG(FATAL) << "Message could not be sent from Worker with TMB client "
            "ID " << worker_client_id_ << " to Foreman";
        }
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

}  // namespace quickstep
