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

#ifndef QUICKSTEP_THREADING_POSIX_SHARED_MUTEX_HPP_
#define QUICKSTEP_THREADING_POSIX_SHARED_MUTEX_HPP_

#include <pthread.h>

#include "threading/SharedMutex.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/**
 * @brief Implementation of SharedMutex using POSIX rwlock.
 **/
class SharedMutexImplPosix {
 public:
  inline SharedMutexImplPosix() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_rwlock_init(&internal_rwlock_, nullptr));
  }

  inline ~SharedMutexImplPosix() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_rwlock_destroy(&internal_rwlock_));
  }

  inline void lock() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_rwlock_wrlock(&internal_rwlock_));
  }

  inline void unlock() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_rwlock_unlock(&internal_rwlock_));
  }

  inline void lockShared() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_rwlock_rdlock(&internal_rwlock_));
  }

  inline void unlockShared() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_rwlock_unlock(&internal_rwlock_));
  }

 private:
  pthread_rwlock_t internal_rwlock_;

  DISALLOW_COPY_AND_ASSIGN(SharedMutexImplPosix);
};

typedef SharedMutexImplPosix SharedMutex;

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_POSIX_SHARED_MUTEX_HPP_
