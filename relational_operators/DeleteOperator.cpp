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

#include "relational_operators/DeleteOperator.hpp"

#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"

#include "tmb/message_bus.h"

#include "glog/logging.h"

namespace quickstep {

bool DeleteOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (relation_is_stored_) {
    // If relation_ is stored, iterate over the list of blocks in relation_.
    if (!started_) {
      for (const block_id input_block_id : relation_block_ids_) {
        container->addNormalWorkOrder(
            new DeleteWorkOrder(relation_.getID(),
                                predicate_index_,
                                input_block_id,
                                op_index_,
                                foreman_client_id_,
                                bus_),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new DeleteWorkOrder(relation_.getID(),
                              predicate_index_,
                              relation_block_ids_[num_workorders_generated_],
                              op_index_,
                              foreman_client_id_,
                              bus_),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

void DeleteWorkOrder::execute(QueryContext *query_context,
                              CatalogDatabase *database,
                              StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  MutableBlockReference block(
      storage_manager->getBlockMutable(input_block_id_,
                                       *database->getRelationById(rel_id_)));
  block->deleteTuples(query_context->getPredicate(predicate_index_));

  // TODO(harshad): Stream the block ID only if the predicate returned at least
  // one match in the StorageBlock.
  ForemanMessage message(ForemanMessage::DataPipelineMessage(delete_operator_index_,
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
