/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/BinaryExpression.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/scalar/ScalarBinaryExpression.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"

#include "glog/logging.h"

namespace quickstep {

class Type;

namespace optimizer {
namespace expressions {

BinaryExpression::BinaryExpression(const BinaryOperation &operation,
                                   const ScalarPtr &left,
                                   const ScalarPtr &right)
    : operation_(operation), left_(left), right_(right) {
  DCHECK(operation_.canApplyToTypes(left_->getValueType(),
                                    right_->getValueType()))
      << toString();
  addChild(left_);
  addChild(right_);
}

std::string BinaryExpression::getName() const {
  switch (operation_.getBinaryOperationID()) {
    case BinaryOperationID::kAdd:
      return "Add";
    case BinaryOperationID::kSubtract:
      return "Subtract";
    case BinaryOperationID::kMultiply:
      return "Multiply";
    case BinaryOperationID::kDivide:
      return "Divide";
    default:
      LOG(FATAL) << "Unknown binary operation";
  }
}

const Type &BinaryExpression::getValueType() const {
  return *operation_.resultTypeForArgumentTypes(left_->getValueType(),
                                                right_->getValueType());
}

ExpressionPtr BinaryExpression::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  DCHECK(SomeScalar::Matches(new_children[0]));
  DCHECK(SomeScalar::Matches(new_children[1]));
  return BinaryExpression::Create(
      operation_,
      std::static_pointer_cast<const Scalar>(new_children[0]),
      std::static_pointer_cast<const Scalar>(new_children[1]));
}

std::vector<AttributeReferencePtr> BinaryExpression::getReferencedAttributes() const {
  std::vector<AttributeReferencePtr> referenced_attributes =
      left_->getReferencedAttributes();
  const std::vector<AttributeReferencePtr> referenced_attributes_by_right_operand =
      right_->getReferencedAttributes();
  referenced_attributes.insert(referenced_attributes.end(),
                               referenced_attributes_by_right_operand.begin(),
                               referenced_attributes_by_right_operand.end());
  return referenced_attributes;
}

::quickstep::Scalar *BinaryExpression::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  return new ::quickstep::ScalarBinaryExpression(
      operation_,
      left_->concretize(substitution_map),
      right_->concretize(substitution_map));
}

void BinaryExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  container_child_field_names->push_back("");
  container_child_fields->push_back({left_, right_});
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
