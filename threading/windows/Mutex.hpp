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

#ifndef QUICKSTEP_THREADING_WINDOWS_MUTEX_HPP_
#define QUICKSTEP_THREADING_WINDOWS_MUTEX_HPP_

#include "threading/Mutex.hpp"
#include "threading/WinThreadsAPI.hpp"
#include "threading/windows/ConditionVariable.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of RecursiveMutex using MS Windows threads.
 **/
class RecursiveMutexImplWindows {
 public:
  RecursiveMutexImplWindows() {
    InitializeCriticalSection(&critical_section_);
  }

  ~RecursiveMutexImplWindows() {
    DeleteCriticalSection(&critical_section_);
  }

  inline void lock() {
    EnterCriticalSection(&critical_section_);
  }

  inline void unlock() {
    LeaveCriticalSection(&critical_section_);
  }

  ConditionVariable* createConditionVariable() {
    ConditionVariable *new_condition = new ConditionVariable(&critical_section_);
    condition_vars_.push_back(new_condition);
    return new_condition;
  }

 private:
  CRITICAL_SECTION critical_section_;
  PtrVector<ConditionVariable> condition_vars_;

  DISALLOW_COPY_AND_ASSIGN(RecursiveMutexImplWindows);
};
typedef RecursiveMutexImplWindows RecursiveMutex;

// For debug builds, use an implementation which actually prevents recursive
// locking. For release builds, don't bother (it's faster).
#ifdef QUICKSTEP_DEBUG
/**
 * @brief Implementation of Mutex using MS Windows threads.
 **/
class MutexImplWindows {
 public:
  MutexImplWindows()
      : held_(false) {
  }

  ~MutexImplWindows() {
  }

  inline void lock() {
    internal_mutex_.lock();
    DCHECK(!held_);
    held_ = true;
  }

  inline void unlock() {
    DCHECK(held_);
    held_ = false;
    internal_mutex_.unlock();
  }

  ConditionVariableImplWindows* createConditionVariable() {
    return internal_mutex_.createConditionVariable();
  }

 private:
  RecursiveMutexImplWindows internal_mutex_;
  bool held_;

  DISALLOW_COPY_AND_ASSIGN(MutexImplWindows);
};
typedef MutexImplWindows Mutex;
#else
typedef RecursiveMutexImplWindows Mutex;
#endif

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_WINDOWS_MUTEX_HPP_
