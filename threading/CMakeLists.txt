#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015-2016 Pivotal Software, Inc.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

include(CheckIncludeFileCXX)
include(CheckCXXSourceCompiles)

# Find a threading library we can use.
set(FOUND_THREADS FALSE)

if(NOT FOUND_THREADS)
  CHECK_CXX_SOURCE_COMPILES("
#include <thread>
#include <mutex>
#include <condition_variable>

int main() {
  std::thread t;

  std::mutex m;
  std::recursive_mutex rm;

  std::condition_variable_any cv;

  std::lock_guard<std::mutex> lock_m(m);
  std::lock_guard<std::recursive_mutex> lock_rm(rm);

  return 0;
}
" QUICKSTEP_HAVE_CPP11_THREADS)

  if (QUICKSTEP_HAVE_CPP11_THREADS)
    CHECK_CXX_SOURCE_COMPILES("
#include <shared_mutex>

int main() {
  std::shared_mutex m;

  m.lock();
  m.unlock();

  m.lock_shared();
  m.unlock_shared();

  return 0;
}
" QUICKSTEP_HAVE_CPP17_SHARED_MUTEX)

    if (NOT QUICKSTEP_HAVE_CPP17_SHARED_MUTEX)
      CHECK_CXX_SOURCE_COMPILES("
#include <shared_mutex>

int main() {
  std::shared_timed_mutex m;

  m.lock();
  m.unlock();

  m.lock_shared();
  m.unlock_shared();

  return 0;
}
" QUICKSTEP_HAVE_CPP14_SHARED_TIMED_MUTEX)
    endif()

    message("Using threading implementation: cpp11")
    set(FOUND_THREADS TRUE)
  endif()
endif()

if(NOT FOUND_THREADS)
  if(CMAKE_HAVE_PTHREAD_H)
    message("Using threading implementation: posix")
    set(QUICKSTEP_HAVE_POSIX_THREADS TRUE)
    set(FOUND_THREADS TRUE)
  endif()
endif()

if(NOT FOUND_THREADS)
  CHECK_CXX_SOURCE_COMPILES("
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main() {
  CRITICAL_SECTION cs;
  InitializeCriticalSection(&cs);
  EnterCriticalSection(&cs);
  LeaveCriticalSection(&cs);
  DeleteCriticalSection(&cs);
  return 0;
}
" QUICKSTEP_HAVE_WINDOWS_THREADS)
  if (QUICKSTEP_HAVE_WINDOWS_THREADS)
    message("Using threading implementation: windows")
    set(FOUND_THREADS TRUE)
  endif()
endif()

if(NOT FOUND_THREADS)
  message(FATAL_ERROR "No viable threading library found.")
endif()


# Try to find some yield function we can use.
set(FOUND_YIELD FALSE)

if (NOT FOUND_YIELD)
  CHECK_CXX_SOURCE_COMPILES("
#include <thread>

int main() {
  std::this_thread::yield();
  return 0;
}
  " QUICKSTEP_HAVE_CPP11_YIELD)
  if (QUICKSTEP_HAVE_CPP11_YIELD)
    set(FOUND_YIELD TRUE)
  endif()
endif()

if (NOT FOUND_YIELD)
  CHECK_CXX_SOURCE_COMPILES("
#include <sched.h>

int main() {
  sched_yield();
  return 0;
}
  " QUICKSTEP_HAVE_SCHED_YIELD)
  if (QUICKSTEP_HAVE_SCHED_YIELD)
    set(FOUND_YIELD TRUE)
  endif()
endif()

if (NOT FOUND_YIELD)
  CHECK_CXX_SOURCE_COMPILES("
#include <pthread.h>

int main() {
  pthread_yield();
  return 0;
}
  " QUICKSTEP_HAVE_PTHREAD_YIELD)
  if (QUICKSTEP_HAVE_PTHREAD_YIELD)
    set(FOUND_YIELD TRUE)
  endif()
endif()

if (NOT FOUND_YIELD)
  CHECK_CXX_SOURCE_COMPILES("
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main() {
  SwitchToThread();
  return 0;
}
  " QUICKSTEP_HAVE_WIN_SWITCHTOTHREAD)
  if (QUICKSTEP_HAVE_WIN_SWITCHTOTHREAD)
    set(FOUND_YIELD TRUE)
  endif()
endif()

if(NOT FOUND_YIELD)
  message(WARNING "No yield function found. ThreadUtil::Yield() will be a no-op.")
endif()

# Try to find a thread-affinitization function that we can use. First, try the
# Linux version of pthread_setaffinity_np().
CHECK_CXX_SOURCE_COMPILES("
  #ifndef _GNU_SOURCE
  #define _GNU_SOURCE 1
  #endif
  #include <pthread.h>
  #include <sched.h>

  int main() {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset) != 0) {
      return 1;
    }
    return 0;
  }
  " QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_LINUX)

# Next, try the FreeBSD version of pthread_setaffinity_np(). It is almost
# identical to the Linux version, except that it is in the special header
# pthread_np.h, and the CPU set type is called cpuset_t instead of cpu_set_t.
if (NOT QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_LINUX)
  CHECK_CXX_SOURCE_COMPILES("
    #ifndef _BSD_SOURCE
    #define _BSD_SOURCE 1
    #endif
    #include <pthread.h>
    #include <pthread_np.h>
    #include <sched.h>

    int main() {
      cpuset_t cpuset;
      CPU_ZERO(&cpuset);
      CPU_SET(0, &cpuset);
      if (pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset) != 0) {
        return 1;
      }
      return 0;
    }
    " QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_FREEBSD)
endif()

# Finally, try the NetBSD version of pthread_setaffinity_np(). Unlike Linux or
# FreeBSD, NetBSD's version of cpuset_t is dynamically allocated/freed on the
# heap by some special functions in sched.h.
if (NOT (QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_LINUX OR QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_FREEBSD))
  CHECK_CXX_SOURCE_COMPILES("
    #ifndef _BSD_SOURCE
    #define _BSD_SOURCE 1
    #endif
    #include <pthread.h>
    #include <sched.h>

    int main() {
      cpuset_t *cset = cpuset_create();
      if (cpuset_set(0, cset) != 0) {
        return 1;
      }
      if (pthread_setaffinity_np(pthread_self(), cpuset_size(cset), cset) != 0) {
        return 1;
      }
      cpuset_destroy(cset);
      return 0;
    }
    " QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_NETBSD)
endif()

if (NOT (QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_LINUX
         OR QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_FREEBSD
         OR QUICKSTEP_HAVE_PTHREAD_SETAFFINITY_NP_NETBSD))
  message(WARNING "Thread pinning operation will be a no-op.")
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/ThreadingConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ThreadingConfig.h"
)

# Declare micro-libs:
if (QUICKSTEP_HAVE_CPP11_THREADS)
  add_library(quickstep_threading_ConditionVariable
              ConditionVariable.cpp
              ConditionVariable.hpp
              cpp11/ConditionVariable.hpp)
  add_library(quickstep_threading_Mutex
              Mutex.cpp
              Mutex.hpp
              cpp11/Mutex.hpp)
  if (QUICKSTEP_HAVE_CPP14_SHARED_MUTEX)
    add_library(quickstep_threading_SharedMutex
                ../empty_src.cpp
                SharedMutex.hpp
                cpp11/cpp14/SharedMutex.hpp)
  else()
    add_library(quickstep_threading_SharedMutex
                ../empty_src.cpp
                SharedMutex.hpp
                cpp11/SharedMutex.hpp)
  endif()
  add_library(quickstep_threading_Thread
              Thread.cpp
              Thread.hpp
              cpp11/Thread.hpp)
elseif (QUICKSTEP_HAVE_POSIX_THREADS)
  add_library(quickstep_threading_ConditionVariable
              ConditionVariable.cpp
              ConditionVariable.hpp
              posix/ConditionVariable.hpp)
  add_library(quickstep_threading_Mutex
              Mutex.cpp
              Mutex.hpp
              posix/Mutex.hpp)
  add_library(quickstep_threading_SharedMutex
              ../empty_src.cpp
              SharedMutex.hpp
              posix/SharedMutex.hpp)
  add_library(quickstep_threading_Thread
              Thread.cpp
              Thread.hpp
              posix/Thread.hpp)
elseif (QUICKSTEP_HAVE_WINDOWS_THREADS)
  add_library(quickstep_threading_ConditionVariable
              ConditionVariable.cpp
              ConditionVariable.hpp
              windows/ConditionVariable.hpp)
  add_library(quickstep_threading_Mutex
              Mutex.cpp
              Mutex.hpp
              windows/Mutex.hpp)
  add_library(quickstep_threading_SharedMutex
              ../empty_src.cpp
              SharedMutex.hpp
              windows/SharedMutex.hpp)
  add_library(quickstep_threading_Thread
              Thread.cpp
              Thread.hpp
              windows/Thread.hpp)
endif()

add_library(quickstep_threading_SpinMutex ../empty_src.cpp SpinMutex.hpp)
add_library(quickstep_threading_SpinSharedMutex ../empty_src.cpp SpinSharedMutex.hpp)
add_library(quickstep_threading_ThreadIDBasedMap ../empty_src.cpp ThreadIDBasedMap.hpp)
add_library(quickstep_threading_ThreadUtil ../empty_src.cpp ThreadUtil.hpp)

# Link dependencies:
target_link_libraries(quickstep_threading_ConditionVariable
                      glog
                      quickstep_utility_Macros)
target_link_libraries(quickstep_threading_Mutex
                      glog
                      quickstep_threading_ConditionVariable
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
if (QUICKSTEP_HAVE_CPP11_THREADS AND NOT QUICKSTEP_HAVE_CPP14_SHARED_MUTEX)
  target_link_libraries(quickstep_threading_SharedMutex
                        glog
                        quickstep_threading_ConditionVariable
                        quickstep_threading_Mutex
                        quickstep_utility_Macros)
else()
  target_link_libraries(quickstep_threading_SharedMutex
                        quickstep_threading_Mutex
                        quickstep_utility_Macros)
endif()
target_link_libraries(quickstep_threading_SpinMutex
                      quickstep_threading_Mutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_threading_SpinSharedMutex
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_ThreadUtil
                      quickstep_utility_Macros)
target_link_libraries(quickstep_threading_Thread
                      quickstep_utility_Macros)
target_link_libraries(quickstep_threading_ThreadIDBasedMap
                      glog
                      quickstep_storage_StorageConstants
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_threading_ThreadUtil
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_threading ../empty_src.cpp ThreadingModule.hpp)
target_link_libraries(quickstep_threading
                      quickstep_threading_ConditionVariable
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_threading_Thread
                      quickstep_threading_ThreadIDBasedMap
                      quickstep_threading_ThreadUtil)

# Tests:
add_executable(Mutex_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/Mutex_unittest.cpp")
target_link_libraries(Mutex_unittest
                      gtest
                      gtest_main
                      quickstep_threading_Mutex
                      quickstep_threading_SpinMutex
                      quickstep_threading_Thread
                      quickstep_utility_Macros
                      ${CMAKE_THREAD_LIBS_INIT})
add_test(Mutex_unittest Mutex_unittest)

add_executable(SharedMutex_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/SharedMutex_unittest.cpp")
target_link_libraries(SharedMutex_unittest
                      gtest
                      gtest_main
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_threading_Thread
                      quickstep_utility_Macros
                      ${CMAKE_THREAD_LIBS_INIT})
add_test(SharedMutex_unittest SharedMutex_unittest)
