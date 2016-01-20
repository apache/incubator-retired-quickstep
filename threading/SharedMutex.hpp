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

#ifndef QUICKSTEP_THREADING_SHARED_MUTEX_HPP_
#define QUICKSTEP_THREADING_SHARED_MUTEX_HPP_

#include "threading/Mutex.hpp"
#include "threading/ThreadingConfig.h"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief A shared mutex, also known as an RW-lock. May be locked in exclusive
 *        (write) mode, which prevents any other thread from locking, or shared
 *        (read) mode, which allows an unlimited number of threads to share the
 *        mutex, so long as none are in exclusive mode.
 * @note This interface exists to provide a central point of documentation for
 *       platform-specific SharedMutex implementations, you should never use it
 *       directly. Instead, simply use the SharedMutex class, which will be
 *       typedefed to the appropriate implementation.
 **/
class SharedMutexInterface : public MutexInterface {
 public:
  /**
   * @brief Lock this mutex in shared mode. If the mutex is locked in exclusive
   *        mode by another thread, execution will block until the mutex
   *        becomes available. Any number of threads may simultaneously lock
   *        the mutex in shared mode.
   **/
  virtual void lockShared() = 0;

  /**
   * @brief Release a shared-mode lock on this mutex. It is an error to call
   *        this from a thread that does not hold a shared-mode lock on this
   *        mutex.
   **/
  virtual void unlockShared() = 0;
};

/** @} */

}  // namespace quickstep

#ifdef QUICKSTEP_HAVE_CPP11_THREADS
#  if defined(QUICKSTEP_HAVE_CPP17_SHARED_MUTEX)
#    include "threading/cpp11/cpp17/SharedMutex.hpp"
#  elif defined(QUICKSTEP_HAVE_CPP14_SHARED_TIMED_MUTEX)
#    include "threading/cpp11/cpp14/SharedMutex.hpp"
#  else
#    include "threading/cpp11/SharedMutex.hpp"
#  endif
#endif

#ifdef QUICKSTEP_HAVE_POSIX_THREADS
#include "threading/posix/SharedMutex.hpp"
#endif

#ifdef QUICKSTEP_HAVE_WINDOWS_THREADS
#include "threading/windows/SharedMutex.hpp"
#endif

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief A scoped exclusive lock-holder for a SharedMutex. Locks a SharedMutex
 *        in exclusive mode when it is constructed, and unlocks when it goes
 *        out of scope.
 **/
typedef MutexLockImpl<SharedMutex, true> SharedMutexExclusiveLock;
template <bool actually_lock> using StaticConditionalSharedMutexExclusiveLock
    = MutexLockImpl<SharedMutex, actually_lock>;
typedef DynamicConditionalMutexLockImpl<SharedMutex>
    DynamicConditionalSharedMutexExclusiveLock;

/**
 * @brief A scoped shared lock-holder for a SharedMutex. Locks a SharedMutex in
 *        shared mode when it is constructed, and unlocks when it goes out of
 *        scope.
 **/
template <typename SharedMutexT, bool actually_lock>
class SharedMutexSharedLockImpl {
 public:
  explicit inline SharedMutexSharedLockImpl(SharedMutexT &mutex)  // NOLINT(runtime/references)
      : mutex_ptr_(&mutex) {
    if (actually_lock) {
      mutex_ptr_->lockShared();
    }
  }

  inline ~SharedMutexSharedLockImpl() {
    if (actually_lock) {
      mutex_ptr_->unlockShared();
    }
  }

 private:
  SharedMutexT *mutex_ptr_;

  DISALLOW_COPY_AND_ASSIGN(SharedMutexSharedLockImpl);
};

// Partial specialization: do nothing and store nothing if actually_lock = false.
template <typename SharedMutexT>
class SharedMutexSharedLockImpl<SharedMutexT, false> {
 public:
  explicit inline SharedMutexSharedLockImpl(SharedMutexT &mutex) {  // NOLINT(runtime/references)
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SharedMutexSharedLockImpl);
};

template <typename SharedMutexT>
class DynamicConditionalSharedMutexSharedLockImpl {
 public:
  inline DynamicConditionalSharedMutexSharedLockImpl(SharedMutexT &mutex,  // NOLINT(runtime/references)
                                                     const bool actually_lock)
      : mutex_ptr_(&mutex),
        actually_lock_(actually_lock) {
    if (actually_lock_) {
      mutex_ptr_->lockShared();
    }
  }

  inline ~DynamicConditionalSharedMutexSharedLockImpl() {
    if (actually_lock_) {
      mutex_ptr_->unlockShared();
    }
  }

 private:
  SharedMutexT *mutex_ptr_;
  const bool actually_lock_;

  DISALLOW_COPY_AND_ASSIGN(DynamicConditionalSharedMutexSharedLockImpl);
};

typedef SharedMutexSharedLockImpl<SharedMutex, true> SharedMutexSharedLock;
template <bool actually_lock> using StaticConditionalSharedMutexSharedLock
    = SharedMutexSharedLockImpl<SharedMutex, actually_lock>;
typedef DynamicConditionalSharedMutexSharedLockImpl<SharedMutex>
    DynamicConditionalSharedMutexSharedLock;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_SHARED_MUTEX_HPP_
