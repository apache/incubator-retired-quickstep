/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/SharedSubplanReference.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

void SharedSubplanReference::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("subplan_id");
  inline_field_values->push_back(std::to_string(subplan_id_));

  container_child_field_names->push_back("output_attributes");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(output_attributes_));
}

LogicalPtr SharedSubplanReference::copyWithNewChildren(const std::vector<LogicalPtr> &new_children) const {
  DCHECK(new_children.empty());
  return Create(subplan_id_, output_attributes_);
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
