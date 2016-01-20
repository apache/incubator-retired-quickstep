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

#ifndef QUICKSTEP_THREADING_MUTEX_HPP_
#define QUICKSTEP_THREADING_MUTEX_HPP_

#include "threading/ThreadingConfig.h"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief A Mutex. A normal Mutex may only be locked once, a RecursiveMutex may
 *        be locked an unlimited number of times by one thread, so long as it
 *        is unlocked the same number of times.
 * @note This interface exists to provide a central point of documentation for
 *       platform-specific Mutex and RecursiveMutex implementations, you should
 *       never use it directly. Instead, simply use the Mutex or RecursiveMutex
 *       class, which will be typedefed to the appropriate implementation.
 **/
class MutexInterface {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~MutexInterface() = 0;

  /**
   * @brief Lock this Mutex. If the mutex is locked by another thread,
   *        execution will block until the Mutex becomes available.
   * @note Normal Mutex version only: It is an error to call lock() while
   *       already holding the Mutex.
   * @note RecursiveMutex version only: May be called an unlimited number of
   *       times by a single thread, so long as unlock() is called the same
   *       number of times.
   **/
  virtual void lock() = 0;

  /**
   * @brief Unlock this Mutex. It is an error to unlock a Mutex which is not
   *        locked by the current thread.
   **/
  virtual void unlock() = 0;
};

/**
 * @brief A scoped lock-holder for a Mutex. Locks a Mutex when it is
 *        constructed, and unlocks the Mutex when it goes out of scope.
 * @note This interface exists to provide a central point of documentation
 *       for platform-specific MutexLock and RecursiveMutexLock
 *       implementations, you should never use it directly. Instead, simply use
 *       the MutexLock or RecursiveMutexLock class, which will be typedefed to
 *       the appropriate implementation.
 **/
class MutexLockInterface {
 public:
  /**
   * @brief Virtual destructor. Unlocks the held Mutex.
   **/
  virtual ~MutexLockInterface() = 0;
};

/** @} */

}  // namespace quickstep

#ifdef QUICKSTEP_HAVE_CPP11_THREADS
#include "threading/cpp11/Mutex.hpp"
#endif

#ifdef QUICKSTEP_HAVE_POSIX_THREADS
#include "threading/posix/Mutex.hpp"
#endif

#ifdef QUICKSTEP_HAVE_WINDOWS_THREADS
#include "threading/windows/Mutex.hpp"
#endif

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief @brief A scoped lock-holder for a Mutex. Locks a Mutex when it is
 *        constructed, and unlocks the Mutex when it goes out of scope.
 **/
template <typename MutexT, bool actually_lock>
class MutexLockImpl {
 public:
  explicit inline MutexLockImpl(MutexT &mutex)  // NOLINT(runtime/references)
      : mutex_ptr_(&mutex) {
    mutex_ptr_->lock();
  }

  inline ~MutexLockImpl() {
    mutex_ptr_->unlock();
  }

 private:
  MutexT *mutex_ptr_;

  DISALLOW_COPY_AND_ASSIGN(MutexLockImpl);
};

// Partial specialization: do nothing and store nothing if
// actually_lock = false.
template <typename MutexT>
class MutexLockImpl<MutexT, false> {
 public:
  explicit inline MutexLockImpl(MutexT &mutex) {  // NOLINT(runtime/references)
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(MutexLockImpl);
};

template <typename MutexT>
class DynamicConditionalMutexLockImpl {
 public:
  inline DynamicConditionalMutexLockImpl(MutexT &mutex,  // NOLINT(runtime/references)
                                         const bool actually_lock)
      : mutex_ptr_(&mutex),
        actually_lock_(actually_lock) {
    if (actually_lock_) {
      mutex_ptr_->lock();
    }
  }

  inline ~DynamicConditionalMutexLockImpl() {
    if (actually_lock_) {
      mutex_ptr_->unlock();
    }
  }

 private:
  MutexT *mutex_ptr_;
  const bool actually_lock_;

  DISALLOW_COPY_AND_ASSIGN(DynamicConditionalMutexLockImpl);
};

typedef MutexLockImpl<Mutex, true> MutexLock;
typedef MutexLockImpl<RecursiveMutex, true> RecursiveMutexLock;
template <bool actually_lock> using StaticConditionalMutexLock
    = MutexLockImpl<Mutex, actually_lock>;
template <bool actually_lock> using StaticConditionalRecursiveMutexLock
    = MutexLockImpl<RecursiveMutex, actually_lock>;
typedef DynamicConditionalMutexLockImpl<Mutex> DynamicConditionalMutexLock;
typedef DynamicConditionalMutexLockImpl<RecursiveMutex> DynamicConditionalRecursiveMutexLock;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_MUTEX_HPP_
