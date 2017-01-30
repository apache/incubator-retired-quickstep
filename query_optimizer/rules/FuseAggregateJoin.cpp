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

#include "query_optimizer/rules/FuseAggregateJoin.hpp"

#include <algorithm>
#include <cstddef>
#include <unordered_set>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/CrossReferenceCoalesceAggregate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr FuseAggregateJoin::applyToNode(
    const P::PhysicalPtr &node) {
  // Currently we consider only Aggregate on HashLeftOuterJoin.
  P::AggregatePtr aggregate;
  if (!P::SomeAggregate::MatchesWithConditionalCast(node, &aggregate) ||
      aggregate->filter_predicate() != nullptr) {
    return node;
  }

  P::HashJoinPtr hash_join;
  if ((!P::SomeHashJoin::MatchesWithConditionalCast(aggregate->input(), &hash_join)) ||
      hash_join->join_type() != P::HashJoin::JoinType::kLeftOuterJoin ||
      hash_join->residual_predicate() != nullptr) {
    return node;
  }

  // Single left join attribute with unique values.
  const std::vector<E::AttributeReferencePtr> &left_join_attributes =
      hash_join->left_join_attributes();
  if (left_join_attributes.size() != 1u ||
      (!cost_model_->impliesUniqueAttributes(hash_join->left(), left_join_attributes))) {
    return node;
  }

  // Single group-by attribute that is the same as the right join attribute.
  const std::vector<E::NamedExpressionPtr> &grouping_expressions =
      aggregate->grouping_expressions();
  if (grouping_expressions.size() != 1u ||
      grouping_expressions.front()->id() != left_join_attributes.front()->id()) {
    return node;
  }

  std::unordered_set<E::ExprId> right_side_attr_ids;
  for (const auto &attr : hash_join->right()->getOutputAttributes()) {
    right_side_attr_ids.insert(attr->id());
  }

  // Aggregate expressions only depend on attributes from the right child.
  const std::vector<E::AliasPtr> &aggregate_expressions =
      aggregate->aggregate_expressions();
  for (const auto &expr : aggregate_expressions) {
    const E::AggregateFunctionPtr aggr_expr =
        std::static_pointer_cast<const E::AggregateFunction>(expr->expression());

    const std::vector<E::ScalarPtr> &arguments = aggr_expr->getArguments();
    if (arguments.size() != 1u) {
      return node;
    }

    E::AttributeReferencePtr arg_attr;
    if (!E::SomeAttributeReference::MatchesWithConditionalCast(arguments.front(), &arg_attr) ||
        right_side_attr_ids.find(arg_attr->id()) == right_side_attr_ids.end()) {
      return node;
    }
  }

  // Collision-free vector aggregation is applicable, and both the left and right
  // join attributes are range-bounded integer values.
  const std::size_t estimated_num_groups =
      cost_model_->estimateNumGroupsForAggregate(aggregate);

  std::size_t max_num_groups_left;
  if (!cost_model_->canUseCollisionFreeAggregation(aggregate,
                                                   estimated_num_groups,
                                                   &max_num_groups_left)) {
    return node;
  }

  std::size_t max_num_groups_right;
  if (!cost_model_->canUseCollisionFreeAggregation(
           P::Aggregate::Create(hash_join->right(),
                                E::ToNamedExpressions(hash_join->right_join_attributes()),
                                aggregate->aggregate_expressions(),
                                nullptr),
           estimated_num_groups,
           &max_num_groups_right)) {
    return node;
  }

  // Fuse right child's filter predicate.
  P::PhysicalPtr right_child = hash_join->right();
  const std::vector<E::AttributeReferencePtr> &right_join_attributes =
      hash_join->right_join_attributes();
  E::PredicatePtr right_filter_predicate = nullptr;

  P::SelectionPtr selection;
  if (P::SomeSelection::MatchesWithConditionalCast(right_child, &selection)) {
    if (E::SubsetOfExpressions(right_join_attributes,
                               selection->input()->getOutputAttributes())) {
      right_child = selection->input();
      right_filter_predicate = selection->filter_predicate();
    }
  }

  const std::size_t max_num_groups =
      std::max(max_num_groups_left, max_num_groups_right);

  return P::CrossReferenceCoalesceAggregate::Create(hash_join->left(),
                                                    right_child,
                                                    left_join_attributes,
                                                    right_join_attributes,
                                                    right_filter_predicate,
                                                    aggregate_expressions,
                                                    max_num_groups);
}

void FuseAggregateJoin::init(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  const P::TopLevelPlanPtr top_level_plan =
      std::static_pointer_cast<const P::TopLevelPlan>(input);
  cost_model_.reset(
      new cost::StarSchemaSimpleCostModel(top_level_plan->shared_subplans()));
}

}  // namespace optimizer
}  // namespace quickstep
