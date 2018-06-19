# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

find_package(Threads REQUIRED)
set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

include_directories(include)

# Check for some version pthread_setaffinity_np() we can use to affinitize
# threads.
include(CheckCXXSourceCompiles)
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
  " TMB_AFFINITY_LINUX)

if (NOT TMB_AFFINITY_LINUX)
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
    " TMB_AFFINITY_FREEBSD)
endif()

if (NOT (TMB_AFFINITY_LINUX OR TMB_AFFINITY_FREEBSD))
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
    " TMB_AFFINITY_NETBSD)
endif()

if (NOT (TMB_AFFINITY_LINUX OR TMB_AFFINITY_FREEBSD OR TMB_AFFINITY_NETBSD))
  message(WARNING
          "Could not find a usable version of pthread_setaffinity_np(). "
          "Thread-pinning for benchmarks will not be available.")
endif()

if (TMB_AFFINITY_LINUX)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_AFFINITY_LINUX)
elseif (TMB_AFFINITY_FREEBSD)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_AFFINITY_FREEBSD)
elseif (TMB_AFFINITY_NETBSD)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_AFFINITY_NETBSD)
endif()

add_library(tmbbench src/affinity.cc
                     src/bus_setup.cc
                     src/receiver_thread.cc
                     src/sender_thread.cc
                     src/thread.cc)
target_link_libraries(tmbbench
                      gflags_nothreads-static
                      tmb
                      ${CMAKE_THREAD_LIBS_INIT})

add_executable(oneway_throughput src/oneway_throughput.cc)
target_link_libraries(oneway_throughput
                      gflags_nothreads-static
                      tmb
                      tmbbench
                      ${LIBS})

add_executable(oneway_throughput_numa src/oneway_throughput_numa.cc)
target_link_libraries(oneway_throughput_numa
                      gflags_nothreads-static
                      tmb
                      tmbbench
                      ${LIBS})

add_executable(oneway_throughput_distributed
               src/oneway_throughput_distributed.cc)
target_link_libraries(oneway_throughput_distributed
                      gflags_nothreads-static
                      tmb
                      tmbbench
                      ${LIBS})

add_executable(oneway_throughput_distributed_coordinator
               src/oneway_throughput_distributed_coordinator.cc)
target_link_libraries(oneway_throughput_distributed_coordinator
                      gflags_nothreads-static
                      tmb
                      tmbbench
                      ${LIBS})

add_executable(reset_bus src/reset_bus.cc)
target_link_libraries(reset_bus
                      gflags_nothreads-static
                      tmb
                      tmbbench)
