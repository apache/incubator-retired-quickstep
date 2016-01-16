# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# All rights reserved.
# See file CREDITS.txt for details.

add_test(quickstep_queryoptimizer_tests_logicalgenerator_select
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Select.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Select.test")
