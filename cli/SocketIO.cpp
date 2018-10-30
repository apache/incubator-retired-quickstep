/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "cli/SocketIO.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

DEFINE_int32(cli_socket_port, 3000,
             "Listens for TCP connections on this port when socket mode is enabled. "
             "This is only used if the cli is set to use the socket mode (-mode=socket).");

DEFINE_string(cli_socket_ip, "0.0.0.0",
              "The ip address which the cli should open a connection on. "
              "This is only used if the cli is set to use the network mode (-mode=socket). "
              "Defaults to the address of localhost.");

}  // namespace quickstep
