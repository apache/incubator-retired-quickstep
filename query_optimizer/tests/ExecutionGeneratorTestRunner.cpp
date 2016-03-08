/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include "query_optimizer/tests/ExecutionGeneratorTestRunner.hpp"

#include <cstdio>
#include <set>
#include <string>

#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "query_execution/Foreman.hpp"
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

    OptimizerContext optimizer_context(0 /* query_id */,
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
        QueryHandle query_handle;
        LogicalGenerator logical_generator(&optimizer_context);
        PhysicalGenerator physical_generator;
        ExecutionGenerator execution_generator(&optimizer_context,
                                               &query_handle);

        const physical::PhysicalPtr physical_plan =
            physical_generator.generatePlan(
                logical_generator.generatePlan(*result.parsed_statement));
        execution_generator.generatePlan(physical_plan);
        foreman_->setQueryPlan(
            query_handle.getQueryPlanMutable()->getQueryPlanDAGMutable());

        foreman_->reconstructQueryContextFromProto(query_handle.getQueryContextProto());

        foreman_->start();
        foreman_->join();

        const CatalogRelation *query_result_relation = query_handle.getQueryResultRelation();
        if (query_result_relation) {
            PrintToScreen::PrintRelation(*query_result_relation,
                                         test_database_loader_.storage_manager(),
                                         output_stream.file());
            DropRelation::Drop(*query_result_relation,
                               test_database_loader_.catalog_database(),
                               test_database_loader_.storage_manager());
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

}  // namespace optimizer
}  // namespace quickstep
