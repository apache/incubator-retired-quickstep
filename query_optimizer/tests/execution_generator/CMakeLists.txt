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

if (ENABLE_DISTRIBUTED)
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_create
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Create.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedCreate.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedCreate/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_delete
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Delete.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedDelete.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedDelete/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_distinct
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Distinct.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedDistinct.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedDistinct/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_drop
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Drop.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedDrop.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedDrop/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_index
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Index.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedIndex.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedIndex/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_insert
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Insert.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedInsert.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedInsert/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_join
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Join.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedJoin.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedJoin/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_select
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Select.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedSelect.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedSelect/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_stringpatternmatching
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/StringPatternMatching.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedStringPatternMatching.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedStringPatternMatching/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_tablegenerator
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/TableGenerator.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedTableGenerator.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedTableGenerator/")
  add_test(quickstep_queryoptimizer_tests_distributed_executiongenerator_update
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Update.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedUpdate.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistributedUpdate/")
endif(ENABLE_DISTRIBUTED)
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
add_test(quickstep_queryoptimizer_tests_executiongenerator_partition
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Partition.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Partition.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Partition/")
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
if (ENABLE_DISTRIBUTED)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedCreate)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedDelete)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedDistinct)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedDrop)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedIndex)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedInsert)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedJoin)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedSelect)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedStringPatternMatching)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedTableGenerator)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistributedUpdate)
endif(ENABLE_DISTRIBUTED)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Drop)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Index)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Insert)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Join)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Partition)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Select)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatching)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/TableGenerator)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Update)
