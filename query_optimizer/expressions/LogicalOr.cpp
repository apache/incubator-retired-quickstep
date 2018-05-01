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

#include "query_optimizer/expressions/LogicalOr.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "expressions/predicate/DisjunctionPredicate.hpp"
#include "expressions/predicate/TrivialPredicates.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/PredicateLiteral.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

LogicalOr::LogicalOr(const std::vector<PredicatePtr> &operands) {
  // Flatten the predicate tree.
  operands_.reserve(operands.size());
  for (const PredicatePtr &operand : operands) {
    LogicalOrPtr logical_or;
    PredicateLiteralPtr literal;
    if (SomeLogicalOr::MatchesWithConditionalCast(operand, &logical_or)) {
      operands_.insert(operands_.end(),
                       logical_or->operands().begin(),
                       logical_or->operands().end());
    } else if (!SomePredicateLiteral::MatchesWithConditionalCast(operand, &literal) ||
               !literal->is_true()) {
      operands_.push_back(operand);
    }
  }

  for (const PredicatePtr &operand : operands_) {
    addChild(operand);
  }
}

ExpressionPtr LogicalOr::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  std::vector<PredicatePtr> operands;
  for (const ExpressionPtr &expression : new_children) {
    PredicatePtr new_expression;
    CHECK(SomePredicate::MatchesWithConditionalCast(expression, &new_expression));
    operands.push_back(new_expression);
  }
  return LogicalOr::Create(operands);
}

std::vector<AttributeReferencePtr> LogicalOr::getReferencedAttributes() const {
  std::vector<AttributeReferencePtr> referenced_attributes;
  for (const PredicatePtr &operand : operands()) {
    const std::vector<AttributeReferencePtr> referenced_attributes_in_operand =
        operand->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_operand.begin(),
                                 referenced_attributes_in_operand.end());
  }
  return referenced_attributes;
}

::quickstep::Predicate* LogicalOr::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
    const std::unordered_set<ExprId> &left_expr_ids,
    const std::unordered_set<ExprId> &right_expr_ids) const {
  if (operands_.empty()) {
    return new FalsePredicate();
  }

  if (operands_.size() == 1u) {
    return operands_[0]->concretize(substitution_map, left_expr_ids, right_expr_ids);
  }

  std::unique_ptr<::quickstep::DisjunctionPredicate> concretized_predicate;
  concretized_predicate.reset(new ::quickstep::DisjunctionPredicate());

  for (const PredicatePtr &operand : operands_) {
    concretized_predicate->addPredicate(
        operand->concretize(substitution_map, left_expr_ids, right_expr_ids));
  }

  return concretized_predicate.release();
}

void LogicalOr::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  if (!operands_.empty()) {
    container_child_field_names->push_back("");
    container_child_fields->emplace_back(CastSharedPtrVector<OptimizerTreeBase>(operands_));
  } else {
    inline_field_names->push_back("static_value");
    inline_field_values->push_back("false");
  }
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
