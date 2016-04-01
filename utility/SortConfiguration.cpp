/**
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include "utility/SortConfiguration.hpp"

#include <cstddef>
#include <utility>
#include <vector>

#include "expressions/ExpressionFactories.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "utility/PtrVector.hpp"
#include "utility/SortConfiguration.pb.h"

#include "glog/logging.h"

using std::move;
using std::vector;

namespace quickstep {

SortConfiguration* SortConfiguration::ReconstructFromProto(const serialization::SortConfiguration &proto,
                                                           const CatalogDatabaseLite &database) {
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
                                     const CatalogDatabaseLite &database) {
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
