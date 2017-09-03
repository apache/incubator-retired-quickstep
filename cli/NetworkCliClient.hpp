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

#ifndef QUICKSTEP_CLI_SINGLE_NODE_CLIENT_HPP_
#define QUICKSTEP_CLI_SINGLE_NODE_CLIENT_HPP_

#include <grpc++/grpc++.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cli/NetworkCli.grpc.pb.h"
#include "cli/NetworkCli.pb.h"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

namespace quickstep {

/**
 * A simple wrapper class used to do CLI interactions with QuickstepCLI via the gRPC interface.
 */
class NetworkCliClient {
 public:
  explicit NetworkCliClient(const std::shared_ptr<Channel> &channel)
      : stub_(NetworkCli::NewStub(channel)) {}

  /**
   * Assembles the client's payload, sends it and presents the response back from the server.
   * @param user_query A SQL statement or command to be executed on the server.
   * @return The text of the server's response.
   */
  std::string Query(const std::string &user_query, const std::string &data) {
    QueryRequest request;
    request.set_query(user_query);
    request.set_data(data);
    QueryResponse response;

    Status status = SendQuery(request, &response);

    if (status.ok()) {
      return HandleQueryResponse(response);
    } else {
      std::cout << "RPC call failed with code " << status.error_code()
                << " and message: " << status.error_message() << "\n";
      return "RPC failed";
    }
  }

  Status SendQuery(const QueryRequest& request, QueryResponse* response) {
    ClientContext context;
    return stub_->SendQuery(&context, request, response);
  }

 private:
  /**
   * Handle a valid response from the server.
   * @param response A valid query response.
   * @return The response string.
   */
  std::string HandleQueryResponse(QueryResponse const &response) const {
    return response.query_result() + response.error_result();
  }

  std::unique_ptr<NetworkCli::Stub> stub_;

  DISALLOW_COPY_AND_ASSIGN(NetworkCliClient);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_SINGLE_NODE_CLIENT_HPP_
