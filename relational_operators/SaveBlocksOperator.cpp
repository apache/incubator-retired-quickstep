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

#include "relational_operators/SaveBlocksOperator.hpp"

#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

bool SaveBlocksOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    while (num_workorders_generated_[part_id] < destination_block_ids_[part_id].size()) {
      container->addNormalWorkOrder(
          new SaveBlocksWorkOrder(
              query_id_,
              part_id,
              destination_block_ids_[part_id][num_workorders_generated_[part_id]],
              force_,
              storage_manager),
          op_index_);
      ++num_workorders_generated_[part_id];
    }
  }
  return done_feeding_input_relation_;
}

bool SaveBlocksOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
    while (num_workorders_generated_[part_id] < destination_block_ids_[part_id].size()) {
      serialization::WorkOrder *proto = new serialization::WorkOrder;
      proto->set_work_order_type(serialization::SAVE_BLOCKS);
      proto->set_query_id(query_id_);
      proto->SetExtension(serialization::SaveBlocksWorkOrder::block_id,
                          destination_block_ids_[part_id][num_workorders_generated_[part_id]]);
      proto->SetExtension(serialization::SaveBlocksWorkOrder::force, force_);
      proto->SetExtension(serialization::SaveBlocksWorkOrder::partition_id, part_id);

      container->addWorkOrderProto(proto, op_index_);

      ++num_workorders_generated_[part_id];
    }
  }
  return done_feeding_input_relation_;
}

void SaveBlocksOperator::updateCatalogOnCompletion() {
  // Note(jianqiao): We need to reset the exactness flag whenever a stored
  // relation gets changed. Given the pre-condition that all the data manipulation
  // operations (insert, delete, update, copy) use this SaveBlocksOperator as a
  // common routine. It is valid to put the updating call here to minimize the
  // changes to other relational operators (e.g. InsertOperator, DeleteOperator,
  // TextScanOperator, SelectOperator, etc.).
  relation_->getStatisticsMutable()->setExactness(false);
}

void SaveBlocksWorkOrder::execute() {
  // It may happen that the block gets saved to disk as a result of an eviction,
  // before this invocation. In either case, we don't care about the return
  // value of saveBlockOrBlob.
  storage_manager_->saveBlockOrBlob(save_block_id_, force_);
}

}  // namespace quickstep
