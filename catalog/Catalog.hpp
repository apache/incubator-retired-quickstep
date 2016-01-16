/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_CATALOG_CATALOG_HPP_
#define QUICKSTEP_CATALOG_CATALOG_HPP_

#include <exception>
#include <string>
#include <unordered_map>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief Exception thrown for a database name collision.
 **/
class DatabaseNameCollision : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param db_name The name of the database for which there was a collision.
   **/
  explicit DatabaseNameCollision(const std::string &db_name)
      : message_("DatabaseNameCollision: catalog already has a database ") {
    message_.append(db_name);
  }

  ~DatabaseNameCollision() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};


/**
 * @brief Exception thrown when a database with the specified name can't be
 *        found.
 **/
class DatabaseNameNotFound : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param dbName The database name which could not be found.
   **/
  explicit DatabaseNameNotFound(const std::string &db_name)
      : message_("DatabaseNameNotFound: catalog has no database named ") {
    message_.append(db_name);
  }

  ~DatabaseNameNotFound() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};


/**
 * @brief Exception thrown when a database with the specified ID can't be
 *        found.
 **/
class DatabaseIdNotFound : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param id The id which could not be found.
   **/
  explicit DatabaseIdNotFound(const database_id id)
      : message_("DatabaseIdNotFound: catalog has no database with ID ") {
    message_.append(std::to_string(id));
  }

  ~DatabaseIdNotFound() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};


/**
 * @brief The entire database catalog.
 **/
class Catalog {
 public:
  typedef std::unordered_map<std::string, CatalogDatabase*>::size_type size_type;
  typedef PtrVector<CatalogDatabase, true>::const_skip_iterator const_iterator;

  /**
   * @brief Construct an empty catalog.
   **/
  Catalog() {
  }

  /**
   * @brief Reconstruct a catalog from its serialized Protocol Buffer form.
   *
   * @param proto The Protocol Buffer serialization of a catalog,
   *        previously produced by getProto().
   **/
  explicit Catalog(const serialization::Catalog &proto);

  /**
   * @brief Destructor which recursively destroys children.
   **/
  ~Catalog() {
  }

  /**
   * @brief Check whether a database with the given name exists.
   *
   * @param db_name The name to check for.
   * @return Whether the database exists.
   **/
  bool hasDatabaseWithName(const std::string &db_name) const {
    return (db_map_.find(db_name) != db_map_.end());
  }

  /**
   * @brief Check whether a database with the given id exists.
   *
   * @param id The id to check for.
   * @return Whether the database exists.
   **/
  bool hasDatabaseWithId(const database_id id) const {
    return (idInRange(id) && !db_vec_.elementIsNull(id));
  }

  /**
   * @brief Get a database by name.
   *
   * @param db_name The name to search for.
   * @exception DatabaseNameNotFound No database with the given name exists.
   * @return The database with the given name.
   **/
  const CatalogDatabase& getDatabaseByName(const std::string &db_name) const;

  /**
   * @brief Get a mutable pointer to a database by name.
   *
   * @param db_name The name to search for.
   * @exception DatabaseNameNotFound No database with the given name exists.
   * @return The database with the given name.
   **/
  CatalogDatabase* getDatabaseByNameMutable(const std::string &db_name);

  /**
   * @brief Get a database by ID.
   *
   * @param id The id to search for.
   * @exception DatabaseIdNotFound No database with the given ID exists.
   * @return The database with the given ID.
   **/
  const CatalogDatabase& getDatabaseById(const database_id id) const {
    if (hasDatabaseWithId(id)) {
      return db_vec_[id];
    } else {
      throw DatabaseIdNotFound(id);
    }
  }

  /**
   * @brief Get a mutable pointer to a database by ID.
   *
   * @param id The id to search for.
   * @exception DatabaseIdNotFound No database with the given ID exists.
   * @return The database with the given ID.
   **/
  CatalogDatabase* getDatabaseByIdMutable(const database_id id) {
    if (hasDatabaseWithId(id)) {
      return &(db_vec_[id]);
    } else {
      throw DatabaseIdNotFound(id);
    }
  }

  /**
   * @brief Add a new database to the catalog. If the database already has an
   *        ID and/or parent, it will be overwritten.
   *
   * @param new_db The database to be added.
   * @exception DatabaseNameCollision A database with the same name as new_db
   *            is already present in the catalog.
   * @return The id assigned to the database.
   **/
  database_id addDatabase(CatalogDatabase *new_db);

  /**
   * @brief Serialize the catalog as Protocol Buffer.
   *
   * @return The Protocol Buffer representation of the catalog.
   **/
  serialization::Catalog getProto() const;

  /**
   * @brief Get the number of child databases.
   *
   * @return The number of child databases.
   **/
  size_type size() const {
    return db_map_.size();
  }

  /**
   * @brief Get an iterator at the beginning of the child databases.
   *
   * @return An iterator on the first child database.
   **/
  const_iterator begin() const {
    return db_vec_.begin_skip();
  }

  /**
   * @brief Get an iterator at one-past-the-end of the child databases.
   *
   * @return An iterator one-past-the-end of the child databases.
   **/
  const_iterator end() const {
    return db_vec_.end_skip();
  }

 private:
  /**
   * @brief Check whether a database_id is within the range of IDs contained
   *        in this Catalog.
   *
   * @param id The id to check.
   * @return true if id is in range, false otherwise.
   **/
  bool idInRange(const database_id id) const {
    return ((id >= 0)
            && (static_cast<PtrVector<CatalogDatabase>::size_type>(id) < db_vec_.size()));
  }

  /**
   * @brief Check whether a serialization::Catalog is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a Catalog,
   *        originally generated by getProto().
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::Catalog &proto);

  // A vector of databases, and NULL if it has dropped from the catalog.
  PtrVector<CatalogDatabase, true> db_vec_;

  // A map from a database name to a pointer to the database.
  std::unordered_map<std::string, CatalogDatabase*> db_map_;

  DISALLOW_COPY_AND_ASSIGN(Catalog);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_HPP_
