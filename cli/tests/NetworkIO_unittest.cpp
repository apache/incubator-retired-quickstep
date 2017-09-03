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

#include <cstddef>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "cli/LineReaderBuffered.hpp"
#include "cli/NetworkCliClient.hpp"
#include "cli/NetworkIO.hpp"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {

using networkio_internal::RequestState;

static std::string const kQueryRequest = "O Captain! My Captain!";
static std::string const kQueryResponse = "Our fearful trip is done,";

/**
 * Contains a server instance for testing.
 */
class TestNetworkIO {
 public:
  TestNetworkIO() : server_address_(NetworkIO::GetAddress()) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    server_ = builder.BuildAndStart();
    CHECK(server_) << "Failed to start server";
    LOG(INFO) << "TestSingleNodeServer listening on " << server_address_;
  }

  ~TestNetworkIO() {
    service_.kill();
    server_->Shutdown();
    server_->Wait();
  }

  /**
   * @brief Waits on the service for a sent message.
   */
  std::string getSentMessage() {
    CHECK(current_request_ == nullptr);
    current_request_ = service_.waitForRequest();
    EXPECT_EQ(current_request_->getCanceled(), false);
    return current_request_->getRequest().query();
  }

  /**
   * @brief Sets the response message of the Service worker. Alerts it that the request is ready.
   */
  void setResponse(std::string response) {
    CHECK_NOTNULL(current_request_);
    current_request_->responseReady(response, std::string(""));
    current_request_ = nullptr;
  }

  NetworkCliServiceImpl& getService() {
    return service_;
  }

 private:
  NetworkCliServiceImpl service_;
  std::string server_address_;
  std::unique_ptr<grpc::Server> server_;
  RequestState* current_request_;

  DISALLOW_COPY_AND_ASSIGN(TestNetworkIO);
};

/**
 * We will pass this thread a lambda based on the desired server interactions.
 */
class HelperThread : public Thread {
 public:
  explicit HelperThread(std::function<void(void)> function) : function_(function) {}

 protected:
  void run() override {
    function_();
  }

 private:
  std::function<void(void)> function_;

  DISALLOW_COPY_AND_ASSIGN(HelperThread);
};

/**
 * Tests a simple call and response to the Service.
 */
TEST(NetworkIOTest, TestNetworkIOCommandInteraction) {
  NetworkIO networkIO;

  // This thread will handle the response from the client in a similar way as the quickstep cli will.
  HelperThread server_handler([&networkIO]() {
    std::unique_ptr<IOHandle> command(networkIO.getNextIOHandle());
    EXPECT_EQ(command->getCommand(), kQueryRequest);

    // Set some output for the main test thread, destruction of the handle will return the request.
    fprintf(command->out(), "%s", kQueryResponse.c_str());
  });
  server_handler.start();

  NetworkCliClient client(
    grpc::CreateChannel(NetworkIO::GetAddress(),
                        grpc::InsecureChannelCredentials()));
  QueryRequest request;
  request.set_query(kQueryRequest);
  QueryResponse response;
  Status status = client.SendQuery(request, &response);
  ASSERT_TRUE(status.ok());
  EXPECT_EQ(kQueryResponse, response.query_result());
  EXPECT_TRUE(response.error_result().empty());

  server_handler.join();
}

/**
 * Tests that killing the service will cancel requests.
 */
TEST(NetworkIOTest, TestShutdown) {
  // Start a server:
  NetworkIO networkIO;

  std::function<void(void)> send_request_fn([]() {
    // Create a request, and, on return it should be canceled.
    NetworkCliClient client(grpc::CreateChannel(NetworkIO::GetAddress(),
                            grpc::InsecureChannelCredentials()));
    QueryRequest request;
    request.set_query(kQueryRequest);
    QueryResponse response;
    Status status = client.SendQuery(request, &response);
    EXPECT_EQ(grpc::OK, status.error_code());

    // Server will kill the next request.
    status = client.SendQuery(request, &response);
    EXPECT_EQ(grpc::CANCELLED, status.error_code());
  });

  HelperThread client_thread(send_request_fn);
  client_thread.start();

  {
    std::unique_ptr<IOHandle> ioHandle(networkIO.getNextIOHandle());
    EXPECT_EQ(ioHandle->getCommand(), kQueryRequest);
    // Killing the service should cause the response the client thread receives to be canceled.
    networkIO.killService();
  }

  client_thread.join();
}

}  // namespace quickstep

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  grpc_init();
  return RUN_ALL_TESTS();
}
