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

#include "catalog/CatalogRelationStatistics.hpp"

#include "catalog/Catalog.pb.h"
#include "types/NullType.hpp"

namespace quickstep {

const TypedValue CatalogRelationStatistics::kNullValue =
    NullType::InstanceNullable().makeNullValue();

CatalogRelationStatistics::CatalogRelationStatistics(
    const serialization::CatalogRelationStatistics &proto) {
  if (proto.has_num_tuples()) {
    num_tuples_ = TypedValue::ReconstructFromProto(proto.num_tuples());
  } else {
    num_tuples_ = kNullValue;
  }

  for (int i = 0; i < proto.column_stats_size(); ++i) {
    const auto &stat_proto = proto.column_stats(i);
    auto &stat = column_stats_[stat_proto.attr_id()];
    if (stat_proto.has_num_distinct_values()) {
      stat.num_distinct_values =
          TypedValue::ReconstructFromProto(stat_proto.num_distinct_values());
    }
    if (stat_proto.has_min_value()) {
      stat.min_value =
          TypedValue::ReconstructFromProto(stat_proto.min_value());
    }
    if (stat_proto.has_max_value()) {
      stat.max_value =
          TypedValue::ReconstructFromProto(stat_proto.max_value());
    }
  }
}

serialization::CatalogRelationStatistics CatalogRelationStatistics::getProto() const {
  serialization::CatalogRelationStatistics proto;
  if (!num_tuples_.isNull()) {
    proto.mutable_num_tuples()->CopyFrom(num_tuples_.getProto());
  }
  for (const auto &pair : column_stats_) {
    auto entry = proto.add_column_stats();
    entry->set_attr_id(pair.first);
    const auto &stat = pair.second;
    if (!stat.num_distinct_values.isNull()) {
      entry->mutable_num_distinct_values()->CopyFrom(
          stat.num_distinct_values.getProto());
    }
    if (!stat.min_value.isNull()) {
      entry->mutable_min_value()->CopyFrom(
          stat.min_value.getProto());
    }
    if (!stat.max_value.isNull()) {
      entry->mutable_max_value()->CopyFrom(
          stat.max_value.getProto());
    }
  }
  return proto;
}

}  // namespace quickstep
