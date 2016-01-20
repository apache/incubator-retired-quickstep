/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_UPDATE_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_UPDATE_EXPRESSION_HPP_

#include <string>
#include <unordered_map>

#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Performs a bottom-up traverse on an expression tree and replaces named
 *        expressions with new expressions based on a substitution map.
 */
class UpdateExpression : public BottomUpRule<expressions::Expression> {
 public:
  /**
   * @brief Constructor.
   *
   * @param substitution_map Map from an ExprId of the named expression to
   *                         replace with an expression to be replaced with.
   */
  explicit UpdateExpression(
      const std::unordered_map<expressions::ExprId, expressions::ExpressionPtr> &substitution_map)
      : substitution_map_(substitution_map) {}

  std::string getName() const override { return "UpdateExpressions"; }

 protected:
  expressions::ExpressionPtr applyToNode(const expressions::ExpressionPtr &input) override;

 private:
  const std::unordered_map<expressions::ExprId, expressions::ExpressionPtr> &substitution_map_;

  DISALLOW_COPY_AND_ASSIGN(UpdateExpression);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_UPDATE_EXPRESSION_HPP_ */
