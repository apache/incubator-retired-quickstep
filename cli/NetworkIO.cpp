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

#include "cli/NetworkIO.hpp"

#include <grpc++/grpc++.h>

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

static bool ValidatePort(const char *flagname, std::int32_t value) {
  int const min = 0, max = 65536;
  if (value > min && value < max) {
    return true;
  }
  std::cout << "Invalid value for --" << flagname << ": " << value
            << "\nUse ports between " << min << " and "
            << max << std::endl;
  return false;
}

DEFINE_int32(cli_network_port, 3000,
             "Listens for TCP connections on this port when network mode is enabled. "
               "This is only used if the cli is set to use the network mode (--mode=network).");
DEFINE_validator(cli_network_port, &ValidatePort);

DEFINE_string(cli_network_ip, "0.0.0.0",
              "The ip address which the cli should open a connection on. This is only used "
                "if the cli is set to use the network mode (--mode=network). Defaults to "
                "the address of localhost.");

DEFINE_int32(cli_network_max_message_length, 1073741824,
             "The maximum message length transferred through grpc");

NetworkIO::NetworkIO() {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(GetAddress(), grpc::InsecureServerCredentials());
  builder.RegisterService(&service_);
  builder.SetMaxReceiveMessageSize(FLAGS_cli_network_max_message_length);
  builder.SetMaxSendMessageSize(FLAGS_cli_network_max_message_length);
  server_ = builder.BuildAndStart();
  LOG(INFO) << "Listening on " << GetAddress();
}

}  // namespace quickstep
