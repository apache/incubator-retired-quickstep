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
if (ENABLE_DISTRIBUTED)
  add_executable(quickstep_cli_tests_DistributedCommandExecutorTest
                 DistributedCommandExecutorTest.cpp
                 DistributedCommandExecutorTestRunner.cpp
                 DistributedCommandExecutorTestRunner.hpp
                 "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.cpp"
                 "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.hpp")
endif(ENABLE_DISTRIBUTED)
add_executable(LineReaderBuffered_unittest
               LineReaderBuffered_unittest.cpp)
if (ENABLE_NETWORK_CLI)
  set(CMAKE_MODULE_PATH
      ${CMAKE_MODULE_PATH}
      "${PROJECT_SOURCE_DIR}/third_party/src/tmb/cmake")

  find_package(Grpc++ REQUIRED)

  add_executable(NetworkIO_unittest
                 NetworkIO_unittest.cpp)
endif()

target_link_libraries(quickstep_cli_tests_CommandExecutorTest
                      glog
                      gtest
                      quickstep_catalog_CatalogDatabase
                      quickstep_cli_CommandExecutor
                      quickstep_cli_DefaultsConfigurator
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
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryProcessor
                      quickstep_storage_StorageConstants
                      quickstep_utility_Macros
                      quickstep_utility_MemStream
                      quickstep_utility_SqlError
                      quickstep_utility_TextBasedTestDriver
                      tmb
                      ${LIBS})
if (ENABLE_DISTRIBUTED)
  target_link_libraries(quickstep_cli_tests_DistributedCommandExecutorTest
                        glog
                        gtest
                        quickstep_catalog_CatalogTypedefs
                        quickstep_cli_CommandExecutorUtil
                        quickstep_cli_Constants
                        quickstep_cli_DropRelation
                        quickstep_cli_PrintToScreen
                        quickstep_parser_ParseStatement
                        quickstep_parser_SqlParserWrapper
                        quickstep_queryexecution_BlockLocator
                        quickstep_queryexecution_BlockLocatorUtil
                        quickstep_queryexecution_ForemanDistributed
                        quickstep_queryexecution_QueryExecutionTypedefs
                        quickstep_queryexecution_QueryExecutionUtil
                        quickstep_queryexecution_Shiftboss
                        quickstep_queryexecution_Worker
                        quickstep_queryexecution_WorkerDirectory
                        quickstep_queryoptimizer_Optimizer
                        quickstep_queryoptimizer_OptimizerContext
                        quickstep_queryoptimizer_QueryHandle
                        quickstep_queryoptimizer_tests_TestDatabaseLoader
                        quickstep_storage_DataExchangerAsync
                        quickstep_storage_StorageManager
                        quickstep_utility_Macros
                        quickstep_utility_MemStream
                        quickstep_utility_SqlError
                        quickstep_utility_TextBasedTestDriver
                        tmb
                        ${GFLAGS_LIB_NAME}
                        ${LIBS})
endif(ENABLE_DISTRIBUTED)

target_link_libraries(LineReaderBuffered_unittest
                      ${GFLAGS_LIB_NAME}
                      glog
                      gtest
                      quickstep_cli_LineReaderBuffered)
add_test(LineReaderBuffered_unittest LineReaderBuffered_unittest)

if (ENABLE_NETWORK_CLI)
  target_link_libraries(NetworkIO_unittest
                        ${GFLAGS_LIB_NAME}
                        ${GRPCPLUSPLUS_LIBRARIES}
                        glog
                        gtest
                        quickstep_cli_Flags
                        quickstep_cli_IOInterface
                        quickstep_cli_LineReaderBuffered
                        quickstep_cli_NetworkCliClient
                        quickstep_cli_NetworkIO
                        quickstep_threading_Thread
                        quickstep_utility_Macros)
  add_test(NetworkIO_unittest NetworkIO_unittest)
endif()
