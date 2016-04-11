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

add_test(quickstep_cli_tests_commandexecutor_d
         "../quickstep_cli_tests_CommandExecutorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/D.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Dt.test"
         "${CMAKE_CURRENT_BINARY_DIR}/D/")
add_test(quickstep_cli_tests_commandexecutor_dt
         "../quickstep_cli_tests_CommandExecutorTest"
         "${CMAKE_CURRENT_SOURCE_DIR}/Dt.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Dt.test"
         "${CMAKE_CURRENT_BINARY_DIR}/Dt/")

# Create the folders where the unit tests will store their data blocks for the
# duration of their test.
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/D)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Dt)

