/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
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

#include "relational_operators/UpdateOperator.hpp"

#include <ostream>
#include <utility>

#include "catalog/CatalogDatabase.hpp"
#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "utility/Macros.hpp"

#include "tmb/message_bus.h"

#include "glog/logging.h"

namespace quickstep {

bool UpdateOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (blocking_dependencies_met_ && !started_) {
    for (const block_id input_block_id : input_blocks_) {
      container->addNormalWorkOrder(
          new UpdateWorkOrder(relation_.getID(),
                              relocation_destination_index_,
                              predicate_index_,
                              update_group_index_,
                              input_block_id,
                              op_index_,
                              foreman_client_id_,
                              bus_),
          op_index_);
    }
    started_ = true;
  }
  return started_;
}

void UpdateWorkOrder::execute(QueryContext *query_context,
                              CatalogDatabase *database,
                              StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  MutableBlockReference block(
      storage_manager->getBlockMutable(input_block_id_,
                                       *database->getRelationById(rel_id_)));

  InsertDestination *relocation_destination =
      query_context->getInsertDestination(relocation_destination_index_);
  DCHECK(relocation_destination != nullptr);

  StorageBlock::UpdateResult result =
      block->update(query_context->getUpdateGroup(update_group_index_),
                    query_context->getPredicate(predicate_index_),
                    relocation_destination);

  if (!result.indices_consistent) {
    LOG_WARNING("An UPDATE caused one or more IndexSubBlocks in StorageBlock "
                << BlockIdUtil::ToString(input_block_id_) << " to become inconsistent "
                << "(not enough space to index all tuples in block).");
  }

  // TODO(harshad): Stream the block ID only if the predicate returned at least
  // one match in the StorageBlock.
  ForemanMessage message(ForemanMessage::DataPipelineMessage(update_operator_index_,
                                                             input_block_id_,
                                                             rel_id_));
  TaggedMessage foreman_tagged_msg;
  foreman_tagged_msg.set_message(
      &message, sizeof(message), kDataPipelineMessage);

  tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(
          bus_,
          ClientIDMap::Instance()->getValue(),
          foreman_client_id_,
          std::move(foreman_tagged_msg));
  if (send_status != tmb::MessageBus::SendStatus::kOK) {
    LOG(FATAL) << "Message could not be sent from thread with TMB client "
        "ID " << ClientIDMap::Instance()->getValue() << " to Foreman with TMB "
        "client ID " << foreman_client_id_;
  }
}

}  // namespace quickstep
