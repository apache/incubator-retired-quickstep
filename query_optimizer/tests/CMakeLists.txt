#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015-2016 Pivotal Software, Inc.
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

add_subdirectory(execution_generator)
add_subdirectory(logical_generator)
add_subdirectory(physical_generator)
add_subdirectory(resolver)

add_library(quickstep_queryoptimizer_tests_OptimizerTest OptimizerTest.cpp OptimizerTest.hpp)
add_library(quickstep_queryoptimizer_tests_TestDatabaseLoader TestDatabaseLoader.cpp TestDatabaseLoader.hpp)

target_link_libraries(quickstep_queryoptimizer_tests_OptimizerTest
                      gtest
                      quickstep_catalog_Catalog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_BinaryExpression
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_logical_TableReference
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_tests_TestDatabaseLoader
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_queryexecution_QueryExecutionMessages_proto
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_storage_InsertDestination
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_types_CharType
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_containers_Tuple
                      quickstep_utility_Macros
                      tmb)

add_executable(quickstep_queryoptimizer_tests_ExecutionGeneratorTest
               ExecutionGeneratorTest.cpp
               ExecutionGeneratorTestRunner.cpp
               ExecutionGeneratorTestRunner.hpp
               "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.cpp"
               "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.hpp")
add_executable(quickstep_queryoptimizer_tests_OptimizerTextTest
               OptimizerTextTest.cpp
               OptimizerTextTestRunner.cpp
               OptimizerTextTestRunner.hpp
               "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.cpp"
               "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.hpp")

target_link_libraries(quickstep_queryoptimizer_tests_ExecutionGeneratorTest
                      glog
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogDatabase
                      quickstep_cli_DropRelation
                      quickstep_cli_PrintToScreen
                      quickstep_parser_ParseStatement
                      quickstep_parser_SqlParserWrapper
                      quickstep_queryexecution_Foreman
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_Worker
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_queryexecution_WorkerMessage
                      quickstep_queryoptimizer_ExecutionGenerator
                      quickstep_queryoptimizer_LogicalGenerator
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_tests_TestDatabaseLoader
                      quickstep_threading_ThreadIDBasedMap
                      quickstep_utility_Macros
                      quickstep_utility_MemStream
                      quickstep_utility_SqlError
                      quickstep_utility_TextBasedTestDriver
                      tmb
                      ${LIBS})
target_link_libraries(quickstep_queryoptimizer_tests_OptimizerTextTest
                      glog
                      gtest
                      gtest_main
                      quickstep_parser_ParseStatement
                      quickstep_parser_SqlParserWrapper
                      quickstep_queryoptimizer_LogicalGenerator
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_resolver_Resolver
                      quickstep_queryoptimizer_tests_TestDatabaseLoader
                      quickstep_utility_Macros
                      quickstep_utility_SqlError
                      quickstep_utility_TextBasedTestDriver
                      ${LIBS})
