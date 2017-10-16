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
add_library(quickstep_expressions_predicate_ComparisonPredicate
            ComparisonPredicate.cpp
            ComparisonPredicate.hpp)
add_library(quickstep_expressions_predicate_ConjunctionPredicate
            ConjunctionPredicate.cpp
            ConjunctionPredicate.hpp)
add_library(quickstep_expressions_predicate_DisjunctionPredicate
            DisjunctionPredicate.cpp
            DisjunctionPredicate.hpp)
add_library(quickstep_expressions_predicate_NegationPredicate
            NegationPredicate.cpp
            NegationPredicate.hpp)
add_library(quickstep_expressions_predicate_Predicate
            Predicate.cpp
            Predicate.hpp)
add_library(quickstep_expressions_predicate_PredicateCost
            ../../empty_src.cpp
            PredicateCost.hpp)
add_library(quickstep_expressions_predicate_PredicateWithList
            PredicateWithList.cpp
            PredicateWithList.hpp)
add_library(quickstep_expressions_predicate_TrivialPredicates
            ../../empty_src.cpp
            TrivialPredicates.hpp)

# Link dependencies:
target_link_libraries(quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_storage_IndexSubBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_SubBlocksReference
                      quickstep_storage_TupleStorageSubBlock
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_types_TypeErrors
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_expressions_predicate_ConjunctionPredicate
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_predicate_PredicateWithList
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_expressions_predicate_DisjunctionPredicate
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_predicate_PredicateWithList
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_expressions_predicate_NegationPredicate
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_predicate_Predicate
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_predicate_Predicate
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expression
                      quickstep_expressions_Expressions_proto
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_predicate_PredicateWithList
                      quickstep_expressions_predicate_Predicate
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_expressions_predicate_TrivialPredicates
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_predicate_Predicate
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_Macros)

# Submodule all-in-one library:
add_library(quickstep_expressions_predicate ../../empty_src.cpp)
target_link_libraries(quickstep_expressions_predicate
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_ConjunctionPredicate
                      quickstep_expressions_predicate_DisjunctionPredicate
                      quickstep_expressions_predicate_NegationPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_predicate_PredicateCost
                      quickstep_expressions_predicate_PredicateWithList
                      quickstep_expressions_predicate_TrivialPredicates)

# Tests:
add_executable(Predicate_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/Predicate_unittest.cpp")
target_link_libraries(Predicate_unittest
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_expressions_ExpressionFactories
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_ConjunctionPredicate
                      quickstep_expressions_predicate_DisjunctionPredicate
                      quickstep_expressions_predicate_NegationPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_predicate_PredicateWithList
                      quickstep_expressions_predicate_TrivialPredicates
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
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationID
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
add_test(Predicate_unittest Predicate_unittest)
