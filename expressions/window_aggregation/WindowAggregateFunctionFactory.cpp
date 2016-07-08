/**
 *   Licensed to the Apache Software Foundation (ASF) under one
 *   or more contributor license agreements.  See the NOTICE file
 *   distributed with this work for additional information
 *   regarding copyright ownership.  The ASF licenses this file
 *   to you under the Apache License, Version 2.0 (the
 *   "License"); you may not use this file except in compliance
 *   with the License.  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *   Unless required by applicable law or agreed to in writing,
 *   software distributed under the License is distributed on an
 *   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *   KIND, either express or implied.  See the License for the
 *   specific language governing permissions and limitations
 *   under the License.
 **/

#include "expressions/window_aggregation/WindowAggregateFunctionFactory.hpp"

#include <string>
#include <type_traits>

#include "expressions/window_aggregation/WindowAggregateFunction.pb.h"
#include "expressions/window_aggregation/WindowAggregateFunctionAvg.hpp"
#include "expressions/window_aggregation/WindowAggregateFunctionCount.hpp"
#include "expressions/window_aggregation/WindowAggregateFunctionMax.hpp"
#include "expressions/window_aggregation/WindowAggregateFunctionMin.hpp"
#include "expressions/window_aggregation/WindowAggregateFunctionSum.hpp"
#include "expressions/window_aggregation/WindowAggregationID.hpp"

#include "glog/logging.h"

namespace quickstep {

const WindowAggregateFunction& WindowAggregateFunctionFactory::Get(
    const WindowAggregationID agg_id) {
  switch (agg_id) {
    case WindowAggregationID::kAvg:
      return WindowAggregateFunctionAvg::Instance();
    case WindowAggregationID::kCount:
      return WindowAggregateFunctionCount::Instance();
    case WindowAggregationID::kMax:
      return WindowAggregateFunctionMax::Instance();
    case WindowAggregationID::kMin:
      return WindowAggregateFunctionMin::Instance();
    case WindowAggregationID::kSum:
      return WindowAggregateFunctionSum::Instance();
    default: {
      LOG(FATAL) << "Unrecognized WindowAggregationID: "
                 << static_cast<std::underlying_type<WindowAggregationID>::type>(agg_id);
    }
  }
}

const WindowAggregateFunction* WindowAggregateFunctionFactory::GetByName(
    const std::string &name) {
  if (name == "avg") {
    return &WindowAggregateFunctionAvg::Instance();
  } else if (name == "count") {
    return &WindowAggregateFunctionCount::Instance();
  } else if (name == "max") {
    return &WindowAggregateFunctionMax::Instance();
  } else if (name == "min") {
    return &WindowAggregateFunctionMin::Instance();
  } else if (name == "sum") {
    return &WindowAggregateFunctionSum::Instance();
  } else {
    return nullptr;
  }
}

bool WindowAggregateFunctionFactory::ProtoIsValid(
    const serialization::WindowAggregateFunction &proto) {
  return proto.IsInitialized() &&
         serialization::WindowAggregateFunction::WindowAggregationID_IsValid(proto.window_aggregation_id());
}

const WindowAggregateFunction& WindowAggregateFunctionFactory::ReconstructFromProto(
    const serialization::WindowAggregateFunction &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to reconstruct an WindowAggregateFunction from an invalid proto:\n"
      << proto.DebugString();

  switch (proto.window_aggregation_id()) {
    case serialization::WindowAggregateFunction::AVG:
      return WindowAggregateFunctionAvg::Instance();
    case serialization::WindowAggregateFunction::COUNT:
      return WindowAggregateFunctionCount::Instance();
    case serialization::WindowAggregateFunction::MAX:
      return WindowAggregateFunctionMax::Instance();
    case serialization::WindowAggregateFunction::MIN:
      return WindowAggregateFunctionMin::Instance();
    case serialization::WindowAggregateFunction::SUM:
      return WindowAggregateFunctionSum::Instance();
    default: {
      LOG(FATAL) << "Unrecognized serialization::WindowAggregateFunction::WindowAggregationID: "
                 << proto.window_aggregation_id()
                 << "\nFull proto debug string:\n"
                 << proto.DebugString();
    }
  }
}

}  // namespace quickstep
