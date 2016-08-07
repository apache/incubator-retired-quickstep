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

#include "query_optimizer/expressions/SimpleCase.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/ScalarCaseExpression.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/Type.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

// We treat a SimpleCase as constant only if all operands are constant.
bool SimpleCase::isConstant() const {
  if (!case_operand_->isConstant()) {
    return false;
  }
  for (const ScalarPtr &condition_operand : condition_operands_) {
    if (!condition_operand->isConstant()) {
      return false;
    }
  }
  for (const ScalarPtr &conditional_result_expression : conditional_result_expressions_) {
    if (!conditional_result_expression->isConstant()) {
      return false;
    }
  }
  return (else_result_expression_ == nullptr || else_result_expression_->isConstant());
}

std::vector<AttributeReferencePtr> SimpleCase::getReferencedAttributes() const {
  std::vector<AttributeReferencePtr> referenced_attributes;

  const std::vector<AttributeReferencePtr> referenced_attributes_in_case_operand =
      case_operand_->getReferencedAttributes();
  referenced_attributes.insert(referenced_attributes.end(),
                               referenced_attributes_in_case_operand.begin(),
                               referenced_attributes_in_case_operand.end());

  for (const ScalarPtr &condition_operand : condition_operands_) {
    const std::vector<AttributeReferencePtr> referenced_attributes_in_condition_operand =
        condition_operand->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_condition_operand.begin(),
                                 referenced_attributes_in_condition_operand.end());
  }
  for (const ScalarPtr &conditional_result_expression : conditional_result_expressions_) {
    const std::vector<AttributeReferencePtr> referenced_attributes_in_conditional_result_expression =
        conditional_result_expression->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_conditional_result_expression.begin(),
                                 referenced_attributes_in_conditional_result_expression.end());
  }
  if (else_result_expression_ != nullptr) {
    const std::vector<AttributeReferencePtr> referenced_attributes_in_else_result_expression =
        else_result_expression_->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_else_result_expression.begin(),
                                 referenced_attributes_in_else_result_expression.end());
  }

  return referenced_attributes;
}

ExpressionPtr SimpleCase::copyWithNewChildren(const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());

  ScalarPtr new_case_operand;
  std::vector<ScalarPtr> new_condition_operands;
  std::vector<ScalarPtr> new_conditional_result_expressions;
  ScalarPtr new_else_result_expression;

  new_condition_operands.reserve(condition_operands_.size());
  new_conditional_result_expressions.reserve(condition_operands_.size());

  new_case_operand = std::static_pointer_cast<const Scalar>(new_children[0]);
  std::size_t num_when_clauses = condition_operands_.size();
  for (std::size_t index = 1; index <= num_when_clauses; ++index) {
    new_condition_operands.push_back(
        std::static_pointer_cast<const Scalar>(new_children[index]));

    new_conditional_result_expressions.push_back(
        std::static_pointer_cast<const Scalar>(new_children[index + num_when_clauses]));
  }
  if (else_result_expression_ != nullptr) {
    new_else_result_expression =
        std::static_pointer_cast<const Scalar>(new_children.back());
  }

  return Create(new_case_operand,
                new_condition_operands,
                new_conditional_result_expressions,
                new_else_result_expression,
                value_type_);
}

::quickstep::Scalar* SimpleCase::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  std::vector<std::unique_ptr<quickstep::Predicate>> when_predicates;
  for (const ScalarPtr &condition_operand : condition_operands_) {
    const PredicatePtr predicate =
        ComparisonExpression::Create(
            quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual),
            case_operand_,
            condition_operand);
    when_predicates.emplace_back(predicate->concretize(substitution_map));
  }

  std::vector<std::unique_ptr<quickstep::Scalar>> result_expressions;
  for (const ScalarPtr &expression : conditional_result_expressions_) {
    result_expressions.emplace_back(expression->concretize(substitution_map));
  }

  std::unique_ptr<quickstep::Scalar> else_result_expression;
  if (else_result_expression_ == nullptr) {
    else_result_expression.reset(
        new quickstep::ScalarLiteral(value_type_.makeNullValue(), value_type_));
  } else {
    else_result_expression.reset(
        else_result_expression_->concretize(substitution_map));
  }

  return new quickstep::ScalarCaseExpression(
      value_type_,
      std::move(when_predicates),
      std::move(result_expressions),
      else_result_expression.release());
}

void SimpleCase::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("case_operand");
  non_container_child_fields->push_back(case_operand_);

  container_child_field_names->push_back("condition_operands");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(condition_operands_));

  container_child_field_names->push_back("result_expressions");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(conditional_result_expressions_));

  if (else_result_expression_ != nullptr) {
    non_container_child_field_names->push_back("else_result_expression");
    non_container_child_fields->push_back(else_result_expression_);
  }
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
