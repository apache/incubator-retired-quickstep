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

cmake_minimum_required(VERSION 2.8)

# Declare TMB project.
project(tmb CXX C)

# Look for flags to enable C++11 support, if any.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_HAS_STD_CXX11)
if (COMPILER_HAS_STD_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_HAS_STD_CXX0X)
  if (COMPILER_HAS_STD_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endif()

if (WIN32)
  # Disable warnings about using standard C library functions on Windows.
  set_property(DIRECTORY
               APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
endif()

# Turn on all warnings.
if (NOT MSVC)
  # We don't do -Wall on MSVC, because that is interpreted as /W4, which in
  # turn implies /WX, meaning that warnings are treated as errors.
  CHECK_CXX_COMPILER_FLAG("-Wall" COMPILER_HAS_WALL)
  if (COMPILER_HAS_WALL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif()
  CHECK_CXX_COMPILER_FLAG("-pedantic" COMPILER_HAS_PEDANTIC)
  if (COMPILER_HAS_PEDANTIC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
  endif()
endif()

# For release builds, disable warnings about unused variables that are only
# used in assertions.
CHECK_CXX_COMPILER_FLAG("-Wno-unused-variable"
                        COMPILER_HAS_WNO_UNUSED_VARIABLE)
if (COMPILER_HAS_WNO_UNUSED_VARIABLE)
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-variable")
endif()
CHECK_CXX_COMPILER_FLAG("-Wno-unused-but-set-variable"
                        COMPILER_HAS_WNO_UNUSED_BUT_SET_VARIABLE)
if (COMPILER_HAS_WNO_UNUSED_BUT_SET_VARIABLE)
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-but-set-variable")
endif()

# Find the system's threading library.
find_package(Threads REQUIRED)

option(BUILD_BENCHMARKS "Build TMB benchmarks in addition to core library" OFF)

option(ENABLE_LEVELDB "Enable LevelDBMessageBus implementation" OFF)
option(ENABLE_MEMORYMIRROR "Enable MemoryMirrorMessageBus implementation" ON)
option(
    ENABLE_NATIVELOG
    "Enable NativeTransactionLog for persistence with MemoryMirrorMessageBus"
    ON)
option(ENABLE_NATIVENET "Enable native network protocol" ON)
option(ENABLE_PUREMEMORY "Enable PureMemoryMessageBus implementation" ON)
option(ENABLE_SQLITE "Enable SQLiteMessageBus implementation" OFF)
option(ENABLE_VOLTDB "Enable VoltDBMessageBus implementation" OFF)
option(ENABLE_ZOOKEEPER "Enable ZookeeperMessageBus implementation" OFF)

set(VOLTDB_CLASSPATH "/opt/voltdb/voltdb/*"
                     CACHE STRING "Classpath for VoltDB jars")
set(VOLTDB_COMMAND "/opt/voltdb/bin/voltdb"
                   CACHE STRING "Path for voltdb command")

# Find necessary third-party libraries.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(TMB_SRCS src/message_bus.cc)
set(TMB_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

if (ENABLE_LEVELDB)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_LEVELDB_ENABLED)

  set(TMB_SRCS ${TMB_SRCS}
               src/leveldb_key_comparator.cc
               src/leveldb_message_bus.cc)

  find_package(LevelDB REQUIRED)
  set(TMB_INCLUDE_DIRS ${TMB_INCLUDE_DIRS}
                       ${LEVELDB_INCLUDE_DIRS})
  include_directories(${LEVELDB_INCLUDE_DIRS})
endif()

if (ENABLE_PUREMEMORY OR ENABLE_MEMORYMIRROR)
  set(TMB_SRCS ${TMB_SRCS}
               src/heap_receiver_message_queue.cc
               src/memory_based_message_bus.cc
               src/tree_receiver_message_queue.cc)
endif()

if (ENABLE_MEMORYMIRROR)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_MEMORYMIRROR_ENABLED)

  set(TMB_SRCS ${TMB_SRCS}
               src/memory_mirror_message_bus.cc)
endif()

if (ENABLE_NATIVELOG)
  if (NOT ENABLE_MEMORYMIRROR)
    message(FATAL_ERROR
            "ENABLE_NATIVELOG is turned on but ENABLE_MEMORYMIRROR is turned "
            "off. The NativeTransactionLog is useless without "
            "MemoryMirrorMessageBus.")
  endif()

  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_NATIVELOG_ENABLED)

  # Check for necessary POSIX I/O syscalls.
  include(CheckCXXSourceCompiles)
  CHECK_CXX_SOURCE_COMPILES("
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int main() {
  int fd = open(\"/dev/null\", O_WRONLY, S_IRUSR | S_IWUSR);

  off_t endpos = lseek(fd, 0, SEEK_END);
  ftruncate(fd, endpos + 10);

  struct iovec desc;
  desc.iov_base = nullptr;
  desc.iov_len = 0;
  writev(fd, &desc, 1);

  close(fd);

  return 0;
}
" POSIX_IO_SUPPORTED)
  if (POSIX_IO_SUPPORTED)
    set_property(DIRECTORY
                 APPEND PROPERTY COMPILE_DEFINITIONS TMB_POSIX_IO_SUPPORTED)

    # Check for fdatasync() syscall. It is part of the POSIX realtime
    # extensions, not the core spec. Linux has it, for example, but FreeBSD
    # does not.
    include(CheckSymbolExists)
    CHECK_SYMBOL_EXISTS(fdatasync "unistd.h" POSIX_FDATASYNC_SUPPORTED)
    if (POSIX_FDATASYNC_SUPPORTED)
      set_property(DIRECTORY
                   APPEND PROPERTY COMPILE_DEFINITIONS TMB_POSIX_FDATASYNC_SUPPORTED)
    else()
      # Alternately, check for F_FULLFSYNC fcntl on Mac.
      CHECK_SYMBOL_EXISTS(F_FULLFSYNC "fcntl.h" DARWIN_FFULLFSYNC_SUPPORTED)
      if (DARWIN_FFULLSYNC_SUPPORTED)
        set_property(DIRECTORY
                     APPEND PROPERTY COMPILE_DEFINITIONS TMB_DARWIN_FFULLFSYNC_SUPPORTED)
      endif()
    endif()

    set(TMB_SRCS ${TMB_SRCS}
                 src/log_reader_posix.cc
                 src/log_writer_posix.cc)
  else()
    message(WARNING "POSIX.1-2001 I/O support is not available. Building SLOW "
                    "stdio-based native transaction log.")

    # Try to find a way to sync stdio file streams.
    CHECK_CXX_SOURCE_COMPILES("
#include <unistd.h>
#include <cstdio>

int main() {
  FILE *file = std::fopen(\"/dev/null\", \"wb\");
  std::fflush(file);
  int fd = fileno(file);
  fdatasync(fd);
  std::fclose(file);

  return 0;
}
" POSIX_FDATASYNC_SUPPORTED)

    if (NOT POSIX_FDATASYNC_SUPPORTED)
      CHECK_CXX_SOURCE_COMPILES("
#include <unistd.h>
#include <cstdio>

int main() {
  FILE *file = std::fopen(\"/dev/null\", \"wb\");
  std::fflush(file);
  int fd = fileno(file);
  fsync(fd);
  std::fclose(file);

  return 0;
}
" POSIX_FSYNC_SUPPORTED)
    endif()

    if (NOT (POSIX_FDATASYNC_SUPPORTED OR POSIX_FSYNC_SUPPORTED))
      CHECK_CXX_SOURCE_COMPILES("
#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <stdio.h>
#include <windows.h>

int main() {
  FILE *file = fopen(\"/dev/null\", \"wb\");
  fflush(file);
  HANDLE filehandle = reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(file)));
  FlushFileBuffers(filehandle);
  fclose(file);

  return 0;
}
" WINDOWS_FLUSHFILEBUFFERS_SUPPORTED)
    endif()

    if (POSIX_FDATASYNC_SUPPORTED)
      set_property(
          DIRECTORY
          APPEND PROPERTY COMPILE_DEFINITIONS TMB_POSIX_FDATASYNC_SUPPORTED)
    elseif (POSIX_FSYNC_SUPPORTED)
      set_property(
          DIRECTORY
          APPEND PROPERTY COMPILE_DEFINITIONS TMB_POSIX_FSYNC_SUPPORTED)
    elseif (WINDOWS_FLUSHFILEBUFFERS_SUPPORTED)
      set_property(
          DIRECTORY
          APPEND PROPERTY COMPILE_DEFINITIONS TMB_WINDOWS_FLUSHFILEBUFFERS_SUPPORTED)
    else()
      message(WARNING "Can not find any way to synchronously flush file "
                      "buffers to disk (tried POSIX fsync() and fdatasync(), "
                      "Windows FlushFileBuffers()). Synchronous logging will "
                      "NOT be available.")
    endif()

    set(TMB_SRCS ${TMB_SRCS}
                 src/log_reader_stdio.cc
                 src/log_writer_stdio.cc)
  endif()

  set(TMB_SRCS ${TMB_SRCS}
               src/crc32.cc
               src/native_transaction_log.cc)
endif()

if (ENABLE_NATIVENET)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_NATIVENET_ENABLED)

  find_package(Grpc++ REQUIRED)
  include_directories(${GRPCPLUSPLUS_INCLUDE_DIRS})

  GRPC_GENERATE_CPP(tmb_net_srcs tmb_net_hdrs include/tmb/internal src/proto/tmb_net.proto)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

  set(TMB_SRCS ${TMB_SRCS}
               src/native_net_client_message_bus.cc
               src/net_service_impl.cc
               ${tmb_net_srcs})
endif()

if (ENABLE_PUREMEMORY)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_PUREMEMORY_ENABLED)

  set(TMB_SRCS ${TMB_SRCS}
               src/pure_memory_message_bus.cc)
endif()

if (ENABLE_SQLITE)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_SQLITE_ENABLED)

  set(TMB_SRCS ${TMB_SRCS}
               src/sqlite_connection.cc
               src/sqlite_message_bus.cc)

  find_package(SQLite3 REQUIRED)
  set(TMB_INCLUDE_DIRS ${TMB_INCLUDE_DIRS}
                       ${SQLITE3_INCLUDE_DIRS})
  include_directories(${SQLITE3_INCLUDE_DIRS})
endif()

if (ENABLE_VOLTDB)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_VOLTDB_ENABLED)

  set(TMB_SRCS ${TMB_SRCS}
               src/voltdb_connection_pool.cc
               src/voltdb_message_bus.cc
               src/voltdb_procedure_factory.cc)

  # VoltDB C++ bindings.
  find_package(VoltDB REQUIRED)
  set(TMB_INCLUDE_DIRS ${TMB_INCLUDE_DIRS}
                       ${VOLTDB_INCLUDE_DIRS})
  include_directories(${VOLTDB_INCLUDE_DIRS})

  find_package(Java 1.7 REQUIRED)
  include(UseJava)

  # Compile Java stored procedures.
  set(CMAKE_JAVA_INCLUDE_PATH ${VOLTDB_CLASSPATH})
  add_jar(tmb_voltdb_stored_procedures
          src/java/CancelMessages.java
          src/java/ConnectClient.java
          src/java/DeleteMessages.java
          src/java/DeleteMessagesUnchecked.java
          src/java/DisconnectClient.java
          src/java/LoadState.java
          src/java/Receive.java
          src/java/ReceiveAndDelete.java
          src/java/RegisterReceiver.java
          src/java/RegisterSender.java
          src/java/ResetBus.java
          src/java/SendToAny.java
          src/java/SendToExplicitReceivers.java
          src/java/SendToExplicitReceiversUnchecked.java
          src/java/SendToSingleExplicitReceiver.java
          src/java/SendToSingleExplicitReceiverUnchecked.java)

  # Compile VoltDB catalog.
  add_custom_command(
      OUTPUT tmb_voltdb.jar
      COMMAND ${VOLTDB_COMMAND}
      ARGS compile --classpath=${CMAKE_CURRENT_BINARY_DIR}/tmb_voltdb_stored_procedures.jar -o tmb_voltdb.jar ${CMAKE_CURRENT_SOURCE_DIR}/src/sql/voltdb_schema.sql
      DEPENDS src/sql/voltdb_schema.sql src/java/* tmb_voltdb_stored_procedures
      COMMENT "Compiling VoltDB Catalog")
  add_custom_target(forcebuild ALL DEPENDS tmb_voltdb.jar)
endif()

if (ENABLE_ZOOKEEPER)
  set_property(
      DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS TMB_ZOOKEEPER_ENABLED)

  set(TMB_SRCS ${TMB_SRCS}
               src/zookeeper_message_bus.cc
               src/zookeeper_transaction_batch.cc)

  find_package(Zookeeper REQUIRED)
  set(TMB_INCLUDE_DIRS ${TMB_INCLUDE_DIRS}
                       ${ZOOKEEPER_INCLUDE_DIRS})
  include_directories(${ZOOKEEPER_INCLUDE_DIRS})
endif()

set_gflags_lib_name ()

# NOTE(chasseur): We only add gflags and gtest to the build if those targets
# don't already exist, so that TMB can be brought in to a build that already
# uses one or both of those libraries with add_subdirectory() and not cause
# name collisions.

# Build GFlags command-line processing library if needed.
if ((NOT TARGET ${GFLAGS_LIB_NAME}) AND (BUILD_BENCHMARKS OR ENABLE_NATIVENET))
  add_subdirectory(third_party/gflags)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/third_party/gflags/include)
endif()

# Googletest Framework For Unit Testing
if (NOT TARGET gtest)
  add_subdirectory(third_party/gtest)
  include_directories(third_party/gtest/include)
  enable_testing()
endif()

# Include path for TMB.
include_directories(${PROJECT_SOURCE_DIR}/include)
set(TMB_INCLUDE_DIRS ${TMB_INCLUDE_DIRS} CACHE STRING
    "Required include paths for applications using TMB")

# Where TMB libraries will be found.
link_directories(${tmb_BINARY_DIR}/src)

# Build TMB library.
add_library(tmb
            ${TMB_SRCS})
target_link_libraries(tmb
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${GFLAGS_LIB_NAME})

if (ENABLE_LEVELDB)
  target_link_libraries(tmb
                        ${LEVELDB_LIBRARIES})
endif()

if (ENABLE_NATIVENET)
  target_link_libraries(tmb
                        ${GRPCPLUSPLUS_LIBRARIES})
endif()

if (ENABLE_SQLITE)
  target_link_libraries(tmb
                        ${SQLITE3_LIBRARIES})
endif()

if (ENABLE_VOLTDB)
  target_link_libraries(tmb
                        ${VOLTDB_LIBRARIES})
endif()

if (ENABLE_ZOOKEEPER)
  target_link_libraries(tmb
                        ${ZOOKEEPER_LIBRARIES})
endif()

# Build the tmb_net_server executable if enabled.
if (ENABLE_NATIVENET)
  add_executable(tmb_net_server src/tmb_net_server.cc)
  target_link_libraries(tmb_net_server
                        gflags_nothreads-static
                        tmb)
endif()

if (BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

# Tests
include_directories(${PROJECT_SOURCE_DIR})

# Internal components
add_executable(rcu_unittest
               tests/rcu_unittest.cc)
target_link_libraries(rcu_unittest
                      gtest
                      gtest_main)
add_test(rcu_unittest rcu_unittest)

# Message Bus Implementations
if (ENABLE_LEVELDB)
  add_executable(leveldb_message_bus_unittest
                 tests/leveldb_message_bus_unittest.cc)
  target_link_libraries(leveldb_message_bus_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(leveldb_message_bus_unittest leveldb_message_bus_unittest)

  add_executable(leveldb_message_bus_async_unittest
                 tests/leveldb_message_bus_async_unittest.cc)
  target_link_libraries(leveldb_message_bus_async_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(leveldb_message_bus_async_unittest
           leveldb_message_bus_async_unittest)
endif()

if (ENABLE_MEMORYMIRROR AND ENABLE_LEVELDB)
  add_executable(memory_mirror_message_bus_with_leveldb_unittest
                 tests/memory_mirror_message_bus_with_leveldb_unittest.cc)
  target_link_libraries(memory_mirror_message_bus_with_leveldb_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(memory_mirror_message_bus_with_leveldb_unittest
           memory_mirror_message_bus_with_leveldb_unittest)
endif()

if (ENABLE_MEMORYMIRROR AND ENABLE_NATIVELOG)
  add_executable(native_logging_message_bus_unittest
                 tests/native_logging_message_bus_unittest.cc)
  target_link_libraries(native_logging_message_bus_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(native_logging_message_bus_unittest
           native_logging_message_bus_unittest)

  add_executable(native_logging_message_bus_async_unittest
                 tests/native_logging_message_bus_async_unittest.cc)
  target_link_libraries(native_logging_message_bus_async_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(native_logging_message_bus_async_unittest
           native_logging_message_bus_async_unittest)
endif()

if (ENABLE_MEMORYMIRROR AND ENABLE_SQLITE)
  add_executable(memory_mirror_message_bus_with_sqlite_unittest
                 tests/memory_mirror_message_bus_with_sqlite_unittest.cc)
  target_link_libraries(memory_mirror_message_bus_with_sqlite_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(memory_mirror_message_bus_with_sqlite_unittest
           memory_mirror_message_bus_with_sqlite_unittest)
endif()

if (ENABLE_MEMORYMIRROR AND ENABLE_VOLTDB)
  add_executable(memory_mirror_message_bus_with_voltdb_unittest
                 tests/memory_mirror_message_bus_with_voltdb_unittest.cc)
  target_link_libraries(memory_mirror_message_bus_with_voltdb_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(memory_mirror_message_bus_with_voltdb_unittest
           memory_mirror_message_bus_with_voltdb_unittest)
endif()

if (ENABLE_MEMORYMIRROR AND ENABLE_ZOOKEEPER)
  add_executable(memory_mirror_message_bus_with_zookeeper_unittest
                 tests/memory_mirror_message_bus_with_zookeeper_unittest.cc)
  target_link_libraries(memory_mirror_message_bus_with_zookeeper_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(memory_mirror_message_bus_with_zookeeper_unittest
           memory_mirror_message_bus_with_zookeeper_unittest)
endif()

if (ENABLE_NATIVENET)
  add_executable(native_net_client_message_bus_unittest
                 tests/native_net_client_message_bus_unittest.cc)
  target_link_libraries(native_net_client_message_bus_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(native_net_client_message_bus_unittest
           native_net_client_message_bus_unittest)
endif()

if (ENABLE_PUREMEMORY)
  add_executable(pure_memory_message_bus_unittest
                 tests/pure_memory_message_bus_unittest.cc)
  target_link_libraries(pure_memory_message_bus_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(pure_memory_message_bus_unittest pure_memory_message_bus_unittest)
endif()

if (ENABLE_SQLITE)
  add_executable(sqlite_message_bus_unittest
                 tests/sqlite_message_bus_unittest.cc)
  target_link_libraries(sqlite_message_bus_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(sqlite_message_bus_unittest sqlite_message_bus_unittest)
endif()

if (ENABLE_VOLTDB)
  add_executable(voltdb_message_bus_unittest
                 tests/voltdb_message_bus_unittest.cc)
  target_link_libraries(voltdb_message_bus_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(voltdb_message_bus_unittest voltdb_message_bus_unittest)
endif()

if (ENABLE_ZOOKEEPER)
  add_executable(zookeeper_message_bus_unittest
                 tests/zookeeper_message_bus_unittest.cc)
  target_link_libraries(zookeeper_message_bus_unittest
                        gtest
                        gtest_main
                        tmb)
  add_test(zookeeper_message_bus_unittest zookeeper_message_bus_unittest)
endif()
