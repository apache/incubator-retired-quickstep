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

#include "expressions/aggregation/AggregateFunctionAvg.hpp"

#include <vector>

#include "expressions/aggregation/AggregationHandleAvg.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/OperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"

#include "glog/logging.h"

namespace quickstep {

bool AggregateFunctionAvg::canApplyToTypes(
    const std::vector<const Type*> &argument_types) const {
  // AVG is unary.
  if (argument_types.size() != 1) {
    return false;
  }

  // Argument must be addable and divisible.
  const Type &type = *argument_types.front();
  if (!OperationFactory::Instance()
          .getBinaryOperation("+", {type.getTypeID(), type.getTypeID()})
              ->canApplyTo(type, type)) {
    return false;
  }
  return OperationFactory::Instance()
      .getBinaryOperation("/", {type.getTypeID(), kDouble})
          ->canApplyTo(type, TypeFactory::GetType(kDouble));
}

const Type* AggregateFunctionAvg::resultTypeForArgumentTypes(
    const std::vector<const Type*> &argument_types) const {
  if (!canApplyToTypes(argument_types)) {
    return nullptr;
  }

  // The type used to sum values is nullable, and we automatically widen int to
  // long and float to double to have more headroom when adding up many values.
  const Type *sum_type = &(argument_types.front()->getNullableVersion());
  switch (sum_type->getTypeID()) {
    case kInt:
      sum_type = &TypeFactory::GetType(kLong, true);
      break;
    case kFloat:
      sum_type = &TypeFactory::GetType(kDouble, true);
      break;
    default:
      break;
  }

  return OperationFactory::Instance()
      .getBinaryOperation("/", {sum_type->getTypeID(), kDouble})
          ->getResultType(*sum_type, TypeFactory::GetType(kDouble));
}

AggregationHandle* AggregateFunctionAvg::createHandle(
    const std::vector<const Type*> &argument_types) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create an AggregationHandleAvg for argument Type(s) "
      << "that AVG can not be applied to.";

  return new AggregationHandleAvg(*argument_types.front());
}

}  // namespace quickstep
