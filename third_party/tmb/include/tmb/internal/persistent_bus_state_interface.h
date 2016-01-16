// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_PERSISTENT_BUS_STATE_INTERFACE_H_
#define TMB_INTERNAL_PERSISTENT_BUS_STATE_INTERFACE_H_

#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

#include "tmb/id_typedefs.h"

#include "tmb/internal/queued_message.h"

namespace tmb {

struct AnnotatedMessage;
class CancellationToken;

namespace internal {

class MemoryMirrorDeleteBatch;
class QueuedMessage;
template <typename> class IteratorAdapter;
template <typename> class ThreadsafeSet;

/**
 * @brief Abstract interface class for persistent, durable underlying storage,
 *        suitable for use with MemoryMirrorMessageBus.
 * @warning Do NOT call any of this interface's methods directly. They should
 *          be used only by MemoryMirrorMessageBus.
 **/
class PersistentBusStateInterface {
 public:
  struct TempClientHandle {
    TempClientHandle()
        : sendable(new std::unordered_set<message_type_id>()),
          receivable(new std::unordered_set<message_type_id>()),
          sent_message_counter(0) {
    }

    std::unique_ptr<std::unordered_set<message_type_id>> sendable;
    std::unique_ptr<std::unordered_set<message_type_id>> receivable;
    std::vector<QueuedMessage> queued_messages;
    std::uint32_t sent_message_counter;
  };

  PersistentBusStateInterface() {
  }

  virtual ~PersistentBusStateInterface() {
  }

  virtual void ResetBusUnchecked() = 0;

  virtual client_id ConnectClientUnchecked() = 0;

  virtual bool DisconnectRequiresQueueDrain() const = 0;

  virtual void DisconnectClientUnchecked(
      const client_id client,
      const ThreadsafeSet<message_type_id> &sendable_set,
      const ThreadsafeSet<message_type_id> &receivable_set,
      const MemoryMirrorDeleteBatch *queued_message_batch) = 0;

  virtual void InsertSendableUnchecked(const client_id client,
                                       const message_type_id message_type) = 0;

  virtual void InsertMessageTypeUnchecked(const message_type_id message_type)
      = 0;

  virtual void InsertReceivableUnchecked(
      const client_id client,
      const message_type_id message_type,
      const ThreadsafeSet<message_type_id> &receivable_set) = 0;

  // Some implementations may use '*sender_message_counter' together with the
  // sender ID embedded in 'message' to generate a unique message ID without
  // global coordination. If a particular implementation does not support this
  // feature, '*sender_message_counter' is ignored.
  virtual std::int64_t SendUnchecked(
      const QueuedMessage &message,
      const std::vector<client_id> &verified_receivers,
      CancellationToken *cancellation_token,
      std::uint32_t *sender_message_counter) = 0;

  virtual internal::MemoryMirrorDeleteBatch* CreateDeleteBatch(
      const client_id receiver_id) const = 0;

  virtual void DeleteMessagesUnchecked(
      const MemoryMirrorDeleteBatch &delete_batch) = 0;

  virtual void CancelMessageUnchecked(
      const CancellationToken &cancellation_token) = 0;

  virtual void CancelMessagesUnchecked(
      IteratorAdapter<const AnnotatedMessage> *adapter) = 0;

  virtual std::unordered_map<client_id, TempClientHandle>* LoadState() = 0;

 private:
  // Disallow copy and assign:
  PersistentBusStateInterface(
      const PersistentBusStateInterface &orig) = delete;
  PersistentBusStateInterface& operator=(
      const PersistentBusStateInterface &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_PERSISTENT_BUS_STATE_INTERFACE_H_
