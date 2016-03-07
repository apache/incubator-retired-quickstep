# Copyright 2015 The RE2 Authors.  All Rights Reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

#   Copyright 2016 Pivotal Software, Inc.
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

# This CMAKELists file is modified based on the original one from re2. The
# changes include disabling benchmarks and unit tests, and suppressing build
# warnings.

set(RE2_SOURCE_DIR "../re2")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  if(MSVC_VERSION LESS 1800)
    message(FATAL_ERROR "you need Visual Studio 2013 or later")
  endif()
endif()

# Quickstep is normally compiled with -Wall, but we disable some warnings ONLY
# when compiling re2 that would otherwise produce a flurry of warnings
# during the build that have nothing to do with actual Quickstep code.
include(CheckCXXCompilerFlag)
set(RE2_WARNING_FLAGS)

CHECK_CXX_COMPILER_FLAG("-Wno-pedantic" COMPILER_HAS_WNO_PEDANTIC)
if (COMPILER_HAS_WNO_PEDANTIC)
  set(RE2_WARNING_FLAGS "${RE2_WARNING_FLAGS} -Wno-pedantic")
endif()

include_directories(${RE2_SOURCE_DIR})

set(RE2_LIBRARY_SOURCES
    ${RE2_SOURCE_DIR}/re2/bitstate.cc
    ${RE2_SOURCE_DIR}/re2/compile.cc
    ${RE2_SOURCE_DIR}/re2/dfa.cc
    ${RE2_SOURCE_DIR}/re2/filtered_re2.cc
    ${RE2_SOURCE_DIR}/re2/mimics_pcre.cc
    ${RE2_SOURCE_DIR}/re2/nfa.cc
    ${RE2_SOURCE_DIR}/re2/onepass.cc
    ${RE2_SOURCE_DIR}/re2/parse.cc
    ${RE2_SOURCE_DIR}/re2/perl_groups.cc
    ${RE2_SOURCE_DIR}/re2/prefilter.cc
    ${RE2_SOURCE_DIR}/re2/prefilter_tree.cc
    ${RE2_SOURCE_DIR}/re2/prog.cc
    ${RE2_SOURCE_DIR}/re2/re2.cc
    ${RE2_SOURCE_DIR}/re2/regexp.cc
    ${RE2_SOURCE_DIR}/re2/set.cc
    ${RE2_SOURCE_DIR}/re2/simplify.cc
    ${RE2_SOURCE_DIR}/re2/stringpiece.cc
    ${RE2_SOURCE_DIR}/re2/tostring.cc
    ${RE2_SOURCE_DIR}/re2/unicode_casefold.cc
    ${RE2_SOURCE_DIR}/re2/unicode_groups.cc
    ${RE2_SOURCE_DIR}/util/hash.cc
    ${RE2_SOURCE_DIR}/util/logging.cc
    ${RE2_SOURCE_DIR}/util/rune.cc
    ${RE2_SOURCE_DIR}/util/stringprintf.cc
    ${RE2_SOURCE_DIR}/util/strutil.cc
    ${RE2_SOURCE_DIR}/util/valgrind.cc
    )

add_library(re2 ${RE2_LIBRARY_SOURCES})
if (RE2_WARNING_FLAGS)
  set_target_properties(re2 PROPERTIES COMPILE_FLAGS ${RE2_WARNING_FLAGS})
endif()
target_link_libraries(re2 ${CMAKE_THREAD_LIBS_INIT})
