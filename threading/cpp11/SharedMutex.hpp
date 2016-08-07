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

#ifndef QUICKSTEP_THREADING_CPP11_SHARED_MUTEX_HPP_
#define QUICKSTEP_THREADING_CPP11_SHARED_MUTEX_HPP_

#include <cstdint>

#include "threading/ConditionVariable.hpp"
#include "threading/Mutex.hpp"
#include "threading/SharedMutex.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/**
 * @brief Implementation of SharedMutex built on C++11 threads.
 **/
class SharedMutexImplCPP11 {
 public:
  SharedMutexImplCPP11()
      : state_(0) {
    write_condition_ = base_mutex_.createConditionVariable();
    read_condition_ = base_mutex_.createConditionVariable();
  }

  inline void lock() {
    MutexLock lock(base_mutex_);
    while (state_ & kWriteMask) {
      read_condition_->await();
    }
    state_ |= kWriteMask;
    while (state_ & kReadersMask) {
      write_condition_->await();
    }
  }

  inline void unlock() {
    MutexLock lock(base_mutex_);
    DCHECK_EQ(kWriteMask, state_);
    state_ = 0;
    read_condition_->signalAll();
  }

  inline void lockShared() {
    MutexLock lock(base_mutex_);
    while (state_ & kWriteMask) {
      read_condition_->await();
    }
    ++state_;
  }

  inline void unlockShared() {
    MutexLock lock(base_mutex_);
    DCHECK(state_ & kReadersMask);
    --state_;
    if (state_ == kWriteMask) {
      write_condition_->signalOne();
    }
  }

 private:
  static const std::uint32_t kWriteMask = 0x80000000;
  static const std::uint32_t kReadersMask = ~kWriteMask;

  std::uint32_t state_;
  Mutex base_mutex_;
  ConditionVariable *write_condition_;
  ConditionVariable *read_condition_;

  DISALLOW_COPY_AND_ASSIGN(SharedMutexImplCPP11);
};

typedef SharedMutexImplCPP11 SharedMutex;

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_CPP11_SHARED_MUTEX_HPP_
