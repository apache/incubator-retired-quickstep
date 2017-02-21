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

#include "relational_operators/BuildHashOperator.hpp"

#include <memory>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/HashTable.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleReference.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/EventProfiler.hpp"
#include "utility/lip_filter/LIPFilterBuilder.hpp"
#include "utility/lip_filter/LIPFilterUtil.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

namespace {

class TupleReferenceGenerator {
 public:
  explicit TupleReferenceGenerator(const block_id block)
      : block_(block) {
  }

  template <typename ValueAccessorT>
  inline TupleReference operator()(const ValueAccessorT &accessor) const {
    return TupleReference(block_, accessor.getCurrentPosition());
  }

 private:
  const block_id block_;
};

}  // namespace

bool BuildHashOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  if (input_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      JoinHashTable *hash_table = query_context->getJoinHashTable(hash_table_index_, part_id);
      for (const block_id block : input_relation_block_ids_[part_id]) {
        container->addNormalWorkOrder(
            new BuildHashWorkOrder(query_id_, input_relation_, join_key_attributes_, any_join_key_attributes_nullable_,
                                   part_id, block, hash_table, storage_manager,
                                   CreateLIPFilterBuilderHelper(lip_deployment_index_, query_context)),
            op_index_);
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      JoinHashTable *hash_table = query_context->getJoinHashTable(hash_table_index_, part_id);
      while (num_workorders_generated_[part_id] <
             input_relation_block_ids_[part_id].size()) {
        container->addNormalWorkOrder(
            new BuildHashWorkOrder(query_id_, input_relation_, join_key_attributes_, any_join_key_attributes_nullable_,
                                   part_id, input_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                   hash_table, storage_manager,
                                   CreateLIPFilterBuilderHelper(lip_deployment_index_, query_context)),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }
    return done_feeding_input_relation_;
  }
}

bool BuildHashOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (input_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      for (const block_id block : input_relation_block_ids_[part_id]) {
        container->addWorkOrderProto(createWorkOrderProto(block, part_id), op_index_);
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      while (num_workorders_generated_[part_id] < input_relation_block_ids_[part_id].size()) {
        container->addWorkOrderProto(
            createWorkOrderProto(input_relation_block_ids_[part_id][num_workorders_generated_[part_id]], part_id),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }
    return done_feeding_input_relation_;
  }
}

serialization::WorkOrder* BuildHashOperator::createWorkOrderProto(const block_id block, const partition_id part_id) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::BUILD_HASH);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::BuildHashWorkOrder::relation_id, input_relation_.getID());
  for (const attribute_id attr_id : join_key_attributes_) {
    proto->AddExtension(serialization::BuildHashWorkOrder::join_key_attributes, attr_id);
  }
  proto->SetExtension(serialization::BuildHashWorkOrder::any_join_key_attributes_nullable,
                      any_join_key_attributes_nullable_);
  proto->SetExtension(serialization::BuildHashWorkOrder::join_hash_table_index, hash_table_index_);
  proto->SetExtension(serialization::BuildHashWorkOrder::partition_id, part_id);
  proto->SetExtension(serialization::BuildHashWorkOrder::block_id, block);
  proto->SetExtension(serialization::BuildHashWorkOrder::lip_deployment_index, lip_deployment_index_);

  return proto;
}

void BuildHashWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(build_block_id_, input_relation_));

  TupleReferenceGenerator generator(build_block_id_);
  std::unique_ptr<ValueAccessor> accessor(block->getTupleStorageSubBlock().createValueAccessor());

  auto *container = simple_profiler.getContainer();

  // Build LIPFilters if enabled.
  if (lip_filter_builder_ != nullptr) {
    auto *event_lip = container->getEventLine("BuildLIP");
    event_lip->emplace_back();
    lip_filter_builder_->insertValueAccessor(accessor.get());
    accessor->beginIterationVirtual();
    event_lip->back().endEvent();
  }

  auto *event_hash = container->getEventLine("BuildHash");
  event_hash->emplace_back();
  HashTablePutResult result;
  if (join_key_attributes_.size() == 1) {
    result = hash_table_->putValueAccessor(accessor.get(),
                                           join_key_attributes_.front(),
                                           any_join_key_attributes_nullable_,
                                           &generator);
  } else {
    result = hash_table_->putValueAccessorCompositeKey(accessor.get(),
                                                       join_key_attributes_,
                                                       any_join_key_attributes_nullable_,
                                                       &generator);
  }
  event_hash->back().endEvent();

  CHECK(result == HashTablePutResult::kOK)
      << "Failed to add entries to join hash table.";
}

}  // namespace quickstep
