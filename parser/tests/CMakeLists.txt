#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015 Pivotal Software, Inc.
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
