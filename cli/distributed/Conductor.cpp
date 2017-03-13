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
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "cli/CommandExecutorUtil.hpp"
#include "cli/Constants.hpp"
#include "cli/DefaultsConfigurator.hpp"
#include "cli/Flags.hpp"
#include "expressions/aggregation/AggregateFunctionMax.hpp"
#include "expressions/aggregation/AggregateFunctionMin.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/ParseString.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "storage/StorageConstants.hpp"
#include "utility/PtrVector.hpp"
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

class Type;

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
    const tmb::message_type_id message_type = tagged_message.message_type();
    const client_id sender = annotated_message.sender;
    DLOG(INFO) << "Conductor received " << QueryExecutionUtil::MessageTypeToString(message_type)
               << " from Client " << sender;
    switch (message_type) {
      case kDistributedCliRegistrationMessage: {
        TaggedMessage message(kDistributedCliRegistrationResponseMessage);

        DLOG(INFO) << "Conductor sent DistributedCliRegistrationResponseMessage to DistributedCLI with Client "
                   << sender;
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
  CHECK_EQ(ParseResult::kSuccess, parse_result.condition)
      << "Any syntax error should be addressed in the DistributedCli.";

  const ParseStatement &statement = *parse_result.parsed_statement;

  try {
    if (statement.getStatementType() == ParseStatement::kCommand) {
      const ParseCommand &parse_command = static_cast<const ParseCommand &>(statement);
      const PtrVector<ParseString> &arguments = *(parse_command.arguments());
      const string &command = parse_command.command()->value();

      if (command == C::kAnalyzeCommand) {
        executeAnalyze(sender, arguments);
        return;
      }

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

      DLOG(INFO) << "Conductor sent CommandResponseMessage to DistributedCLI with Client " << sender;
      CHECK(MessageBus::SendStatus::kOK ==
          QueryExecutionUtil::SendTMBMessage(&bus_, conductor_client_id_, sender, move(message)));
    } else {
      auto query_handle = make_unique<QueryHandle>(query_processor_->query_id(),
                                                   sender,
                                                   statement.getPriority());
      query_processor_->generateQueryHandle(statement, query_handle.get());
      DCHECK(query_handle->getQueryPlanMutable() != nullptr);

      CHECK(MessageBus::SendStatus::kOK ==
          QueryExecutionUtil::ConstructAndSendAdmitRequestMessage(
              conductor_client_id_, foreman_->getBusClientID(), query_handle.release(), &bus_));
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

    DLOG(INFO) << "Conductor (on behalf of Optimizer) sent QueryExecutionErrorMessage to DistributedCLI with Client "
               << sender;
    CHECK(MessageBus::SendStatus::kOK ==
        QueryExecutionUtil::SendTMBMessage(&bus_, conductor_client_id_, sender, move(message)));
  }
}

void Conductor::executeAnalyze(const tmb::client_id sender, const PtrVector<ParseString> &arguments) {
  std::vector<std::reference_wrapper<const CatalogRelation>> relations;
  if (arguments.empty()) {
    relations.insert(relations.end(), catalog_database_->begin(), catalog_database_->end());
  } else {
    for (const auto &argument : arguments) {
      const CatalogRelation *relation = catalog_database_->getRelationByName(argument.value());
      if (relation == nullptr) {
        THROW_SQL_ERROR_AT(&argument) << "Table does not exist";
      }

      relations.emplace_back(*relation);
    }
  }

  SqlParserWrapper parser_wrapper;
  std::vector<QueryHandle*> query_handles;

  // Analyze each relation in the database.
  for (const CatalogRelation &relation : relations) {
    const relation_id rel_id = relation.getID();
    const string rel_name = EscapeQuotes(relation.getName(), '"');

    string *query_string = nullptr;

    // Get the number of distinct values for each column.
    for (const CatalogAttribute &attribute : relation) {
      const string attr_name = EscapeQuotes(attribute.getName(), '"');
      const Type &attr_type = attribute.getType();
      const bool is_min_applicable =
          AggregateFunctionMin::Instance().canApplyToTypes({&attr_type});
      const bool is_max_applicable =
          AggregateFunctionMax::Instance().canApplyToTypes({&attr_type});

      // NOTE(jianqiao): Note that the relation name and the attribute names may
      // contain non-letter characters, e.g. CREATE TABLE "with space"("1" int).
      // So here we need to format the names with double quotes (").
      query_string = new string("SELECT COUNT(DISTINCT \"");
      query_string->append(attr_name);
      query_string->append("\")");
      if (is_min_applicable) {
        query_string->append(", MIN(\"");
        query_string->append(attr_name);
        query_string->append("\")");
      }
      if (is_max_applicable) {
        query_string->append(", MAX(\"");
        query_string->append(attr_name);
        query_string->append("\")");
      }
      query_string->append(" FROM \"");
      query_string->append(rel_name);
      query_string->append("\";");

      parser_wrapper.feedNextBuffer(query_string);
      const ParseResult parse_result = parser_wrapper.getNextStatement();
      DCHECK_EQ(ParseResult::kSuccess, parse_result.condition);

      const ParseStatement &statement = *parse_result.parsed_statement;

      // Generate the query plan.
      auto query_handle =
          make_unique<QueryHandle>(query_processor_->query_id(), sender, statement.getPriority(),
                                   new QueryHandle::AnalyzeQueryInfo(true /* is_analyze_attribute_query */, rel_id,
                                                                     relations.size(), attribute.getID(),
                                                                     is_min_applicable, is_max_applicable));
      query_processor_->generateQueryHandle(statement, query_handle.get());
      DCHECK(query_handle->getQueryPlanMutable() != nullptr);

      query_handles.push_back(query_handle.release());
    }

    // Get the number of tuples for the relation.
    query_string = new string("SELECT COUNT(*) FROM \"");
    query_string->append(rel_name);
    query_string->append("\";");

    parser_wrapper.feedNextBuffer(query_string);
    const ParseResult parse_result = parser_wrapper.getNextStatement();
    DCHECK_EQ(ParseResult::kSuccess, parse_result.condition);

    const ParseStatement &statement = *parse_result.parsed_statement;

    // Generate the query plan.
    auto query_handle =
        make_unique<QueryHandle>(query_processor_->query_id(), sender, statement.getPriority(),
                                 new QueryHandle::AnalyzeQueryInfo(false /* is_analyze_attribute_query */, rel_id,
                                                                   relations.size()));
    query_processor_->generateQueryHandle(statement, query_handle.get());
    DCHECK(query_handle->getQueryPlanMutable() != nullptr);

    query_handles.push_back(query_handle.release());
  }

  if (!query_handles.empty()) {
    auto request_message = make_unique<AdmitRequestMessage>(query_handles);
    const std::size_t size_of_request_msg = sizeof(*request_message);

    CHECK(MessageBus::SendStatus::kOK ==
        QueryExecutionUtil::SendTMBMessage(&bus_, conductor_client_id_, foreman_->getBusClientID(),
                                           TaggedMessage(request_message.release(), size_of_request_msg,
                                                         kAdmitRequestMessage)));
  }
}

}  // namespace quickstep
