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
                      ${LIBS}
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
                      quickstep_utility_ScopedBuffer)
add_test(CompressionDictionary_unittest CompressionDictionary_unittest)
