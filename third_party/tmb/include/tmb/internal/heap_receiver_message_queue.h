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

#ifndef TMB_INTERNAL_HEAP_RECEIVER_MESSAGE_QUEUE_H_
#define TMB_INTERNAL_HEAP_RECEIVER_MESSAGE_QUEUE_H_

#include <atomic>
#include <condition_variable>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>               // NOLINT(build/c++11)
#include <queue>
#include <utility>
#include <vector>

#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/queued_message.h"

namespace tmb {
namespace internal {

class ContainerPusher;
class MemoryMirrorDeleteBatch;

/**
 * @brief A threadsafe queue of incoming messages for a client. Messages are
 *        popped in order of descending priority, and messages that expire
 *        while queued are silently discarded.
 * @note This version is internally structured as a heap and does NOT support
 *       deleting messages separately from popping (i.e. all messages are
 *       removed from the queue as they are popped). See also
 *       TreeReceiverMessageQueue.
 **/
template <bool memory_mirror_version>
class HeapReceiverMessageQueue {
 public:
  HeapReceiverMessageQueue()
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
  std::priority_queue<QueuedMessage> internal_queue_;
  std::int16_t minimum_waiting_priority_;
  std::mutex queue_mutex_;
  std::condition_variable message_available_condition_;

  std::atomic<std::size_t> queue_length_;

  HeapReceiverMessageQueue(const HeapReceiverMessageQueue &orig) = delete;
  HeapReceiverMessageQueue& operator=(const HeapReceiverMessageQueue &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_HEAP_RECEIVER_MESSAGE_QUEUE_H_
