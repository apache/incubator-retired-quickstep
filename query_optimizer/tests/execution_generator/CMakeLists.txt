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

add_test(quickstep_queryoptimizer_tests_executiongenerator_commonsubexpression
         "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/CommonSubexpression.test"
         "${CMAKE_CURRENT_BINARY_DIR}/CommonSubexpression.test"
         "${CMAKE_CURRENT_BINARY_DIR}/CommonSubexpression/")
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

if (ENABLE_DISTRIBUTED)
  add_test(quickstep_queryoptimizer_tests_executiongenerator_commonsubexpression_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/CommonSubexpression.test"
           "${CMAKE_CURRENT_BINARY_DIR}/CommonSubexpressionDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/CommonSubexpressionDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_create_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Create.test"
           "${CMAKE_CURRENT_BINARY_DIR}/CreateDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/CreateDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_delete_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Delete.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DeleteDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DeleteDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_distinct_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Distinct.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistinctDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DistinctDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_drop_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Drop.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DropDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/DropDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_index_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Index.test"
           "${CMAKE_CURRENT_BINARY_DIR}/IndexDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/IndexDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_insert_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Insert.test"
           "${CMAKE_CURRENT_BINARY_DIR}/InsertDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/InsertDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_join_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Join.test"
           "${CMAKE_CURRENT_BINARY_DIR}/JoinDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/JoinDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_partition_distributed
           "../quickstep_queryoptimizer_tests_ExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Partition.test"
           "${CMAKE_CURRENT_BINARY_DIR}/PartitionDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/PartitionDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_select_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Select.test"
           "${CMAKE_CURRENT_BINARY_DIR}/SelectDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/SelectDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_stringpatternmatching_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/StringPatternMatching.test"
           "${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatchingDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatchingDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_tablegenerator_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/TableGenerator.test"
           "${CMAKE_CURRENT_BINARY_DIR}/TableGeneratorDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/TableGeneratorDistributed/")
  add_test(quickstep_queryoptimizer_tests_executiongenerator_update_distributed
           "../quickstep_queryoptimizer_tests_DistributedExecutionGeneratorTest"
           "${CMAKE_CURRENT_SOURCE_DIR}/Update.test"
           "${CMAKE_CURRENT_BINARY_DIR}/UpdateDistributed.test"
           "${CMAKE_CURRENT_BINARY_DIR}/UpdateDistributed/")
endif(ENABLE_DISTRIBUTED)

# Create the folders where the unit tests will store their data blocks for the
# duration of their test.
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CommonSubexpression)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Create)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Delete)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Distinct)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Drop)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Index)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Insert)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Join)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Partition)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Select)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatching)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/TableGenerator)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Update)

if (ENABLE_DISTRIBUTED)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CommonSubexpressionDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CreateDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DeleteDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DistinctDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DropDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/IndexDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/InsertDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/JoinDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/PartitionDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/SelectDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/StringPatternMatchingDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/TableGeneratorDistributed)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/UpdateDistributed)
endif(ENABLE_DISTRIBUTED)
