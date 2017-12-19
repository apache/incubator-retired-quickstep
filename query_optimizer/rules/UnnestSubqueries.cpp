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

#include "query_optimizer/rules/UnnestSubqueries.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/Exists.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/InTableQuery.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/LogicalNot.hpp"
#include "query_optimizer/expressions/LogicalOr.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/SubqueryExpression.hpp"
#include "query_optimizer/logical/Aggregate.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "query_optimizer/logical/MultiwayCartesianJoin.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/TopLevelPlan.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

struct CorrelatedQueryInfo {
  enum class JoinType {
    kInnerJoin = 0,
    kLeftSemiJoin,
    kLeftAntiJoin,
    kCartesianJoin
  };

  CorrelatedQueryInfo(const JoinType join_type_in,
                      const L::LogicalPtr &correlated_query_in,
                      std::vector<E::AttributeReferencePtr> &&probe_join_attributes_in,
                      std::vector<E::AttributeReferencePtr> &&build_join_attributes_in,
                      std::vector<E::PredicatePtr> &&non_hash_join_predicates_in)
      : join_type(join_type_in),
        correlated_query(correlated_query_in),
        probe_join_attributes(std::move(probe_join_attributes_in)),
        build_join_attributes(std::move(build_join_attributes_in)),
        non_hash_join_predicates(std::move(non_hash_join_predicates_in)) {}

  JoinType join_type;
  L::LogicalPtr correlated_query;
  std::vector<E::AttributeReferencePtr> probe_join_attributes;
  std::vector<E::AttributeReferencePtr> build_join_attributes;
  std::vector<E::PredicatePtr> non_hash_join_predicates;
};

L::LogicalPtr UnnestSubqueries::apply(const L::LogicalPtr &input) {
  DCHECK(L::SomeTopLevelPlan::Matches(input))
      << "The input logical node must be of TopLevelPlan";

  const L::TopLevelPlan &top_level_plan = static_cast<const L::TopLevelPlan&>(*input);

  bool has_changes = false;

  std::vector<E::AttributeReferencePtr> empty_probe_join_predicates;
  std::vector<E::AttributeReferencePtr> empty_build_join_predicates;
  std::vector<E::PredicatePtr> empty_correlated_non_hash_join_predicates;
  std::unordered_map<E::ExprId, L::LogicalPtr> uncorrelated_subqueries;
  expressions::UnorderedNamedExpressionSet empty_attributes;
  UnnestSubqueriesForNonRootLogical unnest_rule(false /* scalar query */,
                                                empty_attributes,
                                                context_,
                                                &uncorrelated_subqueries,
                                                &empty_probe_join_predicates,
                                                &empty_build_join_predicates,
                                                &empty_correlated_non_hash_join_predicates);
  DCHECK(empty_probe_join_predicates.empty());
  DCHECK(empty_build_join_predicates.empty());
  DCHECK(empty_correlated_non_hash_join_predicates.empty());

  // Unnest subqueries in each subplan and the main plan.
  std::vector<L::LogicalPtr> new_shared_subplans;
  for (const L::LogicalPtr &shared_subplan : top_level_plan.shared_subplans()) {
    const L::LogicalPtr new_shared_subplan = unnest_rule.apply(shared_subplan);
    if (new_shared_subplan != shared_subplan && !has_changes) {
      has_changes = true;
    }
    new_shared_subplans.emplace_back(new_shared_subplan);
  }

  const L::LogicalPtr new_main_plan = unnest_rule.apply(top_level_plan.plan());
  if (new_main_plan != top_level_plan.plan() && !has_changes) {
    has_changes = true;
  }

  std::unordered_map<E::ExprId, int> uncorrelated_map;
  std::vector<L::LogicalPtr>::size_type subplan_id = new_shared_subplans.size();
  for (const std::pair<const E::ExprId, L::LogicalPtr> &uncorrelated_query_info : uncorrelated_subqueries) {
    uncorrelated_map.emplace(uncorrelated_query_info.first,
                             subplan_id);
    new_shared_subplans.emplace_back(uncorrelated_query_info.second);
    ++subplan_id;
  }

  if (!uncorrelated_subqueries.empty() || has_changes) {
    const L::LogicalPtr new_top_level_plan =
        L::TopLevelPlan::Create(new_main_plan,
                                new_shared_subplans,
                                uncorrelated_map);
    LOG_APPLYING_RULE(input, new_top_level_plan);
    return new_top_level_plan;
  }

  LOG_IGNORING_RULE(input);
  return input;
}

L::LogicalPtr UnnestSubqueriesForNonRootLogical::applyInternal(
    const L::LogicalPtr &input,
    E::UnorderedAttributeSet *inner_attributes,
    std::vector<E::AttributeReferencePtr> *probe_join_attributes,
    std::vector<E::AttributeReferencePtr> *build_join_attributes,
    std::vector<E::PredicatePtr> *non_hash_join_predicates) {
  DCHECK(inner_attributes->empty());
  DCHECK(probe_join_attributes->empty());
  DCHECK(build_join_attributes->empty());
  DCHECK(non_hash_join_predicates->empty());

  // First, handle subquery expressions in the children.
  bool has_change = false;
  std::vector<L::LogicalPtr> new_children;
  for (const L::LogicalPtr &child : input->children()) {
    E::UnorderedAttributeSet inner_attributes_in_child;
    std::vector<E::AttributeReferencePtr> probe_join_predicates_in_child;
    std::vector<E::AttributeReferencePtr> build_join_predicates_in_child;
    std::vector<E::PredicatePtr> non_hash_join_predicates_in_child;
    const L::LogicalPtr new_child =
        applyInternal(child,
                      &inner_attributes_in_child,
                      &probe_join_predicates_in_child,
                      &build_join_predicates_in_child,
                      &non_hash_join_predicates_in_child);
    inner_attributes->insert(inner_attributes_in_child.begin(),
                             inner_attributes_in_child.end());
    probe_join_attributes->insert(probe_join_attributes->end(),
                                  probe_join_predicates_in_child.begin(),
                                  probe_join_predicates_in_child.end());
    build_join_attributes->insert(build_join_attributes->end(),
                                  build_join_predicates_in_child.begin(),
                                  build_join_predicates_in_child.end());
    non_hash_join_predicates->insert(non_hash_join_predicates->end(),
                                     non_hash_join_predicates_in_child.begin(),
                                     non_hash_join_predicates_in_child.end());
    if (new_child != child && !has_change) {
      has_change = true;
    }
    new_children.emplace_back(new_child);
  }

  L::LogicalPtr input_with_new_children = input;
  if (has_change) {
    input_with_new_children = input->copyWithNewChildren(new_children);
  }

  // Next, process subquery expressions in the current node.
  const L::LogicalPtr output = applyToNode(input_with_new_children,
                                           inner_attributes,
                                           probe_join_attributes,
                                           build_join_attributes,
                                           non_hash_join_predicates);
  return output;
}

L::LogicalPtr UnnestSubqueriesForNonRootLogical::applyToNode(
    const L::LogicalPtr &input,
    E::UnorderedAttributeSet *inner_attributes,
    std::vector<E::AttributeReferencePtr> *probe_join_attributes,
    std::vector<E::AttributeReferencePtr> *build_join_attributes,
    std::vector<E::PredicatePtr> *non_hash_join_predicates) {
  // First eliminate subqueries expressions in this node (that is, this node is the outer query).
  L::LogicalPtr input_with_no_subqueries = eliminateNestedScalarQueries(input);
  DCHECK(input_with_no_subqueries != nullptr);

  // Next determine whether this node has any outer reference (that is, this node is the inner query).
  const std::vector<E::ExpressionPtr> &input_expressions = input_with_no_subqueries->input_expressions();
  std::vector<E::ExpressionPtr> new_input_expressions;
  bool has_change = false;
  for (const E::ExpressionPtr &input_expression : input_expressions) {
    const E::ExpressionPtr new_input_expression =
        eliminateOuterAttributeReference(input_expression,
                                         inner_attributes,
                                         probe_join_attributes,
                                         build_join_attributes,
                                         non_hash_join_predicates);
    if (new_input_expression != nullptr) {
      new_input_expressions.emplace_back(new_input_expression);
    }
    if (new_input_expression != input_expression && !has_change) {
      has_change = true;
    }
  }

  switch (input_with_no_subqueries->getLogicalType()) {
    case L::LogicalType::kAggregate: {
      if (!non_hash_join_predicates->empty()) {
        THROW_SQL_ERROR()
            << "Non-equality join predicate with an outer query must be above any aggregate";
      }

      const L::Aggregate &aggregate = static_cast<const L::Aggregate&>(*input_with_no_subqueries);
      DCHECK(!has_change);
      if (!inner_attributes->empty()) {
        std::vector<E::NamedExpressionPtr> group_expressions = aggregate.grouping_expressions();
        group_expressions.insert(group_expressions.end(),
                                 inner_attributes->begin(),
                                 inner_attributes->end());
        return L::Aggregate::Create(aggregate.input(),
                                    group_expressions,
                                    aggregate.aggregate_expressions());
      }
      return input_with_no_subqueries;
    }
    case L::LogicalType::kFilter: {
      if (new_input_expressions.empty()) {
        const L::Filter &filter = static_cast<const L::Filter&>(*input_with_no_subqueries);
        return filter.input();
      }
      if (has_change) {
        return input_with_no_subqueries->copyWithNewInputExpressions(new_input_expressions);
      }
      return input_with_no_subqueries;
    }
    case L::LogicalType::kProject: {
      if (has_change) {
        input_with_no_subqueries =
            input_with_no_subqueries->copyWithNewInputExpressions(new_input_expressions);
      }
      if (!inner_attributes->empty()) {
         const L::Project &project = static_cast<const L::Project&>(*input_with_no_subqueries);
         // Insert those inner attributes into the front of the project expression
         // list rather than the back to help reduce unnecessary selection for
         // reordering output attributes, since those attributes are usually also
         // grouping attributes that proceed others in projection.
         std::vector<E::NamedExpressionPtr> new_project_expressions;
         const E::UnorderedNamedExpressionSet project_expression_set(project.project_expressions().begin(),
                                                                     project.project_expressions().end());
         for (const E::AttributeReferencePtr &inner_attribute : *inner_attributes) {
           if (project_expression_set.find(inner_attribute) == project_expression_set.end()) {
             new_project_expressions.emplace_back(inner_attribute);
           }
         }
         new_project_expressions.insert(new_project_expressions.end(),
                                        project_expression_set.begin(),
                                        project_expression_set.end());
         if (new_project_expressions.size() != project.project_expressions().size()) {
           input_with_no_subqueries = L::Project::Create(project.input(),
                                                         new_project_expressions);
         }
      }
      return input_with_no_subqueries;
    }
    default:
      if (has_change) {
        return input_with_no_subqueries->copyWithNewInputExpressions(new_input_expressions);
      }
      return input_with_no_subqueries;
  }
}

E::ExpressionPtr UnnestSubqueriesForNonRootLogical::eliminateOuterAttributeReference(
    const E::ExpressionPtr &expression,
    E::UnorderedAttributeSet *inner_attributes,
    std::vector<E::AttributeReferencePtr> *probe_join_attributes,
    std::vector<E::AttributeReferencePtr> *build_join_attributes,
    std::vector<E::PredicatePtr> *non_hash_join_predicates) {
  DCHECK(expression->getExpressionType() != E::ExpressionType::kInTableQuery);
  DCHECK(expression->getExpressionType() != E::ExpressionType::kExists);
  DCHECK(expression->getExpressionType() != E::ExpressionType::kSubqueryExpression);

  switch (expression->getExpressionType()) {
    case E::ExpressionType::kPredicateLiteral:  // Fall through
    case E::ExpressionType::kScalarLiteral:
      return expression;
    case E::ExpressionType::kLogicalAnd: {
      const E::LogicalAnd &logical_and = static_cast<const E::LogicalAnd&>(*expression);
      bool has_change = false;
      std::vector<E::PredicatePtr> new_children;
      for (const E::PredicatePtr &child : logical_and.operands()) {
        const E::ExpressionPtr new_child =
            eliminateOuterAttributeReference(child,
                                             inner_attributes,
                                             probe_join_attributes,
                                             build_join_attributes,
                                             non_hash_join_predicates);
        if (new_child != nullptr) {
          new_children.emplace_back(
              std::static_pointer_cast<const E::Predicate>(new_child));
        }
        if (new_child != child && !has_change) {
          has_change = true;
        }
      }
      if (has_change) {
        if (new_children.empty()) {
          return E::ExpressionPtr();
        }
        if (new_children.size() == 1u) {
          return new_children[0];
        }
        return E::LogicalAnd::Create(new_children);
      }
      return expression;
    }
    case E::ExpressionType::kComparisonExpression: {
      const E::ComparisonExpressionPtr comparison_expression =
          std::static_pointer_cast<const E::ComparisonExpression>(expression);
      E::AttributeReferencePtr outer_attribute;
      E::AttributeReferencePtr inner_attribute;
      // Hash-join predicate.
      if (E::SomeAttributeReference::MatchesWithConditionalCast(
              comparison_expression->left(), &outer_attribute) &&
          E::SomeAttributeReference::MatchesWithConditionalCast(
              comparison_expression->right(), &inner_attribute) &&
          comparison_expression->comparison().getComparisonID() == ComparisonID::kEqual) {
        if (!isCorrelatedOuterAttribute(outer_attribute)) {
          if (!isCorrelatedOuterAttribute(inner_attribute)) {
            return expression;
          }
          std::swap(outer_attribute, inner_attribute);
        } else if (isCorrelatedOuterAttribute(inner_attribute)) {
          THROW_SQL_ERROR() << "Comparison of two outer attribute references is not allowed";
        }
        if (visiable_attrs_from_outer_query_.find(outer_attribute) == visiable_attrs_from_outer_query_.end()) {
          THROW_SQL_ERROR()
              << "Nested queries can only reference attributes in the outer query one level above";
        }
        outer_attribute =
            E::AttributeReference::Create(outer_attribute->id(),
                                          outer_attribute->attribute_name(),
                                          outer_attribute->attribute_alias(),
                                          outer_attribute->relation_name(),
                                          outer_attribute->getValueType(),
                                          E::AttributeReferenceScope::kLocal);

        inner_attributes->emplace(inner_attribute);
        probe_join_attributes->emplace_back(outer_attribute);
        build_join_attributes->emplace_back(inner_attribute);
        return E::ExpressionPtr();
      } else {
        DeOuterAttributeReference de_outer_reference_rule(!scalar_query_,
                                                          *uncorrelated_subqueries_,
                                                          visiable_attrs_from_outer_query_);
        const E::ExpressionPtr new_expression =
            de_outer_reference_rule.apply(expression);
        if (new_expression != expression) {
          // All the inner attributes should be insert into the inner_attributes,
          // so that they will be visible from the outer query.
          // Note that we use expression instead of new_expression, since
          // the outer attribute reference in new_expression is de-outer referenced.
          const std::vector<E::AttributeReferencePtr> &inner_attrs_vec =
              E::GetAttributeReferencesWithinScope(expression->getReferencedAttributes(),
                                                   E::AttributeReferenceScope::kLocal);
          for (const E::AttributeReferencePtr &attr : inner_attrs_vec) {
            inner_attributes->emplace(attr);
          }

          non_hash_join_predicates->emplace_back(
              std::static_pointer_cast<const E::Predicate>(new_expression));
          return E::ExpressionPtr();
        }
      }
      return expression;
    }
    default: {
      validateNonOuterAttributeReference(expression);
      return expression;
    }
  }
}

void UnnestSubqueriesForNonRootLogical::validateNonOuterAttributeReference(
    const E::ExpressionPtr &expression) {
  const std::vector<E::AttributeReferencePtr> referenced_attributes =
      expression->getReferencedAttributes();
  for (const E::AttributeReferencePtr &referenced_attribute : referenced_attributes) {
    if (isCorrelatedOuterAttribute(referenced_attribute)) {
      THROW_SQL_ERROR()
          << "Outer attribute reference can only appear in a single-attribute equality predicate";
    }
  }
}

bool UnnestSubqueriesForNonRootLogical::isCorrelatedOuterAttribute(
    const E::AttributeReferencePtr &attribute) const {
  return (attribute->scope() == E::AttributeReferenceScope::kOuter &&
          uncorrelated_subqueries_->find(attribute->id()) == uncorrelated_subqueries_->end());
}

L::LogicalPtr UnnestSubqueriesForNonRootLogical::eliminateNestedScalarQueries(const L::LogicalPtr &node) {
  const std::vector<E::ExpressionPtr> &input_expressions = node->input_expressions();
  const std::vector<E::AttributeReferencePtr> input_attributes =
      node->getInputAttributes();
  E::UnorderedNamedExpressionSet input_attribute_set(input_attributes.begin(),
                                                     input_attributes.end());

  // Transform subquery experssions into non-subquery expressions.
  std::vector<CorrelatedQueryInfo> correlated_query_info_vec;
  std::vector<E::ExpressionPtr> new_input_expressions;
  bool has_changed_expression = false;
  for (const E::ExpressionPtr &input_expression : input_expressions) {
    std::vector<E::PredicatePtr> correlated_predicates;
    UnnestSubqueriesForExpession unnest_rule_for_expr(
        input_attribute_set,
        context_,
        uncorrelated_subqueries_,
        &correlated_query_info_vec);
    const E::ExpressionPtr new_input_expression =
        unnest_rule_for_expr.apply(input_expression);
    if (new_input_expression != input_expression && !has_changed_expression) {
      has_changed_expression = true;
    }
    new_input_expressions.emplace_back(new_input_expression);
  }

  if (has_changed_expression) {
    // If there are correlated subquery expressions, add logical joins.
    if (!correlated_query_info_vec.empty()) {
      L::LogicalPtr new_child;

      if (new_input_expressions[0] == nullptr &&
          node->getLogicalType() == L::LogicalType::kNestedLoopsJoin) {
        new_child = node;
      } else {
        DCHECK_EQ(1u, node->children().size());
        new_child = node->children()[0];
      }

      // Join uncorrelated subqueries early.
      L::LogicalPtr uncorrelated_query_child;
      for (const CorrelatedQueryInfo &correlated_query_info : correlated_query_info_vec) {
        if (correlated_query_info.join_type == CorrelatedQueryInfo::JoinType::kCartesianJoin) {
          // The only case for this nested loop join is that it is an uncorrelated
          // subquery which returns a scalar (single column and single row) result.
          DCHECK(correlated_query_info.probe_join_attributes.empty());
          DCHECK_EQ(0u, correlated_query_info.non_hash_join_predicates.size());
          if (uncorrelated_query_child == nullptr) {
            uncorrelated_query_child = correlated_query_info.correlated_query;
          } else {
            uncorrelated_query_child = L::MultiwayCartesianJoin::Create(
                { uncorrelated_query_child, correlated_query_info.correlated_query });
          }
        }
      }
      if (uncorrelated_query_child != nullptr) {
        new_child = L::MultiwayCartesianJoin::Create({ new_child, uncorrelated_query_child });
      }

      for (const CorrelatedQueryInfo &correlated_query_info : correlated_query_info_vec) {
        if (correlated_query_info.join_type == CorrelatedQueryInfo::JoinType::kInnerJoin) {
          DCHECK(!correlated_query_info.probe_join_attributes.empty());
          DCHECK(correlated_query_info.non_hash_join_predicates.empty())
              << correlated_query_info.non_hash_join_predicates[0]->toString();
          new_child = L::HashJoin::Create(new_child,
                                          correlated_query_info.correlated_query,
                                          correlated_query_info.probe_join_attributes,
                                          correlated_query_info.build_join_attributes,
                                          nullptr, /* residual_predicate */
                                          L::HashJoin::JoinType::kInnerJoin);
        } else if (correlated_query_info.join_type == CorrelatedQueryInfo::JoinType::kLeftSemiJoin ||
                   correlated_query_info.join_type == CorrelatedQueryInfo::JoinType::kLeftAntiJoin) {
          DCHECK(!correlated_query_info.probe_join_attributes.empty());
          E::PredicatePtr filter_predicate;
          if (correlated_query_info.non_hash_join_predicates.size() > 1u) {
            filter_predicate = E::LogicalAnd::Create(correlated_query_info.non_hash_join_predicates);
          } else if (!correlated_query_info.non_hash_join_predicates.empty()) {
            filter_predicate = correlated_query_info.non_hash_join_predicates[0];
          }

          L::HashJoin::JoinType join_type =
              (correlated_query_info.join_type == CorrelatedQueryInfo::JoinType::kLeftSemiJoin)
                  ? L::HashJoin::JoinType::kLeftSemiJoin
                  : L::HashJoin::JoinType::kLeftAntiJoin;
          new_child = L::HashJoin::Create(new_child,
                                          correlated_query_info.correlated_query,
                                          correlated_query_info.probe_join_attributes,
                                          correlated_query_info.build_join_attributes,
                                          filter_predicate,
                                          join_type);
        }
      }

      // Special case for filter and nested loops join which may contain EXISTS and IN.
      if (new_input_expressions[0] == nullptr) {
        DCHECK_EQ(1u, new_input_expressions.size());
        DCHECK(node->getLogicalType() == L::LogicalType::kFilter ||
               node->getLogicalType() == L::LogicalType::kNestedLoopsJoin) << node->toString();
        LOG_APPLYING_RULE(node, new_child);
        return new_child;
      }

      L::LogicalPtr new_logical = node->copyWithNewInputExpressions(new_input_expressions);
      new_logical = new_logical->copyWithNewChildren({new_child});
      LOG_APPLYING_RULE(node, new_logical);
      return new_logical;
    }

    L::LogicalPtr new_logical = node->copyWithNewInputExpressions(new_input_expressions);
    LOG_APPLYING_RULE(node, new_logical);
    return new_logical;
  }

  LOG_IGNORING_RULE(node);
  return node;
}

E::ExpressionPtr UnnestSubqueriesForExpession::applyInternal(
    const bool allow_exists_or_in,
    const E::ExpressionPtr &node) {
  switch (node->getExpressionType()) {
    case E::ExpressionType::kSubqueryExpression: {
      const E::SubqueryExpression &subquery_expression =
          static_cast<const E::SubqueryExpression&>(*node);

      std::vector<E::AttributeReferencePtr> probe_join_attributes;
      std::vector<E::AttributeReferencePtr> build_join_attributes;
      std::vector<E::PredicatePtr> non_hash_join_predicates;
      UnnestSubqueriesForNonRootLogical unnest_logical_rule(true,  // scalar_query
                                                            visible_attributes_from_outer_query_,
                                                            context_,
                                                            uncorrelated_subqueries_,
                                                            &probe_join_attributes,
                                                            &build_join_attributes,
                                                            &non_hash_join_predicates);
      const L::LogicalPtr subquery = subquery_expression.subquery();
      const L::LogicalPtr new_subquery = unnest_logical_rule.apply(subquery);
      const E::AttributeReferencePtr output_attribute = subquery->getOutputAttributes()[0];
      DCHECK(!new_subquery->getOutputAttributes().empty());
      if (probe_join_attributes.empty()) {
        DCHECK(non_hash_join_predicates.empty());
        DCHECK_EQ(1u, new_subquery->getOutputAttributes().size()) << node->toString();
        correlated_query_info_vec_->emplace_back(CorrelatedQueryInfo::JoinType::kCartesianJoin,
                                                 new_subquery,
                                                 std::move(probe_join_attributes),
                                                 std::move(build_join_attributes),
                                                 std::move(non_hash_join_predicates));
      } else {
        correlated_query_info_vec_->emplace_back(CorrelatedQueryInfo::JoinType::kInnerJoin,
                                                 new_subquery,
                                                 std::move(probe_join_attributes),
                                                 std::move(build_join_attributes),
                                                 std::move(non_hash_join_predicates));
      }

      return output_attribute;
    }
    case E::ExpressionType::kExists: {
      if (!allow_exists_or_in) {
        THROW_SQL_ERROR() << "EXISTS can only appear in (un-nested) NOT, AND or by itself";
      }
      transformExists(static_cast<const E::Exists&>(*node));
      return E::ExpressionPtr();
    }
    case E::ExpressionType::kInTableQuery: {
      if (!allow_exists_or_in) {
        THROW_SQL_ERROR() << "IN(table query) can only appear in (un-nested) NOT, AND or by itself";
      }
      transformInTableQuery(static_cast<const E::InTableQuery&>(*node));
      return E::ExpressionPtr();
    }
    case E::ExpressionType::kLogicalNot: {
      const E::LogicalNot &logical_not = static_cast<const E::LogicalNot&>(*node);
      const E::PredicatePtr &operand = logical_not.operand();
      if (operand->getExpressionType() == E::ExpressionType::kExists) {
        if (!allow_exists_or_in) {
          THROW_SQL_ERROR() << "EXISTS can only appear in (un-nested) NOT, AND or by itself";
        }
        transformExists(static_cast<const E::Exists&>(*operand));
        correlated_query_info_vec_->back().join_type = CorrelatedQueryInfo::JoinType::kLeftAntiJoin;
        return E::PredicatePtr();
      } else if (operand->getExpressionType() == E::ExpressionType::kInTableQuery) {
        if (!allow_exists_or_in) {
          THROW_SQL_ERROR() << "IN(table query) can only appear in (un-nested) NOT, AND or by itself";
        }
        transformInTableQuery(static_cast<const E::InTableQuery&>(*operand));
        correlated_query_info_vec_->back().join_type = CorrelatedQueryInfo::JoinType::kLeftAntiJoin;
        return E::PredicatePtr();
      }
      const E::ExpressionPtr new_operand =
          applyInternal(false /* allow_exists_or_in */,
                        operand);
      if (new_operand != operand) {
        return E::LogicalNot::Create(std::static_pointer_cast<const E::Predicate>(new_operand));
      } else {
        return node;
      }
    }
    case E::ExpressionType::kLogicalAnd: {
      const E::LogicalAnd &logical_and = static_cast<const E::LogicalAnd&>(*node);

      std::vector<E::PredicatePtr> new_operands;
      bool has_change = false;
      for (const E::PredicatePtr &operand : logical_and.operands()) {
        const E::ExpressionPtr new_operand =
            applyInternal(allow_exists_or_in,
                          operand);
        if (new_operand != operand && !has_change) {
          has_change = true;
        }
        if (new_operand != nullptr) {
          new_operands.emplace_back(std::static_pointer_cast<const E::Predicate>(new_operand));
        }
      }

      if (new_operands.empty()) {
        return E::PredicatePtr();
      } else if (new_operands.size() == 1u) {
        return new_operands[0];
      }

      if (has_change) {
        return E::LogicalAnd::Create(new_operands);
      }
      return node;
    }
    case E::ExpressionType::kLogicalOr: {
      const E::LogicalOr &logical_or = static_cast<const E::LogicalOr&>(*node);

      std::vector<E::PredicatePtr> new_operands;
      bool has_change = false;
      for (const E::PredicatePtr &operand : logical_or.operands()) {
        const E::ExpressionPtr new_operand =
            applyInternal(false,
                          operand);
        if (new_operand != operand && !has_change) {
          has_change = true;
        }
        DCHECK(new_operand != nullptr);
        new_operands.emplace_back(std::static_pointer_cast<const E::Predicate>(new_operand));
      }

      if (has_change) {
        return E::LogicalOr::Create(new_operands);
      }
      return node;
    }
    default:
      std::vector<E::ExpressionPtr> new_children;
      bool has_change = false;
      for (const E::ExpressionPtr &child : node->children()) {
        const E::ExpressionPtr new_child =
            applyInternal(allow_exists_or_in, child);
        if (new_child != child && !has_change) {
          has_change = true;
        }
        DCHECK(child != nullptr);
        new_children.emplace_back(new_child);
      }

      if (has_change) {
        return node->copyWithNewChildren(new_children);
      }
      return node;
  }
}

void UnnestSubqueriesForExpession::transformExists(
    const E::Exists &exists_predicate) {
  std::vector<E::AttributeReferencePtr> probe_join_attributes;
  std::vector<E::AttributeReferencePtr> build_join_attributes;
  std::vector<E::PredicatePtr> non_hash_join_predicates;
  UnnestSubqueriesForNonRootLogical unnest_logical_rule(false,  // scalar_query
                                                        visible_attributes_from_outer_query_,
                                                        context_,
                                                        uncorrelated_subqueries_,
                                                        &probe_join_attributes,
                                                        &build_join_attributes,
                                                        &non_hash_join_predicates);
  DCHECK_EQ(probe_join_attributes.size(), build_join_attributes.size());
  const L::LogicalPtr new_subquery =
      unnest_logical_rule.apply(exists_predicate.exists_subquery()->subquery());

  if (probe_join_attributes.empty()) {
    if (!non_hash_join_predicates.empty()) {
      THROW_SQL_ERROR() << "Correlated queries must have an equality join predicate with the outer query";
    }
    THROW_SQL_ERROR() << "EXISTS subquery cannot be un-correlated with the outer query";
  }

  correlated_query_info_vec_->emplace_back(CorrelatedQueryInfo::JoinType::kLeftSemiJoin,
                                           new_subquery,
                                           std::move(probe_join_attributes),
                                           std::move(build_join_attributes),
                                           std::move(non_hash_join_predicates));
}

void UnnestSubqueriesForExpession::transformInTableQuery(
    const E::InTableQuery &in_table_query) {
  std::vector<E::AttributeReferencePtr> probe_join_attributes;
  std::vector<E::AttributeReferencePtr> build_join_attributes;
  std::vector<E::PredicatePtr> non_hash_join_predicates;
  UnnestSubqueriesForNonRootLogical unnest_logical_rule(false,  // scalar_query
                                                        visible_attributes_from_outer_query_,
                                                        context_,
                                                        uncorrelated_subqueries_,
                                                        &probe_join_attributes,
                                                        &build_join_attributes,
                                                        &non_hash_join_predicates);
  const L::LogicalPtr subquery = in_table_query.table_query()->subquery();
  const L::LogicalPtr new_subquery = unnest_logical_rule.apply(subquery);

  DCHECK(!new_subquery->getOutputAttributes().empty());
  const E::AttributeReferencePtr join_attr_in_subquery = subquery->getOutputAttributes()[0];

  E::AttributeReferencePtr test_attribute;
  if (E::SomeAttributeReference::MatchesWithConditionalCast(in_table_query.test_expression(),
                                                            &test_attribute)) {
    probe_join_attributes.emplace_back(test_attribute);
    build_join_attributes.emplace_back(join_attr_in_subquery);
  } else {
    if (!probe_join_attributes.empty()) {
      non_hash_join_predicates.emplace_back(
          E::ComparisonExpression::Create(ComparisonFactory::GetComparison(ComparisonID::kEqual),
                                          in_table_query.test_expression(),
                                          join_attr_in_subquery));
    } else {
      // TODO(qzeng): We can actually add a project to precompute the test expression.
      THROW_SQL_ERROR() << "Cannot find an equality join predicate for IN";
    }
  }

  correlated_query_info_vec_->emplace_back(CorrelatedQueryInfo::JoinType::kLeftSemiJoin,
                                           new_subquery,
                                           std::move(probe_join_attributes),
                                           std::move(build_join_attributes),
                                           std::move(non_hash_join_predicates));
}

E::ExpressionPtr DeOuterAttributeReference::applyToNode(const E::ExpressionPtr &input) {
  E::AttributeReferencePtr attr;
  if (E::SomeAttributeReference::MatchesWithConditionalCast(input, &attr) &&
      attr->scope() == E::AttributeReferenceScope::kOuter &&
      uncorrelated_subqueries_.find(attr->id()) == uncorrelated_subqueries_.end()) {
    if (!allow_outer_reference_) {
      THROW_SQL_ERROR() << "Non-equality join predicate is not allowed in scalar subqueries";
    }
    if (visiable_attrs_from_outer_query_.find(attr) == visiable_attrs_from_outer_query_.end()) {
      THROW_SQL_ERROR()
          << "Nested queries can only reference attributes in the outer query one level above";
    }
    return E::AttributeReference::Create(attr->id(),
                                         attr->attribute_name(),
                                         attr->attribute_alias(),
                                         attr->relation_name(),
                                         attr->getValueType(),
                                         E::AttributeReferenceScope::kLocal);
  }
  return input;
}

}  // namespace optimizer
}  // namespace quickstep
