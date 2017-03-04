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
add_library(quickstep_types_operations_unaryoperations_ArithmeticUnaryOperations
            ../../../empty_src.cpp
            ArithmeticUnaryOperations.hpp)
add_library(quickstep_types_operations_unaryoperations_AsciiStringUnaryOperations
            ../../../empty_src.cpp
            AsciiStringUnaryOperations.hpp)
add_library(quickstep_types_operations_unaryoperations_CMathUnaryOperations
            ../../../empty_src.cpp
            CMathUnaryOperations.hpp)
add_library(quickstep_types_operations_unaryoperations_CastOperation CastOperation.cpp CastOperation.hpp)
add_library(quickstep_types_operations_unaryoperations_DateExtractOperation
            DateExtractOperation.cpp
            DateExtractOperation.hpp)
add_library(quickstep_types_operations_unaryoperations_SubstringOperation
            SubstringOperation.cpp
            SubstringOperation.hpp)
add_library(quickstep_types_operations_unaryoperations_UnaryOperation UnaryOperation.cpp UnaryOperation.hpp)
add_library(quickstep_types_operations_unaryoperations_UnaryOperationWrapper
            ../../../empty_src.cpp
            UnaryOperationWrapper.hpp)

# Link dependencies:
target_link_libraries(quickstep_types_operations_unaryoperations_ArithmeticUnaryOperations
                      glog
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_operations_OperationUtil
                      quickstep_types_operations_unaryoperations_UnaryOperationWrapper)
target_link_libraries(quickstep_types_operations_unaryoperations_AsciiStringUnaryOperations
                      quickstep_types_CharType
                      quickstep_types_IntType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_VarCharType
                      quickstep_types_operations_OperationUtil
                      quickstep_types_operations_unaryoperations_UnaryOperationWrapper
                      quickstep_types_port_strnlen
                      quickstep_utility_meta_Common)
target_link_libraries(quickstep_types_operations_unaryoperations_CMathUnaryOperations
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_operations_OperationUtil
                      quickstep_types_operations_unaryoperations_UnaryOperationWrapper
                      quickstep_utility_meta_Common)
target_link_libraries(quickstep_types_operations_unaryoperations_CastOperation
                      glog
                      quickstep_types_CharType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypeUtil
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationWrapper
                      quickstep_types_port_strnlen
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_types_operations_unaryoperations_DateExtractOperation
                      glog
                      quickstep_types_DateType
                      quickstep_types_DatetimeLit
                      quickstep_types_DatetimeType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationWrapper
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_types_operations_unaryoperations_SubstringOperation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_CharType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorUtil
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_port_strnlen
                      quickstep_utility_Macros
                      quickstep_utility_meta_Dispatchers)
target_link_libraries(quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_types_TypedValue
                      quickstep_types_operations_Operation
                      quickstep_types_operations_OperationSignature
                      quickstep_types_operations_Operation_proto
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_unaryoperations_UnaryOperationWrapper
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_operations_OperationSignature
                      quickstep_types_operations_OperationUtil
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_types_operations_unaryoperations ../../../empty_src.cpp)
target_link_libraries(quickstep_types_operations_unaryoperations
                      quickstep_types_operations_unaryoperations_ArithmeticUnaryOperations
                      quickstep_types_operations_unaryoperations_AsciiStringUnaryOperations
                      quickstep_types_operations_unaryoperations_CMathUnaryOperations
                      quickstep_types_operations_unaryoperations_CastOperation
                      quickstep_types_operations_unaryoperations_DateExtractOperation
                      quickstep_types_operations_unaryoperations_SubstringOperation
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationWrapper)

# Tests:

# Unified test executable.
add_executable(UnaryOperation_tests
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/DateExtractOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/NegateUnaryOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/NumericCastOperation_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/UnaryOperation_unittest.cpp")
target_link_libraries(UnaryOperation_tests
                      gtest
                      gtest_main
                      quickstep_types_DatetimeLit
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_unaryoperations_ArithmeticUnaryOperations
                      quickstep_types_operations_unaryoperations_CastOperation
                      quickstep_types_operations_unaryoperations_DateExtractOperation
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
add_test(UnaryOperation_tests UnaryOperation_tests)
