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

#include "query_optimizer/tests/DistributedExecutionGeneratorTestRunner.hpp"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/BlockLocatorUtil.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageManager.hpp"
#include "utility/MemStream.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::free;
using std::make_unique;
using std::malloc;
using std::move;
using std::string;
using std::vector;

using tmb::TaggedMessage;

namespace quickstep {

class CatalogRelation;

namespace optimizer {

const char *DistributedExecutionGeneratorTestRunner::kResetOption =
    "reset_before_execution";

DistributedExecutionGeneratorTestRunner::DistributedExecutionGeneratorTestRunner(const string &storage_path)
    : query_id_(0),
      data_exchangers_(kNumInstances) {
  bus_.Initialize();

  cli_id_ = bus_.Connect();
  bus_.RegisterClientAsSender(cli_id_, kAdmitRequestMessage);
  bus_.RegisterClientAsSender(cli_id_, kPoisonMessage);
  bus_.RegisterClientAsReceiver(cli_id_, kQueryExecutionSuccessMessage);

  bus_.RegisterClientAsSender(cli_id_, kBlockDomainRegistrationMessage);
  bus_.RegisterClientAsReceiver(cli_id_, kBlockDomainRegistrationResponseMessage);

  block_locator_ = make_unique<BlockLocator>(&bus_);
  block_locator_->start();

  test_database_loader_ = make_unique<TestDatabaseLoader>(
      storage_path,
      block_locator::getBlockDomain(
          test_database_loader_data_exchanger_.network_address(), cli_id_, &locator_client_id_, &bus_),
      locator_client_id_,
      &bus_);
  DCHECK_EQ(block_locator_->getBusClientID(), locator_client_id_);
  test_database_loader_data_exchanger_.set_storage_manager(test_database_loader_->storage_manager());
  test_database_loader_data_exchanger_.start();

  test_database_loader_->createTestRelation(false /* allow_vchar */);
  test_database_loader_->loadTestRelation();

  // NOTE(zuyu): Foreman should initialize before Shiftboss so that the former
  // could receive a registration message from the latter.
  foreman_ = make_unique<ForemanDistributed>(&bus_, test_database_loader_->catalog_database());

  // We don't use the NUMA aware version of worker code.
  const vector<numa_node_id> numa_nodes(1 /* Number of worker threads per instance */,
                                        kAnyNUMANodeID);

  for (int i = 0; i < kNumInstances; ++i) {
    workers_.push_back(make_unique<Worker>(0 /* worker_thread_index */, &bus_));

    const vector<tmb::client_id> worker_client_ids(1, workers_.back()->getBusClientID());
    worker_directories_.push_back(
        make_unique<WorkerDirectory>(worker_client_ids.size(), worker_client_ids, numa_nodes));

    auto storage_manager = make_unique<StorageManager>(
        storage_path,
        block_locator::getBlockDomain(
            data_exchangers_[i].network_address(), cli_id_, &locator_client_id_, &bus_),
        locator_client_id_, &bus_);
    DCHECK_EQ(block_locator_->getBusClientID(), locator_client_id_);

    data_exchangers_[i].set_storage_manager(storage_manager.get());
    shiftbosses_.push_back(
        make_unique<Shiftboss>(&bus_, storage_manager.get(), worker_directories_.back().get()));

    storage_managers_.push_back(move(storage_manager));
  }

  foreman_->start();

  for (int i = 0; i < kNumInstances; ++i) {
    data_exchangers_[i].start();
    shiftbosses_[i]->start();
    workers_[i]->start();
  }
}

void DistributedExecutionGeneratorTestRunner::runTestCase(
    const string &input, const std::set<string> &options, string *output) {
  // TODO(qzeng): Test multi-threaded query execution when we have a Sort operator.

  VLOG(4) << "Test SQL(s): " << input;

  if (options.find(kResetOption) != options.end()) {
    test_database_loader_->clear();
    test_database_loader_->createTestRelation(false /* allow_vchar */);
    test_database_loader_->loadTestRelation();
  }

  MemStream output_stream;
  sql_parser_.feedNextBuffer(new string(input));

  while (true) {
    ParseResult result = sql_parser_.getNextStatement();
    if (result.condition != ParseResult::kSuccess) {
      if (result.condition == ParseResult::kError) {
        *output = result.error_message;
      }
      break;
    }

    const ParseStatement &parse_statement = *result.parsed_statement;
    std::printf("%s\n", parse_statement.toString().c_str());
    try {
      OptimizerContext optimizer_context;
      QueryHandle query_handle(query_id_++, cli_id_);

      optimizer_.generateQueryHandle(parse_statement,
                                     test_database_loader_->catalog_database(),
                                     &optimizer_context,
                                     &query_handle);

      QueryExecutionUtil::ConstructAndSendAdmitRequestMessage(
          cli_id_,
          foreman_->getBusClientID(),
          &query_handle,
          &bus_);

      const tmb::AnnotatedMessage annotated_message = bus_.Receive(cli_id_, 0, true);
      DCHECK_EQ(kQueryExecutionSuccessMessage, annotated_message.tagged_message.message_type());

      const CatalogRelation *query_result_relation = query_handle.getQueryResultRelation();
      if (query_result_relation) {
          PrintToScreen::PrintRelation(*query_result_relation,
                                       test_database_loader_->storage_manager(),
                                       output_stream.file());
          DropRelation::Drop(*query_result_relation,
                             test_database_loader_->catalog_database(),
                             test_database_loader_->storage_manager());
      }
    } catch (const SqlError &error) {
      *output = error.formatMessage(input);
      break;
    }
  }

  if (output->empty()) {
    *output = output_stream.str();
  }
}

}  // namespace optimizer
}  // namespace quickstep
