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

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
  set(QUICKSTEP_OS_WINDOWS TRUE)
endif()

if (USE_LINENOISE)
  set(QUICKSTEP_USE_LINENOISE TRUE)
endif()

if(LIBNUMA_FOUND)
  set(QUICKSTEP_HAVE_LIBNUMA TRUE)
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/CliConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/CliConfig.h"
)

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

add_library(quickstep_cli_InputParserUtil InputParserUtil.cpp InputParserUtil.hpp)
add_library(quickstep_cli_PrintToScreen PrintToScreen.cpp PrintToScreen.hpp)

# Link dependencies:
target_link_libraries(quickstep_cli_InputParserUtil
                      glog
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
if(QUICKSTEP_HAVE_LIBNUMA)
target_link_libraries(quickstep_cli_InputParserUtil
                      ${LIBNUMA_LIBRARY})
endif()
target_link_libraries(quickstep_cli_PrintToScreen
                      gflags_nothreads-static
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_cli ../empty_src.cpp CliModule.hpp)

target_link_libraries(quickstep_cli
                      quickstep_cli_DropRelation
                      quickstep_cli_InputParserUtil
                      quickstep_cli_LineReader
                      quickstep_cli_PrintToScreen)
