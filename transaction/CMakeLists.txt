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

if (BUILD_SHARED_LIBS)
  set(GFLAGS_LIB_NAME gflags_nothreads-shared)
else()
  set(GFLAGS_LIB_NAME gflags_nothreads-static)
endif()

add_library(quickstep_transaction_AccessMode
            AccessMode.cpp
            AccessMode.hpp)
add_library(quickstep_transaction_CycleDetector
            CycleDetector.cpp
            CycleDetector.hpp)
add_library(quickstep_transaction_DeadLockDetector
            DeadLockDetector.cpp
            DeadLockDetector.cpp)
add_library(quickstep_transaction_DirectedGraph
            ../empty_src.cpp
            DirectedGraph.hpp)
add_library(quickstep_transaction_Lock
            ../empty_src.cpp
            Lock.hpp)
add_library(quickstep_transaction_LockManager
            LockManager.hpp
            LockManager.cpp)
add_library(quickstep_transaction_LockRequest
            ../empty_src.cpp
            LockRequest.hpp)
add_library(quickstep_transaction_LockTable
            LockTable.cpp
            LockTable.hpp)
add_library(quickstep_transaction_ResourceId
            ResourceId.cpp
            ResourceId.hpp)
add_library(quickstep_transaction_StronglyConnectedComponents
            StronglyConnectedComponents.cpp
            StronglyConnectedComponents.hpp)
add_library(quickstep_transaction_Transaction
            ../empty_src.cpp
            Transaction.hpp)
add_library(quickstep_transaction_TransactionTable
            TransactionTable.cpp
            TransactionTable.hpp)

target_link_libraries(quickstep_transaction_CycleDetector
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_StronglyConnectedComponents
                      quickstep_utility_Macros)
target_link_libraries(quickstep_transaction_DeadLockDetector
                      glog
                      quickstep_transaction_CycleDetector
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_LockTable
                      quickstep_transaction_Transaction)
target_link_libraries(quickstep_transaction_DirectedGraph
                      glog
                      quickstep_transaction_Transaction
                      quickstep_utility_Macros)
target_link_libraries(quickstep_transaction_Lock
                      quickstep_transaction_AccessMode
                      quickstep_transaction_ResourceId)
target_link_libraries(quickstep_transaction_LockManager
                      ${GFLAGS_LIB_NAME}
                      glog
                      quickstep_utility_ThreadSafeQueue
                      quickstep_threading_Thread
                      quickstep_transaction_AccessMode
                      quickstep_transaction_DeadLockDetector
                      quickstep_transaction_LockRequest
                      quickstep_transaction_LockTable
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction
                      quickstep_transaction_TransactionTable)
target_link_libraries(quickstep_transaction_LockRequest
                      quickstep_transaction_AccessMode
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction)
target_link_libraries(quickstep_transaction_LockTable
                      quickstep_threading_SharedMutex
                      quickstep_transaction_AccessMode
                      quickstep_transaction_Lock
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction
                      quickstep_utility_Macros)
target_link_libraries(quickstep_transaction_ResourceId
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_HashPair)
target_link_libraries(quickstep_transaction_ResourceId
                      glog)
target_link_libraries(quickstep_transaction_StronglyConnectedComponents
                      quickstep_transaction_DirectedGraph
                      quickstep_utility_Macros)
target_link_libraries(quickstep_transaction_TransactionTable
                      quickstep_transaction_AccessMode
                      quickstep_transaction_Lock
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction
                      quickstep_utility_Macros)

add_library(quickstep_transaction
            ../empty_src.cpp
            TransactionModule.hpp)
target_link_libraries(quickstep_transaction
                      quickstep_transaction_AccessMode
                      quickstep_transaction_CycleDetector
                      quickstep_transaction_DeadLockDetector
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_Lock
                      quickstep_transaction_LockManager
                      quickstep_transaction_LockRequest
                      quickstep_transaction_LockTable
                      quickstep_transaction_ResourceId
                      quickstep_transaction_StronglyConnectedComponents
                      quickstep_transaction_Transaction
                      quickstep_transaction_TransactionTable)

add_executable(AccessMode_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/AccessMode_unittest.cpp")
target_link_libraries(AccessMode_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_AccessMode)
add_test(AccessMode_unittest AccessMode_unittest)

add_executable(CycleDetector_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/CycleDetector_unittest.cpp")
target_link_libraries(CycleDetector_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_CycleDetector
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_StronglyConnectedComponents)
add_test(CycleDetector_unittest CycleDetector_unittest)

add_executable(DeadLockDetector_unittest
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/DeadLockDetector_unittest.cpp")
target_link_libraries(DeadLockDetector_unittest
                      gtest
                      gtest_main
                      quickstep_threading_Thread
                      quickstep_transaction_AccessMode
                      quickstep_transaction_DeadLockDetector
                      quickstep_transaction_LockTable
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction)
add_test(DeadLockDetector_unittest DeadLockDetector_unittest)

add_executable(DirectedGraph_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/DirectedGraph_unittest.cpp")
target_link_libraries(DirectedGraph_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_Transaction)
add_test(DirectedGraph_unittest DirectedGraph_unittest)

add_executable(Lock_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/Lock_unittest.cpp")
target_link_libraries(Lock_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_AccessMode
                      quickstep_transaction_Lock
                      quickstep_transaction_ResourceId)
add_test(Lock_unittest Lock_unittest)

add_executable(LockRequest_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/LockRequest_unittest.cpp")
target_link_libraries(LockRequest_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_AccessMode
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction)
add_test(LockRequest_unittest LockRequest_unittest)

add_executable(LockTable_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/LockTable_unittest.cpp")
target_link_libraries(LockTable_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_AccessMode
                      quickstep_transaction_LockTable
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction)
add_test(LockTable_unittest LockTable_unittest)

add_executable(ResourceId_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/ResourceId_unittest.cpp")
target_link_libraries(ResourceId_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_ResourceId)
add_test(ResourceId_unittest ResourceId_unittest)

add_executable(StronglyConnectedComponents_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/StronglyConnectedComponents_unittest.cpp")
target_link_libraries(StronglyConnectedComponents_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_StronglyConnectedComponents)
add_test(StronglyConnectedComponents_unittest StronglyConnectedComponents_unittest)

add_executable(TransactionTable_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/TransactionTable_unittest.cpp")
target_link_libraries(TransactionTable_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_AccessMode
                      quickstep_transaction_ResourceId
                      quickstep_transaction_Transaction
                      quickstep_transaction_TransactionTable)
add_test(TransactionTable_unittest TransactionTable_unittest)
