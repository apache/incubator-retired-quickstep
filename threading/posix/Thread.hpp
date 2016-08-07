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
