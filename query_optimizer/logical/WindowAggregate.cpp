/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_optimizer/logical/WindowAggregate.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Alias.hpp"
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

LogicalPtr WindowAggregate::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  return Create(new_children[0], window_aggregate_expression_);
}

std::vector<E::AttributeReferencePtr> WindowAggregate::getOutputAttributes() const {
  std::vector<E::AttributeReferencePtr> output_attributes(input_->getOutputAttributes());
  output_attributes.push_back(E::ToRef(window_aggregate_expression_));
  return output_attributes;
}

std::vector<E::AttributeReferencePtr> WindowAggregate::getReferencedAttributes() const {
  return window_aggregate_expression_->getReferencedAttributes();
}

LogicalPtr WindowAggregate::copyWithNewInputExpressions(
    const std::vector<E::ExpressionPtr> &input_expressions) const {
  // Only one expression needed
  DCHECK_EQ(1u, input_expressions.size());

  E::AliasPtr window_aggregate_expression;
  E::SomeAlias::MatchesWithConditionalCast(input_expressions[0],
                                           &window_aggregate_expression);

  return Create(input_, window_aggregate_expression);
}

void WindowAggregate::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  non_container_child_field_names->push_back("window_aggregate_expression");
  non_container_child_fields->push_back(window_aggregate_expression_);
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
