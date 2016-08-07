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

#ifndef QUICKSTEP_CATALOG_CATALOG_DATABASE_LITE_HPP_
#define QUICKSTEP_CATALOG_CATALOG_DATABASE_LITE_HPP_

#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief A base class that the catalog database implements for both the master
 *        Catalog, and the CatalogCache in the distributed version.
 *
 * @note This class only contains virtual methods related to the relation id and
 *       CatalogRelationSchema.
 **/
class CatalogDatabaseLite {
 public:
  /**
   * @brief Virtual destructor which recursively destroys children.
   **/
  virtual ~CatalogDatabaseLite() {
  }

  /**
   * @brief Get this database's ID.
   *
   * @return This database's ID.
   **/
  database_id getID() const {
    return id_;
  }

  /**
   * @brief Check whether a relation with the given id exists.
   *
   * @param id The id to check for.
   * @return Whether the relation exists.
   **/
  virtual bool hasRelationWithId(const relation_id id) const = 0;

  /**
   * @brief Get a relation schema by ID.
   *
   * @param id The id to search for.
   * @return The relation schema with the given ID.
   **/
  virtual const CatalogRelationSchema& getRelationSchemaById(const relation_id id) const = 0;

  /**
   * @brief Drop (delete) a relation by id.
   *
   * @param id The ID of the relation to drop.
   **/
  virtual void dropRelationById(const relation_id id) = 0;

 protected:
  /**
   * @brief Create a new database.
   *
   * @param parent The catalog this database belongs to.
   * @param name This database's name.
   * @param id This database's ID (defaults to -1, which means invalid/unset).
   **/
  explicit CatalogDatabaseLite(const database_id id = -1)
      : id_(id) {
  }

  // The database id in Catalog.
  database_id id_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CatalogDatabaseLite);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_DATABASE_LITE_HPP_
