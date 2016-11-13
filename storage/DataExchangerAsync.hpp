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

#ifndef QUICKSTEP_STORAGE_DATA_EXCHANGER_ASYNC_HPP_
#define QUICKSTEP_STORAGE_DATA_EXCHANGER_ASYNC_HPP_

#include <grpc++/grpc++.h>

#include <memory>
#include <string>

#include "storage/DataExchange.grpc.pb.h"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class StorageManager;

/**
 * @brief A class which exchanges data from a StorageManager to its peer.
 **/
class DataExchangerAsync final : public Thread {
 public:
  /**
   * @brief Constructor.
   **/
  DataExchangerAsync();

  ~DataExchangerAsync() override {}

  /**
   * @brief Set the local StorageManager.
   *
   * @param storage_manager The StorageManager to use.
   **/
  void set_storage_manager(StorageManager *storage_manager) {
    storage_manager_ = storage_manager;
  }

  /**
   * @brief Return its network address for peers to connect.
   *
   * @return Its network address.
   **/
  const std::string& network_address() const {
    DCHECK(!server_address_.empty());
    return server_address_;
  }

  /**
   * @brief Shutdown itself.
   **/
  void shutdown() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    queue_->Shutdown();
  }

 protected:
  void run() override;

 private:
  DataExchange::AsyncService service_;

  int port_ = -1;
  // Format IP:port, i.e., "0.0.0.0:0".
  std::string server_address_;

  std::unique_ptr<grpc::ServerCompletionQueue> queue_;
  std::unique_ptr<grpc::Server> server_;

  StorageManager *storage_manager_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(DataExchangerAsync);
};

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_DATA_EXCHANGER_ASYNC_HPP_
