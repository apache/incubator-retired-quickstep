/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
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

#include "relational_operators/DropTableOperator.hpp"

#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool DropTableOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) {
  if (blocking_dependencies_met_ && !work_generated_) {
    work_generated_ = true;

    std::vector<block_id> relation_blocks(relation_.getBlocksSnapshot());

    // DropTableWorkOrder only drops blocks, if any.
    container->addNormalWorkOrder(
        new DropTableWorkOrder(std::move(relation_blocks)),
        op_index_);

    // TODO(zuyu): move the following code to a better place.
    const relation_id rel_id = relation_.getID();
    if (only_drop_blocks_) {
      database_->getRelationByIdMutable(rel_id)->clearBlocks();
    } else {
      database_->dropRelationById(rel_id);
    }
  }
  return work_generated_;
}

void DropTableWorkOrder::execute(QueryContext *query_context,
                                 CatalogDatabase *database,
                                 StorageManager *storage_manager) {
  DCHECK(storage_manager != nullptr);

  for (const block_id block : blocks_) {
    storage_manager->deleteBlockOrBlobFile(block);
  }
}

}  // namespace quickstep
