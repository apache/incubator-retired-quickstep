//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef TMB_SQLITE_MESSAGE_BUS_H_
#define TMB_SQLITE_MESSAGE_BUS_H_

#include <cstddef>
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
#include "tmb/internal/sqlite_connection_pool.h"

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
 * @brief An implementation of MessageBus which uses a SQLite database to
 *        implement persistence and transactions.
 **/
class SQLiteMessageBus : public MessageBus,
                         public internal::NetMessageRemovalInterface,
                         public internal::PersistentBusStateInterface {
 public:
  /**
   * @brief Constructor.
   *
   * @param database_filename The filename of the SQLite database to use.
   * @param use_wal If true, use a write-ahead log for the database journal.
   *        This is only supported with SQLite 3.7.0 or newer, and this option
   *        has no effect when using an older SQLite. WAL is MUCH faster and
   *        than the old rollback journal, but it uses shared memory and
   *        requires that all of the processes sharing a database reside on the
   *        same machine (i.e. it may break with a network filesystem). In
   *        general, it is always preferred to use WAL on a single node, but it
   *        should be disabled when the database file is on a network file
   *        system.
   **/
  explicit SQLiteMessageBus(const std::string &database_filename,
                            const bool use_wal = true)
      : connection_pool_(database_filename, use_wal) {
  }

  ~SQLiteMessageBus() {
  }

  bool Initialize() override {
    return connection_pool_.CheckConnection();
  }

  void ResetBus() override {
    connection_pool_.InitializeDatabase();
  }

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
      const std::vector<std::int64_t> &message_ids) override {
    DeleteInternal(receiver_id, message_ids);
  }

  void SenderCancelById(
      const client_id sender_id,
      const std::vector<std::int64_t> &message_ids) override {
    CancelInternal(message_ids);
  }

  void ReceiverCancelById(
      const client_id receiver_id,
      const std::vector<std::int64_t> &message_ids) override {
    CancelInternal(message_ids);
  }

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

  // No-op for SQLiteMessageBus.
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
  void DeleteInternal(const client_id receiver_id,
                      const std::vector<std::int64_t> &message_ids);

  void CancelInternal(const std::vector<std::int64_t> &message_ids);

  internal::SQLiteConnectionPool connection_pool_;

  // Disallow copy and assign:
  SQLiteMessageBus(const SQLiteMessageBus &orig) = delete;
  SQLiteMessageBus& operator=(const SQLiteMessageBus &rhs) = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_SQLITE_MESSAGE_BUS_H_
