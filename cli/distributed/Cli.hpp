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

#ifndef QUICKSTEP_CLI_DISTRIBUTED_CLI_HPP_
#define QUICKSTEP_CLI_DISTRIBUTED_CLI_HPP_

#include <memory>

#include "cli/distributed/Role.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageManager.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

/** \addtogroup CliDistributed
 *  @{
 */

/**
 * @brief A class for the Cli component in the distributed version.
 **/
class Cli final : public Role {
 public:
  /**
   * @brief Constructor.
   **/
  Cli() = default;

  ~Cli() override {
    data_exchanger_.shutdown();
    storage_manager_.reset();
    data_exchanger_.join();
  }

  void init() override;

  void run() override;

 private:
  tmb::client_id cli_id_, conductor_client_id_;

  DataExchangerAsync data_exchanger_;
  std::unique_ptr<StorageManager> storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(Cli);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_DISTRIBUTED_CLI_HPP_
