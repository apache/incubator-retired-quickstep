/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_SELECTION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_SELECTION_HPP_

#include <string>
#include <vector>

#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
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
 * @brief Strategy to create a Selection plan.
 *        It converts logical project, filter or project-filter to
 *        a physical Selection.
 */
class Selection : public Strategy {
 public:
  /**
   * @brief Constructor.
   *
   * @param physical_mapper The physical plan generator used to get child
   *        physical plans.
   */
  explicit Selection(LogicalToPhysicalMapper *physical_mapper)
      : Strategy(physical_mapper) {}

  std::string getName() const override { return "Selection"; }

  bool generatePlan(const logical::LogicalPtr &logical_input,
                    physical::PhysicalPtr *physical_output) override;

 private:
  /**
   * @brief Returns in \p physical_output a physical plan rooted by a Selection.
   *
   * @param logical_input The logical input to the Selection.
   * @param filter_predicate The filter predicate that should be included
   *                         in the physical Selection. The predicate in
   *                         the returned Selection may have additional
   *                         predicates in the logical input.
   * @param project_expressions The project expressions for the physical
   *                            Selection. The expressions in the
   *                            returned Selection may change.
   * @param physical_output A physical plan with a Selection as the root.
   */
  void addSelection(
      const logical::LogicalPtr &logical_input,
      const expressions::PredicatePtr &filter_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      physical::PhysicalPtr *physical_output);

  DISALLOW_COPY_AND_ASSIGN(Selection);
};

/** @} */

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_SELECTION_HPP_ */
