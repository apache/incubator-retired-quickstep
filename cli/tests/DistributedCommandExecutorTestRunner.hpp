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

#ifndef QUICKSTEP_CLI_TESTS_DISTRIBUTED_COMMAND_EXECUTOR_TEST_RUNNER_HPP_
#define QUICKSTEP_CLI_TESTS_DISTRIBUTED_COMMAND_EXECUTOR_TEST_RUNNER_HPP_

#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "parser/SqlParserWrapper.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/Shiftboss.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_optimizer/Optimizer.hpp"
#include "query_optimizer/tests/TestDatabaseLoader.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageManager.hpp"
#include "utility/Macros.hpp"
#include "utility/textbased_test/TextBasedTestRunner.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

/**
 * @brief TextBasedTestRunner for testing the CommandExecutor in the
 *        distributed version.
 */
class DistributedCommandExecutorTestRunner : public TextBasedTestRunner {
 public:
  /**
   * @brief If this option is enabled, recreate the entire database and
   *        repopulate the data before every test.
   */
  static const char *kResetOption;

  /**
   * @brief Constructor.
   */
  explicit DistributedCommandExecutorTestRunner(const std::string &storage_path);

  ~DistributedCommandExecutorTestRunner();

  void runTestCase(const std::string &input,
                   const std::set<std::string> &options,
                   std::string *output) override;

 private:
  std::size_t query_id_;

  SqlParserWrapper sql_parser_;
  std::unique_ptr<optimizer::TestDatabaseLoader> test_database_loader_;
  DataExchangerAsync test_database_loader_data_exchanger_;
  optimizer::Optimizer optimizer_;

  MessageBusImpl bus_;
  tmb::client_id cli_id_, locator_client_id_;

  std::unique_ptr<BlockLocator> block_locator_;

  std::unique_ptr<ForemanDistributed> foreman_;

  MessageBusImpl bus_local_;
  std::unique_ptr<Worker> worker_;
  std::unique_ptr<WorkerDirectory> worker_directory_;
  DataExchangerAsync data_exchanger_;
  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<Shiftboss> shiftboss_;

  DISALLOW_COPY_AND_ASSIGN(DistributedCommandExecutorTestRunner);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_TESTS_DISTRIBUTED_COMMAND_EXECUTOR_TEST_RUNNER_HPP_
