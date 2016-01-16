/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
