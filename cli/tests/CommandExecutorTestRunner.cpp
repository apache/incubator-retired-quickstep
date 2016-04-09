/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "cli/tests/CommandExecutorTestRunner.hpp"

#include <cstdio>
#include <set>
#include <string>
#include <utility>

#include "cli/CommandExecutor.hpp"
#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "query_execution/AdmitRequestMessage.hpp"
#include "query_execution/Foreman.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/Worker.hpp"
#include "query_optimizer/ExecutionGenerator.hpp"
#include "query_optimizer/LogicalGenerator.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/PhysicalGenerator.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"
#include "utility/MemStream.hpp"
#include "utility/PtrList.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

#include "tmb/tagged_message.h"

namespace quickstep {

class CatalogRelation;

namespace O = ::quickstep::optimizer;
namespace P = ::quickstep::optimizer::physical;
namespace L = ::quickstep::optimizer::logical;

const char CommandExecutorTestRunner::kResetOption[] =
    "reset_before_execution";

void CommandExecutorTestRunner::runTestCase(
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

    O::OptimizerContext optimizer_context(0 /* query_id */,
                                          test_database_loader_.catalog_database(),
                                          test_database_loader_.storage_manager());

    if (result.condition != ParseResult::kSuccess) {
      if (result.condition == ParseResult::kError) {
        *output = result.error_message;
      }
      break;
    } else {
      std::printf("%s\n", result.parsed_statement->toString().c_str());
      try {
        if (result.parsed_statement->getStatementType() == ParseStatement::kCommand) {
          quickstep::cli::executeCommand(
              *result.parsed_statement,
              *(test_database_loader_.catalog_database()),
              main_thread_client_id_,
              foreman_->getBusClientID(),
              &bus_,
              test_database_loader_.storage_manager(),
              nullptr,
              output_stream.file());
        } else  {
          QueryHandle query_handle(optimizer_context.query_id());
          O::LogicalGenerator logical_generator(&optimizer_context);
          O::PhysicalGenerator physical_generator;
          O::ExecutionGenerator execution_generator(&optimizer_context, &query_handle);
          const P::PhysicalPtr physical_plan =
              physical_generator.generatePlan(
                  logical_generator.generatePlan(*result.parsed_statement));
          execution_generator.generatePlan(physical_plan);

          AdmitRequestMessage request_message(&query_handle);
          TaggedMessage admit_tagged_message(
              &request_message, sizeof(request_message), kAdmitRequestMessage);
          QueryExecutionUtil::SendTMBMessage(&bus_,
                                             main_thread_client_id_,
                                             foreman_->getBusClientID(),
                                             std::move(admit_tagged_message));

          // Receive workload completion message from Foreman.
          const AnnotatedMessage annotated_msg =
              bus_.Receive(main_thread_client_id_, 0, true);
          const TaggedMessage &tagged_message = annotated_msg.tagged_message;
          DCHECK_EQ(kWorkloadCompletionMessage, tagged_message.message_type());
          const CatalogRelation *query_result_relation = query_handle.getQueryResultRelation();
          if (query_result_relation) {
            PrintToScreen::PrintRelation(*query_result_relation,
                                         test_database_loader_.storage_manager(),
                                         output_stream.file());
            DropRelation::Drop(*query_result_relation,
                               test_database_loader_.catalog_database(),
                               test_database_loader_.storage_manager());
          }
        }
      } catch (const SqlError &error) {
        *output = error.formatMessage(input);
        break;
      }
    }
  }

  if (output->empty()) {
    *output = output_stream.str();
  }
}

}  // namespace quickstep
