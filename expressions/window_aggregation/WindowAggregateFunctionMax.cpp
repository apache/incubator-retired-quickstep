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

#include "expressions/window_aggregation/WindowAggregateFunctionMax.hpp"

#include <memory>
#include <vector>

#include "expressions/window_aggregation/WindowAggregationHandle.hpp"
#include "types/Type.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "glog/logging.h"

namespace quickstep {

bool WindowAggregateFunctionMax::canApplyToTypes(
    const std::vector<const Type*> &argument_types) const {
  // MAX is unary.
  if (argument_types.size() != 1) {
    return false;
  }

  // Argument must be comparable by '>'.
  return ComparisonFactory::GetComparison(ComparisonID::kGreater).canCompareTypes(
      *argument_types.front(),
      *argument_types.front());
}

const Type* WindowAggregateFunctionMax::resultTypeForArgumentTypes(
    const std::vector<const Type*> &argument_types) const {
  if (!canApplyToTypes(argument_types)) {
    return nullptr;
  }

  return &(argument_types.front()->getNullableVersion());
}

WindowAggregationHandle* WindowAggregateFunctionMax::createHandle(
    const std::vector<const Type*> &argument_types,
    const std::vector<std::unique_ptr<const Scalar>> &partition_by_attributes,
    const std::vector<std::unique_ptr<const Scalar>> &order_by_attributes,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create a WindowAggregationHandleMax for argument Type(s) "
      << "that MAX can not be applied to.";

  // TODO(Shixuan): Add handle for Max.
  return nullptr;
}

}  // namespace quickstep
