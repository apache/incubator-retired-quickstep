/**
 * This file copyright (c) 2015, Pivotal Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/WindowAggregate.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

LogicalPtr WindowAggregate::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  return Create(new_children[0],
                window_attribute_,
                window_duration_,
                emit_duration_,
                age_duration_,
                grouping_expressions_,
                aggregate_expressions_);
}

std::vector<E::AttributeReferencePtr> WindowAggregate::getOutputAttributes() const {
  std::vector<E::AttributeReferencePtr> output_attributes;
  output_attributes.push_back(window_attribute_);
  const std::vector<E::AttributeReferencePtr> grouping_output_attributes(
      E::ToRefVector(grouping_expressions_));
  output_attributes.insert(output_attributes.end(),
                           grouping_output_attributes.begin(),
                           grouping_output_attributes.end());
  const std::vector<E::AttributeReferencePtr> aggregate_output_attributes =
      E::ToRefVector(aggregate_expressions_);
  output_attributes.insert(output_attributes.end(),
                           aggregate_output_attributes.begin(),
                           aggregate_output_attributes.end());
  return output_attributes;
}

std::vector<E::AttributeReferencePtr> WindowAggregate::getReferencedAttributes() const {
  std::vector<E::AttributeReferencePtr> referenced_attributes;
  referenced_attributes.push_back(window_attribute_);
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

void WindowAggregate::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  non_container_child_field_names->push_back("window_attribute");
  non_container_child_fields->push_back(window_attribute_);

  non_container_child_field_names->push_back("window_duration");
  non_container_child_fields->push_back(window_duration_);

  non_container_child_field_names->push_back("emit_duration");
  non_container_child_fields->push_back(emit_duration_);

  non_container_child_field_names->push_back("age_duration");
  non_container_child_fields->push_back(age_duration_);

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
