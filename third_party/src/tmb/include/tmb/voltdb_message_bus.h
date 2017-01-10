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

#ifndef TMB_VOLTDB_MESSAGE_BUS_H_
#define TMB_VOLTDB_MESSAGE_BUS_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/net_message_removal_interface.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/voltdb_connection_pool.h"
#include "tmb/internal/voltdb_procedure_warehouse.h"

namespace tmb {

namespace internal {
class ContainerPusher;
template <typename T> class IteratorAdapter;
class MemoryMirrorDeleteBatch;
class QueuedMessage;
template <typename T> class ThreadsafeSet;
}  // namespace internal

class Address;
class CancellationToken;
class MessageStyle;

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief An implementation of MessageBus which uses a VoltDB database to
 *        implement persistence and transactions. Suitable for communication
 *        across networks.
 **/
class VoltDBMessageBus : public MessageBus,
                         public internal::NetMessageRemovalInterface,
                         public internal::PersistentBusStateInterface {
 public:
  /**
   * @brief Construct a VoltDBMessageBus with the default client parameters
   *        (i.e. a blank username and password).
   * @warning This VoltDBMessageBus will not be usable until after at least one
   *          call to AddServer() followed by a call to Initialize().
   **/
  VoltDBMessageBus() {
  }

  /**
   * @brief Construct a VoltDBMessageBus which will supply the specified
   *        username and password when connecting to VoltDB servers.
   * @warning This VoltDBMessageBus will not be usable until after at least one
   *          call to AddServer() followed by a call to Initialize().
   *
   * @param username The username to provide when connecting to VoltDB servers.
   * @param password The password to provide when connecting to VoltDB servers.
   **/
  VoltDBMessageBus(const std::string &username, const std::string &password)
      : connection_pool_(username, password) {
  }

  ~VoltDBMessageBus() {
  }

  inline void AddServer(const std::string &hostname,
                        const std::uint16_t port) override {
    connection_pool_.AddServer(hostname, port);
  }

  inline bool Initialize() override {
    return connection_pool_.Initialize();
  }

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

  bool SupportsNetMessageRemovalInterface() const override {
    return true;
  }

  internal::NetMessageRemovalInterface* GetNetMessageRemovalInterface()
      override {
    return this;
  }

  // NetMessageRemovalInterface implementations:
  void DeleteById(
      const client_id receiver_id,
      const std::vector<std::int64_t> &message_ids) override;

  void SenderCancelById(
      const client_id sender_id,
      const std::vector<std::int64_t> &message_ids) override;

  void ReceiverCancelById(
      const client_id receiver_id,
      const std::vector<std::int64_t> &message_ids) override;

  // PersistentBusStateInterface implementations:
  void ResetBusUnchecked() override {
    ResetBus();
  }

  client_id ConnectClientUnchecked() override {
    return Connect();
  }

  bool DisconnectRequiresQueueDrain() const override {
    return false;
  }

  void DisconnectClientUnchecked(
      const client_id client,
      const internal::ThreadsafeSet<message_type_id> &sendable_set,
      const internal::ThreadsafeSet<message_type_id> &receivable_set,
      const internal::MemoryMirrorDeleteBatch *queued_message_batch) override {
    Disconnect(client);
  }

  void InsertSendableUnchecked(const client_id client,
                               const message_type_id message_type) override;

  void InsertMessageTypeUnchecked(
      const message_type_id message_type) override {
  }

  void InsertReceivableUnchecked(
      const client_id client,
      const message_type_id message_type,
      const internal::ThreadsafeSet<message_type_id> &receivable_set) override;

  // This implementation ignores '*sender_message_counter'.
  std::int64_t SendUnchecked(
      const internal::QueuedMessage &message,
      const std::vector<client_id> &verified_receivers,
      CancellationToken *cancellation_token,
      std::uint32_t *sender_message_counter) override;

  internal::MemoryMirrorDeleteBatch* CreateDeleteBatch(
      const client_id receiver_id) const override;

  void DeleteMessagesUnchecked(
      const internal::MemoryMirrorDeleteBatch &delete_batch) override;

  void CancelMessageUnchecked(
      const CancellationToken &cancellation_token) override;

  void CancelMessagesUnchecked(
      internal::IteratorAdapter<const AnnotatedMessage> *adapter) override;

  std::unordered_map<client_id, TempClientHandle>* LoadState() override;

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
  typedef internal::VoltDBProcedureWarehouse::ProcedureID ProcedureID;
  typedef internal::VoltDBProcedureWarehouse::ProcedureHandle ProcedureHandle;

  internal::VoltDBConnectionPool connection_pool_;
  internal::VoltDBProcedureWarehouse warehouse_;

  // Disallow copy and assign:
  VoltDBMessageBus(const VoltDBMessageBus &orig) = delete;
  VoltDBMessageBus& operator=(const VoltDBMessageBus &rhs) = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_VOLTDB_MESSAGE_BUS_H_
