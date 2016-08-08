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

#include "query_execution/ForemanSingleNode.hpp"

#include <cstddef>
#include <cstdio>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "query_execution/AdmitRequestMessage.hpp"
#include "query_execution/PolicyEnforcerSingleNode.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/EqualsAnyConstant.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::move;
using std::size_t;
using std::unique_ptr;
using std::vector;

namespace quickstep {

class QueryHandle;

DEFINE_uint64(min_load_per_worker, 2, "The minimum load defined as the number "
              "of pending work orders for the worker. This information is used "
              "by the Foreman to assign work orders to worker threads");

ForemanSingleNode::ForemanSingleNode(
    const tmb::client_id main_thread_client_id,
    WorkerDirectory *worker_directory,
    tmb::MessageBus *bus,
    CatalogDatabaseLite *catalog_database,
    StorageManager *storage_manager,
    const int cpu_id,
    const size_t num_numa_nodes,
    const bool profile_individual_workorders)
    : ForemanBase(bus, cpu_id),
      main_thread_client_id_(main_thread_client_id),
      worker_directory_(DCHECK_NOTNULL(worker_directory)),
      catalog_database_(DCHECK_NOTNULL(catalog_database)),
      storage_manager_(DCHECK_NOTNULL(storage_manager)) {
  const std::vector<QueryExecutionMessageType> sender_message_types{
      kPoisonMessage,
      kRebuildWorkOrderMessage,
      kWorkOrderMessage,
      kWorkloadCompletionMessage};

  for (const auto message_type : sender_message_types) {
    bus_->RegisterClientAsSender(foreman_client_id_, message_type);
  }

  const std::vector<QueryExecutionMessageType> receiver_message_types{
      kAdmitRequestMessage,
      kCatalogRelationNewBlockMessage,
      kDataPipelineMessage,
      kPoisonMessage,
      kRebuildWorkOrderCompleteMessage,
      kWorkOrderFeedbackMessage,
      kWorkOrderCompleteMessage};

  for (const auto message_type : receiver_message_types) {
    bus_->RegisterClientAsReceiver(foreman_client_id_, message_type);
  }

  policy_enforcer_.reset(new PolicyEnforcerSingleNode(
      foreman_client_id_,
      num_numa_nodes,
      catalog_database_,
      storage_manager_,
      worker_directory_,
      bus_,
      profile_individual_workorders));
}

void ForemanSingleNode::run() {
  if (cpu_id_ >= 0) {
    // We can pin the foreman thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }

  // Event loop
  for (;;) {
    // Receive() causes this thread to sleep until next message is received.
    const AnnotatedMessage annotated_msg =
        bus_->Receive(foreman_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_msg.tagged_message;
    const tmb::message_type_id message_type = tagged_message.message_type();
    switch (message_type) {
      case kCatalogRelationNewBlockMessage:  // Fall through
      case kDataPipelineMessage:
      case kRebuildWorkOrderCompleteMessage:
      case kWorkOrderCompleteMessage:
      case kWorkOrderFeedbackMessage: {
        policy_enforcer_->processMessage(tagged_message);
        break;
      }

      case kAdmitRequestMessage: {
        const AdmitRequestMessage *msg =
            static_cast<const AdmitRequestMessage *>(tagged_message.message());
        const vector<QueryHandle *> &query_handles = msg->getQueryHandles();

        DCHECK(!query_handles.empty());
        bool all_queries_admitted = true;
        if (query_handles.size() == 1u) {
          all_queries_admitted =
              policy_enforcer_->admitQuery(query_handles.front());
        } else {
          all_queries_admitted = policy_enforcer_->admitQueries(query_handles);
        }
        if (!all_queries_admitted) {
          LOG(WARNING) << "The scheduler could not admit all the queries";
          // TODO(harshad) - Inform the main thread about the failure.
        }
        break;
      }
      case kPoisonMessage: {
        if (policy_enforcer_->hasQueries()) {
          LOG(WARNING) << "Foreman thread exiting while some queries are "
                          "under execution or waiting to be admitted";
        }
        return;
      }
      default:
        LOG(FATAL) << "Unknown message type to Foreman";
    }

    if (canCollectNewMessages(message_type)) {
      vector<unique_ptr<WorkerMessage>> new_messages;
      policy_enforcer_->getWorkerMessages(&new_messages);
      dispatchWorkerMessages(new_messages);
    }

    // We check again, as some queries may produce zero work orders and finish
    // their execution.
    if (!policy_enforcer_->hasQueries()) {
      // Signal the main thread that there are no queries to be executed.
      // Currently the message doesn't have any real content.
      TaggedMessage completion_tagged_message(kWorkloadCompletionMessage);
      const tmb::MessageBus::SendStatus send_status =
          QueryExecutionUtil::SendTMBMessage(
              bus_,
              foreman_client_id_,
              main_thread_client_id_,
              move(completion_tagged_message));
      CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
          << "Message could not be sent from Foreman with TMB client ID "
          << foreman_client_id_ << " to main thread with TMB client ID"
          << main_thread_client_id_;
    }
  }
}

bool ForemanSingleNode::canCollectNewMessages(const tmb::message_type_id message_type) {
  if (QUICKSTEP_EQUALS_ANY_CONSTANT(message_type,
                                    kCatalogRelationNewBlockMessage,
                                    kWorkOrderFeedbackMessage)) {
    return false;
  } else if (worker_directory_->getLeastLoadedWorker().second <=
             FLAGS_min_load_per_worker) {
    // If the least loaded worker has only one pending work order, we should
    // collect new messages and dispatch them.
    return true;
  } else {
    return false;
  }
}

void ForemanSingleNode::dispatchWorkerMessages(const vector<unique_ptr<WorkerMessage>> &messages) {
  for (const auto &message : messages) {
    DCHECK(message != nullptr);
    const int recipient_worker_thread_index = message->getRecipientHint();
    if (recipient_worker_thread_index != WorkerMessage::kInvalidRecipientIndexHint) {
      sendWorkerMessage(static_cast<size_t>(recipient_worker_thread_index),
                        *message);
      worker_directory_->incrementNumQueuedWorkOrders(recipient_worker_thread_index);
    } else {
      const size_t least_loaded_worker_thread_index = worker_directory_->getLeastLoadedWorker().first;
      sendWorkerMessage(least_loaded_worker_thread_index, *message);
      worker_directory_->incrementNumQueuedWorkOrders(least_loaded_worker_thread_index);
    }
  }
}

void ForemanSingleNode::sendWorkerMessage(const size_t worker_thread_index,
                                          const WorkerMessage &message) {
  tmb::message_type_id type;
  if (message.getType() == WorkerMessage::WorkerMessageType::kRebuildWorkOrder) {
    type = kRebuildWorkOrderMessage;
  } else if (message.getType() == WorkerMessage::WorkerMessageType::kWorkOrder) {
    type = kWorkOrderMessage;
  } else {
    FATAL_ERROR("Invalid WorkerMessageType");
  }
  TaggedMessage worker_tagged_message(&message, sizeof(message), type);

  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         worker_directory_->getClientID(worker_thread_index),
                                         move(worker_tagged_message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK) <<
      "Message could not be sent from Foreman with TMB client ID "
      << foreman_client_id_ << " to Foreman with TMB client ID "
      << worker_directory_->getClientID(worker_thread_index);
}

const std::vector<WorkOrderTimeEntry>& ForemanSingleNode
    ::getWorkOrderProfilingResults(const std::size_t query_id) const {
  return policy_enforcer_->getProfilingResults(query_id);
}

void ForemanSingleNode::printWorkOrderProfilingResults(const std::size_t query_id,
                                                       std::FILE *out) const {
  const std::vector<WorkOrderTimeEntry> &recorded_times =
      policy_enforcer_->getProfilingResults(query_id);
  fputs("Query ID,Worker ID,NUMA Socket,Operator ID,Time (microseconds)\n", out);
  for (auto workorder_entry : recorded_times) {
    // Note: Index of the "worker thread index" in the tuple is 0.
    const std::size_t worker_id = workorder_entry.worker_id;
    fprintf(out,
            "%lu,%lu,%d,%lu,%lu\n",
            query_id,
            worker_id,
            worker_directory_->getNUMANode(worker_id),
            workorder_entry.operator_id,  // Operator ID.
            workorder_entry.end_time - workorder_entry.start_time);  // Time.
  }
}

}  // namespace quickstep
