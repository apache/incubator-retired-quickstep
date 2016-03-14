/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
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

#include "query_optimizer/logical/Sample.hpp"

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

LogicalPtr Sample::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return Sample::Create(new_children[0], is_block_sample_, percentage_);
}

void Sample::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields)
    const {
    non_container_child_field_names->push_back("input");
    non_container_child_fields->push_back(input_);

    inline_field_names->push_back("percentage");
    inline_field_values->push_back(std::to_string(percentage_));

    inline_field_names->push_back("is_block_sample");
    inline_field_values->push_back(std::to_string(is_block_sample_));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
