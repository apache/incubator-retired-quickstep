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

QS_PROTOBUF_GENERATE_CPP(queryexecution_QueryContext_proto_srcs queryexecution_QueryContext_proto_hdrs
                         QueryContext.proto)
QS_PROTOBUF_GENERATE_CPP(queryexecution_QueryExecutionMessages_proto_srcs
                         queryexecution_QueryExecutionMessages_proto_hdrs
                         QueryExecutionMessages.proto)

# Declare micro-libs:
add_library(quickstep_queryexecution_Foreman Foreman.cpp Foreman.hpp)
add_library(quickstep_queryexecution_ForemanLite ../empty_src.cpp ForemanLite.hpp)
add_library(quickstep_queryexecution_QueryContext QueryContext.cpp QueryContext.hpp)
add_library(quickstep_queryexecution_QueryContext_proto
            ${queryexecution_QueryContext_proto_srcs}
            ${queryexecution_QueryContext_proto_hdrs})
add_library(quickstep_queryexecution_QueryExecutionMessages_proto
            ${queryexecution_QueryExecutionMessages_proto_srcs}
            ${queryexecution_QueryExecutionMessages_proto_hdrs})
add_library(quickstep_queryexecution_QueryExecutionTypedefs ../empty_src.cpp QueryExecutionTypedefs.hpp)
add_library(quickstep_queryexecution_QueryExecutionUtil ../empty_src.cpp QueryExecutionUtil.hpp)
add_library(quickstep_queryexecution_WorkOrdersContainer WorkOrdersContainer.cpp WorkOrdersContainer.hpp)
add_library(quickstep_queryexecution_Worker Worker.cpp Worker.hpp)
add_library(quickstep_queryexecution_WorkerDirectory ../empty_src.cpp WorkerDirectory.hpp)
add_library(quickstep_queryexecution_WorkerMessage ../empty_src.cpp WorkerMessage.hpp)
add_library(quickstep_queryexecution_WorkerSelectionPolicy ../empty_src.cpp WorkerSelectionPolicy.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryexecution_Foreman
                      glog
                      gtest
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_PartitionScheme
                      quickstep_queryexecution_ForemanLite
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryExecutionMessages_proto
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_QueryExecutionUtil
                      quickstep_queryexecution_WorkOrdersContainer
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_queryexecution_WorkerMessage
                      quickstep_relationaloperators_RebuildWorkOrder
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_relationaloperators_WorkOrder
                      quickstep_storage_InsertDestination
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_threading_ThreadUtil
                      quickstep_utility_DAG
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryexecution_ForemanLite
                      glog
                      quickstep_threading_Thread
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryexecution_QueryContext
                      glog
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_ExpressionFactories
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_tablegenerator_GeneratorFunctionFactory
                      quickstep_expressions_tablegenerator_GeneratorFunctionHandle
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_storage_AggregationOperationState
                      quickstep_storage_HashTable
                      quickstep_storage_HashTableFactory
                      quickstep_storage_InsertDestination
                      quickstep_storage_InsertDestination_proto
                      quickstep_types_containers_Tuple
                      quickstep_utility_Macros
                      quickstep_utility_SortConfiguration)
target_link_libraries(quickstep_queryexecution_QueryContext_proto
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_tablegenerator_GeneratorFunction_proto
                      quickstep_storage_AggregationOperationState_proto
                      quickstep_storage_HashTable_proto
                      quickstep_storage_InsertDestination_proto
                      quickstep_types_containers_Tuple_proto
                      quickstep_utility_SortConfiguration_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_queryexecution_QueryExecutionMessages_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_threading_ThreadIDBasedMap
                      tmb)
target_link_libraries(quickstep_queryexecution_QueryExecutionUtil
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryexecution_WorkOrdersContainer
                      glog
                      quickstep_relationaloperators_WorkOrder
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_queryexecution_Worker
                      glog
                      quickstep_queryexecution_QueryExecutionMessages_proto
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_QueryExecutionUtil
                      quickstep_queryexecution_WorkerMessage
                      quickstep_relationaloperators_WorkOrder
                      quickstep_threading_Thread
                      quickstep_threading_ThreadIDBasedMap
                      quickstep_threading_ThreadUtil
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryexecution_WorkerDirectory
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryexecution_WorkerSelectionPolicy
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryexecution ../empty_src.cpp QueryExecutionModule.hpp)
target_link_libraries(quickstep_queryexecution
                      quickstep_queryexecution_Foreman
                      quickstep_queryexecution_ForemanLite
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_queryexecution_QueryExecutionMessages_proto
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_QueryExecutionUtil
                      quickstep_queryexecution_WorkOrdersContainer
                      quickstep_queryexecution_Worker
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_queryexecution_WorkerMessage
                      quickstep_queryexecution_WorkerSelectionPolicy)
# Tests:
add_executable(Foreman_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/Foreman_unittest.cpp")
target_link_libraries(Foreman_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_queryexecution_Foreman
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_WorkOrdersContainer
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_queryexecution_WorkerMessage
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_relationaloperators_WorkOrder
                      quickstep_storage_InsertDestination
                      quickstep_storage_InsertDestination_proto
                      quickstep_storage_StorageBlock
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageManager
                      quickstep_utility_DAG
                      quickstep_utility_Macros
                      tmb)
add_test(Foreman_unittest Foreman_unittest)

add_executable(WorkOrdersContainer_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/WorkOrdersContainer_unittest.cpp")
target_link_libraries(WorkOrdersContainer_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_queryexecution_WorkOrdersContainer
                      quickstep_relationaloperators_WorkOrder
                      quickstep_utility_Macros
                      quickstep_utility_PtrVector)
add_test(WorkOrdersContainer_unittest WorkOrdersContainer_unittest)

add_executable(WorkerDirectory_unittest
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/WorkerDirectory_unittest.cpp")
target_link_libraries(WorkerDirectory_unittest
                      gtest
                      gtest_main
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_WorkerDirectory)
add_test(WorkerDirectory_unittest WorkerDirectory_unittest)

add_executable(WorkerSelectionPolicy_unittest
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/WorkerSelectionPolicy_unittest.cpp")
target_link_libraries(WorkerSelectionPolicy_unittest
                      gtest
                      gtest_main
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryexecution_WorkerDirectory
                      quickstep_queryexecution_WorkerSelectionPolicy)
add_test(WorkerSelectionPolicy_unittest WorkerSelectionPolicy_unittest)
