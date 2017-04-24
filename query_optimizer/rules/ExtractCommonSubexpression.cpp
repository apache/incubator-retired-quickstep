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

#include "query_optimizer/rules/ExtractCommonSubexpression.hpp"

#include <cstddef>
#include <memory>
#include <unordered_set>
#include <vector>

#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/CommonSubexpression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "utility/HashError.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

ExtractCommonSubexpression::ExtractCommonSubexpression(
    OptimizerContext *optimizer_context)
    : optimizer_context_(optimizer_context) {
  const std::vector<E::ExpressionType> homogeneous_expr_types = {
      E::ExpressionType::kAlias,
      E::ExpressionType::kAttributeReference,
      E::ExpressionType::kBinaryExpression,
      E::ExpressionType::kCast,
      E::ExpressionType::kCommonSubexpression,
      E::ExpressionType::kScalarLiteral,
      E::ExpressionType::kUnaryExpression
  };

  for (const auto &expr_type : homogeneous_expr_types) {
    homogeneous_expression_types_.emplace(expr_type);
  }
}

P::PhysicalPtr ExtractCommonSubexpression::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  return applyInternal(input);
}

P::PhysicalPtr ExtractCommonSubexpression::applyInternal(
    const P::PhysicalPtr &input) {
  // First process all child nodes.
  std::vector<P::PhysicalPtr> new_children;
  for (const auto &child : input->children()) {
    new_children.emplace_back(applyInternal(child));
  }

  const P::PhysicalPtr node =
      new_children == input->children()
          ? input
          : input->copyWithNewChildren(new_children);

  // Process expressions of the current node.
  switch (node->getPhysicalType()) {
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr aggregate =
          std::static_pointer_cast<const P::Aggregate>(node);

      std::vector<E::ExpressionPtr> expressions;
      // Gather grouping expressions and aggregate functions' argument expressions.
      for (const auto &expr : aggregate->grouping_expressions()) {
        expressions.emplace_back(expr);
      }
      for (const auto &expr : aggregate->aggregate_expressions()) {
        const E::AggregateFunctionPtr &func =
            std::static_pointer_cast<const E::AggregateFunction>(expr->expression());
        for (const auto &arg : func->getArguments()) {
          expressions.emplace_back(arg);
        }
      }

      // Transform the expressions so that common subexpressions are labelled.
      const std::vector<E::ExpressionPtr> new_expressions =
          transformExpressions(expressions);

      if (new_expressions != expressions) {
        std::vector<E::AliasPtr> new_aggregate_expressions;
        std::vector<E::NamedExpressionPtr> new_grouping_expressions;

        // Reconstruct grouping expressions.
        const std::size_t num_grouping_expressions =
            aggregate->grouping_expressions().size();
        for (std::size_t i = 0; i < num_grouping_expressions; ++i) {
          DCHECK(E::SomeNamedExpression::Matches(new_expressions[i]));
          new_grouping_expressions.emplace_back(
              std::static_pointer_cast<const E::NamedExpression>(new_expressions[i]));
        }

        // Reconstruct aggregate expressions.
        auto it = new_expressions.begin() + num_grouping_expressions;
        for (const auto &expr : aggregate->aggregate_expressions()) {
          const E::AggregateFunctionPtr &func =
              std::static_pointer_cast<const E::AggregateFunction>(
                  expr->expression());

          std::vector<E::ScalarPtr> new_arguments;
          for (std::size_t i = 0; i < func->getArguments().size(); ++i, ++it) {
            DCHECK(E::SomeScalar::Matches(*it));
            new_arguments.emplace_back(std::static_pointer_cast<const E::Scalar>(*it));
          }

          if (new_arguments == func->getArguments()) {
            new_aggregate_expressions.emplace_back(expr);
          } else {
            const E::AggregateFunctionPtr new_func =
                E::AggregateFunction::Create(func->getAggregate(),
                                             new_arguments,
                                             func->is_vector_aggregate(),
                                             func->is_distinct());
            new_aggregate_expressions.emplace_back(
                E::Alias::Create(expr->id(),
                                 new_func,
                                 expr->attribute_name(),
                                 expr->attribute_alias(),
                                 expr->relation_name()));
          }
        }
        return P::Aggregate::Create(aggregate->input(),
                                    new_grouping_expressions,
                                    new_aggregate_expressions,
                                    aggregate->filter_predicate());
      }
      break;
    }
    case P::PhysicalType::kSelection: {
      const P::SelectionPtr selection =
          std::static_pointer_cast<const P::Selection>(node);

      // Transform Selection's project expressions.
      const std::vector<E::NamedExpressionPtr> new_expressions =
          DownCast<E::NamedExpression>(
              transformExpressions(UpCast(selection->project_expressions())));

      if (new_expressions != selection->project_expressions()) {
        return P::Selection::Create(selection->input(),
                                    new_expressions,
                                    selection->filter_predicate());
      }
      break;
    }
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr hash_join =
          std::static_pointer_cast<const P::HashJoin>(node);

      // Transform HashJoin's project expressions.
      const std::vector<E::NamedExpressionPtr> new_expressions =
          DownCast<E::NamedExpression>(
              transformExpressions(UpCast(hash_join->project_expressions())));

      if (new_expressions != hash_join->project_expressions()) {
        return P::HashJoin::Create(hash_join->left(),
                                   hash_join->right(),
                                   hash_join->left_join_attributes(),
                                   hash_join->right_join_attributes(),
                                   hash_join->residual_predicate(),
                                   new_expressions,
                                   hash_join->join_type());
      }
      break;
    }
    case P::PhysicalType::kNestedLoopsJoin: {
      const P::NestedLoopsJoinPtr nested_loops_join =
          std::static_pointer_cast<const P::NestedLoopsJoin>(node);

      // Transform NestedLoopsJoin's project expressions.
      const std::vector<E::NamedExpressionPtr> new_expressions =
          DownCast<E::NamedExpression>(
              transformExpressions(UpCast(nested_loops_join->project_expressions())));

      if (new_expressions != nested_loops_join->project_expressions()) {
        return P::NestedLoopsJoin::Create(nested_loops_join->left(),
                                          nested_loops_join->right(),
                                          nested_loops_join->join_predicate(),
                                          new_expressions);
      }
      break;
    }
    default:
      break;
  }

  return node;
}

std::vector<E::ExpressionPtr> ExtractCommonSubexpression::transformExpressions(
    const std::vector<E::ExpressionPtr> &expressions) {
  // Step 1. For each subexpression, count the number of its occurrences.
  ScalarCounter counter;
  ScalarHashable hashable;
  for (const auto &expr : expressions) {
    visitAndCount(expr, &counter, &hashable);
  }

  // Note that any subexpression with count > 1 is a common subexpression.
  // However, it is not necessary to create a CommonSubexpression node for every
  // such subexpression. E.g. consider the case
  // --
  //   SELECT (x+1)*(x+2), (x+1)*(x+2)*3 FROM s;
  // --
  // We only need to create one *dominant* CommonSubexpression (x+1)*(x+2) and
  // do not need to create the child (x+1) or (x+2) nodes.
  //
  // To address this issue. We define that a subtree S *dominates* its descendent
  // subtree (or leaf node) T if and only if counter[S] >= counter[T].
  //
  // Then we create CommonSubexpression nodes for every subexpression that is
  // not dominated by any ancestor.

  ScalarMap substitution_map;
  std::vector<E::ExpressionPtr> new_expressions;
  for (const auto &expr : expressions) {
    new_expressions.emplace_back(
        visitAndTransform(expr, 1, counter, hashable, &substitution_map));
  }
  return new_expressions;
}

E::ExpressionPtr ExtractCommonSubexpression::transformExpression(
    const E::ExpressionPtr &expression) {
  return transformExpressions({expression}).front();
}

bool ExtractCommonSubexpression::visitAndCount(
    const E::ExpressionPtr &expression,
    ScalarCounter *counter,
    ScalarHashable *hashable) const {
  // This bool flag is for avoiding some unnecessary hash() computation.
  bool children_hashable = true;

  const auto homogeneous_expression_types_it =
      homogeneous_expression_types_.find(expression->getExpressionType());
  if (homogeneous_expression_types_it != homogeneous_expression_types_.end()) {
    for (const auto &child : expression->children()) {
      children_hashable &= visitAndCount(child, counter, hashable);
    }
  }

  E::ScalarPtr scalar;
  if (children_hashable &&
      E::SomeScalar::MatchesWithConditionalCast(expression, &scalar)) {
    // A scalar expression may or may not support the hash() computation.
    // In the later case, a HashNotSupported exception will be thrown and we
    // simply ignore this expression (and all its ancestor expressions).
    try {
      ++(*counter)[scalar];
    } catch (const HashNotSupported &e) {
      return false;
    }
    hashable->emplace(scalar);
    return true;
  }
  return false;
}

E::ExpressionPtr ExtractCommonSubexpression::visitAndTransform(
    const E::ExpressionPtr &expression,
    const std::size_t max_reference_count,
    const ScalarCounter &counter,
    const ScalarHashable &hashable,
    ScalarMap *substitution_map) {
  // TODO(jianqiao): Currently it is hardly beneficial to make AttributeReference
  // a common subexpression due to the inefficiency of ScalarAttribute's
  // size-not-known-at-compile-time std::memcpy() calls, compared to copy-elision
  // at selection level. Even in the case of compressed column store, it requires
  // an attribute to occur at least 4 times for the common subexpression version
  // to outperform the direct decoding version. We may look into ScalarAttribute
  // and modify the heuristic here later.
  if (expression->getExpressionType() == E::ExpressionType::kScalarLiteral ||
      expression->getExpressionType() == E::ExpressionType::kAttributeReference) {
    return expression;
  }

  E::ScalarPtr scalar;
  const bool is_hashable =
      E::SomeScalar::MatchesWithConditionalCast(expression, &scalar)
          && hashable.find(scalar) != hashable.end();

  std::size_t new_max_reference_count;
  if (is_hashable) {
    // CommonSubexpression node already generated somewhere. Just refer to that
    // and return.
    const auto substitution_map_it = substitution_map->find(scalar);
    if (substitution_map_it != substitution_map->end()) {
      return substitution_map_it->second;
    }

    // Otherwise, update the dominance count.
    const auto counter_it = counter.find(scalar);
    DCHECK(counter_it != counter.end());
    DCHECK_LE(max_reference_count, counter_it->second);
    new_max_reference_count = counter_it->second;
  } else {
    new_max_reference_count = max_reference_count;
  }

  // Process children.
  std::vector<E::ExpressionPtr> new_children;
  const auto homogeneous_expression_types_it =
      homogeneous_expression_types_.find(expression->getExpressionType());
  if (homogeneous_expression_types_it == homogeneous_expression_types_.end()) {
    // If child subexpressions cannot be hoisted through the current expression,
    // treat child expressions as isolated sub-optimizations.
    for (const auto &child : expression->children()) {
      new_children.emplace_back(transformExpression(child));
    }
  } else {
    for (const auto &child : expression->children()) {
      new_children.emplace_back(
          visitAndTransform(child,
                            new_max_reference_count,
                            counter,
                            hashable,
                            substitution_map));
    }
  }

  E::ExpressionPtr output;
  if (new_children == expression->children()) {
    output = expression;
  } else {
    output = std::static_pointer_cast<const E::Scalar>(
        expression->copyWithNewChildren(new_children));
  }

  // Wrap it with a new CommonSubexpression node if the current expression is a
  // dominant subexpression.
  if (is_hashable && new_max_reference_count > max_reference_count) {
    DCHECK(E::SomeScalar::Matches(output));
    const E::CommonSubexpressionPtr common_subexpression =
        E::CommonSubexpression::Create(
            optimizer_context_->nextExprId(),
            std::static_pointer_cast<const E::Scalar>(output));
    substitution_map->emplace(scalar, common_subexpression);
    output = common_subexpression;
  }

  return output;
}

}  // namespace optimizer
}  // namespace quickstep
