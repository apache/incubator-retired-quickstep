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

if(LIBNUMA_FOUND)
  set(QUICKSTEP_HAVE_LIBNUMA TRUE)
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/CatalogConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CatalogConfig.h"
)

QS_PROTOBUF_GENERATE_CPP(catalog_Catalog_proto_srcs catalog_Catalog_proto_hdrs Catalog.proto)

# Declare micro-libs:
add_library(quickstep_catalog_Catalog Catalog.cpp Catalog.hpp)
add_library(quickstep_catalog_Catalog_proto ${catalog_Catalog_proto_srcs})
add_library(quickstep_catalog_CatalogAttribute CatalogAttribute.cpp CatalogAttribute.hpp)
add_library(quickstep_catalog_CatalogDatabase CatalogDatabase.cpp CatalogDatabase.hpp)
add_library(quickstep_catalog_CatalogDatabaseCache CatalogDatabaseCache.cpp CatalogDatabaseCache.hpp)
add_library(quickstep_catalog_CatalogDatabaseLite ../empty_src.cpp CatalogDatabaseLite.hpp)
add_library(quickstep_catalog_CatalogErrors ../empty_src.cpp CatalogErrors.hpp)
add_library(quickstep_catalog_CatalogRelation CatalogRelation.cpp CatalogRelation.hpp)
add_library(quickstep_catalog_CatalogRelationSchema
            CatalogRelationSchema.cpp
            CatalogRelationSchema.hpp)
add_library(quickstep_catalog_CatalogTypedefs ../empty_src.cpp CatalogTypedefs.hpp)
add_library(quickstep_catalog_IndexScheme IndexScheme.cpp IndexScheme.hpp)
if(QUICKSTEP_HAVE_LIBNUMA)
  add_library(quickstep_catalog_NUMAPlacementScheme NUMAPlacementScheme.cpp NUMAPlacementScheme.hpp)
endif()
add_library(quickstep_catalog_PartitionScheme PartitionScheme.cpp PartitionScheme.hpp)
add_library(quickstep_catalog_PartitionSchemeHeader PartitionSchemeHeader.cpp PartitionSchemeHeader.hpp)

# Link dependencies:
target_link_libraries(quickstep_catalog_Catalog
                      glog
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogErrors
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_catalog_Catalog_proto
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_catalog_CatalogAttribute
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_Type_proto
                      quickstep_utility_Macros)
target_link_libraries(quickstep_catalog_CatalogDatabase
                      glog
                      quickstep_catalog_CatalogDatabaseLite
                      quickstep_catalog_CatalogErrors
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_storage_StorageConstants
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_catalog_CatalogDatabaseCache
                      glog
                      quickstep_catalog_CatalogDatabaseLite
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_storage_StorageConstants
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_catalog_CatalogDatabaseLite
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros)
target_link_libraries(quickstep_catalog_CatalogRelation
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_catalog_IndexScheme
                      quickstep_catalog_PartitionScheme
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_catalog_IndexScheme
                      glog
                      quickstep_catalog_Catalog_proto
                      quickstep_storage_IndexSubBlockDescriptionFactory
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_utility_Macros)

if(QUICKSTEP_HAVE_LIBNUMA)
target_link_libraries(quickstep_catalog_CatalogRelation
                      quickstep_catalog_NUMAPlacementScheme)
endif()
target_link_libraries(quickstep_catalog_CatalogRelationSchema
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogErrors
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_types_Type
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector
                      quickstep_utility_StringUtil)
if(QUICKSTEP_HAVE_LIBNUMA)
target_link_libraries(quickstep_catalog_NUMAPlacementScheme
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_Macros
                      ${LIBNUMA_LIBRARY})
endif()
target_link_libraries(quickstep_catalog_PartitionScheme
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_catalog_PartitionSchemeHeader
                      quickstep_storage_StorageBlockInfo
                      quickstep_threading_Mutex
                      quickstep_threading_SharedMutex
                      quickstep_threading_SpinSharedMutex
                      quickstep_utility_Macros)
target_link_libraries(quickstep_catalog_PartitionSchemeHeader
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_LessComparison
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_catalog ../empty_src.cpp CatalogModule.hpp)
target_link_libraries(quickstep_catalog
                      quickstep_catalog_Catalog
                      quickstep_catalog_Catalog_proto
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogDatabaseCache
                      quickstep_catalog_CatalogDatabaseLite
                      quickstep_catalog_CatalogErrors
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_IndexScheme
                      quickstep_catalog_PartitionScheme
                      quickstep_catalog_PartitionSchemeHeader)
if(QUICKSTEP_HAVE_LIBNUMA)
target_link_libraries(quickstep_catalog
                      quickstep_catalog_NUMAPlacementScheme)
endif()

# Tests:
add_executable(Catalog_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/Catalog_unittest.cpp")
target_link_libraries(Catalog_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_Catalog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogDatabaseCache
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_catalog_IndexScheme
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_SubBlockTypeRegistryMacros
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
add_test(Catalog_unittest Catalog_unittest)

if(QUICKSTEP_HAVE_LIBNUMA)
add_executable(NUMAPlacementScheme_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/NUMAPlacementScheme_unittest.cpp")
target_link_libraries(NUMAPlacementScheme_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_NUMAPlacementScheme
                      quickstep_catalog_PartitionSchemeHeader
                      quickstep_storage_StorageBlockInfo
                      ${LIBS})
add_test(NUMAPlacementScheme_unittest NUMAPlacementScheme_unittest)
endif()

add_executable(PartitionScheme_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/PartitionScheme_unittest.cpp")
target_link_libraries(PartitionScheme_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_PartitionScheme
                      quickstep_catalog_PartitionSchemeHeader
                      quickstep_storage_StorageBlockInfo
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_EqualComparison)
add_test(PartitionScheme_unittest PartitionScheme_unittest)
