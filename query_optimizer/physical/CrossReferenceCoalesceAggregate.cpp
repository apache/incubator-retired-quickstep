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

#include "query_optimizer/physical/CrossReferenceCoalesceAggregate.hpp"

#include <string>
#include <unordered_set>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = ::quickstep::optimizer::expressions;

std::vector<E::AttributeReferencePtr> CrossReferenceCoalesceAggregate
    ::getOutputAttributes() const {
  std::vector<E::AttributeReferencePtr> output_attributes(left_join_attributes_);
  for (const auto &aggregate_expr : aggregate_expressions_) {
    output_attributes.emplace_back(E::ToRef(aggregate_expr));
  }
  return output_attributes;
}

std::vector<E::AttributeReferencePtr> CrossReferenceCoalesceAggregate
    ::getReferencedAttributes() const {
  std::unordered_set<E::AttributeReferencePtr> referenced_attributes;

  referenced_attributes.insert(left_join_attributes_.begin(),
                               left_join_attributes_.end());
  referenced_attributes.insert(right_join_attributes_.begin(),
                               right_join_attributes_.end());

  if (right_filter_predicate_ != nullptr) {
    const std::vector<E::AttributeReferencePtr> attrs_in_predicate =
        right_filter_predicate_->getReferencedAttributes();
    referenced_attributes.insert(attrs_in_predicate.begin(),
                                 attrs_in_predicate.end());
  }

  for (const auto &aggregate_expr : aggregate_expressions_) {
    const std::vector<E::AttributeReferencePtr> attrs_in_expr =
        aggregate_expr->getReferencedAttributes();
    referenced_attributes.insert(attrs_in_expr.begin(), attrs_in_expr.end());
  }

  return std::vector<E::AttributeReferencePtr>(
      referenced_attributes.begin(), referenced_attributes.end());
}

void CrossReferenceCoalesceAggregate::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("group_by_key_value_range");
  inline_field_values->push_back(std::to_string(group_by_key_value_range_));

  non_container_child_field_names->push_back("left_child");
  non_container_child_fields->push_back(left_child_);
  non_container_child_field_names->push_back("right_child");
  non_container_child_fields->push_back(right_child_);

  container_child_field_names->push_back("left_join_attributes");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(left_join_attributes_));
  container_child_field_names->push_back("right_join_attributes");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(right_join_attributes_));

  if (right_filter_predicate_ != nullptr) {
    non_container_child_field_names->push_back("right_filter_predicate");
    non_container_child_fields->push_back(right_filter_predicate_);
  }
  container_child_field_names->push_back("aggregate_expressions");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(aggregate_expressions_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
