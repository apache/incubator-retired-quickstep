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

add_test(quickstep_queryoptimizer_tests_logicalgenerator_create
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Create.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Create.test")
add_test(quickstep_queryoptimizer_tests_logicalgenerator_index
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Index.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Index.test")
add_test(quickstep_queryoptimizer_tests_logicalgenerator_join
         "../quickstep_queryoptimizer_tests_OptimizerTextTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Join.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Join.test")
add_test(quickstep_queryoptimizer_tests_logicalgenerator_select
        "../quickstep_queryoptimizer_tests_OptimizerTextTest"
        "${CMAKE_CURRENT_SOURCE_DIR}/Select.test"
        "${CMAKE_CURRENT_BINARY_DIR}/Select.test")
