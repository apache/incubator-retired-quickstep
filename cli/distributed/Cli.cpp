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

#include "cli/distributed/Cli.hpp"

#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "cli/CliConfig.h"  // For QUICKSTEP_USE_LINENOISE.
#include "cli/Flags.hpp"

#ifdef QUICKSTEP_USE_LINENOISE
#include "cli/LineReaderLineNoise.hpp"
typedef quickstep::LineReaderLineNoise LineReaderImpl;
#else
#include "cli/LineReaderDumb.hpp"
typedef quickstep::LineReaderDumb LineReaderImpl;
#endif

#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/BlockLocatorUtil.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "utility/StringUtil.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/native_net_client_message_bus.h"
#include "tmb/tagged_message.h"

#include "glog/logging.h"

using std::fprintf;
using std::free;
using std::make_unique;
using std::malloc;
using std::move;
using std::printf;
using std::size_t;
using std::string;
using std::vector;

using tmb::AnnotatedMessage;
using tmb::TaggedMessage;
using tmb::client_id;

namespace quickstep {

namespace S = ::quickstep::serialization;

void Cli::init() {
  cli_id_ = bus_.Connect();
  DLOG(INFO) << "DistributedCli TMB Client ID: " << cli_id_;

  bus_.RegisterClientAsSender(cli_id_, kDistributedCliRegistrationMessage);
  bus_.RegisterClientAsReceiver(cli_id_, kDistributedCliRegistrationResponseMessage);

  DLOG(INFO) << "DistributedCli sent DistributedCliRegistrationMessage (typed '"
             << kDistributedCliRegistrationMessage
             << "') to all";

  tmb::Address all_addresses;
  all_addresses.All(true);
  // NOTE(zuyu): The singleton Conductor would need one copy of the message.
  tmb::MessageStyle style;

  TaggedMessage cli_reg_message(kDistributedCliRegistrationMessage);
  CHECK(tmb::MessageBus::SendStatus::kOK ==
      bus_.Send(cli_id_, all_addresses, style, move(cli_reg_message)));

  // Wait for Conductor to response.
  const AnnotatedMessage cli_reg_response_message(bus_.Receive(cli_id_, 0, true));
  CHECK_EQ(kDistributedCliRegistrationResponseMessage,
           cli_reg_response_message.tagged_message.message_type());
  conductor_client_id_ = cli_reg_response_message.sender;

  DLOG(INFO) << "DistributedCli received typed '" << kDistributedCliRegistrationResponseMessage
             << "' message from Conductor (id'" << conductor_client_id_ << "').";

  // Setup StorageManager.
  bus_.RegisterClientAsSender(cli_id_, kBlockDomainRegistrationMessage);
  bus_.RegisterClientAsReceiver(cli_id_, kBlockDomainRegistrationResponseMessage);

  client_id locator_client_id;
  storage_manager_ = make_unique<StorageManager>(
      FLAGS_storage_path,
      block_locator::getBlockDomain(data_exchanger_.network_address(), cli_id_, &locator_client_id, &bus_),
      locator_client_id, &bus_);

  data_exchanger_.set_storage_manager(storage_manager_.get());
  data_exchanger_.start();

  // Prepare for submitting a query.
  bus_.RegisterClientAsSender(cli_id_, kSqlQueryMessage);

  bus_.RegisterClientAsReceiver(cli_id_, kQueryExecutionSuccessMessage);
  bus_.RegisterClientAsSender(cli_id_, kQueryResultTeardownMessage);

  bus_.RegisterClientAsReceiver(cli_id_, kQueryExecutionErrorMessage);
}

void Cli::run() {
  LineReaderImpl line_reader("distributed_quickstep> ",
                             "                  ...> ");
  auto parser_wrapper = make_unique<SqlParserWrapper>();
  std::chrono::time_point<std::chrono::steady_clock> start, end;

  for (;;) {
    string *command_string = new string();
    *command_string = line_reader.getNextCommand();
    if (command_string->size() == 0) {
      delete command_string;
      break;
    }

    parser_wrapper->feedNextBuffer(command_string);

    bool quitting = false;
    // A parse error should reset the parser. This is because the thrown quickstep
    // SqlError does not do the proper reset work of the YYABORT macro.
    bool reset_parser = false;
    for (;;) {
      ParseResult result = parser_wrapper->getNextStatement();
      const ParseStatement &statement = *result.parsed_statement;
      if (result.condition == ParseResult::kSuccess) {
        if (statement.getStatementType() == ParseStatement::kQuit) {
          quitting = true;
          break;
        }

        CHECK_NE(statement.getStatementType(), ParseStatement::kCommand)
            << "TODO(quickstep-team)";

        DLOG(INFO) << "DistributedCli sent SqlQueryMessage (typed '" << kSqlQueryMessage
                   << "') to Conductor";
        S::SqlQueryMessage proto;
        proto.set_sql_query(*command_string);

        const size_t proto_length = proto.ByteSize();
        char *proto_bytes = static_cast<char*>(malloc(proto_length));
        CHECK(proto.SerializeToArray(proto_bytes, proto_length));

        TaggedMessage sql_query_message(static_cast<const void*>(proto_bytes),
                                        proto_length,
                                        kSqlQueryMessage);
        free(proto_bytes);

        QueryExecutionUtil::SendTMBMessage(&bus_,
                                           cli_id_,
                                           conductor_client_id_,
                                           move(sql_query_message));

        start = std::chrono::steady_clock::now();

        const AnnotatedMessage annotated_message(bus_.Receive(cli_id_, 0, true));
        const TaggedMessage &tagged_message = annotated_message.tagged_message;
        DLOG(INFO) << "DistributedCli received typed '" << tagged_message.message_type()
                   << "' message from client " << annotated_message.sender;
        switch (tagged_message.message_type()) {
          case kQueryExecutionSuccessMessage: {
            end = std::chrono::steady_clock::now();

            S::QueryExecutionSuccessMessage proto;
            CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

            if (proto.has_result_relation()) {
              const CatalogRelation result_relation(proto.result_relation());

              PrintToScreen::PrintRelation(result_relation, storage_manager_.get(), stdout);
              PrintToScreen::PrintOutputSize(result_relation, storage_manager_.get(), stdout);

              const vector<block_id> blocks(result_relation.getBlocksSnapshot());
              for (const block_id block : blocks) {
                storage_manager_->deleteBlockOrBlobFile(block);
              }

              // Notify Conductor to remove the temp query result relation in the Catalog.
              S::QueryResultTeardownMessage proto_response;
              proto_response.set_relation_id(result_relation.getID());

              const size_t proto_response_length = proto_response.ByteSize();
              char *proto_response_bytes = static_cast<char*>(malloc(proto_response_length));
              CHECK(proto_response.SerializeToArray(proto_response_bytes, proto_response_length));

              TaggedMessage response_message(static_cast<const void*>(proto_response_bytes),
                                             proto_response_length,
                                             kQueryResultTeardownMessage);
              free(proto_response_bytes);

              QueryExecutionUtil::SendTMBMessage(&bus_, cli_id_, conductor_client_id_, move(response_message));
            }

            std::chrono::duration<double, std::milli> time_in_ms = end - start;
            printf("Time: %s ms\n", DoubleToStringWithSignificantDigits(time_in_ms.count(), 3).c_str());
            break;
          }
          case kQueryExecutionErrorMessage: {
            S::QueryExecutionErrorMessage proto;
            CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

            fprintf(stderr, "%s", proto.error_message().c_str());
            break;
          }
          default: {
            LOG(ERROR) << "Unknown TMB message type";
          }
        }
      } else {
        if (result.condition == ParseResult::kError) {
          fprintf(stderr, "%s", result.error_message.c_str());
        }
        reset_parser = true;
        break;
      }
    }

    if (quitting) {
      break;
    } else if (reset_parser) {
      parser_wrapper = make_unique<SqlParserWrapper>();
      reset_parser = false;
    }
  }

  bus_.Disconnect(cli_id_);
}

}  // namespace quickstep
