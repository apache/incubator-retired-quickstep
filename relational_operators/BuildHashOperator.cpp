/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/BuildHashOperator.hpp"

#include <memory>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/HashTable.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleReference.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"

#include "glog/logging.h"

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

bool BuildHashOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addNormalWorkOrder(
            new BuildHashWorkOrder(input_relation_.getID(),
                                   join_key_attributes_,
                                   any_join_key_attributes_nullable_,
                                   input_block_id,
                                   hash_table_index_),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new BuildHashWorkOrder(
              input_relation_.getID(),
              join_key_attributes_,
              any_join_key_attributes_nullable_,
              input_relation_block_ids_[num_workorders_generated_],
              hash_table_index_),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

void BuildHashWorkOrder::execute(QueryContext *query_context,
                                 CatalogDatabase *database,
                                 StorageManager *storage_manager) {
  DCHECK(database != nullptr);
  DCHECK(query_context != nullptr);
  DCHECK(storage_manager != nullptr);

  JoinHashTable *hash_table = query_context->getJoinHashTable(hash_table_index_);
  DCHECK(hash_table != nullptr);

  BlockReference block(
      storage_manager->getBlock(build_block_id_,
                                *database->getRelationById(rel_id_)));
  TupleReferenceGenerator generator(build_block_id_);
  std::unique_ptr<ValueAccessor> accessor(block->getTupleStorageSubBlock().createValueAccessor());
  HashTablePutResult result;
  if (join_key_attributes_.size() == 1) {
    result = hash_table->putValueAccessor(accessor.get(),
                                          join_key_attributes_.front(),
                                          any_join_key_attributes_nullable_,
                                          &generator);
  } else {
    result = hash_table->putValueAccessorCompositeKey(accessor.get(),
                                                      join_key_attributes_,
                                                      any_join_key_attributes_nullable_,
                                                      &generator);
  }
  if (result != HashTablePutResult::kOK) {
    FATAL_ERROR("Failed to add entries to join hash table.");
  }
}

}  // namespace quickstep
