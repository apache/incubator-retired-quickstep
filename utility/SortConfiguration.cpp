/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "utility/SortConfiguration.hpp"

#include <cstddef>
#include <utility>
#include <vector>

#include "expressions/ExpressionFactories.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "utility/PtrVector.hpp"
#include "utility/SortConfiguration.pb.h"

#include "glog/logging.h"

using std::move;
using std::vector;

namespace quickstep {

SortConfiguration* SortConfiguration::ReconstructFromProto(const serialization::SortConfiguration &proto,
                                                           const CatalogDatabase &database) {
  DCHECK(ProtoIsValid(proto, database));

  PtrVector<Scalar> order_by;
  vector<bool> ordering;
  vector<bool> null_ordering;

  for (int i = 0; i < proto.order_by_list_size(); ++i) {
    const serialization::SortConfiguration::OrderBy &order_by_proto = proto.order_by_list(i);

    order_by.push_back(ScalarFactory::ReconstructFromProto(order_by_proto.expression(), database));
    ordering.push_back(order_by_proto.is_ascending());
    null_ordering.push_back(order_by_proto.null_first());
  }

  return new SortConfiguration(order_by, move(ordering), move(null_ordering));
}

bool SortConfiguration::ProtoIsValid(const serialization::SortConfiguration &proto,
                                     const CatalogDatabase &database) {
  for (int i = 0; i < proto.order_by_list_size(); ++i) {
    const serialization::SortConfiguration::OrderBy &order_by_proto = proto.order_by_list(i);

    if (!order_by_proto.IsInitialized() ||
        !ScalarFactory::ProtoIsValid(order_by_proto.expression(), database)) {
      return false;
    }
  }

  return true;
}

serialization::SortConfiguration SortConfiguration::getProto() const {
  serialization::SortConfiguration proto;
  for (std::size_t i = 0; i < order_by_.size(); ++i) {
    auto *order_by_attr_proto = proto.add_order_by_list();
    order_by_attr_proto->mutable_expression()->CopyFrom(order_by_[i].getProto());
    order_by_attr_proto->set_is_ascending(ordering_[i]);
    order_by_attr_proto->set_null_first(null_ordering_[i]);
  }
  return proto;
}

}  // namespace quickstep
