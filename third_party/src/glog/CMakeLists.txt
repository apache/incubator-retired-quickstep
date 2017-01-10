# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

# This file allows building glog with CMake instead of autotools. glog itself
# is copyright Google, Inc. and is covered by the license described in the
# COPYING file in this directory.

if(CMAKE_COMPILER_IS_GNUCXX OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
endif()

if (WIN32)
  # Windows uses a preprocessed config file and some other wierdness.
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS GOOGLE_GLOG_DLL_DECL=
  )

  # Check if snprintf is available. It usually isn't on Windows, but MinGW
  # provides it.
  include(CheckCXXSymbolExists)
  check_cxx_symbol_exists(snprintf "stdio.h" HAVE_SNPRINTF)

  # Similarly, check if pid_t is defined in process.h. Usually the answer is no
  # on Windows, but MinGW has it.
  include(CheckTypeSize)
  check_type_size("pid_t" PID_T)
  configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/src/windows/config_cmake.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
  )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include_directories(src/windows)
  add_library(glog
              src/windows/logging.cc
              src/windows/port.cc
              src/windows/raw_logging.cc
              src/windows/utilities.cc
              src/windows/vlog_is_on.cc)
else()
  # The big, happy UNIX family (and friends).

  # System headers.
  include(CheckIncludeFileCXX)
  check_include_file_cxx(dlfcn.h HAVE_DLFCN_H)
  check_include_file_cxx(execinfo.h HAVE_EXECINFO_H)
  check_include_file_cxx(glob.h HAVE_GLOB_H)
  check_include_file_cxx(inttypes.h HAVE_INTTYPES_H)
  check_include_file_cxx(libunwind.h HAVE_LIBUNWIND_H)
  check_include_file_cxx(memory.h HAVE_MEMORY_H)
  check_include_file_cxx(pwd.h HAVE_PWD_H)
  check_include_file_cxx(libunwind.h HAVE_LIBUNWIND_H)
  check_include_file_cxx(stdint.h HAVE_STDINT_H)
  check_include_file_cxx(stdlib.h HAVE_STDLIB_H)
  check_include_file_cxx(strings.h HAVE_STRINGS_H)
  check_include_file_cxx(string.h HAVE_STRING_H)
  check_include_file_cxx(syscall.h HAVE_SYSCALL_H)
  check_include_file_cxx(syslog.h HAVE_SYSLOG_H)
  check_include_file_cxx(sys/stat.h HAVE_SYS_STAT_H)
  check_include_file_cxx(sys/syscall.h HAVE_SYS_SYSCALL_H)
  check_include_file_cxx(sys/time.h HAVE_SYS_TIME_H)
  check_include_file_cxx(sys/types.h HAVE_SYS_TYPES_H)
  check_include_file_cxx(sys/ucontext.h HAVE_SYS_UCONTEXT_H)
  check_include_file_cxx(sys/utsname.h HAVE_SYS_UTSNAME_H)
  check_include_file_cxx(ucontext.h HAVE_UCONTEXT_H)
  check_include_file_cxx(unistd.h HAVE_UNISTD_H)
  check_include_file_cxx(unwind.h HAVE_UNWIND_H)

  # Check if functions exist.
  include(CheckFunctionExists)
  check_function_exists(fcntl HAVE_FCNTL)
  check_function_exists(sigalstack HAVE_SIGALSTACK)

  # Check if we are using pthreads.
  find_package(Threads)
  if (CMAKE_HAVE_PTHREAD_H)
    set(HAVE_LIBPTHREAD 1)
    set(HAVE_PTHREAD 1)
    set(HAVE_RWLOCK 1)
  endif()

  # Check pointer size.
  include(CheckTypeSize)
  check_type_size("void*" SIZEOF_VOID_P)

  # Check compiler builtins.
  include(CheckCXXSourceCompiles)

  CHECK_CXX_SOURCE_COMPILES("
    int main(int argc, char **argv) {
      if (__builtin_expect(argc, 1)) {
        return 0;
      } else {
        return argc;
      }
    }
  " HAVE___BUILTIN_EXPECT)

  CHECK_CXX_SOURCE_COMPILES("
    int numfun(int n) __attribute__((const));

    int main(int argc, char **argv) {
      return numfun(argc);
    }

    int numfun(int n) {
      return n + 1;
    }
  " HAVE___ATTRIBUTE__)

  CHECK_CXX_SOURCE_COMPILES("
    int atomicswap(int *ptr, int oldval, int newval) {
      return __sync_val_compare_and_swap(ptr, oldval, newval);
    }

    int main(int argc, char **argv) {
      return atomicswap(&argc, 1, 2);
    }
  " HAVE___SYNC_VAL_COMPARE_AND_SWAP)

  # Try a bunch of different platform-specific ways to get the program counter
  # from a ucontext_t. This particular code snippet is based on Apache-licensed
  # code from https://maiter.googlecode.com/svn/trunk/Maiter/cmake/PcFromUcontext.cmake
  set(UCONTEXT_INCLUDES)
  if (HAVE_SYS_UCONTEXT_H)
    set(UCONTEXT_INCLUDES "${UCONTEXT_INCLUDES}\n#include <sys/ucontext.h>")
  endif()
  if (HAVE_UCONTEXT_H)
    set(UCONTEXT_INCLUDES "${UCONTEXT_INCLUDES}\n#include <ucontext.h>")
  endif()

  foreach (pc_field
           "uc_mcontext.gregs[REG_EIP]"
           "uc_mcontext.gregs[REG_RIP]"
           "uc_mcontext.sc_ip"
           "uc_mcontext.uc_regs->gregs[PT_NIP]"
           "uc_mcontext.gregs[R15]"
           "uc_mcontext.arm_pc"
           "uc_mcontext.mc_eip"
           "uc_mcontext.mc_rip"
           "uc_mcontext.__gregs[_REG_EIP]"
           "uc_mcontext.__gregs[_REG_RIP]"
           "uc_mcontext->ss.eip"
           "uc_mcontext->__ss.__eip"
           "uc_mcontext->ss.rip"
           "uc_mcontext->__ss.__rip"
           "uc_mcontext->ss.srr0"
           "uc_mcontext->__ss.__srr0")
    message(STATUS "Checking program counter fetch from ucontext_t member: ${pc_field}")

    CHECK_CXX_SOURCE_COMPILES("
      ${UCONTEXT_INCLUDES}
      int main(int argc, char **argv) {
        ucontext_t ctx;
        return ctx.${pc_field} == 0;
      }
    " PC_FROM_UCONTEXT_COMPILES)

    if (PC_FROM_UCONTEXT_COMPILES)
      unset(PC_FROM_UCONTEXT_COMPILES CACHE)
      message(STATUS "Found program counter field in ucontext_t: ${pc_field}")
      set(PC_FROM_UCONTEXT ${pc_field})
      break()
    else()
      unset(PC_FROM_UCONTEXT_COMPILES CACHE)
    endif()
  endforeach()

  if (NOT PC_FROM_UCONTEXT)
    message(WARNING "Unable to find program counter field in ucontext_t. GLOG "
                    "signal handler will not be able to report precise PC "
                    "position.")
  endif()

  # Generate config.h
  configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config_cmake.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
  )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

  include_directories(src)
  add_library(glog
              src/demangle.cc
              src/logging.cc
              src/raw_logging.cc
              src/signalhandler.cc
              src/symbolize.cc
              src/utilities.cc
              src/vlog_is_on.cc)
  target_link_libraries(glog ${CMAKE_THREAD_LIBS_INIT})
endif()
