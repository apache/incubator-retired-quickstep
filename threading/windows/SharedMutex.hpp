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

#ifndef QUICKSTEP_THREADING_WINDOWS_SHARED_MUTEX_HPP_
#define QUICKSTEP_THREADING_WINDOWS_SHARED_MUTEX_HPP_

#include "threading/SharedMutex.hpp"
#include "threading/WinThreadsAPI.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of SharedMutex using MS Windows SRWLock.
 **/
class SharedMutexImplWindows {
 public:
  SharedMutexImplWindows() {
    InitializeSRWLock(&srw_lock_);
  }

  ~SharedMutexImplWindows() {
  }

  inline void lock() {
    AcquireSRWLockExclusive(&srw_lock_);
  }

  inline void unlock() {
    ReleaseSRWLockExclusive(&srw_lock_);
  }

  inline void lockShared() {
    AcquireSRWLockShared(&srw_lock_);
  }

  inline void unlockShared() {
    ReleaseSRWLockShared(&srw_lock_);
  }

 private:
  SRWLOCK srw_lock_;

  DISALLOW_COPY_AND_ASSIGN(SharedMutexImplWindows);
};

/** @} */

typedef SharedMutexImplWindows SharedMutex;

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_WINDOWS_SHARED_MUTEX_HPP_
