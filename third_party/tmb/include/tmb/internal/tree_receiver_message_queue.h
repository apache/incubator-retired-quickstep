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

#ifndef TMB_INTERNAL_TREE_RECEIVER_MESSAGE_QUEUE_H_
#define TMB_INTERNAL_TREE_RECEIVER_MESSAGE_QUEUE_H_

#include <atomic>
#include <condition_variable>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <mutex>               // NOLINT(build/c++11)
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/queued_message.h"

namespace tmb {
namespace internal {

class ContainerPusher;
template <typename T> class IteratorAdapter;
class MemoryMirrorDeleteBatch;

/**
 * @brief A threadsafe queue of incoming messages for a client. Messages are
 *        popped in order of descending priority, and messages that expire
 *        while queued are silently discarded.
 * @note This version is internally structured as a binary tree and supports
 *       popping and deleting messages separately. See also
 *       HeapReceiverMessageQueue.
 **/
template <bool memory_mirror_version>
class TreeReceiverMessageQueue {
 public:
  TreeReceiverMessageQueue()
      : minimum_waiting_priority_(kAboveMaxPriority),
        queue_length_(0) {
  }

  void Push(const QueuedMessage &message);

  void Push(QueuedMessage &&message);  // NOLINT(build/c++11)

  std::size_t Pop(const Priority minimum_priority,
                  const std::size_t max_messages,
                  const bool delete_immediately,
                  ContainerPusher *pusher,
                  MemoryMirrorDeleteBatch *delete_batch);

  std::size_t PopIfAvailable(const Priority minimum_priority,
                             const std::size_t max_messages,
                             const bool delete_immediately,
                             ContainerPusher *pusher,
                             MemoryMirrorDeleteBatch *delete_batch);

  void DeleteReceivedMessages(
      internal::IteratorAdapter<const AnnotatedMessage> *adapter,
      MemoryMirrorDeleteBatch *delete_batch);

  void DeleteReceivedMessagesById(
      const std::vector<std::int64_t> &message_ids,
      MemoryMirrorDeleteBatch *delete_batch);

  void UnsafeBulkInsert(std::vector<QueuedMessage> *messages) {
    for (QueuedMessage &msg : *messages) {
      internal_queue_.emplace(std::move(msg));
    }
  }

  void Drain(MemoryMirrorDeleteBatch *delete_batch);

  inline std::size_t Length() const {
    return queue_length_.load(std::memory_order_relaxed);
  }

 private:
  // Must lock 'queue_mutex_' before calling this method. Discards expired and
  // cancelled messages from the front of the queue.
  void DiscardDeadMessages(MemoryMirrorDeleteBatch *delete_batch);

  // Must lock 'queue_mutex_' before calling this method. Erase '*it' from
  // 'internal_queue_', and also from 'received_message_iterators_' if it is
  // present there.
  void EraseAndAdvance(
      std::multiset<QueuedMessage, std::greater<QueuedMessage>>::iterator *it);

  std::multiset<QueuedMessage, std::greater<QueuedMessage>> internal_queue_;

  std::unordered_map<std::int64_t,
                     std::multiset<QueuedMessage,
                                   std::greater<QueuedMessage>>::iterator>
      received_message_iterators_;

  std::int16_t minimum_waiting_priority_;
  std::mutex queue_mutex_;
  std::condition_variable message_available_condition_;

  std::atomic<std::size_t> queue_length_;

  TreeReceiverMessageQueue(const TreeReceiverMessageQueue &orig) = delete;
  TreeReceiverMessageQueue& operator=(const TreeReceiverMessageQueue &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_TREE_RECEIVER_MESSAGE_QUEUE_H_
