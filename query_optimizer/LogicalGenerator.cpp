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

#include "query_optimizer/LogicalGenerator.hpp"

#include <memory>
#include <vector>

#include "parser/ParseStatement.hpp"

#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/Validator.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/resolver/Resolver.hpp"
#include "query_optimizer/rules/CollapseProject.hpp"
#include "query_optimizer/rules/GenerateJoins.hpp"
#include "query_optimizer/rules/PushDownFilter.hpp"
#include "query_optimizer/rules/PushDownSemiAntiJoin.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/UnnestSubqueries.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace L = ::quickstep::optimizer::logical;

LogicalGenerator::LogicalGenerator(OptimizerContext *optimizer_context)
    : optimizer_context_(optimizer_context) {}

LogicalGenerator::~LogicalGenerator() {}

L::LogicalPtr LogicalGenerator::generatePlan(
    const ParseStatement &parse_statement) {
  resolver::Resolver resolver(optimizer_context_);
  DVLOG(4) << "Parse tree:\n" << parse_statement.toString();
  logical_plan_ = resolver.resolve(parse_statement);
  DVLOG(4) << "Initial logical plan:\n" << logical_plan_->toString();

  optimizePlan();
  DVLOG(4) << "Optimized logical plan:\n" << logical_plan_->toString();

  return logical_plan_;
}

void LogicalGenerator::optimizePlan() {
  std::vector<std::unique_ptr<Rule<L::Logical>>> rules;
  if (optimizer_context_->has_nested_queries()) {
    rules.emplace_back(new UnnestSubqueries(optimizer_context_));
  }
  rules.emplace_back(new PushDownSemiAntiJoin());
  rules.emplace_back(new PushDownFilter());
  rules.emplace_back(new GenerateJoins());
  rules.emplace_back(new PushDownFilter());
  rules.emplace_back(new CollapseProject());

  for (std::unique_ptr<Rule<L::Logical>> &rule : rules) {
    logical_plan_ = rule->apply(logical_plan_);
    DVLOG(5) << "After applying rule " << rule->getName() << ":\n"
             << logical_plan_->toString();
  }

#ifdef QUICKSTEP_DEBUG
  Validate(logical_plan_);
#endif
}

}  // namespace optimizer
}  // namespace quickstep
