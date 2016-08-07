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

#ifndef QUICKSTEP_CATALOG_CATALOG_DATABASE_HPP_
#define QUICKSTEP_CATALOG_CATALOG_DATABASE_HPP_

#include <exception>
#include <string>
#include <unordered_map>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabaseLite.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageConstants.hpp"
#include "threading/Mutex.hpp"
#include "threading/SharedMutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

class Catalog;
class CatalogRelationSchema;

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief Exception thrown for a relation name collision.
 **/
class RelationNameCollision : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param db_name Name of the database in which the collision occurred.
   * @param rel_name Name of the relation for which there was a collision.
   **/
  RelationNameCollision(const std::string &db_name, const std::string &rel_name)
      : message_("RelationNameCollision: database ") {
    message_.append(db_name);
    message_.append(" already has a relation ");
    message_.append(rel_name);
  }

  ~RelationNameCollision() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when a relation with the specified name can't be
 *        found.
 **/
class RelationNameNotFound : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param db_name Name of the database in which the exception occurred.
   * @param rel_name The relation name which could not be found.
   **/
  RelationNameNotFound(const std::string &db_name, const std::string &rel_name)
      : message_("RelationNameNotFound: database ") {
    message_.append(db_name);
    message_.append(" has no relation named ");
    message_.append(rel_name);
  }

  ~RelationNameNotFound() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when a relation with the specified ID can't be found.
 **/
class RelationIdNotFound : public std::exception {
 public:
  /**
   * @brief Constructor
   *
   * @param db_name Name of the database in which the exception occurred.
   * @param id The relation ID which could not be found.
   **/
  RelationIdNotFound(const std::string &db_name, const relation_id id)
      : message_("RelationIdNotFound: database ") {
    message_.append(db_name);
    message_.append(" has no relation with ID ");
    message_.append(std::to_string(id));
  }

  ~RelationIdNotFound() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief A single database in the catalog.
 **/
class CatalogDatabase : public CatalogDatabaseLite {
 public:
  typedef std::unordered_map<std::string, CatalogRelation*>::size_type size_type;
  typedef PtrVector<CatalogRelation, true>::const_skip_iterator const_iterator;

  enum class Status {
    kConsistent = 0,
    kPendingBlockDeletions
  };

  /**
   * @brief Create a new database.
   *
   * @param parent The catalog this database belongs to.
   * @param name This database's name.
   * @param id This database's ID (defaults to -1, which means invalid/unset).
   **/
  CatalogDatabase(Catalog *parent, const std::string &name, const database_id id = -1)
      : CatalogDatabaseLite(id),
        parent_(parent),
        name_(name),
        status_(Status::kConsistent) {
  }

  /**
   * @brief Reconstruct a database from its serialized Protocol Buffer form.
   *
   * @param proto The Protocol Buffer serialization of a database, previously
   *        produced by getProto().
   **/
  explicit CatalogDatabase(const serialization::CatalogDatabase &proto);

  /**
   * @brief Check whether a serialization::CatalogDatabase is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a CatalogDatabase,
   *        originally generated by getProto().
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::CatalogDatabase &proto);

  /**
   * @brief Destructor which recursively destroys children.
   **/
  ~CatalogDatabase() override {
  }

  bool hasRelationWithId(const relation_id id) const override {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    return hasRelationWithIdUnsafe(id);
  }

  const CatalogRelationSchema& getRelationSchemaById(const relation_id id) const override {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    DCHECK(hasRelationWithIdUnsafe(id));
    return rel_vec_[id];
  }

  /**
   * @exception RelationIdNotFound No relation with the given ID exists.
   **/
  void dropRelationById(const relation_id id) override;

  /**
   * @brief Get the parent catalog.
   *
   * @return Parent catalog.
   **/
  const Catalog& getParent() const {
    return *DCHECK_NOTNULL(parent_);
  }

  /**
   * @brief Get a mutable pointer to the parent catalog.
   *
   * @return Parent catalog.
   **/
  Catalog* getParentMutable() {
    return parent_;
  }

  /**
   * @brief Get this database's name.
   *
   * @return This database's name.
   **/
  const std::string& getName() const {
    return name_;
  }

  /**
   * @brief Get this database's status.
   *
   * @return This database's status.
   **/
  Status status() const {
    SpinSharedMutexSharedLock<false> lock(status_mutex_);
    return status_;
  }

  /**
   * @brief Whether this database is consistent.
   *
   * @return True if it is consistent. Otherwise, false.
   **/
  bool isStatusConsistent() const {
    SpinSharedMutexSharedLock<false> lock(status_mutex_);
    return status_ == Status::kConsistent;
  }

  /**
   * @brief Set this database's status.
   *
   * @param status The status to set.
   **/
  void setStatus(const Status status) {
    SpinSharedMutexExclusiveLock<false> lock(status_mutex_);
    status_ = status;
  }

  /**
   * @brief Check whether a relation with the given name exists. The search is case-insensitive.
   *
   * @param rel_name The name to check for.
   * @return Whether the relation exists.
   **/
  bool hasRelationWithName(const std::string &rel_name) const {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    return hasRelationWithNameUnsafe(rel_name);
  }

  /**
   * @brief Get a relation by name. The search is case-insensitive.
   *
   * @param rel_name The name to search for.
   * @return The relation with the given name. NULL if the relation is not found.
   **/
  const CatalogRelation* getRelationByName(const std::string &rel_name) const;

  /**
   * @brief Get a mutable pointer to a relation by name. The search is case-insensitive.
   *
   * @param rel_name The name to search for.
   * @return The relation with the given name. NULL if the relation is not found.
   **/
  CatalogRelation* getRelationByNameMutable(const std::string &rel_name);

  /**
   * @brief Get a relation by ID.
   *
   * @param id The id to search for.
   * @return The relation with the given ID.
   **/
  const CatalogRelation* getRelationById(const relation_id id) const {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    if (hasRelationWithIdUnsafe(id)) {
      return &rel_vec_[id];
    } else {
      return nullptr;
    }
  }

  /**
   * @brief Get a mutable pointer to a relation by ID.
   *
   * @param id The id to search for.
   * @return The relation with the given ID.
   **/
  CatalogRelation* getRelationByIdMutable(const relation_id id) {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    if (hasRelationWithIdUnsafe(id)) {
      return &(rel_vec_[id]);
    } else {
      return nullptr;
    }
  }

  /**
   * @brief Add a new relation to the database. If the relation already has an
   *        ID and/or parent, it will be overwritten.
   *
   * @param new_rel The relation to be added.
   * @exception RelationNameCollision A relation with the same name as new_rel
   *            is already present in the database.
   * @return The id assigned to the relation.
   **/
  relation_id addRelation(CatalogRelation *new_rel);

  /**
   * @brief Drop (delete) a relation by name.
   *
   * @param rel_name The name of the relation to drop.
   * @exception RelationNameNotFound No relation with the given name exists.
   **/
  void dropRelationByName(const std::string &rel_name);

  /**
   * @brief Serialize the database as Protocol Buffer.
   *
   * @return The Protocol Buffer representation of the database.
   **/
  serialization::CatalogDatabase getProto() const;

  /**
   * @brief Get the number of child relations.
   *
   * @return The number of child relations.
   **/
  size_type size() const {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    return rel_map_.size();
  }

  /**
   * @brief Get an iterator at the beginning of the child relations.
   *
   * @return An iterator on the first child relation.
   **/
  const_iterator begin() const {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    return rel_vec_.begin_skip();
  }

  /**
   * @brief Get an iterator at one-past-the-end of the child relations.
   *
   * @return An iterator one-past-the-end of the child relations.
   **/
  const_iterator end() const {
    SpinSharedMutexSharedLock<false> lock(relations_mutex_);
    return rel_vec_.end_skip();
  }

 private:
  /**
   * @brief Set the parent Catalog of this database. Used by Catalog (a friend
   *        of this class) when adding a new database.
   *
   * @param parent The new parent for this CatalogDatabase.
   **/
  void setParent(Catalog *parent) {
    parent_ = parent;
  }

  /**
   * @brief Set the ID of this database. Used by Catalog (a friend of this
   *        class) when adding a new database.
   *
   * @param id The new ID for this CatalogDatabase.
   **/
  void setID(const database_id id) {
    id_ = id;
  }

  /**
   * @brief Check whether a relation_id is within the range of IDs contained
   *        in this CatalogDatabase.
   *
   * @param id The id to check.
   * @return true if id is in range, false otherwise.
   *
   * @note The caller of this function should make it sure that it holds at
   *       least a shared lock before calling this function.
   *
   **/
  bool idInRange(const relation_id id) const {
    return ((id >= 0)
            && (static_cast<PtrVector<CatalogRelation>::size_type>(id) < rel_vec_.size()));
  }

  /**
   * @brief Check whether a relation with the given name exists. The search is case-insensitive.
   *
   * @note This method assumes that the caller has already acquired the
   *       necessary locks before invoking it.
   *
   * @param rel_name The name to check for.
   * @return Whether the relation exists.
   **/
  inline bool hasRelationWithNameUnsafe(const std::string &rel_name) const {
    return (rel_map_.find(ToLower(rel_name)) != rel_map_.end());
  }

  /**
   * @brief Check whether a relation with the given id exists.
   *
   * @note This method assumes that the caller has already acquired the
   *       necessary locks before invoking it.
   *
   * @param id The id to check for.
   * @return Whether the relation exists.
   **/
  inline bool hasRelationWithIdUnsafe(const relation_id id) const {
    return (idInRange(id) && !rel_vec_.elementIsNull(id));
  }

  Catalog *parent_;

  // The database name.
  const std::string name_;

  // Indicate the status of this database (i.e., consistent or not).
  Status status_;
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> status_mutex_;

  // A vector of relations. NULL if the relation has dropped from the database.
  PtrVector<CatalogRelation, true> rel_vec_;

  /**
   * @brief Map from relation name to the pointer to the corresponding relation.
   *        The relation name stored in the map is in lowercase characters.
   */
  std::unordered_map<std::string, CatalogRelation*> rel_map_;

  // Concurrency protection for 'rel_vec_' and 'rel_map_'.
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> relations_mutex_;

  friend class Catalog;

  DISALLOW_COPY_AND_ASSIGN(CatalogDatabase);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_DATABASE_HPP_
