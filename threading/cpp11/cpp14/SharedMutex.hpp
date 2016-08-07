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

#ifndef QUICKSTEP_THREADING_CPP11_CPP14_SHARED_MUTEX_HPP_
#define QUICKSTEP_THREADING_CPP11_CPP14_SHARED_MUTEX_HPP_

#include <shared_mutex>

#include "threading/SharedMutex.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of SharedMutex using C++14 std::shared_timed_mutex.
 **/
class SharedMutexImplCPP14 {
 public:
  inline SharedMutexImplCPP14() {}
  inline ~SharedMutexImplCPP14() {}

  inline void lock() {
    internal_shared_mutex_.lock();
  }

  inline void unlock() {
    internal_shared_mutex_.unlock();
  }

  inline void lockShared() {
    internal_shared_mutex_.lock_shared();
  }

  inline void unlockShared() {
    internal_shared_mutex_.unlock_shared();
  }

 private:
  std::shared_timed_mutex internal_shared_mutex_;

  DISALLOW_COPY_AND_ASSIGN(SharedMutexImplCPP14);
};

/** @} */

typedef SharedMutexImplCPP14 SharedMutex;

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_CPP11_CPP14_SHARED_MUTEX_HPP_
