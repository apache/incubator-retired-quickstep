/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "query_optimizer/expressions/Alias.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"

#include "glog/logging.h"

namespace quickstep {

class Scalar;

namespace optimizer {
namespace expressions {

Alias::Alias(attribute_id id,
             const ExpressionPtr &expression,
             const std::string &attribute_name,
             const std::string &attribute_alias,
             const std::string &table_name)
    : NamedExpression(id, attribute_name, attribute_alias, table_name) {
  AliasPtr attribute_child;
  // Do not add an Alias as the child of another Alias.
  if (SomeAlias::MatchesWithConditionalCast(expression, &attribute_child)) {
    expression_ = attribute_child->expression();
  } else {
    expression_ = expression;
  }
  addChild(expression_);
}

ExpressionPtr Alias::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return Alias::Create(id(),
                       new_children[0],
                       attribute_name(),
                       attribute_alias(),
                       relation_name());
}

::quickstep::Scalar *Alias::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  // Alias should be converted to a CatalogAttribute.
  LOG(FATAL) << "Cannot concretize Alias to a scalar for evaluation";
}

void Alias::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields)
    const {
  NamedExpression::getFieldStringItems(inline_field_names,
                                       inline_field_values,
                                       non_container_child_field_names,
                                       non_container_child_fields,
                                       container_child_field_names,
                                       container_child_fields);

  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(expression_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
