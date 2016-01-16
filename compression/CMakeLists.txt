# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# Copyright (c) 2015, Pivotal Software, Inc.
# All rights reserved.
# See file CREDITS.txt for details.

# Declare micro-libs:
add_library(quickstep_compression_CompressionDictionary
            CompressionDictionary.cpp
            CompressionDictionary.hpp)
add_library(quickstep_compression_CompressionDictionaryBuilder
            CompressionDictionaryBuilder.cpp
            CompressionDictionaryBuilder.hpp)
add_library(quickstep_compression_CompressionDictionaryLite
            CompressionDictionaryLite.cpp
            CompressionDictionaryLite.hpp)

# Link dependencies:
target_link_libraries(quickstep_compression_CompressionDictionary
                      glog
                      quickstep_compression_CompressionDictionaryLite
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_types_operations_comparisons_EqualComparison
                      quickstep_utility_Macros)
target_link_libraries(quickstep_compression_CompressionDictionaryBuilder
                      glog
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_utility_Macros)
target_link_libraries(quickstep_compression_CompressionDictionaryLite
                      glog
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_compression ../empty_src.cpp CompressionModule.hpp)
target_link_libraries(quickstep_compression
                      quickstep_compression_CompressionDictionary
                      quickstep_compression_CompressionDictionaryBuilder
                      quickstep_compression_CompressionDictionaryLite)

# Tests:
add_executable(CompressionDictionary_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/CompressionDictionary_unittest.cpp")
target_link_libraries(CompressionDictionary_unittest
                      gtest
                      gtest_main
                      quickstep_compression_CompressionDictionary
                      quickstep_compression_CompressionDictionaryBuilder
                      quickstep_types_CharType
                      quickstep_types_DoubleType
                      quickstep_types_IntType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_ScopedBuffer
                      ${LIBS})
add_test(CompressionDictionary_unittest CompressionDictionary_unittest)
