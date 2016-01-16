/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_THREADING_POSIX_MUTEX_HPP_
#define QUICKSTEP_THREADING_POSIX_MUTEX_HPP_

#include <pthread.h>

#include "threading/Mutex.hpp"
#include "threading/posix/ConditionVariable.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

namespace threading_posix_internal {

// This wrapper class handles creating and destroying the global
// pthread_mutexattr_t instances at program start and exit, respectively.
class MutexAttrWrapper {
 public:
  static const pthread_mutexattr_t* GetNormal() {
    static MutexAttrWrapper wrapper(false);
    return &wrapper.internal_mutex_attr_;
  }

  static const pthread_mutexattr_t* GetRecursive() {
    static MutexAttrWrapper wrapper(true);
    return &wrapper.internal_mutex_attr_;
  }

 private:
  explicit MutexAttrWrapper(bool recursive) {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_mutexattr_init(&internal_mutex_attr_));
    if (recursive) {
      DO_AND_DEBUG_ASSERT_ZERO(pthread_mutexattr_settype(&internal_mutex_attr_,
                                                         PTHREAD_MUTEX_RECURSIVE));
    } else {
      DO_AND_DEBUG_ASSERT_ZERO(pthread_mutexattr_settype(&internal_mutex_attr_,
                                                         PTHREAD_MUTEX_NORMAL));
    }
  }

  ~MutexAttrWrapper() {
    pthread_mutexattr_destroy(&internal_mutex_attr_);
  }

  pthread_mutexattr_t internal_mutex_attr_;
};

static const pthread_mutexattr_t* kMutexattrNormal = MutexAttrWrapper::GetNormal();
static const pthread_mutexattr_t* kMutexattrRecursive = MutexAttrWrapper::GetRecursive();

}  // namespace threading_posix_internal

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of Mutex and RecursiveMutex using POSIX threads.
 **/
template <bool RECURSIVE>
class MutexImplPosix {
 public:
  inline MutexImplPosix() {
    if (RECURSIVE) {
      DO_AND_DEBUG_ASSERT_ZERO(pthread_mutex_init(&internal_mutex_,
                                                  threading_posix_internal::kMutexattrRecursive));
    } else {
      DO_AND_DEBUG_ASSERT_ZERO(pthread_mutex_init(&internal_mutex_,
                                                  threading_posix_internal::kMutexattrNormal));
    }
  }

  inline ~MutexImplPosix() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_mutex_destroy(&internal_mutex_));
  }

  inline void lock() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_mutex_lock(&internal_mutex_));
  }

  inline void unlock() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_mutex_unlock(&internal_mutex_));
  }

  ConditionVariable* createConditionVariable() {
    ConditionVariable *new_condition = new ConditionVariable(&internal_mutex_);
    condition_vars_.push_back(new_condition);
    return new_condition;
  }

 private:
  pthread_mutex_t internal_mutex_;
  PtrVector<ConditionVariable> condition_vars_;

  DISALLOW_COPY_AND_ASSIGN(MutexImplPosix);
};
typedef MutexImplPosix<false> Mutex;
typedef MutexImplPosix<true> RecursiveMutex;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_POSIX_MUTEX_HPP_
