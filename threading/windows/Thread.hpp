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

#ifndef QUICKSTEP_THREADING_WINDOWS_THREAD_HPP_
#define QUICKSTEP_THREADING_WINDOWS_THREAD_HPP_

#include "threading/Thread.hpp"
#include "threading/WinThreadsAPI.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

namespace threading_windows_internal {
class WindowsExecHelper {
 public:
  static DWORD WINAPI executeRunMethodForThread(LPVOID thread_ptr) {
    static_cast<ThreadInterface*>(thread_ptr)->run();
    return 0;
  }
};
}  // namespace threading_windows_internal

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of Thread using MS Windows threads.
 **/
class ThreadImplWindows : public ThreadInterface {
 public:
  ThreadImplWindows() {
    thread_handle_ = CreateThread(nullptr,
                                  0,
                                  threading_windows_internal::WindowsExecHelper::executeRunMethodForThread,
                                  this,
                                  CREATE_SUSPENDED,
                                  nullptr);
  }

  virtual ~ThreadImplWindows() {
    CloseHandle(thread_handle_);
  }

  inline void start() {
    ResumeThread(thread_handle_);
  }

  inline void join() {
    WaitForSingleObject(thread_handle_, INFINITE);
  }

 private:
  HANDLE thread_handle_;

  DISALLOW_COPY_AND_ASSIGN(ThreadImplWindows);
};
typedef ThreadImplWindows Thread;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_WINDOWS_THREAD_HPP_
