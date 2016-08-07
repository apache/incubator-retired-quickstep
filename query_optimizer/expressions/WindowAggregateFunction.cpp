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

#include "query_optimizer/expressions/WindowAggregateFunction.hpp"

#include <string>
#include <utility>
#include <vector>

#include "expressions/window_aggregation/WindowAggregateFunction.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/Type.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

bool WindowAggregateFunction::isNullable() const {
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments_) {
    argument_types.emplace_back(&argument->getValueType());
  }

  const Type *return_type = window_aggregate_.resultTypeForArgumentTypes(argument_types);
  DCHECK(return_type != nullptr);
  return return_type->isNullable();
}

const Type& WindowAggregateFunction::getValueType() const {
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments_) {
    argument_types.emplace_back(&argument->getValueType());
  }

  const Type *return_type = window_aggregate_.resultTypeForArgumentTypes(argument_types);
  DCHECK(return_type != nullptr);
  return *return_type;
}

WindowAggregateFunctionPtr WindowAggregateFunction::Create(
    const ::quickstep::WindowAggregateFunction &window_aggregate,
    const std::vector<ScalarPtr> &arguments,
    const WindowInfo &window_info,
    const std::string &window_name,
    const bool is_distinct) {
#ifdef QUICKSTEP_DEBUG
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments) {
    argument_types.emplace_back(&argument->getValueType());
  }
  DCHECK(window_aggregate.canApplyToTypes(argument_types));
#endif  // QUICKSTEP_DEBUG

  return WindowAggregateFunctionPtr(
      new WindowAggregateFunction(window_aggregate, arguments, window_info, window_name, is_distinct));
}

ExpressionPtr WindowAggregateFunction::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  std::vector<ScalarPtr> new_arguments;
  for (const ExpressionPtr &expression_ptr : new_children) {
    ScalarPtr expr_as_scalar;
    CHECK(SomeScalar::MatchesWithConditionalCast(expression_ptr, &expr_as_scalar))
        << expression_ptr->toString();
    new_arguments.emplace_back(std::move(expr_as_scalar));
  }

  return WindowAggregateFunctionPtr(
      new WindowAggregateFunction(window_aggregate_,
                                  new_arguments,
                                  window_info_,
                                  window_name_,
                                  is_distinct_));
}

std::vector<AttributeReferencePtr> WindowAggregateFunction::getReferencedAttributes() const {
  std::vector<AttributeReferencePtr> referenced_attributes;
  for (const ScalarPtr &argument : arguments_) {
    const std::vector<AttributeReferencePtr> referenced_attributes_in_argument =
        argument->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_argument.begin(),
                                 referenced_attributes_in_argument.end());
  }

  referenced_attributes.insert(referenced_attributes.end(),
                               window_info_.partition_by_attributes.begin(),
                               window_info_.partition_by_attributes.end());

  referenced_attributes.insert(referenced_attributes.end(),
                               window_info_.order_by_attributes.begin(),
                               window_info_.order_by_attributes.end());

  return referenced_attributes;
}

void WindowAggregateFunction::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("function");
  inline_field_values->push_back(window_aggregate_.getName());

  container_child_field_names->push_back("arguments");
  container_child_fields->emplace_back(CastSharedPtrVector<OptimizerTreeBase>(arguments_));

  inline_field_names->push_back("window_name");
  inline_field_values->push_back(window_name_);

  container_child_field_names->push_back("partition_by");
  container_child_fields->emplace_back(
      CastSharedPtrVector<OptimizerTreeBase>(window_info_.partition_by_attributes));

  container_child_field_names->push_back("order_by");
  container_child_fields->emplace_back(
      CastSharedPtrVector<OptimizerTreeBase>(window_info_.order_by_attributes));

  inline_field_names->push_back("is_ascending");
  std::string ascending_list("[");
  for (const bool is_ascending : window_info_.order_by_directions) {
    if (is_ascending) {
      ascending_list.append("true,");
    } else {
      ascending_list.append("false,");
    }
  }
  if (!window_info_.order_by_directions.empty()) {
    ascending_list.pop_back();
  }
  ascending_list.append("]");
  inline_field_values->push_back(ascending_list);

  inline_field_names->push_back("nulls_first");
  std::string nulls_first_flags("[");
  for (const bool nulls_first_flag : window_info_.nulls_first) {
    if (nulls_first_flag) {
      nulls_first_flags.append("true,");
    } else {
      nulls_first_flags.append("false,");
    }
  }
  if (!window_info_.nulls_first.empty()) {
    nulls_first_flags.pop_back();
  }
  nulls_first_flags.append("]");
  inline_field_values->push_back(nulls_first_flags);

  if (window_info_.frame_info != nullptr) {
    const WindowFrameInfo *frame_info = window_info_.frame_info;

    inline_field_names->push_back("frame_mode");
    inline_field_values->push_back(frame_info->is_row ? "row" : "range");

    inline_field_names->push_back("num_preceding");
    inline_field_values->push_back(std::to_string(frame_info->num_preceding));

    inline_field_names->push_back("num_following");
    inline_field_values->push_back(std::to_string(frame_info->num_following));
  }

  if (is_distinct_) {
    inline_field_names->push_back("is_distinct");
    inline_field_values->push_back("true");
  }
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
