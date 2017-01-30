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

set_gflags_lib_name ()

include(CheckCXXSourceCompiles)

# Look for GCC-style builtins (also in clang, ICC, and possibly others) that
# translate to fast assembly instructions for fancy bitwise operations.
CHECK_CXX_SOURCE_COMPILES("
  int main() {
    return __builtin_popcount(1234);
  }
  " QUICKSTEP_HAVE_BUILTIN_POPCOUNT)

CHECK_CXX_SOURCE_COMPILES("
  int main() {
    return __builtin_clz(12340);
  }
  " QUICKSTEP_HAVE_BUILTIN_CLZ)

CHECK_CXX_SOURCE_COMPILES("
  int main() {
    return __builtin_ctz(12340);
  }
  " QUICKSTEP_HAVE_BUILTIN_CTZ)

# Look for c++11 std::align (still not available in G++ 4.8.3).
CHECK_CXX_SOURCE_COMPILES("
  #include <cstddef>
  #include <memory>

  int main() {
    void *ptr = nullptr;
    std::size_t space = 16;

    void *aligned = std::align(8, 8, ptr, space);
    return 0;
  }
  " QUICKSTEP_HAVE_STD_ALIGN)

# Try to find some version of malloc that we can use to do aligned allocations.
set (ALIGNED_MALLOC_FOUND FALSE)
if ((NOT ALIGNED_MALLOC_FOUND) AND USE_TCMALLOC)
  # TCMalloc provides its own version of posix_memalign we can use directly.
  set(ALIGNED_MALLOC_FOUND TRUE)
  set(QUICKSTEP_HAVE_TCMALLOC TRUE)
endif()
if (NOT ALIGNED_MALLOC_FOUND)
  # Try C11's aligned_alloc.
  CHECK_CXX_SOURCE_COMPILES("
  #include <stdlib.h>

  int main() {
    void *ptr = aligned_alloc(64, 256);
    return 0;
  }
  " QUICKSTEP_HAVE_ALIGNED_ALLOC)
  if (QUICKSTEP_HAVE_ALIGNED_ALLOC)
    set(ALIGNED_MALLOC_FOUND TRUE)
  endif()
endif()
if (NOT ALIGNED_MALLOC_FOUND)
  # Try posix_memalign
  CHECK_CXX_SOURCE_COMPILES("
  #include <stdlib.h>

  int main() {
    void *ptr;
    return posix_memalign(&ptr, 64, 256);
  }
  " QUICKSTEP_HAVE_POSIX_MEMALIGN)
  if (QUICKSTEP_HAVE_POSIX_MEMALIGN)
    set(ALIGNED_MALLOC_FOUND TRUE)
  endif()
endif()
if (NOT ALIGNED_MALLOC_FOUND)
  # Try memalign (now deprecated in favor of posix_memalign)
  CHECK_CXX_SOURCE_COMPILES("
  #include <malloc.h>

  int main() {
    void *ptr = memalign(64, 256);
    return 0;
  }
  " QUICKSTEP_HAVE_MEMALIGN)
  if (QUICKSTEP_HAVE_MEMALIGN)
    set(ALIGNED_MALLOC_FOUND TRUE)
  endif()
endif()
if (NOT ALIGNED_MALLOC_FOUND)
  # Try windows _aligned_malloc
  CHECK_CXX_SOURCE_COMPILES("
  #include <malloc.h>

  int main() {
    void *ptr = _aligned_malloc(256, 64);
    return 0;
  }
  " QUICKSTEP_HAVE_WIN_ALIGNED_MALLOC)
  if (QUICKSTEP_HAVE_WIN_ALIGNED_MALLOC)
    set(ALIGNED_MALLOC_FOUND TRUE)
  endif()
endif()

if (NOT ALIGNED_MALLOC_FOUND)
  message(FATAL_ERROR "Can't find any version of malloc capable of doing "
                      "aligned memory allocation.")
endif()

# Check for sysconf on Linux
include(CheckIncludeFileCXX)
include(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("
  #include <unistd.h>
  int main() {
  return (sysconf(_SC_PAGE_SIZE) > 0 && sysconf(_SC_PHYS_PAGES) > 0);
  }
" QUICKSTEP_HAVE_SYSCONF)
if (NOT QUICKSTEP_HAVE_SYSCONF)
  check_include_files("windows.h" QUICKSTEP_HAVE_WINDOWS)
endif()

include(CheckIncludeFileCXX)
check_include_files("glob.h" QUICKSTEP_HAVE_GLOB)
if (NOT QUICKSTEP_HAVE_GLOB)
  check_include_files("windows.h" QUICKSTEP_HAVE_WINDOWS)
endif()

# Check for POSIX.1-2008 memstreams.
include(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(open_memstream QUICKSTEP_HAVE_OPEN_MEMSTREAM)

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/UtilityConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/UtilityConfig.h"
)

QS_PROTOBUF_GENERATE_CPP(quickstep_utility_BloomFilter_proto_srcs
                         quickstep_utility_BloomFilter_proto_hdrs
                         BloomFilter.proto)

QS_PROTOBUF_GENERATE_CPP(quickstep_utility_SortConfiguration_proto_srcs
                         quickstep_utility_SortConfiguration_proto_hdrs
                         SortConfiguration.proto)

add_subdirectory(lip_filter)

# Declare micro-libs:
add_library(quickstep_utility_Alignment ../empty_src.cpp Alignment.hpp)
add_library(quickstep_utility_BitManipulation ../empty_src.cpp BitManipulation.hpp)
add_library(quickstep_utility_BitVector ../empty_src.cpp BitVector.hpp)
add_library(quickstep_utility_BloomFilter ../empty_src.cpp BloomFilter.hpp)
add_library(quickstep_utility_BloomFilter_proto
            ${quickstep_utility_BloomFilter_proto_srcs}
            ${quickstep_utility_BloomFilter_proto_hdrs})
add_library(quickstep_utility_CalculateInstalledMemory CalculateInstalledMemory.cpp CalculateInstalledMemory.hpp)
add_library(quickstep_utility_Cast ../empty_src.cpp Cast.hpp)
add_library(quickstep_utility_CheckSnprintf ../empty_src.cpp CheckSnprintf.hpp)
add_library(quickstep_utility_CompositeHash ../empty_src.cpp CompositeHash.hpp)
add_library(quickstep_utility_BarrieredReadWriteConcurrentBitVector
            ../empty_src.cpp
            BarrieredReadWriteConcurrentBitVector.hpp)
add_library(quickstep_utility_DAG ../empty_src.cpp DAG.hpp)
add_library(quickstep_utility_DisjointTreeForest ../empty_src.cpp DisjointTreeForest.hpp)
add_library(quickstep_utility_EqualsAnyConstant ../empty_src.cpp EqualsAnyConstant.hpp)
add_library(quickstep_utility_ExecutionDAGVisualizer
            ExecutionDAGVisualizer.cpp
            ExecutionDAGVisualizer.hpp)
add_library(quickstep_utility_Glob Glob.cpp Glob.hpp)
add_library(quickstep_utility_HashPair ../empty_src.cpp HashPair.hpp)
add_library(quickstep_utility_Macros ../empty_src.cpp Macros.hpp)
add_library(quickstep_utility_MemStream ../empty_src.cpp MemStream.hpp)
add_library(quickstep_utility_NetworkUtil NetworkUtil.cpp NetworkUtil.hpp)
add_library(quickstep_utility_PlanVisualizer PlanVisualizer.cpp PlanVisualizer.hpp)
add_library(quickstep_utility_PrimeNumber PrimeNumber.cpp PrimeNumber.hpp)
add_library(quickstep_utility_PtrList ../empty_src.cpp PtrList.hpp)
add_library(quickstep_utility_PtrMap ../empty_src.cpp PtrMap.hpp)
add_library(quickstep_utility_PtrVector ../empty_src.cpp PtrVector.hpp)
add_library(quickstep_utility_ScopedBuffer ../empty_src.cpp ScopedBuffer.hpp)
add_library(quickstep_utility_ScopedDeleter ../empty_src.cpp ScopedDeleter.hpp)
add_library(quickstep_utility_ShardedLockManager ../empty_src.cpp ShardedLockManager.hpp)
add_library(quickstep_utility_SortConfiguration SortConfiguration.cpp SortConfiguration.hpp)
add_library(quickstep_utility_SortConfiguration_proto
            ${quickstep_utility_SortConfiguration_proto_srcs}
            ${quickstep_utility_SortConfiguration_proto_hdrs})
add_library(quickstep_utility_SqlError SqlError.cpp SqlError.hpp)
add_library(quickstep_utility_StringUtil StringUtil.cpp StringUtil.hpp)
add_library(quickstep_utility_TemplateUtil ../empty_src.cpp TemplateUtil.hpp)
# Note that TextBasedTest.{hpp, cpp} are not in this static library.
# Any tests that use them need to include them in the
# executable.
add_library(quickstep_utility_TextBasedTestDriver
            textbased_test/TextBasedTestDriver.cpp
            textbased_test/TextBasedTestDriver.hpp
            textbased_test/TextBasedTestRunner.hpp)
add_library(quickstep_utility_ThreadSafeQueue ../empty_src.cpp ThreadSafeQueue.hpp)
add_library(quickstep_utility_TreeStringSerializable ../empty_src.cpp TreeStringSerializable.hpp)
add_library(quickstep_utility_VectorUtil ../empty_src.cpp VectorUtil.hpp)

# Link dependencies:
target_link_libraries(quickstep_utility_Alignment
                      quickstep_utility_Macros)
if (QUICKSTEP_HAVE_TCMALLOC)
  target_link_libraries(quickstep_utility_Alignment
                        libtcmalloc_minimal)
endif()
target_link_libraries(quickstep_utility_BitVector
                      glog
                      quickstep_utility_BitManipulation
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_BloomFilter
                      glog
                      quickstep_storage_StorageConstants
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_BloomFilter_proto
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_BloomFilter_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_utility_CalculateInstalledMemory
                      glog)
target_link_libraries(quickstep_utility_CheckSnprintf
                      glog)
target_link_libraries(quickstep_utility_CompositeHash
                      quickstep_types_TypedValue
                      quickstep_utility_HashPair
                      glog)
target_link_libraries(quickstep_utility_BarrieredReadWriteConcurrentBitVector
                      quickstep_utility_BitManipulation
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_DAG
                      glog
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_DisjointTreeForest
                      glog)
target_link_libraries(quickstep_utility_ExecutionDAGVisualizer
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_relationaloperators_AggregationOperator
                      quickstep_relationaloperators_BuildHashOperator
                      quickstep_relationaloperators_HashJoinOperator
                      quickstep_relationaloperators_SelectOperator
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_utility_DAG
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_utility_Glob
                      glog)
target_link_libraries(quickstep_utility_MemStream
                      glog
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_NetworkUtil
                      glog
                      ${GFLAGS_LIB_NAME})
target_link_libraries(quickstep_utility_PrimeNumber
                      glog)
target_link_libraries(quickstep_utility_PlanVisualizer
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_physical_FilterJoin
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_LIPFilterConfiguration
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_utility_PtrList
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_PtrMap
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_PtrVector
                      glog
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_ScopedBuffer
                      glog
                      quickstep_utility_Alignment
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_ScopedDeleter
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_SqlError
                      glog)
target_link_libraries(quickstep_utility_SortConfiguration
                      glog
                      quickstep_expressions_ExpressionFactories
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_scalar_Scalar
                      quickstep_utility_PtrVector
                      quickstep_utility_SortConfiguration_proto)
target_link_libraries(quickstep_utility_SortConfiguration_proto
                      quickstep_expressions_Expressions_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_utility_ShardedLockManager
                      quickstep_storage_StorageConstants
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_StringUtil
                      glog)
target_link_libraries(quickstep_utility_TemplateUtil)
target_link_libraries(quickstep_utility_TextBasedTestDriver
                      glog
                      gtest
                      gtest_main
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_ThreadSafeQueue
                      quickstep_threading_ConditionVariable
                      quickstep_threading_Mutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_TreeStringSerializable
                      glog
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_utility ../empty_src.cpp UtilityModule.hpp)
target_link_libraries(quickstep_utility
                      quickstep_utility_Alignment
                      quickstep_utility_BarrieredReadWriteConcurrentBitVector
                      quickstep_utility_BitManipulation
                      quickstep_utility_BitVector
                      quickstep_utility_BloomFilter
                      quickstep_utility_BloomFilter_proto
                      quickstep_utility_CalculateInstalledMemory
                      quickstep_utility_Cast
                      quickstep_utility_CheckSnprintf
                      quickstep_utility_CompositeHash
                      quickstep_utility_DAG
                      quickstep_utility_DisjointTreeForest
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_ExecutionDAGVisualizer
                      quickstep_utility_Glob
                      quickstep_utility_HashPair
                      quickstep_utility_Macros
                      quickstep_utility_MemStream
                      quickstep_utility_NetworkUtil
                      quickstep_utility_PlanVisualizer
                      quickstep_utility_PrimeNumber
                      quickstep_utility_PtrList
                      quickstep_utility_PtrMap
                      quickstep_utility_PtrVector
                      quickstep_utility_ScopedBuffer
                      quickstep_utility_ScopedDeleter
                      quickstep_utility_ShardedLockManager
                      quickstep_utility_SortConfiguration
                      quickstep_utility_SortConfiguration_proto
                      quickstep_utility_SqlError
                      quickstep_utility_StringUtil
                      quickstep_utility_TemplateUtil
                      quickstep_utility_TextBasedTestDriver
                      quickstep_utility_ThreadSafeQueue
                      quickstep_utility_TreeStringSerializable
                      quickstep_utility_VectorUtil)

# Tests:
add_executable(BitVector_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/BitVector_unittest.cpp")
target_link_libraries(BitVector_unittest
                      gtest
                      gtest_main
                      quickstep_utility_BitVector
                      ${LIBS})
add_test(BitVector_unittest BitVector_unittest)

add_executable(BloomFilter_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/BloomFilter_unittest.cpp")
target_link_libraries(BloomFilter_unittest
                      gtest
                      gtest_main
                      quickstep_utility_BloomFilter
                      ${LIBS})
add_test(BloomFilter_unittest BloomFilter_unittest)

add_executable(CalculateInstalledMemory_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/CalculateInstalledMemory_unittest.cpp")
target_link_libraries(CalculateInstalledMemory_unittest
                      gtest
                      gtest_main
                      quickstep_utility_CalculateInstalledMemory)
add_test(CalculateInstalledMemory_unittest CalculateInstalledMemory_unittest)

add_executable(DAG_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/DAG_unittest.cpp")
target_link_libraries(DAG_unittest
                      gtest
                      gtest_main
                      quickstep_utility_DAG
                      ${LIBS})
add_test(DAG_unittest DAG_unittest)

add_executable(DisjointTreeForest_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/DisjointTreeForest_unittest.cpp")
target_link_libraries(DisjointTreeForest_unittest
                      gtest
                      gtest_main
                      quickstep_utility_DisjointTreeForest)
add_test(DisjointTreeForest_unittest DisjointTreeForest_unittest)

add_executable(EqualsAnyConstant_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/EqualsAnyConstant_unittest.cpp")
target_link_libraries(EqualsAnyConstant_unittest
                      gtest
                      gtest_main
                      quickstep_utility_EqualsAnyConstant
                      ${LIBS})
add_test(EqualsAnyConstant_unittest EqualsAnyConstant_unittest)

add_executable(NetworkUtil_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/NetworkUtil_unittest.cpp")
target_link_libraries(NetworkUtil_unittest
                      gtest
                      gtest_main
                      quickstep_utility_NetworkUtil
                      ${GFLAGS_LIB_NAME}
                      ${LIBS})
add_test(NetworkUtil_unittest NetworkUtil_unittest)

add_executable(PrimeNumber_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/PrimeNumber_unittest.cpp")
target_link_libraries(PrimeNumber_unittest
                      gtest
                      gtest_main
                      quickstep_utility_PrimeNumber
                      ${LIBS})
add_test(PrimeNumber_unittest PrimeNumber_unittest)

add_executable(ScopedDeleter_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/ScopedDeleter_unittest.cpp")
target_link_libraries(ScopedDeleter_unittest
                      gtest
                      gtest_main
                      quickstep_utility_ScopedDeleter
                      ${LIBS})
add_test(ScopedDeleter_unittest ScopedDeleter_unittest)

add_executable(SqlError_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/SqlError_unittest.cpp")
target_link_libraries(SqlError_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_utility_Macros
                      quickstep_utility_SqlError)
add_test(SqlError_unittest SqlError_unittest)

add_executable(TextBasedTestDriver_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/TextBasedTestDriver_unittest.cpp")
target_link_libraries(TextBasedTestDriver_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_utility_Macros
                      quickstep_utility_TextBasedTestDriver)
add_test(TextBasedTestDriver_unittest TextBasedTestDriver_unittest)

add_executable(ThreadSafeQueue_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/ThreadSafeQueue_unittest.cpp")
target_link_libraries(ThreadSafeQueue_unittest
                      gtest
                      gtest_main
                      quickstep_utility_ThreadSafeQueue
                      ${LIBS})
add_test(ThreadSafeQueue_unittest ThreadSafeQueue_unittest)

add_executable(TreeStringSerializable_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/TreeStringSerializable_unittest.cpp")
target_link_libraries(TreeStringSerializable_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_utility_TreeStringSerializable)
add_test(TreeStringSerializable_unittest TreeStringSerializable_unittest)

add_executable(TemplateUtil_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/TemplateUtil_unittest.cpp")
target_link_libraries(TemplateUtil_unittest
                      gtest
                      gtest_main
                      quickstep_utility_Macros
                      quickstep_utility_TemplateUtil)
add_test(TemplateUtil_unittest TemplateUtil_unittest)

# Benchmarks:
if (UNIX)
  add_executable(EqualsAnyConstant_benchmark
                 "${CMAKE_CURRENT_SOURCE_DIR}/tests/EqualsAnyConstant_benchmark.cpp")
  target_link_libraries(EqualsAnyConstant_benchmark
                        benchmark
                        quickstep_utility_EqualsAnyConstant
                        ${LIBS})

  add_executable(HashPair_benchmark "${CMAKE_CURRENT_SOURCE_DIR}/tests/HashPair_benchmark.cpp")
  target_link_libraries(HashPair_benchmark
                        benchmark
                        quickstep_utility_HashPair
                        quickstep_types_TypedValue
                        ${LIBS})
endif()
