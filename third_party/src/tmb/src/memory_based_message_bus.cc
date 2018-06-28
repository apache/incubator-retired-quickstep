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

#include "tmb/internal/memory_based_message_bus.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"

#include "tmb/internal/heap_receiver_message_queue.h"
#include "tmb/internal/queued_message.h"
#include "tmb/internal/rcu.h"
#include "tmb/internal/shared_bool.h"
#include "tmb/internal/tree_receiver_message_queue.h"

namespace tmb {
namespace internal {

namespace {

// Functor which erases entries for a given client from the directory of
// receivers.
class ReceiverDirectoryEraser {
 public:
  ReceiverDirectoryEraser(
      const client_id client,
      const std::unordered_map<message_type_id,
                               std::shared_ptr<ThreadsafeSet<client_id>>>
          &receiver_directory)
      : client_(client),
        receiver_directory_(receiver_directory) {
  }

  void operator()(const message_type_id message_type) {
    std::unordered_map<message_type_id,
                       std::shared_ptr<ThreadsafeSet<client_id>>>
        ::const_iterator it = receiver_directory_.find(message_type);
    assert(it != receiver_directory_.end());
    const_cast<ThreadsafeSet<client_id>*>(it->second.get())->erase(client_);
  }

 private:
  const client_id client_;
  const std::unordered_map<message_type_id,
                           std::shared_ptr<ThreadsafeSet<client_id>>>
      &receiver_directory_;
};

}  // namespace

template <bool enable_deletion_support, bool memory_mirror>
std::size_t MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
    ::CountQueuedMessagesForClient(const client_id receiver_id) {
  typename ClientMap::ReadHandle read_handle = clients_.GetReadHandle();
  typename ClientMap::value_type::const_iterator it
      = read_handle->find(receiver_id);
  if (it == read_handle->end()) {
    return 0;
  } else {
    return it->second->incoming_messages_.Length();
  }
}

template <bool enable_deletion_support, bool memory_mirror>
void MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
    ::PurgeDeadCancelTokens(ClientHandle *handle) {
  std::unordered_map<std::int64_t, CancellationToken>::iterator it
      = handle->send_cancel_tokens_.begin();
  while (it != handle->send_cancel_tokens_.end()) {
    if (it->second.in_memory_cancel_flag_.Get()
        || it->second.in_memory_cancel_flag_.OnlyCopy()) {
      it = handle->send_cancel_tokens_.erase(it);
    } else {
      ++it;
    }
  }

  it = handle->receive_cancel_tokens_.begin();
  while (it != handle->receive_cancel_tokens_.end()) {
    if (it->second.in_memory_cancel_flag_.Get()
        || it->second.in_memory_cancel_flag_.OnlyCopy()) {
      it = handle->receive_cancel_tokens_.erase(it);
    } else {
      ++it;
    }
  }
}

template <bool enable_deletion_support, bool memory_mirror>
void MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
    ::InsertClientHandle(const client_id client) {
  typename ClientMap::WriteHandle write_handle = clients_.GetWriteHandle();
  write_handle->emplace(client,
                        std::shared_ptr<ClientHandle>(new ClientHandle()));
  write_handle.Commit();
}

template <bool enable_deletion_support, bool memory_mirror>
std::shared_ptr<typename MemoryBasedMessageBus<enable_deletion_support,
                                               memory_mirror>::ClientHandle>
    MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
        ::RemoveClientHandle(const client_id client) {
  std::shared_ptr<ClientHandle> retval;

  typename ClientMap::ReadHandle clients_read_handle
      = clients_.GetReadHandle();
  typename ClientMap::value_type::const_iterator it
      = clients_read_handle->find(client);
  if (it == clients_read_handle->end()) {
    return std::move(retval);
  }

  if (!it->second->receivable_message_types_.empty()) {
    ReceiverDirectory::ReadHandle receiver_directory_handle
        = receiver_directory_.GetReadHandle();
    it->second->receivable_message_types_.for_each(
        ReceiverDirectoryEraser(client, *receiver_directory_handle));
  }
  clients_read_handle.release();

  typename ClientMap::WriteHandle clients_write_handle
      = clients_.GetWriteHandle();
  typename ClientMap::value_type::iterator write_it
      = clients_write_handle->find(client);
  retval = write_it->second;
  clients_write_handle->erase(write_it);
  clients_write_handle.Commit();

  return std::move(retval);
}

template <bool enable_deletion_support, bool memory_mirror>
typename MemoryBasedMessageBus<enable_deletion_support,
                               memory_mirror>::ClientHandle*
    MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
        ::LookupClientHandle(const client_id client) const {
  typename ClientMap::ReadHandle clients_handle = clients_.GetReadHandle();
  typename ClientMap::value_type::const_iterator it
      = clients_handle->find(client);
  if (it == clients_handle->end()) {
    return nullptr;
  } else {
    return it->second.get();
  }
}

template <bool enable_deletion_support, bool memory_mirror>
typename MemoryBasedMessageBus<enable_deletion_support,
                               memory_mirror>::RegistrationState
    MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
        ::CheckSenderRegistered(
            const typename ClientMap::ReadHandle &read_handle,
            const client_id client,
            const message_type_id message_type,
            typename ClientMap::value_type::const_iterator *it) const {
  *it = read_handle->find(client);
  if (*it == read_handle->end()) {
    return RegistrationState::kNotConnected;
  } else if ((*it)->second->sendable_message_types_.contains(message_type)) {
    return RegistrationState::kRegistered;
  } else {
    return RegistrationState::kNotRegistered;
  }
}

template <bool enable_deletion_support, bool memory_mirror>
typename MemoryBasedMessageBus<enable_deletion_support,
                               memory_mirror>::RegistrationState
    MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
        ::CheckReceiverRegistered(
            const typename ClientMap::ReadHandle &read_handle,
            const client_id client,
            const message_type_id message_type,
            typename ClientMap::value_type::const_iterator *it) const {
  *it = read_handle->find(client);
  if (*it == read_handle->end()) {
    return RegistrationState::kNotConnected;
  } else if ((*it)->second->receivable_message_types_.contains(message_type)) {
    return RegistrationState::kRegistered;
  } else {
    return RegistrationState::kNotRegistered;
  }
}

template <bool enable_deletion_support, bool memory_mirror>
bool MemoryBasedMessageBus<enable_deletion_support, memory_mirror>
    ::GetReceiversForType(const message_type_id message_type,
                          ThreadsafeSet<client_id> **receivers) {
  ReceiverDirectory::ReadHandle receiver_directory_handle
      = receiver_directory_.GetReadHandle();
  ReceiverDirectory::value_type::const_iterator it
      = receiver_directory_handle->find(message_type);
  if (it == receiver_directory_handle->end()) {
    // Message type was not already present, so we will have to modify the
    // receiver directory to add it.
    receiver_directory_handle.release();

    ReceiverDirectory::WriteHandle receiver_directory_write_handle
        = receiver_directory_.GetWriteHandle();
    *receivers = (*receiver_directory_write_handle)[message_type].get();
    if (*receivers == nullptr) {
      *receivers = new ThreadsafeSet<client_id>();
      (*receiver_directory_write_handle)[message_type].reset(*receivers);
      receiver_directory_write_handle.Commit();
    }
    return true;
  } else {
    *receivers = const_cast<ThreadsafeSet<client_id>*>((it->second.get()));
    return false;
  }
}

template <bool enable_deletion_support, bool memory_mirror>
MessageBus::SendStatus MemoryBasedMessageBus<enable_deletion_support,
                                             memory_mirror>
    ::FinalizeReceivers(
        const typename ClientMap::ReadHandle &clients_read_handle,
        const Address &destination_address,
        const MessageStyle &style,
        const message_type_id message_type,
        std::vector<client_id> *receivers) const {
  if (destination_address.send_to_all_) {
    ReceiverDirectory::ReadHandle receiver_directory_handle
        = receiver_directory_.GetReadHandle();
    typename ReceiverDirectory::value_type::const_iterator receiver_dir_it
        = receiver_directory_handle->find(message_type);
    if (receiver_dir_it != receiver_directory_handle->end()) {
      receiver_dir_it->second->CopyIntoVector(receivers);
    }
  } else {
    // Check that each explicitly-specified recipient is registered as a
    // receiver for the message's type.
    typename ClientMap::value_type::const_iterator it;
    receivers->reserve(destination_address.explicit_recipients_.size());
    for (const client_id recipient_id
         : destination_address.explicit_recipients_) {
      switch (CheckReceiverRegistered(clients_read_handle,
                                      recipient_id,
                                      message_type,
                                      &it)) {
        case RegistrationState::kNotConnected:
          break;
        case RegistrationState::kNotRegistered:
          return SendStatus::kReceiverNotRegisteredForMessageType;
        case RegistrationState::kRegistered:
          receivers->push_back(recipient_id);
          break;
      }
    }
  }
  if (receivers->empty()) {
    return SendStatus::kNoReceivers;
  }

  // If there are multiple possible receivers and we are not broadcasting,
  // randomly select a single receiver.
  if ((receivers->size() > 1) & (!style.broadcast_)) {
    std::random_device hw_rand;
    std::uniform_int_distribution<std::vector<client_id>::size_type>
        dist(0, receivers->size() - 1);
    client_id chosen_receiver = (*receivers)[dist(hw_rand)];
    receivers->clear();
    receivers->push_back(chosen_receiver);
  }

  return SendStatus::kOK;
}

template <bool enable_deletion_support, bool memory_mirror>
void MemoryBasedMessageBus<enable_deletion_support, memory_mirror>::FinishSend(
    const typename ClientMap::ReadHandle &clients_read_handle,
    const std::vector<client_id> &receivers,
    QueuedMessage &&msg) const {  // NOLINT(whitespace/operators)
  for (std::vector<client_id>::size_type idx = 1;
       idx < receivers.size();
       ++idx) {
    clients_read_handle->find(receivers[idx])->second->incoming_messages_.Push(
        msg);
  }
  // For the very first receiver, move rather than copy.
  clients_read_handle->find(receivers.front())->second->incoming_messages_.Push(
      std::move(msg));
}

// Explicitly instantiate and compile-in all versions of MemoryBasedMessageBus.
template class MemoryBasedMessageBus<false, false>;
template class MemoryBasedMessageBus<false, true>;
template class MemoryBasedMessageBus<true, false>;
template class MemoryBasedMessageBus<true, true>;

}  // namespace internal
}  // namespace tmb
