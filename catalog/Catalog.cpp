/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "catalog/Catalog.hpp"

#include <cstddef>
#include <cstring>
#include <string>
#include <unordered_map>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogErrors.hpp"
#include "utility/PtrVector.hpp"

#include "glog/logging.h"

using std::size_t;
using std::strcmp;
using std::string;

namespace quickstep {

bool Catalog::ProtoIsValid(const serialization::Catalog &proto) {
  for (int i = 0; i < proto.databases_size(); ++i) {
    if (!CatalogDatabase::ProtoIsValid(proto.databases(i))) {
      return false;
    }
  }
  return true;
}

Catalog::Catalog(const serialization::Catalog &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create Catalog from an invalid proto description:\n"
      << proto.DebugString();

  for (int i = 0; i < proto.databases_size(); ++i) {
    if (proto.databases(i).IsInitialized()) {
      addDatabase(new CatalogDatabase(proto.databases(i)));
    } else {
      db_vec_.push_back(NULL);
    }
  }
}

const CatalogDatabase& Catalog::getDatabaseByName(const string &db_name) const {
  std::unordered_map<std::string, CatalogDatabase*>::const_iterator it = db_map_.find(db_name);
  if (it == db_map_.end()) {
    throw DatabaseNameNotFound(db_name);
  } else {
    return *(it->second);
  }
}

CatalogDatabase* Catalog::getDatabaseByNameMutable(const string &db_name) {
  std::unordered_map<std::string, CatalogDatabase*>::const_iterator it = db_map_.find(db_name);
  if (it == db_map_.end()) {
    throw DatabaseNameNotFound(db_name);
  } else {
    return it->second;
  }
}

database_id Catalog::addDatabase(CatalogDatabase *new_db) {
  const string &db_name = new_db->getName();
  if (hasDatabaseWithName(db_name)) {
    throw DatabaseNameCollision(db_name);
  } else if (db_vec_.size() > static_cast<size_t>(kCatalogMaxID)) {
    throw CatalogIDOverflow("database");
  } else {
    db_map_[db_name] = new_db;
    db_vec_.push_back(new_db);
    new_db->setParent(this);
    new_db->setID(static_cast<database_id>(db_vec_.size() - 1));
    return (new_db->getID());
  }
}

serialization::Catalog Catalog::getProto() const {
  serialization::Catalog proto;

  for (PtrVector<CatalogDatabase, true>::const_iterator it = db_vec_.begin();
       it != db_vec_.end();
       ++it) {
    if (it.isNull()) {
      proto.add_databases();
    } else {
      proto.add_databases()->CopyFrom(it->getProto());
    }
  }

  return proto;
}
}  // namespace quickstep
