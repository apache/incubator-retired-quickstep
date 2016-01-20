/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_TESTS_EXECUTION_GENERATOR_TEST_RUNNER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_TESTS_EXECUTION_GENERATOR_TEST_RUNNER_HPP_

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "parser/SqlParserWrapper.hpp"
#include "query_execution/Foreman.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "query_optimizer/tests/TestDatabaseLoader.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "utility/Macros.hpp"
#include "utility/textbased_test/TextBasedTestDriver.hpp"

namespace quickstep {

namespace optimizer {

/**
 * @brief TextBasedTestRunner for testing the ExecutionGenerator.
 */
class ExecutionGeneratorTestRunner : public TextBasedTestRunner {
 public:
  /**
   * @brief If this option is enabled, recreate the entire database and
   * repopulate the data before every test.
   */
  static const std::string kResetOption;

  /**
   * @brief Constructor.
   */
  explicit ExecutionGeneratorTestRunner(const std::string &storage_path)
      : test_database_loader_(storage_path),
        thread_id_map_(ClientIDMap::Instance()) {
    test_database_loader_.createTestRelation(false /* allow_vchar */);
    test_database_loader_.loadTestRelation();

    bus_.Initialize();

    foreman_.reset(new Foreman(&bus_));
    worker_.reset(new Worker(0,
                             query_context_,
                             &bus_,
                             test_database_loader_.catalog_database(),
                             test_database_loader_.storage_manager()));

    std::vector<client_id> worker_client_ids;
    worker_client_ids.push_back(worker_->getBusClientID());

    // We don't use the NUMA aware version of foreman code.
    std::vector<int> numa_nodes;
    numa_nodes.push_back(-1);

    workers_.reset(new WorkerDirectory(1 /* number of workers */,
                                       worker_client_ids, numa_nodes));
    foreman_->setWorkerDirectory(workers_.get());

    worker_->start();
  }

  ~ExecutionGeneratorTestRunner() {
    WorkerMessage poison_message(WorkerMessage::PoisonMessage());
    TaggedMessage poison_tagged_message;
    poison_tagged_message.set_message(&poison_message,
                                      sizeof(poison_message),
                                      quickstep::kPoisonMessage);

    Address worker_address;
    MessageStyle single_receiver_style;

    worker_address.AddRecipient(worker_->getBusClientID());
    bus_.Send(foreman_->getBusClientID(),
              worker_address,
              single_receiver_style,
              std::move(poison_tagged_message));

    worker_->join();
  }

  void runTestCase(const std::string &input,
                   const std::set<std::string> &options,
                   std::string *output) override;

 private:
  SqlParserWrapper sql_parser_;
  TestDatabaseLoader test_database_loader_;

  MessageBusImpl bus_;
  std::unique_ptr<Foreman> foreman_;
  std::unique_ptr<Worker> worker_;
  std::unique_ptr<QueryContext> query_context_;

  std::unique_ptr<WorkerDirectory> workers_;

  // This map is needed for InsertDestination and some operators that send
  // messages to Foreman directly. To know the reason behind the design of this
  // map, see the note in InsertDestination.hpp.
  ClientIDMap *thread_id_map_;

  DISALLOW_COPY_AND_ASSIGN(ExecutionGeneratorTestRunner);
};

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_TESTS_EXECUTION_GENERATOR_TEST_RUNNER_HPP_ */
