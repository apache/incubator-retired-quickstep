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

#include "expressions/window_aggregation/WindowAggregateFunctionCount.hpp"

#include <vector>

#include "expressions/window_aggregation/WindowAggregationHandle.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"

#include "glog/logging.h"

namespace quickstep {

bool WindowAggregateFunctionCount::canApplyToTypes(
    const std::vector<const Type*> &argument_types) const {
  // COUNT may be nullary (i.e. COUNT(*)) or unary.
  return argument_types.size() <= 1;
}

const Type* WindowAggregateFunctionCount::resultTypeForArgumentTypes(
    const std::vector<const Type*> &argument_types) const {
  if (!canApplyToTypes(argument_types)) {
    return nullptr;
  }

  return &TypeFactory::GetType(kLong);
}

WindowAggregationHandle* WindowAggregateFunctionCount::createHandle(
    const std::vector<const Type*> &argument_types,
    const std::vector<std::unique_ptr<const Scalar>> &partition_by_attributes,
    const std::vector<std::unique_ptr<const Scalar>> &order_by_attributes,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create a WindowAggregationHandleCount for argument Types "
      << "that COUNT can not be applied to (> 1 argument).";

  // TODO(Shixuan): Add handle for Count.
  return nullptr;
}

}  // namespace quickstep
