/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/HashJoin.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

std::vector<expressions::AttributeReferencePtr> HashJoin::getReferencedAttributes() const {
  std::vector<expressions::AttributeReferencePtr> referenced_attributes;
  for (const expressions::NamedExpressionPtr &project_expression :
       project_expressions()) {
    const std::vector<expressions::AttributeReferencePtr> referenced_attributes_in_expression =
        project_expression->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_expression.begin(),
                                 referenced_attributes_in_expression.end());
  }
  referenced_attributes.insert(referenced_attributes.end(),
                               left_join_attributes_.begin(),
                               left_join_attributes_.end());
  referenced_attributes.insert(referenced_attributes.end(),
                               right_join_attributes_.begin(),
                               right_join_attributes_.end());
  if (residual_predicate_ != nullptr) {
    const std::vector<expressions::AttributeReferencePtr> referenced_attributes_in_residual =
        residual_predicate_->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_residual.begin(),
                                 referenced_attributes_in_residual.end());
  }
  return referenced_attributes;
}

bool HashJoin::maybeCopyWithPrunedExpressions(
    const expressions::UnorderedNamedExpressionSet &referenced_expressions,
    PhysicalPtr *output) const {
  std::vector<expressions::NamedExpressionPtr> new_project_expressions;
  const std::vector<expressions::NamedExpressionPtr> &
      current_project_expressions = project_expressions();
  for (const expressions::NamedExpressionPtr &project_expression :
       current_project_expressions) {
    if (referenced_expressions.find(project_expression) !=
        referenced_expressions.end()) {
      new_project_expressions.emplace_back(project_expression);
    }
  }
  if (new_project_expressions.size() != current_project_expressions.size()) {
    *output = Create(left(),
                     right(),
                     left_join_attributes_,
                     right_join_attributes_,
                     residual_predicate_,
                     new_project_expressions);
    return true;
  }
  return false;
}

void HashJoin::getFieldStringItems(
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
  if (residual_predicate_ != nullptr) {
    non_container_child_field_names->push_back("residual_predicate");
    non_container_child_fields->push_back(residual_predicate_);
  }
  container_child_field_names->push_back("left_join_attributes");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(left_join_attributes_));
  container_child_field_names->push_back("right_join_attributes");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(right_join_attributes_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
