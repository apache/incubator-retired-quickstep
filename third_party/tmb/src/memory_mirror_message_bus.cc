// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "tmb/memory_mirror_message_bus.h"

#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstdint>
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
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/net_memory_container_pusher.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/queued_message.h"
#include "tmb/internal/rcu.h"
#include "tmb/internal/shared_bool.h"
#include "tmb/internal/tree_receiver_message_queue.h"

namespace tmb {

class Address;

namespace internal { template <typename T> class IteratorAdapter; }

template <bool enable_deletion_support>
bool MemoryMirrorMessageBus<enable_deletion_support>::Initialize() {
  std::unique_ptr<std::unordered_map<
      client_id,
      internal::PersistentBusStateInterface::TempClientHandle>> bus_state(
          underlying_bus_->LoadState());

  std::unique_ptr<typename ClientMap::value_type> reconstructed_client_map(
      new typename ClientMap::value_type);

  std::unordered_map<message_type_id,
                     std::unique_ptr<std::unordered_set<client_id>>>
      temp_receiver_directory;

  for (std::pair<const client_id,
                 internal::PersistentBusStateInterface::TempClientHandle>
           &loaded_client : *bus_state) {
    // Insert appropriate entries in temporary receiver directory.
    const client_id id = loaded_client.first;
    for (const message_type_id receivable_type
         : *(loaded_client.second.receivable)) {
      std::unordered_map<message_type_id,
                         std::unique_ptr<std::unordered_set<client_id>>>
          ::iterator receiver_dir_it = temp_receiver_directory.find(
              receivable_type);
      if (receiver_dir_it == temp_receiver_directory.end()) {
        receiver_dir_it = temp_receiver_directory.emplace(
            receivable_type,
            std::unique_ptr<std::unordered_set<client_id>>(
                new std::unordered_set<client_id>)).first;
      }
      receiver_dir_it->second->emplace(id);
    }

    // Rebuild client handle.
    std::shared_ptr<ClientHandle> client_handle(new ClientHandle(
        loaded_client.second.sendable.release(),
        loaded_client.second.receivable.release(),
        loaded_client.second.sent_message_counter));
    client_handle->incoming_messages_.UnsafeBulkInsert(
        &(loaded_client.second.queued_messages));

    reconstructed_client_map->emplace(id, client_handle);
  }

  // Finalize receiver directory.
  std::unique_ptr<typename ReceiverDirectory::value_type>
      reconstructed_receiver_directory(
          new typename ReceiverDirectory::value_type);
  for (std::pair<const message_type_id,
                 std::unique_ptr<std::unordered_set<client_id>>> &temp_entry
           : temp_receiver_directory) {
    reconstructed_receiver_directory->emplace(
        temp_entry.first,
        std::shared_ptr<internal::ThreadsafeSet<client_id>>(
            new internal::ThreadsafeSet<client_id>(
                temp_entry.second.release())));
  }

  // Put the reconstructed data into the RCUs.
  typename ClientMap::WriteHandle clients_handle
      = this->clients_.GetWriteHandleWithValue(
          reconstructed_client_map.release());
  clients_handle.Commit();

  typename ReceiverDirectory::WriteHandle receiver_dir_handle
      = this->receiver_directory_.GetWriteHandleWithValue(
          reconstructed_receiver_directory.release());
  receiver_dir_handle.Commit();

  return true;
}


template <bool enable_deletion_support>
void MemoryMirrorMessageBus<enable_deletion_support>::ResetBus() {
  underlying_bus_->ResetBusUnchecked();
  Initialize();
}

template <bool enable_deletion_support>
client_id MemoryMirrorMessageBus<enable_deletion_support>::Connect() {
  client_id new_client_id = underlying_bus_->ConnectClientUnchecked();
  this->InsertClientHandle(new_client_id);
  return new_client_id;
}

template <bool enable_deletion_support>
bool MemoryMirrorMessageBus<enable_deletion_support>::Disconnect(
    const client_id client) {
  std::shared_ptr<ClientHandle> removed(this->RemoveClientHandle(client));
  if (removed.get() != nullptr) {
    std::unique_ptr<internal::MemoryMirrorDeleteBatch> queued_delete_batch;
    if (underlying_bus_->DisconnectRequiresQueueDrain()) {
      queued_delete_batch.reset(underlying_bus_->CreateDeleteBatch(client));
      removed->incoming_messages_.Drain(queued_delete_batch.get());
    }
    underlying_bus_->DisconnectClientUnchecked(
        client,
        removed->sendable_message_types_,
        removed->receivable_message_types_,
        queued_delete_batch.get());
    return true;
  } else {
    return false;
  }
}

template <bool enable_deletion_support>
bool MemoryMirrorMessageBus<enable_deletion_support>::RegisterClientAsSender(
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
      underlying_bus_->InsertSendableUnchecked(sender_id, message_type);
      return it->second->sendable_message_types_.insert(message_type);
  }
  // Should be unreachable:
  return false;
}

template <bool enable_deletion_support>
bool MemoryMirrorMessageBus<enable_deletion_support>::RegisterClientAsReceiver(
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

  tmb::internal::ThreadsafeSet<client_id> *receiver_set;
  if (this->GetReceiversForType(message_type, &receiver_set)) {
    underlying_bus_->InsertMessageTypeUnchecked(message_type);
  }

  underlying_bus_->InsertReceivableUnchecked(
      receiver_id,
      message_type,
      it->second->receivable_message_types_);

  it->second->receivable_message_types_.insert(message_type);
  receiver_set->insert(receiver_id);

  return true;
}

template <bool enable_deletion_support>
MessageBus::SendStatus MemoryMirrorMessageBus<enable_deletion_support>::Send(
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

  const std::int64_t actual_message_id
      = underlying_bus_->SendUnchecked(queued_message,
                                       verified_receivers,
                                       cancellation_token,
                                       &(it->second->sent_message_counter_));
  queued_message.SetMessageID(actual_message_id);

  if (cancellation_token != nullptr) {
    queued_message.SetMemoryMirrorCancellationSet(
        cancellation_token->memory_mirror_cancellation_set_);
  }

  this->FinishSend(clients_handle,
                   verified_receivers,
                   std::move(queued_message));

  if (cancellation_token != nullptr) {
    cancellation_token->in_memory_cancel_flag_ = std::move(cancel_flag);
    if (this->net_support_enabled_) {
      // Clean up dead cancel flags, then insert a new one.
      this->PurgeDeadCancelTokens(it->second.get());
      cancellation_token->message_id_ = actual_message_id;
      it->second->send_cancel_tokens_.emplace(actual_message_id,
                                              *cancellation_token);
    }
  }
  return SendStatus::kOK;
}

template <bool enable_deletion_support>
void MemoryMirrorMessageBus<enable_deletion_support>::CancelMessage(
    const client_id sender_id,
    const CancellationToken &cancellation_token) {
  if (cancellation_token.in_memory_cancel_flag_.Valid()) {
    if (!cancellation_token.in_memory_cancel_flag_.Set(true)) {
      // TODO(chasseur): If a crash happens before the call below finishes,
      // a message that appears cancelled in-memory could come back from the
      // dead on recovery.
      underlying_bus_->CancelMessageUnchecked(cancellation_token);
    }
  }
}

template <>
void MemoryMirrorMessageBus<false>::DeleteById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
}

template <>
void MemoryMirrorMessageBus<true>::DeleteById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
  ClientHandle *handle = this->LookupClientHandle(receiver_id);
  assert(handle != nullptr);

  std::unique_ptr<internal::MemoryMirrorDeleteBatch> delete_batch(
      underlying_bus_->CreateDeleteBatch(receiver_id));
  handle->incoming_messages_.DeleteReceivedMessagesById(message_ids,
                                                        delete_batch.get());
  underlying_bus_->DeleteMessagesUnchecked(*delete_batch);

  // Clean up leftover dead cancel flags.
  this->PurgeDeadCancelTokens(handle);
}

template <bool enable_deletion_support>
void MemoryMirrorMessageBus<enable_deletion_support>::SenderCancelById(
    const client_id sender_id,
    const std::vector<std::int64_t> &message_ids) {
  ClientHandle *handle = this->LookupClientHandle(sender_id);
  assert(handle != nullptr);

  for (const std::int64_t message_id : message_ids) {
    std::unordered_map<std::int64_t, CancellationToken>::iterator it
        = handle->send_cancel_tokens_.find(message_id);
    if (it != handle->send_cancel_tokens_.end()) {
      if (!it->second.in_memory_cancel_flag_.Set(true)) {
        underlying_bus_->CancelMessageUnchecked(it->second);
      }
      handle->send_cancel_tokens_.erase(it);
    }
  }

  // Clean up leftover dead cancel flags.
  this->PurgeDeadCancelTokens(handle);
}

template <bool enable_deletion_support>
void MemoryMirrorMessageBus<enable_deletion_support>::ReceiverCancelById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
  ClientHandle *handle = this->LookupClientHandle(receiver_id);
  assert(handle != nullptr);

  // TODO(chasseur): Can probably make this more efficient if we add an API to
  // send multiple cancellation tokens to the underlying bus in one call.
  for (const std::int64_t message_id : message_ids) {
    std::unordered_map<std::int64_t, CancellationToken>::iterator it
        = handle->receive_cancel_tokens_.find(message_id);
    if (it != handle->receive_cancel_tokens_.end()) {
      if (!it->second.in_memory_cancel_flag_.Set(true)) {
        underlying_bus_->CancelMessageUnchecked(it->second);
      }
      handle->receive_cancel_tokens_.erase(it);
    }
  }

  // Clean up leftover dead cancel flags.
  this->PurgeDeadCancelTokens(handle);
}

template <bool enable_deletion_support>
std::size_t MemoryMirrorMessageBus<enable_deletion_support>::ReceiveImpl(
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

  std::unique_ptr<internal::MemoryMirrorDeleteBatch> delete_batch(
      underlying_bus_->CreateDeleteBatch(receiver_id));
  std::size_t retval = handle->incoming_messages_.Pop(
      minimum_priority,
      max_messages,
      delete_immediately,
      this->net_support_enabled_ ? net_pusher.get() : pusher,
      delete_batch.get());
  underlying_bus_->DeleteMessagesUnchecked(*delete_batch);
  return retval;
}

template <bool enable_deletion_support>
std::size_t MemoryMirrorMessageBus<enable_deletion_support>
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

  std::unique_ptr<internal::MemoryMirrorDeleteBatch> delete_batch(
      underlying_bus_->CreateDeleteBatch(receiver_id));
  std::size_t retval = handle->incoming_messages_.PopIfAvailable(
      minimum_priority,
      max_messages,
      delete_immediately,
      this->net_support_enabled_ ? net_pusher.get() : pusher,
      delete_batch.get());
  underlying_bus_->DeleteMessagesUnchecked(*delete_batch);
  return retval;
}

template <>
void MemoryMirrorMessageBus<true>::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  ClientHandle *handle = LookupClientHandle(receiver_id);
  assert(handle != nullptr);

  std::unique_ptr<internal::MemoryMirrorDeleteBatch> delete_batch(
      underlying_bus_->CreateDeleteBatch(receiver_id));
  handle->incoming_messages_.DeleteReceivedMessages(adapter,
                                                    delete_batch.get());
  underlying_bus_->DeleteMessagesUnchecked(*delete_batch);
}

template <>
void MemoryMirrorMessageBus<false>::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
}

template <bool enable_deletion_support>
void MemoryMirrorMessageBus<enable_deletion_support>::CancelImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  std::unique_ptr<internal::IteratorAdapter<const AnnotatedMessage>>
      adapter_clone(adapter->Clone());

  while (adapter->Valid()) {
    if ((*adapter)->in_memory_cancel_flag.Valid()) {
      (*adapter)->in_memory_cancel_flag.Set(true);
    }
    adapter->Next();
  }

  // TODO(chasseur): If a crash happens before the call below finishes,
  // a message that appears cancelled in-memory could come back from the
  // dead on recovery.
  underlying_bus_->CancelMessagesUnchecked(adapter_clone.get());
}


// Explicitly instantiate and compile-in both versions of
// MemoryMirrorMessageBus.
template class MemoryMirrorMessageBus<true>;
template class MemoryMirrorMessageBus<false>;

}  // namespace tmb
