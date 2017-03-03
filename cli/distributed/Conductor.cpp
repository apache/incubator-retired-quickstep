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

#include "cli/distributed/Conductor.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "catalog/CatalogDatabase.hpp"
#include "cli/CommandExecutorUtil.hpp"
#include "cli/Constants.hpp"
#include "cli/DefaultsConfigurator.hpp"
#include "cli/Flags.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "storage/StorageConstants.hpp"
#include "utility/SqlError.hpp"
#include "utility/StringUtil.hpp"

#include "tmb/id_typedefs.h"
#include "tmb/native_net_client_message_bus.h"
#include "tmb/tagged_message.h"

#include "glog/logging.h"

using std::free;
using std::make_unique;
using std::malloc;
using std::move;
using std::size_t;
using std::string;

using tmb::AnnotatedMessage;
using tmb::MessageBus;
using tmb::TaggedMessage;
using tmb::client_id;

namespace quickstep {

namespace C = cli;
namespace S = serialization;

void Conductor::init() {
  try {
    string catalog_path = FLAGS_storage_path + kCatalogFilename;

    if (FLAGS_initialize_db) {  // Initialize the database
      DefaultsConfigurator::InitializeDefaultDatabase(FLAGS_storage_path, catalog_path);
    }

    query_processor_ = make_unique<QueryProcessor>(move(catalog_path));
    catalog_database_ = query_processor_->getDefaultDatabase();
  } catch (const std::exception &e) {
    LOG(FATAL) << "FATAL ERROR DURING STARTUP: " << e.what()
               << "\nIf you intended to create a new database, "
               << "please use the \"-initialize_db=true\" command line option.";
  } catch (...) {
    LOG(FATAL) << "NON-STANDARD EXCEPTION DURING STARTUP";
  }

  bus_.ResetBus();

  conductor_client_id_ = bus_.Connect();
  DLOG(INFO) << "Conductor TMB Client ID: " << conductor_client_id_;

  bus_.RegisterClientAsReceiver(conductor_client_id_, kDistributedCliRegistrationMessage);
  bus_.RegisterClientAsSender(conductor_client_id_, kDistributedCliRegistrationResponseMessage);

  bus_.RegisterClientAsReceiver(conductor_client_id_, kSqlQueryMessage);
  bus_.RegisterClientAsSender(conductor_client_id_, kCommandResponseMessage);
  bus_.RegisterClientAsSender(conductor_client_id_, kQueryExecutionErrorMessage);
  bus_.RegisterClientAsSender(conductor_client_id_, kAdmitRequestMessage);


  bus_.RegisterClientAsReceiver(conductor_client_id_, kQueryResultTeardownMessage);

  block_locator_ = make_unique<BlockLocator>(&bus_);
  block_locator_->start();

  foreman_ = make_unique<ForemanDistributed>(*block_locator_, &bus_, catalog_database_, query_processor_.get());
  foreman_->start();
}

void Conductor::run() {
  for (;;) {
    AnnotatedMessage annotated_message(bus_.Receive(conductor_client_id_, 0, true));
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    const client_id sender = annotated_message.sender;

    DLOG(INFO) << "Conductor received typed '" << tagged_message.message_type()
               << "' message from client " << sender;
    switch (tagged_message.message_type()) {
      case kDistributedCliRegistrationMessage: {
        TaggedMessage message(kDistributedCliRegistrationResponseMessage);

        DLOG(INFO) << "Conductor sent DistributedCliRegistrationResponseMessage (typed '"
                   << kDistributedCliRegistrationResponseMessage
                   << "') to Distributed CLI " << sender;
        CHECK(MessageBus::SendStatus::kOK ==
            QueryExecutionUtil::SendTMBMessage(&bus_, conductor_client_id_, sender, move(message)));
        break;
      }
      case kSqlQueryMessage: {
        S::SqlQueryMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));
        DLOG(INFO) << "Conductor received the following SQL query: " << proto.sql_query();

        processSqlQueryMessage(sender, new string(move(proto.sql_query())));
        break;
      }
      case kQueryResultTeardownMessage: {
        S::QueryResultTeardownMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        catalog_database_->dropRelationById(proto.relation_id());
        break;
      }
      default:
        LOG(FATAL) << "Unknown TMB message type";
    }
  }
}

void Conductor::processSqlQueryMessage(const tmb::client_id sender, string *command_string) {
  SqlParserWrapper parser_wrapper;
  parser_wrapper.feedNextBuffer(command_string);
  ParseResult parse_result = parser_wrapper.getNextStatement();
  CHECK(parse_result.condition == ParseResult::kSuccess)
      << "Any syntax error should be addressed in the DistributedCli.";

  const ParseStatement &statement = *parse_result.parsed_statement;

  try {
    if (statement.getStatementType() == ParseStatement::kCommand) {
      const ParseCommand &parse_command = static_cast<const ParseCommand &>(statement);
      const PtrVector<ParseString> &arguments = *(parse_command.arguments());
      const string &command = parse_command.command()->value();

      string command_response;
      if (command == C::kDescribeDatabaseCommand) {
        command_response = C::ExecuteDescribeDatabase(arguments, *catalog_database_);
      } else if (command == C::kDescribeTableCommand) {
        if (arguments.empty()) {
          command_response = C::ExecuteDescribeDatabase(arguments, *catalog_database_);
        } else {
          command_response = C::ExecuteDescribeTable(arguments, *catalog_database_);
        }
      }

      S::CommandResponseMessage proto;
      proto.set_command_response(command_response);

      const size_t proto_length = proto.ByteSize();
      char *proto_bytes = static_cast<char*>(malloc(proto_length));
      CHECK(proto.SerializeToArray(proto_bytes, proto_length));

      TaggedMessage message(static_cast<const void*>(proto_bytes), proto_length, kCommandResponseMessage);
      free(proto_bytes);

      DLOG(INFO) << "Conductor sent CommandResponseMessage (typed '" << kCommandResponseMessage
                 << "') to Distributed CLI " << sender;
      CHECK(MessageBus::SendStatus::kOK ==
          QueryExecutionUtil::SendTMBMessage(&bus_, conductor_client_id_, sender, move(message)));
    } else {
      auto query_handle = make_unique<QueryHandle>(query_processor_->query_id(),
                                                   sender,
                                                   statement.getPriority());
      query_processor_->generateQueryHandle(statement, query_handle.get());
      DCHECK(query_handle->getQueryPlanMutable() != nullptr);

      QueryExecutionUtil::ConstructAndSendAdmitRequestMessage(
          conductor_client_id_,
          foreman_->getBusClientID(),
          query_handle.release(),
          &bus_);
    }
  } catch (const SqlError &sql_error) {
    // Set the query execution status along with the error message.
    S::QueryExecutionErrorMessage proto;
    proto.set_error_message(sql_error.formatMessage(*command_string));

    const size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    TaggedMessage message(static_cast<const void*>(proto_bytes),
                          proto_length,
                          kQueryExecutionErrorMessage);
    free(proto_bytes);

    DLOG(INFO) << "Conductor (on behalf of Optimizer) sent QueryExecutionErrorMessage (typed '"
               << kQueryExecutionErrorMessage
               << "') to Distributed CLI " << sender;
    CHECK(MessageBus::SendStatus::kOK ==
        QueryExecutionUtil::SendTMBMessage(&bus_, conductor_client_id_, sender, move(message)));
  }
}

}  // namespace quickstep
