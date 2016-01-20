#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015 Pivotal Software, Inc.
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

add_library(quickstep_queryoptimizer_rules_tests_ExpressionRuleTest ../../../empty_src.cpp ExpressionRuleTest.hpp)
add_library(quickstep_queryoptimizer_rules_tests_LogicalRuleTest ../../../empty_src.cpp LogicalRuleTest.hpp)
add_library(quickstep_queryoptimizer_rules_tests_PhysicalRuleTest ../../../empty_src.cpp PhysicalRuleTest.hpp)
add_library(quickstep_queryoptimizer_rules_tests_RuleTest ../../../empty_src.cpp RuleTest.hpp)

target_link_libraries(quickstep_queryoptimizer_rules_tests_ExpressionRuleTest
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_tests_OptimizerTest
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_tests_LogicalRuleTest
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_tests_OptimizerTest
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_rules_tests_PhysicalRuleTest
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_tests_OptimizerTest
                      quickstep_utility_Macros)

# Unified test executable.
add_executable(quickstep_queryoptimizer_rules_tests
               "${CMAKE_CURRENT_SOURCE_DIR}/CollapseProject_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/GenerateJoins_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/PruneColumns_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/PushDownFilter_unittest.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/UpdateExpression_unittest.cpp")

target_link_libraries(quickstep_queryoptimizer_rules_tests
                      glog
                      gtest
                      gtest_main
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_BinaryExpression
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_LogicalNot
                      quickstep_queryoptimizer_expressions_LogicalOr
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_MultiwayCartesianJoin
                      quickstep_queryoptimizer_logical_NestedLoopsJoin
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_logical_TableReference
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_rules_CollapseProject
                      quickstep_queryoptimizer_rules_GenerateJoins
                      quickstep_queryoptimizer_rules_PruneColumns
                      quickstep_queryoptimizer_rules_PushDownFilter
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_UpdateExpression
                      quickstep_queryoptimizer_rules_tests_ExpressionRuleTest
                      quickstep_queryoptimizer_rules_tests_LogicalRuleTest
                      quickstep_queryoptimizer_rules_tests_PhysicalRuleTest
                      quickstep_queryoptimizer_rules_tests_RuleTest
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Cast
                      quickstep_utility_Macros
                      ${LIBS})
add_test(quickstep_queryoptimizer_rules_tests quickstep_queryoptimizer_rules_tests)
