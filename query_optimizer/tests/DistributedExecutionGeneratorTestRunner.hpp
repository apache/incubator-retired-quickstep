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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_TESTS_DISTRIBUTED_EXECUTION_GENERATOR_TEST_RUNNER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_TESTS_DISTRIBUTED_EXECUTION_GENERATOR_TEST_RUNNER_HPP_

#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "parser/SqlParserWrapper.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/Shiftboss.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_optimizer/Optimizer.hpp"
#include "query_optimizer/tests/TestDatabaseLoader.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"
#include "utility/textbased_test/TextBasedTestRunner.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {
namespace optimizer {

namespace {
constexpr int kNumInstances = 3;
}  // namespace

/**
 * @brief TextBasedTestRunner for testing the ExecutionGenerator in the
 *        distributed version.
 */
class DistributedExecutionGeneratorTestRunner : public TextBasedTestRunner {
 public:
  /**
   * @brief If this option is enabled, recreate the entire database and
   *        repopulate the data before every test.
   */
  static const char *kResetOption;

  /**
   * @brief Constructor.
   */
  explicit DistributedExecutionGeneratorTestRunner(const std::string &storage_path);

  ~DistributedExecutionGeneratorTestRunner() {
    tmb::TaggedMessage poison_tagged_message(quickstep::kPoisonMessage);

    const tmb::MessageBus::SendStatus send_status =
        QueryExecutionUtil::SendTMBMessage(
            &bus_,
            cli_id_,
            foreman_->getBusClientID(),
            std::move(poison_tagged_message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK);

    for (int i = 0; i < kNumInstances; ++i) {
      workers_[i]->join();
      shiftbosses_[i]->join();
    }

    foreman_->join();

    test_database_loader_data_exchanger_.shutdown();
    test_database_loader_.reset();
    for (int i = 0; i < kNumInstances; ++i) {
      data_exchangers_[i].shutdown();
      storage_managers_[i].reset();
    }

    CHECK(MessageBus::SendStatus::kOK ==
        QueryExecutionUtil::SendTMBMessage(&bus_,
                                           cli_id_,
                                           locator_client_id_,
                                           tmb::TaggedMessage(quickstep::kPoisonMessage)));

    test_database_loader_data_exchanger_.join();
    for (int i = 0; i < kNumInstances; ++i) {
      data_exchangers_[i].join();
    }
    block_locator_->join();
  }

  void runTestCase(const std::string &input,
                   const std::set<std::string> &options,
                   std::string *output) override;

 private:
  block_id_domain getBlockDomain(const std::string &network_address);

  std::size_t query_id_;

  SqlParserWrapper sql_parser_;
  std::unique_ptr<TestDatabaseLoader> test_database_loader_;
  DataExchangerAsync test_database_loader_data_exchanger_;
  Optimizer optimizer_;

  MessageBusImpl bus_;
  tmb::client_id cli_id_, locator_client_id_;

  std::unique_ptr<BlockLocator> block_locator_;

  std::unique_ptr<ForemanDistributed> foreman_;

  std::vector<std::unique_ptr<Worker>> workers_;
  std::vector<std::unique_ptr<WorkerDirectory>> worker_directories_;
  std::vector<DataExchangerAsync> data_exchangers_;
  std::vector<std::unique_ptr<StorageManager>> storage_managers_;
  std::vector<std::unique_ptr<Shiftboss>> shiftbosses_;

  DISALLOW_COPY_AND_ASSIGN(DistributedExecutionGeneratorTestRunner);
};

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_TESTS_DISTRIBUTED_EXECUTION_GENERATOR_TEST_RUNNER_HPP_
