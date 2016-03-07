/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include "query_optimizer/strategy/Join.hpp"

#include <vector>

#include "query_optimizer/LogicalToPhysicalMapper.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/NestedLoopsJoin.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/rules/RuleHelper.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

bool Join::generatePlan(const L::LogicalPtr &logical_input,
                        P::PhysicalPtr *physical_output) {
  L::ProjectPtr logical_project;
  L::FilterPtr logical_filter;
  L::HashJoinPtr logical_hash_join;
  L::NestedLoopsJoinPtr logical_nested_loops_join;

  // Collapse project-join.
  if (L::SomeProject::MatchesWithConditionalCast(logical_input, &logical_project)) {
    if (L::SomeHashJoin::MatchesWithConditionalCast(logical_project->input(),
                                                    &logical_hash_join)) {
      addHashJoin(logical_project,
                  logical_filter,
                  logical_hash_join,
                  physical_output);
      return true;
    }

    if (L::SomeNestedLoopsJoin::MatchesWithConditionalCast(logical_project->input(),
                                                           &logical_nested_loops_join)) {
      addNestedLoopsJoin(logical_nested_loops_join,
                         logical_nested_loops_join->join_predicate(),
                         logical_project->project_expressions(),
                         physical_output);
      return true;
    }
  }

  // Collapse project-filter-join.
  if (logical_project != nullptr &&
      L::SomeFilter::MatchesWithConditionalCast(logical_project->input(), &logical_filter)) {
    if (L::SomeHashJoin::MatchesWithConditionalCast(logical_filter->input(),
                                                    &logical_hash_join)) {
      addHashJoin(logical_project,
                  logical_filter,
                  logical_hash_join,
                  physical_output);
      return true;
    }

    if (L::SomeNestedLoopsJoin::MatchesWithConditionalCast(logical_filter->input(),
                                                           &logical_nested_loops_join)) {
      addNestedLoopsJoin(
          logical_nested_loops_join,
          E::LogicalAnd::Create({logical_filter->filter_predicate(),
                                 logical_nested_loops_join->join_predicate()}),
          logical_project->project_expressions(),
          physical_output);
      return true;
    }
  }

  // Collapse filter-join.
  if (L::SomeFilter::MatchesWithConditionalCast(logical_input, &logical_filter)) {
    if (L::SomeHashJoin::MatchesWithConditionalCast(logical_filter->input(),
                                                    &logical_hash_join)) {
      addHashJoin(logical_project,
                  logical_filter,
                  logical_hash_join,
                  physical_output);
      return true;
    }

    if (L::SomeNestedLoopsJoin::MatchesWithConditionalCast(logical_filter->input(),
                                                           &logical_nested_loops_join)) {
      addNestedLoopsJoin(
          logical_nested_loops_join,
          E::LogicalAnd::Create({logical_filter->filter_predicate(),
                                 logical_nested_loops_join->join_predicate()}),
          CastSharedPtrVector<E::NamedExpression>(
              logical_nested_loops_join->getOutputAttributes()),
          physical_output);
      return true;
    }
  }

  // Convert a single binary join.
  if (L::SomeHashJoin::MatchesWithConditionalCast(logical_input, &logical_hash_join)) {
    addHashJoin(logical_project,
                logical_filter,
                logical_hash_join,
                physical_output);
    return true;
  }
  if (L::SomeNestedLoopsJoin::MatchesWithConditionalCast(logical_input, &logical_nested_loops_join)) {
    addNestedLoopsJoin(
        logical_nested_loops_join,
        logical_nested_loops_join->join_predicate(),
        CastSharedPtrVector<E::NamedExpression>(logical_nested_loops_join->getOutputAttributes()),
        physical_output);
    return true;
  }

  return false;
}

void Join::addHashJoin(const logical::ProjectPtr &logical_project,
                       const logical::FilterPtr &logical_filter,
                       const logical::HashJoinPtr &logical_hash_join,
                       P::PhysicalPtr *physical_output) {
  P::PhysicalPtr left =
      physical_mapper_->createOrGetPhysicalFromLogical(logical_hash_join->left());
  P::PhysicalPtr right =
      physical_mapper_->createOrGetPhysicalFromLogical(logical_hash_join->right());

  std::vector<E::NamedExpressionPtr> project_expressions;
  if (logical_project != nullptr) {
    project_expressions = logical_project->project_expressions();
  } else {
    project_expressions = CastSharedPtrVector<E::NamedExpression>(logical_hash_join->getOutputAttributes());
  }

  std::vector<E::AttributeReferencePtr> left_join_attributes = logical_hash_join->left_join_attributes();
  std::vector<E::AttributeReferencePtr> right_join_attributes = logical_hash_join->right_join_attributes();
  std::vector<E::ExpressionPtr> non_hash_join_predicates;
  if (logical_filter != nullptr) {
    std::vector<E::PredicatePtr> filter_predicates;
    const std::vector<E::AttributeReferencePtr> left_input_attributes =
        logical_hash_join->left()->getOutputAttributes();
    const E::UnorderedNamedExpressionSet left_input_attributes_set(left_input_attributes.begin(),
                                                                   left_input_attributes.end());
#ifdef QUICKSTEP_DEBUG
    const std::vector<E::AttributeReferencePtr> right_input_attributes =
        logical_hash_join->right()->getOutputAttributes();
    const E::UnorderedNamedExpressionSet right_input_attributes_set(right_input_attributes.begin(),
                                                                    right_input_attributes.end());
#endif

    filter_predicates = GetConjunctivePredicates(logical_filter->filter_predicate());
    for (const E::PredicatePtr &filter_predicate : filter_predicates) {
      E::ComparisonExpressionPtr equality_predicate;
      E::AttributeReferencePtr left_attribute;
      E::AttributeReferencePtr right_attribute;
      if (E::SomeComparisonExpression::MatchesWithConditionalCast(filter_predicate,
                                                                  &equality_predicate)
          && equality_predicate->isEqualityComparisonPredicate()
          && E::SomeAttributeReference::MatchesWithConditionalCast(equality_predicate->left(),
                                                                   &left_attribute)
          && E::SomeAttributeReference::MatchesWithConditionalCast(equality_predicate->right(),
                                                                   &right_attribute)) {
        if (left_input_attributes_set.find(left_attribute) != left_input_attributes_set.end()) {
          // Do not need to check the source of right_attribute. It must come from the right logical;
          // otherwise it would have been pushed down.
#ifdef QUICKSTEP_DEBUG
          DCHECK(right_input_attributes_set.find(right_attribute) != right_input_attributes_set.end())
              << logical_filter->toString();
#endif
          left_join_attributes.emplace_back(left_attribute);
          right_join_attributes.emplace_back(right_attribute);
        } else {
#ifdef QUICKSTEP_DEBUG
          DCHECK(left_input_attributes_set.find(right_attribute) != left_input_attributes_set.end())
              << logical_filter->toString();
          DCHECK(right_input_attributes_set.find(left_attribute) != right_input_attributes_set.end())
              << logical_filter->toString();
#endif
          left_join_attributes.emplace_back(right_attribute);
          right_join_attributes.emplace_back(left_attribute);
        }
      } else {
        non_hash_join_predicates.emplace_back(filter_predicate);
      }
    }
  }

  // Pull up the left Selection.
  P::SelectionPtr selection;
  if (P::SomeSelection::MatchesWithConditionalCast(left, &selection) &&
      selection->filter_predicate() == nullptr) {
    const std::vector<E::NamedExpressionPtr> old_project_expressions =
        project_expressions;
    const std::vector<E::ExpressionPtr> old_non_hash_join_predicates =
        non_hash_join_predicates;
    std::vector<E::ExpressionPtr> join_attributes(left_join_attributes.begin(),
                                                  left_join_attributes.end());

    PullUpProjectExpressions(selection->project_expressions(),
                             {&join_attributes, &non_hash_join_predicates} /* non_project_expression_lists */,
                             {&project_expressions} /* project_expression_lists */);

    bool pull_up_selection = true;
    // Do not pull up the Selection if original join attributes have been changed to non-attributes.
    for (const E::ExpressionPtr &new_join_expression : join_attributes) {
      if (!E::SomeAttributeReference::Matches(new_join_expression)) {
        pull_up_selection = false;
        break;
      }
    }

    if (pull_up_selection) {
      left = selection->input();
      left_join_attributes = CastSharedPtrVector<E::AttributeReference>(join_attributes);
    } else {
      non_hash_join_predicates = old_non_hash_join_predicates;
      project_expressions = old_project_expressions;
    }
  }

  // Pull up the right Selection.
  if (P::SomeSelection::MatchesWithConditionalCast(right, &selection) &&
      selection->filter_predicate() == nullptr) {
    const std::vector<E::NamedExpressionPtr> old_project_expressions =
        project_expressions;
    const std::vector<E::ExpressionPtr> old_non_hash_join_predicates =
        non_hash_join_predicates;
    std::vector<E::ExpressionPtr> join_attributes(right_join_attributes.begin(),
                                                  right_join_attributes.end());

    PullUpProjectExpressions(selection->project_expressions(),
                             {&join_attributes, &non_hash_join_predicates} /* non_project_expression_lists */,
                             {&project_expressions} /* project_expression_lists */);

    bool pull_up_selection = true;
    // Do not pull up the Selection if original join attributes have been changed to non-attributes.
    for (const E::ExpressionPtr &new_join_expression : join_attributes) {
      if (!E::SomeAttributeReference::Matches(new_join_expression)) {
        pull_up_selection = false;
        break;
      }
    }

    if (pull_up_selection) {
      right = selection->input();
      right_join_attributes = CastSharedPtrVector<E::AttributeReference>(join_attributes);
    } else {
      non_hash_join_predicates = old_non_hash_join_predicates;
      project_expressions = old_project_expressions;
    }
  }

  E::PredicatePtr residual_predicate;
  if (non_hash_join_predicates.size() == 1u) {
    residual_predicate = std::static_pointer_cast<const E::Predicate>(non_hash_join_predicates[0]);
  } else if (non_hash_join_predicates.size() > 1u) {
    residual_predicate = E::LogicalAnd::Create(CastSharedPtrVector<E::Predicate>(non_hash_join_predicates));
  }

  *physical_output =
      P::HashJoin::Create(left,
                          right,
                          left_join_attributes,
                          right_join_attributes,
                          residual_predicate,
                          project_expressions);
}

void Join::addNestedLoopsJoin(const L::NestedLoopsJoinPtr &logical_nested_loops_join,
                              expressions::PredicatePtr join_predicate,
                              std::vector<expressions::NamedExpressionPtr> project_expressions,
                              physical::PhysicalPtr *physical_output) {
  P::PhysicalPtr left =
      physical_mapper_->createOrGetPhysicalFromLogical(logical_nested_loops_join->left());
  P::PhysicalPtr right =
      physical_mapper_->createOrGetPhysicalFromLogical(logical_nested_loops_join->right());

  // Pull up the left Selection.
  P::SelectionPtr selection;
  std::vector<E::ExpressionPtr> predicate_to_update{join_predicate};
  if (P::SomeSelection::MatchesWithConditionalCast(left, &selection) &&
      selection->filter_predicate() == nullptr) {
    const std::vector<E::NamedExpressionPtr> old_project_expressions =
        project_expressions;
    PullUpProjectExpressions(selection->project_expressions(),
                             {&predicate_to_update} /* non_project_expression_lists */,
                             {&project_expressions} /* project_expression_lists */);
    left = selection->input();
  }

  // Pull up the right Selection.
  if (P::SomeSelection::MatchesWithConditionalCast(right, &selection) &&
      selection->filter_predicate() == nullptr) {
    const std::vector<E::ExpressionPtr> old_predicate_to_update =
        predicate_to_update;
    const std::vector<E::NamedExpressionPtr> old_project_expressions =
        project_expressions;
    PullUpProjectExpressions(selection->project_expressions(),
                             {&predicate_to_update} /* non_project_expression_lists */,
                             {&project_expressions} /* project_expression_lists */);
    right = selection->input();
  }

  DCHECK(E::SomePredicate::Matches(predicate_to_update[0]));

  *physical_output = P::NestedLoopsJoin::Create(
      left,
      right,
      std::static_pointer_cast<const E::Predicate>(predicate_to_update[0]),
      project_expressions);
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
