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

  container_child_field_names->push_back("referenced_attributes");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(referenced_attributes_));

  container_child_field_names->push_back("output_attributes");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(output_attributes_));
}

LogicalPtr SharedSubplanReference::copyWithNewChildren(const std::vector<LogicalPtr> &new_children) const {
  DCHECK(new_children.empty());
  return Create(subplan_id_, referenced_attributes_, output_attributes_);
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
