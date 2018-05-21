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

add_test(quickstep_cli_tests_commandexecutor_analyze
         "../quickstep_cli_tests_CommandExecutorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Analyze.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Analyze.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Analyze/")
add_test(quickstep_cli_tests_commandexecutor_d
         "../quickstep_cli_tests_CommandExecutorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/D.test"
         "${CMAKE_CURRENT_BINARY_DIR}/D.test"
         "${CMAKE_CURRENT_BINARY_DIR}/D/")
add_test(quickstep_cli_tests_commandexecutor_dt
         "../quickstep_cli_tests_CommandExecutorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Dt.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Dt.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Dt/")

if (ENABLE_DISTRIBUTED)
  add_test(quickstep_cli_tests_commandexecutor_analyze_distributed
           "../quickstep_cli_tests_CommandExecutorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Analyze.test"
           "${CMAKE_CURRENT_BINARY_DIR}/AnalyzeDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/AnalyzeDistributed/")
  add_test(quickstep_cli_tests_commandexecutor_d_distributed
           "../quickstep_cli_tests_DistributedCommandExecutorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/D.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DDistributed/")
  add_test(quickstep_cli_tests_commandexecutor_dt_distributed
           "../quickstep_cli_tests_DistributedCommandExecutorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Dt.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DtDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DtDistributed/")
endif(ENABLE_DISTRIBUTED)

# Create the folders where the unit tests will store their data blocks for the
# duration of their test.
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Analyze)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/D)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Dt)

if (ENABLE_DISTRIBUTED)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/AnalyzeDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DtDistributed)
endif(ENABLE_DISTRIBUTED)
