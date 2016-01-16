/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/rules/UpdateExpression.hpp"

#include <unordered_map>

#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/rules/Rule.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;

E::ExpressionPtr UpdateExpression::applyToNode(const E::ExpressionPtr &input) {
  E::NamedExpressionPtr named_expression;

  if (E::SomeNamedExpression::MatchesWithConditionalCast(input, &named_expression)) {
    const std::unordered_map<E::ExprId, E::ExpressionPtr>::const_iterator it =
        substitution_map_.find(named_expression->id());
    if (it != substitution_map_.end()) {
      LOG_APPLYING_RULE(input, (it->second));
      return it->second;
    }
  }

  LOG_IGNORING_RULE(input);
  return input;
}

}  // namespace optimizer
}  // namespace quickstep
