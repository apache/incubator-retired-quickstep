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

# Declare micro-libs:
add_library(quickstep_types_operations_comparisons_AsciiStringComparators
            ../../../empty_src.cpp
            AsciiStringComparators.hpp
            AsciiStringComparators-inl.hpp)
add_library(quickstep_types_operations_comparisons_BasicComparison BasicComparison.cpp BasicComparison.hpp)
add_library(quickstep_types_operations_comparisons_Comparison Comparison.cpp Comparison.hpp Comparison-inl.hpp)
add_library(quickstep_types_operations_comparisons_ComparisonFactory ComparisonFactory.cpp ComparisonFactory.hpp)
add_library(quickstep_types_operations_comparisons_ComparisonID ComparisonID.cpp ComparisonID.hpp)
add_library(quickstep_types_operations_comparisons_ComparisonUtil ../../../empty_src.cpp ComparisonUtil.hpp)
add_library(quickstep_types_operations_comparisons_EqualComparison
            EqualComparison.cpp
            EqualComparison.hpp)
add_library(quickstep_types_operations_comparisons_GreaterComparison GreaterComparison.cpp GreaterComparison.hpp)
add_library(quickstep_types_operations_comparisons_GreaterOrEqualComparison
            GreaterOrEqualComparison.cpp
            GreaterOrEqualComparison.hpp)
add_library(quickstep_types_operations_comparisons_LessComparison LessComparison.cpp LessComparison.hpp)
add_library(quickstep_types_operations_comparisons_LessOrEqualComparison
            LessOrEqualComparison.cpp
            LessOrEqualComparison.hpp)
add_library(quickstep_types_operations_comparisons_LiteralComparators
            ../../../empty_src.cpp
            LiteralComparators.hpp
            LiteralComparators-inl.hpp)
add_library(quickstep_types_operations_comparisons_NotEqualComparison
            NotEqualComparison.cpp
            NotEqualComparison.hpp)
add_library(quickstep_types_operations_comparisons_PatternMatchingComparators
            ../../../empty_src.cpp
            PatternMatchingComparators.hpp
            PatternMatchingComparators-inl.hpp)
add_library(quickstep_types_operations_comparisons_PatternMatchingComparison
            PatternMatchingComparison.cpp
            PatternMatchingComparison.hpp)

# Link dependencies:
target_link_libraries(quickstep_types_operations_comparisons_AsciiStringComparators
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorUtil
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_port_strnlen
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_BasicComparison
                      glog
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_Comparison
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorUtil
                      quickstep_types_operations_Operation
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_ComparisonFactory
                      glog
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_EqualComparison
                      quickstep_types_operations_comparisons_GreaterComparison
                      quickstep_types_operations_comparisons_GreaterOrEqualComparison
                      quickstep_types_operations_comparisons_LessComparison
                      quickstep_types_operations_comparisons_LessOrEqualComparison
                      quickstep_types_operations_comparisons_NotEqualComparison
                      quickstep_types_operations_comparisons_PatternMatchingComparison
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_ComparisonID
                      glog)
target_link_libraries(quickstep_types_operations_comparisons_ComparisonUtil
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_types_DatetimeLit
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_EqualComparison
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_BasicComparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_GreaterComparison
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_BasicComparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_GreaterOrEqualComparison
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_BasicComparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_LessComparison
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_BasicComparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_LessOrEqualComparison
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_BasicComparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_operations_comparisons_Comparison)
target_link_libraries(quickstep_types_operations_comparisons_NotEqualComparison
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_BasicComparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_comparisons_PatternMatchingComparators
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorUtil
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_port_strnlen
                      re2)
target_link_libraries(quickstep_types_operations_comparisons_PatternMatchingComparison
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_operations_comparisons_PatternMatchingComparators
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_TemplateUtil
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_types_operations_comparisons ../../../empty_src.cpp)
target_link_libraries(quickstep_types_operations_comparisons
                      quickstep_types_operations_comparisons_AsciiStringComparators
                      quickstep_types_operations_comparisons_BasicComparison
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_comparisons_ComparisonUtil
                      quickstep_types_operations_comparisons_EqualComparison
                      quickstep_types_operations_comparisons_GreaterComparison
                      quickstep_types_operations_comparisons_GreaterOrEqualComparison
                      quickstep_types_operations_comparisons_LessComparison
                      quickstep_types_operations_comparisons_LessOrEqualComparison
                      quickstep_types_operations_comparisons_LiteralComparators
                      quickstep_types_operations_comparisons_NotEqualComparison
                      quickstep_types_operations_comparisons_PatternMatchingComparators
                      quickstep_types_operations_comparisons_PatternMatchingComparison)

# Tests:
add_executable(Comparison_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/Comparison_unittest.cpp")
target_link_libraries(Comparison_unittest
                      gtest
                      gtest_main
                      quickstep_types_DatetimeLit
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros
                      quickstep_utility_ScopedBuffer)
add_test(Comparison_unittest Comparison_unittest)
