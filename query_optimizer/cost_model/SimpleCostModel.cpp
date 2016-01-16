/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/cost_model/SimpleCostModel.hpp"

#include <algorithm>
#include <memory>

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"

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
    case P::PhysicalType::kHashJoin:
      return estimateCardinalityForHashJoin(
          std::static_pointer_cast<const P::HashJoin>(physical_plan));
    case P::PhysicalType::kNestedLoopsJoin:
      return estimateCardinalityForNestedLoopsJoin(
          std::static_pointer_cast<const P::NestedLoopsJoin>(physical_plan));
    case P::PhysicalType::kAggregate:
      return estimateCardinalityForAggregate(
          std::static_pointer_cast<const P::Aggregate>(physical_plan));
    default:
      LOG(FATAL) << "Unsupported physical plan:" << physical_plan->toString();
  }
}

std::size_t SimpleCostModel::estimateCardinalityForTopLevelPlan(
    const P::TopLevelPlanPtr &physical_plan) {
  return estimateCardinality(physical_plan->plan());
}

std::size_t SimpleCostModel::estimateCardinalityForTableReference(
    const P::TableReferencePtr &physical_plan) {
  return physical_plan->relation()->estimateTupleCardinality();
}

std::size_t SimpleCostModel::estimateCardinalityForSelection(
    const P::SelectionPtr &physical_plan) {
  return estimateCardinality(physical_plan->input());
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

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep
