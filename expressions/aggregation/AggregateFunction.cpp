/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
