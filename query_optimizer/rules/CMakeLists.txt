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

add_subdirectory(tests)

# Declare micro-libs:
add_library(quickstep_queryoptimizer_rules_AttachLIPFilters AttachLIPFilters.cpp AttachLIPFilters.hpp)
add_library(quickstep_queryoptimizer_rules_BottomUpRule ../../empty_src.cpp BottomUpRule.hpp)
add_library(quickstep_queryoptimizer_rules_CollapseProject CollapseProject.cpp CollapseProject.hpp)
add_library(quickstep_queryoptimizer_rules_CollapseSelection CollapseSelection.cpp CollapseSelection.hpp)
add_library(quickstep_queryoptimizer_rules_ExtractCommonSubexpression
            ExtractCommonSubexpression.cpp
            ExtractCommonSubexpression.hpp)
add_library(quickstep_queryoptimizer_rules_FuseAggregateJoin FuseAggregateJoin.cpp FuseAggregateJoin.hpp)
add_library(quickstep_queryoptimizer_rules_GenerateJoins GenerateJoins.cpp GenerateJoins.hpp)
add_library(quickstep_queryoptimizer_rules_InjectJoinFilters InjectJoinFilters.cpp InjectJoinFilters.hpp)
add_library(quickstep_queryoptimizer_rules_PruneColumns PruneColumns.cpp PruneColumns.hpp)
add_library(quickstep_queryoptimizer_rules_PushDownFilter PushDownFilter.cpp PushDownFilter.hpp)
add_library(quickstep_queryoptimizer_rules_PushDownLowCostDisjunctivePredicate
            PushDownLowCostDisjunctivePredicate.cpp
            PushDownLowCostDisjunctivePredicate.hpp)
add_library(quickstep_queryoptimizer_rules_PushDownSemiAntiJoin PushDownSemiAntiJoin.cpp PushDownSemiAntiJoin.hpp)
add_library(quickstep_queryoptimizer_rules_ReduceGroupByAttributes
            ReduceGroupByAttributes.cpp
            ReduceGroupByAttributes.hpp)
add_library(quickstep_queryoptimizer_rules_ReorderColumns ReorderColumns.cpp ReorderColumns.hpp)
add_library(quickstep_queryoptimizer_rules_ReuseAggregateExpressions
            ReuseAggregateExpressions.cpp
            ReuseAggregateExpressions.hpp)
add_library(quickstep_queryoptimizer_rules_Rule ../../empty_src.cpp Rule.hpp)
add_library(quickstep_queryoptimizer_rules_RuleHelper RuleHelper.cpp RuleHelper.hpp)
add_library(quickstep_queryoptimizer_rules_StarSchemaHashJoinOrderOptimization
            StarSchemaHashJoinOrderOptimization.cpp
            StarSchemaHashJoinOrderOptimization.hpp)
add_library(quickstep_queryoptimizer_rules_SwapProbeBuild SwapProbeBuild.cpp SwapProbeBuild.hpp)
add_library(quickstep_queryoptimizer_rules_TopDownRule ../../empty_src.cpp TopDownRule.hpp)
add_library(quickstep_queryoptimizer_rules_UpdateExpression UpdateExpression.cpp UpdateExpression.hpp)
add_library(quickstep_queryoptimizer_rules_UnnestSubqueries UnnestSubqueries.cpp UnnestSubqueries.hpp)


# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_rules_AttachLIPFilters
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_LIPFilterConfiguration
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilter)
target_link_libraries(quickstep_queryoptimizer_rules_BottomUpRule
                      glog
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_CollapseProject
                      glog
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_CollapseSelection
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_ExtractCommonSubexpression
                      glog
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_CommonSubexpression
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_utility_HashError
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_FuseAggregateJoin
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_CrossReferenceCoalesceAggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_GenerateJoins
                      glog
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_MultiwayCartesianJoin
                      quickstep_queryoptimizer_logical_NestedLoopsJoin
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_queryoptimizer_rules_TopDownRule
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros
                      quickstep_utility_SqlError
                      quickstep_utility_VectorUtil)
target_link_libraries(quickstep_queryoptimizer_rules_PruneColumns
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_TopDownRule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_PushDownFilter
                      glog
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_queryoptimizer_rules_TopDownRule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_PushDownLowCostDisjunctivePredicate
                      ${GFLAGS_LIB_NAME}
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_LogicalOr
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_PushDownSemiAntiJoin
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_rules_TopDownRule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_ReduceGroupByAttributes
                      ${GFLAGS_LIB_NAME}
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_PruneColumns
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_ReorderColumns
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_ReuseAggregateExpressions
                      ${GFLAGS_LIB_NAME}
                      glog
                      quickstep_expressions_aggregation_AggregateFunction
                      quickstep_expressions_aggregation_AggregateFunctionFactory
                      quickstep_expressions_aggregation_AggregationID
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_BinaryExpression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_HashError
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_Rule
                      glog
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_rules_UpdateExpression)
target_link_libraries(quickstep_queryoptimizer_rules_StarSchemaHashJoinOrderOptimization
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_DisjointTreeForest
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_SwapProbeBuild
                      quickstep_queryoptimizer_costmodel_SimpleCostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_TopDownRule
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_InjectJoinFilters
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_FilterJoin
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_LIPFilterConfiguration
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_PruneColumns
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilter)
target_link_libraries(quickstep_queryoptimizer_rules_UnnestSubqueries
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_Exists
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_InTableQuery
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_LogicalNot
                      quickstep_queryoptimizer_expressions_LogicalOr
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_SubqueryExpression
                      quickstep_queryoptimizer_logical_Aggregate
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_queryoptimizer_logical_MultiwayCartesianJoin
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_logical_TopLevelPlan
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros
                      quickstep_utility_SqlError)
target_link_libraries(quickstep_queryoptimizer_rules_UpdateExpression
                      glog
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_rules ../../empty_src.cpp OptimizerRulesModule.hpp)
target_link_libraries(quickstep_queryoptimizer_rules
                      quickstep_queryoptimizer_rules_AttachLIPFilters
                      quickstep_queryoptimizer_rules_BottomUpRule
                      quickstep_queryoptimizer_rules_CollapseProject
                      quickstep_queryoptimizer_rules_CollapseSelection
                      quickstep_queryoptimizer_rules_ExtractCommonSubexpression
                      quickstep_queryoptimizer_rules_FuseAggregateJoin
                      quickstep_queryoptimizer_rules_GenerateJoins
                      quickstep_queryoptimizer_rules_InjectJoinFilters
                      quickstep_queryoptimizer_rules_PruneColumns
                      quickstep_queryoptimizer_rules_PushDownFilter
                      quickstep_queryoptimizer_rules_PushDownLowCostDisjunctivePredicate
                      quickstep_queryoptimizer_rules_PushDownSemiAntiJoin
                      quickstep_queryoptimizer_rules_ReduceGroupByAttributes
                      quickstep_queryoptimizer_rules_ReorderColumns
                      quickstep_queryoptimizer_rules_ReuseAggregateExpressions
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_queryoptimizer_rules_StarSchemaHashJoinOrderOptimization
                      quickstep_queryoptimizer_rules_SwapProbeBuild
                      quickstep_queryoptimizer_rules_TopDownRule
                      quickstep_queryoptimizer_rules_UpdateExpression
                      quickstep_queryoptimizer_rules_UnnestSubqueries)
