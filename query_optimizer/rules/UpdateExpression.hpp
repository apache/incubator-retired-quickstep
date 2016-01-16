/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
