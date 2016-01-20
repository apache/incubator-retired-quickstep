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
