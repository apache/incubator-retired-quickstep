// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "tmb/internal/tree_receiver_message_queue.h"

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

#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/container_pusher.h"
#include "tmb/internal/iterator_adapter.h"
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/queued_message.h"

namespace tmb {
namespace internal {

template <bool memory_mirror_version>
void TreeReceiverMessageQueue<memory_mirror_version>::Push(
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
void TreeReceiverMessageQueue<memory_mirror_version>::Push(
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
std::size_t TreeReceiverMessageQueue<memory_mirror_version>::Pop(
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    ContainerPusher *pusher,
    MemoryMirrorDeleteBatch *delete_batch) {
  std::unique_lock<std::mutex> lock(queue_mutex_);

  // Burn through any expired or cancelled messages at the front of the queue.
  DiscardDeadMessages(delete_batch);

  // Wait for a message that satisfies minimum_priority to show up at the
  // front of the queue.
  while (internal_queue_.empty()
         || internal_queue_.begin()->GetPriority() < minimum_priority) {
    minimum_waiting_priority_ = minimum_priority;
    message_available_condition_.wait(lock);

    // Burn through expired or cancelled messages.
    DiscardDeadMessages(delete_batch);
  }

  // There is at least one suitable message at the front of the queue.
  std::multiset<QueuedMessage, std::greater<QueuedMessage>>::const_iterator it
      = internal_queue_.begin();
  if (delete_immediately) {
    if (memory_mirror_version) {
      delete_batch->AddMessage(*it);
    }
    pusher->Push(const_cast<QueuedMessage&>(*it).ReleaseAnnotatedMessage());
    EraseAndAdvance(&it);
  } else {
    pusher->Push(it->CopyAnnotatedMessage());
    received_message_iterators_.emplace(it->GetMessageID(), it);
    ++it;
  }
  std::size_t popped = 1;

  // Get additional messages (up to 'max_messages') that are already on the
  // queue and which satisfy 'minimum_priority'.
  while (((max_messages == 0) || (popped < max_messages))
         && (it != internal_queue_.end())
         && (it->GetPriority() >= minimum_priority)) {
    if (it->ExpiredOrCancelled()) {
      if (memory_mirror_version && !it->Cancelled()) {
        delete_batch->AddMessage(*it);
      }
      EraseAndAdvance(&it);
    } else if (delete_immediately) {
      if (memory_mirror_version) {
        delete_batch->AddMessage(*it);
      }
      pusher->Push(const_cast<QueuedMessage&>(*it).ReleaseAnnotatedMessage());
      EraseAndAdvance(&it);
      ++popped;
    } else {
      pusher->Push(it->CopyAnnotatedMessage());
      received_message_iterators_.emplace(it->GetMessageID(), it);
      ++it;
      ++popped;
    }
  }

  minimum_waiting_priority_ = kAboveMaxPriority;
  return popped;
}

template <bool memory_mirror_version>
std::size_t TreeReceiverMessageQueue<memory_mirror_version>::PopIfAvailable(
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    ContainerPusher *pusher,
    MemoryMirrorDeleteBatch *delete_batch) {
  std::lock_guard<std::mutex> lock(queue_mutex_);

  std::multiset<QueuedMessage, std::greater<QueuedMessage>>::const_iterator it
      = internal_queue_.begin();
  std::size_t popped = 0;
  while (((max_messages == 0) || (popped < max_messages))
         && (it != internal_queue_.end())
         && (it->GetPriority() >= minimum_priority)) {
    if (it->ExpiredOrCancelled()) {
      if (memory_mirror_version && !it->Cancelled()) {
        delete_batch->AddMessage(*it);
      }
      EraseAndAdvance(&it);
    } else if (delete_immediately) {
      if (memory_mirror_version) {
        delete_batch->AddMessage(*it);
      }
      pusher->Push(const_cast<QueuedMessage&>(*it).ReleaseAnnotatedMessage());
      EraseAndAdvance(&it);
      ++popped;
    } else {
      pusher->Push(it->CopyAnnotatedMessage());
      received_message_iterators_.emplace(it->GetMessageID(), it);
      ++it;
      ++popped;
    }
  }

  return popped;
}

template <bool memory_mirror_version>
void TreeReceiverMessageQueue<memory_mirror_version>::DeleteReceivedMessages(
    internal::IteratorAdapter<const AnnotatedMessage> *adapter,
    MemoryMirrorDeleteBatch *delete_batch) {
  std::lock_guard<std::mutex> lock(queue_mutex_);

  while (adapter->Valid()) {
    std::unordered_map<
        std::int64_t,
        std::multiset<QueuedMessage,
                      std::greater<QueuedMessage>>::iterator>::iterator it
            = received_message_iterators_.find(
                (*adapter)->deletion_token.message_id);
    if (it != received_message_iterators_.end()) {
      if (memory_mirror_version && !it->second->Cancelled()) {
        delete_batch->AddMessage(*(it->second));
      }
      internal_queue_.erase(it->second);
      received_message_iterators_.erase(it);
      queue_length_.fetch_sub(1, std::memory_order_relaxed);
    }
    adapter->Next();
  }
}

template <bool memory_mirror_version>
void TreeReceiverMessageQueue<memory_mirror_version>
    ::DeleteReceivedMessagesById(
        const std::vector<std::int64_t> &message_ids,
        MemoryMirrorDeleteBatch *delete_batch) {
  std::lock_guard<std::mutex> lock(queue_mutex_);

  for (const std::int64_t message_id : message_ids) {
    // First, look in the known received messages (the ordinary case).
    std::unordered_map<
        std::int64_t,
        std::multiset<QueuedMessage,
                      std::greater<QueuedMessage>>::iterator>::iterator it
            = received_message_iterators_.find(message_id);
    if (it != received_message_iterators_.end()) {
      if (memory_mirror_version && !it->second->Cancelled()) {
        delete_batch->AddMessage(*(it->second));
      }
      internal_queue_.erase(it->second);
      received_message_iterators_.erase(it);
      queue_length_.fetch_sub(1, std::memory_order_relaxed);
    } else if (memory_mirror_version) {
      // If the bus was destroyed and later reconstituted, but clients still
      // hold received non-deleted messages from the previous incarnation of
      // the bus, then we need to scan through the whole queue to find it.
      for (std::multiset<QueuedMessage, std::greater<QueuedMessage>>::iterator
               queue_it = internal_queue_.begin();
           queue_it != internal_queue_.end();
           ++queue_it) {
        if (queue_it->GetMessageID() == message_id) {
          delete_batch->AddMessage(*queue_it);
          internal_queue_.erase(queue_it);
          queue_length_.fetch_sub(1, std::memory_order_relaxed);
          break;
        }
      }
    }
  }
}

template <bool memory_mirror_version>
void TreeReceiverMessageQueue<memory_mirror_version>::DiscardDeadMessages(
    MemoryMirrorDeleteBatch *delete_batch) {
  std::multiset<QueuedMessage, std::greater<QueuedMessage>>::const_iterator it
      = internal_queue_.begin();
  while ((it != internal_queue_.end()) && it->ExpiredOrCancelled()) {
    if (memory_mirror_version && !it->Cancelled()) {
      delete_batch->AddMessage(*it);
    }
    EraseAndAdvance(&it);
  }
}

template <bool memory_mirror_version>
void TreeReceiverMessageQueue<memory_mirror_version>::EraseAndAdvance(
    std::multiset<QueuedMessage, std::greater<QueuedMessage>>::iterator *it) {
  std::unordered_map<
      std::int64_t,
      std::multiset<QueuedMessage,
                    std::greater<QueuedMessage>>::iterator>::iterator
          received_it
              = received_message_iterators_.find((*it)->GetMessageID());
  if (received_it != received_message_iterators_.end()) {
    received_message_iterators_.erase(received_it);
  }
  *it = internal_queue_.erase(*it);
  queue_length_.fetch_sub(1, std::memory_order_relaxed);
}

template <bool memory_mirror_version>
void TreeReceiverMessageQueue<memory_mirror_version>::Drain(
    MemoryMirrorDeleteBatch *delete_batch) {
  if (memory_mirror_version) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    for (const QueuedMessage &message : internal_queue_) {
      if (!message.Cancelled()) {
        delete_batch->AddMessage(message);
      }
    }
    internal_queue_.clear();
    queue_length_.store(0, std::memory_order_relaxed);
  }
}

// Compile both versions of class.
template class TreeReceiverMessageQueue<false>;
template class TreeReceiverMessageQueue<true>;

}  // namespace internal
}  // namespace tmb
