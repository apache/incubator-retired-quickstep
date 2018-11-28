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

add_test(quickstep_queryoptimizer_tests_executiongenerator_create
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Create.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Create.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Create/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_delete
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Delete.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Delete.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Delete/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_distinct
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Distinct.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Distinct.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Distinct/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_drop
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Drop.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Drop.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Drop/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_index
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Index.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Index.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Index/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_insert
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Insert.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Insert.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Insert/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_join
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Join.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Join.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Join/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_select
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Select.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Select.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Select/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_stringpatternmatching
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/StringPatternMatching.test"
         "${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatching.test"
         "${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatching/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_tablegenerator
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/TableGenerator.test"
         "${CMAKE_CURRENT_BINARY_DIR}/TableGenerator.test"
         "${CMAKE_CURRENT_BINARY_DIR}/TableGenerator/")
add_test(quickstep_queryoptimizer_tests_executiongenerator_update
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Update.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Update.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Update/")

# Create the folders where the unit tests will store their data blocks for the
# duration of their test.
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Create)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Delete)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Distinct)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Drop)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Index)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Insert)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Join)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Select)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatching)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/TableGenerator)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Update)