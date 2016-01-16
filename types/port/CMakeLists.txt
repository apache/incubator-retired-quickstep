# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# Copyright (c) 2015, Pivotal Software, Inc.
# All rights reserved.
# See file CREDITS.txt for details.

include(CheckCXXSymbolExists)

# strnlen, gmtime_r, and localtime_r are POSIX extensions, not strictly ANSI C.
# timegm is a nonstandard extension present on Linux and the BSDs, but missing
# on other systems.
CHECK_CXX_SYMBOL_EXISTS(strnlen "cstring" QUICKSTEP_HAVE_STRNLEN)
CHECK_CXX_SYMBOL_EXISTS(gmtime_r "ctime" QUICKSTEP_HAVE_GMTIME_R)
CHECK_CXX_SYMBOL_EXISTS(localtime_r "ctime" QUICKSTEP_HAVE_LOCALTIME_R)
CHECK_CXX_SYMBOL_EXISTS(timegm "ctime" QUICKSTEP_HAVE_TIMEGM)

if (NOT QUICKSTEP_HAVE_GMTIME_R)
  # Try Windows gmtime_s
  CHECK_CXX_SYMBOL_EXISTS(gmtime_s "ctime" QUICKSTEP_HAVE_GMTIME_S)
endif()

if (NOT (QUICKSTEP_HAVE_GMTIME_R OR QUICKSTEP_HAVE_GMTIME_S))
  message(FATAL_ERROR "Unable to find a reentrant (threadsafe) version of "
                      "gmtime (tried POSIX gmtime_r and Windows gmtime_s).")
endif()

if (NOT QUICKSTEP_HAVE_LOCALTIME_R)
  # Try Windows localtime_s
  CHECK_CXX_SYMBOL_EXISTS(localtime_s "ctime" QUICKSTEP_HAVE_LOCALTIME_S)
endif()

if (NOT (QUICKSTEP_HAVE_LOCALTIME_R OR QUICKSTEP_HAVE_LOCALTIME_S))
  message(FATAL_ERROR "Unable to find a reentrant (threadsafe) version of "
                      "localtime (tried POSIX localtime_r and Windows localtime_s).")
endif()

CHECK_CXX_SYMBOL_EXISTS(setenv "stdlib.h" QUICKSTEP_HAVE_SETENV)
CHECK_CXX_SYMBOL_EXISTS(tzset "time.h" QUICKSTEP_HAVE_TZSET)


configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/TypesPortConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/TypesPortConfig.h"
)

add_library(quickstep_types_port_gmtime_r ../../empty_src.cpp gmtime_r.hpp)
add_library(quickstep_types_port_localtime_r ../../empty_src.cpp localtime_r.hpp)
add_library(quickstep_types_port_strnlen ../../empty_src.cpp strnlen.hpp)
add_library(quickstep_types_port_timegm timegm.cpp timegm.hpp)

target_link_libraries(quickstep_types_port_gmtime_r
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_port_localtime_r
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_port_timegm
                      quickstep_types_port_gmtime_r
                      quickstep_utility_Macros)

add_library(quickstep_types_port ../../empty_src.cpp)
target_link_libraries(quickstep_types_port
                      quickstep_types_port_gmtime_r
                      quickstep_types_port_localtime_r
                      quickstep_types_port_strnlen
                      quickstep_types_port_timegm)

# Tests
add_executable(timegm_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/timegm_unittest.cpp")
target_link_libraries(timegm_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_types_port_timegm)
add_test(timegm_unittest timegm_unittest)

# Benchmarks
if (UNIX)
  add_executable(timegm_benchmark "${CMAKE_CURRENT_SOURCE_DIR}/tests/timegm_benchmark.cpp")
  target_link_libraries(timegm_benchmark
                        benchmark
                        quickstep_types_port_timegm
                        ${LIBS})
endif()
