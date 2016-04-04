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

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "threading/Mutex.hpp"
#include "threading/SpinSharedMutex.hpp"

#include "glog/logging.h"

using std::make_unique;
using std::move;

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

  SpinSharedMutexExclusiveLock<false> lock(relations_mutex_);
  for (int i = 0; i < proto.relations_size(); ++i) {
    auto relation_schema = make_unique<const CatalogRelationSchema>(proto.relations(i));
    const relation_id rel_id = relation_schema->getID();

    auto it = rel_map_.find(rel_id);
    if (it == rel_map_.end()) {
      rel_map_.emplace(rel_id, move(relation_schema));
    } else {
      // TODO(zuyu): Support the schema changes based on versioning.
    }
  }
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
