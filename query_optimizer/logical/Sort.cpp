/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "query_optimizer/logical/Sort.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

LogicalPtr Sort::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  return Create(new_children[0], sort_attributes_, sort_ascending_, nulls_first_flags_, limit_);
}

void Sort::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  inline_field_names->push_back("is_ascending");
  std::string ascending_list("[");
  for (bool is_ascending : sort_ascending_) {
    if (is_ascending) {
      ascending_list.append("true,");
    } else {
      ascending_list.append("false,");
    }
  }
  ascending_list.pop_back();
  ascending_list.append("]");
  inline_field_values->push_back(ascending_list);

  inline_field_names->push_back("nulls_first");
  std::string nulls_first_flags("[");
  for (bool nulls_first_flag : nulls_first_flags_) {
    if (nulls_first_flag) {
      nulls_first_flags.append("true,");
    } else {
      nulls_first_flags.append("false,");
    }
  }
  nulls_first_flags.pop_back();
  nulls_first_flags.append("]");
  inline_field_values->push_back(nulls_first_flags);

  container_child_field_names->push_back("sort_expressions");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(sort_attributes_));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
