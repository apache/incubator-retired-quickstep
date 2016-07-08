#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015-2016 Pivotal Software, Inc.
#   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
#     University of Wisconsin—Madison.
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
add_library(quickstep_types_operations_binaryoperations_AddBinaryOperation
            AddBinaryOperation.cpp
            AddBinaryOperation.hpp)
add_library(quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
            ../../../empty_src.cpp
            ArithmeticBinaryOperation.hpp)
add_library(quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
            ../../../empty_src.cpp
            ArithmeticBinaryOperators.hpp)
add_library(quickstep_types_operations_binaryoperations_BinaryOperation
            BinaryOperation.cpp
            BinaryOperation.hpp)
add_library(quickstep_types_operations_binaryoperations_BinaryOperationFactory
            BinaryOperationFactory.cpp
            BinaryOperationFactory.hpp)
add_library(quickstep_types_operations_binaryoperations_BinaryOperationID
            BinaryOperationID.cpp
            BinaryOperationID.hpp)
add_library(quickstep_types_operations_binaryoperations_DivideBinaryOperation
            DivideBinaryOperation.cpp
            DivideBinaryOperation.hpp)
add_library(quickstep_types_operations_binaryoperations_ModuloBinaryOperation
            ModuloBinaryOperation.cpp
            ModuloBinaryOperation.hpp)
add_library(quickstep_types_operations_binaryoperations_MultiplyBinaryOperation
            MultiplyBinaryOperation.cpp
            MultiplyBinaryOperation.hpp)
add_library(quickstep_types_operations_binaryoperations_SubtractBinaryOperation
            SubtractBinaryOperation.cpp
            SubtractBinaryOperation.hpp)

# Link dependencies:
target_link_libraries(quickstep_types_operations_binaryoperations_AddBinaryOperation
                      glog
                      quickstep_types_DateOperatorOverloads
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DatetimeLit
                      quickstep_types_DatetimeType
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
                      glog
                      quickstep_types_DecimalType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_NumericTypeUnifier
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_DecimalLit
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_types_TypedValue
                      quickstep_types_operations_Operation
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      glog
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_binaryoperations_AddBinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_binaryoperations_DivideBinaryOperation
                      quickstep_types_operations_binaryoperations_ModuloBinaryOperation
                      quickstep_types_operations_binaryoperations_MultiplyBinaryOperation
                      quickstep_types_operations_binaryoperations_SubtractBinaryOperation
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_DivideBinaryOperation
                      glog
                      quickstep_types_DateOperatorOverloads
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_IntervalLit
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_ModuloBinaryOperation
                      glog
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_MultiplyBinaryOperation
                      glog
                      quickstep_types_DateOperatorOverloads
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_IntervalLit
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_SubtractBinaryOperation
                      glog
                      quickstep_types_DateOperatorOverloads
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DatetimeLit
                      quickstep_types_DatetimeType
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_types_operations_binaryoperations ../../../empty_src.cpp)
target_link_libraries(quickstep_types_operations_binaryoperations
                      quickstep_types_operations_binaryoperations_AddBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperators
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_binaryoperations_DivideBinaryOperation
                      quickstep_types_operations_binaryoperations_ModuloBinaryOperation
                      quickstep_types_operations_binaryoperations_MultiplyBinaryOperation
                      quickstep_types_operations_binaryoperations_SubtractBinaryOperation)

# Tests:
add_library(quickstep_types_operations_binaryoperations_tests_BinaryOperationTestUtil
            ../../../empty_src.cpp
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/BinaryOperationTestUtil.hpp")
target_link_libraries(quickstep_types_operations_binaryoperations_tests_BinaryOperationTestUtil
                      gtest
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_utility_Macros)

# Unified test executable.
add_executable(BinaryOperation_tests
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/AddBinaryOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/BinaryOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/DivideBinaryOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/ModuloBinaryOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/MultiplyBinaryOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/SubtractBinaryOperation_unittest.cpp")
target_link_libraries(BinaryOperation_tests
                      glog
                      gtest
                      gtest_main
                      quickstep_types_DateOperatorOverloads
                      quickstep_types_DatetimeLit
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_binaryoperations_AddBinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_binaryoperations_DivideBinaryOperation
                      quickstep_types_operations_binaryoperations_ModuloBinaryOperation
                      quickstep_types_operations_binaryoperations_MultiplyBinaryOperation
                      quickstep_types_operations_binaryoperations_SubtractBinaryOperation
                      quickstep_types_operations_binaryoperations_tests_BinaryOperationTestUtil
                      quickstep_utility_Macros)
add_test(BinaryOperation_tests BinaryOperation_tests)
