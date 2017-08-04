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
add_library(quickstep_queryoptimizer_strategy_Aggregate Aggregate.cpp Aggregate.hpp)
add_library(quickstep_queryoptimizer_strategy_Join Join.cpp Join.hpp)
add_library(quickstep_queryoptimizer_strategy_OneToOne OneToOne.cpp OneToOne.hpp)
add_library(quickstep_queryoptimizer_strategy_Selection Selection.cpp Selection.hpp)
add_library(quickstep_queryoptimizer_strategy_Strategy ../../empty_src.cpp Strategy.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_strategy_Aggregate
                      quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_logical_Aggregate
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_queryoptimizer_strategy_Strategy
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_strategy_Join
                      glog
                      quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_NestedLoopsJoin
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_logical_SetOperation
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_queryoptimizer_strategy_Strategy
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_strategy_OneToOne
                      quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_logical_CopyFrom
                      quickstep_queryoptimizer_logical_CopyTo
                      quickstep_queryoptimizer_logical_CreateIndex
                      quickstep_queryoptimizer_logical_CreateTable
                      quickstep_queryoptimizer_logical_DeleteTuples
                      quickstep_queryoptimizer_logical_DropTable
                      quickstep_queryoptimizer_logical_InsertSelection
                      quickstep_queryoptimizer_logical_InsertTuple
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_queryoptimizer_logical_Sample
                      quickstep_queryoptimizer_logical_SetOperation
                      quickstep_queryoptimizer_logical_SharedSubplanReference
                      quickstep_queryoptimizer_logical_Sort
                      quickstep_queryoptimizer_logical_TableGenerator
                      quickstep_queryoptimizer_logical_TableReference
                      quickstep_queryoptimizer_logical_TopLevelPlan
                      quickstep_queryoptimizer_logical_UpdateTable
                      quickstep_queryoptimizer_logical_WindowAggregate
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_CopyFrom
                      quickstep_queryoptimizer_physical_CopyTo
                      quickstep_queryoptimizer_physical_CreateIndex
                      quickstep_queryoptimizer_physical_CreateTable
                      quickstep_queryoptimizer_physical_DeleteTuples
                      quickstep_queryoptimizer_physical_DropTable
                      quickstep_queryoptimizer_physical_InsertSelection
                      quickstep_queryoptimizer_physical_InsertTuple
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_Sample
                      quickstep_queryoptimizer_physical_SharedSubplanReference
                      quickstep_queryoptimizer_physical_Sort
                      quickstep_queryoptimizer_physical_TableGenerator
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_physical_UnionAll
                      quickstep_queryoptimizer_physical_UpdateTable
                      quickstep_queryoptimizer_physical_WindowAggregate
                      quickstep_queryoptimizer_strategy_Strategy
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_strategy_Selection
                      glog
                      quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_physical_PartitionSchemeHeader
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_rules_RuleHelper
                      quickstep_queryoptimizer_strategy_Strategy
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_strategy_Strategy
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_strategy ../../empty_src.cpp OptimizerStrategyModule.hpp)
target_link_libraries(quickstep_queryoptimizer_strategy
                      quickstep_queryoptimizer_strategy_Aggregate
                      quickstep_queryoptimizer_strategy_Join
                      quickstep_queryoptimizer_strategy_OneToOne
                      quickstep_queryoptimizer_strategy_Selection
                      quickstep_queryoptimizer_strategy_Strategy)
