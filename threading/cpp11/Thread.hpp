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

#ifndef QUICKSTEP_THREADING_CPP11_THREAD_HPP_
#define QUICKSTEP_THREADING_CPP11_THREAD_HPP_

#include <thread>  // NOLINT(build/c++11)

#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of Thread using C++11 threads.
 **/
class ThreadImplCPP11 : public ThreadInterface {
 public:
  inline ThreadImplCPP11() {
  }

  inline virtual ~ThreadImplCPP11() {
  }

  inline void start() {
    internal_thread_ = std::thread(threading_internal::executeRunMethodForThreadReturnNothing, this);
  }

  inline void join() {
    internal_thread_.join();
  }

 private:
  std::thread internal_thread_;

  DISALLOW_COPY_AND_ASSIGN(ThreadImplCPP11);
};
typedef ThreadImplCPP11 Thread;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_CPP11_THREAD_HPP_
