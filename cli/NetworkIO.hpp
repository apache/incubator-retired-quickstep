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

#ifndef QUICKSTEP_CLI_NETWORK_IO_HPP_
#define QUICKSTEP_CLI_NETWORK_IO_HPP_

#include <grpc++/grpc++.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

#include "cli/IOInterface.hpp"
#include "cli/NetworkCli.grpc.pb.h"
#include "cli/NetworkCli.pb.h"
#include "threading/ConditionVariable.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/Macros.hpp"
#include "utility/MemStream.hpp"
#include "utility/ThreadSafeQueue.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::Status;

namespace quickstep {
DECLARE_int32(cli_network_port);
DECLARE_string(cli_network_ip);
DECLARE_int32(cli_network_max_message_length);
namespace networkio_internal {

/**
 * Contains state and helper methods for managing interactions between a producer/consumer thread. A producer thread
 * will wait on a condition variable. When the consumer thread returns, it will notify the producer.
 */
class RequestState {
 public:
  explicit RequestState(const QueryRequest& request)
      : response_ready_(false),
        canceled_(false),
        request_(request),
        condition_(mutex_.createConditionVariable()) {}

  /**
   * @brief Notifies that the consumer has finished consuming and that a response is ready.
   * To be called after the consumer has executed.
   * @param stdout_str Stdout from Quickstep.
   * @param stderr_str Stderr from Quickstep.
   */
  void responseReady(const std::string& stdout_str, const std::string& stderr_str) {
    std::unique_lock<Mutex> lock(mutex_);
    response_message_.set_query_result(stdout_str);
    response_message_.set_error_result(stderr_str);
    response_ready_ = true;
    condition_->signalOne();
  }

  /**
   * @brief The producer thread blocks until Quickstep signals that it has finished.
   * @note Quickstep may either produce a query response or cancel. Both these actions must notify the condition.
   */
  void waitForResponse() {
    while (!response_ready_) {
      condition_->await();
    }
  }

  /**
   * @brief Notifies the producer that its request will not be served by Quickstep.
   */
  void cancel() {
    std::unique_lock<Mutex> lock(mutex_);
    canceled_ = true;
    response_ready_ = true;
    condition_->signalOne();
  }

  /**
   * @return The producer's request for Quickstep to process.
   */
  const QueryRequest& getRequest() const {
    return request_;
  }

  /**
   * @return The response message from Quickstep.
   */
  const QueryResponse& getResponse() const {
    DCHECK(response_ready_);
    return response_message_;
  }

  /**
   * @return True if query was canceled.
   */
  bool getCanceled() const {
    DCHECK(response_ready_);
    return canceled_;
  }

 private:
  bool response_ready_;
  bool canceled_;
  const QueryRequest& request_;
  QueryResponse response_message_;
  Mutex mutex_;
  ConditionVariable *condition_;  // note: owned by the mutex.

  DISALLOW_COPY_AND_ASSIGN(RequestState);
};

}  // namespace networkio_internal

using networkio_internal::RequestState;

/**
 * @brief Contains the callback methods which the gRPC service defines.
 * When a request is made of gRPC, a gRPC worker thread is spun up and enters one of the callback methods
 * (eg SendQuery). This thread keeps the network connection open while Quickstep processes the query. Concurrency
 * control between the gRPC worker thread, and the Quickstep thread is controlled by a RequestState object which is
 * created for each interaction.
 */
class NetworkCliServiceImpl final : public NetworkCli::Service {
 public:
  NetworkCliServiceImpl()
      : running_(true) {}

  /**
   * @brief Handles gRPC request.
   * Sets the buffer in the RequestState, places the request on a queue, then waits for a response. The response shall
   * be triggered by the Quickstep system.
   */
  Status SendQuery(grpc::ServerContext *context,
                   const QueryRequest *request,
                   QueryResponse *response) override {
    std::unique_ptr<RequestState> request_state(new RequestState(*request));
    // Check to see if the gRPC service has been shutdown.
    {
      SpinSharedMutexSharedLock<true> lock(service_mtx_);

      if (!running_) {
        return Status::CANCELLED;
      }
      // While we have a service lock, we add to the Queue. Note that we keep the service lock to protect ourselves from
      // a race condition in the kill() method.

      // Pushing to the queue will notify consumers.
      request_queue_.push(request_state.get());
    }

    DCHECK(request_state);

    // We have pushed to the request queue, so all there is to do now is wait for Quickstep to process the request.
    request_state->waitForResponse();
    if (request_state->getCanceled()) {
      return Status::CANCELLED;
    }
    *response = request_state->getResponse();
    return Status::OK;
  }

  /**
   * @brief The consumer thread waits for a request to materialize.
   * @return A non-owned RequestState.
   */
  RequestState* waitForRequest() {
    return request_queue_.popOne();
  }

  /**
   * @brief Stops accepting further requests and cancels all pending requests.
   */
  void kill() {
    {
      // This action guarantees that no further requests are added to the queue.
      SpinSharedMutexExclusiveLock<true> lock(service_mtx_);
      running_ = false;
    }
    // Go through each pending request, and cancel them.
    while (!request_queue_.empty()) {
      request_queue_.popOne()->cancel();
    }
  }

 private:
  SpinSharedMutex<true> service_mtx_;
  bool running_;
  ThreadSafeQueue<RequestState*> request_queue_;

  DISALLOW_COPY_AND_ASSIGN(NetworkCliServiceImpl);
};

class NetworkIOHandle final : public IOHandle {
 public:
  explicit NetworkIOHandle(RequestState* state)
      : request_state_(state) {
  }

  ~NetworkIOHandle() override {
    // All the commands from the last network interaction have completed, return our response.
    // This signals to the producer thread that the interaction is complete.
    request_state_->responseReady(out_stream_.str(), err_stream_.str());
  }

  const std::string* data() const override {
    return &request_state_->getRequest().data();
  }

  FILE* out() override {
    return out_stream_.file();
  }

  FILE* err() override {
    return err_stream_.file();
  }

  std::string getCommand() const override {
    return request_state_->getRequest().query();
  }

 private:
  MemStream out_stream_, err_stream_;
  RequestState *request_state_;

  DISALLOW_COPY_AND_ASSIGN(NetworkIOHandle);
};

/**
 * A network interface that uses gRPC to accept commands.
 */
class NetworkIO final : public IOInterface {
 public:
  NetworkIO();

  ~NetworkIO() override {
    service_.kill();
    server_->Shutdown();
    server_->Wait();
  }

  IOHandle* getNextIOHandle() override {
    return new NetworkIOHandle(service_.waitForRequest());
  }

  /**
   * @brief Kills the underlying gRPC service.
   */
  void killService() {
    service_.kill();
  }

  /**
   * @return IP address and port of the network address specified by the user flags.
   */
  static std::string GetAddress() {
    return FLAGS_cli_network_ip + ":" + std::to_string(FLAGS_cli_network_port);
  }

 private:
  std::unique_ptr<Server> server_;
  NetworkCliServiceImpl service_;

  DISALLOW_COPY_AND_ASSIGN(NetworkIO);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_NETWORK_IO_HPP_
