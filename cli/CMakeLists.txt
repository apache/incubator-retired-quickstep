# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# Copyright (c) 2015, Pivotal Software, Inc.
# All rights reserved.
# See file CREDITS.txt for details.

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
