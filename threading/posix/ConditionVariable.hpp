/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_THREADING_POSIX_CONDITION_VARIABLE_HPP_
#define QUICKSTEP_THREADING_POSIX_CONDITION_VARIABLE_HPP_

#include <pthread.h>
#include <sys/time.h>

#include <cerrno>
#include <cstdint>
#include <ctime>

#include "threading/ConditionVariable.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

template <bool RECURSIVE> class MutexImplPosix;

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of ConditionVariable using POSIX threads.
 **/
class ConditionVariableImplPosix {
 public:
  ~ConditionVariableImplPosix() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_cond_destroy(&internal_condition_var_));
  }

  inline void await() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_cond_wait(&internal_condition_var_, parent_mutex_));
  }

  inline bool awaitWithTimeout(std::uint64_t milliseconds) {
    struct timespec timeout_time;
    struct timeval current_time;
    gettimeofday(&current_time, nullptr);
    std::uint64_t nanoseconds = (current_time.tv_usec + milliseconds * 1000ULL) * 1000ULL;
    timeout_time.tv_sec = current_time.tv_sec + nanoseconds / 1000000000ULL;
    timeout_time.tv_nsec = nanoseconds % 1000000000ULL;
    int error_code = pthread_cond_timedwait(&internal_condition_var_,
                                            parent_mutex_, &timeout_time);
    if (ETIMEDOUT == error_code) {
      return false;
    } else {
      DCHECK_EQ(0, error_code);
      return true;
    }
  }

  inline void signalOne() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_cond_signal(&internal_condition_var_));
  }

  inline void signalAll() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_cond_broadcast(&internal_condition_var_));
  }

 private:
  explicit ConditionVariableImplPosix(pthread_mutex_t *parent_mutex)
      : parent_mutex_(parent_mutex) {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_cond_init(&internal_condition_var_, nullptr));
  }

  pthread_mutex_t *parent_mutex_;
  pthread_cond_t internal_condition_var_;

  friend class MutexImplPosix<true>;
  friend class MutexImplPosix<false>;

  DISALLOW_COPY_AND_ASSIGN(ConditionVariableImplPosix);
};
typedef ConditionVariableImplPosix ConditionVariable;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_POSIX_CONDITION_VARIABLE_HPP_
