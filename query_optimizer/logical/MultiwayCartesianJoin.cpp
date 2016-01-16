/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/MultiwayCartesianJoin.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

std::vector<E::AttributeReferencePtr> MultiwayCartesianJoin::getOutputAttributes() const {
  std::vector<E::AttributeReferencePtr> output_attributes;
  for (const LogicalPtr &operand : operands()) {
    const std::vector<E::AttributeReferencePtr> output_attributes_in_operand =
        operand->getOutputAttributes();
    output_attributes.insert(output_attributes.end(),
                             output_attributes_in_operand.begin(),
                             output_attributes_in_operand.end());
  }
  return output_attributes;
}

LogicalPtr MultiwayCartesianJoin::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return MultiwayCartesianJoin::Create(new_children);
}

void MultiwayCartesianJoin::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  container_child_field_names->push_back("");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(operands_));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
