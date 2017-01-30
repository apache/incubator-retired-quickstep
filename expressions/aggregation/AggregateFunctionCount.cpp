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

#include "expressions/aggregation/AggregateFunctionCount.hpp"

#include <vector>

#include "expressions/aggregation/AggregationHandleCount.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"

#include "glog/logging.h"

namespace quickstep {

bool AggregateFunctionCount::canApplyToTypes(
    const std::vector<const Type*> &argument_types) const {
  // COUNT may be nullary (i.e. COUNT(*)) or unary.
  return argument_types.size() <= 1;
}

const Type* AggregateFunctionCount::resultTypeForArgumentTypes(
    const std::vector<const Type*> &argument_types) const {
  if (!canApplyToTypes(argument_types)) {
    return nullptr;
  }

  return &TypeFactory::GetType(kLong);
}

AggregationHandle* AggregateFunctionCount::createHandle(
    const std::vector<const Type*> &argument_types) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create an AggregationHandleCount for argument Types "
      << "that COUNT can not be applied to (> 1 argument).";

  if (argument_types.empty()) {
    // COUNT(*)
    return new AggregationHandleCount<true, false>(nullptr);
  } else if (argument_types.front()->isNullable()) {
    // COUNT(some_nullable_argument)
    return new AggregationHandleCount<false, true>(argument_types.front());
  } else {
    // COUNT(non_nullable_argument)
    //
    // TODO(chasseur): Modify query optimizer to optimize-away COUNT with a
    // non-nullable argument and convert it to COUNT(*).
    return new AggregationHandleCount<false, false>(argument_types.front());
  }
}

}  // namespace quickstep
