/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/UpdateTable.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

std::vector<expressions::AttributeReferencePtr> UpdateTable::getReferencedAttributes() const {
  std::vector<expressions::AttributeReferencePtr> referenced_attributes = assignees_;
  for (const expressions::ScalarPtr& assignment_expression : assignment_expressions_) {
    std::vector<expressions::AttributeReferencePtr> attrs_referenced_in_expression =
        assignment_expression->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 attrs_referenced_in_expression.begin(),
                                 attrs_referenced_in_expression.end());
  }
  if (predicate_ != nullptr) {
    std::vector<expressions::AttributeReferencePtr> attrs_referenced_in_predicate =
        predicate_->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 attrs_referenced_in_predicate.begin(),
                                 attrs_referenced_in_predicate.end());
  }
  return referenced_attributes;
}

void UpdateTable::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);
  if (predicate_ != nullptr) {
    non_container_child_field_names->push_back("predicate");
    non_container_child_fields->push_back(predicate_);
  }

  container_child_field_names->push_back("attributes");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(assignees_));
  container_child_field_names->push_back("assigned_values");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(assignment_expressions_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
