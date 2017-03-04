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

add_subdirectory(binary_operations)
add_subdirectory(comparisons)
add_subdirectory(unary_operations)

QS_PROTOBUF_GENERATE_CPP(types_operations_Operation_proto_srcs
                         types_operations_Operation_proto_hdrs
                         Operation.proto)

# Declare micro-libs:
add_library(quickstep_types_operations_Operation Operation.cpp Operation.hpp)
add_library(quickstep_types_operations_OperationFactory OperationFactory.cpp OperationFactory.hpp)
add_library(quickstep_types_operations_OperationUtil ../../empty_src.cpp OperationUtil.hpp)
add_library(quickstep_types_operations_OperationSignature OperationSignature.cpp OperationSignature.hpp)
add_library(quickstep_types_operations_Operation_proto ${types_operations_Operation_proto_srcs})

# Link dependencies:
target_link_libraries(quickstep_types_operations_Operation
                      quickstep_types_operations_OperationSignature
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_OperationFactory
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypeUtil
                      quickstep_types_TypedValue
                      quickstep_types_operations_Operation
                      quickstep_types_operations_OperationSignature
                      quickstep_types_operations_binaryoperations_ArithmeticBinaryOperations
                      quickstep_types_operations_binaryoperations_AsciiStringBinaryOperations
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationWrapper
                      quickstep_types_operations_binaryoperations_CMathBinaryOperations
                      quickstep_types_operations_unaryoperations_ArithmeticUnaryOperations
                      quickstep_types_operations_unaryoperations_AsciiStringUnaryOperations
                      quickstep_types_operations_unaryoperations_CMathUnaryOperations
                      quickstep_types_operations_unaryoperations_CastOperation
                      quickstep_types_operations_unaryoperations_DateExtractOperation
                      quickstep_types_operations_unaryoperations_SubstringOperation
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationWrapper
                      quickstep_utility_HashPair
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_types_operations_OperationUtil
                      quickstep_catalog_CatalogTypedefs
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector)
target_link_libraries(quickstep_types_operations_OperationSignature
                      quickstep_types_TypeID
                      quickstep_types_Type_proto
                      quickstep_types_operations_Operation_proto
                      quickstep_utility_HashPair
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_Operation_proto
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue_proto
                      ${PROTOBUF_LIBRARY})

# Module all-in-one library:
add_library(quickstep_types_operations ../../empty_src.cpp)
target_link_libraries(quickstep_types_operations
                      quickstep_types_operations_Operation
                      quickstep_types_operations_OperationFactory
                      quickstep_types_operations_OperationUtil
                      quickstep_types_operations_OperationSignature
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_binaryoperations
                      quickstep_types_operations_comparisons
                      quickstep_types_operations_unaryoperations)
