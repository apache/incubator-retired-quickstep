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

#include "relational_operators/SortRunGenerationOperator.hpp"

#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "utility/SortConfiguration.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool SortRunGenerationOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    CatalogDatabase *catalog_database,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) {
  const SortConfiguration &sort_config = query_context->getSortConfig(sort_config_index_);
  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);

  if (input_relation_is_stored_) {
    // Input blocks are from a base relation.
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addNormalWorkOrder(
            new SortRunGenerationWorkOrder(input_relation_,
                                           input_block_id,
                                           sort_config,
                                           output_destination,
                                           storage_manager),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    // Input blocks are pipelined.
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new SortRunGenerationWorkOrder(
              input_relation_,
              input_relation_block_ids_[num_workorders_generated_],
              sort_config,
              output_destination,
              storage_manager),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

void SortRunGenerationWorkOrder::execute(QueryContext *query_context,
                                         CatalogDatabase *database,
                                         StorageManager *storage_manager) {
  BlockReference block(
      storage_manager_->getBlock(input_block_id_, input_relation_));

  OrderedTupleIdSequence sorted_sequence;

  // Sort and write the tuples in sorted order into output_destination.
  block->sort(sort_config_.getOrderByList(),
              sort_config_.getOrdering(),
              sort_config_.getNullOrdering(),
              &sorted_sequence,
              output_destination_);
}

}  // namespace quickstep
