// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

// TODO(chasseur): Better error handling in place of asserts (also handle
// exceptions thrown by VoltDB).

#include "tmb/voltdb_message_bus.h"

#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "InvocationResponse.hpp"  // NOLINT(build/include)
#include "ParameterSet.hpp"        // NOLINT(build/include)
#include "Procedure.hpp"           // NOLINT(build/include)
#include "Row.hpp"                 // NOLINT(build/include)
#include "Table.h"                 // NOLINT(build/include)
#include "TableIterator.h"         // NOLINT(build/include)

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/c_string_buffer.h"
#include "tmb/internal/container_pusher.h"
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/queued_message.h"
#include "tmb/internal/shared_bool.h"
#include "tmb/internal/voltdb_connection_pool.h"
#include "tmb/internal/voltdb_procedure_warehouse.h"

namespace {

inline void AddTimestampParam(
    const std::chrono::time_point<std::chrono::high_resolution_clock> &time,
    voltdb::ParameterSet *parameter_set) {
  if (time.time_since_epoch().count() == 0) {
    parameter_set->addNull();
  } else {
    // VoltDB timestamps are represented as an int64_t counting microseconds
    // since the epoch.
    parameter_set->addTimestamp(
        std::chrono::duration_cast<std::chrono::microseconds>(
            time.time_since_epoch()).count());
  }
}

class VoltDBMemoryMirrorDeleteBatch
    : public tmb::internal::MemoryMirrorDeleteBatch {
 public:
  explicit VoltDBMemoryMirrorDeleteBatch(const tmb::client_id receiver_id)
      : tmb::internal::MemoryMirrorDeleteBatch(receiver_id) {
  }

  ~VoltDBMemoryMirrorDeleteBatch() {
  }

  inline void AddMessage(
      const tmb::internal::QueuedMessage &message) override {
    message_ids_.push_back(message.GetMessageID());
  }

 private:
  std::vector<std::int64_t> message_ids_;

  friend class tmb::VoltDBMessageBus;
};

}  // namespace

namespace tmb {

void VoltDBMessageBus::ResetBus() {
  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(warehouse_.GetProcedure(
          ProcedureID::kResetBus).get()));
  assert(response.success());
}

client_id VoltDBMessageBus::Connect() {
  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(warehouse_.GetProcedure(
          ProcedureID::kConnectClient).get()));
  while (response.statusCode() == voltdb::STATUS_CODE_GRACEFUL_FAILURE) {
    // Probably an accidental collision on client_id, so retry.
    response = connection_pool_.InvokeProcedure(warehouse_.GetProcedure(
        internal::VoltDBProcedureWarehouse::ProcedureID::kConnectClient)
            .get());
  }
  assert(response.success());

  std::vector<voltdb::Table> data = response.results();
  assert(data.size() == 1);
  assert(data[0].rowCount() == 1);
  assert(data[0].columnCount() == 1);
  voltdb::TableIterator table_it = data[0].iterator();
  assert(table_it.hasNext());
  voltdb::Row row = table_it.next();
  return row.getInt64(0);
}

bool VoltDBMessageBus::Disconnect(const client_id client) {
  ProcedureHandle disconnect_procedure(warehouse_.GetProcedure(
      ProcedureID::kDisconnectClient));
  disconnect_procedure->params()->addInt32(client);
  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(disconnect_procedure.get()));
  disconnect_procedure.release();

  if (response.success()) {
    return true;
  } else {
    // Expect that transaction was aborted because client was not connected.
    assert(response.statusCode() == voltdb::STATUS_CODE_USER_ABORT);
    return false;
  }
}

bool VoltDBMessageBus::RegisterClientAsSender(
    const client_id sender_id,
    const message_type_id message_type) {
  ProcedureHandle register_sender_procedure(warehouse_.GetProcedure(
      ProcedureID::kRegisterSender));
  register_sender_procedure->params()->addInt32(sender_id)
                                      .addInt32(message_type);
  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(register_sender_procedure.get()));
  register_sender_procedure.release();

  if (response.success()) {
    return true;
  } else {
    // STATUS_CODE_USER_ABORT occurs if the client was not connected.
    // STATUS_CODE_GRACEFUL_FAILURE occurs if the client is already registered
    // as a sender of the given message type.
    assert((response.statusCode() == voltdb::STATUS_CODE_USER_ABORT)
           || (response.statusCode() == voltdb::STATUS_CODE_GRACEFUL_FAILURE));
    return false;
  }
}

bool VoltDBMessageBus::RegisterClientAsReceiver(
    const client_id receiver_id,
    const message_type_id message_type) {
  ProcedureHandle register_receiver_procedure(warehouse_.GetProcedure(
      ProcedureID::kRegisterReceiver));
  register_receiver_procedure->params()->addInt32(receiver_id)
                                        .addInt32(message_type);
  voltdb::InvocationResponse response(connection_pool_.InvokeProcedure(
      register_receiver_procedure.get()));
  register_receiver_procedure.release();

  if (response.success()) {
    return true;
  } else {
    // STATUS_CODE_USER_ABORT occurs if the client was not connected.
    // STATUS_CODE_GRACEFUL_FAILURE occurs if the client is already registered
    // as a receiver of the given message type.
    assert((response.statusCode() == voltdb::STATUS_CODE_USER_ABORT)
           || (response.statusCode() == voltdb::STATUS_CODE_GRACEFUL_FAILURE));
    return false;
  }
}

MessageBus::SendStatus VoltDBMessageBus::Send(
    const client_id sender_id,
    const Address &destination_address,
    const MessageStyle &style,
    TaggedMessage &&message,  // NOLINT(whitespace/operators)
    const Priority priority,
    CancellationToken *cancellation_token) {
  ProcedureHandle send_procedure;
  if (destination_address.send_to_all_) {
    send_procedure = warehouse_.GetProcedure(ProcedureID::kSendToAny);
    voltdb::ParameterSet *params = send_procedure->params();
    params->addInt32(sender_id);
    AddTimestampParam(style.expiration_time_, params);
    params->addInt16(priority);
    params->addInt32(message.message_type());
    params->addBytes(message.message_bytes(),
                     static_cast<const std::uint8_t*>(message.message()));
    params->addInt8(style.broadcast_ ? 1 : 0);
    params->addInt8(cancellation_token == nullptr ? 0 : 1);
  } else if ((destination_address.explicit_recipients_.size() == 1)
             && (cancellation_token == nullptr)) {
    send_procedure = warehouse_.GetProcedure(
        ProcedureID::kSendToSingleExplicitReceiver);
    voltdb::ParameterSet *params = send_procedure->params();
    params->addInt32(sender_id);
    params->addInt32(*(destination_address.explicit_recipients_.begin()));
    AddTimestampParam(style.expiration_time_, params);
    params->addInt16(priority);
    params->addInt32(message.message_type());
    params->addBytes(message.message_bytes(),
                     static_cast<const std::uint8_t*>(message.message()));
  } else {
    send_procedure = warehouse_.GetProcedure(
        ProcedureID::kSendToExplicitReceivers);
    voltdb::ParameterSet *params = send_procedure->params();
    params->addInt32(sender_id);
    std::vector<std::int32_t> receivers_vec(
        destination_address.explicit_recipients_.begin(),
        destination_address.explicit_recipients_.end());
    params->addInt32(receivers_vec);
    AddTimestampParam(style.expiration_time_, params);
    params->addInt16(priority);
    params->addInt32(message.message_type());
    params->addBytes(
        message.message_bytes(),
        static_cast<const std::uint8_t*>(message.message()));
    params->addInt8(style.broadcast_ ? 1 : 0);
    params->addInt8(cancellation_token == nullptr ? 0 : 1);
  }

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(send_procedure.get()));
  send_procedure.release();
  assert(response.success());

  std::vector<voltdb::Table> data = response.results();
  assert(data.size() == 1);
  assert(data[0].rowCount() == 1);
  assert((data[0].columnCount() == 1) || (data[0].columnCount() == 2));
  voltdb::TableIterator table_it = data[0].iterator();
  assert(table_it.hasNext());
  voltdb::Row row = table_it.next();

  if ((row.getInt64(0) == 0) && (cancellation_token != nullptr)) {
    cancellation_token->message_id_ = row.getInt64(1);
  }
  return static_cast<MessageBus::SendStatus>(row.getInt64(0));
}

void VoltDBMessageBus::CancelMessage(
    const client_id sender_id,
    const CancellationToken &cancellation_token) {
  if (cancellation_token.message_id_ == -1) {
    return;
  }
  std::vector<std::int64_t> message_ids(1, cancellation_token.message_id_);

  ProcedureHandle cancel_messages_procedure(warehouse_.GetProcedure(
      ProcedureID::kCancelMessages));
  cancel_messages_procedure->params()->addInt64(message_ids);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(cancel_messages_procedure.get()));
  assert(response.success());
}

std::size_t VoltDBMessageBus::CountQueuedMessagesForClient(
    const client_id receiver_id) {
  ProcedureHandle count_queued_messages_for_client_procedure(
      warehouse_.GetProcedure(ProcedureID::kCountQueuedMessagesForClient));
  count_queued_messages_for_client_procedure->params()->addInt32(receiver_id);

  voltdb::InvocationResponse response(connection_pool_.InvokeProcedure(
      count_queued_messages_for_client_procedure.get()));
  assert(response.success());

  std::vector<voltdb::Table> data = response.results();
  assert(data.size() == 1);
  assert(data[0].rowCount() == 1);
  assert(data[0].columnCount() == 1);
  voltdb::TableIterator table_it = data[0].iterator();
  assert(table_it.hasNext());
  voltdb::Row row = table_it.next();
  return row.getInt64(0);
}

void VoltDBMessageBus::DeleteById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
  ProcedureHandle delete_procedure(warehouse_.GetProcedure(
      ProcedureID::kDeleteMessagesUnchecked));
  delete_procedure->params()->addInt32(receiver_id)
                             .addInt64(message_ids);
  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(delete_procedure.get()));
  assert(response.success());
}

void VoltDBMessageBus::SenderCancelById(
    const client_id sender_id,
    const std::vector<std::int64_t> &message_ids) {
  ProcedureHandle cancel_messages_procedure(warehouse_.GetProcedure(
      ProcedureID::kCancelMessages));
  cancel_messages_procedure->params()->addInt64(message_ids);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(cancel_messages_procedure.get()));
  assert(response.success());
}

void VoltDBMessageBus::ReceiverCancelById(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
    ProcedureHandle cancel_messages_procedure(warehouse_.GetProcedure(
      ProcedureID::kCancelMessages));
  cancel_messages_procedure->params()->addInt64(message_ids);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(cancel_messages_procedure.get()));
  assert(response.success());
}

void VoltDBMessageBus::InsertSendableUnchecked(
    const client_id client,
    const message_type_id message_type) {
  ProcedureHandle register_sender_unchecked_procedure(warehouse_.GetProcedure(
      ProcedureID::kRegisterSenderUnchecked));
  register_sender_unchecked_procedure->params()->addInt32(client)
                                                .addInt32(message_type);
  voltdb::InvocationResponse response(connection_pool_.InvokeProcedure(
      register_sender_unchecked_procedure.get()));
  assert(response.success());
}

void VoltDBMessageBus::InsertReceivableUnchecked(
    const client_id client,
    const message_type_id message_type,
    const internal::ThreadsafeSet<message_type_id> &receivable_set) {
  ProcedureHandle register_receiver_unchecked_procedure(
      warehouse_.GetProcedure(ProcedureID::kRegisterReceiverUnchecked));
  register_receiver_unchecked_procedure->params()->addInt32(client)
                                                  .addInt32(message_type);
  voltdb::InvocationResponse response(connection_pool_.InvokeProcedure(
      register_receiver_unchecked_procedure.get()));
  assert(response.success());
}

std::int64_t VoltDBMessageBus::SendUnchecked(
    const internal::QueuedMessage &message,
    const std::vector<client_id> &verified_receivers,
    CancellationToken *cancellation_token,
    std::uint32_t *sender_message_counter) {
  ProcedureHandle send_procedure;
  if ((cancellation_token == nullptr)  && (verified_receivers.size() == 1)) {
    send_procedure = warehouse_.GetProcedure(
        ProcedureID::kSendToSingleExplicitReceiverUnchecked);
    voltdb::ParameterSet *params = send_procedure->params();
    params->addInt32(message.GetAnnotatedMessage().sender);
    params->addInt32(verified_receivers.front());
    AddTimestampParam(message.GetAnnotatedMessage().send_time, params);
    AddTimestampParam(message.GetExpirationTime(), params);
    params->addInt16(message.GetPriority());
    params->addInt32(
        message.GetAnnotatedMessage().tagged_message.message_type());
    params->addBytes(
        message.GetAnnotatedMessage().tagged_message.message_bytes(),
        static_cast<const std::uint8_t*>(
            message.GetAnnotatedMessage().tagged_message.message()));
  } else {
    send_procedure = warehouse_.GetProcedure(
        ProcedureID::kSendToExplicitReceiversUnchecked);
    voltdb::ParameterSet *params = send_procedure->params();
    params->addInt32(message.GetAnnotatedMessage().sender);
    std::vector<std::int32_t> receivers_vec(verified_receivers.begin(),
                                            verified_receivers.end());
    params->addInt32(receivers_vec);
    AddTimestampParam(message.GetAnnotatedMessage().send_time, params);
    AddTimestampParam(message.GetExpirationTime(), params);
    params->addInt16(message.GetPriority());
    params->addInt32(
        message.GetAnnotatedMessage().tagged_message.message_type());
    params->addBytes(
        message.GetAnnotatedMessage().tagged_message.message_bytes(),
        static_cast<const std::uint8_t*>(
            message.GetAnnotatedMessage().tagged_message.message()));
    params->addInt8(cancellation_token == nullptr ? 0 : 1);
  }

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(send_procedure.get()));
  send_procedure.release();
  assert(response.success());

  std::vector<voltdb::Table> data = response.results();
  assert(data.size() == 1);
  assert(data[0].rowCount() == 1);
  assert(data[0].columnCount() == 1);
  voltdb::TableIterator table_it = data[0].iterator();
  assert(table_it.hasNext());
  voltdb::Row row = table_it.next();
  std::int64_t message_id = row.getInt64(0);

  if (cancellation_token != nullptr) {
    cancellation_token->message_id_ = message_id;
  }
  return message_id;
}

internal::MemoryMirrorDeleteBatch* VoltDBMessageBus::CreateDeleteBatch(
    const client_id receiver_id) const {
  return new VoltDBMemoryMirrorDeleteBatch(receiver_id);
}

void VoltDBMessageBus::DeleteMessagesUnchecked(
    const internal::MemoryMirrorDeleteBatch &delete_batch) {
  const VoltDBMemoryMirrorDeleteBatch &voltdb_delete_batch
      = static_cast<const VoltDBMemoryMirrorDeleteBatch&>(delete_batch);

  ProcedureHandle delete_procedure(warehouse_.GetProcedure(
      ProcedureID::kDeleteMessagesUnchecked));
  delete_procedure->params()->addInt32(voltdb_delete_batch.receiver_id_)
                             .addInt64(voltdb_delete_batch.message_ids_);
  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(delete_procedure.get()));
  assert(response.success());
}

void VoltDBMessageBus::CancelMessageUnchecked(
    const CancellationToken &cancellation_token) {
  std::vector<std::int64_t> message_ids(1, cancellation_token.message_id_);

  ProcedureHandle cancel_messages_procedure(warehouse_.GetProcedure(
      ProcedureID::kCancelMessages));
  cancel_messages_procedure->params()->addInt64(message_ids);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(cancel_messages_procedure.get()));
  assert(response.success());
}

void VoltDBMessageBus::CancelMessagesUnchecked(
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  std::vector<std::int64_t> message_ids;

  while (adapter->Valid()) {
    if ((*adapter)->in_memory_cancel_flag.Valid()) {
      message_ids.push_back((*adapter)->deletion_token.message_id);
    }
    adapter->Next();
  }

  ProcedureHandle cancel_messages_procedure(warehouse_.GetProcedure(
      ProcedureID::kCancelMessages));
  cancel_messages_procedure->params()->addInt64(message_ids);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(cancel_messages_procedure.get()));
  assert(response.success());
}

std::unordered_map<client_id,
                   internal::PersistentBusStateInterface::TempClientHandle>*
    VoltDBMessageBus::LoadState() {
  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(warehouse_.GetProcedure(
          ProcedureID::kLoadState).get()));
  assert(response.success());

  std::vector<voltdb::Table> data = response.results();
  assert(data.size() == 5);

  std::unique_ptr<std::unordered_map<client_id, TempClientHandle>> client_map(
      new std::unordered_map<client_id, TempClientHandle>());

  // Create client records.
  voltdb::TableIterator client_it = data[0].iterator();
  while (client_it.hasNext()) {
    voltdb::Row client_row = client_it.next();
    client_map->emplace(client_row.getInt32(0), TempClientHandle());
  }

  // Load sendable types.
  voltdb::TableIterator sendable_it = data[1].iterator();
  while (sendable_it.hasNext()) {
    voltdb::Row sendable_row = sendable_it.next();
    client_map->find(sendable_row.getInt32(0))->second.sendable->emplace(
        sendable_row.getInt32(1));
  }

  // Load receivable types.
  voltdb::TableIterator receivable_it = data[2].iterator();
  while (receivable_it.hasNext()) {
    voltdb::Row receivable_row = receivable_it.next();
    client_map->find(receivable_row.getInt32(0))->second.receivable->emplace(
        receivable_row.getInt32(1));
  }

  // Reconstruct cancellation tokens for cancellable messages.
  std::unordered_map<std::int64_t, internal::SharedBool> cancellation_flags;
  voltdb::TableIterator cancellable_it = data[3].iterator();
  while (cancellable_it.hasNext()) {
    voltdb::Row cancellable_row = cancellable_it.next();
    std::int64_t message_id = cancellable_row.getInt64(0);
    cancellation_flags.emplace(message_id, internal::SharedBool(false));
  }

  // Load messages.
  voltdb::TableIterator message_it = data[4].iterator();
  while (message_it.hasNext()) {
    voltdb::Row message_row = message_it.next();
    client_id receiver_id = message_row.getInt32(0);
    std::int64_t message_id = message_row.getInt64(1);

    std::unordered_map<client_id, TempClientHandle>::iterator client_it
        = client_map->find(receiver_id);
    assert(client_it != client_map->end());

    int payload_bytes = message_row.getInt32(8);
    internal::CStringBuffer payload_buffer(payload_bytes);
    int bytes_written;
    bool written = message_row.getVarbinary(
          7,
          payload_bytes,
          reinterpret_cast<std::uint8_t*>(payload_buffer.Get()),
          &bytes_written);
    assert(written);
    assert(bytes_written == payload_bytes);

    TaggedMessage msg;
    msg.acquire_message(payload_buffer.Release(),
                        payload_bytes,
                        message_row.getInt32(6));

    std::chrono::time_point<std::chrono::high_resolution_clock> send_time(
        std::chrono::microseconds(message_row.getTimestamp(3)));
    std::chrono::time_point<std::chrono::high_resolution_clock>
        expiration_time;
    if (message_row.getTimestamp(4)
        != std::numeric_limits<std::int64_t>::max()) {
      expiration_time
          = std::chrono::time_point<std::chrono::high_resolution_clock>(
                std::chrono::microseconds(message_row.getTimestamp(4)));
    }

    internal::SharedBool cancellation_flag;
    std::unordered_map<std::int64_t, internal::SharedBool>::const_iterator
        cancellation_flag_it = cancellation_flags.find(message_id);
    if (cancellation_flag_it != cancellation_flags.end()) {
      cancellation_flag = cancellation_flag_it->second;
    }

    internal::QueuedMessage queued_msg(message_row.getInt32(2),
                                       message_row.getInt16(5),
                                       send_time,
                                       expiration_time,
                                       cancellation_flag,
                                       std::move(msg));
    queued_msg.SetMessageID(message_id);
    client_it->second.queued_messages.emplace_back(std::move(queued_msg));
  }

  return client_map.release();
}

std::size_t VoltDBMessageBus::ReceiveIfAvailableImpl(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    internal::ContainerPusher *pusher) {
  ProcedureHandle receive_procedure(warehouse_.GetProcedure(
      delete_immediately ? ProcedureID::kReceiveAndDelete
                         : ProcedureID::kReceive));
  receive_procedure->params()->addInt32(receiver_id)
                              .addInt16(minimum_priority)
                              .addInt64(max_messages);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(receive_procedure.get()));
  receive_procedure.release();
  assert(response.success());

  std::vector<voltdb::Table> data = response.results();
  assert(data.size() == 1);

  if (data[0].rowCount() > 0) {
    voltdb::TableIterator table_it = data[0].iterator();
    while (table_it.hasNext()) {
      AnnotatedMessage message;

      voltdb::Row message_row = table_it.next();
      message.deletion_token.message_id = message_row.getInt64(0);
      message.sender = message_row.getInt32(1);
      message.send_time
          = std::chrono::time_point<std::chrono::high_resolution_clock>(
              std::chrono::microseconds(message_row.getTimestamp(2)));

      int payload_bytes = message_row.getInt32(5);
      internal::CStringBuffer payload_buffer(payload_bytes);
      int bytes_written;
      bool written = message_row.getVarbinary(
          4,
          payload_bytes,
          reinterpret_cast<std::uint8_t*>(payload_buffer.Get()),
          &bytes_written);
      assert(written);
      assert(bytes_written == payload_bytes);

      message.tagged_message.acquire_message(payload_buffer.Release(),
                                             payload_bytes,
                                             message_row.getInt32(3));
      pusher->Push(std::move(message));
    }
  }

  return data[0].rowCount();
}

void VoltDBMessageBus::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  std::vector<std::int64_t> message_ids;
  while (adapter->Valid()) {
    message_ids.push_back((*adapter)->deletion_token.message_id);
    adapter->Next();
  }
  if (message_ids.empty()) {
    return;
  }

  ProcedureHandle delete_messages_procedure(warehouse_.GetProcedure(
      ProcedureID::kDeleteMessages));
  delete_messages_procedure->params()->addInt32(receiver_id)
                                      .addInt64(message_ids);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(delete_messages_procedure.get()));
  assert(response.success());
}

void VoltDBMessageBus::CancelImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  std::vector<std::int64_t> message_ids;
  while (adapter->Valid()) {
    message_ids.push_back((*adapter)->deletion_token.message_id);
    adapter->Next();
  }
  if (message_ids.empty()) {
    return;
  }

  ProcedureHandle cancel_messages_procedure(warehouse_.GetProcedure(
      ProcedureID::kCancelMessages));
  cancel_messages_procedure->params()->addInt64(message_ids);

  voltdb::InvocationResponse response(
      connection_pool_.InvokeProcedure(cancel_messages_procedure.get()));
  assert(response.success());
}

}  // namespace tmb
