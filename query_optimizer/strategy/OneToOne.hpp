/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_ONE_TO_ONE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_ONE_TO_ONE_HPP_

#include <string>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/strategy/Strategy.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class LogicalToPhysicalMapper;

namespace strategy {

/** \addtogroup OptimizerStrategy
 *  @{
 */

/**
 * @brief Simple strategy that converts one logical node to one physical node.
 */
class OneToOne : public Strategy {
 public:
  /**
   * @brief Constructor.
   *
   * @param physical_mapper The physical plan generator used to get child
   *        physical plans.
   */
  explicit OneToOne(LogicalToPhysicalMapper *physical_mapper)
      : Strategy(physical_mapper) {}

  std::string getName() const override { return "OneToOne"; }

  bool generatePlan(const logical::LogicalPtr &logical_input,
                    physical::PhysicalPtr *physical_output) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(OneToOne);
};

/** @} */

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_ONE_TO_ONE_HPP_ */
