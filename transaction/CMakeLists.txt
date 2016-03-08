#   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
#     University of Wisconsin—Madison.
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

add_library(quickstep_transaction_DirectedGraph
            ../empty_src.cpp
            DirectedGraph.hpp)
add_library(quickstep_transaction_StronglyConnectedComponents
            StronglyConnectedComponents.cpp
            StronglyConnectedComponents.hpp)
add_library(quickstep_transaction_Transaction
            Transaction.cpp
            Transaction.hpp)

target_link_libraries(quickstep_transaction_DirectedGraph
                      quickstep_transaction_Transaction
                      quickstep_utility_Macros)
target_link_libraries(quickstep_transaction_StronglyConnectedComponents
                      quickstep_transaction_DirectedGraph
                      quickstep_utility_Macros)

add_library(quickstep_transaction
            ../empty_src.cpp
            TransactionModule.hpp)
target_link_libraries(quickstep_transaction
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_StronglyConnectedComponents
                      quickstep_transaction_Transaction)

add_executable(DirectedGraph_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/DirectedGraph_unittest.cpp")
target_link_libraries(DirectedGraph_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_Transaction)
add_test(DirectedGraph_unittest DirectedGraph_unittest)

add_executable(StronglyConnectedComponents_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/StronglyConnectedComponents_unittest.cpp")
target_link_libraries(StronglyConnectedComponents_unittest
                      gtest
                      gtest_main
                      quickstep_transaction_DirectedGraph
                      quickstep_transaction_StronglyConnectedComponents)
add_test(StronglyConnectedComponents_unittest StronglyConnectedComponents_unittest)
