/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUERY_OPTIMIZER_COST_MODEL_SIMPLE_COST_MODEL_HPP_
#define QUERY_OPTIMIZER_COST_MODEL_SIMPLE_COST_MODEL_HPP_

#include <cstddef>

#include "query_optimizer/cost_model/CostModel.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace cost {

/** \addtogroup CostModel
 *  @{
 */

/**
 * @brief A simple cost model.
 */
class SimpleCostModel : public CostModel {
 public:
  /**
   * @brief Constructor.
   */
  SimpleCostModel() {}

  std::size_t estimateCardinality(
      const physical::PhysicalPtr &physical_plan) override;

 private:
  // Returns the estimated cardinality of the input plan.
  std::size_t estimateCardinalityForTopLevelPlan(
      const physical::TopLevelPlanPtr &physical_plan);

  // Returns the estimated cardinality of the relation.
  std::size_t estimateCardinalityForTableReference(
      const physical::TableReferencePtr &physical_plan);

  // Returns the estimated cardinality of the input plan.
  std::size_t estimateCardinalityForSelection(
      const physical::SelectionPtr &physical_plan);

  // Returns the larger value of the estimated cardinalities of two
  // input plans.
  std::size_t estimateCardinalityForHashJoin(
      const physical::HashJoinPtr &physical_plan);

  // Returns the larger value of the estimated cardinalities of two
  // input plans.
  std::size_t estimateCardinalityForNestedLoopsJoin(
      const physical::NestedLoopsJoinPtr &physical_plan);

  // Returns 1 for a scalar aggregation; otherwise, returns the estimated
  // cardinality of the input plan divided by 10.
  std::size_t estimateCardinalityForAggregate(
      const physical::AggregatePtr &physical_plan);

  DISALLOW_COPY_AND_ASSIGN(SimpleCostModel);
};

/** @} */

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUERY_OPTIMIZER_COST_MODEL_SIMPLE_COST_MODEL_HPP_ */
