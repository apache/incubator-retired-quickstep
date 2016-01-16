# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# All rights reserved.
# See file CREDITS.txt for details.

add_executable(quickstep_parser_tests_ParserTest
              "${CMAKE_CURRENT_SOURCE_DIR}/ParserTest.cpp"
              "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.cpp"
              "${PROJECT_SOURCE_DIR}/utility/textbased_test/TextBasedTest.hpp")

target_link_libraries(quickstep_parser_tests_ParserTest
                      glog
                      gtest
                      gtest_main
                      quickstep_parser
                      quickstep_utility_Macros
                      quickstep_utility_TextBasedTestDriver)

add_test(quickstep_parser_tests_ParserTest_aggregate
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Aggregate.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Aggregate.test")
add_test(quickstep_parser_tests_ParserTest_alter
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Alter.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Alter.test")
add_test(quickstep_parser_tests_ParserTest_copy
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Copy.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Copy.test")
add_test(quickstep_parser_tests_ParserTest_create
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Create.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Create.test")
add_test(quickstep_parser_tests_ParserTest_delete
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Delete.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Delete.test")
add_test(quickstep_parser_tests_ParserTest_drop
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Drop.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Drop.test")
add_test(quickstep_parser_tests_ParserTest_insert
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Insert.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Insert.test")
add_test(quickstep_parser_tests_ParserTest_select
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Select.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Select.test")
add_test(quickstep_parser_tests_ParserTest_tpch
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/TPCH.test"
         "${CMAKE_CURRENT_BINARY_DIR}/TPCH.test")
add_test(quickstep_parser_tests_ParserTest_update
         quickstep_parser_tests_ParserTest
         "${CMAKE_CURRENT_SOURCE_DIR}/Update.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Update.test")
