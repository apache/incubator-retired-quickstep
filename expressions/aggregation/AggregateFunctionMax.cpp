/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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

  return &(argument_types.front()->getNullableVersion());
}

AggregationHandle* AggregateFunctionMax::createHandle(
    const std::vector<const Type*> &argument_types) const {
  DCHECK(canApplyToTypes(argument_types))
      << "Attempted to create an AggregationHandleMax for argument Type(s) "
      << "that MAX can not be applied to.";

  return new AggregationHandleMax(*argument_types.front());
}

}  // namespace quickstep
