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

#include "cli/tests/DistributedCommandExecutorTestRunner.hpp"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "cli/CommandExecutor.hpp"
#include "cli/Constants.hpp"
#include "cli/DefaultsConfigurator.hpp"
#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/BlockLocatorUtil.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "utility/MemStream.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::make_unique;
using std::string;
using std::vector;

using tmb::TaggedMessage;

namespace quickstep {

class CatalogRelation;

DistributedCommandExecutorTestRunner::DistributedCommandExecutorTestRunner(const string &storage_path)
    : catalog_path_(storage_path + kCatalogFilename),
      query_id_(0) {
  bus_.Initialize();

  cli_id_ = bus_.Connect();
  bus_.RegisterClientAsSender(cli_id_, kAdmitRequestMessage);
  bus_.RegisterClientAsSender(cli_id_, kPoisonMessage);
  bus_.RegisterClientAsReceiver(cli_id_, kQueryExecutionSuccessMessage);

  bus_.RegisterClientAsSender(cli_id_, kBlockDomainRegistrationMessage);
  bus_.RegisterClientAsReceiver(cli_id_, kBlockDomainRegistrationResponseMessage);

  block_locator_ = make_unique<BlockLocator>(&bus_);
  block_locator_->start();

  DefaultsConfigurator::InitializeDefaultDatabase(storage_path, catalog_path_);
  query_processor_ = std::make_unique<QueryProcessor>(std::string(catalog_path_));

  // NOTE(zuyu): Foreman should initialize before Shiftboss so that the former
  // could receive a registration message from the latter.
  foreman_ = make_unique<ForemanDistributed>(*block_locator_, &bus_, query_processor_->getDefaultDatabase(),
                                             query_processor_.get());
  foreman_->start();

  // We don't use the NUMA aware version of worker code.
  const vector<numa_node_id> numa_nodes(1 /* Number of worker threads per instance */,
                                        kAnyNUMANodeID);

  bus_local_.Initialize();

  worker_ = make_unique<Worker>(0 /* worker_thread_index */, &bus_local_);

  const vector<tmb::client_id> worker_client_ids(1, worker_->getBusClientID());
  worker_directory_ = make_unique<WorkerDirectory>(worker_client_ids.size(), worker_client_ids, numa_nodes);

  storage_manager_ = make_unique<StorageManager>(
      storage_path,
      block_locator::getBlockDomain(
          data_exchanger_.network_address(), cli_id_, &locator_client_id_, &bus_),
      locator_client_id_, &bus_);
  DCHECK_EQ(block_locator_->getBusClientID(), locator_client_id_);

  data_exchanger_.set_storage_manager(storage_manager_.get());
  shiftboss_ =
      make_unique<Shiftboss>(&bus_, &bus_local_, storage_manager_.get(), worker_directory_.get(),
                             storage_manager_->hdfs());

  data_exchanger_.start();
  shiftboss_->start();
  worker_->start();
}

DistributedCommandExecutorTestRunner::~DistributedCommandExecutorTestRunner() {
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(&bus_, cli_id_, foreman_->getBusClientID(), TaggedMessage(kPoisonMessage));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK);

  worker_->join();
  shiftboss_->join();

  foreman_->join();

  data_exchanger_.shutdown();
  storage_manager_.reset();

  CHECK(MessageBus::SendStatus::kOK ==
      QueryExecutionUtil::SendTMBMessage(&bus_, cli_id_, locator_client_id_, TaggedMessage(kPoisonMessage)));

  data_exchanger_.join();
  block_locator_->join();

  const std::string command = "rm -f " + catalog_path_;
  CHECK(!std::system(command.c_str()))
      << "Failed when attempting to remove catalog proto file: " << catalog_path_;
}

void DistributedCommandExecutorTestRunner::runTestCase(
    const string &input, const std::set<string> &options, string *output) {
  // TODO(qzeng): Test multi-threaded query execution when we have a Sort operator.

  VLOG(4) << "Test SQL(s): " << input;

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
      if (parse_statement.getStatementType() == ParseStatement::kCommand) {
        cli::executeCommand(
            *result.parsed_statement,
            *query_processor_->getDefaultDatabase(),
            cli_id_,
            foreman_->getBusClientID(),
            &bus_,
            storage_manager_.get(),
            query_processor_.get(),
            output_stream.file());
      } else {
        auto query_handle = std::make_unique<QueryHandle>(query_id_++, cli_id_);
        query_processor_->generateQueryHandle(parse_statement, query_handle.get());

        const CatalogRelation *query_result_relation = query_handle->getQueryResultRelation();

        QueryExecutionUtil::ConstructAndSendAdmitRequestMessage(
            cli_id_, foreman_->getBusClientID(), query_handle.release(), &bus_);

        const tmb::AnnotatedMessage annotated_message = bus_.Receive(cli_id_, 0, true);
        DCHECK_EQ(kQueryExecutionSuccessMessage, annotated_message.tagged_message.message_type());

        if (query_result_relation) {
          PrintToScreen::PrintRelation(*query_result_relation,
                                       storage_manager_.get(),
                                       output_stream.file());
          DropRelation::Drop(*query_result_relation,
                             query_processor_->getDefaultDatabase(),
                             storage_manager_.get());
        }
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

}  // namespace quickstep
