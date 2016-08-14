/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_TESTS_DISTRIBUTED_EXECUTION_GENERATOR_TEST_RUNNER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_TESTS_DISTRIBUTED_EXECUTION_GENERATOR_TEST_RUNNER_HPP_

#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/Shiftboss.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_optimizer/tests/TestDatabaseLoader.hpp"
#include "utility/Macros.hpp"
#include "utility/textbased_test/TextBasedTestRunner.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {
namespace optimizer {

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
  explicit DistributedExecutionGeneratorTestRunner(const std::string &storage_path)
      : query_id_(0),
        test_database_loader_(storage_path) {
    test_database_loader_.createTestRelation(false /* allow_vchar */);
    test_database_loader_.loadTestRelation();

    bus_.Initialize();

    // NOTE(zuyu): Foreman should initialize before Shiftboss so that the former
    // could receive a registration message from the latter.
    foreman_.reset(new ForemanDistributed(&bus_, test_database_loader_.catalog_database()));

    worker_.reset(new Worker(0 /* worker_thread_index */, &bus_));

    std::vector<tmb::client_id> worker_client_ids;
    worker_client_ids.push_back(worker_->getBusClientID());

    // We don't use the NUMA aware version of worker code.
    const std::vector<numa_node_id> numa_nodes(worker_client_ids.size(), kAnyNUMANodeID);

    workers_.reset(
        new WorkerDirectory(worker_client_ids.size(), worker_client_ids, numa_nodes));

    shiftboss_.reset(new Shiftboss(&bus_, test_database_loader_.storage_manager(), workers_.get()));

    cli_id_ = bus_.Connect();
    bus_.RegisterClientAsSender(cli_id_, kAdmitRequestMessage);
    bus_.RegisterClientAsSender(cli_id_, kPoisonMessage);
    bus_.RegisterClientAsReceiver(cli_id_, kQueryExecutionSuccessMessage);

    foreman_->start();

    shiftboss_->start();
    worker_->start();
  }

  ~DistributedExecutionGeneratorTestRunner() {
    tmb::TaggedMessage poison_tagged_message(quickstep::kPoisonMessage);

    const tmb::MessageBus::SendStatus send_status =
        QueryExecutionUtil::SendTMBMessage(
            &bus_,
            cli_id_,
            foreman_->getBusClientID(),
            std::move(poison_tagged_message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK);

    worker_->join();
    shiftboss_->join();

    foreman_->join();
  }

  void runTestCase(const std::string &input,
                   const std::set<std::string> &options,
                   std::string *output) override;

 private:
  std::size_t query_id_;

  SqlParserWrapper sql_parser_;
  TestDatabaseLoader test_database_loader_;

  MessageBusImpl bus_;

  tmb::client_id cli_id_;

  std::unique_ptr<ForemanDistributed> foreman_;

  std::unique_ptr<Worker> worker_;
  std::unique_ptr<WorkerDirectory> workers_;

  std::unique_ptr<Shiftboss> shiftboss_;

  DISALLOW_COPY_AND_ASSIGN(DistributedExecutionGeneratorTestRunner);
};

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_TESTS_DISTRIBUTED_EXECUTION_GENERATOR_TEST_RUNNER_HPP_
