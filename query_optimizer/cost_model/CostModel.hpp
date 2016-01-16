/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUERY_OPTIMIZER_COST_MODEL_COST_MODEL_HPP_
#define QUERY_OPTIMIZER_COST_MODEL_COST_MODEL_HPP_

#include <cstddef>

#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace cost {

/** \addtogroup CostModel
 *  @{
 */

/**
 * @brief Interface to a cost model of physical plans.
 */
class CostModel {
 public:
  /**
   * @brief Destructor.
   */
  virtual ~CostModel() {}

  /**
   * @brief Estimate the cardinality of the output relation of \p physical_plan.
   *
   * @param physical_plan The physical plan for which the cardinality of the
   *                      output relation is estimated.
   * @return The estimated cardinality.
   */
  virtual std::size_t estimateCardinality(
      const physical::PhysicalPtr &physical_plan) = 0;

 protected:
  /**
   * @brief Constructor.
   */
  CostModel() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(CostModel);
};

/** @} */

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUERY_OPTIMIZER_COST_MODEL_COST_MODEL_HPP_ */
