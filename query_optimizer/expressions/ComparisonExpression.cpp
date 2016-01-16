/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/ComparisonExpression.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/predicate/ComparisonPredicate.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

ComparisonExpression::ComparisonExpression(const Comparison &comparison,
                                           const ScalarPtr &left,
                                           const ScalarPtr &right)
    : comparison_(comparison), left_(left), right_(right) {
  DCHECK(comparison_.canCompareTypes(left->getValueType(),
                                     right->getValueType()));
  addChild(left_);
  addChild(right_);
}

std::string ComparisonExpression::getName() const {
  switch (comparison_.getComparisonID()) {
    case ComparisonID::kEqual:
      return "Equal";
    case ComparisonID::kNotEqual:
      return "NotEqual";
    case ComparisonID::kLess:
      return "Less";
    case ComparisonID::kLessOrEqual:
      return "LessOrEqual";
    case ComparisonID::kGreater:
      return "Greater";
    case ComparisonID::kGreaterOrEqual:
      return "GreaterOrEqual";
    default:
      LOG(FATAL) << "Unknown comparison type";
  }
}

bool ComparisonExpression::isEqualityComparisonPredicate() const {
  return comparison_.getComparisonID() == ComparisonID::kEqual;
}

ExpressionPtr ComparisonExpression::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  ScalarPtr left_operand;
  ScalarPtr right_operand;
  CHECK(SomeScalar::MatchesWithConditionalCast(new_children[0], &left_operand));
  CHECK(SomeScalar::MatchesWithConditionalCast(new_children[1], &right_operand));
  return ComparisonExpression::Create(comparison_, left_operand, right_operand);
}

std::vector<AttributeReferencePtr>
ComparisonExpression::getReferencedAttributes() const {
  std::vector<AttributeReferencePtr> referenced_attributes =
      left_->getReferencedAttributes();
  const std::vector<AttributeReferencePtr> referenced_attributes_by_right_operand =
      right_->getReferencedAttributes();
  referenced_attributes.insert(referenced_attributes.end(),
                               referenced_attributes_by_right_operand.begin(),
                               referenced_attributes_by_right_operand.end());
  return referenced_attributes;
}

::quickstep::Predicate* ComparisonExpression::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  return new ::quickstep::ComparisonPredicate(
      comparison_,
      left_->concretize(substitution_map),
      right_->concretize(substitution_map));
}

void ComparisonExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("");
  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(left_);
  non_container_child_fields->push_back(right_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
