/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/NestedLoopsJoin.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

std::vector<expressions::AttributeReferencePtr> NestedLoopsJoin::getReferencedAttributes() const {
  std::vector<expressions::AttributeReferencePtr> referenced_attributes;
  for (const expressions::NamedExpressionPtr &project_expression :
       project_expressions()) {
    const std::vector<expressions::AttributeReferencePtr> referenced_attributes_in_expression =
        project_expression->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_expression.begin(),
                                 referenced_attributes_in_expression.end());
  }
  const std::vector<expressions::AttributeReferencePtr> referenced_attributes_in_predicate =
      join_predicate_->getReferencedAttributes();
  referenced_attributes.insert(referenced_attributes.end(),
                               referenced_attributes_in_predicate.begin(),
                               referenced_attributes_in_predicate.end());
  return referenced_attributes;
}

bool NestedLoopsJoin::maybeCopyWithPrunedExpressions(
    const expressions::UnorderedNamedExpressionSet &referenced_expressions,
    PhysicalPtr *output) const {
  std::vector<expressions::NamedExpressionPtr> new_project_expressions;
  const std::vector<expressions::NamedExpressionPtr> &current_project_expressions =
      project_expressions();
  for (const expressions::NamedExpressionPtr &project_expression :
       current_project_expressions) {
    if (referenced_expressions.find(project_expression) !=
        referenced_expressions.end()) {
      new_project_expressions.emplace_back(project_expression);
    }
  }
  if (new_project_expressions.size() != current_project_expressions.size()) {
    *output = Create(left(), right(), join_predicate_, new_project_expressions);
    return true;
  }
  return false;
}

void NestedLoopsJoin::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  BinaryJoin::getFieldStringItems(inline_field_names,
                                  inline_field_values,
                                  non_container_child_field_names,
                                  non_container_child_fields,
                                  container_child_field_names,
                                  container_child_fields);
  non_container_child_field_names->push_back("join_predicate");
  non_container_child_fields->push_back(join_predicate_);
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
