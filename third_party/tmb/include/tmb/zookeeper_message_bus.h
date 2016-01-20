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

#ifndef TMB_ZOOKEEPER_MESSAGE_BUS_H_
#define TMB_ZOOKEEPER_MESSAGE_BUS_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/lock_free_garbage_collector.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/zookeeper_receiver_context.h"

struct _zhandle;
struct String_vector;

namespace tmb {

namespace internal {
class ContainerPusher;
template <typename T> class IteratorAdapter;
class MemoryMirrorDeleteBatch;
class QueuedMessage;
template <typename T> class ThreadsafeSet;
class ZookeeperTransactionBatch;
}  // namespace internal

class Address;
class CancellationToken;
class MessageStyle;

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief An implementation of MessageBus backed by an Apache Zookeeper server
 *        or servers. Suitable for communication across networks.
 **/
class ZookeeperMessageBus : public MessageBus,
                            public internal::PersistentBusStateInterface {
 public:
  /**
   * @brief Constructor.
   *
   * @param path_prefix The path in the Zookeeper data tree where a particular
   *        TMB instance should store its data. Multiple TMB instances can
   *        coexist with each other, and with other applications, sharing the
   *        same Zookeeper instance, so long as they have different paths.
   **/
  explicit ZookeeperMessageBus(const std::string &path_prefix);

  ~ZookeeperMessageBus();

  void AddServer(const std::string &hostname, const std::uint16_t port)
       override;

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

  // PersistentBusStateInterface implementations.
  void ResetBusUnchecked() override {
    ResetBus();
  }

  client_id ConnectClientUnchecked() override {
    return Connect();
  }

  bool DisconnectRequiresQueueDrain() const override {
    return true;
  }

  void DisconnectClientUnchecked(
      const client_id client,
      const internal::ThreadsafeSet<message_type_id> &sendable_set,
      const internal::ThreadsafeSet<message_type_id> &receivable_set,
      const internal::MemoryMirrorDeleteBatch *queued_message_batch) override;

  void InsertSendableUnchecked(const client_id client,
                               const message_type_id message_type) override {
    RegisterClientAsSender(client, message_type);
  }

  void InsertMessageTypeUnchecked(
      const message_type_id message_type) override;

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
  std::size_t ReceiveImpl(const client_id receiver_id,
                          const Priority minimum_priority,
                          const std::size_t max_messages,
                          const bool delete_immediately,
                          internal::ContainerPusher *pusher) override;

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
  // When sorting children of a queue node, if N nodes are the minimum
  // required, then sort N + (N >> kExtraSortShift) at first so that there are
  // some "extra" sorted nodes in case of expired or cancelled messages. Note
  // that this is purely a heuristic optimization, and in case all of the
  // sorted children are exhausted before N is satisfied, then additional
  // children will be sorted as needed.
  static const std::uint8_t kExtraSortShift = 3;

  // Callback for asynchronous events related to the zookeeper connection
  // itself.
  static void HandleConnectionEvent(_zhandle *connection_handle,
                                    int event_type,
                                    int connection_state,
                                    const char *node_path,
                                    void *watcher_context);

  // Zookeeper-compatible callback which casts '*watcher_context' to a
  // ZookeeperReceiverContext and invokes HandleQueueChange().
  static void QueueWatchCallback(_zhandle *connection_handle,
                                 int event_type,
                                 int connection_state,
                                 const char *node_path,
                                 void *watcher_context);

  // Add operations to '*batch' to recursively delete 'path' and all of its
  // children. Deletion will not actually occur until '*batch' is committed.
  void RecursivelyDeletePath(const std::string &path,
                             internal::ZookeeperTransactionBatch *batch);

  // Add operations to '*batch' to delete any entries for 'client' in the
  // receiver directory.
  void RemoveReceiverDirectoryEntries(
      const client_id client,
      internal::ZookeeperTransactionBatch *batch);

  // Helper method for ReceiveImpl(), ReceiveIfAvailableImpl(), and
  // HandleQueueChange() which sorts and parses '*queue_node_children' under
  // '*queue_node_path' and attempts to fetch valid messages, optionally
  // deleting them according to 'delete_immediately'. Returns the number of
  // messages successfully received and pushed onto '*pusher'. Also deallocates
  // all strings in '*queue_node_children'.
  std::size_t ReceiveInternal(const client_id receiver_id,
                              const Priority minimum_priority,
                              const std::size_t max_messages,
                              const bool delete_immediately,
                              const char *queue_node_path,
                              internal::ContainerPusher *pusher,
                              String_vector *queue_node_children);

  // Invoked by a callback when the set of children under '*queue_node_path'
  // changes, which may possibly indicate that a receivable message has been
  // enqueued. This method will invoke ReceiveInternal and wake up the thread
  // waiting in ReceiveImpl() if at least one message has successfully been
  // received.
  void HandleQueueChange(const char *queue_node_path,
                         internal::ZookeeperReceiverContext *context);

  // Retrieves the set of receivers for the message as '*message_path' and
  // attempts to cancel the message by removing it from each receiver's queue.
  // Effectively a no-op if the message was not originally created as
  // cancellable, or was already cancelled. Note that this merely adds delete
  // operations to '*batch' which will not actually take place until '*batch'
  // is committed.
  void CancelInternal(const char *message_path,
                      internal::ZookeeperTransactionBatch *batch);

  const std::string path_prefix_;
  std::string client_basepath_;
  std::string receiver_directory_basepath_;
  std::string message_basepath_;
  std::string queue_basepath_;

  std::string host_string_;

  _zhandle *zookeeper_handle_;

  // A set of "leftover" contexts that still have a spurious watch set for
  // them, and should be deleted to avoid leaking memory if the watch callbacks
  // do not fire before this ZookeeperMessageBus is destroyed.
  //
  // TODO(chasseur): Once support for removing watches comes to the Zookeeper
  // C API, revisit this.
  internal::LockFreeGarbageCollector<internal::ZookeeperReceiverContext>
      dead_watch_contexts_;

  // Disallow copy and assign:
  ZookeeperMessageBus(const ZookeeperMessageBus &orig) = delete;
  ZookeeperMessageBus& operator=(const ZookeeperMessageBus &rhs) = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_ZOOKEEPER_MESSAGE_BUS_H_
