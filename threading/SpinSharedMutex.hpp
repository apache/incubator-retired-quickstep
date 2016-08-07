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

#ifndef QUICKSTEP_THREADING_SPIN_SHARED_MUTEX_HPP_
#define QUICKSTEP_THREADING_SPIN_SHARED_MUTEX_HPP_

#include <atomic>
#include <cstdint>

#include "threading/Mutex.hpp"
#include "threading/SharedMutex.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief SharedMutex implementation as a pure user-space spinlock. May be more
 *        efficient than an ordinary SharedMutex if it is rarely locked in
 *        exclusive mode (in particular, SpinSharedMutex is expected to
 *        outperform SharedMutex when locking/unlocking in shared mode is
 *        frequent but locking in exclusive mode is rare).
 *
 * @note In some cases, SpinSharedMutex will perform better if it is aligned on
 *       its own cache line to avoid false sharing.
 *
 * @param yield_when_contended If true, lock() and lockShared() will yield when
 *        the SpinSharedMutex is under contention. If false, they will simply
 *        simply spin without yielding. If contention is expected to be brief,
 *        then yield_when_contended should be false, otherwise it should be
 *        true.
 **/
template <bool yield_when_contended>
class SpinSharedMutex {
 public:
  SpinSharedMutex()
      : state_(0) {
  }

  inline void lock() {
    // First, attempt to set the write bit. If it was already set, spin until
    // it is cleared. Use 'std::memory_order_acquire' so that any and all
    // writes made by threads that previously locked this SpinSharedMutex will
    // be guaranteed to be visible.
    std::uint32_t observed_state = state_.fetch_or(kWriteBit, std::memory_order_acquire);
    while (observed_state & kWriteBit) {
      if (yield_when_contended) {
        ThreadUtil::Yield();
      }
      observed_state = state_.fetch_or(kWriteBit, std::memory_order_acquire);
    }
    observed_state |= kWriteBit;

    // Spin until there are no active readers.
    while (observed_state != kWriteBit) {
      if (yield_when_contended) {
        ThreadUtil::Yield();
      }
      // Use 'std::memory_order_relaxed' to observe count of readers. This is
      // safe because since the write bit is now set, no other writer can come
      // in ahead of this one, and readers should not make any writes to memory
      // that this writer would need to observe.
      observed_state = state_.load(std::memory_order_relaxed);
    }

    // Now locked in exclusive mode.
  }

  inline void unlock() {
    // Clear the write bit. Use 'std::memory_order_release' so that any thread
    // that subsequently locks this SpinSharedMutex (by modifying state with
    // 'std::memory_order_acquire') is guaranteed to observe all writes made
    // by this writer thread before unlocking.
    state_.fetch_and(~kWriteBit, std::memory_order_release);
  }

  inline void lockShared() {
    // Increment the reader count. Use 'std::memory_order_acquire' so that any
    // previous writes made by writers while holding this SpinSharedMutex
    // exclusively are guaranteed to be visible.
    while (state_.fetch_add(1, std::memory_order_acquire) & kWriteBit) {
      // The write bit was set, so there is either an active or a pending
      // writer. Decrement reader count and spin until the write bit is clear.
      //
      // Using 'std::memory_order_relaxed' is safe because this reader thread
      // obviously didn't make any writes to other memory since incrementing the
      // reader count that other threads would need to observe.
      std::uint32_t observed_state = state_.fetch_sub(1, std::memory_order_relaxed);
      while (observed_state & kWriteBit) {
        if (yield_when_contended) {
          ThreadUtil::Yield();
        }
        // Only use 'std::memory_order_relaxed' to check the write bit, avoiding
        // possible overhead of full synchronization while spinning. This is
        // safe, because once the write bit is clear the outer while loop will
        // increment the reader count with 'std::memory_order_acquire' and
        // ensure that any of the writer's modifications to shared memory are
        // visible.
        observed_state = state_.load(std::memory_order_relaxed);
      }
    }

    // Now locked in shared mode.
  }

  inline void unlockShared() {
    // Decrement the reader count. Use 'std::memory_order_relaxed', because a
    // reader thread is not allowed to make any writes to shared memory
    // protected by this SpinSharedMutex that other threads would need to
    // observe.
    state_.fetch_sub(1, std::memory_order_relaxed);
  }

 private:
  static constexpr std::uint32_t kWriteBit = 0x80000000u;

  // Top bit of state (kWriteBit) indicates an active or pending exclusive
  // lock. Bottom 31 bits are a count of shared locks.
  std::atomic<std::uint32_t> state_;

  DISALLOW_COPY_AND_ASSIGN(SpinSharedMutex);
};

template <bool yield_when_contended>
using SpinSharedMutexExclusiveLock
    = MutexLockImpl<SpinSharedMutex<yield_when_contended>, true>;

template <bool yield_when_contended, bool actually_lock>
using StaticConditionalSpinSharedMutexExclusiveLock
    = MutexLockImpl<SpinSharedMutex<yield_when_contended>, actually_lock>;

template <bool yield_when_contended>
using DynamicConditionalSpinSharedMutexExclusiveLock
    = DynamicConditionalMutexLockImpl<SpinSharedMutex<yield_when_contended>>;

template <bool yield_when_contended>
using SpinSharedMutexSharedLock
    = SharedMutexSharedLockImpl<SpinSharedMutex<yield_when_contended>, true>;

template <bool yield_when_contended, bool actually_lock>
using StaticConditionalSpinSharedMutexSharedLock
    = SharedMutexSharedLockImpl<SpinSharedMutex<yield_when_contended>, actually_lock>;

template <bool yield_when_contended>
using DynamicConditionalSpinSharedMutexSharedLock
    = DynamicConditionalSharedMutexSharedLockImpl<SpinSharedMutex<yield_when_contended>>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_SPIN_SHARED_MUTEX_HPP_
