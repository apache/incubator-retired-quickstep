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

#include "expressions/aggregation/AggregateFunction.hpp"

#include <type_traits>

#include "expressions/aggregation/AggregateFunction.pb.h"
#include "expressions/aggregation/AggregationID.hpp"

#include "glog/logging.h"

namespace quickstep {

serialization::AggregateFunction AggregateFunction::getProto() const {
  serialization::AggregateFunction proto;
  switch (agg_id_) {
    case AggregationID::kAvg:
      proto.set_aggregation_id(serialization::AggregateFunction::AVG);
      break;
    case AggregationID::kCount:
      proto.set_aggregation_id(serialization::AggregateFunction::COUNT);
      break;
    case AggregationID::kMax:
      proto.set_aggregation_id(serialization::AggregateFunction::MAX);
      break;
    case AggregationID::kMin:
      proto.set_aggregation_id(serialization::AggregateFunction::MIN);
      break;
    case AggregationID::kSum:
      proto.set_aggregation_id(serialization::AggregateFunction::SUM);
      break;
    default: {
      LOG(FATAL) << "Unrecognized AggregationID: "
                 << static_cast<std::underlying_type<AggregationID>::type>(agg_id_);
    }
  }

  return proto;
}

}  // namespace quickstep
