/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
