/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
