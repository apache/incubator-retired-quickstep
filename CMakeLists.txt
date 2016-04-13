#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015 Pivotal Software, Inc.
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

cmake_minimum_required (VERSION 2.8.6)
project (QUICKSTEP)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)

if (WIN32)
  # Make Windows compilers shut up about using standard libc functions.
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS
  )
endif()

include(CheckIncludeFiles)
check_include_files(
    "termios.h;unistd.h;stdlib.h;stdio.h;errno.h;string.h;stdlib.h;ctype.h;sys/types.h;sys/ioctl.h;unistd.h"
    HAVE_LINENOISE_HEADERS)
if (HAVE_LINENOISE_HEADERS)
  option(USE_LINENOISE "Use linenoise command-line editing library" ON)
else()
  option(USE_LINENOISE "Use linenoise command-line editing library" OFF)
endif()

check_include_files("hdfs/hdfs.h" HAVE_LIBHDFS3_HEADERS)
if (HAVE_LIBHDFS3_HEADERS)
  option(ENABLE_HDFS "Enable HDFS FileManager" ON)
else()
  option(ENABLE_HDFS "Enable HDFS FileManager" OFF)
endif()

if (UNIX AND NOT CYGWIN)
  option(USE_TCMALLOC "Use tcmalloc (Google's thread-cacheing malloc) instead of system-provided malloc" ON)
else()
  option(USE_TCMALLOC "Use tcmalloc (Google's thread-cacheing malloc) instead of system-provided malloc" OFF)
endif()

# Option to enable/disable LTO.
option(ENABLE_LTO "Enable LTO (Link Time Optimization), if the compiler supports it." OFF)

set(VECTOR_COPY_ELISION_MSG_LIST
    "This options controls whether extra code paths for vectorized evaluation "
    "of expressions and predicates directly on values inside storage blocks "
    "without copying into ColumnVectors is enabled. This may improve "
    "performance, but causes a combinatorial explosion of templated code to "
    "be compiled, which may cause very long builds (especially in Release "
    "mode) and very large executables (especially in Debug mode). The "
    "valid settings for this option are, in order of increasing code paths: "
    "none (attribute values are always copied into ColumnVectors before "
    "computing expressions on them), selection (copies are elided for "
    "single-relation SELECT operations), join (copies are also elided for "
    "expressions in the output of hash joins, but binary expressions will "
    "only elide a copy on one side, not both), and joinwithbinaryexpressions "
    "(copies are elided for both sides of a binary expression over a join).")
string(REPLACE ";" "" VECTOR_COPY_ELISION_MSG ${VECTOR_COPY_ELISION_MSG_LIST})

set(VECTOR_COPY_ELISION_LEVEL "selection" CACHE STRING ${VECTOR_COPY_ELISION_MSG})
set_property(CACHE VECTOR_COPY_ELISION_LEVEL PROPERTY STRINGS none selection join joinwithbinaryexpressions)

if (${VECTOR_COPY_ELISION_LEVEL} STREQUAL "none")
  message("Vector copy elision level set to: none")
elseif(${VECTOR_COPY_ELISION_LEVEL} STREQUAL "selection")
  message("Vector copy elision level set to: single-relation selection")
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  )
elseif(${VECTOR_COPY_ELISION_LEVEL} STREQUAL "join")
  message("Vector copy elision level set to: joins (limited to one side of binary expressions)")
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  )
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  )
elseif(${VECTOR_COPY_ELISION_LEVEL} STREQUAL "joinwithbinaryexpressions")
  message("Vector copy elision level set to: joins (including binary expressions)")
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  )
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
  )
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN_WITH_BINARY_EXPRESSIONS
  )
else()
  message(
      FATAL_ERROR
      "Unknown value for VECTOR_COPY_ELISION_LEVEL. Valid choices are: none, selection, join, joinwithbinaryexpressions")
endif()

# TODO(chasseur): There are some interesting performance experiments we could
# do with short-circuiting turned on and off. We could even make it selective
# based on the density of the filter bitmap.
set(VECTOR_PREDICATE_SHORT_CIRCUIT_MSG_LIST
    "If enabled, vectorized predicate evaluation for conjunctions and "
    "disjunctions will keep track of a filter of tuples already known to "
    "match or not match the overall predicate to reduce the number of "
    "comparisons that actually need to be checked. Otherwise sub-predicates "
    "will always be checked for all tuples in a block and their match sets "
    "will be intersected/unioned after the fact. Turning this option on is "
    "expected to reduce the number of comparisons performed, but it changes "
    "the inner loop for predicate evaluation from simply incrementing a "
    "tuple_id to scanning for bits set in a filter. Which option performs "
    "best is likely to be dependent on the selectivity of sub-predicates for "
    "a particular query.")
string(REPLACE ";" "" VECTOR_PREDICATE_SHORT_CIRCUIT_MSG ${VECTOR_PREDICATE_SHORT_CIRCUIT_MSG_LIST})
option(ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT ${VECTOR_PREDICATE_SHORT_CIRCUIT_MSG} ON)
if (ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT)
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  )
endif()

# Turn on the QUICKSTEP_DEBUG flag in the source if this is a debug build.
if (CMAKE_MAJOR_VERSION GREATER 2)
  cmake_policy(SET CMP0043 NEW)
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:QUICKSTEP_DEBUG>
  )
else()
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG QUICKSTEP_DEBUG
  )
endif()

# Link against the system's threading library.
find_package(Threads REQUIRED)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})
set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# Link with libnuma library if it's present
find_package(LibNuma)
if(LIBNUMA_FOUND)
  include_directories(${LIBNUMA_INCLUDE_DIR})
endif()

# Enable C++14 support.
set(CPP14_SUPPORTED FALSE)
include(CheckCXXSourceCompiles)
if(NOT CPP14_SUPPORTED)
  if (MSVC_VERSION GREATER 1899)
    message("Your C++ compiler appears to be Microsoft Visual Studio 2015 or "
            "better, which supports C++14 by default.")
    set(CPP14_SUPPORTED TRUE)
  endif()
endif()
if (NOT CPP14_SUPPORTED)
  CHECK_CXX_COMPILER_FLAG("-std=c++14" CXX_COMPILER_SUPPORTS_CPP14)
  if (CXX_COMPILER_SUPPORTS_CPP14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    set(CPP14_SUPPORTED TRUE)
  endif()
endif()
if (NOT CPP14_SUPPORTED)
  CHECK_CXX_COMPILER_FLAG("-std=c++1y" CXX_COMPILER_SUPPORTS_CPP1Y)
  if (CXX_COMPILER_SUPPORTS_CPP1Y)
    message(WARNING "Your C++ compiler appears to support pre-standardized "
                    "C++14 (AKA C++1y). Quickstep may fail to build if your "
                    "compiler and standard library are not sufficiently C++14 "
                    "compliant.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
    set(CPP14_SUPPORTED TRUE)
  endif()
endif()
if (NOT CPP14_SUPPORTED)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" CXX_COMPILER_SUPPORTS_CPP11)
  if (CXX_COMPILER_SUPPORTS_CPP11)
    message(WARNING "Your C++ compiler appears to support C++11, but does not "
                    "have flags to enable C++14 support. Quickstep may fail "
                    "to build if your compiler and standard library are not "
                    "sufficiently C++14 compliant.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CPP14_SUPPORTED TRUE)
  endif()
endif()
if (NOT CPP14_SUPPORTED)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" CXX_COMPILER_SUPPORTS_CPP0X)
  if (CXX_COMPILER_SUPPORTS_CPP0X)
    message(WARNING "Your C++ compiler appears to only support "
                    "pre-standardized C++0X (eventually ratified as C++11). "
                    "Quickstep requires C++14 support, and the build is "
                    "likely to fail if your compiler and standard library are "
                    "not sufficiently C++14 compliant.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    set(CPP14_SUPPORTED TRUE)
  endif()
endif()
if (NOT CPP14_SUPPORTED)
  message (WARNING "Could not find flags to enable C++14 or C++11 support for "
                   "your compiler. If your compiler does not support C++14 by "
                   "default, Quickstep will fail to build.")
endif()

# Also define _ISOC11_SOURCE, which turns on C library declarations from C11.
set_property(
  DIRECTORY
  APPEND PROPERTY COMPILE_DEFINITIONS _ISOC11_SOURCE
)

# Enable all warnings.
if (MSVC)
  CHECK_CXX_COMPILER_FLAG("/W3" MSVC_HAS_W3)
  if (MSVC_HAS_W3)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
  endif()
else()
  CHECK_CXX_COMPILER_FLAG("-Wall" COMPILER_HAS_WALL)
  if (COMPILER_HAS_WALL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif()
  CHECK_CXX_COMPILER_FLAG("-pedantic" COMPILER_HAS_PEDANTIC)
  if (COMPILER_HAS_PEDANTIC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
  endif()

  # Treat warnings as errors, 'cause we hardcore.
  CHECK_CXX_COMPILER_FLAG("-Werror" COMPILER_HAS_WERROR)
  if (COMPILER_HAS_WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()

  # One of the protobuf headers includes a nested anonymous union within
  # another anonymous type. Ordinarily we work around this by compiling the
  # protobuf libraries themselves with "-Wno-nested-anon-types" and including
  # the protobuf headers as "system" headers so that the compiler automatically
  # suppresses warnings originating from them. XCode does not honor the
  # system-include setting, however, so we must instead suppress the warnings
  # with global CXXFLAGS for the project.
  #
  # TODO(chasseur): Once protobuf 3.0 has a stable release and Quickstep has
  # upgraded, remove this workaround.
  if (${CMAKE_GENERATOR} MATCHES "Xcode")
    CHECK_CXX_COMPILER_FLAG("-Wno-nested-anon-types" XCODE_HAS_WNO_NESTED_ANON_TYPES)
    if (XCODE_HAS_WNO_NESTED_ANON_TYPES)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nested-anon-types")
    endif()
  endif()
endif()

# Option to use LLVM's libc++ even if it is not the default C++ standard
# library on this platform.
#
# NOTE(chasseur): clang is the default compiler and libc++ is the default
# standard library on recent versions of FreeBSD and Mac OS X. On some other
# OSes (e.g. most Linux distributions), clang will compile against the
# platform's default C++ library (e.g. GNU libstdc++) instead for reasons of
# ABI/linking compatibility. This option allows us to override that behavior if
# we want to.
option(USE_LIBCXX "Use LLVM's libc++ instead of the default C++ standard library" OFF)
if (USE_LIBCXX)
  CHECK_CXX_COMPILER_FLAG("-stdlib=libc++" CXX_COMPILER_HAS_LIBCXX)
  if (NOT CXX_COMPILER_HAS_LIBCXX)
    message(FATAL_ERROR "USE_LIBCXX is set to ON, but compiler doesn't allow "
                        "-stdlib=libc++ (are you using some compiler other "
                        "than clang++?)")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  set(THIRD_PARTY_CXX_FLAGS "${THIRD_PARTY_CXX_FLAGS} -stdlib=libc++")

  # Check that a minimal program compiles and links. Clang will accept the
  # command-line flag for libc++ even if the library isn't actually installed.
  CHECK_CXX_SOURCE_COMPILES("
    #include <string>
    #include <iostream>

    int main() {
      std::string msg(\"hello world\");
      std::cout << msg;
      return 0;
    }" LIBCXX_CHECK)
    if (NOT LIBCXX_CHECK)
      message(FATAL_ERROR "Compiler accepts -stdlib=libc++ flag, but cannot "
                          "compile programs when it is turned on.")
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  # Optimization flags for GCC.

  # Strip binaries for release builds, except on Mac OS X, where this confuses
  # the linker.
  if (NOT (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -s")
  endif()

  # If using tcmalloc, prevent GCC from optimizing for builtin malloc and friends.
  if(USE_TCMALLOC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
  endif()

  # Automatically optimize for host processor.
  CHECK_CXX_COMPILER_FLAG("-march=native" GCC_HAS_MARCH_NATIVE)
  if (GCC_HAS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
  endif()

  # On 32-bit x86, GCC defaults to using i387 (not a typo) for floating-point
  # math. This is slower than SSE, and also uses 80-bit temporaries that don't
  # correspond to any of the IEEE 754 floating-point formats, so that rounding
  # errors may be introduced relative to FPUs that do math in a normal IEEE 754
  # encoding. Using SSE is preferable for both performance and correctness, so
  # we enable it here.
  if ((CMAKE_SIZEOF_VOID_P EQUAL 4)
      AND ((CMAKE_SYSTEM_PROCESSOR MATCHES "[0-6x]86")
           OR (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64")))
    CHECK_CXX_COMPILER_FLAG("-mfpmath=sse" GCC_HAS_MFPMATH_SSE)
    if (GCC_HAS_MFPMATH_SSE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse")
    endif()
  endif()

  # Loop-transformation optimizations that aren't turned on by -O3
  CHECK_CXX_COMPILER_FLAG("-ftree-loop-linear" GCC_HAS_FTREE_LOOP_LINEAR)
  if (GCC_HAS_FTREE_LOOP_LINEAR)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ftree-loop-linear")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -ftree-loop-linear")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -ftree-loop-linear")
  endif()

  CHECK_CXX_COMPILER_FLAG("-ftree-loop-distribution" GCC_HAS_FTREE_LOOP_DISTRIBUTION)
  if (GCC_HAS_FTREE_LOOP_DISTRIBUTION)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ftree-loop-distribution")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -ftree-loop-distribution")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -ftree-loop-distribution")
  endif()

  CHECK_CXX_COMPILER_FLAG("-floop-strip-mine" GCC_HAS_FLOOP_STRIP_MINE)
  if (GCC_HAS_FLOOP_STRIP_MINE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -floop-strip-mine")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -floop-strip-mine")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -floop-strip-mine")
  endif()

  CHECK_CXX_COMPILER_FLAG("-floop-block" GCC_HAS_FLOOP_BLOCK)
  if (GCC_HAS_FLOOP_BLOCK)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -floop-block")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -floop-block")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -floop-block")
  endif()

  if (ENABLE_LTO)
    # If GCC supports inter-procedural (link-time) optimizations, turn them on.
    CHECK_CXX_COMPILER_FLAG("-flto" GCC_HAS_FLTO)
    if (GCC_HAS_FLTO)
      if(CMAKE_COMPILER_IS_GNUC)
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto")
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -flto")
        set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -flto")
      endif()

      set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -flto")
      set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -flto")

      set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
      set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} -flto")
      set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto")
    endif()
  endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  # Optimization flags for Clang.

  # Strip binaries for release builds, except on Mac OS X, where this confuses
  # the linker.
  if (NOT (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -s")
  endif()

  # Automatically optimize for host processor.
  CHECK_CXX_COMPILER_FLAG("-march=native" CLANG_HAS_MARCH_NATIVE)
  if(CLANG_HAS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
  endif()
#  Getting LTO to work with clang is a bit of a nightmare because of linker and ar dependencies, so it's not enabled.
#  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4")
#  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O4")
#  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Intel")
  # Optimization flags for the Intel C++ compiler.
  if (WIN32)
    # /fast implies /O3 /Qipo /no-prec-div
    # /QxHost automatically optimizes for host processor.
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /fast /QxHost")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /fast /QxHost")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /QxHost")
  else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHost")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -no-prec-div")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -no-prec-div")

    if (ENABLE_LTO)
      # Inter-procedural optimization requires using Intel's xiar tool instead
      # of vanilla ar.
      find_program(XIAR xiar)
      if(XIAR)
        set(CMAKE_AR "${XIAR}")

        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ipo")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -ipo")
      else()
        message(WARNING "Could not find Intel xiar tool, so turning OFF inter-procedural "
                        "optimizations for release builds.")
      endif()
    endif()

  endif()
elseif(MSVC)
  # Microsoft Visual studio.
  # Work around variadic template bug.
  add_definitions( /D _VARIADIC_MAX=10 )
endif()

# Set the include directories to the project root directory and the root of the build tree (where
# generated headers will go).
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Setup for third-party libs.
#
# NOTE(chasseur): If USE_LIBCXX is on, we should assume that we need to build
# external third-party C++ libraries ourselves even if they are installed on
# the system, because they may be linked against a different C++ standard
# library (e.g. GNU libstdc++) that is not ABI-compatible. This applies only to
# C++ libraries; pure C libraries should have no such issue.
set(THIRD_PARTY_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party")
include(ExternalProject)

if(USE_TCMALLOC)
  if(WIN32)
    message(FATAL_ERROR "You are building with tcmalloc on Windows. Our build system can not yet "
                        "produce tcmalloc on Windows (and the tcmalloc port for Windows is "
                        "incomplete anyway). You should use the system-provided malloc instead "
                        "(invoke cmake with \"-D USE_TCMALLOC=0\").")
  elseif(NOT UNIX)
    message(WARNING "Your system does not appear to be UNIX or Windows, but you are building with "
                    "tcmalloc. This is quite likely to fail, in which case you should use the "
                    "system-provided malloc instead (invoke cmake with \"-D USE_TCMALLOC=0\").")
  elseif(CYGWIN)
    message(WARNING "You are building with tcmalloc on Windows. For many configurations, this is "
                    "not stable. If you experience build errors, or runtime hangs/crashes, you "
                    "should use the system-provided malloc instead (invoke cmake with "
                    "\"-D USE_TCMALLOC=0\").")
  endif()

  # Build tcmalloc as a cmake ExternalProject.
  ExternalProject_Add(
    libtcmalloc_ext
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third_party/gperftools
    SOURCE_DIR ${THIRD_PARTY_SOURCE_DIR}/gperftools
    CONFIGURE_COMMAND <SOURCE_DIR>/configure
        --prefix=<INSTALL_DIR>
        --enable-minimal
        --disable-debugalloc
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
        CFLAGS=${THIRD_PARTY_C_FLAGS}
        CXXFLAGS=${THIRD_PARTY_CXX_FLAGS}
    BUILD_COMMAND make
    BUILD_IN_SOURCE 0
  )
  # Static libtcmalloc_minimal.a
  add_library(libtcmalloc_minimal STATIC IMPORTED)
  set_property(TARGET libtcmalloc_minimal PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/third_party/gperftools/lib/libtcmalloc_minimal.a)
  # Linking against tcmalloc also requires linking against the system threading
  # library.
  set_property(TARGET libtcmalloc_minimal PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
  add_dependencies(libtcmalloc_minimal libtcmalloc_ext)
  set(LIBS ${LIBS} libtcmalloc_minimal)

  # With tcmalloc enabled, utility/Alignment.hpp includes tcmalloc.h, which can
  # trigger a compiler warning about the linkage of the tc_mallinfo() function.
  # This supresses that warning.
  CHECK_CXX_COMPILER_FLAG("-Wno-return-type-c-linkage"
                          COMPILER_HAS_WNO_RETURN_TYPE_C_LINKAGE)
  if (COMPILER_HAS_WNO_RETURN_TYPE_C_LINKAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage")
  endif()
endif()

# Add Protocol Buffers.
# First, see if protobufs are already installed on this system (unless
# USE_LIBCXX is on, in which case we compile ourselves to be safe, in
# accordance with the above note).
set(PROTOBUF_GLOBAL_DEPS)
if (NOT USE_LIBCXX)
  include(FindProtobuf)
endif()
if(NOT PROTOBUF_FOUND)
  # Build the copy of protobufs in third_party/
  add_subdirectory("${THIRD_PARTY_SOURCE_DIR}/protobuf_cmake" "${CMAKE_CURRENT_BINARY_DIR}/third_party/protobuf")
  set(PROTOBUF_FOUND 1)
  set(PROTOBUF_INCLUDE_DIR "${THIRD_PARTY_SOURCE_DIR}/protobuf/src")
  set(PROTOBUF_LIBRARY libprotobuf)
  set(PROTOBUF_GLOBAL_DEPS protoc)
  set(PROTOBUF_PROTOC_EXECUTABLE protoc)
endif()
include_directories(SYSTEM ${PROTOBUF_INCLUDE_DIR})

# This function is partially based on a similar one from FindProtobuf.cmake,
# but it adds the ability to scan .proto files for imports and automatically
# set the proper dependencies.
function(QS_PROTOBUF_GENERATE_CPP SRCS HDRS)
  if(NOT ARGN)
    message(SEND_ERROR "Error: QS_PROTOBUF_GENERATE_CPP() called without any proto files")
    return()
  endif()

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    # Scan for other protos imported in the .proto file.
    set(PROTO_IMPORT_TARGETS)
    file(STRINGS ${ABS_FIL} PROTO_SRC)
    foreach(PROTO_SRC_LINE ${PROTO_SRC})
      string(REGEX MATCH "import \"(.*)[.]proto\"" TMPMATCH ${PROTO_SRC_LINE})
      if (CMAKE_MATCH_1)
        string(REPLACE "/" "_" IMPORTED_PROTO_FLAT "${CMAKE_BINARY_DIR}/${CMAKE_MATCH_1}")
        string(REPLACE ":" "_" IMPORTED_PROTO_FLAT ${IMPORTED_PROTO_FLAT})
        list(APPEND PROTO_IMPORT_TARGETS "protoc_${IMPORTED_PROTO_FLAT}")
      endif()
    endforeach()

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")

    # Note that this adds dependencies for the .proto file itself, as well as
    # for any proto imports and the protobuf library if we have to build it
    # ourselves.
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc"
             "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h"
      COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
      ARGS --cpp_out  ${CMAKE_BINARY_DIR} -I${PROJECT_SOURCE_DIR} ${ABS_FIL}
      DEPENDS ${ABS_FIL} ${PROTO_IMPORT_TARGETS} ${PROTOBUF_GLOBAL_DEPS}
      COMMENT "Running C++ protocol buffer compiler on ${FIL}"
      VERBATIM)

    # Make a custom global target so that we have something that protos in
    # other directories can depend on for imports.
    string(REPLACE "/" "_" PROTOC_TARGET_NAME "protoc_${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}")
    string(REPLACE ":" "_" PROTOC_TARGET_NAME ${PROTOC_TARGET_NAME})
    add_custom_target(${PROTOC_TARGET_NAME} DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
  endforeach()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

# Add gtest unit-testing framework.
set(gtest_force_shared_crt ON CACHE BOOL "Link gtest against shared DLLs on Windows")
add_subdirectory ("${THIRD_PARTY_SOURCE_DIR}/googletest/googletest"
                  "${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest/googletest")
include_directories(SYSTEM "${THIRD_PARTY_SOURCE_DIR}/googletest/googletest/include")
enable_testing()

if (UNIX)
  # Add google/benchmark micro-benchmarking framework.
  # This does not build in Windows. It builds fine on NetBSD, but produces
  # executables that always segfault on startup.
  if (${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
    message(WARNING "Building micro-benchmarks on NetBSD using google "
                    "benchmark library. This will build fine, but as of "
                    "NetBSD 6.1.5 the micro-benchmark executables always "
                    "segfault on startup due to a bug in the implementation "
                    "of c++11 threads on top of NetBSD's pthread library. "
                    "This issue ONLY affects micro-benchmarks; quickstep "
                    "itself, as well as unit tests and other utilities are "
                    "not affected.")
  endif()
  add_subdirectory ("${THIRD_PARTY_SOURCE_DIR}/benchmark" "${CMAKE_CURRENT_BINARY_DIR}/third_party/benchmark")
  include_directories("${THIRD_PARTY_SOURCE_DIR}/benchmark/include")
endif()

# Add required cmake-controlled third-party libraries (farmhash, gflags, glog, and re2).
add_subdirectory ("${THIRD_PARTY_SOURCE_DIR}/farmhash" "${CMAKE_CURRENT_BINARY_DIR}/third_party/farmhash")

add_subdirectory ("${THIRD_PARTY_SOURCE_DIR}/gflags" "${CMAKE_CURRENT_BINARY_DIR}/third_party/gflags")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/third_party/gflags/include")

add_subdirectory ("${THIRD_PARTY_SOURCE_DIR}/glog" "${CMAKE_CURRENT_BINARY_DIR}/third_party/glog")
if (WIN32)
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS GOOGLE_GLOG_DLL_DECL=
  )
  include_directories(${THIRD_PARTY_SOURCE_DIR}/glog/src/windows)
else()
  include_directories(${THIRD_PARTY_SOURCE_DIR}/glog/src)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/third_party)
endif()

add_subdirectory ("${THIRD_PARTY_SOURCE_DIR}/re2_cmake" "${CMAKE_CURRENT_BINARY_DIR}/third_party/re2")
include_directories("${THIRD_PARTY_SOURCE_DIR}/re2")

# Add optional linenoise command-line editing library.
if (USE_LINENOISE)
  if (NOT HAVE_LINENOISE_HEADERS)
    message(WARNING "You are attempting to enable the linenoise command-line "
                    "editing library, but CMake was unable to find the "
                    "necessary headers on your system. Your build is likely "
                    "to fail, in which case you should try again with "
                    "-D USE_LINENOISE=0")
  endif()
  add_subdirectory ("${THIRD_PARTY_SOURCE_DIR}/linenoise" "${CMAKE_CURRENT_BINARY_DIR}/third_party/linenoise")
endif()

# Add optional libhdfs3 library
if (ENABLE_HDFS)
  find_package(Libhdfs3 REQUIRED)
  include_directories(${LIBHDFS3_INCLUDE_DIR})
endif()

# Add TMB, but only enable the PureMemory implementation.
set(ENABLE_PUREMEMORY ON CACHE BOOL "Enable PureMemory TMB")
set(ENABLE_LEVELDB OFF CACHE BOOL "Enable LevelDB TMB")
set(ENABLE_MEMORYMIRROR OFF CACHE BOOL "Enable MemoryMirror TMB")
set(ENABLE_NATIVELOG OFF CACHE BOOL "Enable NativeLog TMB")
set(ENABLE_NATIVENET OFF CACHE BOOL "Enable NativeNet TMB")
set(ENABLE ENABLE_SQLITE OFF CACHE BOOL "Enable SQLite TMB")
set(ENABLE_VOLTDB OFF CACHE BOOL "Enable VoltDB TMB")
set(ENABLE_ZOOKEEPER OFF CACHE BOOL "Enable Zookeeper TMB")

add_subdirectory("${THIRD_PARTY_SOURCE_DIR}/tmb" "${CMAKE_CURRENT_BINARY_DIR}/third_party/tmb")
include_directories(${TMB_INCLUDE_DIRS})

# Add all of the module subdirectories. CMakeLists.txt in each of the subdirectories
# defines how to build that module's libraries.
add_subdirectory(catalog)
add_subdirectory(cli)
add_subdirectory(compression)
add_subdirectory(expressions)
add_subdirectory(parser)
add_subdirectory(query_execution)
add_subdirectory(query_optimizer)
add_subdirectory(relational_operators)
add_subdirectory(storage)
add_subdirectory(threading)
add_subdirectory(transaction)
add_subdirectory(types)
add_subdirectory(utility)
add_subdirectory(yarn)

# Build the quickstep_cli_shell executable.
add_executable (quickstep_cli_shell cli/QuickstepCli.cpp)
# Link against direct deps (will transitively pull in everything needed).
target_link_libraries(quickstep_cli_shell
                      gflags_nothreads-static
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_cli_CommandExecutor
                      quickstep_cli_DefaultsConfigurator
                      quickstep_cli_DropRelation
                      quickstep_cli_InputParserUtil
                      quickstep_cli_LineReader
                      quickstep_cli_PrintToScreen
                      quickstep_parser_ParseStatement
                      quickstep_parser_SqlParserWrapper
                      quickstep_queryexecution_Foreman
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_Worker
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_queryexecution_WorkerMessage
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_queryoptimizer_QueryProcessor
                      quickstep_storage_PreloaderThread
                      quickstep_threading_ThreadIDBasedMap
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector
                      quickstep_utility_SqlError)
if (ENABLE_HDFS)
  target_link_libraries(quickstep_cli_shell
                        quickstep_storage_FileManagerHdfs)
endif()

# Link against other required system and third-party libraries.
target_link_libraries(quickstep_cli_shell ${LIBS})
