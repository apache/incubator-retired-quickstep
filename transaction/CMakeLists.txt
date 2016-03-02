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
                      quickstep_transaction_StronglyConnectedComponents)

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
