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

#include "tmb/internal/heap_receiver_message_queue.h"

#include <atomic>
#include <condition_variable>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>               // NOLINT(build/c++11)
#include <queue>
#include <utility>

#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/container_pusher.h"
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/queued_message.h"

namespace tmb {
namespace internal {

template <bool memory_mirror_version>
void HeapReceiverMessageQueue<memory_mirror_version>::Push(
    const QueuedMessage &message) {
  bool signal = false;
  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    signal = message.GetPriority() >= minimum_waiting_priority_;
    internal_queue_.emplace(message);
  }
  queue_length_.fetch_add(1, std::memory_order_relaxed);
  if (signal) {
    message_available_condition_.notify_all();
  }
}

template <bool memory_mirror_version>
void HeapReceiverMessageQueue<memory_mirror_version>::Push(
    QueuedMessage &&message) {  // NOLINT(build/c++11)
  bool signal = false;
  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    signal = message.GetPriority() >= minimum_waiting_priority_;
    internal_queue_.emplace(std::move(message));
  }
  queue_length_.fetch_add(1, std::memory_order_relaxed);
  if (signal) {
    message_available_condition_.notify_all();
  }
}

template <bool memory_mirror_version>
std::size_t HeapReceiverMessageQueue<memory_mirror_version>::Pop(
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    ContainerPusher *pusher,
    MemoryMirrorDeleteBatch *delete_batch) {
  std::unique_lock<std::mutex> lock(queue_mutex_);

  // Burn through any expired messages at the front of the queue.
  while ((!internal_queue_.empty()) && internal_queue_.top().Expired()) {
    if (memory_mirror_version) {
      delete_batch->AddMessage(internal_queue_.top());
    }
    internal_queue_.pop();
    queue_length_.fetch_sub(1, std::memory_order_relaxed);
  }

  // Wait for a message that satisfies minimum_priority to show up at the
  // front of the queue.
  while (internal_queue_.empty()
         || internal_queue_.top().GetPriority() < minimum_priority) {
    minimum_waiting_priority_ = minimum_priority;
    message_available_condition_.wait(lock);

    // Burn through expired or cancelled messages.
    while ((!internal_queue_.empty())
           && internal_queue_.top().ExpiredOrCancelled()) {
      if (memory_mirror_version && !internal_queue_.top().Cancelled()) {
        // Only add to the delete batch if message is expired but not
        // cancelled (the client who cancelled the message is responsible for
        // deleting it from the underlying storage.
        delete_batch->AddMessage(internal_queue_.top());
      }
      internal_queue_.pop();
      queue_length_.fetch_sub(1, std::memory_order_relaxed);
    }
  }

  // There is at least one suitable message at the front of the queue.
  if (memory_mirror_version) {
    delete_batch->AddMessage(internal_queue_.top());
  }
  pusher->Push(const_cast<QueuedMessage&>(internal_queue_.top())
                   .ReleaseAnnotatedMessage());
  internal_queue_.pop();
  queue_length_.fetch_sub(1, std::memory_order_relaxed);
  std::size_t popped = 1;

  // Get additional messages (up to 'max_messages') that are already on the
  // queue and which satisfy 'minimum_priority'.
  while (((max_messages == 0) || (popped < max_messages))
         && (!internal_queue_.empty())
         && (internal_queue_.top().GetPriority() >= minimum_priority)) {
    // Discard expired or cancelled messages (the "first" messages at a lower
    // priority may already be expired).
    if (memory_mirror_version && !internal_queue_.top().Cancelled()) {
      delete_batch->AddMessage(internal_queue_.top());
    }
    if (!internal_queue_.top().ExpiredOrCancelled()) {
      pusher->Push(const_cast<QueuedMessage&>(internal_queue_.top())
                       .ReleaseAnnotatedMessage());
      ++popped;
    }
    internal_queue_.pop();
    queue_length_.fetch_sub(1, std::memory_order_relaxed);
  }

  minimum_waiting_priority_ = kAboveMaxPriority;
  return popped;
}

template <bool memory_mirror_version>
std::size_t HeapReceiverMessageQueue<memory_mirror_version>::PopIfAvailable(
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    ContainerPusher *pusher,
    MemoryMirrorDeleteBatch *delete_batch) {
  std::lock_guard<std::mutex> lock(queue_mutex_);

  std::size_t popped = 0;
  while (((max_messages == 0) || (popped < max_messages))
         && (!internal_queue_.empty())
         && (internal_queue_.top().GetPriority() >= minimum_priority)) {
    // Discard expired or cancelled messages.
    if (memory_mirror_version && !internal_queue_.top().Cancelled()) {
      delete_batch->AddMessage(internal_queue_.top());
    }
    if (!internal_queue_.top().ExpiredOrCancelled()) {
      pusher->Push(const_cast<QueuedMessage&>(internal_queue_.top())
                       .ReleaseAnnotatedMessage());
      ++popped;
    }
    internal_queue_.pop();
    queue_length_.fetch_sub(1, std::memory_order_relaxed);
  }

  return popped;
}

template <bool memory_mirror_version>
void HeapReceiverMessageQueue<memory_mirror_version>::Drain(
    MemoryMirrorDeleteBatch *delete_batch) {
  if (memory_mirror_version) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    while (!internal_queue_.empty()) {
      if (!internal_queue_.top().Cancelled()) {
        delete_batch->AddMessage(internal_queue_.top());
      }
      internal_queue_.pop();
      queue_length_.fetch_sub(1, std::memory_order_relaxed);
    }
  }
}

// Compile both versions of class.
template class HeapReceiverMessageQueue<false>;
template class HeapReceiverMessageQueue<true>;

}  // namespace internal
}  // namespace tmb
