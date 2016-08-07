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

#ifndef TMB_INTERNAL_MEMORY_BASED_MESSAGE_BUS_H_
#define TMB_INTERNAL_MEMORY_BASED_MESSAGE_BUS_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"

#include "tmb/internal/heap_receiver_message_queue.h"
#include "tmb/internal/rcu.h"
#include "tmb/internal/threadsafe_set.h"
#include "tmb/internal/tree_receiver_message_queue.h"

namespace tmb {

class Address;
class MessageStyle;

namespace internal {

class QueuedMessage;

/**
 * @brief Abstract base class which implements functionality common to
 *        PureMemoryMessageBus and MemoryMirrorMessageBus.
 **/
template <bool enable_deletion_support, bool memory_mirror>
class MemoryBasedMessageBus : public MessageBus {
 public:
  struct ClientHandle {
    ClientHandle() : sent_message_counter_(0) {
    }

    ClientHandle(std::unordered_set<message_type_id> *sendable_types,
                 std::unordered_set<message_type_id> *receivable_types,
                 const std::uint32_t sent_message_counter)
        : sendable_message_types_(sendable_types),
          receivable_message_types_(receivable_types),
          sent_message_counter_(sent_message_counter) {
    }

    ThreadsafeSet<message_type_id> sendable_message_types_;
    ThreadsafeSet<message_type_id> receivable_message_types_;
    typename std::conditional<enable_deletion_support,
                              TreeReceiverMessageQueue<memory_mirror>,
                              HeapReceiverMessageQueue<memory_mirror>>::type
        incoming_messages_;

    // This is used, together with sender ID, to generate unique message IDs
    // until it overflows, at which point we give up and resort to a global
    // atomic counter.
    std::uint32_t sent_message_counter_;

    // When net support is enabled, these are used to lookup cancellation
    // tokens by ID. Otherwise they are unused.
    //
    // NOTE(chasseur): The message ID is also stored in the token itself. We
    // could use a simple custom hasher class and an unordered_set to avoid
    // storing an extra copy, but doing things this way makes it simpler to
    // look up a token using only an ID.
    //
    // FIXME(chasseur): These are not properly repopulated when reloading a
    // MemoryMirror from persistent storage, so it may be possible for a server
    // to go down and then recover while clients stay up, after which the
    // clients may attempt to cancel messages they received before the failure,
    // which will then not actually be cancelled.
    std::unordered_map<std::int64_t, CancellationToken> send_cancel_tokens_;
    std::unordered_map<std::int64_t, CancellationToken> receive_cancel_tokens_;
  };

  explicit MemoryBasedMessageBus(const bool enable_net_support)
      : net_support_enabled_(enable_net_support),
        clients_(new std::unordered_map<client_id,
                                        std::shared_ptr<ClientHandle>>()),
        receiver_directory_(
            new std::unordered_map<
                message_type_id,
                std::shared_ptr<ThreadsafeSet<client_id>>>()) {
  }

  virtual ~MemoryBasedMessageBus() {
  }

  std::size_t CountQueuedMessagesForClient(const client_id receiver_id)
      override;

 protected:
  typedef internal::RCU<
              std::unordered_map<client_id, std::shared_ptr<ClientHandle>>>
          ClientMap;

  typedef internal::RCU<
              std::unordered_map<
                  message_type_id,
                  std::shared_ptr<tmb::internal::ThreadsafeSet<client_id>>>>
          ReceiverDirectory;

  enum class RegistrationState {
    kNotConnected,
    kNotRegistered,
    kRegistered
  };

  // TODO(chasseur): Currently we are very aggressive about calling this to
  // garbage-collect dead tokens: if net support is enabled, it is called every
  // time a cancellable message is sent (for the sender's handle), every time a
  // message is cancelled (for the caller's handle, either send-side or
  // receive-side), and every time messages are received or explicitly deleted
  // (for the receiver's handle). This is very effective at reclaiming memory
  // and keeping the lookup tables of CancellationTokens small, but it may be
  // overkill and eat up CPU cycles unnecessarily. We might consider
  // implementing a lazier policy where dead tokens are purged only every N
  // calls (for some reasonably tuned value of N), or only when the size of
  // 'send_cancel_tokens_' and 'receive_cancel_tokens_' for a handle exceeds a
  // certain threshold.
  static void PurgeDeadCancelTokens(ClientHandle *handle);

  void InsertClientHandle(const client_id client);

  std::shared_ptr<ClientHandle> RemoveClientHandle(const client_id client);

  ClientHandle* LookupClientHandle(const client_id client) const;

  RegistrationState CheckSenderRegistered(
      const typename ClientMap::ReadHandle &read_handle,
      const client_id client,
      const message_type_id message_type,
      typename ClientMap::value_type::const_iterator *it) const;

  RegistrationState CheckReceiverRegistered(
      const typename ClientMap::ReadHandle &read_handle,
      const client_id client,
      const message_type_id message_type,
      typename ClientMap::value_type::const_iterator *it) const;

  // Returns true if new set may have been created for 'message_type', false if
  // set already existed. Note that in cases where two callers call this method
  // for the same 'message_type' which was not present in ReceiverDirectory at
  // the same time, this method may return 'true' to both callers. This is an
  // intentional quirk so that MemoryMirrorMessageBus will always make sure
  // that PersistentMessageBus::InsertMessageTypeUnchecked() is completed
  // before PersistentMessageBus::InsertReceivableUnchecked() is called.
  bool GetReceiversForType(const message_type_id message_type,
                           ThreadsafeSet<client_id> **receivers);

  MessageBus::SendStatus FinalizeReceivers(
      const typename ClientMap::ReadHandle &clients_read_handle,
      const Address &destination_address,
      const MessageStyle &style,
      const message_type_id message_type,
      std::vector<client_id> *receivers) const;

  void FinishSend(
      const typename ClientMap::ReadHandle &clients_read_handle,
      const std::vector<client_id> &receivers,
      QueuedMessage &&msg) const;  // NOLINT(whitespace/operators)

  const bool net_support_enabled_;
  ClientMap clients_;
  ReceiverDirectory receiver_directory_;

 private:
  // Disallow copy and assign:
  MemoryBasedMessageBus(const MemoryBasedMessageBus &orig) = delete;
  MemoryBasedMessageBus& operator=(const MemoryBasedMessageBus &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_MEMORY_BASED_MESSAGE_BUS_H_
