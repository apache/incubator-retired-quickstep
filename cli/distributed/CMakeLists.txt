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

set_gflags_lib_name ()

# Declare micro-libs and link dependencies:
add_library(quickstep_cli_distributed_Conductor Conductor.cpp Conductor.hpp)
add_library(quickstep_cli_distributed_Executor Executor.cpp Executor.hpp)
add_library(quickstep_cli_distributed_Role Role.cpp Role.hpp)

# Link dependencies:
target_link_libraries(quickstep_cli_distributed_Conductor
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_cli_CommandExecutorUtil
                      quickstep_cli_Constants
                      quickstep_cli_DefaultsConfigurator
                      quickstep_cli_Flags
                      quickstep_cli_distributed_Role
                      quickstep_expressions_aggregation_AggregateFunctionMax
                      quickstep_expressions_aggregation_AggregateFunctionMin
                      quickstep_parser_ParseStatement
                      quickstep_parser_ParseString
                      quickstep_parser_SqlParserWrapper
                      quickstep_queryexecution_BlockLocator
                      quickstep_queryexecution_ForemanDistributed
                      quickstep_queryexecution_QueryExecutionMessages_proto
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_QueryExecutionUtil
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryProcessor
                      quickstep_storage_StorageConstants
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector
                      quickstep_utility_SqlError
                      quickstep_utility_StringUtil
                      tmb)
target_link_libraries(quickstep_cli_distributed_Executor
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_cli_Flags
                      quickstep_cli_distributed_Role
                      quickstep_queryexecution_BlockLocatorUtil
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_Shiftboss
                      quickstep_queryexecution_Worker
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_storage_DataExchangerAsync
                      quickstep_storage_StorageManager
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_cli_distributed_Role
                      quickstep_utility_Macros
                      tmb
                      ${GFLAGS_LIB_NAME})

# Module all-in-one library:
add_library(quickstep_cli_distributed ../../empty_src.cpp CliDistributedModule.hpp)

target_link_libraries(quickstep_cli_distributed
                      quickstep_cli_distributed_Conductor
                      quickstep_cli_distributed_Executor
                      quickstep_cli_distributed_Role)
