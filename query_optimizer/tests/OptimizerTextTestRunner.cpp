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

#include "query_optimizer/tests/OptimizerTextTestRunner.hpp"

#include <set>
#include <string>
#include <vector>

#include "query_optimizer/LogicalGenerator.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/PhysicalGenerator.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/resolver/Resolver.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

const std::vector<std::string> OptimizerTextTestRunner::kTestOptions = {
    "initial_logical_plan", "optimized_logical_plan", "physical_plan"};

void OptimizerTextTestRunner::runTestCase(const std::string &input,
                                          const std::set<std::string> &options,
                                          std::string *output) {
  CHECK(!options.empty()) << "No options specified";
  VLOG(4) << "Test SQL: " << input;

  sql_parser_.feedNextBuffer(new std::string(input));
  ParseResult result = sql_parser_.getNextStatement();

  OptimizerContext optimizer_context;
  if (result.condition != ParseResult::kSuccess) {
    *output = result.error_message;
  } else {
    // Number of options specified.
    int num_options = 0;
    std::ostringstream out_ss;
    try {
      logical::LogicalPtr initial_logical_plan;
      logical::LogicalPtr optimized_logical_plan;
      physical::PhysicalPtr physical_plan;

      const bool output_initial_logical_plan =
          (options.find(kTestOptions[0]) != options.end());
      const bool output_optimized_logical_plan =
          (options.find(kTestOptions[1]) != options.end());
      const bool output_physical_plan =
          (options.find(kTestOptions[2]) != options.end());
      if (output_initial_logical_plan) {
        initial_logical_plan =
            resolveParseTree(*result.parsed_statement, &optimizer_context);
        ++num_options;
      }
      if (output_optimized_logical_plan || output_physical_plan) {
        optimized_logical_plan =
            generateLogicalPlan(*result.parsed_statement, &optimizer_context);
        if (output_optimized_logical_plan) {
          ++num_options;
        }
      }
      if (output_physical_plan) {
        physical_plan =
            generatePhysicalPlan(optimized_logical_plan, &optimizer_context);
        ++num_options;
      }

      if (output_initial_logical_plan) {
        if (num_options > 1) {
          out_ss << "[Initial Logical Plan]\n";
        }
        out_ss << initial_logical_plan->toString();
      }
      if (output_optimized_logical_plan) {
        if (num_options > 1) {
          out_ss << "[Optimized Logical Plan]\n";
        }
        out_ss << optimized_logical_plan->toString();
      }
      if (output_physical_plan) {
        if (num_options > 1) {
          out_ss << "[Physical Plan]\n";
        }
        out_ss << physical_plan->toString();
      }
      *output = out_ss.str();
      CHECK(!output->empty());
    } catch (const SqlError &error) {
      *output = error.formatMessage(input);
    }
  }
}

logical::LogicalPtr OptimizerTextTestRunner::resolveParseTree(
    const ParseStatement &parse_statement,
    OptimizerContext *optimizer_context) {
  resolver::Resolver resolver(*test_database_loader_.catalog_database(), optimizer_context);
  return resolver.resolve(parse_statement);
}

logical::LogicalPtr OptimizerTextTestRunner::generateLogicalPlan(
    const ParseStatement &parse_statement,
    OptimizerContext *optimizer_context) {
  LogicalGenerator logical_generator(optimizer_context);
  return logical_generator.generatePlan(*test_database_loader_.catalog_database(),
                                        parse_statement);
}

physical::PhysicalPtr OptimizerTextTestRunner::generatePhysicalPlan(
    const logical::LogicalPtr &logical_plan,
    OptimizerContext *optimizer_context) {
  PhysicalGenerator physical_generator(optimizer_context);
  return physical_generator.generatePlan(logical_plan);
}

}  // namespace optimizer
}  // namespace quickstep
