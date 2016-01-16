/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/Project.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

LogicalPtr Project::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
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
