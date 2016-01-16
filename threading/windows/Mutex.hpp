/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
