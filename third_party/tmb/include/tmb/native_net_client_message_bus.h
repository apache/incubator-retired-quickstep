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

#ifndef TMB_NATIVE_NET_CLIENT_MESSAGE_BUS_H_
#define TMB_NATIVE_NET_CLIENT_MESSAGE_BUS_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/tmb_net.grpc.pb.h"

namespace tmb {

namespace internal {
class ContainerPusher;
template <typename T> class IteratorAdapter;
}  // namespace internal

class Address;
class CancellationToken;
class MessageStyle;

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief A MessageBus that represents a client-side connection to a native TMB
 *        server. Suitable for communication across networks.
 **/
class NativeNetClientMessageBus : public MessageBus {
 public:
  NativeNetClientMessageBus() {
  }

  ~NativeNetClientMessageBus() override {
  }

  void AddServer(const std::string &hostname,
                 const std::uint16_t port) override;

  bool Initialize() override;

  void ResetBus() override;

  client_id Connect() override;

  bool Disconnect(const client_id client) override;

  bool RegisterClientAsSender(const client_id sender_id,
                              const message_type_id message_type) override;

  bool RegisterClientAsReceiver(const client_id receiver_id,
                                const message_type_id message_type) override;

  MessageBus::SendStatus Send(
      const client_id sender_id,
      const Address &destination_address,
      const MessageStyle &style,
      TaggedMessage &&message,  // NOLINT(whitespace/operators)
      const Priority priority = kDefaultPriority,
      CancellationToken *cancellation_token = nullptr) override;

  void CancelMessage(const client_id sender_id,
                     const CancellationToken &cancellation_token) override;

  std::size_t CountQueuedMessagesForClient(const client_id receiver_id)
      override;

 protected:
  std::size_t ReceiveIfAvailableImpl(const client_id receiver_id,
                                     const Priority minimum_priority,
                                     const std::size_t max_messages,
                                     const bool delete_immediately,
                                     internal::ContainerPusher *pusher)
                                         override;

  void DeleteImpl(const client_id receiver_id,
                  internal::IteratorAdapter<const AnnotatedMessage> *adapter)
                      override;

  void CancelImpl(const client_id receiver_id,
                  internal::IteratorAdapter<const AnnotatedMessage> *adapter)
                      override;

 private:
  std::string server_address_;
  std::shared_ptr<internal::net::MessageBus::Stub> stub_;

  // Disallow copy and assign:
  NativeNetClientMessageBus(const NativeNetClientMessageBus &orig) = delete;
  NativeNetClientMessageBus& operator=(
      const NativeNetClientMessageBus &rhs) = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_NATIVE_NET_CLIENT_MESSAGE_BUS_H_
