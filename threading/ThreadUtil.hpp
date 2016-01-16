/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_THREADING_THREAD_UTIL_HPP_
#define QUICKSTEP_THREADING_THREAD_UTIL_HPP_

#include "threading/ThreadingConfig.h"
#include "utility/Macros.hpp"

// Set defines as needed for non-portable GNU or FreeBSD extensions to pthreads
// and include appropriate headers.
#if defined(QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_LINUX)

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <pthread.h>
#include <sched.h>

#elif defined(QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_FREEBSD)

#ifndef _BSD_SOURCE
#define _BSD_SOURCE 1
#endif
#include <pthread.h>
#include <pthread_np.h>
#include <sched.h>

#elif defined(QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_NETBSD)

#ifndef _BSD_SOURCE
#define _BSD_SOURCE 1
#endif
#include <pthread.h>
#include <sched.h>

#endif

// Include appropriate header for yield function.
#if defined(QUICKSTEP_HAVE_CPP11_YIELD)
#include <thread>  // NOLINT(build/c++11)
#elif defined(QUICKSTEP_HAVE_SCHED_YIELD)
#include <sched.h>  // NOLINT(build/include)
#elif defined(QUICKSTEP_HAVE_PTHREAD_YIELD)
#include <pthread.h>  // NOLINT(build/include)
#elif defined(QUICKSTEP_HAVE_WIN_SWITCHTOTHREAD)
#include "threading/WinThreadsAPI.hpp"
#endif

namespace quickstep {

/**
 * @brief All-static class with methods controlling the scheduling and behavior
 *        of the calling thread.
 **/
class ThreadUtil {
 public:
  inline static void Yield() {
#if defined(QUICKSTEP_HAVE_CPP11_YIELD)
    std::this_thread::yield();
#elif defined(QUICKSTEP_HAVE_SCHED_YIELD)
    DO_AND_DEBUG_ASSERT_ZERO(sched_yield());
#elif defined(QUICKSTEP_HAVE_PTHREAD_YIELD)
    DO_AND_DEBUG_ASSERT_ZERO(pthread_yield());
#elif defined(QUICKSTEP_HAVE_WIN_SWITCHTOTHREAD)
    SwitchToThread();
#endif
  }

  /**
   * @brief Bind (pin) the calling thread so that it only runs on the specified
   *        logical CPU.
   *
   * @TODO(harshad) This function is essentially a no-op for systems other than
   *                Linux, FreeBSD, or NetBSD. We would like to support thread
   *                pinning on more OSes where possible.
   *
   * @note If the system doesn't have any of pthread.h, sched.h or the function
   *        pthread_setaffinity_np, this function will be a no-op.
   *
   * @param cpu_id The system ID of the CPU which the calling thread should be
   *        bound to.
   **/
  inline static void BindToCPU(const int cpu_id) {
#if defined(QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_LINUX) \
    || defined(QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_FREEBSD)

#ifdef QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_LINUX
    cpu_set_t cpuset;
#else
    cpuset_t cpuset;
#endif
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);

    if (pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset) != 0) {
      FATAL_ERROR("Failed to pin thread to CPU " << cpu_id);
    }
#elif defined(QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_NETBSD)
    cpuset_t *cset = cpuset_create();
    if (cpuset_set(cpu_id, cset) != 0) {
      FATAL_ERROR("Failed to pin thread to CPU " << cpu_id);
    }
    if (pthread_setaffinity_np(pthread_self(), cpuset_size(cset), cset) != 0) {
      FATAL_ERROR("Failed to pin thread to CPU " << cpu_id);
    }
    cpuset_destroy(cset);
#endif
  }

 private:
  // Undefined default constructor. Class is all-static.
  ThreadUtil();

  DISALLOW_COPY_AND_ASSIGN(ThreadUtil);
};

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_THREAD_UTIL_HPP_
