/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/strategy/Aggregate.hpp"

#include <vector>

#include "query_optimizer/LogicalToPhysicalMapper.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Aggregate.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/rules/RuleHelper.hpp"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

bool Aggregate::generatePlan(const L::LogicalPtr &logical_input,
                             P::PhysicalPtr *physical_output) {
  L::AggregatePtr logical_aggregate;

  if (!L::SomeAggregate::MatchesWithConditionalCast(logical_input, &logical_aggregate)) {
    return false;
  }

  std::vector<E::NamedExpressionPtr> grouping_expressions =
      logical_aggregate->grouping_expressions();
  std::vector<E::NamedExpressionPtr> aggregate_expressions =
      E::ToNamedExpressions(logical_aggregate->aggregate_expressions());

  P::PhysicalPtr best_input_plan =
      physical_mapper_->createOrGetPhysicalFromLogical(
          logical_aggregate->input());

  // Pull up the child Selection.
  P::SelectionPtr input_selection;
  E::PredicatePtr filter_predicate;
  if (P::SomeSelection::MatchesWithConditionalCast(best_input_plan, &input_selection)) {
    filter_predicate = input_selection->filter_predicate();
    PullUpProjectExpressions(
        input_selection->project_expressions(),
        {} /* non_project_expression_lists */,
        {&aggregate_expressions,
         &grouping_expressions} /* project_expression_lists */);
    best_input_plan = input_selection->input();
  }

  std::vector<E::AliasPtr> typed_aggregate_expressions;
  for (const E::NamedExpressionPtr& aggregate_expression : aggregate_expressions) {
    typed_aggregate_expressions.emplace_back();
    CHECK(E::SomeAlias::MatchesWithConditionalCast(aggregate_expression,
                                                   &typed_aggregate_expressions.back()))
        << logical_aggregate->toString();
  }

  *physical_output = P::Aggregate::Create(
      best_input_plan,
      grouping_expressions,
      typed_aggregate_expressions,
      filter_predicate);
  return true;
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
