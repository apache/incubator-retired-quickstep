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

#include "relational_operators/BuildHashOperator.hpp"

#include <memory>
#include <vector>

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

  JoinHashTable *hash_table = query_context->getJoinHashTable(hash_table_index_);
  if (input_relation_is_stored_) {
    if (!started_) {
      for (const block_id input_block_id : input_relation_block_ids_) {
        container->addNormalWorkOrder(
            new BuildHashWorkOrder(input_relation_,
                                   join_key_attributes_,
                                   any_join_key_attributes_nullable_,
                                   input_block_id,
                                   query_id_,
                                   hash_table,
                                   storage_manager),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new BuildHashWorkOrder(
              input_relation_,
              join_key_attributes_,
              any_join_key_attributes_nullable_,
              input_relation_block_ids_[num_workorders_generated_],
              query_id_,
              hash_table,
              storage_manager),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

void BuildHashWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(build_block_id_, input_relation_));

  TupleReferenceGenerator generator(build_block_id_);
  std::unique_ptr<ValueAccessor> accessor(block->getTupleStorageSubBlock().createValueAccessor());
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

  CHECK(result == HashTablePutResult::kOK)
      << "Failed to add entries to join hash table.";
}

}  // namespace quickstep
