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

#ifndef QUICKSTEP_CLI_DISTRIBUTED_CONDUCTOR_HPP_
#define QUICKSTEP_CLI_DISTRIBUTED_CONDUCTOR_HPP_

#include <memory>
#include <string>

#include "cli/distributed/Role.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

class CatalogDatabase;
class ParseString;

/** \addtogroup CliDistributed
 *  @{
 */

/**
 * @brief A class for the Conductor component in the distributed version.
 **/
class Conductor final : public Role {
 public:
  /**
   * @brief Constructor.
   **/
  Conductor() = default;

  ~Conductor() override {
    foreman_->join();
    block_locator_->join();
  }

  void init() override;

  void run() override;

 private:
  void processSqlQueryMessage(const tmb::client_id sender, std::string *command_string);

  void executeAnalyze(const tmb::client_id sender, const PtrVector<ParseString> &arguments);

  void submitQuery(const tmb::client_id sender, std::string *query, QueryHandle::AnalyzeQueryInfo *query_info);

  std::unique_ptr<QueryProcessor> query_processor_;
  // Not owned.
  CatalogDatabase *catalog_database_;

  tmb::client_id conductor_client_id_;

  std::unique_ptr<BlockLocator> block_locator_;

  std::unique_ptr<ForemanDistributed> foreman_;

  DISALLOW_COPY_AND_ASSIGN(Conductor);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_DISTRIBUTED_CONDUCTOR_HPP_
