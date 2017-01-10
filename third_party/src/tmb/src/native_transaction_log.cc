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

#include "tmb/internal/native_transaction_log.h"

#include <atomic>
#include <cassert>
#include <cerrno>
#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/iterator_adapter.h"
#include "tmb/internal/log_read_status.h"
#include "tmb/internal/log_reader_base.h"
#include "tmb/internal/log_writer_base.h"
#include "tmb/internal/memory_mirror_cancellation_set.h"
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/queued_message.h"
#include "tmb/internal/shared_bool.h"

#ifdef TMB_POSIX_IO_SUPPORTED

#include "tmb/internal/log_reader_posix.h"
#include "tmb/internal/log_writer_posix.h"
namespace {
typedef tmb::internal::LogReaderPosix LogReader;
typedef tmb::internal::LogWriterPosix LogWriter;
}  // namespace

#else  // TMB_POSIX_IO_SUPPORTED

#include "tmb/internal/log_reader_stdio.h"
#include "tmb/internal/log_writer_stdio.h"
namespace {
typedef tmb::internal::LogReaderStdio LogReader;
typedef tmb::internal::LogWriterStdio LogWriter;
}  // namespace

#endif  // TMB_POSIX_IO_SUPPORTED

namespace tmb {
namespace internal {

namespace {

enum class RecordType {
  kConnectClient,
  kDisconnectClient,
  kRegisterSendable,
  kRegisterReceivable,
  kSendMessage,
  kDeleteMessages,
  kCancelMessages
};

struct ConnectClientRecord {
  const RecordType record_type = RecordType::kConnectClient;
  client_id client;
};

struct DisconnectClientRecord {
  const RecordType record_type = RecordType::kDisconnectClient;
  client_id client;
};

struct RegisterSendableRecord {
  const RecordType record_type = RecordType::kRegisterSendable;
  client_id client;
  message_type_id message_type;
};

struct RegisterReceivableRecord {
  const RecordType record_type = RecordType::kRegisterReceivable;
  client_id client;
  message_type_id message_type;
};

// Note that this struct contains the common metadata for all sent messages.
// It is concatenated with an array of receiver IDs, then the message payload
// itself.
struct SendMessageRecord {
  const RecordType record_type = RecordType::kSendMessage;
  std::int64_t message_id;
  client_id sender;
  message_type_id message_type;
  Priority priority;
  bool cancellable;
  std::chrono::time_point<std::chrono::high_resolution_clock> send_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> expiration_time;
  std::size_t num_receivers;
  std::size_t message_size;
};

// This struct is followed by an array of message IDs that are deleted.
struct DeleteMessagesRecord {
  const RecordType record_type = RecordType::kDeleteMessages;
  client_id receiver;
};

// This struct is followed by an array of message IDs that are cancelled.
struct CancelMessagesRecord {
  const RecordType record_type = RecordType::kCancelMessages;
};

class NativeTransactionLogDeleteBatch : public MemoryMirrorDeleteBatch {
 public:
  explicit NativeTransactionLogDeleteBatch(const client_id receiver_id)
      : MemoryMirrorDeleteBatch(receiver_id) {
  }

  ~NativeTransactionLogDeleteBatch() {
  }

  void AddMessage(const QueuedMessage &message) override {
    message_ids_.push_back(message.GetMessageID());
  }

 private:
  std::vector<std::int64_t> message_ids_;

  friend class ::tmb::internal::NativeTransactionLog;
};

class NativeTransactionLogCancellationSet: public MemoryMirrorCancellationSet {
 public:
  explicit NativeTransactionLogCancellationSet(const std::int64_t message_id)
      : message_id_(message_id) {
  }

  ~NativeTransactionLogCancellationSet() {
  }

 private:
  const std::int64_t message_id_;

  friend class ::tmb::internal::NativeTransactionLog;
};

}  // namespace

NativeTransactionLog::NativeTransactionLog(const std::string &log_filename,
                                           const bool sync)
    : reader_(new LogReader(log_filename)),
      writer_(new LogWriter(log_filename)),
      sync_(sync),
      client_id_sequence_(0),
      message_id_sequence_(-1) {
}

NativeTransactionLog::~NativeTransactionLog() {
  const bool reader_close_ok = static_cast<LogReader*>(reader_.get())->Close();
  const bool writer_close_ok = static_cast<LogWriter*>(writer_.get())->Close();
  assert(reader_close_ok && writer_close_ok);
}

void NativeTransactionLog::ResetBusUnchecked() {
  LogWriter &writer = static_cast<LogWriter&>(*writer_);
  bool status = writer.Close();
  assert(status);
  status = writer.OpenReset();
  assert(status);
  status = writer.Sync();
  assert(status);
}

client_id NativeTransactionLog::ConnectClientUnchecked() {
  ConnectClientRecord record;
  record.client = client_id_sequence_.fetch_add(1, std::memory_order_relaxed);
  const bool status = static_cast<LogWriter*>(writer_.get())->WriteRecord(
      &record,
      sizeof(record),
      sync_);
  assert(status);
  return record.client;
}

void NativeTransactionLog::DisconnectClientUnchecked(
    const client_id client,
    const ThreadsafeSet<message_type_id> &sendable_set,
    const ThreadsafeSet<message_type_id> &receivable_set,
    const MemoryMirrorDeleteBatch *queued_message_batch) {
  DisconnectClientRecord record;
  record.client = client;
  const bool status = static_cast<LogWriter*>(writer_.get())->WriteRecord(
      &record,
      sizeof(record),
      sync_);
  assert(status);
}

void NativeTransactionLog::InsertSendableUnchecked(
    const client_id client,
    const message_type_id message_type) {
  RegisterSendableRecord record;
  record.client = client;
  record.message_type = message_type;
  const bool status = static_cast<LogWriter*>(writer_.get())->WriteRecord(
      &record,
      sizeof(record),
      sync_);
  assert(status);
}

void NativeTransactionLog::InsertReceivableUnchecked(
    const client_id client,
    const message_type_id message_type,
    const ThreadsafeSet<message_type_id> &receivable_set) {
  RegisterReceivableRecord record;
  record.client = client;
  record.message_type = message_type;
  const bool status = static_cast<LogWriter*>(writer_.get())->WriteRecord(
      &record,
      sizeof(record),
      sync_);
  assert(status);
}

std::int64_t NativeTransactionLog::SendUnchecked(
    const internal::QueuedMessage &message,
    const std::vector<client_id> &verified_receivers,
    CancellationToken *cancellation_token,
    std::uint32_t *sender_message_counter) {
  // Fill in the fixed record fields.
  SendMessageRecord record;

  record.sender = message.GetAnnotatedMessage().sender;
  // If '*sender_message_counter' hasn't overflowed yet, use it to generate a
  // unique message ID without hitting the shared 'message_id_sequence_'.
  record.message_id
      = *sender_message_counter < std::numeric_limits<std::uint32_t>::max()
          ? (static_cast<std::int64_t>(record.sender) << 32)
              | ++(*sender_message_counter)
          : message_id_sequence_.fetch_sub(1, std::memory_order_relaxed);
  record.message_type
      = message.GetAnnotatedMessage().tagged_message.message_type();
  record.priority = message.GetPriority();
  record.cancellable = (cancellation_token != nullptr);
  record.send_time = message.GetAnnotatedMessage().send_time;
  record.expiration_time = message.GetExpirationTime();
  record.num_receivers = verified_receivers.size();
  record.message_size
      = message.GetAnnotatedMessage().tagged_message.message_bytes();

  // Write multipart record: fixed struct, then array of receivers, then
  // message payload.
  const bool status
      = static_cast<LogWriter*>(writer_.get())->WriteMultipartRecord(
          {std::pair<const void*, std::size_t>(&record, sizeof(record)),
           std::pair<const void*, std::size_t>(
                verified_receivers.data(),
                sizeof(client_id) * verified_receivers.size()),
           std::pair<const void*, std::size_t>(
                message.GetAnnotatedMessage().tagged_message.message(),
                message.GetAnnotatedMessage().tagged_message.message_bytes())},
          sync_);
  assert(status);

  if (cancellation_token != nullptr) {
    cancellation_token->memory_mirror_cancellation_set_.reset(
        new NativeTransactionLogCancellationSet(record.message_id));
  }

  return record.message_id;
}

MemoryMirrorDeleteBatch* NativeTransactionLog::CreateDeleteBatch(
    const client_id receiver_id) const {
  return new NativeTransactionLogDeleteBatch(receiver_id);
}

void NativeTransactionLog::DeleteMessagesUnchecked(
    const MemoryMirrorDeleteBatch &delete_batch) {
  const NativeTransactionLogDeleteBatch &native_batch
      = static_cast<const NativeTransactionLogDeleteBatch&>(delete_batch);

  if (native_batch.message_ids_.empty()) {
    return;
  }

  DeleteMessagesRecord record;
  record.receiver = native_batch.receiver_id_;

  const bool status
      = static_cast<LogWriter*>(writer_.get())->WriteMultipartRecord(
          {std::pair<const void*, std::size_t>(&record, sizeof(record)),
           std::pair<const void*, std::size_t>(
               native_batch.message_ids_.data(),
               sizeof(std::int64_t) * native_batch.message_ids_.size())},
          sync_);
  assert(status);
}

void NativeTransactionLog::CancelMessageUnchecked(
    const CancellationToken &cancellation_token) {
  CancelMessagesRecord record;

  const bool status
      = static_cast<LogWriter*>(writer_.get())->WriteMultipartRecord(
          {std::pair<const void*, std::size_t>(&record, sizeof(record)),
           std::pair<const void*, std::size_t>(
               &(cancellation_token.message_id_),
               sizeof(cancellation_token.message_id_))},
          sync_);
  assert(status);
}

void NativeTransactionLog::CancelMessagesUnchecked(
    IteratorAdapter<const AnnotatedMessage> *adapter) {
  std::vector<std::int64_t> cancelled_ids;
  while (adapter->Valid()) {
    if ((*adapter)->memory_mirror_cancellation_set) {
      cancelled_ids.push_back(
          static_cast<const NativeTransactionLogCancellationSet*>(
              (*adapter)->memory_mirror_cancellation_set.get())->message_id_);
    }
    adapter->Next();
  }

  if (cancelled_ids.empty()) {
    return;
  }

  CancelMessagesRecord record;

  const bool status
      = static_cast<LogWriter*>(writer_.get())->WriteMultipartRecord(
          {std::pair<const void*, std::size_t>(&record, sizeof(record)),
           std::pair<const void*, std::size_t>(
               cancelled_ids.data(),
               sizeof(std::int64_t) * cancelled_ids.size())},
          sync_);
  assert(status);
}

std::unordered_map<client_id, PersistentBusStateInterface::TempClientHandle>*
    NativeTransactionLog::LoadState() {
  LogReader &reader = static_cast<LogReader&>(*reader_);
  LogWriter &writer = static_cast<LogWriter&>(*writer_);

  bool status = writer.Close();
  assert(status);

  status = reader.Open();
  if (!status && (errno == ENOENT)) {
    // Log file doesn't exist, so have the writer create it.
    status = writer.OpenReset();
    assert(status);

    return new std::unordered_map<client_id, TempClientHandle>();
  }

  assert(status);

  // During first pass, determine the set of connected clients and the set of
  // cancelled messages.
  bool found_clients = false;
  client_id max_client_id = 0;
  std::unordered_set<client_id> connected_clients;
  std::unordered_set<std::int64_t> cancelled_messages;

  LogReadStatus read_status;
  while ((read_status = reader.ReadNextRecord())
         == LogReadStatus::kOK) {
    assert(reader.CurrentRecordLength() >= sizeof(RecordType));
    switch (*static_cast<const RecordType*>(reader.CurrentRecord())) {
      case RecordType::kConnectClient: {
        found_clients = true;
        assert(reader.CurrentRecordLength() == sizeof(ConnectClientRecord));
        const client_id client = static_cast<const ConnectClientRecord*>(
            reader.CurrentRecord())->client;
        if (client > max_client_id) {
          max_client_id = client;
        }
        connected_clients.insert(client);
        break;
      }
      case RecordType::kDisconnectClient: {
        assert(reader.CurrentRecordLength() == sizeof(DisconnectClientRecord));
        connected_clients.erase(static_cast<const DisconnectClientRecord*>(
            reader.CurrentRecord())->client);
        break;
      }
      case RecordType::kCancelMessages: {
        assert((reader.CurrentRecordLength() - sizeof(CancelMessagesRecord))
               % sizeof(std::int64_t) == 0);
        const std::size_t num_cancelled
            = (reader.CurrentRecordLength() - sizeof(CancelMessagesRecord))
              / sizeof(std::int64_t);
        const std::int64_t *message_ids
            = reinterpret_cast<const std::int64_t*>(
                static_cast<const char*>(reader.CurrentRecord())
                + sizeof(CancelMessagesRecord));
        for (std::size_t idx = 0; idx < num_cancelled; ++idx) {
          cancelled_messages.insert(message_ids[idx]);
        }
        break;
      }
      case RecordType::kRegisterSendable:
      case RecordType::kRegisterReceivable:
      case RecordType::kSendMessage:
      case RecordType::kDeleteMessages:
        break;
      default:
        // Log was corrupted, but crc32 check didn't pick it up.
        assert(false);
    }
  }

  assert(read_status != LogReadStatus::kIoError);

  std::size_t committed_log_end = reader.LastGoodRecordEnd();

  // Check the end of the log, where incomplete writes might have happened in
  // the event of a crash.
  if (read_status == LogReadStatus::kEndOfFile) {
    // Log is totally clean, reopen appending records as normal.
  } else if (read_status == LogReadStatus::kTruncatedRecord) {
    // Log ends with a partial uncommitted record, so truncate it.
  } else if ((read_status == LogReadStatus::kChecksumFailed)
             || (read_status == LogReadStatus::kOversizeRecord)) {
    while ((read_status == LogReadStatus::kChecksumFailed)
           || (read_status == LogReadStatus::kOversizeRecord)) {
      // Checksum failed or an oversize (probably corrupted) length was read
      // for a record. Try reading ahead to see if this is because of
      // incomplete I/O.
      read_status = reader.ReadNextRecord();
      assert(read_status != LogReadStatus::kIoError);

      if ((read_status == LogReadStatus::kEndOfFile)
          || (read_status == LogReadStatus::kTruncatedRecord)) {
        // Failed checksum because of incomplete I/O. Truncate uncommitted
        // garbage.
      } else if (read_status == LogReadStatus::kOK) {
        // Corrupted log: good records follow bad.
        assert(read_status != LogReadStatus::kOK);
      }
    }
  }

  // Setup the client ID sequence.
  client_id_sequence_.store(found_clients ? max_client_id + 1 : 0,
                            std::memory_order_relaxed);

  // Do another pass, this time collecting the deleted message IDs for each
  // client.
  status = reader.Reset();
  assert(status);
  std::unordered_map<client_id, std::unordered_set<std::uint64_t>>
      deleted_messages;
  while (reader.LastGoodRecordEnd() < committed_log_end) {
    read_status = reader.ReadNextRecord();
    assert(read_status == LogReadStatus::kOK);
    assert(reader.CurrentRecordLength() >= sizeof(RecordType));
    switch (*static_cast<const RecordType*>(reader.CurrentRecord())) {
      case RecordType::kDeleteMessages: {
        assert(reader.CurrentRecordLength() >= sizeof(DeleteMessagesRecord));
        assert((reader.CurrentRecordLength() - sizeof(DeleteMessagesRecord))
               % sizeof(std::int64_t) == 0);
        const client_id client = static_cast<const DeleteMessagesRecord*>(
            reader.CurrentRecord())->receiver;
        if (connected_clients.find(client) != connected_clients.end()) {
          std::unordered_set<std::uint64_t> &deleted_set
              = deleted_messages[client];
          const std::size_t num_deleted
              = (reader.CurrentRecordLength() - sizeof(DeleteMessagesRecord))
                / sizeof(std::int64_t);
          const std::int64_t *message_ids
              = reinterpret_cast<const std::int64_t*>(
                  static_cast<const char*>(reader.CurrentRecord())
                  + sizeof(DeleteMessagesRecord));
          for (size_t idx = 0; idx < num_deleted; ++idx) {
            deleted_set.insert(message_ids[idx]);
          }
        }
        break;
      }
      case RecordType::kConnectClient:
      case RecordType::kDisconnectClient:
      case RecordType::kRegisterSendable:
      case RecordType::kRegisterReceivable:
      case RecordType::kSendMessage:
      case RecordType::kCancelMessages:
        break;
      default:
        // Log was corrupted, but crc32 check didn't pick it up.
        assert(false);
    }
  }

  // Reconstruct bus state from log.
  std::unique_ptr<std::unordered_map<client_id, TempClientHandle>> client_map(
      new std::unordered_map<client_id, TempClientHandle>());

  // Create client records.
  for (client_id client : connected_clients) {
    client_map->emplace(client, TempClientHandle());
  }

  // Rewind log and do a final pass, this time rebuilding the
  // sendable/receivable sets and queues of pending messages for each client.
  std::int64_t min_message_id = 0;
  status = reader.Reset();
  assert(status);
  while (reader.LastGoodRecordEnd() < committed_log_end) {
    read_status = reader.ReadNextRecord();
    assert(read_status == LogReadStatus::kOK);
    assert(reader.CurrentRecordLength() >= sizeof(RecordType));
    switch (*static_cast<const RecordType*>(reader.CurrentRecord())) {
      case RecordType::kRegisterSendable: {
        assert(reader.CurrentRecordLength()
               == sizeof(RegisterSendableRecord));
        const RegisterSendableRecord *record
            = static_cast<const RegisterSendableRecord*>(
                reader.CurrentRecord());
        std::unordered_map<client_id, TempClientHandle>::iterator it
            = client_map->find(record->client);
        if (it != client_map->end()) {
          it->second.sendable->emplace(record->message_type);
        }
        break;
      }
      case RecordType::kRegisterReceivable: {
        assert(reader.CurrentRecordLength()
               == sizeof(RegisterReceivableRecord));
        const RegisterReceivableRecord *record
            = static_cast<const RegisterReceivableRecord*>(
                reader.CurrentRecord());
        std::unordered_map<client_id, TempClientHandle>::iterator it
            = client_map->find(record->client);
        if (it != client_map->end()) {
          it->second.receivable->emplace(record->message_type);
        }
        break;
      }
      case RecordType::kSendMessage: {
        assert(reader.CurrentRecordLength() >= sizeof(SendMessageRecord));
        const SendMessageRecord *record
            = static_cast<const SendMessageRecord*>(reader.CurrentRecord());
        assert(reader.CurrentRecordLength()
               == sizeof(*record)
                  + record->num_receivers * sizeof(client_id)
                  + record->message_size);

        if (record->message_id < min_message_id) {
          min_message_id = record->message_id;
        } else if ((record->message_id > 0)
                   && (record->sender == (record->message_id >> 32))) {
          std::unordered_map<client_id, TempClientHandle>::iterator sender_it
              = client_map->find(record->sender);
          if (sender_it != client_map->end()) {
            const std::uint32_t message_counter
                = record->message_id & 0xFFFFFFFFu;
            if (message_counter > sender_it->second.sent_message_counter) {
              sender_it->second.sent_message_counter = message_counter;
            }
          }
        }

        // First, check if message has been cancelled.
        if (cancelled_messages.find(record->message_id)
            != cancelled_messages.end()) {
          break;
        }

        // Reconstruct the set of receivers that still need the message (i.e.
        // those that are still connected and have not deleted it).
        std::vector<client_id> pruned_receivers;
        const client_id *original_receivers
            = reinterpret_cast<const client_id*>(
                static_cast<const char*>(reader.CurrentRecord())
                + sizeof(*record));
        for (std::size_t receiver_idx = 0;
             receiver_idx < record->num_receivers;
             ++receiver_idx) {
          if (connected_clients.find(original_receivers[receiver_idx])
              == connected_clients.end()) {
            // Skip client if disconnected.
            continue;
          }
          std::unordered_map<
              client_id, std::unordered_set<std::uint64_t>>::const_iterator
                  deleted_set_it = deleted_messages.find(
                      original_receivers[receiver_idx]);
          if (deleted_set_it == deleted_messages.end()) {
            // Client has NO deleted messages, so include it.
            pruned_receivers.emplace_back(original_receivers[receiver_idx]);
            continue;
          }
          if (deleted_set_it->second.find(record->message_id)
              == deleted_set_it->second.end()) {
            // Message is not in the deleted set for client, so include it.
            pruned_receivers.emplace_back(original_receivers[receiver_idx]);
          }
        }
        if (pruned_receivers.empty()) {
          // Nothing to do.
          break;
        }

        // Reconstruct the message.
        TaggedMessage msg(static_cast<const char*>(reader.CurrentRecord())
                              + sizeof(*record)
                              + record->num_receivers * sizeof(client_id),
                          record->message_size,
                          record->message_type);

        // Recreate cancel flag & set if message is cancellable.
        SharedBool cancel_flag;
        std::shared_ptr<MemoryMirrorCancellationSet> cancel_set;
        if (record->cancellable) {
          cancel_flag = SharedBool(false);
          cancel_set.reset(
              new NativeTransactionLogCancellationSet(record->message_id));
        }

        // Finish recreating the QueuedMessage.
        QueuedMessage queued_msg(record->sender,
                                 record->priority,
                                 record->send_time,
                                 record->expiration_time,
                                 cancel_flag,
                                 cancel_set,
                                 std::move(msg));
        queued_msg.SetMessageID(record->message_id);

        // Enqueue the message for each pruned receiver. Use move-emplace for
        // the very last one to avoid a copy.
        for (std::vector<client_id>::const_iterator pruned_receiver_it
                 = pruned_receivers.begin();
             pruned_receiver_it != pruned_receivers.end() - 1;
             ++pruned_receiver_it) {
          (*client_map)[*pruned_receiver_it].queued_messages.emplace_back(
              queued_msg);
        }
        (*client_map)[pruned_receivers.back()].queued_messages.emplace_back(
            std::move(queued_msg));
        break;
      }
      case RecordType::kConnectClient:
      case RecordType::kDisconnectClient:
      case RecordType::kDeleteMessages:
      case RecordType::kCancelMessages:
        // Already handled in previous passes.
        break;
      default:
        // Log was corrupted since it was read in the last pass.
        assert(false);
    }
  }

  // Setup the global message ID sequence.
  message_id_sequence_.store(min_message_id - 1, std::memory_order_relaxed);

  status = reader.Close();
  assert(status);

  // Open the writer.
  status = writer.OpenAtPosition(committed_log_end);
  assert(status);

  return client_map.release();
}

}  // namespace internal
}  // namespace tmb
