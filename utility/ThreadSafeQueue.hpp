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

#ifndef QUICKSTEP_UTILITY_THREAD_SAFE_QUEUE_HPP_
#define QUICKSTEP_UTILITY_THREAD_SAFE_QUEUE_HPP_

#include <atomic>
#include <cstddef>
#include <queue>
#include <utility>

#include "threading/ConditionVariable.hpp"
#include "threading/Mutex.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Threadsafe FIFO queue that can be used with multiple producers and/or
 *        consumers. Internally synchronized by means of a Mutex and CV.
 **/
template <typename T>
class ThreadSafeQueue {
 public:
  typedef typename std::queue<T>::size_type size_type;

  /**
   * @brief Construct an initially empty queue.
   **/
  ThreadSafeQueue()
      : num_waiters_(0) {
    queue_nonempty_condition_ = queue_mutex_.createConditionVariable();
  }

  /**
   * @brief Check if the queue has any items in it.
   * @note This method locks a Mutex.
   *
   * @return true if queue is empty.
   **/
  bool empty() const {
    MutexLock lock(queue_mutex_);
    return internal_queue_.empty();
  }

  /**
   * @brief Check how many items are currently in the queue.
   * @note This method locks a Mutex.
   *
   * @return The number of items currently in this queue.
   **/
  size_type size() const {
    MutexLock lock(queue_mutex_);
    return internal_queue_.size();
  }

  /**
   * @brief Push a new item on the end of the queue.
   *
   * @param element The new item to push on the queue. It is copied.
   **/
  void push(const T& element) {
    MutexLock lock(queue_mutex_);
    internal_queue_.push(element);
    queue_nonempty_condition_->signalAll();
  }

  /**
   * @brief Push a new item on the end of the queue.
   *
   * @param element The new item to push on the queue. It is moved from.
   **/
  void push(T &&element) {
    MutexLock lock(queue_mutex_);
    internal_queue_.emplace(std::move(element));
    queue_nonempty_condition_->signalAll();
  }

  /**
   * @brief Pushes multiple elements of T atomically onto the ThreadSafeQueue.
   * @warning This implementation moves elements and pushes them on the
   *          ThreadSafeQueue. The original queue of elements gets empty.
   *
   * @param elements A pointer to the queue of elements to be pushed.
   **/
  void pushMultiple(std::queue<T> *elements) {
    MutexLock lock(queue_mutex_);
    while (!elements->empty()) {
      internal_queue_.emplace(std::move(elements->front()));
      elements->pop();
    }
    queue_nonempty_condition_->signalAll();
  }

  /**
   * @brief Pop a single item from the head of the queue. This version blocks
   *        indefinitely until an item is available.
   *
   * @return The element popped from the front of the queue.
   **/
  T popOne() {
    MutexLock lock(queue_mutex_);
    const bool initially_empty = internal_queue_.empty();
    num_waiters_.fetch_add(initially_empty, std::memory_order_relaxed);
    while (internal_queue_.empty()) {
      queue_nonempty_condition_->await();
    }
    num_waiters_.fetch_sub(initially_empty, std::memory_order_relaxed);
    T popped_value(internal_queue_.front());
    internal_queue_.pop();
    return popped_value;
  }

  /**
   * @brief Pop a single item from the head of the queue if an item is
   *        available at the time of the call.
   *
   * @param destination_ptr A pointer to a T that is overwritten with the first
   *        item in the queue if any is available.
   * @return true if an item was successfully popped, false if queue was empty.
   **/
  bool popOneIfAvailable(T *destination_ptr) {
    MutexLock lock(queue_mutex_);
    if (internal_queue_.empty()) {
      return false;
    } else {
      *destination_ptr = internal_queue_.front();
      internal_queue_.pop();
      return true;
    }
  }

  /**
   * @brief Check how many threads are waiting (blocked) in popOne().
   * @warning This is informational only, and should be regarded as
   *          approximate.
   *
   * @return The number of threads waiting in popOne()
   **/
  std::size_t numWaitingThreads() const {
    return num_waiters_.load(std::memory_order_relaxed);
  }

 private:
  std::queue<T> internal_queue_;
  mutable Mutex queue_mutex_;
  ConditionVariable *queue_nonempty_condition_;
  std::atomic<std::size_t> num_waiters_;

  DISALLOW_COPY_AND_ASSIGN(ThreadSafeQueue);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_THREAD_SAFE_QUEUE_HPP_
