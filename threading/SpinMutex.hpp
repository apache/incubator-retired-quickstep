/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_THREADING_SPIN_MUTEX_HPP_
#define QUICKSTEP_THREADING_SPIN_MUTEX_HPP_

#include <atomic>

#include "threading/Mutex.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Mutex implementation as a pure user-space spinlock. May be more
 *        efficient than an ordinary Mutex if locks are usually held briefly.
 *
 * @note Unlike a regular Mutex, SpinMutex can not create condition variables.
 * @note In some cases, SpinMutex will perform better if it is aligned on its
 *       own cache line to avoid false sharing.
 **/
class SpinMutex {
 public:
  SpinMutex() : locked_(false) {
  }

  /**
   * @note This call does NOT yield when contended. SpinMutex is intended
   *       mainly for cases where locks are held briefly and it is better to
   *       simply spin for a short time rather than involving the kernel's
   *       scheduler. Otherwise, a regular Mutex (using the futex mechanism on
   *       Linux) is a more appropriate choice.
   **/
  inline void lock() {
    bool previous_locked = false;
    while (!locked_.compare_exchange_weak(previous_locked,
                                          true,
                                          std::memory_order_acquire,
                                          std::memory_order_relaxed)) {
      previous_locked = false;
    }
  }

  inline void unlock() {
    locked_.store(false, std::memory_order_release);
  }

 private:
  std::atomic<bool> locked_;

  DISALLOW_COPY_AND_ASSIGN(SpinMutex);
};

typedef MutexLockImpl<SpinMutex, true> SpinMutexLock;
template <bool actually_lock> using StaticConditionalSpinMutexLock
    = MutexLockImpl<SpinMutex, actually_lock>;
typedef DynamicConditionalMutexLockImpl<SpinMutex> DynamicConditionalSpinMutexLock;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_SPIN_MUTEX_HPP_
