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

#include <stack>

#include "query_optimizer/rules/ReferencedBaseRelations.hpp"

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/rules/UnnestSubqueries.hpp"

namespace quickstep {
namespace optimizer {

class OptimizerContext;

namespace L = ::quickstep::optimizer::logical;

void ReferencedBaseRelations::applyToNode(const L::LogicalPtr &input) {
  L::TableReferencePtr table_reference;
  const CatalogRelation *input_relation = nullptr;
  if (L::SomeTableReference::MatchesWithConditionalCast(input, &table_reference)) {
    input_relation = table_reference->catalog_relation();
    referenced_base_relations_[input_relation->getID()] = input_relation;
  }
}

L::LogicalPtr ReferencedBaseRelations::apply(const L::LogicalPtr &input) {
  L::LogicalPtr unnested_tree = UnnestSubqueries(optimizer_context_).apply(input);

  std::stack<L::LogicalPtr> to_be_visited;
  to_be_visited.push(unnested_tree);

  while (!to_be_visited.empty()) {
    const L::LogicalPtr &curr_node = to_be_visited.top();
    to_be_visited.pop();

    applyToNode(curr_node);

    for (auto child : curr_node->children()) {
      to_be_visited.push(child);
    }
  }
  return input;
}

}  // namespace optimizer
}  // namespace quickstep
