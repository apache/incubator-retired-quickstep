/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_THREADING_POSIX_THREAD_HPP_
#define QUICKSTEP_THREADING_POSIX_THREAD_HPP_

#include <pthread.h>

#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of Thread using POSIX threads.
 **/
class ThreadImplPosix : public ThreadInterface {
 public:
  inline ThreadImplPosix() {
  }

  inline virtual ~ThreadImplPosix() {
  }

  inline void start() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_create(&internal_thread_,
                                            nullptr,
                                            threading_internal::executeRunMethodForThreadReturnNull,
                                            this));
  }

  inline void join() {
    DO_AND_DEBUG_ASSERT_ZERO(pthread_join(internal_thread_, nullptr));
  }

 private:
  pthread_t internal_thread_;

  DISALLOW_COPY_AND_ASSIGN(ThreadImplPosix);
};
typedef ThreadImplPosix Thread;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_POSIX_THREAD_HPP_
