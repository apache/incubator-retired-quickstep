/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/aggregation/AggregateFunctionSum.hpp"

#include <vector>

#include "expressions/aggregation/AggregationHandleSum.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"

#include "glog/logging.h"

namespace quickstep {

bool AggregateFunctionSum::canApplyToTypes(
    const std::vector<const Type*> &argument_types) const {
  // SUM is unary.
  if (argument_types.size() != 1) {
    return false;
  }

  // Argument must be addable.
  return BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
             .canApplyToTypes(*argument_types.front(), *argument_types.front());
}

const Type* AggregateFunctionSum::resultTypeForArgumentTypes(
    const std::vector<const Type*> &argument_types) const {
  if (!canApplyToTypes(argument_types)) {
    return nullptr;
  }

  // SUM may return NULL if there are no input rows, and we automatically widen
  // int to long and float to double to have more headroom when adding up many
  // values.
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

  return sum_type;
}

AggregationHandle* AggregateFunctionSum::createHandle(
    const std::vector<const Type*> &argument_types) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create an AggregationHandleSum for argument Type(s) "
      << "that SUM can not be applied to.";

  return new AggregationHandleSum(*argument_types.front());
}

}  // namespace quickstep
