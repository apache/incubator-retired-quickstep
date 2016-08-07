#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015 Pivotal Software, Inc.
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

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(tests)

if (WIN32)
  set(QUICKSTEP_OS_WINDOWS TRUE)
endif()

if (USE_LINENOISE)
  set(QUICKSTEP_USE_LINENOISE TRUE)
endif()

if(LIBNUMA_FOUND)
  set(QUICKSTEP_HAVE_LIBNUMA TRUE)
endif()

if (BUILD_SHARED_LIBS)
  set(GFLAGS_LIB_NAME gflags_nothreads-shared)
else()
  set(GFLAGS_LIB_NAME gflags_nothreads-static)
endif()

if (ENABLE_GOOGLE_PROFILER)
  set(QUICKSTEP_ENABLE_GOOGLE_PROFILER TRUE)
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/CliConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CliConfig.h"
)
add_library(quickstep_cli_CommandExecutor CommandExecutor.cpp CommandExecutor.hpp)

# Declare micro-libs and link dependencies:
add_library(quickstep_cli_DropRelation DropRelation.cpp DropRelation.hpp)
target_link_libraries(quickstep_cli_DropRelation
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_utility_Macros)

if(USE_LINENOISE)
  add_library(quickstep_cli_LineReader
              LineReader.cpp
              LineReaderLineNoise.cpp
              LineReader.hpp
              LineReaderLineNoise.hpp)
  target_link_libraries(quickstep_cli_LineReader
                        linenoise
                        quickstep_utility_Macros)
else()
  add_library(quickstep_cli_LineReader
              LineReader.cpp
              LineReaderDumb.cpp
              LineReader.hpp
              LineReaderDumb.hpp)
  target_link_libraries(quickstep_cli_LineReader
                        quickstep_utility_Macros)
endif()

add_library(quickstep_cli_DefaultsConfigurator ../empty_src.cpp DefaultsConfigurator.hpp)
add_library(quickstep_cli_InputParserUtil InputParserUtil.cpp InputParserUtil.hpp)
add_library(quickstep_cli_PrintToScreen PrintToScreen.cpp PrintToScreen.hpp)

# Link dependencies:
target_link_libraries(quickstep_cli_CommandExecutor
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_cli_DropRelation
                      quickstep_cli_PrintToScreen
                      quickstep_parser_ParseStatement
                      quickstep_parser_SqlParserWrapper
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_queryoptimizer_QueryProcessor
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_parser_ParseString
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_PtrVector
                      quickstep_utility_SqlError)

target_link_libraries(quickstep_cli_DefaultsConfigurator
                      quickstep_utility_Macros)
if(QUICKSTEP_HAVE_LIBNUMA)
  target_link_libraries(quickstep_cli_DefaultsConfigurator
                      ${LIBNUMA_LIBRARY})
endif()
target_link_libraries(quickstep_cli_InputParserUtil
                      glog
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
if(QUICKSTEP_HAVE_LIBNUMA)
target_link_libraries(quickstep_cli_InputParserUtil
                      ${LIBNUMA_LIBRARY})
endif()
target_link_libraries(quickstep_cli_PrintToScreen
                      ${GFLAGS_LIB_NAME}
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_IntType
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_cli ../empty_src.cpp CliModule.hpp)

target_link_libraries(quickstep_cli
                      quickstep_cli_CommandExecutor
                      quickstep_cli_DefaultsConfigurator
                      quickstep_cli_DropRelation
                      quickstep_cli_InputParserUtil
                      quickstep_cli_LineReader
                      quickstep_cli_PrintToScreen)
