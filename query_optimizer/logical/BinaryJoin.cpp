/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/BinaryJoin.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

std::vector<E::AttributeReferencePtr> BinaryJoin::getOutputAttributes() const {
  std::vector<E::AttributeReferencePtr> output_attributes =
      left_->getOutputAttributes();
  const std::vector<E::AttributeReferencePtr> output_attributes_by_right_operand =
      right_->getOutputAttributes();
  output_attributes.insert(output_attributes.end(),
                           output_attributes_by_right_operand.begin(),
                           output_attributes_by_right_operand.end());
  return output_attributes;
}

void BinaryJoin::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("left");
  non_container_child_field_names->push_back("right");

  non_container_child_fields->push_back(left_);
  non_container_child_fields->push_back(right_);
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
