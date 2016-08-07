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

#include "query_optimizer/logical/Aggregate.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

LogicalPtr Aggregate::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  return Create(new_children[0], grouping_expressions_, aggregate_expressions_);
}

std::vector<E::AttributeReferencePtr> Aggregate::getOutputAttributes() const {
  std::vector<E::AttributeReferencePtr> output_attributes(
      E::ToRefVector(grouping_expressions_));
  const std::vector<E::AttributeReferencePtr> aggregate_output_attributes =
      E::ToRefVector(aggregate_expressions_);
  output_attributes.insert(output_attributes.end(),
                           aggregate_output_attributes.begin(),
                           aggregate_output_attributes.end());
  return output_attributes;
}

std::vector<E::AttributeReferencePtr> Aggregate::getReferencedAttributes() const {
  std::vector<E::AttributeReferencePtr> referenced_attributes;
  for (const E::NamedExpressionPtr &grouping_expression :
       grouping_expressions_) {
    const std::vector<E::AttributeReferencePtr> referenced_attributes_in_expression =
        grouping_expression->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_expression.begin(),
                                 referenced_attributes_in_expression.end());
  }
  for (const E::AliasPtr &aggregate_expression :
       aggregate_expressions_) {
    const std::vector<E::AttributeReferencePtr> referenced_attributes_in_expression =
        aggregate_expression->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_expression.begin(),
                                 referenced_attributes_in_expression.end());
  }
  return referenced_attributes;
}

LogicalPtr Aggregate::copyWithNewInputExpressions(
    const std::vector<E::ExpressionPtr> &input_expressions) const {
  DCHECK_EQ(grouping_expressions_.size() + aggregate_expressions_.size(),
            input_expressions.size());

  std::vector<E::NamedExpressionPtr> new_grouping_expressions;
  for (std::vector<E::ExpressionPtr>::size_type index = 0;
       index < grouping_expressions_.size();
       ++index) {
    E::NamedExpressionPtr grouping_expression;
    E::SomeNamedExpression::MatchesWithConditionalCast(input_expressions[index],
                                                       &grouping_expression);
    DCHECK(grouping_expression != nullptr);
    new_grouping_expressions.emplace_back(grouping_expression);
  }

  std::vector<E::AliasPtr> new_aggregate_expressions;
  for (std::vector<E::ExpressionPtr>::size_type index = grouping_expressions_.size();
       index < input_expressions.size();
       ++index) {
    E::AliasPtr aggregate_expression;
    E::SomeAlias::MatchesWithConditionalCast(input_expressions[index], &aggregate_expression);
    DCHECK(aggregate_expression != nullptr);
    new_aggregate_expressions.emplace_back(aggregate_expression);
  }

  return Create(input_, new_grouping_expressions, new_aggregate_expressions);
}

void Aggregate::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  container_child_field_names->push_back("grouping_expressions");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(grouping_expressions_));
  container_child_field_names->push_back("aggregate_expressions");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(aggregate_expressions_));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
