/**
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "expressions/aggregation/AggregateFunctionMin.hpp"

#include <vector>

#include "expressions/aggregation/AggregationHandleMin.hpp"
#include "types/Type.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "glog/logging.h"

namespace quickstep {

bool AggregateFunctionMin::canApplyToTypes(
    const std::vector<const Type*> &argument_types) const {
  // MIN is unary.
  if (argument_types.size() != 1) {
    return false;
  }

  // Argument must be comparable by '<'.
  return ComparisonFactory::GetComparison(ComparisonID::kLess).canCompareTypes(
      *argument_types.front(),
      *argument_types.front());
}

const Type* AggregateFunctionMin::resultTypeForArgumentTypes(
    const std::vector<const Type*> &argument_types) const {
  if (!canApplyToTypes(argument_types)) {
    return nullptr;
  }

  return &(argument_types.front()->getNullableVersion());
}

AggregationHandle* AggregateFunctionMin::createHandle(
    const std::vector<const Type*> &argument_types) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create an AggregationHandleMin for argument Type(s) "
      << "that MIN can not be applied to.";

  return new AggregationHandleMin(*argument_types.front());
}

}  // namespace quickstep
