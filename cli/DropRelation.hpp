/**
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
