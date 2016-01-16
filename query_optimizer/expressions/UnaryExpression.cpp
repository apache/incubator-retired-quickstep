/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/UnaryExpression.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/scalar/ScalarUnaryExpression.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

std::string UnaryExpression::getName() const {
  DCHECK(operation_.getUnaryOperationID() == UnaryOperationID::kNegate);
  return "Negate";
}

ExpressionPtr UnaryExpression::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  DCHECK(SomeScalar::Matches(new_children[0]));
  return UnaryExpression::Create(
      operation_, std::static_pointer_cast<const Scalar>(new_children[0]));
}

::quickstep::Scalar* UnaryExpression::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  return new ::quickstep::ScalarUnaryExpression(
      operation_, operand_->concretize(substitution_map));
}

void UnaryExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("Operand");
  non_container_child_fields->push_back(operand_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
