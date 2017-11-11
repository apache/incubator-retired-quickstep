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

#include "relational_operators/BuildGeneralizedHashOperator.hpp"

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

bool BuildGeneralizedHashOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  // Get the build predicate from the query context if it exists.
  const Predicate *predicate =
      query_context->getPredicate(build_predicate_index_);

  if (input_relation_is_stored_ && second_input_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      JoinHashTable *hash_table = query_context->getJoinHashTable(hash_table_index_, part_id);
      JoinHashTable *second_hash_table = query_context->getJoinHashTable(second_hash_table_index_, part_id);
      for (const block_id block : input_relation_block_ids_[part_id]) {
        for (const block_id second_block : second_input_relation_block_ids_[part_id]) {
          container->addNormalWorkOrder(
              new BuildGeneralizedHashWorkOrder(query_id_, input_relation_, second_input_relation_,
                                                join_key_attributes_, second_join_key_attributes_,
                                                any_join_key_attributes_nullable_, any_second_join_key_attributes_nullable_,
                                                part_id, block, second_block, predicate, hash_table,
                                                second_hash_table, storage_manager,
                                                CreateLIPFilterBuilderHelper(lip_deployment_index_, query_context)),
              op_index_);
        }
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      JoinHashTable *hash_table = query_context->getJoinHashTable(hash_table_index_, part_id);
      JoinHashTable *second_hash_table = query_context->getJoinHashTable(second_hash_table_index_, part_id);
      while (num_workorders_generated_[part_id] <
             input_relation_block_ids_[part_id].size()) {
        container->addNormalWorkOrder(
            new BuildGeneralizedHashWorkOrder(query_id_, input_relation_, second_input_relation_, 
                                              join_key_attributes_, second_join_key_attributes_,
                                              any_join_key_attributes_nullable_, any_second_join_key_attributes_nullable_,
                                              part_id, input_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                              second_input_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                              predicate, hash_table, second_hash_table, storage_manager,
                                              CreateLIPFilterBuilderHelper(lip_deployment_index_, query_context)),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }
    return done_feeding_input_relation_;
  }
}

bool BuildGeneralizedHashOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  if (input_relation_is_stored_ && second_input_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      for (const block_id block : input_relation_block_ids_[part_id]) {
        for(const block_id second_block : second_input_relation_block_ids_[part_id]) {
         container->addWorkOrderProto(createWorkOrderProto(block, second_block, part_id), op_index_);
        }
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
      while (num_workorders_generated_[part_id] < input_relation_block_ids_[part_id].size()) {
        container->addWorkOrderProto(
            createWorkOrderProto(input_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                 second_input_relation_block_ids_[part_id][num_workorders_generated_[part_id]], part_id),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }
    return done_feeding_input_relation_;
  }
}

serialization::WorkOrder* BuildGeneralizedHashOperator::createWorkOrderProto(const block_id block, const block_id second_block, const partition_id part_id) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::BUILD_GENERALIZED_HASH);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::relation_id, input_relation_.getID());
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::second_relation_id, second_input_relation_.getID());
  for (const attribute_id attr_id : join_key_attributes_) {
    proto->AddExtension(serialization::BuildGeneralizedHashWorkOrder::join_key_attributes, attr_id);
  }
  for (const attribute_id attr_id : second_join_key_attributes_) {
    proto->AddExtension(serialization::BuildGeneralizedHashWorkOrder::second_join_key_attributes, attr_id);
  }
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::any_join_key_attributes_nullable,
                      any_join_key_attributes_nullable_);
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::any_second_join_key_attributes_nullable,
                      any_second_join_key_attributes_nullable_);
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::join_hash_table_index, hash_table_index_);
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::build_predicate_index, build_predicate_index_);
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::partition_id, part_id);
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::block_id, block);
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::second_block_id, second_block);
  proto->SetExtension(serialization::BuildGeneralizedHashWorkOrder::lip_deployment_index, lip_deployment_index_);

  for (const QueryContext::lip_filter_id lip_filter_index : lip_filter_indexes_) {
    proto->AddExtension(serialization::BuildGeneralizedHashWorkOrder::lip_filter_indexes, lip_filter_index);
  }

  return proto;
}

void BuildGeneralizedHashWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(build_block_id_, input_relation_));
  BlockReference second_block(
      storage_manager_->getBlock(second_build_block_id_, second_input_relation_));

  // Create the ValueAccessor to be initialized later.
  std::unique_ptr<ValueAccessor> accessor;
  std::unique_ptr<ValueAccessor> second_accessor;

  // If there is a build predicate, find the tuples that match it in the block.
  std::unique_ptr<TupleIdSequence> predicate_matches;
  if (predicate_ != nullptr) {
    predicate_matches.reset(
        block->getMatchesForPredicate(predicate_));
  }

  // Use the tuples from the build predicate to fill a ValueAccessor, else
  // initialize it to a default state for normal use.
  TupleReferenceGenerator generator(build_block_id_);
  if (predicate_ != nullptr) {
    accessor.reset(
        block->getTupleStorageSubBlock().createValueAccessor(predicate_matches.get()));
  } else {
    accessor.reset(block->getTupleStorageSubBlock().createValueAccessor());
  }

  TupleReferenceGenerator second_generator(second_build_block_id_);
  accessor.reset(second_block->getTupleStorageSubBlock().createValueAccessor());

  // Build LIPFilters if enabled.
  if (lip_filter_builder_ != nullptr) {
    lip_filter_builder_->insertValueAccessor(accessor.get());
    accessor->beginIterationVirtual();
  }

  HashTablePutResult result;
  HashTablePutResult second_result;
  if (join_key_attributes_.size() == 1) {
    result = hash_table_->putValueAccessor(accessor.get(),
                                           join_key_attributes_.front(),
                                           any_join_key_attributes_nullable_,
                                           &generator);
    second_result = second_hash_table_->putValueAccessor(second_accessor.get(),
                                                         second_join_key_attributes_.front(),
                                                         any_second_join_key_attributes_nullable_,
                                                         &second_generator);
  } else {
    result = hash_table_->putValueAccessorCompositeKey(accessor.get(),
                                                       join_key_attributes_,
                                                       any_join_key_attributes_nullable_,
                                                       &generator);
    second_result = second_hash_table_->putValueAccessorCompositeKey(second_accessor.get(),
                                                                     second_join_key_attributes_,
                                                                     any_second_join_key_attributes_nullable_,
                                                                     &second_generator);
  }

  CHECK(result == HashTablePutResult::kOK && second_result == HashTablePutResult::kOK)
      << "Failed to add entries to join hash table.";
}

}  // namespace quickstep
