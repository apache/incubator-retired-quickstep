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

#include "expressions/aggregation/AggregateFunctionMax.hpp"

#include <vector>

#include "expressions/aggregation/AggregationHandleMax.hpp"
#include "types/Type.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "glog/logging.h"

namespace quickstep {

bool AggregateFunctionMax::canApplyToTypes(
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

const Type* AggregateFunctionMax::resultTypeForArgumentTypes(
    const std::vector<const Type*> &argument_types) const {
  if (!canApplyToTypes(argument_types)) {
    return nullptr;
  }

  // FIXME(jianqiao): The result type can be nullable when it is NOT a group-by
  // aggregation.
  return argument_types.front();
}

AggregationHandle* AggregateFunctionMax::createHandle(
    const std::vector<const Type*> &argument_types) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create an AggregationHandleMax for argument Type(s) "
      << "that MAX can not be applied to.";

  return new AggregationHandleMax(*argument_types.front());
}

}  // namespace quickstep
