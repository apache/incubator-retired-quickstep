/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
    return new AggregationHandleCount<true, false>();
  } else if (argument_types.front()->isNullable()) {
    // COUNT(some_nullable_argument)
    return new AggregationHandleCount<false, true>();
  } else {
    // COUNT(non_nullable_argument)
    //
    // TODO(chasseur): Modify query optimizer to optimize-away COUNT with a
    // non-nullable argument and convert it to COUNT(*).
    return new AggregationHandleCount<false, false>();
  }
}

}  // namespace quickstep
