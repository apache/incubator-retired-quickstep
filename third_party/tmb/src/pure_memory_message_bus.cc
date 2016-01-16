// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "tmb/pure_memory_message_bus.h"

#include <atomic>
#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstdint>
#include <limits>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/heap_receiver_message_queue.h"
#include "tmb/internal/memory_based_message_bus.h"
#include "tmb/internal/net_memory_container_pusher.h"
#include "tmb/internal/queued_message.h"
#include "tmb/internal/rcu.h"
#include "tmb/internal/shared_bool.h"
#include "tmb/internal/tree_receiver_message_queue.h"

namespace tmb {

class Address;

namespace internal { template <typename T> class IteratorAdapter; }

template <bool enable_deletion_support>
void PureMemoryMessageBus<enable_deletion_support>::ResetBus() {
  typename ClientMap::WriteHandle clients_handle(
      this->clients_.GetWriteHandleWithValue(
          new typename ClientMap::value_type));
  clients_handle.Commit();

  typename ReceiverDirectory::WriteHandle dir_handle(
      this->receiver_directory_.GetWriteHandleWithValue(
          new typename ReceiverDirectory::value_type));
  dir_handle.Commit();

  client_id_sequence_.store(0, std::memory_order_relaxed);
  message_id_sequence_.store(-1, std::memory_order_relaxed);
}

template <bool enable_deletion_support>
client_id PureMemoryMessageBus<enable_deletion_support>::Connect() {
  client_id new_client_id
      = client_id_sequence_.fetch_add(1, std::memory_order_relaxed);
  this->InsertClientHandle(new_client_id);
  return new_client_id;
}

template <bool enable_deletion_support>
bool PureMemoryMessageBus<enable_deletion_support>::Disconnect(
    const client_id client) {
  std::shared_ptr<ClientHandle> removed(this->RemoveClientHandle(client));
  return removed.get() != nullptr;
}

template <bool enable_deletion_support>
bool PureMemoryMessageBus<enable_deletion_support>::RegisterClientAsSender(
    const client_id sender_id,
    const message_type_id message_type) {
  typename ClientMap::ReadHandle clients_read_handle
      = this->clients_.GetReadHandle();
  typename ClientMap::value_type::const_iterator it;
  switch (this->CheckSenderRegistered(clients_read_handle,
                                      sender_id,
                                      message_type,
                                      &it)) {
    case RegistrationState::kNotConnected:
    case RegistrationState::kRegistered:
      return false;
    case RegistrationState::kNotRegistered:
      return it->second->sendable_message_types_.insert(message_type);
  }
  // Should be unreachable:
  return false;
}

template <bool enable_deletion_support>
bool PureMemoryMessageBus<enable_deletion_support>::RegisterClientAsReceiver(
    const client_id receiver_id,
    const message_type_id message_type) {
  typename ClientMap::ReadHandle clients_read_handle
      = this->clients_.GetReadHandle();
  typename ClientMap::value_type::const_iterator it;
  switch (this->CheckReceiverRegistered(clients_read_handle,
                                        receiver_id,
                                        message_type,
                                        &it)) {
    case RegistrationState::kNotConnected:
    case RegistrationState::kRegistered:
      return false;
    case RegistrationState::kNotRegistered:
      break;
  }

  it->second->receivable_message_types_.insert(message_type);
  clients_read_handle.release();

  tmb::internal::ThreadsafeSet<client_id> *receiver_set;
  this->GetReceiversForType(message_type, &receiver_set);
  receiver_set->insert(receiver_id);
  return true;
}

template <bool enable_deletion_support>
MessageBus::SendStatus PureMemoryMessageBus<enable_deletion_support>::Send(
    const client_id sender_id,
    const Address &destination_address,
    const MessageStyle &style,
    TaggedMessage &&message,  // NOLINT(whitespace/operators)
    const Priority priority,
    CancellationToken *cancellation_token) {
  std::chrono::time_point<std::chrono::high_resolution_clock> send_time
      = std::chrono::high_resolution_clock::now();

  typename ClientMap::ReadHandle clients_handle
      = this->clients_.GetReadHandle();
  typename ClientMap::value_type::const_iterator it;

  switch (this->CheckSenderRegistered(clients_handle,
                                      sender_id,
                                      message.message_type(),
                                      &it)) {
    case RegistrationState::kNotConnected:
      return SendStatus::kSenderNotConnected;
    case RegistrationState::kNotRegistered:
      return SendStatus::kSenderNotRegisteredForMessageType;
    case RegistrationState::kRegistered:
      break;
  }

  // Verify address and finalize the set of receivers.
  // TODO(chasseur): Consider using a vector of iterators to avoid hashing
  // twice.
  std::vector<client_id> verified_receivers;
  SendStatus retval = this->FinalizeReceivers(clients_handle,
                                              destination_address,
                                              style,
                                              message.message_type(),
                                              &verified_receivers);
  if (retval != SendStatus::kOK) {
    return retval;
  }

  const std::int64_t message_id =
      it->second->sent_message_counter_
          < std::numeric_limits<std::uint32_t>::max()
      ? (static_cast<std::int64_t>(sender_id) << 32)
          | ++(it->second->sent_message_counter_)
      : message_id_sequence_.fetch_sub(1, std::memory_order_relaxed);

  // Construct QueuedMessage.
  internal::SharedBool cancel_flag;
  if (cancellation_token != nullptr) {
    cancel_flag = internal::SharedBool(false);
  }
  tmb::internal::QueuedMessage queued_message(sender_id,
                                              priority,
                                              send_time,
                                              style.expiration_time_,
                                              cancel_flag,
                                              std::move(message));
  queued_message.SetMessageID(message_id);

  this->FinishSend(clients_handle,
                   verified_receivers,
                   std::move(queued_message));

  if (cancellation_token != nullptr) {
    cancellation_token->in_memory_cancel_flag_ = std::move(cancel_flag);
    cancellation_token->message_id_ = message_id;
    if (this->net_support_enabled_) {
      // Clean up dead cancel flags, then insert a new one.
      this->PurgeDeadCancelTokens(it->second.get());
      it->second->send_cancel_tokens_.emplace(message_id, *cancellation_token);
    }
  }
  return SendStatus::kOK;
}

template <bool enable_deletion_support>
void PureMemoryMessageBus<enable_deletion_support>::CancelMessage(
    const client_id sender_id,
    const CancellationToken &cancellation_token) {
  if (cancellation_token.in_memory_cancel_flag_.Valid()) {
    cancellation_token.in_memory_cancel_flag_.Set(true);
  }
}

template <>
void PureMemoryMessageBus<false>::DeleteById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
}

template <>
void PureMemoryMessageBus<true>::DeleteById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
  ClientHandle *handle = this->LookupClientHandle(receiver_id);
  assert(handle != nullptr);
  handle->incoming_messages_.DeleteReceivedMessagesById(message_ids, nullptr);

  // Clean up leftover dead cancel flags.
  this->PurgeDeadCancelTokens(handle);
}

template <bool enable_deletion_support>
void PureMemoryMessageBus<enable_deletion_support>::SenderCancelById(
    const client_id sender_id,
    const std::vector<std::int64_t> &message_ids) {
  ClientHandle *handle = this->LookupClientHandle(sender_id);
  assert(handle != nullptr);
  for (const std::int64_t message_id : message_ids) {
    std::unordered_map<std::int64_t, CancellationToken>::iterator it
        = handle->send_cancel_tokens_.find(message_id);
    if (it != handle->send_cancel_tokens_.end()) {
      it->second.in_memory_cancel_flag_.Set(true);
      handle->send_cancel_tokens_.erase(it);
    }
  }

  // Clean up leftover dead cancel flags.
  this->PurgeDeadCancelTokens(handle);
}

template <bool enable_deletion_support>
void PureMemoryMessageBus<enable_deletion_support>::ReceiverCancelById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
  ClientHandle *handle = this->LookupClientHandle(receiver_id);
  assert(handle != nullptr);
  for (const std::int64_t message_id : message_ids) {
    std::unordered_map<std::int64_t, CancellationToken>::iterator it
        = handle->receive_cancel_tokens_.find(message_id);
    if (it != handle->receive_cancel_tokens_.end()) {
      it->second.in_memory_cancel_flag_.Set(true);
      handle->receive_cancel_tokens_.erase(it);
    }
  }

  // Clean up leftover dead cancel flags.
  this->PurgeDeadCancelTokens(handle);
}

template <bool enable_deletion_support>
std::size_t PureMemoryMessageBus<enable_deletion_support>::ReceiveImpl(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    internal::ContainerPusher *pusher) {
  ClientHandle *handle = this->LookupClientHandle(receiver_id);
  assert(handle != nullptr);

  std::unique_ptr<internal::NetMemoryContainerPusher> net_pusher;
  if (this->net_support_enabled_) {
    // Clean up leftover dead cancel flags.
    this->PurgeDeadCancelTokens(handle);

    net_pusher.reset(new internal::NetMemoryContainerPusher(
        pusher,
        &(handle->receive_cancel_tokens_)));
  }

  return handle->incoming_messages_.Pop(
      minimum_priority,
      max_messages,
      delete_immediately,
      this->net_support_enabled_ ? net_pusher.get() : pusher,
      nullptr);
}

template <bool enable_deletion_support>
std::size_t PureMemoryMessageBus<enable_deletion_support>
    ::ReceiveIfAvailableImpl(const client_id receiver_id,
                             const Priority minimum_priority,
                             const std::size_t max_messages,
                             const bool delete_immediately,
                             internal::ContainerPusher *pusher) {
  ClientHandle *handle = this->LookupClientHandle(receiver_id);
  assert(handle != nullptr);

  std::unique_ptr<internal::NetMemoryContainerPusher> net_pusher;
  if (this->net_support_enabled_) {
    // Clean up leftover dead cancel flags.
    this->PurgeDeadCancelTokens(handle);

    net_pusher.reset(new internal::NetMemoryContainerPusher(
        pusher,
        &(handle->receive_cancel_tokens_)));
  }

  return handle->incoming_messages_.PopIfAvailable(
      minimum_priority,
      max_messages,
      delete_immediately,
      this->net_support_enabled_ ? net_pusher.get() : pusher,
      nullptr);
}

template <>
void PureMemoryMessageBus<true>::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  ClientHandle *handle = LookupClientHandle(receiver_id);
  assert(handle != nullptr);
  handle->incoming_messages_.DeleteReceivedMessages(adapter, nullptr);
}

template <>
void PureMemoryMessageBus<false>::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
}

template <bool enable_deletion_support>
void PureMemoryMessageBus<enable_deletion_support>::CancelImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  while (adapter->Valid()) {
    if ((*adapter)->in_memory_cancel_flag.Valid()) {
      (*adapter)->in_memory_cancel_flag.Set(true);
    }
    adapter->Next();
  }
}

// Explicitly instantiate and compile-in both versions of PureMemoryMessageBus.
template class PureMemoryMessageBus<true>;
template class PureMemoryMessageBus<false>;

}  // namespace tmb
