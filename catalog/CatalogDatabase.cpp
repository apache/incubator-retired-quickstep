/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "catalog/CatalogDatabase.hpp"

#include <cstddef>
#include <cstring>
#include <string>
#include <unordered_map>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogErrors.hpp"
#include "catalog/CatalogRelation.hpp"
#include "threading/Mutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/PtrVector.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

using std::size_t;
using std::strcmp;
using std::string;

namespace quickstep {

bool CatalogDatabase::ProtoIsValid(const serialization::CatalogDatabase &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  for (int i = 0; i < proto.relations_size(); ++i) {
    if (!CatalogRelation::ProtoIsValid(proto.relations(i))) {
      return false;
    }
  }

  return true;
}

CatalogDatabase::CatalogDatabase(const serialization::CatalogDatabase &proto)
    : name_(proto.name()),
      status_(Status::kConsistent) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create CatalogDatabase from an invalid proto description:\n"
      << proto.DebugString();

  for (int index_relations = 0, index_null_relations = 0;
       index_relations < proto.null_relations_size() + proto.relations_size();
       ++index_relations) {
    if (index_null_relations < proto.null_relations_size() &&
        index_relations == proto.null_relations(index_null_relations)) {
      rel_vec_.push_back(NULL);
      ++index_null_relations;
    } else {
      addRelation(new CatalogRelation(proto.relations(index_relations - index_null_relations)));
    }
  }
}

const CatalogRelation* CatalogDatabase::getRelationByName(const string &rel_name) const {
  SpinSharedMutexSharedLock<false> lock(relations_mutex_);
  std::unordered_map<string, CatalogRelation*>::const_iterator it = rel_map_.find(ToLower(rel_name));
  if (it == rel_map_.end()) {
    return nullptr;
  } else {
    return it->second;
  }
}

CatalogRelation* CatalogDatabase::getRelationByNameMutable(const string &rel_name) {
  SpinSharedMutexSharedLock<false> lock(relations_mutex_);
  std::unordered_map<string, CatalogRelation*>::const_iterator it = rel_map_.find(ToLower(rel_name));
  if (it == rel_map_.end()) {
    return nullptr;
  } else {
    return it->second;
  }
}

relation_id CatalogDatabase::addRelation(CatalogRelation *new_rel) {
  const string lower_rel_name = ToLower(new_rel->getName());
  {
    SpinSharedMutexExclusiveLock<false> lock(relations_mutex_);
    if (hasRelationWithNameUnsafe(lower_rel_name)) {
      throw RelationNameCollision(name_, new_rel->getName());
    } else if (rel_vec_.size() > static_cast<size_t>(kCatalogMaxID)) {
      throw CatalogIDOverflow("relation");
    } else {
      rel_map_[lower_rel_name] = new_rel;
      rel_vec_.push_back(new_rel);
      new_rel->setParent(this);
      new_rel->setID(static_cast<relation_id>(rel_vec_.size() - 1));
      return (new_rel->getID());
    }
  }
}

void CatalogDatabase::dropRelationByName(const std::string &rel_name) {
  SpinSharedMutexExclusiveLock<false> lock(relations_mutex_);
  std::unordered_map<string, CatalogRelation*>::iterator it = rel_map_.find(ToLower(rel_name));
  if (it == rel_map_.end()) {
    throw RelationNameNotFound(name_, rel_name);
  } else {
    rel_vec_.deleteElement(it->second->getID());
    rel_map_.erase(it);
  }
}

void CatalogDatabase::dropRelationById(const relation_id id) {
  SpinSharedMutexExclusiveLock<false> lock(relations_mutex_);
  if (hasRelationWithIdUnsafe(id)) {
    rel_map_.erase(ToLower(rel_vec_[id].getName()));
    rel_vec_.deleteElement(id);
  } else {
    throw RelationIdNotFound(name_, id);
  }
}

serialization::CatalogDatabase CatalogDatabase::getProto() const {
  serialization::CatalogDatabase proto;
  proto.set_name(name_);

  int i = 0;
  for (PtrVector<CatalogRelation, true>::const_iterator it = rel_vec_.begin(); it != rel_vec_.end(); ++it, ++i) {
    if (it.isNull()) {
      proto.add_null_relations(i);
    } else {
      proto.add_relations()->CopyFrom(it->getProto());
    }
  }
  return proto;
}

}  // namespace quickstep
