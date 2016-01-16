/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_COLLAPSE_PROJECT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_COLLAPSE_PROJECT_HPP_

#include <string>
#include <vector>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Merges nested Projects into one single Project.
 */
class CollapseProject : public BottomUpRule<logical::Logical> {
 public:
  CollapseProject() {}

  std::string getName() const override { return "CollapseProject"; }

 protected:
  logical::LogicalPtr applyToNode(const logical::LogicalPtr &input) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(CollapseProject);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_COLLAPSE_PROJECT_HPP_ */
