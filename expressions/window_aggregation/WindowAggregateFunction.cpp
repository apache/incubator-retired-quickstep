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

#include "expressions/window_aggregation/WindowAggregateFunction.hpp"

#include <type_traits>

#include "expressions/window_aggregation/WindowAggregateFunction.pb.h"
#include "expressions/window_aggregation/WindowAggregationID.hpp"

#include "glog/logging.h"

namespace quickstep {

serialization::WindowAggregateFunction WindowAggregateFunction::getProto() const {
  serialization::WindowAggregateFunction proto;
  switch (win_agg_id_) {
    case WindowAggregationID::kAvg:
      proto.set_window_aggregation_id(serialization::WindowAggregateFunction::AVG);
      break;
    case WindowAggregationID::kCount:
      proto.set_window_aggregation_id(serialization::WindowAggregateFunction::COUNT);
      break;
    case WindowAggregationID::kMax:
      proto.set_window_aggregation_id(serialization::WindowAggregateFunction::MAX);
      break;
    case WindowAggregationID::kMin:
      proto.set_window_aggregation_id(serialization::WindowAggregateFunction::MIN);
      break;
    case WindowAggregationID::kSum:
      proto.set_window_aggregation_id(serialization::WindowAggregateFunction::SUM);
      break;
    default: {
      LOG(FATAL) << "Unrecognized WindowAggregationID: "
                 << static_cast<std::underlying_type<WindowAggregationID>::type>(win_agg_id_);
    }
  }

  return proto;
}

}  // namespace quickstep
