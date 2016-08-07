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

#include "storage/DataExchangerAsync.hpp"

#include <grpc++/grpc++.h>

#include <iostream>
#include <memory>
#include <string>

#include "storage/DataExchange.grpc.pb.h"
#include "storage/DataExchange.pb.h"
#include "storage/StorageManager.hpp"

#include "glog/logging.h"

using grpc::ServerCompletionQueue;

namespace quickstep {
namespace {

/**
 * @brief RPC Request Context Instance.
 **/
class CallContext {
 public:
  /**
   * @brief Constructor.
   *
   * @param service The async service.
   * @param queue The RPC request queue.
   * @param storage_manager The StorageManager to use.
   **/
  CallContext(DataExchange::AsyncService *service,
              ServerCompletionQueue *queue,
              StorageManager *storage_manager)
      : service_(service),
        queue_(queue),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        responder_(&context_),
        status_(CallStatus::CREATE) {
    Proceed();
  }

  /**
   * @brief Process the RPC request.
   **/
  void Proceed();

 private:
  DataExchange::AsyncService *service_;
  ServerCompletionQueue *queue_;

  StorageManager *storage_manager_;

  grpc::ServerContext context_;

  PullRequest request_;
  PullResponse response_;

  grpc::ServerAsyncResponseWriter<PullResponse> responder_;

  enum class CallStatus {
    CREATE = 0,
    PROCESS,
    FINISH,
  };
  CallStatus status_;
};

void CallContext::Proceed() {
  switch (status_) {
    case CallStatus::CREATE: {
      // Change this instance progress to the PROCESS state.
      status_ = CallStatus::PROCESS;

      // As part of the initial CREATE state, we *request* that the system
      // start processing Pull requests. In this request, "this" acts are
      // the tag uniquely identifying the request (so that different CallContext
      // instances can serve different requests concurrently), in this case
      // the memory address of this CallContext instance.
      service_->RequestPull(&context_, &request_, &responder_, queue_, queue_, this);
      break;
    }
    case CallStatus::PROCESS: {
      // Spawn a new CallContext instance to serve new clients while we process
      // the one for this CallContext. The instance will deallocate itself as
      // part of its FINISH state.
      new CallContext(service_, queue_, storage_manager_);

      // The actual processing.
      storage_manager_->pullBlockOrBlob(request_.block_id(), &response_);

      // And we are done! Let the gRPC runtime know we've finished, using the
      // memory address of this instance as the uniquely identifying tag for
      // the event.
      status_ = CallStatus::FINISH;
      responder_.Finish(response_, grpc::Status::OK, this);
      break;
    }
    case CallStatus::FINISH: {
      // Once in the FINISH state, deallocate ourselves (CallContext).
      delete this;
      break;
    }
    default:
      LOG(FATAL) << "Unknown call status.";
  }
}

}  // namespace

const char *DataExchangerAsync::kLocalNetworkAddress = "0.0.0.0:";

DataExchangerAsync::DataExchangerAsync() {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(kLocalNetworkAddress, grpc::InsecureServerCredentials(), &port_);
  builder.RegisterService(&service_);

  queue_ = builder.AddCompletionQueue();
  server_ = builder.BuildAndStart();

  DCHECK_GT(port_, 0);
  server_address_ = kLocalNetworkAddress + std::to_string(port_);
  LOG(INFO) << "DataExchangerAsync Service listening on " << server_address_;
}

void DataExchangerAsync::run() {
  // Self-destruct upon success.
  new CallContext(&service_, queue_.get(), storage_manager_);

  void *tag = nullptr;  // Uniquely identify a request.
  bool ok = false;

  while (true) {
    if (queue_->Next(&tag, &ok)) {
      CallContext *call_context = static_cast<CallContext*>(tag);
      if (ok) {
        call_context->Proceed();
      } else {
        LOG(WARNING) << "Not ok\n";
        delete call_context;
      }
    } else {
      LOG(INFO) << "Shutdown\n";
      return;
    }
  }
}

}  // namespace quickstep
