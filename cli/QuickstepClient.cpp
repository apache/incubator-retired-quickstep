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
#include <memory>
#include <string>
#include <vector>

#include "cli/SingleNodeServer.grpc.pb.h"
#include "cli/SingleNodeServer.pb.h"

#include "gflags/gflags.h"
#include "glog/logging.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

static bool ValidatePort(const char *flagname, std::int32_t value) {
  if (value > 0 && value < 65536) {
    return true;
  }
  std::printf("Invalid value for --%s: %d\n", flagname, static_cast<int>(value));
  return false;
}

DEFINE_int32(port, 3000, "Listens for TCP connection on this port.");
DEFINE_validator(port, &ValidatePort);

namespace quickstep {

class QuickstepClient {
 public:
  explicit QuickstepClient(std::shared_ptr<Channel> const &channel)
      : stub_(SingleNodeServerRequest::NewStub(channel)) {}

  /**
   * Assembles the client's payload, sends it and presents the response back from the server.
   * @param user_query A SQL statement or command to be executed on the server.
   * @return The text of the server's response.
   */
  std::string SendQuery(const std::string &user_query) {
    QueryRequest request;
    request.set_query(user_query);
    QueryResponse response;
    ClientContext context;

    Status status = stub_->SendQuery(&context, request, &response);

    if (status.ok()) {
      return HandleQueryResponse(response);
    } else {
      LOG(WARNING) << "RPC call failed with code " << status.error_code()
                   << " and message: " << status.error_message();
      return "RPC failed";
    }
  }

 private:
  /**
   * Handle a valid response from the server.
   * @param response A valid query response.
   * @return The response string.
   */
  std::string HandleQueryResponse(QueryResponse const &response) const {
    if (response.error()) {
      return response.error_result();
    }
    return response.query_result();
  }

  std::unique_ptr<SingleNodeServerRequest::Stub> stub_;
};

}  // namespace quickstep

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // Attempts to send a single query retrieved from stdin to the Quickstep Server.
  quickstep::QuickstepClient qs_client(
    grpc::CreateChannel("localhost:" + std::to_string(FLAGS_port),
                        grpc::InsecureChannelCredentials()));
  std::string user_query;
  std::cin >> user_query;
  std::cout << "query: " << user_query << std::endl;
  std::cout << qs_client.SendQuery(user_query) << std::endl;
  return 0;
}
