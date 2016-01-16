/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_PRUNE_COLUMNS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_PRUNE_COLUMNS_HPP_

#include <string>

#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/TopDownRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to remove unused project
 *        expressions.
 */
class PruneColumns
    : public TopDownRule<physical::Physical> {
 public:
  PruneColumns() {}

  std::string getName() const override { return "PruneColumns"; }

 protected:
  physical::PhysicalPtr applyToNode(const physical::PhysicalPtr &input) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PruneColumns);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_PRUNE_COLUMNS_HPP_ */
