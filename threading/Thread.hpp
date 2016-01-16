/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
