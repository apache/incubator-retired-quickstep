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

#include "catalog/CatalogDatabaseCache.hpp"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "threading/Mutex.hpp"
#include "threading/SpinSharedMutex.hpp"

#include "glog/logging.h"

using std::make_unique;
using std::move;
using std::vector;

namespace quickstep {

bool CatalogDatabaseCache::ProtoIsValid(const serialization::CatalogDatabase &proto) {
  for (int i = 0; i < proto.relations_size(); ++i) {
    if (!CatalogRelationSchema::ProtoIsValid(proto.relations(i))) {
      return false;
    }
  }

  return proto.null_relations_size() == 0;
}

CatalogDatabaseCache::CatalogDatabaseCache(const serialization::CatalogDatabase &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create CatalogDatabaseCache from an invalid proto description:\n"
      << proto.DebugString();

  for (int i = 0; i < proto.relations_size(); ++i) {
    auto relation_schema = make_unique<const CatalogRelationSchema>(proto.relations(i));
    rel_map_.emplace(relation_schema->getID(), move(relation_schema));
  }
}

void CatalogDatabaseCache::update(const serialization::CatalogDatabase &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create CatalogDatabaseCache from an invalid proto description:\n"
      << proto.DebugString();

  vector<int> new_relation_schema_proto_indices;
  {
    SpinSharedMutexSharedLock<false> read_lock(relations_mutex_);
    for (int i = 0; i < proto.relations_size(); ++i) {
      const auto it = rel_map_.find(proto.relations(i).relation_id());
      if (it == rel_map_.end()) {
        new_relation_schema_proto_indices.push_back(i);
      } else {
        // TODO(quickstep-team): Support schema changes by adding the index of
        // changed schema proto in 'changed_relation_schema_proto_indices'.
      }
    }
  }

  SpinSharedMutexExclusiveLock<false> write_lock(relations_mutex_);
  for (const int i : new_relation_schema_proto_indices) {
    const serialization::CatalogRelationSchema &proto_relation = proto.relations(i);
    auto relation_schema = make_unique<const CatalogRelationSchema>(proto_relation);
    rel_map_.emplace(proto_relation.relation_id(), move(relation_schema));
  }

  // TODO(quickstep-team): Reset the schema for the changes in the following
  // steps for each index in 'changed_relation_schema_proto_indices':
  // 1. Drop the blocks belonged to 'proto.relations(i).relation_id()' in the
  //    buffer pool.
  // 2. Reset the changed schema, while the scheduler ensures no queries will
  //    load back the related blocks.
  // 3. Signal the scheduler to accept new queries for the changed schema.
}

void CatalogDatabaseCache::dropRelationById(const relation_id id) {
  SpinSharedMutexExclusiveLock<false> lock(relations_mutex_);
  auto it = rel_map_.find(id);
  if (it != rel_map_.end()) {
    rel_map_.erase(it);
  } else {
    LOG(ERROR) << "Dropping a non-exist relation schema by id " << id;
  }
}

}  // namespace quickstep
