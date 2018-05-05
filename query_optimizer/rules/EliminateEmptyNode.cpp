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

#include "query_optimizer/rules/EliminateEmptyNode.hpp"

#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationStatistics.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/UnionAll.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = expressions;
namespace P = physical;

namespace {

std::string GetNewRelationName(const std::size_t id) {
  std::ostringstream out;
  out << OptimizerContext::kInternalTemporaryRelationNamePrefix << id;
  return out.str();
}

bool IsTableReferenceOnEmptyRelation(const P::PhysicalPtr &node) {
  P::TableReferencePtr table_reference;
  if (!P::SomeTableReference::MatchesWithConditionalCast(node, &table_reference)) {
    return false;
  }

  const CatalogRelationStatistics &stat = table_reference->relation()->getStatistics();
  return stat.isExact() && (stat.getNumTuples() == 0u);
}

P::PhysicalPtr ApplyToHashJoin(const P::HashJoinPtr &hash_join) {
  const P::PhysicalPtr &left = hash_join->left();
  if (IsTableReferenceOnEmptyRelation(left)) {
    return nullptr;
  }

  const P::PhysicalPtr &right = hash_join->right();
  switch (hash_join->join_type()) {
    case P::HashJoin::JoinType::kInnerJoin:
    case P::HashJoin::JoinType::kLeftSemiJoin:
      if (IsTableReferenceOnEmptyRelation(right)) {
        return nullptr;
      }
      break;
    case P::HashJoin::JoinType::kLeftAntiJoin: {
      const auto &project_expressions = hash_join->project_expressions();
#ifdef QUICKSTEP_DEBUG
      const auto left_output_attributes = left->getOutputAttributes();
      if (!E::SubsetOfExpressions(project_expressions, left_output_attributes)) {
        std::vector<E::NamedExpressionPtr> non_alias_expressions, alias_expressions;
        for (const auto &project_expression : project_expressions) {
          if (E::SomeAlias::Matches(project_expression)) {
            for (const auto referenced_attribute : project_expression->getReferencedAttributes()) {
              alias_expressions.push_back(referenced_attribute);
            }
          } else {
            non_alias_expressions.push_back(project_expression);
          }
        }

        CHECK(E::SubsetOfExpressions(non_alias_expressions, left_output_attributes));
        CHECK(E::SubsetOfExpressions(alias_expressions, left_output_attributes));
      }
#endif

      if (IsTableReferenceOnEmptyRelation(right)) {
        if (hash_join->residual_predicate()) {
          return nullptr;
        }
        return P::Selection::Create(left, project_expressions, nullptr);
      }
      break;
    }
    case P::HashJoin::JoinType::kLeftOuterJoin:
      if (IsTableReferenceOnEmptyRelation(right)) {
        if (hash_join->residual_predicate()) {
          return nullptr;
        }

        const auto &project_expressions = hash_join->project_expressions();
        if (E::SubsetOfExpressions(project_expressions, left->getOutputAttributes())) {
          return P::Selection::Create(left, project_expressions, nullptr);
        }
      }
      break;
  }

  return hash_join;
}

P::PhysicalPtr ApplyToNode(const P::PhysicalPtr &node) {
  switch (node->getPhysicalType()) {
    case P::PhysicalType::kHashJoin:
      return ApplyToHashJoin(std::static_pointer_cast<const P::HashJoin>(node));
    case P::PhysicalType::kTableReference:
      if (IsTableReferenceOnEmptyRelation(node)) {
        return nullptr;
      }
      break;
    default:
      break;
  }

  return node;
}


P::PhysicalPtr CopyWithNewProjectExpressions(const P::UnionAllPtr &union_all,
                                             const P::PhysicalPtr &child) {
  const auto &project_attributes = union_all->project_attributes();

  std::vector<E::NamedExpressionPtr> alias_expressions;
  P::AggregatePtr aggregate;
  if (P::SomeAggregate::MatchesWithConditionalCast(child, &aggregate)) {
    const auto &aggregate_expressions = aggregate->aggregate_expressions();
    alias_expressions.reserve(aggregate_expressions.size());

    int aid = 0;
    for (const auto &project_attribute : project_attributes) {
      const auto alias_referenced_attributes =
          aggregate_expressions[aid]->getReferencedAttributes();
      DCHECK_EQ(1u, alias_referenced_attributes.size());

      alias_expressions.emplace_back(E::Alias::Create(
          project_attribute->id(),
          alias_referenced_attributes.front(),
          project_attribute->attribute_name(),
          project_attribute->attribute_alias(),
          project_attribute->relation_name()));
      ++aid;
    }
  } else {
    const auto child_output_attributes = child->getOutputAttributes();
    alias_expressions.reserve(child_output_attributes.size());

    int aid = 0;
    for (const auto &project_attribute : project_attributes) {
      alias_expressions.emplace_back(E::Alias::Create(
          project_attribute->id(),
          child_output_attributes[aid],
          project_attribute->attribute_name(),
          project_attribute->attribute_alias(),
          project_attribute->relation_name()));
      ++aid;
    }
  }

  return child->copyWithNewProjectExpressions(alias_expressions);
}

// TopDown approach.
P::PhysicalPtr Apply(const P::PhysicalPtr &node) {
  DCHECK(node);

  const auto new_node = ApplyToNode(node);
  if (new_node == nullptr) {
    return nullptr;
  }

  std::vector<P::PhysicalPtr> new_children;
  bool has_changed_children = false;
  for (const auto &child : new_node->children()) {
    const auto new_child = Apply(child);
    if (new_child == nullptr) {
      switch (node->getPhysicalType()) {
        case P::PhysicalType::kUnionAll:
          has_changed_children = true;
          break;
        case P::PhysicalType::kInsertSelection:
          if (!new_children.empty()) {
            // The actual input is empty.
            return nullptr;
          }
          new_children.push_back(child);
          break;
        default:
          return nullptr;
      }
      continue;
    } else if (child != new_child && !has_changed_children) {
      has_changed_children = true;
    }
    new_children.push_back(new_child);
  }

  if (has_changed_children) {
    if (P::SomeUnionAll::Matches(node)) {
      switch (new_children.size()) {
        case 0u:
          return nullptr;
        case 1u:
          return CopyWithNewProjectExpressions(
              std::static_pointer_cast<const P::UnionAll>(node), new_children.front());
        default:
          break;
      }
    }

    DCHECK(!new_children.empty());
    return new_node->copyWithNewChildren(new_children);
  } else {
    return new_node;
  }
}

}  // namespace

P::PhysicalPtr EliminateEmptyNode::apply(const P::PhysicalPtr &input) {
  DCHECK(P::SomeTopLevelPlan::Matches(input));
  const P::TopLevelPlanPtr &top_level_plan =
      std::static_pointer_cast<const P::TopLevelPlan>(input);

  // TODO(quickstep-team): handle subquery.
  if (!top_level_plan->shared_subplans().empty()) {
    return input;
  }

  const auto &plan = top_level_plan->plan();

  P::SelectionPtr selection;
  if (P::SomeSelection::MatchesWithConditionalCast(plan, &selection) &&
      P::SomeTableReference::Matches(selection->input())) {
    return input;
  }

  const P::PhysicalType plan_type = plan->getPhysicalType();
  std::vector<E::AttributeReferencePtr> project_expressions;
  switch (plan_type) {
    case P::PhysicalType::kAggregate:
    case P::PhysicalType::kFilterJoin:
    case P::PhysicalType::kHashJoin:
    case P::PhysicalType::kNestedLoopsJoin:
    case P::PhysicalType::kSample:
    case P::PhysicalType::kSort:
    case P::PhysicalType::kUnionAll:
    case P::PhysicalType::kWindowAggregate:
      project_expressions = plan->getOutputAttributes();
      break;
    case P::PhysicalType::kCopyFrom:
    case P::PhysicalType::kCreateIndex:
    case P::PhysicalType::kCreateTable:
    case P::PhysicalType::kDeleteTuples:
    case P::PhysicalType::kDropTable:
    case P::PhysicalType::kInsertTuple:
    case P::PhysicalType::kSharedSubplanReference:
    case P::PhysicalType::kTableGenerator:
    case P::PhysicalType::kUpdateTable:
      // TODO(quickstep-team): revisit the cases above.
      return input;
    case P::PhysicalType::kCopyTo:
    case P::PhysicalType::kInsertSelection:
      project_expressions = plan->getReferencedAttributes();
      break;
    case P::PhysicalType::kCrossReferenceCoalesceAggregate:
    case P::PhysicalType::kTableReference:
    case P::PhysicalType::kTopLevelPlan:
      LOG(FATAL) << "Unexpected PhysicalType.";
    case P::PhysicalType::kSelection: {
      const auto &selection_input = selection->input();
      switch (selection_input->getPhysicalType()) {
        case P::PhysicalType::kAggregate:
        case P::PhysicalType::kWindowAggregate: {
          DCHECK(project_expressions.empty());
          const auto referenced_attributes = selection_input->getReferencedAttributes();
          std::unordered_set<E::ExprId> unique_expr_ids;
          for (auto &referenced_attribute : referenced_attributes) {
            const E::ExprId expr_id = referenced_attribute->id();
            if (unique_expr_ids.find(expr_id) == unique_expr_ids.end()) {
              project_expressions.emplace_back(std::move(referenced_attribute));
              unique_expr_ids.insert(expr_id);
            }
          }
          break;
        }
        case P::PhysicalType::kCrossReferenceCoalesceAggregate:
          LOG(FATAL) << "Unexpected PhysicalType.";
        default:
          project_expressions = plan->getOutputAttributes();
          break;
      }
      break;
    }
  }

  auto output = Apply(plan);
  if (output == plan) {
    return input;
  }

  if (output) {
    return input->copyWithNewChildren({output});
  }

#ifdef QUICKSTEP_DEBUG
  {
    CHECK(!project_expressions.empty());

    std::unordered_set<E::ExprId> unique_expr_ids;
    unique_expr_ids.reserve(project_expressions.size());
    for (const auto &project_expression : project_expressions) {
      unique_expr_ids.insert(project_expression->id());
    }
    CHECK_EQ(project_expressions.size(), unique_expr_ids.size());
  }
#endif

  auto catalog_relation = std::make_unique<CatalogRelation>(
      catalog_database_, GetNewRelationName(catalog_database_->size()), -1, true);

  attribute_id aid = 0;
  std::unordered_set<std::string> relation_name_alias;
  for (const auto &project_expression : project_expressions) {
    // The attribute name is simply set to the attribute id to make it distinct.
    auto catalog_attribute = std::make_unique<CatalogAttribute>(
        catalog_relation.get(), project_expression->attribute_name(),
        project_expression->getValueType(), aid,
        project_expression->attribute_alias());
    catalog_relation->addAttribute(catalog_attribute.release());
    ++aid;

    relation_name_alias.insert(project_expression->relation_name());
  }

  // TODO(quickstep-team): use Nop physical node.
  const auto temp_table =
      P::TableReference::Create(catalog_relation.get(),
                                relation_name_alias.size() == 1u ? *relation_name_alias.begin() : "",
                                project_expressions);
  catalog_database_->addRelation(catalog_relation.release());

  switch (plan_type) {
    case P::PhysicalType::kAggregate:
    case P::PhysicalType::kCopyTo:
    case P::PhysicalType::kWindowAggregate:
      output = plan->copyWithNewChildren({ temp_table });
      break;
    case P::PhysicalType::kFilterJoin:
    case P::PhysicalType::kHashJoin:
    case P::PhysicalType::kNestedLoopsJoin:
    case P::PhysicalType::kSample:
    case P::PhysicalType::kSort:
    case P::PhysicalType::kUnionAll:
      output = P::Selection::Create(temp_table, E::ToNamedExpressions(project_expressions), nullptr);
      break;
    case P::PhysicalType::kInsertSelection:
      output = plan->copyWithNewChildren({ plan->children().front(), temp_table });
      break;
    case P::PhysicalType::kSelection: {
      const P::PhysicalPtr &selection_input = selection->input();
      switch (selection_input->getPhysicalType()) {
        case P::PhysicalType::kAggregate:
        case P::PhysicalType::kWindowAggregate:
          output = selection_input->copyWithNewChildren({ temp_table });
          break;
        case P::PhysicalType::kCrossReferenceCoalesceAggregate:
          LOG(FATAL) << "Unexpected PhysicalType.";
        default:
          output = temp_table;
          break;
      }
      output = selection->copyWithNewChildren({ output });
      break;
    }
    default:
      LOG(FATAL) << "Unexpected PhysicalType.";
  }

  DCHECK(output);
  return input->copyWithNewChildren({output});
}

}  // namespace optimizer
}  // namespace quickstep
