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

#include "query_execution/Shiftboss.hpp"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "relational_operators/RebuildWorkOrder.hpp"
#include "relational_operators/WorkOrderFactory.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadUtil.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;
using std::size_t;
using std::string;
using std::unique_ptr;
using std::vector;

using tmb::AnnotatedMessage;
using tmb::MessageBus;
using tmb::TaggedMessage;

namespace quickstep {

class WorkOrder;

Shiftboss::Shiftboss(tmb::MessageBus *bus_global,
                     tmb::MessageBus *bus_local,
                     StorageManager *storage_manager,
                     WorkerDirectory *workers,
                     void *hdfs,
                     const int cpu_id)
    : bus_global_(DCHECK_NOTNULL(bus_global)),
      bus_local_(DCHECK_NOTNULL(bus_local)),
      storage_manager_(DCHECK_NOTNULL(storage_manager)),
      workers_(DCHECK_NOTNULL(workers)),
      hdfs_(hdfs),
      cpu_id_(cpu_id),
      shiftboss_client_id_global_(tmb::kClientIdNone),
      shiftboss_client_id_local_(tmb::kClientIdNone),
      foreman_client_id_(tmb::kClientIdNone),
      max_msgs_per_worker_(1),
      start_worker_index_(0u) {
  // Check to have at least one Worker.
  DCHECK_GT(workers->getNumWorkers(), 0u);

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
  if (FLAGS_use_hdfs) {
    CHECK(hdfs_);
  }
#endif  // QUICKSTEP_HAVE_FILE_MANAGER_HDFS

  shiftboss_client_id_global_ = bus_global_->Connect();
  LOG(INFO) << "Shiftboss TMB client ID: " << shiftboss_client_id_global_;
  DCHECK_NE(shiftboss_client_id_global_, tmb::kClientIdNone);

  shiftboss_client_id_local_ = bus_local_->Connect();
  DCHECK_NE(shiftboss_client_id_local_, tmb::kClientIdNone);

  // Messages between Foreman and Shiftboss.
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kShiftbossRegistrationMessage);
  bus_global_->RegisterClientAsReceiver(shiftboss_client_id_global_, kShiftbossRegistrationResponseMessage);

  bus_global_->RegisterClientAsReceiver(shiftboss_client_id_global_, kQueryInitiateMessage);
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kQueryInitiateResponseMessage);

  bus_global_->RegisterClientAsReceiver(shiftboss_client_id_global_, kInitiateRebuildMessage);
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kInitiateRebuildResponseMessage);

  // Message sent to Worker.
  bus_local_->RegisterClientAsSender(shiftboss_client_id_local_, kShiftbossRegistrationResponseMessage);
  bus_local_->RegisterClientAsSender(shiftboss_client_id_local_, kRebuildWorkOrderMessage);

  // Forward the following message types from Foreman to Workers.
  bus_global_->RegisterClientAsReceiver(shiftboss_client_id_global_, kWorkOrderMessage);
  bus_local_->RegisterClientAsSender(shiftboss_client_id_local_, kWorkOrderMessage);

  // Forward the following message types from Workers to Foreman.
  bus_local_->RegisterClientAsReceiver(shiftboss_client_id_local_, kCatalogRelationNewBlockMessage);
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kCatalogRelationNewBlockMessage);

  bus_local_->RegisterClientAsReceiver(shiftboss_client_id_local_, kDataPipelineMessage);
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kDataPipelineMessage);

  bus_local_->RegisterClientAsReceiver(shiftboss_client_id_local_, kWorkOrderFeedbackMessage);
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kWorkOrderFeedbackMessage);

  bus_local_->RegisterClientAsReceiver(shiftboss_client_id_local_, kWorkOrderCompleteMessage);
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kWorkOrderCompleteMessage);

  bus_local_->RegisterClientAsReceiver(shiftboss_client_id_local_, kRebuildWorkOrderCompleteMessage);
  bus_global_->RegisterClientAsSender(shiftboss_client_id_global_, kRebuildWorkOrderCompleteMessage);

  // Clean up query execution states, i.e., QueryContext.
  bus_global_->RegisterClientAsReceiver(shiftboss_client_id_global_, kQueryTeardownMessage);

  // Stop itself.
  bus_global_->RegisterClientAsReceiver(shiftboss_client_id_global_, kPoisonMessage);
  // Stop all workers.
  bus_local_->RegisterClientAsSender(shiftboss_client_id_local_, kPoisonMessage);

  for (std::size_t i = 0; i < workers_->getNumWorkers(); ++i) {
    worker_addresses_.AddRecipient(workers_->getClientID(i));
  }

  registerWithForeman();
}

void Shiftboss::run() {
  if (cpu_id_ >= 0) {
    // We can pin the shiftboss thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }

  processShiftbossRegistrationResponseMessage();

  AnnotatedMessage annotated_message;
  tmb::message_type_id message_type;
  for (;;) {
    if (bus_global_->ReceiveIfAvailable(shiftboss_client_id_global_, &annotated_message, 0, true)) {
      message_type = annotated_message.tagged_message.message_type();
      DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_global_
                 << " received " << QueryExecutionUtil::MessageTypeToString(message_type)
                 << " from Foreman with Client " << annotated_message.sender;
      switch (message_type) {
        case kQueryInitiateMessage: {
          const TaggedMessage &tagged_message = annotated_message.tagged_message;

          serialization::QueryInitiateMessage proto;
          CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

          processQueryInitiateMessage(proto.query_id(), proto.catalog_database_cache(), proto.query_context());
          break;
        }
        case kWorkOrderMessage: {
          const TaggedMessage &tagged_message = annotated_message.tagged_message;

          serialization::WorkOrderMessage proto;
          CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

          const std::size_t query_id = proto.query_id();
          DCHECK_EQ(1u, query_contexts_.count(query_id));

          unique_ptr<WorkOrder> work_order(
              WorkOrderFactory::ReconstructFromProto(proto.work_order(), shiftboss_index_, &database_cache_,
                                                     query_contexts_[query_id].get(), storage_manager_,
                                                     shiftboss_client_id_local_, bus_local_, hdfs_));

          unique_ptr<WorkerMessage> worker_message(
              WorkerMessage::WorkOrderMessage(work_order.release(), proto.operator_index()));

          TaggedMessage worker_tagged_message(worker_message.get(),
                                              sizeof(*worker_message),
                                              kWorkOrderMessage);

          const size_t worker_index = getSchedulableWorker();
          DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_local_
                     << " forwarded WorkOrderMessage from Foreman to Worker " << worker_index;

          const MessageBus::SendStatus send_status =
              QueryExecutionUtil::SendTMBMessage(bus_local_,
                                                 shiftboss_client_id_local_,
                                                 workers_->getClientID(worker_index),
                                                 move(worker_tagged_message));
          CHECK(send_status == MessageBus::SendStatus::kOK);
          break;
        }
        case kInitiateRebuildMessage: {
          // Construct rebuild work orders, and send back their number to
          // 'ForemanDistributed'.
          const TaggedMessage &tagged_message = annotated_message.tagged_message;

          serialization::InitiateRebuildMessage proto;
          CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

          processInitiateRebuildMessage(proto.query_id(),
                                        proto.operator_index(),
                                        proto.insert_destination_index(),
                                        proto.relation_id());
          break;
        }
        case kQueryTeardownMessage: {
          const TaggedMessage &tagged_message = annotated_message.tagged_message;

          serialization::QueryTeardownMessage proto;
          CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

          query_contexts_.erase(proto.query_id());
          break;
        }
        case kPoisonMessage: {
          DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_global_
                     << " forwarded PoisonMessage from Foreman to all Workers";

          tmb::MessageStyle broadcast_style;
          broadcast_style.Broadcast(true);

          const MessageBus::SendStatus send_status =
              bus_local_->Send(shiftboss_client_id_local_, worker_addresses_, broadcast_style,
                               move(annotated_message.tagged_message));
          CHECK(send_status == MessageBus::SendStatus::kOK);
          return;
        }
        default: {
          LOG(FATAL) << "Unknown TMB message type";
        }
      }
    }

    while (bus_local_->ReceiveIfAvailable(shiftboss_client_id_local_, &annotated_message, 0, true)) {
      message_type = annotated_message.tagged_message.message_type();
      switch (message_type) {
        case kCatalogRelationNewBlockMessage:
        case kDataPipelineMessage:
        case kWorkOrderFeedbackMessage:
        case kWorkOrderCompleteMessage:
        case kRebuildWorkOrderCompleteMessage: {
          DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_global_
                     << " forwarded " << QueryExecutionUtil::MessageTypeToString(message_type)
                     << " from Worker with Client " << annotated_message.sender
                     << " to Foreman with Client " << foreman_client_id_;

          DCHECK_NE(foreman_client_id_, tmb::kClientIdNone);
          const MessageBus::SendStatus send_status =
              QueryExecutionUtil::SendTMBMessage(bus_global_,
                                                 shiftboss_client_id_global_,
                                                 foreman_client_id_,
                                                 move(annotated_message.tagged_message));
          CHECK(send_status == MessageBus::SendStatus::kOK);
          break;
        }
        default: {
          LOG(FATAL) << "Unknown TMB message type";
        }
      }
    }
  }
}

size_t Shiftboss::getSchedulableWorker() {
  const size_t num_workers = workers_->getNumWorkers();

  size_t curr_worker = start_worker_index_;
  for (;;) {
    if (workers_->getNumQueuedWorkOrders(curr_worker) < max_msgs_per_worker_) {
      start_worker_index_ = (curr_worker + 1) % num_workers;
      // TODO(zuyu): workers_->incrementNumQueuedWorkOrders(curr_worker);
      // But we need a WorkOrder queue first.
      return curr_worker;
    }

    curr_worker = (curr_worker + 1) % num_workers;
  }
}

void Shiftboss::registerWithForeman() {
  tmb::Address all_addresses;
  all_addresses.All(true);

  tmb::MessageStyle style;

  serialization::ShiftbossRegistrationMessage proto;
  proto.set_work_order_capacity(getWorkOrderCapacity());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kShiftbossRegistrationMessage);
  free(proto_bytes);

  DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_global_
             << " sent ShiftbossRegistrationMessage to all";
  tmb::MessageBus::SendStatus send_status =
      bus_global_->Send(shiftboss_client_id_global_, all_addresses, style, move(message));
  DCHECK(send_status == tmb::MessageBus::SendStatus::kOK);
}

void Shiftboss::processShiftbossRegistrationResponseMessage() {
  AnnotatedMessage annotated_message(bus_global_->Receive(shiftboss_client_id_global_, 0, true));
  const TaggedMessage &tagged_message = annotated_message.tagged_message;
  DCHECK_EQ(kShiftbossRegistrationResponseMessage, tagged_message.message_type());

  foreman_client_id_ = annotated_message.sender;
  DLOG(INFO) << "Shiftboss with Client " << shiftboss_client_id_local_
             << " received ShiftbossRegistrationResponseMessage from Foreman with Client " << foreman_client_id_;

  serialization::ShiftbossRegistrationResponseMessage proto;
  CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

  shiftboss_index_ = proto.shiftboss_index();
  storage_manager_->sendBlockDomainToShiftbossIndexMessage(shiftboss_index_);

  // Forward this message to Workers regarding <shiftboss_index_>.
  QueryExecutionUtil::BroadcastMessage(shiftboss_client_id_local_,
                                       worker_addresses_,
                                       move(annotated_message.tagged_message),
                                       bus_local_);
}

void Shiftboss::processQueryInitiateMessage(
    const std::size_t query_id,
    const serialization::CatalogDatabase &catalog_database_cache_proto,
    const serialization::QueryContext &query_context_proto) {
  database_cache_.update(catalog_database_cache_proto);

  auto query_context = std::make_unique<QueryContext>(
      query_context_proto, database_cache_, storage_manager_, shiftboss_client_id_local_, bus_local_);
  query_contexts_.emplace(query_id, move(query_context));

  serialization::QueryInitiateResponseMessage proto;
  proto.set_query_id(query_id);

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message_response(static_cast<const void*>(proto_bytes),
                                 proto_length,
                                 kQueryInitiateResponseMessage);
  free(proto_bytes);

  DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_global_
             << " sent QueryInitiateResponseMessage to Foreman with Client " << foreman_client_id_;
  const MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_global_,
                                         shiftboss_client_id_global_,
                                         foreman_client_id_,
                                         move(message_response));
  CHECK(send_status == MessageBus::SendStatus::kOK);
}

void Shiftboss::processInitiateRebuildMessage(const std::size_t query_id,
                                              const std::size_t op_index,
                                              const QueryContext::insert_destination_id dest_index,
                                              const relation_id rel_id) {
  DCHECK_NE(foreman_client_id_, tmb::kClientIdNone);

  DCHECK_EQ(1u, query_contexts_.count(query_id));
  InsertDestination *insert_destination = query_contexts_[query_id]->getInsertDestination(dest_index);
  DCHECK(insert_destination != nullptr);

  vector<MutableBlockReference> partially_filled_block_refs;
  insert_destination->getPartiallyFilledBlocks(&partially_filled_block_refs);

  serialization::InitiateRebuildResponseMessage proto;
  proto.set_query_id(query_id);
  proto.set_operator_index(op_index);
  proto.set_num_rebuild_work_orders(partially_filled_block_refs.size());
  proto.set_shiftboss_index(shiftboss_index_);

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message_response(static_cast<const void*>(proto_bytes),
                                 proto_length,
                                 kInitiateRebuildResponseMessage);
  free(proto_bytes);

  DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_global_
             << " sent InitiateRebuildResponseMessage to Foreman with Client " << foreman_client_id_;
  const MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_global_,
                                         shiftboss_client_id_global_,
                                         foreman_client_id_,
                                         move(message_response));
  CHECK(send_status == MessageBus::SendStatus::kOK);

  for (size_t i = 0; i < partially_filled_block_refs.size(); ++i) {
    // NOTE(zuyu): Worker releases the memory after the execution of
    // RebuildWorkOrder on the Worker.
    WorkOrder *rebuild_work_order =
        new RebuildWorkOrder(query_id,
                             move(partially_filled_block_refs[i]),
                             op_index,
                             rel_id,
                             shiftboss_client_id_local_,
                             bus_local_);

    unique_ptr<WorkerMessage> worker_message(
        WorkerMessage::RebuildWorkOrderMessage(rebuild_work_order, op_index));

    TaggedMessage worker_tagged_message(worker_message.get(),
                                        sizeof(*worker_message),
                                        kRebuildWorkOrderMessage);

    const size_t worker_index = getSchedulableWorker();
    DLOG(INFO) << "Shiftboss " << shiftboss_index_ << " with Client " << shiftboss_client_id_local_
               << " sent RebuildWorkOrderMessage to Worker " << worker_index;

    const MessageBus::SendStatus send_status =
        QueryExecutionUtil::SendTMBMessage(bus_local_,
                                           shiftboss_client_id_local_,
                                           workers_->getClientID(worker_index),
                                           move(worker_tagged_message));
    CHECK(send_status == MessageBus::SendStatus::kOK);
  }
}

}  // namespace quickstep
