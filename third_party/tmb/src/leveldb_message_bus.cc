// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "tmb/leveldb_message_bus.h"

#include <atomic>
#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/slice.h"
#include "leveldb/status.h"
#include "leveldb/write_batch.h"

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/message_style.h"

#include "tmb/internal/container_pusher.h"
#include "tmb/internal/iterator_adapter.h"
#include "tmb/internal/leveldb_key_comparator.h"
#include "tmb/internal/leveldb_keys.h"
#include "tmb/internal/memory_mirror_cancellation_set.h"
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/message_metadata.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/queued_message.h"
#include "tmb/internal/shared_bool.h"
#include "tmb/internal/threadsafe_set.h"

namespace tmb {

namespace {

template <typename T>
inline leveldb::Slice AsSlice(const T &item) {
  return leveldb::Slice(reinterpret_cast<const char*>(&item), sizeof(item));
}

class ReceivableTypesListBuilder {
 public:
  explicit ReceivableTypesListBuilder(std::string *output)
      : output_(output) {
  }

  inline void operator()(const message_type_id message_type) {
    output_->append(reinterpret_cast<const char*>(&message_type),
                    sizeof(message_type));
  }

 private:
  std::string *output_;
};

template <typename KeyType>
class MessageTypeDirectoryDeleter {
 public:
  MessageTypeDirectoryDeleter(const client_id client,
                              leveldb::WriteBatch *write_batch)
      : write_batch_(write_batch) {
    key_template_.client = client;
  }

  inline void operator()(const message_type_id message_type) {
    key_template_.message_type = message_type;
    write_batch_->Delete(AsSlice(key_template_));
  }

 private:
  KeyType key_template_;
  leveldb::WriteBatch *write_batch_;
};

class LevelDBMemoryMirrorCancellationSet
    : public internal::MemoryMirrorCancellationSet {
 public:
  explicit LevelDBMemoryMirrorCancellationSet(
      const internal::LevelDBMessageKey &message_key)
      : message_key_(message_key) {
  }

  ~LevelDBMemoryMirrorCancellationSet() {
  }

  inline void AddQueuedMessageKey(
      const internal::LevelDBQueuedMessageKey &key) {
    queued_message_keys_.push_back(key);
  }

 private:
  const internal::LevelDBMessageKey message_key_;
  std::vector<internal::LevelDBQueuedMessageKey> queued_message_keys_;

  friend class tmb::LevelDBMessageBus;
};

class LevelDBMemoryMirrorDeleteBatch
    : public internal::MemoryMirrorDeleteBatch {
 public:
  explicit LevelDBMemoryMirrorDeleteBatch(const client_id receiver_id)
      : internal::MemoryMirrorDeleteBatch(receiver_id) {
  }

  ~LevelDBMemoryMirrorDeleteBatch() {
  }

  void AddMessage(const internal::QueuedMessage &message) override {
    queued_message_keys_.resize(queued_message_keys_.size() + 1);
    queued_message_keys_.back().receiver = receiver_id_;
    queued_message_keys_.back().priority = message.GetPriority();
    queued_message_keys_.back().expiration_time = message.GetExpirationTime();
    queued_message_keys_.back().message_id = message.GetMessageID();
  }

 private:
  std::vector<internal::LevelDBQueuedMessageKey> queued_message_keys_;

  friend class tmb::LevelDBMessageBus;
};

}  // namespace

const std::size_t LevelDBMessageBus::kWriteBufferBytes;
const std::size_t LevelDBMessageBus::kCacheSizeBytes;
const std::size_t LevelDBMessageBus::kBlockSizeBytes;
const bool LevelDBMessageBus::kUseCompression;
const bool LevelDBMessageBus::kUseBloomFilter;
const int LevelDBMessageBus::kBloomFilterBitsPerKey;

LevelDBMessageBus::LevelDBMessageBus(const std::string &database_path,
                                     const bool sync_writes)
    : db_path_(database_path) {
  db_options_.comparator = &comp_;
  db_options_.create_if_missing = true;
  db_options_.write_buffer_size = kWriteBufferBytes;

  db_options_.block_size = kBlockSizeBytes;
  db_options_.compression = kUseCompression ? leveldb::kSnappyCompression
                                            : leveldb::kNoCompression;

  if (kUseBloomFilter) {
    filter_policy_.reset(
        leveldb::NewBloomFilterPolicy(kBloomFilterBitsPerKey));
    db_options_.filter_policy = filter_policy_.get();
  }

  sync_write_options_.sync = sync_writes;
}

bool LevelDBMessageBus::Initialize() {
  // Setup cache.
  cache_.reset(leveldb::NewLRUCache(kCacheSizeBytes));
  db_options_.block_cache = cache_.get();

  // Instantiate DB.
  leveldb::DB* db_ptr = nullptr;
  leveldb::Status status = leveldb::DB::Open(db_options_, db_path_, &db_ptr);
  db_.reset(db_ptr);
  if (!status.ok()) {
    return false;
  }

  // Find the highest-numbered client in the DB.
  internal::LevelDBClientKey max_client_key;
  max_client_key.client = kClientIdNone;

  std::unique_ptr<leveldb::Iterator> it(
      db_->NewIterator(leveldb::ReadOptions()));
  it->Seek(AsSlice(max_client_key));
  if (it->Valid()) {
    // There is something stored past the individual client records.
    it->Prev();
  } else {
    // There is nothing in the DB past the individual client records, so the
    // last record in the DB, if any, will be a client record.
    it->SeekToLast();
  }
  if (it->Valid()) {
    // '*it' is now on the highest-numbered client record.
    leveldb::Slice key_slice = it->key();
    assert(key_slice.size() == sizeof(internal::LevelDBClientKey));
    const internal::LevelDBClientKey *key
        = reinterpret_cast<const internal::LevelDBClientKey*>(
            key_slice.data());
    client_id_sequence_.store(key->client + 1, std::memory_order_relaxed);
  } else {
    // There are no client records, so simply initialize 'client_id_sequence_'
    // to 0.
    client_id_sequence_.store(0, std::memory_order_relaxed);
  }

  // Similarly, find the highest-numbered message to initialize
  // 'message_id_sequence_'.
  internal::LevelDBMessageKey max_message_key;
  max_message_key.message_id = std::numeric_limits<std::int64_t>::max();
  it->Seek(AsSlice(max_message_key));
  if (it->Valid()) {
    it->Prev();
  } else {
    it->SeekToLast();
  }
  if (it->Valid()
      && (it->key().size() >= sizeof(std::uint32_t))
      && (*reinterpret_cast<const std::uint32_t*>(it->key().data())
          == internal::kLevelDBKeyTypeMessage)) {
    leveldb::Slice key_slice = it->key();
    assert(key_slice.size() == sizeof(internal::LevelDBMessageKey));
    const internal::LevelDBMessageKey *key
        = reinterpret_cast<const internal::LevelDBMessageKey*>(
            key_slice.data());
    message_id_sequence_.store(key->message_id + 1, std::memory_order_relaxed);
  } else {
    message_id_sequence_.store(0, std::memory_order_relaxed);
  }

  return true;
}

void LevelDBMessageBus::ResetBus() {
  // First, close the database.
  db_.reset(nullptr);

  // Destroy the cache.
  cache_.reset(nullptr);
  db_options_.block_cache = nullptr;

  // Destroy the database.
  leveldb::Status status = leveldb::DestroyDB(db_path_, db_options_);
  assert(status.ok());

  // Recreate the cache.
  cache_.reset(leveldb::NewLRUCache(kCacheSizeBytes));
  db_options_.block_cache = cache_.get();

  // Reopen the now-empty DB.
  leveldb::DB* db_ptr;
  status = leveldb::DB::Open(db_options_, db_path_, &db_ptr);
  assert(status.ok());

  db_.reset(db_ptr);

  // Reset sequence numbers to 0.
  client_id_sequence_.store(0, std::memory_order_relaxed);
  message_id_sequence_.store(0, std::memory_order_relaxed);
}

client_id LevelDBMessageBus::Connect() {
  internal::LevelDBClientKey client_key;
  client_key.client
      = client_id_sequence_.fetch_add(1, std::memory_order_relaxed);

  leveldb::Status status = db_->Put(sync_write_options_,
                                    AsSlice(client_key),
                                    leveldb::Slice());
  assert(status.ok());

  return client_key.client;
}

bool LevelDBMessageBus::Disconnect(const client_id client) {
  // Perform reads against a consistent snapshot of the DB state.
  leveldb::ReadOptions read_options;
  read_options.snapshot = db_->GetSnapshot();

  internal::LevelDBClientKey client_key;
  client_key.client = client;

  std::string value_buf;
  leveldb::Status status = db_->Get(read_options,
                                    AsSlice(client_key),
                                    &value_buf);
  if (status.IsNotFound()) {
    db_->ReleaseSnapshot(read_options.snapshot);
    return false;
  }
  assert(status.ok());
  assert(value_buf.size() % sizeof(message_type_id) == 0);

  // Use a WriteBatch to atomically delete the client record and all associated
  // sender and receiver directory entries.
  leveldb::WriteBatch disconnect_batch;

  // Delete the client record itself.
  disconnect_batch.Delete(AsSlice(client_key));

  // Delete all of the associated receiver directory entries.
  internal::LevelDBReceiverDirectoryKey receiver_key;
  receiver_key.client = client;
  std::size_t num_receiver_entries = value_buf.size() / sizeof(message_type_id);
  for (std::size_t receiver_entry_num = 0;
       receiver_entry_num < num_receiver_entries;
       ++receiver_entry_num) {
    receiver_key.message_type = reinterpret_cast<const message_type_id*>(
        value_buf.c_str())[receiver_entry_num];
    disconnect_batch.Delete(AsSlice(receiver_key));
  }

  // Iterate over the sender directory entries associated with this client and
  // delete them as well.
  internal::LevelDBSenderDirectoryKey sender_key
      = internal::LevelDBSenderDirectoryKey::MinKeyForClient(client);
  internal::LevelDBSenderDirectoryKey sender_key_limit
      = internal::LevelDBSenderDirectoryKey::LimitKeyForClient(client);

  std::unique_ptr<leveldb::Iterator> sender_it(db_->NewIterator(read_options));
  for (sender_it->Seek(AsSlice(sender_key));
       sender_it->Valid() &&
           (comp_.Compare(sender_it->key(), AsSlice(sender_key_limit)) < 0);
       sender_it->Next()) {
    disconnect_batch.Delete(sender_it->key());
  }
  sender_it.reset(nullptr);

  // Finally, iterate over all the pending undelivered messages for this client
  // and delete them.
  //
  // TODO(chasseur): It is possible for a sender to come in and queue a message
  // for this client after the snapshot was created but before
  // 'disconnect_batch' is committed, in which case there will be some
  // "leftover" undeleted messages.
  internal::LevelDBQueuedMessageKey queued_message_key
      = internal::LevelDBQueuedMessageKey::MinKeyForClient(client);
  internal::LevelDBQueuedMessageKey queued_message_key_limit
      = internal::LevelDBQueuedMessageKey::LimitKeyForClient(client);

  std::unique_ptr<leveldb::Iterator> queued_message_it(
      db_->NewIterator(read_options));
  for (queued_message_it->Seek(AsSlice(queued_message_key));
       queued_message_it->Valid() &&
           (comp_.Compare(queued_message_it->key(),
                          AsSlice(queued_message_key_limit)) < 0);
       queued_message_it->Next()) {
    disconnect_batch.Delete(queued_message_it->key());
  }
  queued_message_it.reset(nullptr);

  // We are done reading so release the snapshot.
  db_->ReleaseSnapshot(read_options.snapshot);

  status = db_->Write(sync_write_options_, &disconnect_batch);
  assert(status.ok());
  return true;
}

bool LevelDBMessageBus::RegisterClientAsSender(
    const client_id sender_id,
    const message_type_id message_type) {
  internal::LevelDBSenderDirectoryKey sender_directory_key;
  sender_directory_key.client = sender_id;
  sender_directory_key.message_type = message_type;

  // First, check if already registered.
  std::string value_buf;
  leveldb::Status status = db_->Get(leveldb::ReadOptions(),
                                    AsSlice(sender_directory_key),
                                    &value_buf);
  if (!status.IsNotFound()) {
    assert(status.ok());
    return false;
  }

  // Check that the client is, in fact, connected.
  internal::LevelDBClientKey client_key;
  client_key.client = sender_id;
  status = db_->Get(leveldb::ReadOptions(),
                    AsSlice(client_key),
                    &value_buf);
  if (status.IsNotFound()) {
    return false;
  }
  assert(status.ok());

  // Write the directory entry.
  status = db_->Put(sync_write_options_,
                    AsSlice(sender_directory_key),
                    leveldb::Slice());
  assert(status.ok());

  return true;
}

bool LevelDBMessageBus::RegisterClientAsReceiver(
    const client_id receiver_id,
    const message_type_id message_type) {
  internal::LevelDBReceiverDirectoryKey receiver_directory_key;
  receiver_directory_key.client = receiver_id;
  receiver_directory_key.message_type = message_type;

  std::string value_buf;

  // Check if already registered.
  leveldb::Status status = db_->Get(leveldb::ReadOptions(),
                                    AsSlice(receiver_directory_key),
                                    &value_buf);
  if (!status.IsNotFound()) {
    assert(status.ok());
    return false;
  }

  // Check that client is connected and fetch its list of receivable types.
  internal::LevelDBClientKey client_key;
  client_key.client = receiver_id;
  status = db_->Get(leveldb::ReadOptions(),
                    AsSlice(client_key),
                    &value_buf);
  if (status.IsNotFound()) {
    return false;
  }
  assert(status.ok());

  // Append the new 'message_type' to the list of types for the client.
  assert(value_buf.size() % sizeof(message_type_id) == 0);
  value_buf.append(reinterpret_cast<const char*>(&message_type),
                   sizeof(message_type));

  // Use a WriteBatch to atomically update both the client record and the entry
  // in the receiver directory.
  leveldb::WriteBatch registration_batch;
  registration_batch.Put(AsSlice(client_key), value_buf);
  registration_batch.Put(AsSlice(receiver_directory_key), leveldb::Slice());
  status = db_->Write(sync_write_options_, &registration_batch);
  assert(status.ok());

  return true;
}

MessageBus::SendStatus LevelDBMessageBus::Send(
    const client_id sender_id,
    const Address &destination_address,
    const MessageStyle &style,
    TaggedMessage &&message,  // NOLINT(whitespace/operators)
    const Priority priority,
    CancellationToken *cancellation_token) {
  internal::MessageMetadata message_metadata;
  message_metadata.message_type = message.message_type();
  message_metadata.sender = sender_id;
  message_metadata.send_time = std::chrono::high_resolution_clock::now();

  leveldb::Status status;
  std::string value_buf;

  // Execute reads against a consistent snapshot.
  leveldb::ReadOptions read_options;
  read_options.snapshot = db_->GetSnapshot();

  // Check that sender is connected.
  internal::LevelDBClientKey client_key;
  client_key.client = sender_id;
  status = db_->Get(read_options, AsSlice(client_key), &value_buf);
  if (status.IsNotFound()) {
    db_->ReleaseSnapshot(read_options.snapshot);
    return SendStatus::kSenderNotConnected;
  }
  assert(status.ok());

  // Check that sender is registered for the given message type.
  internal::LevelDBSenderDirectoryKey sender_key;
  sender_key.client = sender_id;
  sender_key.message_type = message.message_type();
  status = db_->Get(read_options, AsSlice(sender_key), &value_buf);
  if (status.IsNotFound()) {
    db_->ReleaseSnapshot(read_options.snapshot);
    return SendStatus::kSenderNotRegisteredForMessageType;
  }
  assert(status.ok());

  std::vector<client_id> receiver_ids;
  if (destination_address.send_to_all_) {
    // Check if there are any connected receivers capable of receiving this
    // message.
    internal::LevelDBReceiverDirectoryKey receiver_key
        = internal::LevelDBReceiverDirectoryKey::MinKeyForMessageType(
            message.message_type());
    internal::LevelDBReceiverDirectoryKey receiver_limit
        = internal::LevelDBReceiverDirectoryKey::LimitKeyForMessageType(
            message.message_type());

    std::unique_ptr<leveldb::Iterator> receiver_it(
        db_->NewIterator(read_options));
    for (receiver_it->Seek(AsSlice(receiver_key));
         receiver_it->Valid() &&
             (comp_.Compare(receiver_it->key(), AsSlice(receiver_limit)) < 0);
         receiver_it->Next()) {
      receiver_ids.push_back(
          reinterpret_cast<const internal::LevelDBReceiverDirectoryKey*>(
              receiver_it->key().data())->client);
    }
  } else {
    for (const client_id receiver : destination_address.explicit_recipients_) {
      internal::LevelDBClientKey receiver_client_key;
      receiver_client_key.client = receiver;

      status = db_->Get(read_options,
                        AsSlice(receiver_client_key),
                        &value_buf);
      if (!status.IsNotFound()) {
        // Receiver is connected, now check if it is registered for the given
        // message type.
        assert(status.ok());

        internal::LevelDBReceiverDirectoryKey receiver_directory_key;
        receiver_directory_key.message_type = message.message_type();
        receiver_directory_key.client = receiver;

        status = db_->Get(read_options,
                          AsSlice(receiver_directory_key),
                          &value_buf);
        if (status.IsNotFound()) {
          db_->ReleaseSnapshot(read_options.snapshot);
          return SendStatus::kReceiverNotRegisteredForMessageType;
        } else {
          assert(status.ok());
          receiver_ids.push_back(receiver);
        }
      }
    }
  }
  // We are done with read ops, so release the snapshot.
  db_->ReleaseSnapshot(read_options.snapshot);
  if (receiver_ids.empty()) {
    return SendStatus::kNoReceivers;
  }

  // Actually enqueue message.
  leveldb::WriteBatch send_batch;

  leveldb::Slice message_payload;
  char message_payload_inline[sizeof(message.payload_.in_line.data)
                              + sizeof(message_metadata)];
  const std::size_t message_bytes = message.message_bytes();
  if (message.payload_inline_) {
    std::memcpy(message_payload_inline,
                message.message(),
                message_bytes);
    std::memcpy(message_payload_inline + message_bytes,
                &message_metadata,
                sizeof(message_metadata));
    message_payload = leveldb::Slice(message_payload_inline,
                                     message_bytes + sizeof(message_metadata));
  } else {
    message.ExtendOutOfLineAllocation(sizeof(message_metadata));
    std::memcpy(static_cast<char*>(message.payload_.out_of_line.data)
                    + message_bytes,
                &message_metadata,
                sizeof(message_metadata));
    message_payload = leveldb::Slice(
        static_cast<const char*>(message.message()),
        message_bytes + sizeof(message_metadata));
  }

  // TODO(chasseur): Implement a size-based heuristic to determine whether
  // messages with multiple recipients should be stored multiple times in
  // the "queued_message" range or once in the "message" range.
  internal::LevelDBMessageKey message_key;
  message_key.message_id
      = message_id_sequence_.fetch_add(1, std::memory_order_relaxed);
  std::string message_value;
  if (cancellation_token != nullptr) {
    message_value.reserve(
        receiver_ids.size() * sizeof(internal::LevelDBQueuedMessageKey));
  }

  internal::LevelDBQueuedMessageKey queued_message_key;
  queued_message_key.priority = priority;
  queued_message_key.expiration_time = style.expiration_time_;
  queued_message_key.message_id = message_key.message_id;
  for (const client_id receiver : receiver_ids) {
    queued_message_key.receiver = receiver;
    send_batch.Put(AsSlice(queued_message_key), message_payload);

    if (cancellation_token != nullptr) {
      // TODO(chasseur): Don't duplicate redundant parts of the keys.
      message_value.append(reinterpret_cast<const char*>(&queued_message_key),
                           sizeof(queued_message_key));
    }
  }

  send_batch.Put(AsSlice(message_key), message_value);

  status = db_->Write(sync_write_options_, &send_batch);
  assert(status.ok());

  if (cancellation_token != nullptr) {
    cancellation_token->message_id_ = message_key.message_id;
  }
  return SendStatus::kOK;
}

void LevelDBMessageBus::CancelMessage(
    const client_id sender_id,
    const CancellationToken &cancellation_token) {
  const leveldb::Snapshot *snapshot = db_->GetSnapshot();
  leveldb::WriteBatch cancel_batch;

  if (PrepareCancellation(cancellation_token.message_id_,
                          snapshot,
                          &cancel_batch)) {
    leveldb::Status status = db_->Write(sync_write_options_, &cancel_batch);
    assert(status.ok());
  }

  db_->ReleaseSnapshot(snapshot);
}

std::size_t LevelDBMessageBus::CountQueuedMessagesForClient(
    const client_id receiver_id) {
  internal::LevelDBQueuedMessageKey min_message_key
      = internal::LevelDBQueuedMessageKey::MinKeyForClient(receiver_id);
  internal::LevelDBQueuedMessageKey limit_message_key
      = internal::LevelDBQueuedMessageKey::LimitKeyForClient(receiver_id);

  std::size_t message_count = 0;
  std::unique_ptr<leveldb::Iterator> message_it(
      db_->NewIterator(leveldb::ReadOptions()));
  for (message_it->Seek(AsSlice(min_message_key));
       message_it->Valid() &&
           (comp_.Compare(message_it->key(), AsSlice(limit_message_key)) < 0);
       message_it->Next()) {
    ++message_count;
  }

  return message_count;
}

void LevelDBMessageBus::DisconnectClientUnchecked(
    const client_id client,
    const internal::ThreadsafeSet<message_type_id> &sendable_set,
    const internal::ThreadsafeSet<message_type_id> &receivable_set,
    const internal::MemoryMirrorDeleteBatch *queued_message_batch) {
  leveldb::WriteBatch disconnect_batch;

  internal::LevelDBClientKey client_key;
  client_key.client = client;
  disconnect_batch.Delete(AsSlice(client_key));

  sendable_set.for_each(
      MessageTypeDirectoryDeleter<internal::LevelDBSenderDirectoryKey>(
          client, &disconnect_batch));
  receivable_set.for_each(
      MessageTypeDirectoryDeleter<internal::LevelDBReceiverDirectoryKey>(
          client, &disconnect_batch));

  for (const internal::LevelDBQueuedMessageKey &key
       : static_cast<const LevelDBMemoryMirrorDeleteBatch&>(
           *queued_message_batch).queued_message_keys_) {
    disconnect_batch.Delete(AsSlice(key));
  }

  leveldb::Status status = db_->Write(sync_write_options_,
                                      &disconnect_batch);
  assert(status.ok());
}

void LevelDBMessageBus::InsertSendableUnchecked(
    const client_id client,
    const message_type_id message_type) {
  internal::LevelDBSenderDirectoryKey sender_directory_key;
  sender_directory_key.client = client;
  sender_directory_key.message_type = message_type;

  leveldb::Status status = db_->Put(sync_write_options_,
                                    AsSlice(sender_directory_key),
                                    leveldb::Slice());
  assert(status.ok());
}

void LevelDBMessageBus::InsertReceivableUnchecked(
    const client_id client,
    const message_type_id message_type,
    const internal::ThreadsafeSet<message_type_id> &receivable_set) {
  internal::LevelDBClientKey client_key;
  client_key.client = client;

  std::string client_value;
  receivable_set.for_each(ReceivableTypesListBuilder(&client_value));
  client_value.append(reinterpret_cast<const char*>(&message_type),
                      sizeof(message_type));

  internal::LevelDBReceiverDirectoryKey receiver_directory_key;
  receiver_directory_key.client = client;
  receiver_directory_key.message_type = message_type;

  leveldb::WriteBatch registration_batch;
  registration_batch.Put(AsSlice(client_key), client_value);
  registration_batch.Put(AsSlice(receiver_directory_key), leveldb::Slice());
  leveldb::Status status = db_->Write(sync_write_options_,
                                      &registration_batch);
  assert(status.ok());
}

std::int64_t LevelDBMessageBus::SendUnchecked(
    const internal::QueuedMessage &message,
    const std::vector<client_id> &verified_receivers,
    CancellationToken *cancellation_token,
    std::uint32_t *sender_message_counter) {
  leveldb::WriteBatch send_batch;

  // Allocate a message ID and construct a message key.
  internal::LevelDBMessageKey message_key;
  message_key.message_id
      = message_id_sequence_.fetch_add(1, std::memory_order_relaxed);
  std::unique_ptr<LevelDBMemoryMirrorCancellationSet> cancellation_set;
  if (cancellation_token != nullptr) {
    cancellation_set.reset(
        new LevelDBMemoryMirrorCancellationSet(message_key));
  }

  std::string message_value;

  // Construct LevelDB message payload from 'message'.
  std::string message_payload(
      static_cast<const char*>(
          message.GetAnnotatedMessage().tagged_message.message()),
      message.GetAnnotatedMessage().tagged_message.message_bytes());

  internal::MessageMetadata message_metadata;
  message_metadata.message_type
      = message.GetAnnotatedMessage().tagged_message.message_type();
  message_metadata.sender = message.GetAnnotatedMessage().sender;
  message_metadata.send_time = message.GetAnnotatedMessage().send_time;

  message_payload.append(reinterpret_cast<const char*>(&message_metadata),
                         sizeof(message_metadata));

  // Send to each receiver.
  internal::LevelDBQueuedMessageKey queued_message_key;
  queued_message_key.priority = message.GetPriority();
  queued_message_key.expiration_time = message.GetExpirationTime();
  queued_message_key.message_id = message_key.message_id;
  for (const client_id receiver_id : verified_receivers) {
    queued_message_key.receiver = receiver_id;
    send_batch.Put(AsSlice(queued_message_key), message_payload);
    if (cancellation_token != nullptr) {
      message_value.append(reinterpret_cast<const char*>(&queued_message_key),
                           sizeof(queued_message_key));
      cancellation_set->AddQueuedMessageKey(queued_message_key);
    }
  }

  send_batch.Put(AsSlice(message_key), message_value);

  leveldb::Status status = db_->Write(sync_write_options_, &send_batch);
  assert(status.ok());

  if (cancellation_token != nullptr) {
    cancellation_token->memory_mirror_cancellation_set_.reset(
        cancellation_set.release());
  }
  return message_key.message_id;
}

internal::MemoryMirrorDeleteBatch* LevelDBMessageBus::CreateDeleteBatch(
    const client_id receiver_id) const {
  return new LevelDBMemoryMirrorDeleteBatch(receiver_id);
}

void LevelDBMessageBus::DeleteMessagesUnchecked(
    const internal::MemoryMirrorDeleteBatch &delete_batch) {
  leveldb::WriteBatch write_batch;
  for (const internal::LevelDBQueuedMessageKey &key
       : static_cast<const LevelDBMemoryMirrorDeleteBatch&>(delete_batch)
             .queued_message_keys_) {
    write_batch.Delete(AsSlice(key));
  }
  leveldb::Status status = db_->Write(sync_write_options_, &write_batch);
  assert(status.ok());
}

void LevelDBMessageBus::CancelMessageUnchecked(
    const CancellationToken &cancellation_token) {
  const LevelDBMemoryMirrorCancellationSet &cancellation_set_cast
      = static_cast<const LevelDBMemoryMirrorCancellationSet&>(
          *(cancellation_token.memory_mirror_cancellation_set_));

  leveldb::WriteBatch cancel_batch;
  cancel_batch.Delete(AsSlice(cancellation_set_cast.message_key_));
  for (const internal::LevelDBQueuedMessageKey &key
       : cancellation_set_cast.queued_message_keys_) {
    cancel_batch.Delete(AsSlice(key));
  }

  leveldb::Status status = db_->Write(sync_write_options_, &cancel_batch);
  assert(status.ok());
}

void LevelDBMessageBus::CancelMessagesUnchecked(
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  leveldb::WriteBatch cancel_batch;

  while (adapter->Valid()) {
    if ((*adapter)->memory_mirror_cancellation_set) {
      const LevelDBMemoryMirrorCancellationSet *cancellation_set_cast
          = static_cast<const LevelDBMemoryMirrorCancellationSet*>(
              (*adapter)->memory_mirror_cancellation_set.get());
      cancel_batch.Delete(AsSlice(cancellation_set_cast->message_key_));
      for (const internal::LevelDBQueuedMessageKey &key
           : cancellation_set_cast->queued_message_keys_) {
        cancel_batch.Delete(AsSlice(key));
      }
    }
    adapter->Next();
  }

  leveldb::Status status = db_->Write(sync_write_options_, &cancel_batch);
  assert(status.ok());
}

std::unordered_map<client_id,
                   internal::PersistentBusStateInterface::TempClientHandle>*
    LevelDBMessageBus::LoadState() {
  std::unique_ptr<std::unordered_map<client_id, TempClientHandle>> client_map(
      new std::unordered_map<client_id, TempClientHandle>());

  std::unique_ptr<leveldb::Iterator> it(
      db_->NewIterator(leveldb::ReadOptions()));

  // Create client records.
  internal::LevelDBClientKey min_client_key;
  min_client_key.client = std::numeric_limits<client_id>::min();
  internal::LevelDBClientKey max_client_key;
  max_client_key.client = std::numeric_limits<client_id>::max();
  for (it->Seek(AsSlice(min_client_key));
       it->Valid() && (comp_.Compare(it->key(), AsSlice(max_client_key)) <= 0);
       it->Next()) {
    client_id client = reinterpret_cast<const internal::LevelDBClientKey*>(
        it->key().data())->client;
    client_map->emplace(client, TempClientHandle());
  }

  // Load sendable types.
  internal::LevelDBSenderDirectoryKey min_sender_key;
  min_sender_key.client = std::numeric_limits<client_id>::min();
  min_sender_key.message_type = std::numeric_limits<message_type_id>::min();
  internal::LevelDBSenderDirectoryKey max_sender_key;
  max_sender_key.client = std::numeric_limits<client_id>::max();
  max_sender_key.message_type = std::numeric_limits<message_type_id>::max();
  for (it->Seek(AsSlice(min_sender_key));
       it->Valid() && (comp_.Compare(it->key(), AsSlice(max_sender_key)) <= 0);
       it->Next()) {
    client_id client
        = reinterpret_cast<const internal::LevelDBSenderDirectoryKey*>(
            it->key().data())->client;
    message_type_id message_type
        = reinterpret_cast<const internal::LevelDBSenderDirectoryKey*>(
            it->key().data())->message_type;
    client_map->find(client)->second.sendable->emplace(message_type);
  }

  // Load receivable types.
  internal::LevelDBReceiverDirectoryKey min_receiver_key;
  min_receiver_key.message_type = std::numeric_limits<message_type_id>::min();
  min_receiver_key.client = std::numeric_limits<client_id>::min();
  internal::LevelDBReceiverDirectoryKey max_receiver_key;
  max_receiver_key.message_type = std::numeric_limits<message_type_id>::max();
  max_receiver_key.client = std::numeric_limits<client_id>::max();
  for (it->Seek(AsSlice(min_receiver_key));
       it->Valid() && (comp_.Compare(it->key(), AsSlice(max_receiver_key))
                       <= 0);
       it->Next()) {
    client_id client
        = reinterpret_cast<const internal::LevelDBReceiverDirectoryKey*>(
            it->key().data())->client;
    message_type_id message_type
        = reinterpret_cast<const internal::LevelDBReceiverDirectoryKey*>(
            it->key().data())->message_type;
    client_map->find(client)->second.receivable->emplace(message_type);
  }

  // Reconstruct cancellation tokens for cancellable messages.
  std::unordered_map<std::int64_t, internal::SharedBool> cancellation_flags;
  std::unordered_map<std::int64_t,
                     std::shared_ptr<internal::MemoryMirrorCancellationSet>>
      cancellation_sets;
  internal::LevelDBMessageKey min_message_key;
  min_message_key.message_id = std::numeric_limits<std::int64_t>::min();
  internal::LevelDBMessageKey max_message_key;
  max_message_key.message_id = std::numeric_limits<std::int64_t>::max();
  for (it->Seek(AsSlice(min_message_key));
       it->Valid() && (comp_.Compare(it->key(), AsSlice(max_message_key))
                       <= 0);
       it->Next()) {
    leveldb::Slice value_slice(it->value());
    if (!value_slice.empty()) {
      assert((value_slice.size() % sizeof(internal::LevelDBQueuedMessageKey))
             == 0);

      std::int64_t message_id
          = reinterpret_cast<const internal::LevelDBMessageKey*>(
              it->key().data())->message_id;

      LevelDBMemoryMirrorCancellationSet *cancellation_set
          = new LevelDBMemoryMirrorCancellationSet(
              *reinterpret_cast<const internal::LevelDBMessageKey*>(
                  it->key().data()));
      for (const char *key_ptr = value_slice.data();
           key_ptr < value_slice.data() + value_slice.size();
           key_ptr += sizeof(internal::LevelDBQueuedMessageKey)) {
        cancellation_set->AddQueuedMessageKey(
            *reinterpret_cast<const internal::LevelDBQueuedMessageKey*>(
                key_ptr));
      }

      cancellation_flags.emplace(message_id, internal::SharedBool(false));
      cancellation_sets.emplace(
          message_id,
          std::shared_ptr<internal::MemoryMirrorCancellationSet>(
              cancellation_set));
    }
  }

  // Load messages.
  internal::LevelDBQueuedMessageKey min_queued_message_key =
      internal::LevelDBQueuedMessageKey::MinKeyForClient(
          std::numeric_limits<client_id>::min());
  internal::LevelDBQueuedMessageKey max_queued_message_key =
      internal::LevelDBQueuedMessageKey::MinKeyForClient(
          std::numeric_limits<client_id>::max());
  for (it->Seek(AsSlice(min_queued_message_key));
       it->Valid()
           && (comp_.Compare(it->key(), AsSlice(max_queued_message_key)) <= 0);
       it->Next()) {
    const internal::LevelDBQueuedMessageKey *queued_message_key
        = reinterpret_cast<const internal::LevelDBQueuedMessageKey*>(
            it->key().data());

    std::unordered_map<client_id,
                       internal::PersistentBusStateInterface::TempClientHandle>
        ::iterator client_it = client_map->find(queued_message_key->receiver);
    if (client_it == client_map->end()) {
      continue;
    }

    leveldb::Slice value_slice = it->value();
    assert(value_slice.size() >= sizeof(internal::MessageMetadata));
    const internal::MessageMetadata *metadata_ptr
        = reinterpret_cast<const internal::MessageMetadata*>(
            value_slice.data() + value_slice.size()
                               - sizeof(internal::MessageMetadata));

    TaggedMessage msg(value_slice.data(),
                      value_slice.size() - sizeof(internal::MessageMetadata),
                      metadata_ptr->message_type);

    internal::SharedBool cancellation_flag;
    std::unordered_map<std::int64_t, internal::SharedBool>::const_iterator
        cancellation_flag_it = cancellation_flags.find(
            queued_message_key->message_id);
    if (cancellation_flag_it != cancellation_flags.end()) {
      cancellation_flag = cancellation_flag_it->second;
    }

    std::shared_ptr<internal::MemoryMirrorCancellationSet> cancellation_set;
    std::unordered_map<std::int64_t,
                       std::shared_ptr<internal::MemoryMirrorCancellationSet>>
        ::const_iterator cancellation_set_it
            = cancellation_sets.find(queued_message_key->message_id);
    if (cancellation_set_it != cancellation_sets.end()) {
      cancellation_set = cancellation_set_it->second;
    }

    internal::QueuedMessage queued_msg(metadata_ptr->sender,
                                       queued_message_key->priority,
                                       metadata_ptr->send_time,
                                       queued_message_key->expiration_time,
                                       cancellation_flag,
                                       cancellation_set,
                                       std::move(msg));
    queued_msg.SetMessageID(queued_message_key->message_id);
    client_it->second.queued_messages.emplace_back(std::move(queued_msg));
  }

  return client_map.release();
}

std::size_t LevelDBMessageBus::ReceiveIfAvailableImpl(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    internal::ContainerPusher *pusher) {
  std::chrono::time_point<std::chrono::high_resolution_clock> now
      = std::chrono::high_resolution_clock::now();

  std::unique_ptr<leveldb::Iterator> it(
      db_->NewIterator(leveldb::ReadOptions()));
  leveldb::WriteBatch delete_batch;
  std::size_t received = 0;

  internal::LevelDBQueuedMessageKey queued_message_key;
  queued_message_key.receiver = receiver_id;
  queued_message_key.priority = std::numeric_limits<Priority>::max();
  queued_message_key.expiration_time = now;
  queued_message_key.message_id = std::numeric_limits<std::int64_t>::min();
  for (it->Seek(AsSlice(queued_message_key));
       it->Valid() && ((max_messages == 0) || (received < max_messages));
       it->Next()) {
    leveldb::Slice key_slice = it->key();
    assert(key_slice.size() >= sizeof(std::uint32_t));
    if (*reinterpret_cast<const std::uint32_t*>(key_slice.data())
        != internal::kLevelDBKeyTypeQueuedMessage) {
      break;
    }

    assert(key_slice.size() == sizeof(internal::LevelDBQueuedMessageKey));
    const internal::LevelDBQueuedMessageKey *fetched_key
        = reinterpret_cast<const internal::LevelDBQueuedMessageKey*>(
            key_slice.data());
    if ((fetched_key->receiver != receiver_id)
        || (fetched_key->priority < minimum_priority)) {
      break;
    }

    // If message is unexpired, copy it out.
    if ((fetched_key->expiration_time.time_since_epoch().count() == 0)
        || (now < fetched_key->expiration_time)) {
      leveldb::Slice value_slice = it->value();
      assert(value_slice.size() >= sizeof(internal::MessageMetadata));
      const internal::MessageMetadata *metadata_ptr
          = reinterpret_cast<const internal::MessageMetadata*>(
              value_slice.data() + value_slice.size()
                                 - sizeof(internal::MessageMetadata));

      AnnotatedMessage received_message;
      received_message.sender = metadata_ptr->sender;
      received_message.send_time = metadata_ptr->send_time;
      received_message.tagged_message.set_message(
          value_slice.data(),
          value_slice.size() - sizeof(internal::MessageMetadata),
          metadata_ptr->message_type);
      std::memcpy(received_message.deletion_token.leveldb_key,
                  key_slice.data(),
                  sizeof(internal::LevelDBQueuedMessageKey));
      pusher->Push(std::move(received_message));

      ++received;

      if (delete_immediately) {
        delete_batch.Delete(key_slice);
      }
    } else {
      // Delete expired messages regardless of whether 'delete_immediately' is
      // true.
      delete_batch.Delete(key_slice);
    }
  }

  leveldb::Status status = db_->Write(sync_write_options_, &delete_batch);
  assert(status.ok());

  return received;
}

void LevelDBMessageBus::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  if (!adapter->Valid()) {
    return;
  }

  leveldb::WriteBatch delete_batch;
  while (adapter->Valid()) {
    delete_batch.Delete(leveldb::Slice(
        (*adapter)->deletion_token.leveldb_key,
        sizeof(internal::LevelDBQueuedMessageKey)));
    adapter->Next();
  }

  leveldb::Status status = db_->Write(sync_write_options_, &delete_batch);
  assert(status.ok());
}

void LevelDBMessageBus::CancelImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  if (!adapter->Valid()) {
    return;
  }

  const leveldb::Snapshot *snapshot = db_->GetSnapshot();
  leveldb::WriteBatch cancel_batch;
  while (adapter->Valid()) {
    const internal::LevelDBQueuedMessageKey *queued_message_key
        = reinterpret_cast<const internal::LevelDBQueuedMessageKey*>(
            (*adapter)->deletion_token.leveldb_key);
    PrepareCancellation(queued_message_key->message_id,
                        snapshot,
                        &cancel_batch);
    adapter->Next();
  }

  leveldb::Status status = db_->Write(sync_write_options_, &cancel_batch);
  assert(status.ok());

  db_->ReleaseSnapshot(snapshot);
}

bool LevelDBMessageBus::PrepareCancellation(const std::int64_t message_id,
                                            const leveldb::Snapshot *snapshot,
                                            leveldb::WriteBatch *write_batch) {
  internal::LevelDBMessageKey message_key;
  message_key.message_id = message_id;

  leveldb::ReadOptions read_options;
  read_options.snapshot = snapshot;

  std::string message_value;
  leveldb::Status status = db_->Get(read_options,
                                    AsSlice(message_key),
                                    &message_value);

  if (status.IsNotFound()) {
    return false;
  }
  assert(status.ok());

  if (message_value.empty()) {
    return false;
  }
  assert((message_value.size() % sizeof(internal::LevelDBQueuedMessageKey))
         == 0);

  // Delete messages from per-client queues.
  for (const char *key_ptr = message_value.c_str();
       key_ptr < message_value.c_str() + message_value.size();
       key_ptr += sizeof(internal::LevelDBQueuedMessageKey)) {
    write_batch->Delete(
        leveldb::Slice(key_ptr, sizeof(internal::LevelDBQueuedMessageKey)));
  }

  // Wipe-out the cancellation metadata (no longer needed) but keep the message
  // sequence number in place.
  write_batch->Put(AsSlice(message_key), leveldb::Slice());

  return true;
}

}  // namespace tmb
