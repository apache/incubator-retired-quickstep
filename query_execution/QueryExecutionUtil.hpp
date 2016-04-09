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

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_UTIL_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_UTIL_HPP_

#include <memory>
#include <utility>

#include "query_execution/AdmitRequestMessage.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "utility/Macros.hpp"

#include "tmb/address.h"
#include "tmb/message_style.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {

/**
 * @brief A static class for reusable methods in query_execution module.
 **/
class QueryExecutionUtil {
 public:
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
      MessageBus *bus,
      client_id sender_id,
      client_id receiver_id,
      TaggedMessage &&tagged_message) {  // NOLINT(whitespace/operators)
    Address receiver_address;
    receiver_address.AddRecipient(receiver_id);

    MessageStyle single_receiver_style;
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
      MessageBus *bus) {
    std::unique_ptr<AdmitRequestMessage> request_message(
        new AdmitRequestMessage(query_handle));
    const std::size_t size_of_request_msg = sizeof(*request_message);
    TaggedMessage admit_tagged_message(
        request_message.release(), size_of_request_msg, kAdmitRequestMessage);

    return QueryExecutionUtil::SendTMBMessage(
        bus, sender_id, receiver_id, std::move(admit_tagged_message));
  }

  static void BroadcastPoisonMessage(const tmb::client_id sender_id, tmb::MessageBus *bus) {
    // Terminate all threads.
    // The sender thread broadcasts poison message to the workers and foreman.
    // Each worker dies after receiving poison message. The order of workers'
    // death is irrelavant.
    MessageStyle style;
    style.Broadcast(true);
    Address address;
    address.All(true);
    std::unique_ptr<WorkerMessage> poison_message(WorkerMessage::PoisonMessage());
    TaggedMessage poison_tagged_message(poison_message.get(),
                                        sizeof(*poison_message),
                                        kPoisonMessage);

    const tmb::MessageBus::SendStatus send_status = bus->Send(
        sender_id, address, style, std::move(poison_tagged_message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK) <<
       "Broadcast poison message from sender with TMB client ID " << sender_id
       << " failed";
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
