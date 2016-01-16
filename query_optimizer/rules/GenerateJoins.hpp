/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_GENERATE_JOINS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_GENERATE_JOINS_HPP_

#include <map>
#include <string>
#include <vector>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/rules/TopDownRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that converts a MultiwayCartesianJoin to a deterministic chain of
 *        join operators. It does not do cost-based join ordering
 *        optimization TODO(quickstep-team). Hash joins are performed before
 *        nested loops join. The join ordering among hash joins and among nested
 *        loops joins follow the ordering of the predicates given by the query.
 */
class GenerateJoins : public TopDownRule<logical::Logical> {
 public:
  /**
   * @brief Constructor.
   */
  GenerateJoins() {}

  std::string getName() const override { return "GenerateJoins"; }

 protected:
  logical::LogicalPtr applyToNode(const logical::LogicalPtr &input) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(GenerateJoins);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_GENERATE_JOINS_HPP_ */
