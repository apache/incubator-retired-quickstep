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
#include <memory>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationStatistics.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/LogicalOr.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/SharedSubplanReference.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace cost {

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
    case P::PhysicalType::kHashJoin:
      return estimateCardinalityForHashJoin(
          std::static_pointer_cast<const P::HashJoin>(physical_plan));
    case P::PhysicalType::kNestedLoopsJoin:
      return estimateCardinalityForNestedLoopsJoin(
          std::static_pointer_cast<const P::NestedLoopsJoin>(physical_plan));
    case P::PhysicalType::kAggregate:
      return estimateCardinalityForAggregate(
          std::static_pointer_cast<const P::Aggregate>(physical_plan));
    case P::PhysicalType::kSharedSubplanReference: {
      const P::SharedSubplanReferencePtr shared_subplan_reference =
          std::static_pointer_cast<const P::SharedSubplanReference>(physical_plan);
      return estimateCardinality(
          shared_subplans_[shared_subplan_reference->subplan_id()]);
    }
    case P::PhysicalType::kSort:
      return estimateCardinality(
          std::static_pointer_cast<const P::Sort>(physical_plan)->input());
    case P::PhysicalType::kWindowAggregate:
      return estimateCardinalityForWindowAggregate(
          std::static_pointer_cast<const P::WindowAggregate>(physical_plan));
    default:
      LOG(FATAL) << "Unsupported physical plan:" << physical_plan->toString();
  }
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForTopLevelPlan(
    const P::TopLevelPlanPtr &physical_plan) {
  return estimateCardinality(physical_plan->plan());
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForTableReference(
    const P::TableReferencePtr &physical_plan) {
  std::size_t num_tuples = physical_plan->relation()->getStatistics().getNumTuples();
  if (num_tuples == 0) {
    num_tuples = physical_plan->relation()->estimateTupleCardinality();
  }
  return num_tuples;
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForSelection(
    const P::SelectionPtr &physical_plan) {
  double selectivity = estimateSelectivityForSelection(physical_plan);
  return std::max(static_cast<std::size_t>(estimateCardinality(physical_plan->input()) * selectivity),
                  static_cast<std::size_t>(1));
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForTableGenerator(
    const P::TableGeneratorPtr &physical_plan) {
  return physical_plan->generator_function_handle()->getEstimatedCardinality();
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForHashJoin(
    const P::HashJoinPtr &physical_plan) {
  const P::PhysicalPtr &left_child = physical_plan->left();
  const P::PhysicalPtr &right_child = physical_plan->right();

  std::size_t left_cardinality = estimateCardinality(left_child);
  std::size_t right_cardinality = estimateCardinality(right_child);

  std::size_t estimated_cardinality = std::max(left_cardinality, right_cardinality);
  if (left_child->impliesUniqueAttributes(physical_plan->left_join_attributes())) {
    double left_selectivity = estimateSelectivity(left_child);
    estimated_cardinality =
        std::min(estimated_cardinality,
                 static_cast<std::size_t>(right_cardinality * left_selectivity));
  }
  if (right_child->impliesUniqueAttributes(physical_plan->right_join_attributes())) {
    double right_selectivity = estimateSelectivity(right_child);
    estimated_cardinality =
        std::min(estimated_cardinality,
                 static_cast<std::size_t>(left_cardinality * right_selectivity));
  }
  return estimated_cardinality;
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForNestedLoopsJoin(
    const P::NestedLoopsJoinPtr &physical_plan) {
  return std::max(estimateCardinality(physical_plan->left()),
                  estimateCardinality(physical_plan->right()));
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForAggregate(
    const P::AggregatePtr &physical_plan) {
  if (physical_plan->grouping_expressions().empty()) {
    return 1;
  }
  return std::max(static_cast<std::size_t>(1),
                  estimateCardinality(physical_plan->input()) / 100);
}

std::size_t StarSchemaSimpleCostModel::estimateCardinalityForWindowAggregate(
    const P::WindowAggregatePtr &physical_plan) {
  return estimateCardinality(physical_plan->input());
}

double StarSchemaSimpleCostModel::estimateSelectivity(
    const physical::PhysicalPtr &physical_plan) {
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kSelection: {
      return estimateSelectivityForSelection(
          std::static_pointer_cast<const P::Selection>(physical_plan));
    }
    case P::PhysicalType::kHashJoin: {
      const P::HashJoinPtr &hash_join =
          std::static_pointer_cast<const P::HashJoin>(physical_plan);
      double left_selectivity =
          hash_join->left_filter_predicate() == nullptr
              ? estimateSelectivity(hash_join->left())
              : estimateSelectivityForPlanWithFilterPredicate(hash_join->left(),
                                                              hash_join->left_filter_predicate());
      double right_selectivity = estimateSelectivity(hash_join->right());
      double min_sel = std::min(left_selectivity, right_selectivity);
      double max_sel = std::max(left_selectivity, right_selectivity);
      if (max_sel < 1) {
        min_sel *= std::max(max_sel, 0.9);
      }
      return min_sel;
    }
    case P::PhysicalType::kNestedLoopsJoin: {
      const P::NestedLoopsJoinPtr &nested_loop_join =
          std::static_pointer_cast<const P::NestedLoopsJoin>(physical_plan);
      return std::min(estimateSelectivity(nested_loop_join->left()),
                      estimateSelectivity(nested_loop_join->right()));
    }
    case P::PhysicalType::kSharedSubplanReference: {
      const P::SharedSubplanReferencePtr shared_subplan_reference =
          std::static_pointer_cast<const P::SharedSubplanReference>(physical_plan);
      return estimateSelectivity(
          shared_subplans_[shared_subplan_reference->subplan_id()]);
    }
    default:
      return 1.0;
  }
}

double StarSchemaSimpleCostModel::estimateSelectivityForSelection(
    const physical::SelectionPtr &physical_plan) {
  return estimateSelectivityForPlanWithFilterPredicate(physical_plan->input(),
                                                       physical_plan->filter_predicate());
}


double StarSchemaSimpleCostModel::estimateSelectivityForPlanWithFilterPredicate(
    const physical::PhysicalPtr &input_plan,
    const expressions::PredicatePtr &filter_predicate) {
  // If the subplan is a table reference, gather the number of distinct values
  // statistics for each column (attribute).
  std::unordered_map<E::ExprId, std::size_t> num_distinct_values_map;
  if (input_plan->getPhysicalType() == P::PhysicalType::kTableReference) {
    const P::TableReferencePtr &table_reference =
        std::static_pointer_cast<const P::TableReference>(input_plan);
    const CatalogRelation &relation = *table_reference->relation();
    const std::vector<E::AttributeReferencePtr> &attributes = table_reference->attribute_list();
    for (std::size_t i = 0; i < attributes.size(); ++i) {
      std::size_t num_distinct_values = relation.getStatistics().getNumDistinctValues(i);
      if (num_distinct_values > 0) {
        num_distinct_values_map[attributes[i]->id()] = num_distinct_values;
      }
    }
  }

  return estimateSelectivityForPredicate(num_distinct_values_map, filter_predicate);
}


double StarSchemaSimpleCostModel::estimateSelectivityForPredicate(
    const std::unordered_map<expressions::ExprId, std::size_t> &num_distinct_values_map,
    const expressions::PredicatePtr &filter_predicate) {
  if (filter_predicate == nullptr) {
    return 1.0;
  }

  switch (filter_predicate->getExpressionType()) {
    case E::ExpressionType::kComparisonExpression: {
      // Case 1 - Number of distinct values statistics available
      //   Case 1.1 - Equality comparison: 1.0 / num_distinct_values
      //   Case 1.2 - Otherwise: 0.5
      // Case 2 - Number of distinct values statistics not available
      //   Case 2.1 - Equality comparison: 0.1
      //   Case 2.2 - Otherwise: 0.5
      const E::ComparisonExpressionPtr &comparison_expression =
          std::static_pointer_cast<const E::ComparisonExpression>(filter_predicate);
      E::AttributeReferencePtr attr;
      if ((E::SomeAttributeReference::MatchesWithConditionalCast(comparison_expression->left(), &attr) &&
           E::SomeScalarLiteral::Matches(comparison_expression->right())) ||
          (E::SomeAttributeReference::MatchesWithConditionalCast(comparison_expression->right(), &attr) &&
           E::SomeScalarLiteral::Matches(comparison_expression->left()))) {
        const auto it = num_distinct_values_map.find(attr->id());
        if (it != num_distinct_values_map.end() && it->second > 0) {
          double unit_selectivity = 1.0 / it->second;
          return comparison_expression->isEqualityComparisonPredicate()
                     ? unit_selectivity
                     : 0.1;
        }
      }

      return 0.1;
    }
    case E::ExpressionType::kLogicalAnd: {
      const E::LogicalAndPtr &logical_and =
          std::static_pointer_cast<const E::LogicalAnd>(filter_predicate);
      double selectivity = 1.0;
      for (const auto &predicate : logical_and->operands()) {
        selectivity =
            std::min(selectivity,
                     estimateSelectivityForPredicate(num_distinct_values_map, predicate));
      }
      return selectivity;
    }
    case E::ExpressionType::kLogicalOr: {
      const E::LogicalOrPtr &logical_or =
          std::static_pointer_cast<const E::LogicalOr>(filter_predicate);
      double selectivity = 0;
      for (const auto &predicate : logical_or->operands()) {
        selectivity += estimateSelectivityForPredicate(num_distinct_values_map, predicate);
      }
      return std::min(selectivity, 1.0);
    }
    default:
      break;
  }
  return 1.0;
}


void StarSchemaSimpleCostModel::getStatistics(
    const physical::PhysicalPtr &physical_plan,
    const expressions::AttributeReferencePtr &attribute,
    const CatalogRelationStatistics** stat,
    attribute_id* attr_id) {
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kTableReference: {
      const P::TableReferencePtr table_reference =
          std::static_pointer_cast<const P::TableReference>(physical_plan);
      const CatalogRelation *catalog_relation = table_reference->relation();
      const std::vector<E::AttributeReferencePtr> &attributes =
          table_reference->attribute_list();
      for (std::size_t i = 0; i < attributes.size(); ++i) {
        if (attributes[i]->id() == attribute->id()) {
          *stat = &catalog_relation->getStatistics();
          *attr_id = i;
        }
      }
    }
    default:
      break;
  }

  for (const auto &child : physical_plan->children()) {
    for (const auto &attr : child->getOutputAttributes()) {
      if (attr->id() == attribute->id()) {
        getStatistics(child, attribute, stat, attr_id);
      }
    }
  }
}


}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep
