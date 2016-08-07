/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
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
  return comparison_.getName();
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
