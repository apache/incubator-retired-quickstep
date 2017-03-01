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

add_subdirectory(command_executor)

add_executable(quickstep_cli_tests_CommandExecutorTest
               CommandExecutorTest.cpp
               CommandExecutorTestRunner.cpp
               CommandExecutorTestRunner.hpp
               "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.cpp"
               "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.hpp")

target_link_libraries(quickstep_cli_tests_CommandExecutorTest
                      glog
                      gtest
                      quickstep_catalog_CatalogDatabase
                      quickstep_cli_CommandExecutor
                      quickstep_cli_DropRelation
                      quickstep_cli_PrintToScreen
                      quickstep_parser_ParseStatement
                      quickstep_parser_SqlParserWrapper
                      quickstep_queryexecution_AdmitRequestMessage
                      quickstep_queryexecution_ForemanSingleNode
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_QueryExecutionUtil
                      quickstep_queryexecution_Worker
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_queryoptimizer_Optimizer
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_tests_TestDatabaseLoader
                      quickstep_utility_Macros
                      quickstep_utility_MemStream
                      quickstep_utility_SqlError
                      quickstep_utility_TextBasedTestDriver
                      tmb
                      ${LIBS})
