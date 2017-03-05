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

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_UTIL_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_UTIL_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <utility>

#include "query_execution/AdmitRequestMessage.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/QueryOptimizerConfig.h"  // For QUICKSTEP_DISTRIBUTED.
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

namespace quickstep {

class QueryHandle;

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A static class for reusable methods in query_execution module.
 **/
class QueryExecutionUtil {
 public:
  static std::string MessageTypeToString(const tmb::message_type_id message_type) {
    switch (message_type) {
      case kAdmitRequestMessage:                          return "AdmitRequestMessage";
      case kWorkOrderMessage:                             return "WorkOrderMessage";
      case kWorkOrderCompleteMessage:                     return "WorkOrderCompleteMessage";
      case kCatalogRelationNewBlockMessage:               return "CatalogRelationNewBlockMessage";
      case kDataPipelineMessage:                          return "DataPipelineMessage";
      case kWorkOrderFeedbackMessage:                     return "WorkOrderFeedbackMessage";
      case kRebuildWorkOrderMessage:                      return "RebuildWorkOrderMessage";
      case kRebuildWorkOrderCompleteMessage:              return "RebuildWorkOrderCompleteMessage";
      case kWorkloadCompletionMessage:                    return "WorkloadCompletionMessage";
      case kPoisonMessage:                                return "PoisonMessage";
#ifdef QUICKSTEP_DISTRIBUTED
      case kShiftbossRegistrationMessage:                 return "ShiftbossRegistrationMessage";
      case kShiftbossRegistrationResponseMessage:         return "ShiftbossRegistrationResponseMessage";
      case kDistributedCliRegistrationMessage:            return "DistributedCliRegistrationMessage";
      case kDistributedCliRegistrationResponseMessage:    return "DistributedCliRegistrationResponseMessage";
      case kSqlQueryMessage:                              return "SqlQueryMessage";
      case kQueryInitiateMessage:                         return "QueryInitiateMessage";
      case kQueryInitiateResponseMessage:                 return "QueryInitiateResponseMessage";
      case kInitiateRebuildMessage:                       return "InitiateRebuildMessage";
      case kInitiateRebuildResponseMessage:               return "InitiateRebuildResponseMessage";
      case kQueryTeardownMessage:                         return "QueryTeardownMessage";
      case kQueryExecutionSuccessMessage:                 return "QueryExecutionSuccessMessage";
      case kCommandResponseMessage:                       return "CommandResponseMessage";
      case kQueryExecutionErrorMessage:                   return "QueryExecutionErrorMessage";
      case kQueryResultTeardownMessage:                   return "QueryResultTeardownMessage";
      case kBlockDomainRegistrationMessage:               return "BlockDomainRegistrationMessage";
      case kBlockDomainRegistrationResponseMessage:       return "BlockDomainRegistrationResponseMessage";
      case kBlockDomainToShiftbossIndexMessage:           return "BlockDomainToShiftbossIndexMessage";
      case kAddBlockLocationMessage:                      return "AddBlockLocationMessage";
      case kDeleteBlockLocationMessage:                   return "DeleteBlockLocationMessage";
      case kLocateBlockMessage:                           return "LocateBlockMessage";
      case kLocateBlockResponseMessage:                   return "LocateBlockResponseMessage";
      case kGetPeerDomainNetworkAddressesMessage:         return "GetPeerDomainNetworkAddressesMessage";
      case kGetPeerDomainNetworkAddressesResponseMessage: return "GetPeerDomainNetworkAddressesResponseMessage";
      case kBlockDomainUnregistrationMessage:             return "BlockDomainUnregistrationMessage";
#endif  // QUICKSTEP_DISTRIBUTED
      default:
        LOG(FATAL) << "Unknown message type";
    }
  }

  /**
   * @brief Send a TMB message to a single receiver.
   *
   * @param bus A pointer to the TMB.
   * @param sender_id The client ID of the sender.
   * @param receiver_id The client ID of the receiver.
   * @param tagged_message A moved-from reference to the tagged message.
   *
   * @return A status code indicating the result of the message getting sent.
   *         The caller should ensure that the status is SendStatus::kOK.
   **/
  static tmb::MessageBus::SendStatus SendTMBMessage(
      tmb::MessageBus *bus,
      tmb::client_id sender_id,
      tmb::client_id receiver_id,
      tmb::TaggedMessage &&tagged_message) {  // NOLINT(whitespace/operators)
    tmb::Address receiver_address;
    receiver_address.AddRecipient(receiver_id);

    tmb::MessageStyle single_receiver_style;
    return bus->Send(sender_id,
                     receiver_address,
                     single_receiver_style,
                     std::move(tagged_message));
  }

  /**
   * @brief Construct and send an AdmitRequestMessage from a given sender to a
   *        given recipient.
   *
   * @param sender_id The TMB client ID of the sender.
   * @param receiver_id The TMB client ID of the receiver.
   * @param query_handle The QueryHandle used in the AdmitRequestMessage.
   * @param bus A pointer to the TMB.
   * @param tagged_message A moved from reference to the tagged message.
   *
   * @return A status code indicating the result of the message delivery.
   *         The caller should ensure that the status is SendStatus::kOK.
   **/
  static tmb::MessageBus::SendStatus ConstructAndSendAdmitRequestMessage(
      const tmb::client_id sender_id,
      const tmb::client_id receiver_id,
      QueryHandle *query_handle,
      tmb::MessageBus *bus) {
    std::unique_ptr<AdmitRequestMessage> request_message(
        new AdmitRequestMessage(query_handle));
    const std::size_t size_of_request_msg = sizeof(*request_message);
    tmb::TaggedMessage admit_tagged_message(
        request_message.release(), size_of_request_msg, kAdmitRequestMessage);

    return QueryExecutionUtil::SendTMBMessage(
        bus, sender_id, receiver_id, std::move(admit_tagged_message));
  }

  /**
   * @brief Receive a query completion message.
   *
   * @param receiver_id The TMB client ID of the receiver thread.
   * @param bus A pointer to the TMB.
   *
   * @note Right now the query completion message is of no interest to the
   *       caller. In the future, if this message needs to be fetched, make this
   *       function return the TaggedMessage.
   **/
  static void ReceiveQueryCompletionMessage(const tmb::client_id receiver_id,
                                            tmb::MessageBus *bus) {
    const tmb::AnnotatedMessage annotated_msg =
        bus->Receive(receiver_id, 0, true);
    const tmb::TaggedMessage &tagged_message = annotated_msg.tagged_message;
    DCHECK_EQ(kWorkloadCompletionMessage, tagged_message.message_type());
  }

  static void BroadcastMessage(const tmb::client_id sender_id,
                               const tmb::Address &addresses,
                               tmb::TaggedMessage &&tagged_message,  // NOLINT(whitespace/operators)
                               tmb::MessageBus *bus) {
    // The sender broadcasts the given message to all 'addresses'.
    tmb::MessageStyle style;
    style.Broadcast(true);

    const tmb::MessageBus::SendStatus send_status =
        bus->Send(sender_id, addresses, style, std::move(tagged_message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK);
  }

  static void BroadcastPoisonMessage(const tmb::client_id sender_id, tmb::MessageBus *bus) {
    // Terminate all threads.
    // The sender thread broadcasts poison message to the workers and foreman.
    // Each worker dies after receiving poison message. The order of workers'
    // death is irrelavant.
    tmb::MessageStyle style;
    style.Broadcast(true);
    tmb::Address address;
    address.All(true);
    tmb::TaggedMessage poison_tagged_message(kPoisonMessage);

    DLOG(INFO) << "Client " << sender_id << " broadcasts PoisonMessage to all";
    const tmb::MessageBus::SendStatus send_status = bus->Send(
        sender_id, address, style, std::move(poison_tagged_message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK);
  }

 private:
  /**
   * @brief Constructor. Made private to avoid instantiation.
   **/
  QueryExecutionUtil();

  DISALLOW_COPY_AND_ASSIGN(QueryExecutionUtil);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_UTIL_HPP_
