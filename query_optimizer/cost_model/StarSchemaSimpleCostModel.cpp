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

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationStatistics.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "query_optimizer/cost_model/CostModel.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/LogicalOr.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/CrossReferenceCoalesceAggregate.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/FilterJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/SharedSubplanReference.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/UnionAll.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/NullType.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "gflags/gflags.h"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace cost {

DEFINE_int64(collision_free_vector_table_max_size, 1000000000,
             "The maximum allowed key range (number of entries) for using a "
             "CollisionFreeVectorTable.");

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

std::size_t StarSchemaSimpleCostModel::estimateCardinality(
    const P::PhysicalPtr &physical_plan) {
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kTopLevelPlan:
      return estimateCardinalityForTopLevelPlan(
          std::static_pointer_cast<const P::TopLevelPlan>(physical_plan));
    case P::PhysicalType::kTableReference:
      return estimateCardinalityForTableReference(
          std::static_pointer_cast<const P::TableReference>(physical_plan));
    case P::PhysicalType::kSelection:
      return estimateCardinalityForSelection(
          std::static_pointer_cast<const P::Selection>(physical_plan));
    case P::PhysicalType::kTableGenerator:
      return estimateCardinalityForTableGenerator(
          std::static_pointer_cast<const P::TableGenerator>(physical_plan));
    case P::PhysicalType::kFilterJoin:
      return estimateCardinalityForFilterJoin(
          std::static_pointer_cast<const P::FilterJoin>(physical_plan));
    case P::PhysicalType::kHashJoin:
      return estimateCardinalityForHashJoin(
          std::static_pointer_cast<const P::HashJoin>(physical_plan));
    case P::PhysicalType::kNestedLoopsJoin:
      return estimateCardinalityForNestedLoopsJoin(
          std::static_pointer_cast<const P::NestedLoopsJoin>(physical_plan));
    case P::PhysicalType::kAggregate:
      return estimateCardinalityForAggregate(
          std::static_pointer_cast<const P::Aggregate>(physical_plan));
    case P::PhysicalType::kCrossReferenceCoalesceAggregate:
      return estimateCardinalityForCrossReferenceCoalesceAggregate(
          std::static_pointer_cast<const P::CrossReferenceCoalesceAggregate>(physical_plan));
    case P::PhysicalType::kSharedSubplanReference: {
      const P::SharedSubplanReferencePtr shared_subplan_reference =
          std::static_pointer_cast<const P::SharedSubplanReference>(physical_plan);
      return estimateCardinality(
          shared_subplans_[shared_subplan_reference->subplan_id()]);
    }
    case P::PhysicalType::kSort:
      return estimateCardinalityForSort(
          std::static_pointer_cast<const P::Sort>(physical_plan));
    case P::PhysicalType::kWindowAggregate:
      return estimateCardinalityForWindowAggregate(
          std::static_pointer_cast<const P::WindowAggregate>(physical_plan));
    case P::PhysicalType::kUnionAll:
      return estimateCardinalityForUnionAll(
          std::static_pointer_cast<const P::UnionAll>(physical_plan));
    default:
      throw UnsupportedPhysicalPlan(physical_plan);
  }
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForTopLevelPlan(
    const P::TopLevelPlanPtr &physical_plan) {
  return estimateCardinality(physical_plan->plan());
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForTableReference(
    const P::TableReferencePtr &physical_plan) {
  const CatalogRelation *relation = physical_plan->relation();
  const CatalogRelationStatistics &stat = relation->getStatistics();
  const std::size_t num_tuples =
      stat.hasNumTuples() ? stat.getNumTuples()
                          : relation->estimateTupleCardinality();
  return num_tuples;
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForSelection(
    const P::SelectionPtr &physical_plan) {
  double selectivity =
      estimateSelectivityForPredicate(physical_plan->filter_predicate(), physical_plan);
  return static_cast<std::size_t>(
      estimateCardinality(physical_plan->input()) * selectivity);
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForSort(
    const physical::SortPtr &physical_plan) {
  std::size_t cardinality = estimateCardinality(
      std::static_pointer_cast<const P::Sort>(physical_plan)->input());
  return std::min(cardinality, static_cast<std::size_t>(physical_plan->limit()));
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForTableGenerator(
    const P::TableGeneratorPtr &physical_plan) {
  return physical_plan->generator_function_handle()->getEstimatedCardinality();
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForFilterJoin(
    const P::FilterJoinPtr &physical_plan) {
  double build_side_filter_selectivity =
      estimateSelectivityForPredicate(physical_plan->build_side_filter_predicate(),
                                      physical_plan->right());
  std::size_t left_cardinality = estimateCardinality(physical_plan->left());
  double right_selectivity = estimateSelectivity(physical_plan->right());
  return static_cast<std::size_t>(
      left_cardinality * build_side_filter_selectivity * right_selectivity + 0.5);
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForHashJoin(
    const P::HashJoinPtr &physical_plan) {
  std::size_t left_cardinality = estimateCardinality(physical_plan->left());
  std::size_t right_cardinality = estimateCardinality(physical_plan->right());
  double left_selectivity = estimateSelectivity(physical_plan->left());
  double right_selectivity = estimateSelectivity(physical_plan->right());
  return std::max(static_cast<std::size_t>(left_cardinality * right_selectivity + 0.5),
                  static_cast<std::size_t>(right_cardinality * left_selectivity + 0.5));
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForNestedLoopsJoin(
    const P::NestedLoopsJoinPtr &physical_plan) {
  return std::max(estimateCardinality(physical_plan->left()),
                  estimateCardinality(physical_plan->right()));
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForAggregate(
    const P::AggregatePtr &physical_plan) {
  double filter_selectivity =
      estimateSelectivityForPredicate(physical_plan->filter_predicate(), physical_plan);
  return static_cast<std::size_t>(
      estimateNumGroupsForAggregate(physical_plan) * filter_selectivity);
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForCrossReferenceCoalesceAggregate(
    const P::CrossReferenceCoalesceAggregatePtr &physical_plan) {
  return estimateCardinality(physical_plan->left_child());
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForWindowAggregate(
    const P::WindowAggregatePtr &physical_plan) {
  return estimateCardinality(physical_plan->input());
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForUnionAll(
    const P::UnionAllPtr &physical_plan) {
  std::size_t cardinality = 0;
  for (const P::PhysicalPtr &operand : physical_plan->operands()) {
    cardinality += estimateCardinality(operand);
  }
  return cardinality;
}

std::size_t StarSchemaSimpleCostModel::estimateNumGroupsForAggregate(
    const physical::AggregatePtr &aggregate) {
  if (aggregate->grouping_expressions().empty()) {
    return 1uL;
  }

  std::size_t estimated_child_cardinality = estimateCardinality(aggregate->input());
  std::size_t estimated_num_groups = 1;
  std::size_t max_attr_num_distinct_values = 0;
  for (const auto &expr : aggregate->grouping_expressions()) {
    E::AttributeReferencePtr attr;
    if (E::SomeAttributeReference::MatchesWithConditionalCast(expr, &attr)) {
      std::size_t attr_num_distinct_values =
          estimateNumDistinctValues(attr->id(), aggregate->input());
      estimated_num_groups *= std::max(1uL, attr_num_distinct_values);
      max_attr_num_distinct_values =
          std::max(max_attr_num_distinct_values, attr_num_distinct_values);
    } else {
      // TODO(jianqiao): implement estimateNumDistinctValues() for expressions.
      estimated_num_groups *= 64uL;
    }
  }
  estimated_num_groups = std::max(
      std::min(estimated_num_groups, estimated_child_cardinality / 10),
      max_attr_num_distinct_values);
  return estimated_num_groups;
}


std::size_t StarSchemaSimpleCostModel::estimateNumDistinctValues(
    const expressions::ExprId attribute_id,
    const physical::PhysicalPtr &physical_plan) {
  DCHECK(E::ContainsExprId(physical_plan->getOutputAttributes(), attribute_id));

  P::TableReferencePtr table_reference;
  if (P::SomeTableReference::MatchesWithConditionalCast(physical_plan, &table_reference)) {
    return getNumDistinctValues(attribute_id, table_reference);
  }

  double filter_selectivity = estimateSelectivityForFilterPredicate(physical_plan);
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kSelection:  // Fall through
    case P::PhysicalType::kAggregate: {
      const P::PhysicalPtr &child = physical_plan->children()[0];
      if (E::ContainsExprId(child->getOutputAttributes(), attribute_id)) {
        std::size_t child_num_distinct_values =
            estimateNumDistinctValues(attribute_id, child);
        return static_cast<std::size_t>(
            child_num_distinct_values * filter_selectivity + 0.5);
      }
      break;
    }
    case P::PhysicalType::kCrossReferenceCoalesceAggregate: {
      const P::PhysicalPtr left_child = physical_plan->children()[0];
      if (E::ContainsExprId(left_child->getOutputAttributes(), attribute_id)) {
        return estimateNumDistinctValues(attribute_id, left_child);
      }
      break;
    }
    case P::PhysicalType::kFilterJoin: {
      const P::FilterJoinPtr &filter_join =
          std::static_pointer_cast<const P::FilterJoin>(physical_plan);
      if (E::ContainsExprId(filter_join->left()->getOutputAttributes(), attribute_id)) {
        std::size_t left_child_num_distinct_values =
            estimateNumDistinctValues(attribute_id, filter_join->left());
        double right_child_selectivity =
            estimateSelectivity(filter_join->right());
        return static_cast<std::size_t>(
            left_child_num_distinct_values * right_child_selectivity + 0.5);
      }
    }
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr &hash_join =
          std::static_pointer_cast<const P::HashJoin>(physical_plan);
      if (E::ContainsExprId(hash_join->left()->getOutputAttributes(), attribute_id)) {
        std::size_t left_child_num_distinct_values =
            estimateNumDistinctValues(attribute_id, hash_join->left());
        double right_child_selectivity =
            estimateSelectivity(hash_join->right());
        return static_cast<std::size_t>(
            left_child_num_distinct_values * right_child_selectivity * filter_selectivity + 0.5);
      }
      if (E::ContainsExprId(hash_join->right()->getOutputAttributes(), attribute_id)) {
        std::size_t right_child_num_distinct_values =
            estimateNumDistinctValues(attribute_id, hash_join->right());
        double left_child_selectivity =
            estimateSelectivity(hash_join->left());
        return static_cast<std::size_t>(
            right_child_num_distinct_values * left_child_selectivity * filter_selectivity + 0.5);
      }
    }
    default:
      break;
  }

  return 16uL;
}

double StarSchemaSimpleCostModel::estimateSelectivity(
    const physical::PhysicalPtr &physical_plan) {
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr &aggregate =
          std::static_pointer_cast<const P::Aggregate>(physical_plan);
      return estimateSelectivity(aggregate->input()) *
          estimateSelectivityForFilterPredicate(aggregate);
    }
    case P::PhysicalType::kCrossReferenceCoalesceAggregate: {
      const P::CrossReferenceCoalesceAggregatePtr &aggregate_on_left_outer_join =
          std::static_pointer_cast<const P::CrossReferenceCoalesceAggregate>(physical_plan);
      return estimateSelectivity(aggregate_on_left_outer_join->left_child());
    }
    case P::PhysicalType::kSelection: {
      const P::SelectionPtr &selection =
          std::static_pointer_cast<const P::Selection>(physical_plan);
      double filter_selectivity =
          estimateSelectivityForPredicate(selection->filter_predicate(), selection);
      double child_selectivity = estimateSelectivity(selection->input());
      return filter_selectivity * child_selectivity;
    }
    case P::PhysicalType::kFilterJoin: {
      const P::FilterJoinPtr &filter_join =
          std::static_pointer_cast<const P::FilterJoin>(physical_plan);
      double left_selectivity = estimateSelectivity(filter_join->left());
      double right_selectivity = estimateSelectivity(filter_join->right());
      double build_side_filter_selectivity =
          estimateSelectivityForPredicate(filter_join->build_side_filter_predicate(),
                                          filter_join->right());
      return left_selectivity * right_selectivity * build_side_filter_selectivity;
    }
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr &hash_join =
          std::static_pointer_cast<const P::HashJoin>(physical_plan);
      double filter_selectivity =
          estimateSelectivityForPredicate(hash_join->residual_predicate(), hash_join);
      double child_selectivity =
          estimateSelectivity(hash_join->left()) * estimateSelectivity(hash_join->right());
      return filter_selectivity * child_selectivity;
    }
    case P::PhysicalType::kNestedLoopsJoin: {
      const P::NestedLoopsJoinPtr &nested_loop_join =
          std::static_pointer_cast<const P::NestedLoopsJoin>(physical_plan);
      double filter_selectivity =
          estimateSelectivityForPredicate(nested_loop_join->join_predicate(), nested_loop_join);
      double child_selectivity = std::min(
          estimateSelectivity(nested_loop_join->left()),
          estimateSelectivity(nested_loop_join->right()));
      return filter_selectivity * child_selectivity;
    }
    case P::PhysicalType::kSharedSubplanReference: {
      const P::SharedSubplanReferencePtr shared_subplan_reference =
          std::static_pointer_cast<const P::SharedSubplanReference>(physical_plan);
      return estimateSelectivity(
          shared_subplans_[shared_subplan_reference->subplan_id()]);
    }
    default:
      break;
  }

  if (physical_plan->getNumChildren() == 1) {
    return estimateSelectivity(physical_plan->children()[0]);
  }

  return 1.0;
}

double StarSchemaSimpleCostModel::estimateSelectivityForFilterPredicate(
    const physical::PhysicalPtr &physical_plan) {
  P::PhysicalPtr target_plan = physical_plan;
  E::PredicatePtr filter_predicate = nullptr;
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kSelection:
      filter_predicate =
          std::static_pointer_cast<const P::Selection>(physical_plan)->filter_predicate();
      break;
    case P::PhysicalType::kAggregate:
      filter_predicate =
          std::static_pointer_cast<const P::Aggregate>(physical_plan)->filter_predicate();
      target_plan = physical_plan->children()[0];
      break;
    case P::PhysicalType::kHashJoin:
      filter_predicate =
          std::static_pointer_cast<const P::HashJoin>(physical_plan)->residual_predicate();
      break;
    case P::PhysicalType::kNestedLoopsJoin:
      filter_predicate =
          std::static_pointer_cast<const P::NestedLoopsJoin>(physical_plan)->join_predicate();
      break;
    default:
      break;
  }

  if (filter_predicate == nullptr) {
    return 1.0;
  } else {
    return estimateSelectivityForPredicate(filter_predicate, target_plan);
  }
}


double StarSchemaSimpleCostModel::estimateSelectivityForPredicate(
    const expressions::PredicatePtr &filter_predicate,
    const P::PhysicalPtr &physical_plan) {
  if (filter_predicate == nullptr) {
    return 1.0;
  }

  switch (filter_predicate->getExpressionType()) {
    case E::ExpressionType::kComparisonExpression: {
      // Case 1 - Number of distinct values statistics available
      //   Case 1.1 - Equality comparison: 1.0 / num_distinct_values
      //   Case 1.2 - Otherwise: 0.1
      // Case 2 - Otherwise: 0.5
      const E::ComparisonExpressionPtr &comparison_expression =
          std::static_pointer_cast<const E::ComparisonExpression>(filter_predicate);
      E::AttributeReferencePtr attr;
      if ((E::SomeAttributeReference::MatchesWithConditionalCast(comparison_expression->left(), &attr) &&
           E::SomeScalarLiteral::Matches(comparison_expression->right())) ||
          (E::SomeAttributeReference::MatchesWithConditionalCast(comparison_expression->right(), &attr) &&
           E::SomeScalarLiteral::Matches(comparison_expression->left()))) {
        for (const auto &child : physical_plan->children()) {
          if (E::ContainsExprId(child->getOutputAttributes(), attr->id())) {
            const std::size_t child_num_distinct_values = estimateNumDistinctValues(attr->id(), child);
            if (comparison_expression->isEqualityComparisonPredicate()) {
              return 1.0 / child_num_distinct_values;
            } else {
              return 1.0 / std::max(std::min(child_num_distinct_values / 100.0, 10.0), 2.0);
            }
          }
        }
        return 0.1;
      }
      return 0.5;
    }
    case E::ExpressionType::kLogicalAnd: {
      const E::LogicalAndPtr &logical_and =
          std::static_pointer_cast<const E::LogicalAnd>(filter_predicate);
      double selectivity = 1.0;
      for (const auto &predicate : logical_and->operands()) {
        selectivity = std::min(selectivity, estimateSelectivityForPredicate(predicate, physical_plan));
      }
      return selectivity;
    }
    case E::ExpressionType::kLogicalOr: {
      const E::LogicalOrPtr &logical_or =
          std::static_pointer_cast<const E::LogicalOr>(filter_predicate);
      double selectivity = 0;
      for (const auto &predicate : logical_or->operands()) {
        selectivity += estimateSelectivityForPredicate(predicate, physical_plan);
      }
      return std::min(selectivity, 1.0);
    }
    default:
      break;
  }
  return 1.0;
}

std::size_t StarSchemaSimpleCostModel::getNumDistinctValues(
    const E::ExprId attribute_id,
    const P::TableReferencePtr &table_reference) {
  const auto rel_attr_id =
      findCatalogRelationAttributeId(table_reference, attribute_id);
  if (rel_attr_id != kInvalidAttributeID) {
    const CatalogRelationStatistics &stat =
        table_reference->relation()->getStatistics();
    if (stat.hasNumDistinctValues(rel_attr_id)) {
      return stat.getNumDistinctValues(rel_attr_id);
    }
  }
  return estimateCardinalityForTableReference(table_reference);
}

bool StarSchemaSimpleCostModel::impliesUniqueAttributes(
    const P::PhysicalPtr &physical_plan,
    const std::vector<E::AttributeReferencePtr> &attributes) {
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr &aggregate =
          std::static_pointer_cast<const P::Aggregate>(physical_plan);
      return E::SubsetOfExpressions(aggregate->grouping_expressions(), attributes);
    }
    case P::PhysicalType::kCrossReferenceCoalesceAggregate: {
      const P::CrossReferenceCoalesceAggregatePtr &aggregate_on_left_outer_join =
          std::static_pointer_cast<const P::CrossReferenceCoalesceAggregate>(physical_plan);
      return E::SubsetOfExpressions(
          aggregate_on_left_outer_join->left_join_attributes(), attributes);
    }
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr &hash_join =
          std::static_pointer_cast<const P::HashJoin>(physical_plan);
      bool unique_from_left =
          impliesUniqueAttributes(hash_join->right(), hash_join->right_join_attributes())
              && impliesUniqueAttributes(hash_join->left(), attributes);
      bool unique_from_right =
          impliesUniqueAttributes(hash_join->left(), hash_join->left_join_attributes())
              && impliesUniqueAttributes(hash_join->right(), attributes);
      return unique_from_left || unique_from_right;
    }
    case P::PhysicalType::kTableReference: {
      const P::TableReferencePtr &table_reference =
          std::static_pointer_cast<const P::TableReference>(physical_plan);
      const CatalogRelationStatistics &stat =
          table_reference->relation()->getStatistics();
      if (stat.hasNumTuples()) {
        const std::size_t num_tuples = stat.getNumTuples();
        for (const auto &attr : attributes) {
          const attribute_id rel_attr_id =
              findCatalogRelationAttributeId(table_reference, attr->id());
          if (rel_attr_id != kInvalidAttributeID &&
              stat.hasNumDistinctValues(rel_attr_id) &&
              stat.getNumDistinctValues(rel_attr_id) == num_tuples) {
            return true;
          }
        }
      }
      return false;
    }
    case P::PhysicalType::kSample:  // Fall through
    case P::PhysicalType::kSelection:
    case P::PhysicalType::kSort: {
      DCHECK_EQ(physical_plan->getNumChildren(), 1u);
      return impliesUniqueAttributes(physical_plan->children()[0], attributes);
    }
    default:
      break;
  }
  return false;
}

TypedValue StarSchemaSimpleCostModel::findCatalogRelationStat(
    const P::PhysicalPtr &physical_plan,
    const E::ExprId attr_id,
    const StatType stat_type,
    bool *is_exact_stat) {
  P::TableReferencePtr table_reference;
  if (P::SomeTableReference::MatchesWithConditionalCast(physical_plan, &table_reference)) {
    const attribute_id rel_attr_id =
        findCatalogRelationAttributeId(table_reference, attr_id);
    if (rel_attr_id != kInvalidAttributeID) {
      const CatalogRelationStatistics &stat =
          table_reference->relation()->getStatistics();

      if (is_exact_stat != nullptr) {
        *is_exact_stat = stat.isExact();
      }

      switch (stat_type) {
        case StatType::kMin: {
          if (stat.hasMinValue(rel_attr_id)) {
            return stat.getMinValue(rel_attr_id);
          }
          break;
        }
        case StatType::kMax: {
          if (stat.hasMaxValue(rel_attr_id)) {
            return stat.getMaxValue(rel_attr_id);
          }
          break;
        }
        default:
          break;
      }
      return NullType::InstanceNullable().makeNullValue();
    }
  }

  for (const auto &child : physical_plan->children()) {
    if (E::ContainsExprId(child->getOutputAttributes(), attr_id)) {
      return findCatalogRelationStat(child, attr_id, stat_type, is_exact_stat);
    }
  }
  return NullType::InstanceNullable().makeNullValue();
}

attribute_id StarSchemaSimpleCostModel::findCatalogRelationAttributeId(
    const physical::TableReferencePtr &table_reference,
    const expressions::ExprId expr_id) {
  const auto &attribute_list = table_reference->attribute_list();
  for (std::size_t i = 0; i < attribute_list.size(); ++i) {
    if (attribute_list[i]->id() == expr_id) {
      return i;
    }
  }
  return kInvalidAttributeID;
}

bool StarSchemaSimpleCostModel::canUseCollisionFreeAggregation(
    const P::AggregatePtr &aggregate,
    const std::size_t estimated_num_groups,
    std::size_t *max_num_groups) {
#ifdef QUICKSTEP_DISTRIBUTED
  // Currently we cannot do this fast path with the distributed setting. See
  // the TODOs at InitializeAggregationOperator::getAllWorkOrderProtos() and
  // FinalizeAggregationOperator::getAllWorkOrderProtos().
  return false;
#endif

  // Supports only single group-by key.
  if (aggregate->grouping_expressions().size() != 1) {
    return false;
  }

  // We need to know the exact min/max stats of the group-by key.
  // So it must be a CatalogAttribute (but not an expression).
  E::AttributeReferencePtr group_by_key_attr;
  const E::ExpressionPtr agg_expr = aggregate->grouping_expressions().front();
  if (!E::SomeAttributeReference::MatchesWithConditionalCast(agg_expr, &group_by_key_attr)) {
    return false;
  }

  bool min_value_stat_is_exact;
  bool max_value_stat_is_exact;
  const TypedValue min_value = findMinValueStat(
          aggregate, group_by_key_attr, &min_value_stat_is_exact);
  const TypedValue max_value = findMaxValueStat(
          aggregate, group_by_key_attr, &max_value_stat_is_exact);
  if (min_value.isNull() || max_value.isNull() ||
      (!min_value_stat_is_exact) || (!max_value_stat_is_exact)) {
    return false;
  }

  std::int64_t min_cpp_value;
  std::int64_t max_cpp_value;
  switch (group_by_key_attr->getValueType().getTypeID()) {
    case TypeID::kInt: {
      min_cpp_value = min_value.getLiteral<int>();
      max_cpp_value = max_value.getLiteral<int>();
      break;
    }
    case TypeID::kLong: {
      min_cpp_value = min_value.getLiteral<std::int64_t>();
      max_cpp_value = max_value.getLiteral<std::int64_t>();
      break;
    }
    default:
      return false;
  }

  // TODO(jianqiao):
  // 1. Handle the case where min_cpp_value is below 0 or far greater than 0.
  // 2. Reason about the table size bound (e.g. by checking memory size) instead
  //    of hardcoding it as a gflag.
  if (min_cpp_value < 0 ||
      max_cpp_value >= FLAGS_collision_free_vector_table_max_size ||
      max_cpp_value / static_cast<double>(estimated_num_groups) > 256.0) {
    return false;
  }

  for (const auto &agg_expr : aggregate->aggregate_expressions()) {
    const E::AggregateFunctionPtr agg_func =
        std::static_pointer_cast<const E::AggregateFunction>(agg_expr->expression());

    if (agg_func->is_distinct()) {
      return false;
    }

    // TODO(jianqiao): Support AggregationID::AVG.
    if (!QUICKSTEP_EQUALS_ANY_CONSTANT(agg_func->getAggregate().getAggregationID(),
                                       AggregationID::kCount,
                                       AggregationID::kSum)) {
      return false;
    }

    const auto &arguments = agg_func->getArguments();
    if (arguments.size() > 1u) {
      return false;
    }

    if (arguments.size() == 1u) {
      if (!QUICKSTEP_EQUALS_ANY_CONSTANT(arguments.front()->getValueType().getTypeID(),
                                         TypeID::kInt,
                                         TypeID::kLong,
                                         TypeID::kFloat,
                                         TypeID::kDouble)) {
        return false;
      }
    }
  }

  *max_num_groups = static_cast<std::size_t>(max_cpp_value) + 1;
  return true;
}

bool StarSchemaSimpleCostModel::canUseTwoPhaseCompactKeyAggregation(
    const physical::AggregatePtr &aggregate,
    const std::size_t estimated_num_groups) {
  // Require estimated number of groups to be below the specified threshold.
  //
  // TODO(jianqiao): It is good to have the threshold to be the same as
  // FLAGS_partition_aggregation_num_groups_threshold which is defined in
  // AggregationOperationState.cpp. However, there seems to be no sound place
  // to put that flag so that it can be shared by the two cpp files (optimizer
  // vs backend). So here we hardcode the threshold and leave it to be solved
  // later.
  if (estimated_num_groups >= 10000u) {
    return false;
  }

  // Require fix-length non-nullable keys that can be packed into a 64-bit QWORD.
  std::size_t total_key_size = 0;
  for (const auto &key_expr : aggregate->grouping_expressions()) {
    const Type &type = key_expr->getValueType();
    if (type.isVariableLength() || type.isNullable()) {
      return false;
    }
    total_key_size += type.maximumByteLength();
  }

  if (total_key_size > sizeof(std::uint64_t)) {
    return false;
  }

  // Check aggregate arguments.
  for (const auto &agg_alias : aggregate->aggregate_expressions()) {
    const E::AggregateFunctionPtr agg_expr =
        std::static_pointer_cast<const E::AggregateFunction>(agg_alias->expression());

    // Not supporting DISTINCT aggregation.
    if (agg_expr->is_distinct()) {
      return false;
    }

    // Currently we do not handle NULL values.
    const auto &arguments = agg_expr->getArguments();
    for (const auto &arg : arguments) {
      if (arg->getValueType().isNullable()) {
        return false;
      }
    }

    // Restricted to COUNT/SUM with INT/LONG/FLOAT/DOUBLE arguments.
    switch (agg_expr->getAggregate().getAggregationID()) {
      case AggregationID::kCount:
        break;
      case AggregationID::kSum: {
        DCHECK_EQ(1u, arguments.size());
        if (!QUICKSTEP_EQUALS_ANY_CONSTANT(arguments.front()->getValueType().getTypeID(),
                                           kInt, kLong, kFloat, kDouble)) {
          return false;
        }
        break;
      }
      default:
        return false;
    }
  }

  return true;
}

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep
