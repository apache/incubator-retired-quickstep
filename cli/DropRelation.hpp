/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_CLI_DROP_RELATION_HPP_
#define QUICKSTEP_CLI_DROP_RELATION_HPP_

#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class CatalogRelation;
class StorageManager;

/** \addtogroup CLI
 *  @{
 */

/**
 * @brief All static methods which drop a relation.
 **/
class DropRelation {
 public:
  static void Drop(const CatalogRelation &relation,
                   CatalogDatabase *database,
                   StorageManager *storage_manager);

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  DropRelation();

  DISALLOW_COPY_AND_ASSIGN(DropRelation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_DROP_RELATION_HPP_
