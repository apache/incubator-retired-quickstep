/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/aggregation/AggregateFunctionFactory.hpp"

#include <string>
#include <type_traits>

#include "expressions/aggregation/AggregateFunction.pb.h"
#include "expressions/aggregation/AggregateFunctionAvg.hpp"
#include "expressions/aggregation/AggregateFunctionCount.hpp"
#include "expressions/aggregation/AggregateFunctionMax.hpp"
#include "expressions/aggregation/AggregateFunctionMin.hpp"
#include "expressions/aggregation/AggregateFunctionSum.hpp"
#include "expressions/aggregation/AggregationID.hpp"

#include "glog/logging.h"

namespace quickstep {

const AggregateFunction& AggregateFunctionFactory::Get(
    const AggregationID agg_id) {
  switch (agg_id) {
    case AggregationID::kAvg:
      return AggregateFunctionAvg::Instance();
    case AggregationID::kCount:
      return AggregateFunctionCount::Instance();
    case AggregationID::kMax:
      return AggregateFunctionMax::Instance();
    case AggregationID::kMin:
      return AggregateFunctionMin::Instance();
    case AggregationID::kSum:
      return AggregateFunctionSum::Instance();
    default: {
      LOG(FATAL) << "Unrecognized AggregationID: "
                 << static_cast<std::underlying_type<AggregationID>::type>(agg_id);
    }
  }
}

const AggregateFunction* AggregateFunctionFactory::GetByName(const std::string &name) {
  if (name == "avg") {
    return &AggregateFunctionAvg::Instance();
  } else if (name == "count") {
    return &AggregateFunctionCount::Instance();
  } else if (name == "max") {
    return &AggregateFunctionMax::Instance();
  } else if (name == "min") {
    return &AggregateFunctionMin::Instance();
  } else if (name == "sum") {
    return &AggregateFunctionSum::Instance();
  } else {
    return nullptr;
  }
}

bool AggregateFunctionFactory::ProtoIsValid(
    const serialization::AggregateFunction &proto) {
  return proto.IsInitialized()
         && serialization::AggregateFunction::AggregationID_IsValid(proto.aggregation_id());
}

const AggregateFunction& AggregateFunctionFactory::ReconstructFromProto(
    const serialization::AggregateFunction &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to reconstruct an AggregateFunction from an invalid proto:\n"
      << proto.DebugString();

  switch (proto.aggregation_id()) {
    case serialization::AggregateFunction::AVG:
      return AggregateFunctionAvg::Instance();
    case serialization::AggregateFunction::COUNT:
      return AggregateFunctionCount::Instance();
    case serialization::AggregateFunction::MAX:
      return AggregateFunctionMax::Instance();
    case serialization::AggregateFunction::MIN:
      return AggregateFunctionMin::Instance();
    case serialization::AggregateFunction::SUM:
      return AggregateFunctionSum::Instance();
    default: {
      LOG(FATAL) << "Unrecognized serialization::AggregateFunction::AggregationID: "
                 << proto.aggregation_id()
                 << "\nFull proto debug string:\n"
                 << proto.DebugString();
    }
  }
}

}  // namespace quickstep
