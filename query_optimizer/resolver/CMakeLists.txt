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
add_library(quickstep_queryoptimizer_resolver_NameResolver NameResolver.cpp NameResolver.hpp)
add_library(quickstep_queryoptimizer_resolver_Resolver Resolver.cpp Resolver.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_resolver_NameResolver
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_parser_ParseString
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_utility_Macros
                      quickstep_utility_SqlError
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_queryoptimizer_resolver_Resolver
                      glog
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_expressions_aggregation_AggregateFunction
                      quickstep_expressions_aggregation_AggregateFunctionFactory
                      quickstep_expressions_tablegenerator_GeneratorFunction
                      quickstep_expressions_tablegenerator_GeneratorFunctionFactory
                      quickstep_expressions_tablegenerator_GeneratorFunctionHandle
                      quickstep_expressions_windowaggregation_WindowAggregateFunction
                      quickstep_expressions_windowaggregation_WindowAggregateFunctionFactory
                      quickstep_parser_ParseAssignment
                      quickstep_parser_ParseBasicExpressions
                      quickstep_parser_ParseBlockProperties
                      quickstep_parser_ParseCaseExpressions
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseGeneratorTableReference
                      quickstep_parser_ParseGroupBy
                      quickstep_parser_ParseHaving
                      quickstep_parser_ParseJoinedTableReference
                      quickstep_parser_ParseLimit
                      quickstep_parser_ParseLiteralValue
                      quickstep_parser_ParseOrderBy
                      quickstep_parser_ParsePartitionClause
                      quickstep_parser_ParsePredicate
                      quickstep_parser_ParsePredicateExists
                      quickstep_parser_ParsePredicateInTableQuery
                      quickstep_parser_ParseSelect
                      quickstep_parser_ParseSelectionClause
                      quickstep_parser_ParseSimpleTableReference
                      quickstep_parser_ParseStatement
                      quickstep_parser_ParseString
                      quickstep_parser_ParseSubqueryExpression
                      quickstep_parser_ParseSubqueryTableReference
                      quickstep_parser_ParseTableReference
                      quickstep_parser_ParseWindow
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_Validator
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_BinaryExpression
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_Exists
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_InTableQuery
                      quickstep_queryoptimizer_expressions_InValueList
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_LogicalNot
                      quickstep_queryoptimizer_expressions_LogicalOr
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_PredicateLiteral
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_expressions_SearchedCase
                      quickstep_queryoptimizer_expressions_SimpleCase
                      quickstep_queryoptimizer_expressions_SubqueryExpression
                      quickstep_queryoptimizer_expressions_UnaryExpression
                      quickstep_queryoptimizer_expressions_WindowAggregateFunction
                      quickstep_queryoptimizer_logical_Aggregate
                      quickstep_queryoptimizer_logical_CopyFrom
                      quickstep_queryoptimizer_logical_CopyTo
                      quickstep_queryoptimizer_logical_CreateIndex
                      quickstep_queryoptimizer_logical_CreateTable
                      quickstep_queryoptimizer_logical_DeleteTuples
                      quickstep_queryoptimizer_logical_DropTable
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_InsertSelection
                      quickstep_queryoptimizer_logical_InsertTuple
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_MultiwayCartesianJoin
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_logical_Sample
                      quickstep_queryoptimizer_logical_SetOperation
                      quickstep_queryoptimizer_logical_SharedSubplanReference
                      quickstep_queryoptimizer_logical_Sort
                      quickstep_queryoptimizer_logical_TableGenerator
                      quickstep_queryoptimizer_logical_TableReference
                      quickstep_queryoptimizer_logical_TopLevelPlan
                      quickstep_queryoptimizer_logical_UpdateTable
                      quickstep_queryoptimizer_logical_WindowAggregate
                      quickstep_queryoptimizer_resolver_NameResolver
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_types_IntType
                      quickstep_types_Type
                      quickstep_types_TypeUtil
                      quickstep_types_TypedValue
                      quickstep_types_TypeFactory
                      quickstep_types_operations_OperationFactory
                      quickstep_types_operations_OperationSignature
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_utility_BulkIoConfiguration
                      quickstep_utility_Macros
                      quickstep_utility_PtrList
                      quickstep_utility_PtrVector
                      quickstep_utility_SqlError
                      quickstep_utility_StringUtil)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_resolver ../../empty_src.cpp QueryResolverModule.hpp)
target_link_libraries(quickstep_queryoptimizer_resolver
                      quickstep_queryoptimizer_resolver_NameResolver
                      quickstep_queryoptimizer_resolver_Resolver)
