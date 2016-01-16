/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
