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

#include "query_optimizer/tests/ExecutionGeneratorTestRunner.hpp"

#include <cstdio>
#include <memory>
#include <set>
#include <string>

#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "query_execution/ForemanSingleNode.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_optimizer/Optimizer.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "utility/MemStream.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {

const std::string ExecutionGeneratorTestRunner::kResetOption =
    "reset_before_execution";

void ExecutionGeneratorTestRunner::runTestCase(
    const std::string &input, const std::set<std::string> &options,
    std::string *output) {
  // TODO(qzeng): Test multi-threaded query execution when we have a Sort operator.

  VLOG(4) << "Test SQL(s): " << input;

  if (options.find(kResetOption) != options.end()) {
    test_database_loader_.clear();
    test_database_loader_.createTestRelation(false /* allow_vchar */);
    test_database_loader_.loadTestRelation();
  }

  MemStream output_stream;
  sql_parser_.feedNextBuffer(new std::string(input));

  while (true) {
    ParseResult result = sql_parser_.getNextStatement();
    if (result.condition != ParseResult::kSuccess) {
      if (result.condition == ParseResult::kError) {
        *output = result.error_message;
      }
      break;
    } else {
      const ParseStatement &parse_statement = *result.parsed_statement;
      std::printf("%s\n", parse_statement.toString().c_str());

      const CatalogRelation *query_result_relation = nullptr;
      try {
        OptimizerContext optimizer_context;
        auto query_handle = std::make_unique<QueryHandle>(0 /* query_id */, main_thread_client_id_);

        optimizer_.generateQueryHandle(parse_statement,
                                       test_database_loader_.catalog_database(),
                                       &optimizer_context,
                                       query_handle.get());
        query_result_relation = query_handle->getQueryResultRelation();

        QueryExecutionUtil::ConstructAndSendAdmitRequestMessage(
            main_thread_client_id_,
            foreman_->getBusClientID(),
            query_handle.release(),
            &bus_);
      } catch (const SqlError &error) {
        *output = error.formatMessage(input);
        break;
      }

      QueryExecutionUtil::ReceiveQueryCompletionMessage(
          main_thread_client_id_, &bus_);

      if (query_result_relation) {
        PrintToScreen::PrintRelation(*query_result_relation,
                                     test_database_loader_.storage_manager(),
                                     output_stream.file());
        DropRelation::Drop(*query_result_relation,
                           test_database_loader_.catalog_database(),
                           test_database_loader_.storage_manager());
      }
    }
  }

  if (output->empty()) {
    *output = output_stream.str();
  }
}

}  // namespace optimizer
}  // namespace quickstep
