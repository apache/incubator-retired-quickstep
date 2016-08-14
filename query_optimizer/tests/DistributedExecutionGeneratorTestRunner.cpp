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

#include "query_optimizer/tests/DistributedExecutionGeneratorTestRunner.hpp"

#include <cstdio>
#include <set>
#include <string>

#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/Optimizer.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "utility/MemStream.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {

const char *DistributedExecutionGeneratorTestRunner::kResetOption =
    "reset_before_execution";

void DistributedExecutionGeneratorTestRunner::runTestCase(
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
    }

    const ParseStatement &parse_statement = *result.parsed_statement;
    std::printf("%s\n", parse_statement.toString().c_str());
    try {
      QueryHandle query_handle(query_id_++, cli_id_);

      Optimizer optimizer(test_database_loader_.catalog_database(),
                          test_database_loader_.storage_manager());
      optimizer.generateQueryHandle(parse_statement, &query_handle);

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

  if (output->empty()) {
    *output = output_stream.str();
  }
}

}  // namespace optimizer
}  // namespace quickstep
