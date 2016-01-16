# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# All rights reserved.
# See file CREDITS.txt for details.

add_test(quickstep_queryoptimizer_tests_physicalgenerator_copy
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Copy.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Copy.test")
add_test(quickstep_queryoptimizer_tests_physicalgenerator_create
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Create.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Create.test")
add_test(quickstep_queryoptimizer_tests_physicalgenerator_delete
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Delete.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Delete.test")
add_test(quickstep_queryoptimizer_tests_physicalgenerator_drop
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Drop.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Drop.test")
add_test(quickstep_queryoptimizer_tests_physicalgenerator_insert
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Insert.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Insert.test")
add_test(quickstep_queryoptimizer_tests_physicalgenerator_select
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Select.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Select.test")
add_test(quickstep_queryoptimizer_tests_physicalgenerator_update
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Update.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Update.test")
