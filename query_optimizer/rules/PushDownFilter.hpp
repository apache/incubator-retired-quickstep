/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_FILTER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_FILTER_HPP_

#include <string>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/rules/TopDownRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that pushes down filter predicates.
 */
class PushDownFilter : public TopDownRule<logical::Logical> {
 public:
  /**
   * @brief Constructor.
   */
  PushDownFilter() {}

  std::string getName() const override { return "PushDownFilter"; }

 protected:
  logical::LogicalPtr applyToNode(const logical::LogicalPtr &input) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PushDownFilter);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_FILTER_HPP_ */
