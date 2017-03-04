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

add_library(quickstep_queryoptimizer_strategy_tests_StrategyTest ../../../empty_src.cpp StrategyTest.hpp)

target_link_libraries(quickstep_queryoptimizer_strategy_tests_StrategyTest
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_strategy_Strategy
                      quickstep_queryoptimizer_tests_OptimizerTest
                      quickstep_utility_Macros)

# Unified test executable.
add_executable(quickstep_queryoptimizer_strategy_tests
               "${CMAKE_CURRENT_SOURCE_DIR}/Aggregate_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/Join_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/OneToOne_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/Selection_unittest.cpp")

target_link_libraries(quickstep_queryoptimizer_strategy_tests
                      glog
                      gtest
                      gtest_main
                      quickstep_expressions_aggregation_AggregateFunctionFactory
                      quickstep_expressions_aggregation_AggregationID
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_BinaryExpression
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_logical_Aggregate
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_NestedLoopsJoin
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_logical_TableReference
                      quickstep_queryoptimizer_logical_TopLevelPlan
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_strategy_Aggregate
                      quickstep_queryoptimizer_strategy_Join
                      quickstep_queryoptimizer_strategy_OneToOne
                      quickstep_queryoptimizer_strategy_Selection
                      quickstep_queryoptimizer_strategy_tests_StrategyTest
                      quickstep_types_TypeID
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Cast
                      quickstep_utility_Macros
                      ${LIBS})

add_test(quickstep_queryoptimizer_strategy_tests quickstep_queryoptimizer_strategy_tests)
