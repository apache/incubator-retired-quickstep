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

#ifndef TMB_INTERNAL_NATIVE_TRANSACTION_LOG_H_
#define TMB_INTERNAL_NATIVE_TRANSACTION_LOG_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "tmb/id_typedefs.h"

#include "tmb/internal/cache_info.h"
#include "tmb/internal/log_reader_base.h"
#include "tmb/internal/log_writer_base.h"
#include "tmb/internal/persistent_bus_state_interface.h"

namespace tmb {

struct AnnotatedMessage;
class CancellationToken;

namespace internal {

class MemoryMirrorDeleteBatch;
template <typename T> class IteratorAdapter;
template <typename T> class ThreadsafeSet;

/**
 * @brief A lightweight write-ahead log used to record persistent bus state
 *        underneath a MemoryMirrorMessageBus without any third-party database.
 **/
class NativeTransactionLog : public PersistentBusStateInterface {
 public:
  // Not writable until LoadState() or ResetBusUnchecked() is called.
  explicit NativeTransactionLog(const std::string &log_filename,
                                const bool sync);

  ~NativeTransactionLog();

  void ResetBusUnchecked() override;

  client_id ConnectClientUnchecked() override;

  bool DisconnectRequiresQueueDrain() const override {
    return false;
  }

  void DisconnectClientUnchecked(
      const client_id client,
      const ThreadsafeSet<message_type_id> &sendable_set,
      const ThreadsafeSet<message_type_id> &receivable_set,
      const MemoryMirrorDeleteBatch *queued_message_batch) override;

  void InsertSendableUnchecked(const client_id client,
                               const message_type_id message_type) override;

  // No-op for NativeTransactionLog.
  void InsertMessageTypeUnchecked(
      const message_type_id message_type) override {
  }

  void InsertReceivableUnchecked(
      const client_id client,
      const message_type_id message_type,
      const ThreadsafeSet<message_type_id> &receivable_set) override;

  std::int64_t SendUnchecked(
      const internal::QueuedMessage &message,
      const std::vector<client_id> &verified_receivers,
      CancellationToken *cancellation_token,
      std::uint32_t *sender_message_counter) override;

  MemoryMirrorDeleteBatch* CreateDeleteBatch(
      const client_id receiver_id) const override;

  void DeleteMessagesUnchecked(
      const MemoryMirrorDeleteBatch &delete_batch) override;

  void CancelMessageUnchecked(
      const CancellationToken &cancellation_token) override;

  void CancelMessagesUnchecked(
      IteratorAdapter<const AnnotatedMessage> *adapter) override;

  std::unordered_map<client_id, TempClientHandle>* LoadState() override;

 private:
  std::unique_ptr<LogReaderBase> reader_;
  std::unique_ptr<LogWriterBase> writer_;

  const bool sync_;

  alignas(kCacheLineBytes) std::atomic<client_id> client_id_sequence_;

  // This counts downwards through negative numbers.
  alignas(kCacheLineBytes) std::atomic<std::int64_t> message_id_sequence_;

  // Disallow copy and assign:
  NativeTransactionLog(const NativeTransactionLog &orig) = delete;
  NativeTransactionLog& operator=(const NativeTransactionLog &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_NATIVE_TRANSACTION_LOG_H_
