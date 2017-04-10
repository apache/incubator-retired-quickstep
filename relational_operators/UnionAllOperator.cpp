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

#include "relational_operators/UnionAllOperator.hpp"

#include <cstddef>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

void UnionAllOperator::feedInputBlock(const block_id input_block_id,
                                      const relation_id input_relation_id,
                                      const partition_id part_id) {
  std::size_t index = relation_id_to_index_.at(input_relation_id);
  input_relations_block_ids_[index].push_back(input_block_id);
}

void UnionAllOperator::doneFeedingInputBlocks(const relation_id rel_id) {
  std::size_t relation_index = relation_id_to_index_.at(rel_id);
  DCHECK(still_feeding_.find(relation_index) != still_feeding_.end());
  still_feeding_.erase(relation_index);
  if (still_feeding_.size() == 0) {
    done_feeding_input_relation_ = true;
  }
}

void UnionAllOperator::addWorkOrdersSingleRelation(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    InsertDestination *output_destination,
    const std::size_t relation_index) {
  if (input_relations_are_stored_[relation_index]) {
    const std::vector<block_id> &all_blocks = input_relations_block_ids_.at(relation_index);
    for (const block_id input_block_id : all_blocks) {
      container->addNormalWorkOrder(
          new UnionAllWorkOrder(
              query_id_,
              input_relations_[relation_index],
              input_block_id,
              select_attribute_ids_[relation_index],
              output_destination,
              storage_manager),
          op_index_);
    }
  } else {
    std::size_t num_generated = num_workorders_generated_[relation_index];
    const std::vector<block_id> &all_blocks = input_relations_block_ids_[relation_index];
    while (num_generated < all_blocks .size()) {
      container->addNormalWorkOrder(
          new UnionAllWorkOrder(
              query_id_,
              input_relations_[relation_index],
              all_blocks[num_generated],
              select_attribute_ids_[relation_index],
              output_destination,
              storage_manager),
          op_index_);
      ++num_generated;
    }
    num_workorders_generated_[relation_index] = num_generated;
  }
}

bool UnionAllOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);

  if (!stored_generated_) {
    for (std::size_t relation_index = 0; relation_index < input_relations_.size(); ++relation_index) {
      if (input_relations_are_stored_[relation_index]) {
        addWorkOrdersSingleRelation(container,
                                    query_context,
                                    storage_manager,
                                    output_destination,
                                    relation_index);
      }
    }
    stored_generated_ = true;
  }

  for (std::size_t relation_index = 0; relation_index < input_relations_.size(); ++relation_index) {
    if (!input_relations_are_stored_[relation_index]) {
       addWorkOrdersSingleRelation(container,
                                   query_context,
                                   storage_manager,
                                   output_destination,
                                   relation_index);
    }
  }
  return stored_generated_ && done_feeding_input_relation_;
}

bool UnionAllOperator::getAllWorkOrderProtos(WorkOrderProtosContainer* container) {
  // TODO(tianrun): Add protobuf for UnionAllWorkOrder to support distributed mode.
  LOG(FATAL) << "UnionAllOperator is not supported in distributed mode yet.";
  return true;
}

void UnionAllWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(input_block_id_, *input_relation_));
  block->selectSimple(select_attribute_id_,
                      nullptr,
                      output_destination_);
}

}  // namespace quickstep
