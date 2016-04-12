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

#include "query_optimizer/logical/Project.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

LogicalPtr Project::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(children().size(), new_children.size());
  return Project::Create(new_children[0], project_expressions_);
}

std::vector<E::AttributeReferencePtr> Project::getOutputAttributes() const {
  return ToRefVector(project_expressions_);
}

std::vector<E::AttributeReferencePtr> Project::getReferencedAttributes() const {
  std::vector<E::AttributeReferencePtr> referenced_attributes;
  for (const E::NamedExpressionPtr &project_expression : project_expressions_) {
    const std::vector<E::AttributeReferencePtr> referenced_attributes_in_expression =
        project_expression->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_expression.begin(),
                                 referenced_attributes_in_expression.end());
  }
  return referenced_attributes;
}

LogicalPtr Project::copyWithNewInputExpressions(
    const std::vector<E::ExpressionPtr> &input_expressions) const {
  DCHECK_EQ(project_expressions_.size(), input_expressions.size());
  std::vector<E::NamedExpressionPtr> new_project_expressions;
  for (const E::ExpressionPtr &input_expression : input_expressions) {
    E::NamedExpressionPtr project_expression;
    E::SomeNamedExpression::MatchesWithConditionalCast(input_expression, &project_expression);
    DCHECK(project_expression != nullptr);
    new_project_expressions.emplace_back(project_expression);
  }
  return Create(input_, new_project_expressions);
}

void Project::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  container_child_field_names->push_back("project_list");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(project_expressions_));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
