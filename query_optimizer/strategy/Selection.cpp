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

#include "query_optimizer/strategy/Selection.hpp"

#include <map>
#include <vector>

#include "query_optimizer/LogicalToPhysicalMapper.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/rules/RuleHelper.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

bool Selection::generatePlan(const L::LogicalPtr &logical_input,
                             P::PhysicalPtr *physical_output) {
  L::FilterPtr logical_filter;
  L::ProjectPtr logical_project;

  if (L::SomeProject::MatchesWithConditionalCast(logical_input, &logical_project)) {
    E::PredicatePtr filter_predicate;
    L::LogicalPtr logical_input_after_collapse = logical_project->input();
    // Collapse Project and Filter.
    if (L::SomeFilter::MatchesWithConditionalCast(logical_input_after_collapse, &logical_filter)) {
      logical_input_after_collapse = logical_filter->input();
      filter_predicate = logical_filter->filter_predicate();
    }
    addSelection(logical_input_after_collapse, filter_predicate,
                 logical_project->project_expressions(), physical_output);
    return true;
  } else if (L::SomeFilter::MatchesWithConditionalCast(logical_input, &logical_filter)) {
    addSelection(
        logical_filter->input(), logical_filter->filter_predicate(),
        CastSharedPtrVector<E::NamedExpression>(logical_filter->getOutputAttributes()),
        physical_output);
    return true;
  }

  return false;
}

void Selection::addSelection(
    const L::LogicalPtr &logical_input_in,
    const E::PredicatePtr &filter_predicate_in,
    const std::vector<E::NamedExpressionPtr> &project_expressions_in,
    P::PhysicalPtr *physical_output) {
  DCHECK(!project_expressions_in.empty());

  P::PhysicalPtr physical_input =
      physical_mapper_->createOrGetPhysicalFromLogical(logical_input_in);

  E::PredicatePtr filter_predicate = filter_predicate_in;
  std::vector<E::NamedExpressionPtr> project_expressions =
      project_expressions_in;

  P::SelectionPtr child_selection;
  // Pull up the child Selection.
  if (P::SomeSelection::MatchesWithConditionalCast(physical_input, &child_selection)) {
    std::vector<E::ExpressionPtr> new_filter_predicates;
    if (filter_predicate != nullptr) {
      new_filter_predicates.push_back(filter_predicate);
    }

    E::PredicatePtr child_filter_predicate =
        child_selection->filter_predicate();
    physical_input = child_selection->input();
    PullUpProjectExpressions(
        child_selection->project_expressions(),
        {&new_filter_predicates} /* non_project_expressions */,
        {&project_expressions} /* project_expressions */);

    if (filter_predicate != nullptr) {
      CHECK(E::SomePredicate::MatchesWithConditionalCast(new_filter_predicates[0],
                                                         &filter_predicate))
          << new_filter_predicates[0]->toString();
      if (child_filter_predicate != nullptr) {
        filter_predicate = E::LogicalAnd::Create(
            {filter_predicate, child_filter_predicate} /* operands */);
      }
    } else {
      filter_predicate = child_filter_predicate;
    }
  }

  *physical_output = P::Selection::Create(physical_input, project_expressions,
                                          filter_predicate);
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
