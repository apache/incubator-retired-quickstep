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
add_library(quickstep_expressions_scalar_Scalar Scalar.cpp Scalar.hpp)
add_library(quickstep_expressions_scalar_ScalarAttribute
            ScalarAttribute.cpp
            ScalarAttribute.hpp)
add_library(quickstep_expressions_scalar_ScalarBinaryExpression
            ScalarBinaryExpression.cpp
            ScalarBinaryExpression.hpp)
add_library(quickstep_expressions_scalar_ScalarCaseExpression
            ScalarCaseExpression.cpp
            ScalarCaseExpression.hpp)
add_library(quickstep_expressions_scalar_ScalarLiteral
            ScalarLiteral.cpp
            ScalarLiteral.hpp)
add_library(quickstep_expressions_scalar_ScalarSharedExpression
            ScalarSharedExpression.cpp
            ScalarSharedExpression.hpp)
add_library(quickstep_expressions_scalar_ScalarUnaryExpression
            ScalarUnaryExpression.cpp
            ScalarUnaryExpression.hpp)

# Link dependencies:
target_link_libraries(quickstep_expressions_scalar_Scalar
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expression
                      quickstep_expressions_Expressions_proto
                      quickstep_storage_StorageBlockInfo
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_scalar_ScalarAttribute
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_scalar_ScalarBinaryExpression
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_scalar_ScalarCaseExpression
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_scalar_ScalarLiteral
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_types_containers_ColumnVector
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_scalar_ScalarSharedExpression
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_utility_ColumnVectorCache
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_scalar_ScalarUnaryExpression
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_scalar_Scalar
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationID
                      quickstep_utility_Macros)

# Submodule all-in-one library:
add_library(quickstep_expressions_scalar ../../empty_src.cpp)
target_link_libraries(quickstep_expressions_scalar
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarBinaryExpression
                      quickstep_expressions_scalar_ScalarCaseExpression
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_expressions_scalar_ScalarSharedExpression
                      quickstep_expressions_scalar_ScalarUnaryExpression)

# Tests:
add_executable(ScalarCaseExpression_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/ScalarCaseExpression_unittest.cpp")
target_link_libraries(ScalarCaseExpression_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_ConjunctionPredicate
                      quickstep_expressions_predicate_DisjunctionPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarBinaryExpression
                      quickstep_expressions_scalar_ScalarCaseExpression
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_expressions_scalar_ScalarUnaryExpression
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_containers_ColumnVectorsValueAccessor
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_unaryoperations_UnaryOperationFactory
                      quickstep_types_operations_unaryoperations_UnaryOperationID)
add_test(ScalarCaseExpression_unittest ScalarCaseExpression_unittest)

add_executable(Scalar_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/Scalar_unittest.cpp")
target_link_libraries(Scalar_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_expressions_ExpressionFactories
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarBinaryExpression
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_expressions_scalar_ScalarUnaryExpression
                      quickstep_types_DatetimeLit
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_unaryoperations_NumericCastOperation
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationFactory
                      quickstep_types_operations_unaryoperations_UnaryOperationID
                      quickstep_utility_Macros)
add_test(Scalar_unittest Scalar_unittest)
