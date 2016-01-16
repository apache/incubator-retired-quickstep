// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_LEVELDB_MESSAGE_BUS_H_
#define TMB_LEVELDB_MESSAGE_BUS_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "leveldb/filter_policy.h"
#include "leveldb/options.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/leveldb_key_comparator.h"
#include "tmb/internal/persistent_bus_state_interface.h"

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
 * @brief An implementation of MessageBus backed by LevelDB.
 * @note This is suitable for use by multiple threads in a process, but not
 *       by multiple simultaneous processes. In order for this implementation
 *       to work correctly, a single instance should be shared by all threads
 *       and clients in a process (attempting to use multiple instances in
 *       concert will lead to errors).
 **/
class LevelDBMessageBus : public MessageBus,
                          public internal::PersistentBusStateInterface {
 public:
  /**
   * @brief Constructor.
   *
   * @param database_path The filesystem path for the directory which contains
   *        the LevelDB database to use.
   * @param sync_writes If true, synchronous writes will be enabled for
   *        LevelDB, ensuring that the TMB remains totally consistent even in
   *        the event of a machine crash. If false, performance will be faster
   *        (~3X faster receive, ~50X faster send), but it will be possible for
   *        some recent state changes to become lost in the event of a machine
   *        crash (note that if only the process running the TMB crashes, and
   *        not the OS itself, no information will be lost).
   **/
  explicit LevelDBMessageBus(const std::string &database_path,
                             const bool sync_writes = true);

  ~LevelDBMessageBus() override {
  }

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
                               const message_type_id message_type) override;

  // No-op for LevelDBMessageBus.
  void InsertMessageTypeUnchecked(
      const message_type_id message_type) override {
  }

  void InsertReceivableUnchecked(
      const client_id client,
      const message_type_id message_type,
      const internal::ThreadsafeSet<message_type_id> &receivable_set) override;

  // TODO(chasseur): For now, this implementation ignores
  // '*sender_message_counter' because the incrementing message ID is treated
  // as a proxy for send time when ordering message keys. We could change this,
  // but it might not actually be worth it (incrementing the atomic counter
  // doesn't seem to be a bottleneck relative to disk I/O).
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
  // These are the defaults used internally by LevelDB. Testing has shown
  // that bloom filters actually decrease performance, while using much larger
  // cache and write buffers can improve performance on the order of 25% when
  // 'sync_writes' is false, but have little or no effect with 'sync_writes'
  // true.
  static const std::size_t kWriteBufferBytes = 4 * (1 << 20);  // 4 MB
  static const std::size_t kCacheSizeBytes = 8 * (1 << 20);  // 8 MB
  static const std::size_t kBlockSizeBytes = 4 * (1 << 10);  // 4 KB
  static const bool kUseCompression = true;
  static const bool kUseBloomFilter = false;
  static const int kBloomFilterBitsPerKey = 10;

  bool PrepareCancellation(const std::int64_t message_id,
                           const leveldb::Snapshot *snapshot,
                           leveldb::WriteBatch *write_batch);

  const std::string db_path_;
  const internal::LevelDBKeyComparator comp_;

  std::unique_ptr<leveldb::Cache> cache_;
  std::unique_ptr<const leveldb::FilterPolicy> filter_policy_;
  std::unique_ptr<leveldb::DB> db_;
  leveldb::Options db_options_;
  leveldb::WriteOptions sync_write_options_;

  std::atomic<client_id> client_id_sequence_;
  std::atomic<std::int64_t> message_id_sequence_;

  // Disallow copy and assign:
  LevelDBMessageBus(const LevelDBMessageBus &orig) = delete;
  LevelDBMessageBus& operator=(const LevelDBMessageBus &rhs) = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_LEVELDB_MESSAGE_BUS_H_
