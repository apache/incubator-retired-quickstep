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

#include "relational_operators/SaveBlocksOperator.hpp"

#include <vector>

#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool SaveBlocksOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) {
  while (num_workorders_generated_ < destination_block_ids_.size()) {
    container->addNormalWorkOrder(
        new SaveBlocksWorkOrder(
            destination_block_ids_[num_workorders_generated_],
            force_,
            storage_manager),
        op_index_);
    ++num_workorders_generated_;
  }
  return done_feeding_input_relation_;
}

void SaveBlocksOperator::feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) {
  destination_block_ids_.push_back(input_block_id);
}

void SaveBlocksWorkOrder::execute() {
  // It may happen that the block gets saved to disk as a result of an eviction,
  // before this invocation. In either case, we don't care about the return
  // value of saveBlockOrBlob.
  storage_manager_->saveBlockOrBlob(save_block_id_, force_);
}

}  // namespace quickstep
