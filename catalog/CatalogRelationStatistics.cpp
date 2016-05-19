/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "catalog/CatalogRelationStatistics.hpp"

#include "catalog/Catalog.pb.h"

namespace quickstep {

CatalogRelationStatistics::CatalogRelationStatistics(
    const serialization::CatalogRelationStatistics &proto) {
  if (proto.has_num_tuples()) {
    num_tuples_ = proto.num_tuples();
  }
  for (int i = 0; i < proto.num_distinct_values_map_size(); ++i) {
    const auto &entry = proto.num_distinct_values_map(i);
    num_distinct_values_map_.emplace(entry.attr_id(),
                                     entry.num_distinct_values());
  }
}

serialization::CatalogRelationStatistics CatalogRelationStatistics::getProto() const {
  serialization::CatalogRelationStatistics proto;
  if (num_tuples_ != 0) {
    proto.set_num_tuples(num_tuples_);
  }
  for (const auto &pair : num_distinct_values_map_) {
    auto entry = proto.add_num_distinct_values_map();
    entry->set_attr_id(pair.first);
    entry->set_num_distinct_values(pair.second);
  }
  return proto;
}

}  // namespace quickstep
