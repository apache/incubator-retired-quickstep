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

#include "query_optimizer/rules/CollapseSelection.hpp"

#include <vector>

#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/rules/RuleHelper.hpp"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr CollapseSelection::applyToNode(const P::PhysicalPtr &input) {
  P::SelectionPtr selection;
  P::SelectionPtr child_selection;

  if (P::SomeSelection::MatchesWithConditionalCast(input, &selection) &&
      P::SomeSelection::MatchesWithConditionalCast(selection->input(), &child_selection)) {
    E::PredicatePtr filter_predicate = selection->filter_predicate();

    std::vector<E::ExpressionPtr> non_project_expressions;
    if (filter_predicate) {
      non_project_expressions.push_back(filter_predicate);
    }

    std::vector<E::NamedExpressionPtr> project_expressions =
        selection->project_expressions();
    PullUpProjectExpressions(child_selection->project_expressions(),
                             { &non_project_expressions }, { &project_expressions });

    const E::PredicatePtr &child_filter_predicate = child_selection->filter_predicate();
    if (filter_predicate) {
      CHECK(E::SomePredicate::MatchesWithConditionalCast(non_project_expressions[0],
                                                         &filter_predicate))
          << non_project_expressions[0]->toString();
      if (child_filter_predicate) {
        filter_predicate = E::LogicalAnd::Create({ filter_predicate, child_filter_predicate });
      }
    } else {
      filter_predicate = child_filter_predicate;
    }

    return P::Selection::Create(child_selection->input(),
                                project_expressions,
                                filter_predicate,
                                selection->cloneOutputPartitionSchemeHeader());
  }

  return input;
}

}  // namespace optimizer
}  // namespace quickstep
