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

#ifndef QUICKSTEP_THREADING_THREAD_HPP_
#define QUICKSTEP_THREADING_THREAD_HPP_

#include "threading/ThreadingConfig.h"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

namespace threading_internal {
void executeRunMethodForThreadReturnNothing(void *thread_ptr);
void* executeRunMethodForThreadReturnNull(void *thread_ptr);
}  // namespace threading_internal

namespace threading_windows_internal {
class WindowsExecHelper;
}  // namespace threading_windows_internal

/**
 * @brief An independent thread of execution.
 * @note This interface exists to provide a central point of documentation for
 *       platform-specific Thread implementations, you should never use it
 *       directly. Instead, simply use the Thread class, which will be
 *       typedefed to the appropriate implementation.
 **/
class ThreadInterface {
 public:
  inline ThreadInterface() {
  }

  /**
   * @brief Virtual destructor.
   **/
  virtual ~ThreadInterface() = 0;

  /**
   * @brief Start executing this Thread's run() method in an independent
   *        thread.
   * @warning Call this only once.
   **/
  virtual void start() = 0;

  /**
   * @brief Block until this thread stops running (i.e. until the run()
   *        method returns).
   * @warning You must call start() before calling join().
   **/
  virtual void join() = 0;

 protected:
  /**
   * @brief This method is invoked when start() is called. It should be
   *        overridden with the actual code which you wish to execute in the
   *        independent thread.
   **/
  virtual void run() = 0;

 private:
  friend void threading_internal::executeRunMethodForThreadReturnNothing(void *thread_ptr);
  friend void* threading_internal::executeRunMethodForThreadReturnNull(void *thread_ptr);
  friend class threading_windows_internal::WindowsExecHelper;
};

/** @} */

}  // namespace quickstep

#ifdef QUICKSTEP_HAVE_CPP11_THREADS
#include "threading/cpp11/Thread.hpp"
#endif

#ifdef QUICKSTEP_HAVE_POSIX_THREADS
#include "threading/posix/Thread.hpp"
#endif

#ifdef QUICKSTEP_HAVE_WINDOWS_THREADS
#include "threading/windows/Thread.hpp"
#endif

#endif  // QUICKSTEP_THREADING_THREAD_HPP_
