/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/ExpressionUtil.hpp"

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"

namespace quickstep {
namespace optimizer {
namespace expressions {

AttributeReferencePtr ToRef(const NamedExpressionPtr &expression) {
  return AttributeReference::Create(expression->id(),
                                    expression->attribute_name(),
                                    expression->attribute_alias(),
                                    expression->relation_name(),
                                    expression->getValueType());
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
