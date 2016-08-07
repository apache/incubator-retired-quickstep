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

#include "cli/DropRelation.hpp"

#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

void DropRelation::Drop(const CatalogRelation &relation,
                        CatalogDatabase *database,
                        StorageManager *storage_manager) {
  std::vector<block_id> relation_blocks(relation.getBlocksSnapshot());
  for (const block_id relation_block_id : relation_blocks) {
    storage_manager->deleteBlockOrBlobFile(relation_block_id);
  }

  const relation_id rel_id = relation.getID();
  DEBUG_ASSERT(database->hasRelationWithId(rel_id));
  database->dropRelationById(rel_id);
}

}  // namespace quickstep
