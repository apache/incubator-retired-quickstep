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

option(REBUILD_INDEX_ON_UPDATE_OVERFLOW "If an IndexSubBlock runs out of space while re-adding entries during an update() query, try rebuilding it." ON)

if (REBUILD_INDEX_ON_UPDATE_OVERFLOW)
  set(QUICKSTEP_REBUILD_INDEX_ON_UPDATE_OVERFLOW TRUE)
endif()

set_gflags_lib_name ()

include(CheckIncludeFileCXX)
check_include_files("fcntl.h;glob.h;unistd.h;sys/stat.h;sys/types.h" QUICKSTEP_HAVE_FILE_MANAGER_POSIX)
if (NOT QUICKSTEP_HAVE_FILE_MANAGER_POSIX)
  check_include_files("windows.h" QUICKSTEP_HAVE_FILE_MANAGER_WINDOWS)
endif()

if (NOT (QUICKSTEP_HAVE_FILE_MANAGER_POSIX OR QUICKSTEP_HAVE_FILE_MANAGER_WINDOWS))
  message(FATAL_ERROR "Can't find POSIX (fcntl.h, glob.h, unistd.h, sys/stat.h, and sys/types.h) or Windows (windows.h). "
                      "Quickstep requires one of these. If you are running a UNIX-like OS or Windows, "
                      "check your include paths.")
endif()

if (ENABLE_HDFS)
  set(QUICKSTEP_HAVE_FILE_MANAGER_HDFS TRUE)
endif()

if(LIBNUMA_FOUND)
  set(QUICKSTEP_HAVE_LIBNUMA TRUE)
endif()

# See if mmap can be used to allocate Linux hugetlb pages.
include(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("
  #ifndef _GNU_SOURCE
  #define _GNU_SOURCE
  #endif
  #include <sys/mman.h>

  int main() {
    void *mem = mmap(nullptr,
                     0x200000,
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB,
                     -1, 0);
    munmap(mem, 0x200000);
    return 0;
  }
  " QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB)

if (QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB)
  message("You appear to be building on a Linux system with HugeTLB support. "
          "To take advantage of this feature, you will need to configure "
          "kernel support for hugepages by setting /proc/sys/vm/nr_hugepages "
          "and/or /proc/sys/vm/nr_overcommit_hugepages as well as running "
          "quickstep executables under the group id specified in "
          "/proc/sys/vm/hugetlb_shm_group (see Linux documentation on this "
          "feature for more details: "
          "https://www.kernel.org/doc/Documentation/vm/hugetlbpage.txt). You "
          "can also safely ignore this, and quickstep will fall back to using "
          "ordinary small pages for buffer pool memory.")
endif()

# Failing that, try FreeBSD superpages (a different name for the same feature).
if (NOT QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB)
  CHECK_CXX_SOURCE_COMPILES("
    #define _BSD_SOURCE
    #include <sys/mman.h>

    int main() {
      void *mem = mmap(nullptr,
                       0x200000,
                       PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED_SUPER,
                       -1, 0);
      munmap(mem, 0x200000);
      return 0;
    }
  " QUICKSTEP_HAVE_MMAP_BSD_SUPERPAGE)
endif()

# Failing that, try mmap without any special flags for big pages (still
# preferable to malloc, because it gives us a page-aligned allocation that is
# zeroed-out "for free").
if (NOT (QUICKSTEP_HAVE_MMAP_LINUX_HUGETLB OR QUICKSTEP_HAVE_MMAP_BSD_SUPERPAGE))
  CHECK_CXX_SOURCE_COMPILES("
    #include <sys/mman.h>

    int main() {
      void *mem = mmap(nullptr,
                       0x200000,
                       PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS,
                       -1, 0);
      munmap(mem, 0x200000);
      return 0;
    }
  " QUICKSTEP_HAVE_MMAP_PLAIN)
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/StorageConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/StorageConfig.h"
)

QS_PROTOBUF_GENERATE_CPP(storage_AggregationOperationState_proto_srcs
                         storage_AggregationOperationState_proto_hdrs
                         AggregationOperationState.proto)
QS_PROTOBUF_GENERATE_CPP(storage_HashTable_proto_srcs
                         storage_HashTable_proto_hdrs
                         HashTable.proto)
QS_PROTOBUF_GENERATE_CPP(storage_InsertDestination_proto_srcs
                         storage_InsertDestination_proto_hdrs
                         InsertDestination.proto)
QS_PROTOBUF_GENERATE_CPP(storage_StorageBlockLayout_proto_srcs
                         storage_StorageBlockLayout_proto_hdrs
                         StorageBlockLayout.proto)
QS_PROTOBUF_GENERATE_CPP(storage_WindowAggregationOperationState_proto_srcs
                         storage_WindowAggregationOperationState_proto_hdrs
                         WindowAggregationOperationState.proto)

if (ENABLE_DISTRIBUTED)
  GRPC_GENERATE_CPP(storage_DataExchange_proto_srcs
                    storage_DataExchange_proto_hdrs
                    .
                    DataExchange.proto)
endif()

# Declare micro-libs:
add_library(quickstep_storage_AggregationOperationState
            AggregationOperationState.cpp
            AggregationOperationState.hpp)
add_library(quickstep_storage_AggregationOperationState_proto ${storage_AggregationOperationState_proto_srcs})
add_library(quickstep_storage_BasicColumnStoreTupleStorageSubBlock
            BasicColumnStoreTupleStorageSubBlock.cpp
            BasicColumnStoreTupleStorageSubBlock.hpp)
add_library(quickstep_storage_BasicColumnStoreValueAccessor
            ../empty_src.cpp
            BasicColumnStoreValueAccessor.hpp)
add_library(quickstep_storage_BloomFilterIndexSubBlock BloomFilterIndexSubBlock.cpp BloomFilterIndexSubBlock.hpp)
# CMAKE_VALIDATE_IGNORE_BEGIN
if(QUICKSTEP_HAVE_BITWEAVING)
  add_library(quickstep_storage_BitWeavingIndexSubBlock
              bitweaving/BitWeavingIndexSubBlock.cpp
              bitweaving/BitWeavingIndexSubBlock.hpp)
  add_library(quickstep_storage_BitWeavingHIndexSubBlock
              bitweaving/BitWeavingHIndexSubBlock.cpp
              bitweaving/BitWeavingHIndexSubBlock.hpp)
  add_library(quickstep_storage_BitWeavingVIndexSubBlock
              bitweaving/BitWeavingVIndexSubBlock.cpp
              bitweaving/BitWeavingVIndexSubBlock.hpp)
endif()
# CMAKE_VALIDATE_IGNORE_END
add_library(quickstep_storage_CollisionFreeVectorTable
            CollisionFreeVectorTable.cpp
            CollisionFreeVectorTable.hpp)
add_library(quickstep_storage_ColumnStoreUtil ColumnStoreUtil.cpp ColumnStoreUtil.hpp)
add_library(quickstep_storage_CompressedBlockBuilder CompressedBlockBuilder.cpp CompressedBlockBuilder.hpp)
add_library(quickstep_storage_CompressedColumnStoreTupleStorageSubBlock
            CompressedColumnStoreTupleStorageSubBlock.cpp
            CompressedColumnStoreTupleStorageSubBlock.hpp)
add_library(quickstep_storage_CompressedColumnStoreValueAccessor
            ../empty_src.cpp
            CompressedColumnStoreValueAccessor.hpp)
add_library(quickstep_storage_CompressedPackedRowStoreTupleStorageSubBlock
            CompressedPackedRowStoreTupleStorageSubBlock.cpp
            CompressedPackedRowStoreTupleStorageSubBlock.hpp)
add_library(quickstep_storage_CompressedPackedRowStoreValueAccessor
            ../empty_src.cpp
            CompressedPackedRowStoreValueAccessor.hpp)
add_library(quickstep_storage_CompressedStoreUtil CompressedStoreUtil.cpp CompressedStoreUtil.hpp)
add_library(quickstep_storage_CompressedTupleStorageSubBlock
            CompressedTupleStorageSubBlock.cpp
            CompressedTupleStorageSubBlock.hpp)
add_library(quickstep_storage_CountedReference ../empty_src.cpp CountedReference.hpp)
add_library(quickstep_storage_CSBTreeIndexSubBlock CSBTreeIndexSubBlock.cpp CSBTreeIndexSubBlock.hpp)

if (ENABLE_DISTRIBUTED)
  add_library(quickstep_storage_DataExchange_proto
              ${storage_DataExchange_proto_srcs}
              ${storage_DataExchange_proto_hdrs})
  add_library(quickstep_storage_DataExchangerAsync DataExchangerAsync.cpp DataExchangerAsync.hpp)
endif()

add_library(quickstep_storage_EvictionPolicy EvictionPolicy.cpp EvictionPolicy.hpp)
add_library(quickstep_storage_FileManager ../empty_src.cpp FileManager.hpp)
if (QUICKSTEP_HAVE_FILE_MANAGER_HDFS)
  add_library(quickstep_storage_FileManagerHdfs FileManagerHdfs.cpp FileManagerHdfs.hpp)
endif()
add_library(quickstep_storage_FileManagerLocal ../empty_src.cpp FileManagerLocal.hpp)
if (QUICKSTEP_HAVE_FILE_MANAGER_POSIX)
  add_library(quickstep_storage_FileManagerPosix FileManagerPosix.cpp FileManagerPosix.hpp)
elseif (QUICKSTEP_HAVE_FILE_MANAGER_WINDOWS)
  add_library(quickstep_storage_FileManagerWindows FileManagerWindows.cpp FileManagerWindows.hpp)
endif()
add_library(quickstep_storage_Flags Flags.cpp Flags.hpp)
add_library(quickstep_storage_HashTable ../empty_src.cpp HashTable.hpp)
add_library(quickstep_storage_HashTable_proto ${storage_HashTable_proto_srcs})
add_library(quickstep_storage_HashTableBase ../empty_src.cpp HashTableBase.hpp)
add_library(quickstep_storage_HashTableFactory HashTableFactory.cpp HashTableFactory.hpp)
add_library(quickstep_storage_HashTableKeyManager ../empty_src.cpp HashTableKeyManager.hpp)
add_library(quickstep_storage_HashTablePool ../empty_src.cpp HashTablePool.hpp)
add_library(quickstep_storage_IndexSubBlock ../empty_src.cpp IndexSubBlock.hpp)
add_library(quickstep_storage_IndexSubBlockDescriptionFactory ../empty_src.cpp IndexSubBlockDescriptionFactory.hpp)
add_library(quickstep_storage_InsertDestination InsertDestination.cpp InsertDestination.hpp)
add_library(quickstep_storage_InsertDestinationInterface
            ../empty_src.cpp
            InsertDestinationInterface.hpp)
add_library(quickstep_storage_InsertDestination_proto
            ${storage_InsertDestination_proto_srcs}
            ${storage_InsertDestination_proto_hdrs})
add_library(quickstep_storage_LinearOpenAddressingHashTable
            ../empty_src.cpp
            LinearOpenAddressingHashTable.hpp)
add_library(quickstep_storage_PackedPayloadHashTable PackedPayloadHashTable.cpp PackedPayloadHashTable.hpp)
add_library(quickstep_storage_PartitionedHashTablePool ../empty_src.cpp PartitionedHashTablePool.hpp)
add_library(quickstep_storage_PreloaderThread PreloaderThread.cpp PreloaderThread.hpp)
add_library(quickstep_storage_SMAIndexSubBlock SMAIndexSubBlock.cpp SMAIndexSubBlock.hpp)
add_library(quickstep_storage_SeparateChainingHashTable ../empty_src.cpp SeparateChainingHashTable.hpp)
add_library(quickstep_storage_SimpleScalarSeparateChainingHashTable
            SimpleScalarSeparateChainingHashTable.cpp
            SimpleScalarSeparateChainingHashTable.hpp)
add_library(quickstep_storage_SplitRowStoreTupleStorageSubBlock
            SplitRowStoreTupleStorageSubBlock.cpp
            SplitRowStoreTupleStorageSubBlock.hpp)
add_library(quickstep_storage_SplitRowStoreValueAccessor ../empty_src.cpp SplitRowStoreValueAccessor.hpp)
add_library(quickstep_storage_StorageBlob ../empty_src.cpp StorageBlob.hpp)
add_library(quickstep_storage_StorageBlock StorageBlock.cpp StorageBlock.hpp)
add_library(quickstep_storage_StorageBlockBase ../empty_src.cpp StorageBlockBase.hpp)
add_library(quickstep_storage_StorageBlockInfo StorageBlockInfo.cpp StorageBlockInfo.hpp)
add_library(quickstep_storage_StorageBlockLayout StorageBlockLayout.cpp StorageBlockLayout.hpp)
add_library(quickstep_storage_StorageBlockLayout_proto ${storage_StorageBlockLayout_proto_srcs})
add_library(quickstep_storage_StorageConstants ../empty_src.cpp StorageConstants.hpp)
add_library(quickstep_storage_StorageErrors StorageErrors.cpp StorageErrors.hpp)
add_library(quickstep_storage_StorageManager StorageManager.cpp StorageManager.hpp)
add_library(quickstep_storage_SubBlockTypeRegistry SubBlockTypeRegistry.cpp SubBlockTypeRegistry.hpp)
add_library(quickstep_storage_SubBlockTypeRegistryMacros ../empty_src.cpp SubBlockTypeRegistryMacros.hpp)
add_library(quickstep_storage_SubBlocksReference ../empty_src.cpp SubBlocksReference.hpp)
add_library(quickstep_storage_ThreadPrivateCompactKeyHashTable
            ThreadPrivateCompactKeyHashTable.cpp
            ThreadPrivateCompactKeyHashTable.hpp)
add_library(quickstep_storage_TupleIdSequence ../empty_src.cpp TupleIdSequence.hpp)
add_library(quickstep_storage_TupleReference ../empty_src.cpp TupleReference.hpp)
add_library(quickstep_storage_TupleStorageSubBlock TupleStorageSubBlock.cpp TupleStorageSubBlock.hpp)
add_library(quickstep_storage_ValueAccessor ../empty_src.cpp ValueAccessor.hpp)
add_library(quickstep_storage_ValueAccessorMultiplexer ../empty_src.cpp ValueAccessorMultiplexer.hpp)
add_library(quickstep_storage_ValueAccessorUtil ../empty_src.cpp ValueAccessorUtil.hpp)
add_library(quickstep_storage_WindowAggregationOperationState
            WindowAggregationOperationState.hpp
            WindowAggregationOperationState.cpp)
add_library(quickstep_storage_WindowAggregationOperationState_proto ${storage_WindowAggregationOperationState_proto_srcs})


# Link dependencies:
target_link_libraries(quickstep_storage_AggregationOperationState
                      ${GFLAGS_LIB_NAME}
                      glog
                      quickstep_catalog_CatalogDatabaseLite
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_ExpressionFactories
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_aggregation_AggregateFunction
                      quickstep_expressions_aggregation_AggregateFunctionFactory
                      quickstep_expressions_aggregation_AggregationHandle
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_AggregationOperationState_proto
                      quickstep_storage_CollisionFreeVectorTable
                      quickstep_storage_HashTableBase
                      quickstep_storage_HashTableFactory
                      quickstep_storage_HashTablePool
                      quickstep_storage_InsertDestination
                      quickstep_storage_PartitionedHashTablePool
                      quickstep_storage_PackedPayloadHashTable
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_storage_SubBlocksReference
                      quickstep_storage_ThreadPrivateCompactKeyHashTable
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorMultiplexer
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_types_containers_Tuple
                      quickstep_utility_ColumnVectorCache
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilterAdaptiveProber)
target_link_libraries(quickstep_storage_AggregationOperationState_proto
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_aggregation_AggregateFunction_proto
                      quickstep_storage_HashTable_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_storage_BasicColumnStoreTupleStorageSubBlock
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_BasicColumnStoreValueAccessor
                      quickstep_storage_ColumnStoreUtil
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageErrors
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap
                      quickstep_utility_PtrVector
                      quickstep_utility_ScopedBuffer)
target_link_libraries(quickstep_storage_BasicColumnStoreValueAccessor
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_storage_BloomFilterIndexSubBlock
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_storage_IndexSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_BloomFilter
                      quickstep_utility_Macros)
# CMAKE_VALIDATE_IGNORE_BEGIN
if(QUICKSTEP_HAVE_BITWEAVING)
  target_link_libraries(quickstep_storage_BitWeavingHIndexSubBlock
                        glog
                        quickstep_catalog_CatalogTypedefs
                        quickstep_compression_CompressionDictionary
                        quickstep_compression_CompressionDictionaryBuilder
                        quickstep_expressions_predicate_ComparisonPredicate
                        quickstep_expressions_predicate_PredicateCost
                        quickstep_storage_BitWeavingIndexSubBlock
                        quickstep_storage_CompressedTupleStorageSubBlock
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageBlockLayout_proto
                        quickstep_storage_SubBlockTypeRegistry
                        quickstep_storage_SubBlockTypeRegistryMacros
                        quickstep_storage_TupleStorageSubBlock
                        quickstep_types_TypedValue
                        quickstep_types_operations_comparisons_ComparisonID
                        quickstep_utility_Macros)
  target_link_libraries(quickstep_storage_BitWeavingIndexSubBlock
                        glog
                        quickstep_catalog_CatalogAttribute
                        quickstep_catalog_CatalogRelationSchema
                        quickstep_catalog_CatalogTypedefs
                        quickstep_compression_CompressionDictionary
                        quickstep_compression_CompressionDictionaryBuilder
                        quickstep_expressions_predicate_ComparisonPredicate
                        quickstep_expressions_predicate_Predicate
                        quickstep_expressions_scalar_Scalar
                        quickstep_expressions_scalar_ScalarAttribute
                        quickstep_storage_CompressedStoreUtil
                        quickstep_storage_CompressedTupleStorageSubBlock
                        quickstep_storage_IndexSubBlock
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageBlockLayout_proto
                        quickstep_storage_StorageConstants
                        quickstep_storage_StorageErrors
                        quickstep_storage_TupleIdSequence
                        quickstep_storage_TupleStorageSubBlock
                        quickstep_storage_ValueAccessor
                        quickstep_storage_ValueAccessorUtil
                        quickstep_types_Type
                        quickstep_types_TypedValue
                        quickstep_types_operations_comparisons_ComparisonID
                        quickstep_utility_BitManipulation
                        quickstep_utility_Macros)
  target_link_libraries(quickstep_storage_BitWeavingVIndexSubBlock
                        glog
                        quickstep_catalog_CatalogTypedefs
                        quickstep_compression_CompressionDictionary
                        quickstep_compression_CompressionDictionaryBuilder
                        quickstep_expressions_predicate_ComparisonPredicate
                        quickstep_expressions_predicate_PredicateCost
                        quickstep_storage_BitWeavingIndexSubBlock
                        quickstep_storage_CompressedTupleStorageSubBlock
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageBlockLayout_proto
                        quickstep_storage_StorageConstants
                        quickstep_storage_SubBlockTypeRegistry
                        quickstep_storage_SubBlockTypeRegistryMacros
                        quickstep_storage_TupleStorageSubBlock
                        quickstep_storage_ValueAccessor
                        quickstep_storage_ValueAccessorUtil
                        quickstep_types_TypedValue
                        quickstep_types_operations_comparisons_ComparisonID
                        quickstep_utility_Macros)
endif()
# CMAKE_VALIDATE_IGNORE_END
target_link_libraries(quickstep_storage_CollisionFreeVectorTable
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_aggregation_AggregationHandle
                      quickstep_expressions_aggregation_AggregationID
                      quickstep_storage_HashTableBase
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorMultiplexer
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_utility_BarrieredReadWriteConcurrentBitVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_ColumnStoreUtil
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_CompressedBlockBuilder
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionaryBuilder
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_BitManipulation
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_storage_CompressedColumnStoreTupleStorageSubBlock
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionaryLite
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_storage_ColumnStoreUtil
                      quickstep_storage_CompressedColumnStoreValueAccessor
                      quickstep_storage_CompressedTupleStorageSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_TupleIdSequence
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_storage_CompressedColumnStoreValueAccessor
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionaryLite
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap)
target_link_libraries(quickstep_storage_CompressedPackedRowStoreTupleStorageSubBlock
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionaryLite
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_storage_CompressedPackedRowStoreValueAccessor
                      quickstep_storage_CompressedTupleStorageSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_TupleIdSequence
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_BitVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_CompressedPackedRowStoreValueAccessor
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionaryLite
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap)
target_link_libraries(quickstep_storage_CompressedStoreUtil
                      glog
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionary
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_CompressedTupleStorageSubBlock
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionary
                      quickstep_compression_CompressionDictionaryLite
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_storage_CompressedBlockBuilder
                      quickstep_storage_CompressedStoreUtil
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageErrors
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap)
target_link_libraries(quickstep_storage_CountedReference
                      quickstep_storage_EvictionPolicy
                      quickstep_storage_StorageBlockBase
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_CSBTreeIndexSubBlock
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_compression_CompressionDictionary
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_storage_CompressedTupleStorageSubBlock
                      quickstep_storage_IndexSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector
                      quickstep_utility_ScopedBuffer)

if (ENABLE_DISTRIBUTED)
  target_link_libraries(quickstep_storage_DataExchange_proto
                        ${PROTOBUF3_LIBRARY})
  target_link_libraries(quickstep_storage_DataExchangerAsync
                        glog
                        quickstep_storage_DataExchange_proto
                        quickstep_storage_StorageManager
                        quickstep_threading_Thread
                        quickstep_utility_Macros
                        quickstep_utility_NetworkUtil
                        ${GRPCPLUSPLUS_LIBRARIES})
endif()

target_link_libraries(quickstep_storage_EvictionPolicy
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_threading_Mutex
                      quickstep_threading_SpinMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_FileManager
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
if (QUICKSTEP_HAVE_FILE_MANAGER_HDFS)
  target_link_libraries(quickstep_storage_FileManagerHdfs
                        glog
                        quickstep_storage_FileManager
                        quickstep_storage_Flags
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageConstants
                        quickstep_storage_StorageErrors
                        quickstep_utility_Macros
                        quickstep_utility_StringUtil
                        ${LIBHDFS3_LIBRARIES})
endif()
if (QUICKSTEP_HAVE_FILE_MANAGER_POSIX)
  target_link_libraries(quickstep_storage_FileManagerLocal
                        quickstep_storage_FileManagerPosix)
  target_link_libraries(quickstep_storage_FileManagerPosix
                        glog
                        quickstep_storage_FileManager
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageConstants
                        quickstep_storage_StorageErrors
                        quickstep_utility_Macros
                        quickstep_utility_StringUtil)
elseif (QUICKSTEP_HAVE_FILE_MANAGER_WINDOWS)
  target_link_libraries(quickstep_storage_FileManagerLocal
                        quickstep_storage_FileManagerWindows)
  target_link_libraries(quickstep_storage_FileManagerWindows
                        glog
                        quickstep_storage_FileManager
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageConstants
                        quickstep_storage_StorageErrors
                        quickstep_utility_Macros
                        quickstep_utility_StringUtil)
endif()
target_link_libraries(quickstep_storage_Flags
                      ${GFLAGS_LIB_NAME})
target_link_libraries(quickstep_storage_HashTable
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_HashTableBase
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_storage_TupleReference
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_threading_SpinSharedMutex
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_BloomFilter
                      quickstep_utility_HashPair
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_HashTableBase
                      quickstep_storage_ValueAccessorMultiplexer
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_HashTable_proto
                      quickstep_types_Type_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_storage_HashTableFactory
                      glog
                      quickstep_storage_CollisionFreeVectorTable
                      quickstep_storage_HashTable
                      quickstep_storage_HashTable_proto
                      quickstep_storage_HashTableBase
                      quickstep_storage_LinearOpenAddressingHashTable
                      quickstep_storage_PackedPayloadHashTable
                      quickstep_storage_SeparateChainingHashTable
                      quickstep_storage_SimpleScalarSeparateChainingHashTable
                      quickstep_storage_ThreadPrivateCompactKeyHashTable
                      quickstep_storage_TupleReference
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypedValue
                      quickstep_utility_BloomFilter
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_HashTableKeyManager
                      glog
                      quickstep_storage_HashTableBase
                      quickstep_storage_StorageConstants
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_HashTablePool
                      glog
                      quickstep_storage_HashTableBase
                      quickstep_storage_HashTableFactory
                      quickstep_threading_SpinMutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_IndexSubBlock
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_IndexSubBlockDescriptionFactory
                      quickstep_storage_StorageBlockLayout
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_InsertDestination
                      glog
                      gtest
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_catalog_PartitionSchemeHeader
                      quickstep_queryexecution_QueryExecutionMessages_proto
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_QueryExecutionUtil
                      quickstep_storage_InsertDestinationInterface
                      quickstep_storage_InsertDestination_proto
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageManager
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_threading_SpinMutex
                      quickstep_threading_ThreadIDBasedMap
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_storage_InsertDestinationInterface
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_PartitionSchemeHeader
                      quickstep_types_containers_Tuple)
target_link_libraries(quickstep_storage_InsertDestination_proto
                      quickstep_catalog_Catalog_proto
                      quickstep_storage_StorageBlockLayout_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_storage_LinearOpenAddressingHashTable
                      quickstep_storage_HashTable
                      quickstep_storage_HashTableKeyManager
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_threading_SpinSharedMutex
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_Alignment
                      quickstep_utility_Macros
                      quickstep_utility_PrimeNumber)
target_link_libraries(quickstep_storage_PackedPayloadHashTable
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_aggregation_AggregationHandle
                      quickstep_storage_HashTableBase
                      quickstep_storage_HashTableKeyManager
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorMultiplexer
                      quickstep_storage_ValueAccessorUtil
                      quickstep_threading_SpinMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_utility_Alignment
                      quickstep_utility_HashPair
                      quickstep_utility_Macros
                      quickstep_utility_PrimeNumber
                      quickstep_utility_TemplateUtil)
target_link_libraries(quickstep_storage_PartitionedHashTablePool
                      glog
                      quickstep_storage_HashTableBase
                      quickstep_storage_HashTableFactory
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_PreloaderThread
                      glog
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogDatabaseLite
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_threading_Thread
                      quickstep_threading_ThreadUtil
                      quickstep_utility_Macros)
if (QUICKSTEP_HAVE_LIBNUMA)
  target_link_libraries(quickstep_storage_PreloaderThread
                        quickstep_catalog_NUMAPlacementScheme)
endif()
target_link_libraries(quickstep_storage_SMAIndexSubBlock
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_storage_IndexSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_storage_SeparateChainingHashTable
                      quickstep_storage_HashTable
                      quickstep_storage_HashTableBase
                      quickstep_storage_HashTableKeyManager
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_threading_SpinSharedMutex
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_Alignment
                      quickstep_utility_Macros
                      quickstep_utility_PrimeNumber)
target_link_libraries(quickstep_storage_SimpleScalarSeparateChainingHashTable
                      glog
                      quickstep_storage_HashTable
                      quickstep_storage_HashTableBase
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_threading_SpinSharedMutex
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_Alignment
                      quickstep_utility_Macros
                      quickstep_utility_PrimeNumber)
target_link_libraries(quickstep_storage_SplitRowStoreTupleStorageSubBlock
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_storage_SplitRowStoreValueAccessor
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageErrors
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_ScopedBuffer)
target_link_libraries(quickstep_storage_SplitRowStoreValueAccessor
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_utility_BitVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_StorageBlob
                      quickstep_storage_CountedReference
                      quickstep_storage_StorageBlockBase
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_StorageBlock
                      glog
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_BasicColumnStoreTupleStorageSubBlock
                      quickstep_storage_BloomFilterIndexSubBlock
                      quickstep_storage_CSBTreeIndexSubBlock
                      quickstep_storage_CompressedColumnStoreTupleStorageSubBlock
                      quickstep_storage_CompressedPackedRowStoreTupleStorageSubBlock
                      quickstep_storage_CountedReference
                      quickstep_storage_IndexSubBlock
                      quickstep_storage_InsertDestinationInterface
                      quickstep_storage_SMAIndexSubBlock
                      quickstep_storage_SplitRowStoreTupleStorageSubBlock
                      quickstep_storage_StorageBlockBase
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageErrors
                      quickstep_storage_SubBlocksReference
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_ColumnVectorCache
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
# CMAKE_VALIDATE_IGNORE_BEGIN
if(QUICKSTEP_HAVE_BITWEAVING)
  target_link_libraries(quickstep_storage_StorageBlock
                        quickstep_storage_BitWeavingHIndexSubBlock
                        quickstep_storage_BitWeavingIndexSubBlock
                        quickstep_storage_BitWeavingVIndexSubBlock)
endif()
# CMAKE_VALIDATE_IGNORE_END
target_link_libraries(quickstep_storage_StorageBlockBase
                      glog
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_StorageBlockInfo
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_storage_StorageBlockLayout
                      glog
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_utility_Macros
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_storage_StorageBlockLayout_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_storage_StorageManager
                      ${GFLAGS_LIB_NAME}
                      glog
                      gtest
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_CountedReference
                      quickstep_storage_EvictionPolicy
                      quickstep_storage_FileManager
                      quickstep_storage_FileManagerLocal
                      quickstep_storage_Flags
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockBase
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Alignment
                      quickstep_utility_CalculateInstalledMemory
                      quickstep_utility_Macros
                      quickstep_utility_ShardedLockManager
                      tmb)
if (QUICKSTEP_HAVE_FILE_MANAGER_HDFS)
target_link_libraries(quickstep_storage_StorageManager
                      quickstep_storage_FileManagerHdfs)
endif()
if (QUICKSTEP_HAVE_LIBNUMA)
  target_link_libraries(quickstep_storage_StorageManager
                        ${LIBNUMA_LIBRARY})
endif()
if (ENABLE_DISTRIBUTED)
  target_link_libraries(quickstep_storage_StorageManager
                        quickstep_queryexecution_QueryExecutionMessages_proto
                        quickstep_queryexecution_QueryExecutionTypedefs
                        quickstep_queryexecution_QueryExecutionUtil
                        quickstep_storage_DataExchange_proto
                        ${GRPCPLUSPLUS_LIBRARIES})
endif(ENABLE_DISTRIBUTED)
target_link_libraries(quickstep_storage_SubBlockTypeRegistry
                      glog
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_SubBlocksReference
                      glog
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_storage_ThreadPrivateCompactKeyHashTable
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_aggregation_AggregationHandle
                      quickstep_expressions_aggregation_AggregationID
                      quickstep_storage_HashTableBase
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_storage_ValueAccessorMultiplexer
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_utility_Macros
                      quickstep_utility_ScopedBuffer)
target_link_libraries(quickstep_storage_TupleIdSequence
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_BitVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_TupleReference
                      quickstep_storage_StorageBlockInfo)
target_link_libraries(quickstep_storage_TupleStorageSubBlock
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_ValueAccessor
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_ValueAccessorMultiplexer
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_ValueAccessorUtil
                      glog
                      quickstep_storage_BasicColumnStoreValueAccessor
                      quickstep_storage_CompressedColumnStoreValueAccessor
                      quickstep_storage_CompressedPackedRowStoreValueAccessor
                      quickstep_storage_SplitRowStoreValueAccessor
                      quickstep_storage_ValueAccessor
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_WindowAggregationOperationState
                      glog
                      quickstep_catalog_CatalogDatabaseLite
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_ExpressionFactories
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_windowaggregation_WindowAggregateFunction
                      quickstep_expressions_windowaggregation_WindowAggregateFunctionFactory
                      quickstep_expressions_windowaggregation_WindowAggregationHandle
                      quickstep_expressions_windowaggregation_WindowAggregationID
                      quickstep_storage_InsertDestination
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_storage_SubBlocksReference
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_storage_WindowAggregationOperationState_proto
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorUtil
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_utility_ColumnVectorCache
                      quickstep_utility_Macros)
target_link_libraries(quickstep_storage_WindowAggregationOperationState_proto
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_windowaggregation_WindowAggregateFunction_proto
                      ${PROTOBUF_LIBRARY})

# Module all-in-one library:
add_library(quickstep_storage ../empty_src.cpp StorageModule.hpp)
target_link_libraries(quickstep_storage
                      quickstep_storage_AggregationOperationState
                      quickstep_storage_AggregationOperationState_proto
                      quickstep_storage_BasicColumnStoreTupleStorageSubBlock
                      quickstep_storage_BasicColumnStoreValueAccessor
                      quickstep_storage_BloomFilterIndexSubBlock
                      quickstep_storage_CSBTreeIndexSubBlock
                      quickstep_storage_CollisionFreeVectorTable
                      quickstep_storage_ColumnStoreUtil
                      quickstep_storage_CompressedBlockBuilder
                      quickstep_storage_CompressedColumnStoreTupleStorageSubBlock
                      quickstep_storage_CompressedColumnStoreValueAccessor
                      quickstep_storage_CompressedPackedRowStoreTupleStorageSubBlock
                      quickstep_storage_CompressedPackedRowStoreValueAccessor
                      quickstep_storage_CompressedStoreUtil
                      quickstep_storage_CompressedTupleStorageSubBlock
                      quickstep_storage_CountedReference
                      quickstep_storage_EvictionPolicy
                      quickstep_storage_FileManager
                      quickstep_storage_FileManagerLocal
                      quickstep_storage_Flags
                      quickstep_storage_HashTable
                      quickstep_storage_HashTableBase
                      quickstep_storage_HashTableFactory
                      quickstep_storage_HashTableKeyManager
                      quickstep_storage_HashTablePool
                      quickstep_storage_HashTable_proto
                      quickstep_storage_IndexSubBlock
                      quickstep_storage_IndexSubBlockDescriptionFactory
                      quickstep_storage_InsertDestination
                      quickstep_storage_InsertDestinationInterface
                      quickstep_storage_InsertDestination_proto
                      quickstep_storage_LinearOpenAddressingHashTable
                      quickstep_storage_PartitionedHashTablePool
                      quickstep_storage_PackedPayloadHashTable
                      quickstep_storage_PreloaderThread
                      quickstep_storage_SMAIndexSubBlock
                      quickstep_storage_SeparateChainingHashTable
                      quickstep_storage_SimpleScalarSeparateChainingHashTable
                      quickstep_storage_SplitRowStoreTupleStorageSubBlock
                      quickstep_storage_SplitRowStoreValueAccessor
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockBase
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_storage_StorageManager
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_storage_SubBlocksReference
                      quickstep_storage_ThreadPrivateCompactKeyHashTable
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleReference
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorMultiplexer
                      quickstep_storage_ValueAccessorUtil
                      quickstep_storage_WindowAggregationOperationState
                      quickstep_storage_WindowAggregationOperationState_proto)
if (QUICKSTEP_HAVE_FILE_MANAGER_HDFS)
  target_link_libraries(quickstep_storage
                        quickstep_storage_FileManagerHdfs)
endif()
if (QUICKSTEP_HAVE_FILE_MANAGER_POSIX)
  target_link_libraries(quickstep_storage
                        quickstep_storage_FileManagerPosix)
elseif (QUICKSTEP_HAVE_FILE_MANAGER_WINDOWS)
  target_link_libraries(quickstep_storage
                        quickstep_storage_FileManagerWindows)
endif()
if (ENABLE_DISTRIBUTED)
  target_link_libraries(quickstep_storage
                        quickstep_storage_DataExchange_proto
                        quickstep_storage_DataExchangerAsync)
endif()
# CMAKE_VALIDATE_IGNORE_BEGIN
if(QUICKSTEP_HAVE_BITWEAVING)
  target_link_libraries(quickstep_storage
                        quickstep_storage_BitWeavingHIndexSubBlock
                        quickstep_storage_BitWeavingIndexSubBlock
                        quickstep_storage_BitWeavingVIndexSubBlock)
endif()
# CMAKE_VALIDATE_IGNORE_END

# Tests:
include(CheckTypeSize)
CHECK_TYPE_SIZE("size_t" SIZEOF_SIZE_T)
if (SIZEOF_SIZE_T GREATER 7)
  set(QUICKSTEP_LONG_HASH_REVERSIBLE 1)
endif()
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/StorageTestConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/tests/StorageTestConfig.h"
)

add_library(quickstep_storage_tests_FileManager_unittest_common
            ../empty_src.cpp
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/FileManager_unittest_common.hpp")
target_link_libraries(quickstep_storage_tests_FileManager_unittest_common
                      gtest
                      quickstep_storage_FileManager
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants)

add_library(quickstep_storage_tests_HashTable_unittest_common
            ../empty_src.cpp
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/HashTable_unittest_common.hpp")
target_link_libraries(quickstep_storage_tests_HashTable_unittest_common
                      gtest
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_HashTable
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageManager
                      quickstep_threading_Thread
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_utility_Macros
                      quickstep_utility_ScopedBuffer)

add_library(quickstep_storage_tests_TupleStorePredicateUtil
            ../empty_src.cpp
             "${CMAKE_CURRENT_SOURCE_DIR}/tests/TupleStorePredicateUtil.hpp")
target_link_libraries(quickstep_storage_tests_TupleStorePredicateUtil
                      quickstep_expressions_predicate_Predicate
                      quickstep_storage_IndexSubBlock
                      quickstep_storage_SubBlocksReference
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)

add_executable(AggregationOperationState_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/AggregationOperationState_unittest.cpp")
target_link_libraries(AggregationOperationState_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_AggregationOperationState
                      quickstep_storage_AggregationOperationState_proto
                      ${LIBS})
add_test(AggregationOperationState_unittest AggregationOperationState_unittest)

add_executable(BasicColumnStoreTupleStorageSubBlock_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/BasicColumnStoreTupleStorageSubBlock_unittest.cpp")
target_link_libraries(BasicColumnStoreTupleStorageSubBlock_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_storage_BasicColumnStoreTupleStorageSubBlock
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_tests_TupleStorePredicateUtil
                      quickstep_types_CharType
                      quickstep_types_DoubleType
                      quickstep_types_IntType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_BitVector
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(BasicColumnStoreTupleStorageSubBlock_unittest BasicColumnStoreTupleStorageSubBlock_unittest)

add_executable(BloomFilterIndexSubBlock_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/BloomFilterIndexSubBlock_unittest.cpp")
target_link_libraries(BloomFilterIndexSubBlock_unittest
                      gtest
                      gtest_main
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_storage_BloomFilterIndexSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_CharType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_LongType
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(BloomFilterIndexSubBlock_unittest BloomFilterIndexSubBlock_unittest)

if(QUICKSTEP_HAVE_BITWEAVING)
  add_executable(BitWeavingIndexSubBlock_unittest
                 "${CMAKE_CURRENT_SOURCE_DIR}/bitweaving/tests/BitWeavingIndexSubBlock_unittest.cpp")
  target_link_libraries(BitWeavingIndexSubBlock_unittest
                        glog
                        gtest
                        gtest_main
                        quickstep_catalog_CatalogAttribute
                        quickstep_catalog_CatalogRelation
                        quickstep_catalog_CatalogTypedefs
                        quickstep_expressions_predicate_ComparisonPredicate
                        quickstep_expressions_scalar_Scalar
                        quickstep_expressions_scalar_ScalarAttribute
                        quickstep_expressions_scalar_ScalarLiteral
                        quickstep_storage_BitWeavingHIndexSubBlock
                        quickstep_storage_BitWeavingIndexSubBlock
                        quickstep_storage_BitWeavingVIndexSubBlock
                        quickstep_storage_StorageBlock
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageBlockLayout_proto
                        quickstep_storage_StorageErrors
                        quickstep_storage_StorageManager
                        quickstep_storage_TupleIdSequence
                        quickstep_storage_TupleStorageSubBlock
                        quickstep_types_CharType
                        quickstep_types_FloatType
                        quickstep_types_LongType
                        quickstep_types_Type
                        quickstep_types_TypeFactory
                        quickstep_types_TypeID
                        quickstep_types_TypedValue
                        quickstep_types_VarCharType
                        quickstep_types_containers_Tuple
                        quickstep_types_operations_comparisons_Comparison
                        quickstep_types_operations_comparisons_ComparisonFactory
                        quickstep_types_operations_comparisons_ComparisonID
                        quickstep_types_operations_comparisons_ComparisonUtil
                        quickstep_utility_Macros
                        quickstep_utility_ScopedBuffer
                        ${LIBS})
  add_test(BitWeavingIndexSubBlock_unittest BitWeavingIndexSubBlock_unittest)
endif()

add_executable(CompressedColumnStoreTupleStorageSubBlock_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/CompressedColumnStoreTupleStorageSubBlock_unittest.cpp")
target_link_libraries(CompressedColumnStoreTupleStorageSubBlock_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_storage_CompressedColumnStoreTupleStorageSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_tests_TupleStorePredicateUtil
                      quickstep_types_CharType
                      quickstep_types_DoubleType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap
                      quickstep_utility_PtrVector
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(CompressedColumnStoreTupleStorageSubBlock_unittest CompressedColumnStoreTupleStorageSubBlock_unittest)

add_executable(CompressedPackedRowStoreTupleStorageSubBlock_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/CompressedPackedRowStoreTupleStorageSubBlock_unittest.cpp")
target_link_libraries(CompressedPackedRowStoreTupleStorageSubBlock_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_storage_CompressedPackedRowStoreTupleStorageSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_tests_TupleStorePredicateUtil
                      quickstep_types_CharType
                      quickstep_types_DoubleType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap
                      quickstep_utility_PtrVector
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(CompressedPackedRowStoreTupleStorageSubBlock_unittest CompressedPackedRowStoreTupleStorageSubBlock_unittest)

if (ENABLE_DISTRIBUTED)
  add_executable(DataExchange_unittest
                 "${CMAKE_CURRENT_SOURCE_DIR}/tests/DataExchange_unittest.cpp")
  target_link_libraries(DataExchange_unittest
                        ${GFLAGS_LIB_NAME}
                        glog
                        gtest
                        quickstep_catalog_CatalogAttribute
                        quickstep_catalog_CatalogRelation
                        quickstep_catalog_CatalogTypedefs
                        quickstep_queryexecution_BlockLocator
                        quickstep_queryexecution_BlockLocatorUtil
                        quickstep_queryexecution_QueryExecutionTypedefs
                        quickstep_queryexecution_QueryExecutionUtil
                        quickstep_storage_CountedReference
                        quickstep_storage_DataExchangerAsync
                        quickstep_storage_StorageBlob
                        quickstep_storage_StorageBlock
                        quickstep_storage_StorageBlockInfo
                        quickstep_storage_StorageConstants
                        quickstep_storage_StorageManager
                        quickstep_storage_TupleStorageSubBlock
                        quickstep_types_TypeFactory
                        quickstep_types_TypeID
                        quickstep_types_TypedValue
                        quickstep_types_containers_Tuple
                        tmb
                        ${LIBS})
  add_test(DataExchange_unittest DataExchange_unittest)
endif(ENABLE_DISTRIBUTED)

add_executable(CSBTreeIndexSubBlock_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/CSBTreeIndexSubBlock_unittest.cpp")
target_link_libraries(CSBTreeIndexSubBlock_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_storage_CSBTreeIndexSubBlock
                      quickstep_storage_CompressedPackedRowStoreTupleStorageSubBlock
                      quickstep_storage_CompressedTupleStorageSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageErrors
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_CharType
                      quickstep_types_FloatType
                      quickstep_types_LongType
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_BitVector
                      quickstep_utility_Macros
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(CSBTreeIndexSubBlock_unittest CSBTreeIndexSubBlock_unittest)

add_executable(EvictionPolicy_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/EvictionPolicy_unittest.cpp")
target_link_libraries(EvictionPolicy_unittest
                      gtest
                      gtest_main
                      quickstep_storage_EvictionPolicy
                      quickstep_storage_StorageBlockInfo
                      ${LIBS})
add_test(EvictionPolicy_unittest EvictionPolicy_unittest)

if (QUICKSTEP_HAVE_FILE_MANAGER_HDFS)
add_executable(FileManagerHdfs_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/FileManagerHdfs_unittest.cpp")
target_link_libraries(FileManagerHdfs_unittest
                      gtest
                      gtest_main
                      quickstep_storage_FileManagerHdfs
                      quickstep_storage_tests_FileManager_unittest_common
                      ${LIBS})
endif()

add_executable(FileManagerLocal_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/FileManagerLocal_unittest.cpp")
target_link_libraries(FileManagerLocal_unittest
                      gtest
                      gtest_main
                      quickstep_storage_FileManagerLocal
                      quickstep_storage_tests_FileManager_unittest_common
                      ${LIBS})
add_test(FileManagerLocal_unittest FileManagerLocal_unittest)

add_executable(LinearOpenAddressingHashTable_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/LinearOpenAddressingHashTable_unittest.cpp")
target_link_libraries(LinearOpenAddressingHashTable_unittest
                      gtest
                      gtest_main
                      quickstep_storage_LinearOpenAddressingHashTable
                      quickstep_storage_tests_HashTable_unittest_common
                      ${LIBS})
add_test(LinearOpenAddressingHashTable_unittest LinearOpenAddressingHashTable_unittest)

add_executable(SMAIndexSubBlock_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/SMAIndexSubBlock_unittest.cpp")
target_link_libraries(SMAIndexSubBlock_unittest
                      gtest
                      gtest_main
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_storage_CompressedColumnStoreTupleStorageSubBlock
                      quickstep_storage_SMAIndexSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_CharType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(SMAIndexSubBlock_unittest SMAIndexSubBlock_unittest)

add_executable(SeparateChainingHashTable_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/SeparateChainingHashTable_unittest.cpp")
target_link_libraries(SeparateChainingHashTable_unittest
                      gtest
                      gtest_main
                      quickstep_storage_SeparateChainingHashTable
                      quickstep_storage_tests_HashTable_unittest_common
                      ${LIBS})
add_test(SeparateChainingHashTable_unittest SeparateChainingHashTable_unittest)

add_executable(SimpleScalarSeparateChainingHashTable_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/SimpleScalarSeparateChainingHashTable_unittest.cpp")
target_link_libraries(SimpleScalarSeparateChainingHashTable_unittest
                      gtest
                      gtest_main
                      quickstep_storage_SimpleScalarSeparateChainingHashTable
                      quickstep_storage_tests_HashTable_unittest_common
                      ${LIBS})
add_test(SimpleScalarSeparateChainingHashTable_unittest
         SimpleScalarSeparateChainingHashTable_unittest)

add_executable(SplitRowStoreTupleStorageSubBlock_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/SplitRowStoreTupleStorageSubBlock_unittest.cpp")
target_link_libraries(SplitRowStoreTupleStorageSubBlock_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_SplitRowStoreTupleStorageSubBlock
                      quickstep_storage_SplitRowStoreValueAccessor
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_StorageErrors
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_types_CharType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_Tuple
                      quickstep_utility_BitVector
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(SplitRowStoreTupleStorageSubBlock_unittest SplitRowStoreTupleStorageSubBlock_unittest)

add_executable(StorageBlockSort_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/StorageBlockSort_unittest.cpp")
target_link_libraries(StorageBlockSort_unittest
                      gtest
                      gtest_main
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageManager
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_containers_Tuple
                      ${LIBS})
add_test(StorageBlockSort_unittest StorageBlockSort_unittest)

add_executable(StorageManager_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/StorageManager_unittest.cpp")
target_link_libraries(StorageManager_unittest
                      gtest
                      gtest_main
                      quickstep_storage_EvictionPolicy
                      quickstep_storage_StorageBlob
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_utility_ShardedLockManager)

add_executable(WindowAggregationOperationState_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/WindowAggregationOperationState_unittest.cpp")
target_link_libraries(WindowAggregationOperationState_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_WindowAggregationOperationState
                      quickstep_storage_WindowAggregationOperationState_proto
                      ${LIBS})
if (QUICKSTEP_HAVE_LIBNUMA)
  target_link_libraries(StorageManager_unittest
                        ${LIBNUMA_LIBRARY})
endif()
add_test(StorageManager_unittest StorageManager_unittest)
