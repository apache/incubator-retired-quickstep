/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/Optimizer.hpp"

#include "query_optimizer/ExecutionGenerator.hpp"
#include "query_optimizer/LogicalGenerator.hpp"
#include "query_optimizer/PhysicalGenerator.hpp"

namespace quickstep {
namespace optimizer {

void Optimizer::generateQueryHandle(const ParseStatement &parse_statement,
                                    QueryHandle *query_handle) {
  LogicalGenerator logical_generator(&optimizer_context_);
  PhysicalGenerator physical_generator;
  ExecutionGenerator execution_generator(&optimizer_context_, query_handle);

  execution_generator.generatePlan(
      physical_generator.generatePlan(
          logical_generator.generatePlan(parse_statement)));
}

}  // namespace optimizer
}  // namespace quickstep
