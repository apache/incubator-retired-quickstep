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
add_library(quickstep_types_operations_binaryoperations_ArithmeticBinaryFunctors
            ../../../empty_src.cpp
            ArithmeticBinaryFunctors.hpp)
add_library(quickstep_types_operations_binaryoperations_ArithmeticBinaryFunctorOverloads
            ../../../empty_src.cpp
            ArithmeticBinaryFunctorOverloads.hpp)
add_library(quickstep_types_operations_binaryoperations_AsciiStringBinaryFunctors
            ../../../empty_src.cpp
            AsciiStringBinaryFunctors.hpp)
add_library(quickstep_types_operations_binaryoperations_BinaryOperation
            BinaryOperation.cpp
            BinaryOperation.hpp)
add_library(quickstep_types_operations_binaryoperations_BinaryOperationSynthesizer
            ../../../empty_src.cpp
            BinaryOperationSynthesizer.hpp)
add_library(quickstep_types_operations_binaryoperations_CMathBinaryFunctors
            ../../../empty_src.cpp
            CMathBinaryFunctors.hpp)

# Link dependencies:
target_link_libraries(quickstep_types_operations_binaryoperations_ArithmeticBinaryFunctorOverloads
                      quickstep_types_DateOperatorOverloads
                      quickstep_utility_meta_Common)
target_link_libraries(quickstep_types_operations_binaryoperations_ArithmeticBinaryFunctors
                      quickstep_types_DateType
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DatetimeLit
                      quickstep_types_DatetimeType
                      quickstep_types_IntervalLit
                      quickstep_types_NumericTypeUnifier
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryFunctorOverloads
                      quickstep_types_operations_binaryoperations_BinaryOperationSynthesizer
                      quickstep_utility_meta_Common)
target_link_libraries(quickstep_types_operations_binaryoperations_AsciiStringBinaryFunctors
                      glog
                      quickstep_types_CharType
                      quickstep_types_IntType
                      quickstep_types_TextType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_VarCharType
                      quickstep_types_operations_unaryoperations_UnaryOperationSynthesizer
                      quickstep_types_operations_utility_OperationSynthesizeUtil
                      quickstep_types_port_strnlen)
target_link_libraries(quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_types_TypedValue
                      quickstep_types_operations_Operation
                      quickstep_types_operations_OperationSignature
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_binaryoperations_BinaryOperationSynthesizer
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
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_utility_OperationSynthesizeUtil
                      quickstep_utility_Macros
                      quickstep_utility_meta_Common)
target_link_libraries(quickstep_types_operations_binaryoperations_CMathBinaryFunctors
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_operations_binaryoperations_BinaryOperationSynthesizer
                      quickstep_types_operations_utility_OperationSynthesizeUtil
                      quickstep_utility_meta_Common)

# Module all-in-one library:
add_library(quickstep_types_operations_binaryoperations ../../../empty_src.cpp)
target_link_libraries(quickstep_types_operations_binaryoperations
                      quickstep_types_operations_binaryoperations_AddBinaryOperation
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryFunctors
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryFunctorOverloads
                      quickstep_types_operations_binaryoperations_AsciiStringBinaryFunctors
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationSynthesizer
                      quickstep_types_operations_binaryoperations_CMathBinaryFunctors)

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
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_tests_BinaryOperationTestUtil
                      quickstep_utility_Macros)
add_test(BinaryOperation_tests BinaryOperation_tests)
