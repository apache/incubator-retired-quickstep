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

#ifndef QUICKSTEP_CLI_DISTRIBUTED_EXECUTOR_HPP_
#define QUICKSTEP_CLI_DISTRIBUTED_EXECUTOR_HPP_

#include <memory>
#include <vector>

#include "cli/distributed/Role.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/Shiftboss.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageManager.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

/** \addtogroup CliDistributed
 *  @{
 */

/**
 * @brief A class for the Executor component in the distributed version.
 **/
class Executor final : public Role {
 public:
  /**
   * @brief Constructor.
   **/
  Executor() = default;

  ~Executor() override {
    for (const auto &worker : workers_) {
      worker->join();
    }
    shiftboss_->join();

    data_exchanger_.shutdown();
    storage_manager_.reset();
    data_exchanger_.join();
  }

  void init() override;

  void run() override {}

 private:
  // Used between Shiftboss and Workers.
  MessageBusImpl bus_local_;

  tmb::client_id executor_client_id_;

  std::vector<std::unique_ptr<Worker>> workers_;
  std::unique_ptr<WorkerDirectory> worker_directory_;
  DataExchangerAsync data_exchanger_;
  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<Shiftboss> shiftboss_;

  DISALLOW_COPY_AND_ASSIGN(Executor);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_DISTRIBUTED_EXECUTOR_HPP_
