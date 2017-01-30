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

#include "query_optimizer/rules/PushDownLowCostDisjunctivePredicate.hpp"

#include <cstddef>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/LogicalOr.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"

#include "gflags/gflags.h"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

DEFINE_uint64(push_down_disjunctive_predicate_cardinality_threshold, 100u,
              "The cardinality threshold for a stored relation for the "
              "PushDownLowCostDisjunctivePredicate optimization rule to push "
              "down a disjunctive predicate to pre-filter that relation.");

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr PushDownLowCostDisjunctivePredicate::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  const P::TopLevelPlanPtr top_level_plan =
     std::static_pointer_cast<const P::TopLevelPlan>(input);
  cost_model_.reset(
      new cost::StarSchemaSimpleCostModel(
          top_level_plan->shared_subplans()));

  collectApplicablePredicates(input);

  if (!applicable_predicates_.empty()) {
    // Apply the selected predicates to stored relations.
    return attachPredicates(input);
  } else {
    return input;
  }
}

void PushDownLowCostDisjunctivePredicate::collectApplicablePredicates(
    const physical::PhysicalPtr &input) {
  P::TableReferencePtr table_reference;
  if (P::SomeTableReference::MatchesWithConditionalCast(input, &table_reference)) {
    // Consider only stored relations with small cardinality as targets.
    if (cost_model_->estimateCardinality(input) <=
            FLAGS_push_down_disjunctive_predicate_cardinality_threshold) {
      applicable_nodes_.emplace_back(input, &table_reference->attribute_list());
    }
    return;
  }

  for (const auto &child : input->children()) {
    collectApplicablePredicates(child);
  }

  E::PredicatePtr filter_predicate = nullptr;
  switch (input->getPhysicalType()) {
    case P::PhysicalType::kAggregate: {
      filter_predicate =
          std::static_pointer_cast<const P::Aggregate>(input)->filter_predicate();
      break;
    }
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr hash_join =
          std::static_pointer_cast<const P::HashJoin>(input);
      if (hash_join->join_type() == P::HashJoin::JoinType::kInnerJoin) {
        filter_predicate = hash_join->residual_predicate();
      }
      break;
    }
    case P::PhysicalType::kNestedLoopsJoin: {
      filter_predicate =
          std::static_pointer_cast<const P::NestedLoopsJoin>(input)->join_predicate();
      break;
    }
    case P::PhysicalType::kSelection: {
      filter_predicate =
          std::static_pointer_cast<const P::Selection>(input)->filter_predicate();
      break;
    }
    default:
      break;
  }

  E::LogicalOrPtr disjunctive_predicate;
  if (filter_predicate == nullptr ||
      !E::SomeLogicalOr::MatchesWithConditionalCast(filter_predicate, &disjunctive_predicate)) {
    return;
  }

  // Consider only disjunctive normal form, i.e. disjunction of conjunctions.
  // Divide the disjunctive components into groups.
  std::vector<std::vector<E::PredicatePtr>> candidate_predicates;
  std::vector<std::vector<std::vector<E::AttributeReferencePtr>>> candidate_attributes;
  for (const auto &conjunctive_predicate : disjunctive_predicate->operands()) {
    candidate_predicates.emplace_back();
    candidate_attributes.emplace_back();
    E::LogicalAndPtr logical_and;
    if (E::SomeLogicalAnd::MatchesWithConditionalCast(conjunctive_predicate, &logical_and)) {
      for (const auto &predicate : logical_and->operands()) {
        candidate_predicates.back().emplace_back(predicate);
        candidate_attributes.back().emplace_back(
            predicate->getReferencedAttributes());
      }
    } else {
      candidate_predicates.back().emplace_back(conjunctive_predicate);
      candidate_attributes.back().emplace_back(
          conjunctive_predicate->getReferencedAttributes());
    }
  }

  // Check whether the conditions are met for pushing down part of the predicates
  // to each small-cardinality stored relation.
  for (const auto &node_pair : applicable_nodes_) {
    const std::vector<E::AttributeReferencePtr> &target_attributes = *node_pair.second;
    std::vector<E::PredicatePtr> selected_disj_preds;
    for (std::size_t i = 0; i < candidate_predicates.size(); ++i) {
      const auto &cand_preds = candidate_predicates[i];
      const auto &cand_attrs = candidate_attributes[i];

      std::vector<E::PredicatePtr> selected_conj_preds;
      for (std::size_t j = 0; j < cand_preds.size(); ++j) {
        if (E::SubsetOfExpressions(cand_attrs[j], target_attributes)) {
          selected_conj_preds.emplace_back(cand_preds[j]);
        }
      }
      if (selected_conj_preds.empty()) {
        // Not every disjunctive component contains a predicate that can be applied
        // to the table reference node -- condition failed, exit.
        selected_disj_preds.clear();
        break;
      } else {
        selected_disj_preds.emplace_back(
            CreateConjunctive(selected_conj_preds));
      }
    }
    if (!selected_disj_preds.empty()) {
      applicable_predicates_[node_pair.first].add(
          CreateDisjunctive(selected_disj_preds));
    }
  }
}

P::PhysicalPtr PushDownLowCostDisjunctivePredicate::attachPredicates(
    const P::PhysicalPtr &input) const {
  std::vector<P::PhysicalPtr> new_children;
  for (const P::PhysicalPtr &child : input->children()) {
    const P::PhysicalPtr new_child = attachPredicates(child);
    new_children.push_back(new_child);
  }

  const P::PhysicalPtr output =
      new_children == input->children() ? input
                                        : input->copyWithNewChildren(new_children);

  const auto &node_it = applicable_predicates_.find(input);
  if (node_it != applicable_predicates_.end()) {
    const E::PredicatePtr filter_predicate =
        CreateConjunctive(node_it->second.predicates);
    return P::Selection::Create(output,
                                E::ToNamedExpressions(output->getOutputAttributes()),
                                filter_predicate);
  }

  return output;
}

E::PredicatePtr PushDownLowCostDisjunctivePredicate::CreateConjunctive(
    const std::vector<E::PredicatePtr> predicates) {
  DCHECK_GE(predicates.size(), 1u);
  if (predicates.size() == 1) {
    return predicates.front();
  } else {
    return E::LogicalAnd::Create(predicates);
  }
}

E::PredicatePtr PushDownLowCostDisjunctivePredicate::CreateDisjunctive(
    const std::vector<E::PredicatePtr> predicates) {
  DCHECK_GE(predicates.size(), 1u);
  if (predicates.size() == 1) {
    return predicates.front();
  } else {
    return E::LogicalOr::Create(predicates);
  }
}

}  // namespace optimizer
}  // namespace quickstep
