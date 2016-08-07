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
    std::uint64_t nanoseconds
        = (current_time.tv_usec + milliseconds * UINT64_C(1000)) * UINT64_C(1000);
    timeout_time.tv_sec = current_time.tv_sec + nanoseconds / UINT64_C(1000000000);
    timeout_time.tv_nsec = nanoseconds % UINT64_C(1000000000);
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
