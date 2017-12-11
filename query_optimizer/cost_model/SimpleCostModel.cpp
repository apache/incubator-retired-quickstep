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

#include "query_optimizer/cost_model/SimpleCostModel.hpp"

#include <algorithm>
#include <memory>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationStatistics.hpp"
#include "query_optimizer/cost_model/CostModel.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/CrossReferenceCoalesceAggregate.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/FilterJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/SharedSubplanReference.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/TransitiveClosure.hpp"
#include "query_optimizer/physical/UnionAll.hpp"
#include "query_optimizer/physical/WindowAggregate.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace cost {

namespace P = ::quickstep::optimizer::physical;

std::size_t SimpleCostModel::estimateCardinality(
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
    case P::PhysicalType::kTransitiveClosure:
      return estimateCardinalityForTransitiveClosure(
          std::static_pointer_cast<const P::TransitiveClosure>(physical_plan));
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

std::size_t SimpleCostModel::estimateCardinalityForTopLevelPlan(
    const P::TopLevelPlanPtr &physical_plan) {
  return estimateCardinality(physical_plan->plan());
}

std::size_t SimpleCostModel::estimateCardinalityForTableReference(
    const P::TableReferencePtr &physical_plan) {
  const CatalogRelation *relation = physical_plan->relation();
  const CatalogRelationStatistics &stat = relation->getStatistics();
  const std::size_t num_tuples =
      stat.hasNumTuples() ? stat.getNumTuples()
                          : relation->estimateTupleCardinality();
  return num_tuples;
}

std::size_t SimpleCostModel::estimateCardinalityForSelection(
    const P::SelectionPtr &physical_plan) {
  return estimateCardinality(physical_plan->input());
}

std::size_t SimpleCostModel::estimateCardinalityForSort(
    const physical::SortPtr &physical_plan) {
  std::size_t cardinality = estimateCardinality(
      std::static_pointer_cast<const P::Sort>(physical_plan)->input());
  return std::min(cardinality, static_cast<std::size_t>(physical_plan->limit()));
}

std::size_t SimpleCostModel::estimateCardinalityForTableGenerator(
    const P::TableGeneratorPtr &physical_plan) {
  return physical_plan->generator_function_handle()->getEstimatedCardinality();
}

std::size_t SimpleCostModel::estimateCardinalityForFilterJoin(
    const P::FilterJoinPtr &physical_plan) {
  return estimateCardinality(physical_plan->left());
}

std::size_t SimpleCostModel::estimateCardinalityForHashJoin(
    const P::HashJoinPtr &physical_plan) {
  return std::max(estimateCardinality(physical_plan->left()),
                  estimateCardinality(physical_plan->right()));
}

std::size_t SimpleCostModel::estimateCardinalityForNestedLoopsJoin(
    const P::NestedLoopsJoinPtr &physical_plan) {
  return std::max(estimateCardinality(physical_plan->left()),
                  estimateCardinality(physical_plan->right()));
}

std::size_t SimpleCostModel::estimateCardinalityForAggregate(
    const physical::AggregatePtr &physical_plan) {
  if (physical_plan->grouping_expressions().empty()) {
    return 1;
  }
  return std::max(static_cast<std::size_t>(1),
                  estimateCardinality(physical_plan->input()) / 10);
}

std::size_t SimpleCostModel::estimateCardinalityForCrossReferenceCoalesceAggregate(
    const physical::CrossReferenceCoalesceAggregatePtr &physical_plan) {
  return estimateCardinality(physical_plan->left_child());
}

std::size_t SimpleCostModel::estimateCardinalityForWindowAggregate(
    const physical::WindowAggregatePtr &physical_plan) {
  return estimateCardinality(physical_plan->input());
}

std::size_t SimpleCostModel::estimateCardinalityForUnionAll(
    const physical::UnionAllPtr &physical_plan) {
  std::size_t cardinality = 0;
  for (const P::PhysicalPtr &operand : physical_plan->operands()) {
    cardinality += estimateCardinality(operand);
  }
  return cardinality;
}

std::size_t SimpleCostModel::estimateCardinalityForTransitiveClosure(
    const physical::TransitiveClosurePtr &physical_plan) {
  return std::max(estimateCardinality(physical_plan->start()),
                  estimateCardinality(physical_plan->edge()));
}

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep
