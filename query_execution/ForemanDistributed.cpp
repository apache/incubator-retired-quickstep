/**
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

#include "query_execution/ForemanDistributed.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/AdmitRequestMessage.hpp"
#include "query_execution/PolicyEnforcerDistributed.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

using std::move;
using std::size_t;
using std::unique_ptr;
using std::vector;

using tmb::AnnotatedMessage;
using tmb::TaggedMessage;

namespace quickstep {

namespace S = serialization;

ForemanDistributed::ForemanDistributed(
    tmb::MessageBus *bus,
    CatalogDatabaseLite *catalog_database,
    const int cpu_id,
    const bool profile_individual_workorders)
    : ForemanBase(bus, cpu_id),
      catalog_database_(DCHECK_NOTNULL(catalog_database)) {
  const std::vector<QueryExecutionMessageType> sender_message_types{
      kShiftbossRegistrationResponseMessage,
      kQueryInitiateMessage,
      kWorkOrderMessage,
      kInitiateRebuildMessage,
      kSaveQueryResultMessage,
      kQueryExecutionSuccessMessage,
      kPoisonMessage};

  for (const auto message_type : sender_message_types) {
    bus_->RegisterClientAsSender(foreman_client_id_, message_type);
  }

  const std::vector<QueryExecutionMessageType> receiver_message_types{
      kShiftbossRegistrationMessage,
      kAdmitRequestMessage,
      kQueryInitiateResponseMessage,
      kCatalogRelationNewBlockMessage,
      kDataPipelineMessage,
      kInitiateRebuildResponseMessage,
      kWorkOrderCompleteMessage,
      kRebuildWorkOrderCompleteMessage,
      kWorkOrderFeedbackMessage,
      kWorkOrdersAvailableMessage,
      kSaveQueryResultResponseMessage,
      kPoisonMessage};

  for (const auto message_type : receiver_message_types) {
    bus_->RegisterClientAsReceiver(foreman_client_id_, message_type);
  }

  policy_enforcer_.reset(new PolicyEnforcerDistributed(
      foreman_client_id_,
      catalog_database_,
      &shiftboss_directory_,
      bus_,
      profile_individual_workorders));
}

void ForemanDistributed::run() {
  if (cpu_id_ >= 0) {
    // We can pin the foreman thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }

  // Ensure that at least one Shiftboss to register.
  if (shiftboss_directory_.empty()) {
    const AnnotatedMessage annotated_message = bus_->Receive(foreman_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    DCHECK_EQ(kShiftbossRegistrationMessage, tagged_message.message_type());
    LOG(INFO) << "ForemanDistributed received typed '" << tagged_message.message_type()
              << "' message from client " << annotated_message.sender;

    S::ShiftbossRegistrationMessage proto;
    CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

    processShiftbossRegisterationMessage(annotated_message.sender, proto.work_order_capacity());
    DCHECK_EQ(1u, shiftboss_directory_.size());
  }

  // Event loop
  for (;;) {
    // Receive() causes this thread to sleep until next message is received.
    const AnnotatedMessage annotated_message =
        bus_->Receive(foreman_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    const tmb::message_type_id message_type = tagged_message.message_type();
    LOG(INFO) << "ForemanDistributed received typed '" << message_type
              << "' message from client " << annotated_message.sender;
    switch (message_type) {
      case kShiftbossRegistrationMessage: {
        S::ShiftbossRegistrationMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processShiftbossRegisterationMessage(annotated_message.sender, proto.work_order_capacity());
        break;
      }
      case kAdmitRequestMessage: {
        AdmitRequestMessage *request_message =
            const_cast<AdmitRequestMessage*>(
                static_cast<const AdmitRequestMessage*>(tagged_message.message()));

        vector<QueryHandle *> *query_handles = request_message->getQueryHandlesMutable();
        DCHECK(!query_handles->empty());

        for (QueryHandle *handle : *query_handles) {
          handle->setClientId(annotated_message.sender);
        }

        bool all_queries_admitted = true;
        if (query_handles->size() == 1u) {
          all_queries_admitted =
              policy_enforcer_->admitQuery(query_handles->front());
        } else {
          all_queries_admitted = policy_enforcer_->admitQueries(*query_handles);
        }
        if (!all_queries_admitted) {
          LOG(WARNING) << "The scheduler could not admit all the queries";
          // TODO(harshad) - Inform the main thread about the failure.
        }
        break;
      }
      case kQueryInitiateResponseMessage: {
        // TODO(zuyu): check the query id.
        break;
      }
      case kCatalogRelationNewBlockMessage:  // Fall through
      case kDataPipelineMessage:
      case kRebuildWorkOrderCompleteMessage:
      case kWorkOrderCompleteMessage:
      case kWorkOrderFeedbackMessage:
      case kWorkOrdersAvailableMessage: {
        policy_enforcer_->processMessage(tagged_message);
        break;
      }
      case kInitiateRebuildResponseMessage: {
        // A unique case in the distributed version.
        policy_enforcer_->processInitiateRebuildResponseMessage(tagged_message);
        break;
      }
      case kSaveQueryResultResponseMessage: {
        S::SaveQueryResultResponseMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processSaveQueryResultResponseMessage(proto.cli_id(), proto.relation_id());
        break;
      }
      case kPoisonMessage: {
        if (policy_enforcer_->hasQueries()) {
          LOG(WARNING) << "Foreman thread exiting while some queries are "
                          "under execution or waiting to be admitted";
        }

        // Shutdown all Shiftbosses.
        tmb::Address shiftboss_addresses;
        for (std::size_t i = 0; i < shiftboss_directory_.size(); ++i) {
          shiftboss_addresses.AddRecipient(shiftboss_directory_.getClientId(i));
        }

        tmb::MessageStyle broadcast_style;
        broadcast_style.Broadcast(true);

        TaggedMessage poison_message(kPoisonMessage);

        const tmb::MessageBus::SendStatus send_status =
            bus_->Send(foreman_client_id_,
                       shiftboss_addresses,
                       broadcast_style,
                       move(poison_message));
        DCHECK(send_status == tmb::MessageBus::SendStatus::kOK);
        return;
      }
      default:
        LOG(FATAL) << "Unknown message type to Foreman";
    }

    if (canCollectNewMessages(message_type)) {
      vector<unique_ptr<S::WorkOrderMessage>> new_messages;
      policy_enforcer_->getWorkOrderMessages(&new_messages);
      dispatchWorkOrderMessages(new_messages);
    }
  }
}

bool ForemanDistributed::canCollectNewMessages(const tmb::message_type_id message_type) {
  return !QUICKSTEP_EQUALS_ANY_CONSTANT(message_type,
                                        kCatalogRelationNewBlockMessage,
                                        kWorkOrderFeedbackMessage) &&
         // TODO(zuyu): Multiple Shiftbosses support.
         !shiftboss_directory_.hasReachedCapacity(0);
}

void ForemanDistributed::dispatchWorkOrderMessages(const vector<unique_ptr<S::WorkOrderMessage>> &messages) {
  for (const auto &message : messages) {
    DCHECK(message != nullptr);
    // TODO(zuyu): Multiple Shiftbosses support.
    sendWorkOrderMessage(0, *message);
    shiftboss_directory_.incrementNumQueuedWorkOrders(0);
  }
}

void ForemanDistributed::sendWorkOrderMessage(const size_t shiftboss_index,
                                              const S::WorkOrderMessage &proto) {
  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kWorkOrderMessage);
  free(proto_bytes);

  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         shiftboss_directory_.getClientId(shiftboss_index),
                                         move(message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
      << "Message could not be sent from Foreman with TMB client ID " << foreman_client_id_
      << " to Shiftboss with TMB client ID " << shiftboss_directory_.getClientId(shiftboss_index);
}

void ForemanDistributed::printWorkOrderProfilingResults(const std::size_t query_id,
                                                        std::FILE *out) const {
  const std::vector<
      std::tuple<std::size_t, std::size_t, std::size_t>>
          &recorded_times = policy_enforcer_->getProfilingResults(query_id);
  fputs("Query ID,Worker ID,Operator ID,Time (microseconds)\n", out);
  for (const auto &workorder_entry : recorded_times) {
    // Note: Index of the "worker thread index" in the tuple is 0.
    const std::size_t worker_id = std::get<0>(workorder_entry);
    fprintf(out,
            "%lu,%lu,%lu,%lu\n",
            query_id,
            worker_id,
            std::get<1>(workorder_entry),  // Operator ID.
            std::get<2>(workorder_entry));  // Time.
  }
}

void ForemanDistributed::processShiftbossRegisterationMessage(const client_id shiftboss_client_id,
                                                              const std::size_t work_order_capacity) {
  shiftboss_directory_.addShiftboss(shiftboss_client_id, work_order_capacity);

  S::ShiftbossRegistrationResponseMessage proto;

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kShiftbossRegistrationResponseMessage);
  free(proto_bytes);

  LOG(INFO) << "ForemanDistributed sent ShiftbossRegistrationResponseMessage (typed '"
            << kShiftbossRegistrationResponseMessage
            << "') to Shiftboss with TMB client id " << shiftboss_client_id;
  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     shiftboss_client_id,
                                     move(message));
}

void ForemanDistributed::processSaveQueryResultResponseMessage(const tmb::client_id cli_id,
                                                               const relation_id result_relation_id) {
  S::QueryExecutionSuccessMessage proto;
  proto.mutable_result_relation()->MergeFrom(
      static_cast<CatalogDatabase*>(catalog_database_)->getRelationById(result_relation_id)->getProto());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kQueryExecutionSuccessMessage);
  free(proto_bytes);

  // Notify the CLI regarding the query result.
  LOG(INFO) << "ForemanDistributed sent QueryExecutionSuccessMessage (typed '" << kQueryExecutionSuccessMessage
            << "') to CLI with TMB client id " << cli_id;
  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     cli_id,
                                     move(message));
}

}  // namespace quickstep
