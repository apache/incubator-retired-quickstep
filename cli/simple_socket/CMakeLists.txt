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

# Declare micro-libs:
add_library(quickstep_cli_simplesocket_SimpleSocketConnection ../../empty_src.cpp SimpleSocketConnection.hpp)
add_library(quickstep_cli_simplesocket_SimpleSocketContent ../../empty_src.cpp SimpleSocketContent.hpp)
add_library(quickstep_cli_simplesocket_SimpleSocketServer ../../empty_src.cpp SimpleSocketServer.hpp)

# Link dependencies:
target_link_libraries(quickstep_cli_simplesocket_SimpleSocketConnection
                      glog
                      quickstep_cli_simplesocket_SimpleSocketContent
                      quickstep_utility_Macros)
target_link_libraries(quickstep_cli_simplesocket_SimpleSocketContent
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_cli_simplesocket_SimpleSocketServer
                      glog
                      quickstep_cli_simplesocket_SimpleSocketConnection
                      quickstep_utility_Macros
                      quickstep_utility_ThreadSafeQueue)
