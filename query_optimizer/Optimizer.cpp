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
