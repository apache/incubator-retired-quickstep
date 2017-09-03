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

#include <grpc++/grpc++.h>

#include <iostream>
#include <istream>
#include <memory>
#include <string>

#include "cli/LineReaderBuffered.hpp"
#include "cli/NetworkCliClient.hpp"
#include "cli/NetworkIO.hpp"

#include "gflags/gflags.h"

using quickstep::LineReaderBuffered;
using quickstep::NetworkCliClient;

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  grpc_init();

  grpc::ChannelArguments grpc_args;
  grpc_args.SetMaxSendMessageSize(quickstep::FLAGS_cli_network_max_message_length);
  grpc_args.SetMaxReceiveMessageSize(quickstep::FLAGS_cli_network_max_message_length);

  // Attempts to send a single query retrieved from stdin to the Quickstep Server.
  NetworkCliClient qs_client(
    grpc::CreateCustomChannel(quickstep::NetworkIO::GetAddress(),
                              grpc::InsecureChannelCredentials(),
                              grpc_args));

  // Read stdin until EOF, then we use a Line reader to divide query into parts.
  std::cin >> std::noskipws;
  std::istream_iterator<char> it(std::cin), end;
  std::string input(it, end);

  if (input.empty()) {
    return 0;
  }

  // Temporary hack for transmitting data in a separate channel ...
  std::string queries;
  std::string data;
  if (input.front() == 0) {
    const std::size_t r = input.find(static_cast<char>(0), 1);
    queries = input.substr(1, r - 1);
    data = input.substr(r + 1);
  } else {
    queries = std::move(input);
  }

  LineReaderBuffered linereader;
  linereader.setBuffer(queries);
  while (!linereader.bufferEmpty()) {
    std::string query = linereader.getNextCommand();
    if (!query.empty()) {
      std::cout << qs_client.Query(query, data) << std::endl;
    }
  }
  return 0;
}
