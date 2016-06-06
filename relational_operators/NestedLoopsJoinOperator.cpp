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

#include "relational_operators/NestedLoopsJoinOperator.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::unique_ptr;
using std::vector;

namespace quickstep {

void NestedLoopsJoinOperator::feedInputBlocks(const relation_id rel_id,
                                              std::vector<block_id> *partially_filled_blocks) {
  for (std::vector<block_id>::const_iterator it = partially_filled_blocks->begin();
       it != partially_filled_blocks->end();
       ++it) {
    feedInputBlock(*it, rel_id);
  }
}

void NestedLoopsJoinOperator::feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) {
  if (input_relation_id == left_input_relation_.getID()) {
    left_relation_block_ids_.push_back(input_block_id);
  } else if (input_relation_id == right_input_relation_.getID()) {
    right_relation_block_ids_.push_back(input_block_id);
  } else {
    FATAL_ERROR("The input block sent to the NestedLoopsJoinOperator belongs "
                "to a different relation than the left and right relations");
  }
}

bool NestedLoopsJoinOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  if (left_relation_is_stored_ && right_relation_is_stored_) {
    // Make sure we generate workorders only once.
    if (!all_workorders_generated_) {
      for (const block_id left_block_id : left_relation_block_ids_) {
        for (const block_id right_block_id : right_relation_block_ids_) {
          container->addNormalWorkOrder(
              new NestedLoopsJoinWorkOrder(left_input_relation_,
                                           right_input_relation_,
                                           left_block_id,
                                           right_block_id,
                                           query_context->getPredicate(join_predicate_index_),
                                           query_context->getScalarGroup(selection_index_),
                                           query_id_,
                                           query_context->getInsertDestination(output_destination_index_),
                                           storage_manager),
              op_index_);
        }
      }
      all_workorders_generated_ = true;
    }
    return all_workorders_generated_;
  } else if (!(left_relation_is_stored_ || right_relation_is_stored_)) {
    // Both relations are not stored.
    std::vector<block_id>::size_type new_left_blocks
        = left_relation_block_ids_.size() - num_left_workorders_generated_;
    std::vector<block_id>::size_type new_right_blocks
        = right_relation_block_ids_.size() - num_right_workorders_generated_;

    std::size_t new_workorders = 0;
    if (new_left_blocks > 0 && new_right_blocks > 0) {
      // Blocks added to both left and right relations.
      // First generate (left + new_left_blocks) * (new_right_blocks).
      new_workorders = getAllWorkOrdersHelperBothNotStored(container,
                                                           query_context,
                                                           storage_manager,
                                                           0,
                                                           left_relation_block_ids_.size(),
                                                           num_right_workorders_generated_,
                                                           right_relation_block_ids_.size());

      // Now generate new_left_blocks * (right).
      new_workorders += getAllWorkOrdersHelperBothNotStored(container,
                                                            query_context,
                                                            storage_manager,
                                                            num_left_workorders_generated_,
                                                            left_relation_block_ids_.size(),
                                                            0,
                                                            num_right_workorders_generated_);
    } else if (new_left_blocks == 0 && new_right_blocks > 0) {
      // Only new right blocks are added. Generate left * new_right_blocks.
      new_workorders = getAllWorkOrdersHelperBothNotStored(container,
                                                           query_context,
                                                           storage_manager,
                                                           0,
                                                           left_relation_block_ids_.size(),
                                                           num_right_workorders_generated_,
                                                           right_relation_block_ids_.size());
    } else if (new_left_blocks > 0 && new_right_blocks == 0) {
      // Generate new_left_blocks * right
      new_workorders = getAllWorkOrdersHelperBothNotStored(container,
                                                           query_context,
                                                           storage_manager,
                                                           num_left_workorders_generated_,
                                                           left_relation_block_ids_.size(),
                                                           0,
                                                           right_relation_block_ids_.size());
    }
    if (new_workorders > 0) {
      num_left_workorders_generated_ = left_relation_block_ids_.size();
      num_right_workorders_generated_ = right_relation_block_ids_.size();
    }
    return done_feeding_left_relation_ && done_feeding_right_relation_;
  } else {
    // Only one relation is a stored relation.
    return getAllWorkOrdersHelperOneStored(container, query_context, storage_manager);
  }
}

std::size_t NestedLoopsJoinOperator::getAllWorkOrdersHelperBothNotStored(WorkOrdersContainer *container,
                                                                         QueryContext *query_context,
                                                                         StorageManager *storage_manager,
                                                                         std::vector<block_id>::size_type left_min,
                                                                         std::vector<block_id>::size_type left_max,
                                                                         std::vector<block_id>::size_type right_min,
                                                                         std::vector<block_id>::size_type right_max) {
  DCHECK(!(left_relation_is_stored_ || right_relation_is_stored_));
  DCHECK(left_min <= left_max);
  DCHECK(right_min <= right_max);

  for (std::vector<block_id>::size_type left_index = left_min;
       left_index < left_max;
       ++left_index) {
    for (std::vector<block_id>::size_type right_index = right_min;
         right_index < right_max;
         ++right_index) {
      container->addNormalWorkOrder(
          new NestedLoopsJoinWorkOrder(left_input_relation_,
                                       right_input_relation_,
                                       left_relation_block_ids_[left_index],
                                       right_relation_block_ids_[right_index],
                                       query_context->getPredicate(join_predicate_index_),
                                       query_context->getScalarGroup(selection_index_),
                                       query_id_,
                                       query_context->getInsertDestination(output_destination_index_),
                                       storage_manager),
          op_index_);
    }
  }
  // Return the number of workorders produced.
  return (left_max - left_min) * (right_max - right_min);
}

bool NestedLoopsJoinOperator::getAllWorkOrdersHelperOneStored(WorkOrdersContainer *container,
                                                              QueryContext *query_context,
                                                              StorageManager *storage_manager) {
  DCHECK(left_relation_is_stored_ ^ right_relation_is_stored_);
  DCHECK(query_context != nullptr);

  const Predicate *join_predicate = query_context->getPredicate(join_predicate_index_);
  const vector<unique_ptr<const Scalar>> &selection =
      query_context->getScalarGroup(selection_index_);
  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);

  if (left_relation_is_stored_) {
    for (std::vector<block_id>::size_type right_index = num_right_workorders_generated_;
         right_index < right_relation_block_ids_.size();
         ++right_index) {
      for (const block_id left_block_id : left_relation_block_ids_) {
        container->addNormalWorkOrder(
            new NestedLoopsJoinWorkOrder(
                left_input_relation_,
                right_input_relation_,
                left_block_id,
                right_relation_block_ids_[right_index],
                join_predicate,
                selection,
                query_id_,
                output_destination,
                storage_manager),
            op_index_);
      }
    }
    num_right_workorders_generated_ = right_relation_block_ids_.size();
    return done_feeding_right_relation_;
  } else {
    for (std::vector<block_id>::size_type left_index = num_left_workorders_generated_;
         left_index < left_relation_block_ids_.size();
         ++left_index) {
      for (const block_id right_block_id : right_relation_block_ids_) {
        container->addNormalWorkOrder(
            new NestedLoopsJoinWorkOrder(left_input_relation_,
                                         right_input_relation_,
                                         left_relation_block_ids_[left_index],
                                         right_block_id,
                                         join_predicate,
                                         selection,
                                         query_id_,
                                         output_destination,
                                         storage_manager),
            op_index_);
      }
    }
    num_left_workorders_generated_ = left_relation_block_ids_.size();
    return done_feeding_left_relation_;
  }
}

template <bool LEFT_PACKED, bool RIGHT_PACKED>
void NestedLoopsJoinWorkOrder::executeHelper(const TupleStorageSubBlock &left_store,
                                             const TupleStorageSubBlock &right_store) {
  const relation_id left_input_relation_id = left_input_relation_.getID();
  const relation_id right_input_relation_id = right_input_relation_.getID();

  const tuple_id left_max_tid = left_store.getMaxTupleID();
  const tuple_id right_max_tid = right_store.getMaxTupleID();

  std::unique_ptr<ValueAccessor> left_accessor(left_store.createValueAccessor());
  std::unique_ptr<ValueAccessor> right_accessor(right_store.createValueAccessor());

  // Check each pair of tuples in the two blocks and build up a list of
  // matches.
  //
  // TODO(chasseur): Vectorize evaluation of this join predicate.
  std::vector<std::pair<tuple_id, tuple_id>> joined_tuple_ids;
  for (tuple_id left_tid = 0; left_tid <= left_max_tid; ++left_tid) {
    if (LEFT_PACKED || left_store.hasTupleWithID(left_tid)) {
      // For each tuple in the left block...
      for (tuple_id right_tid = 0; right_tid <= right_max_tid; ++right_tid) {
        if (RIGHT_PACKED || right_store.hasTupleWithID(right_tid)) {
          // For each tuple in the right block...
          if (join_predicate_->matchesForJoinedTuples(*left_accessor,
                                                      left_input_relation_id,
                                                      left_tid,
                                                      *right_accessor,
                                                      right_input_relation_id,
                                                      right_tid)) {
            joined_tuple_ids.emplace_back(left_tid, right_tid);
          }
        }
      }
    }
  }

  if (!joined_tuple_ids.empty()) {
    // TODO(chasseur): If all the output expressions are ScalarAttributes,
    // we could implement a similar fast-path to StorageBlock::selectSimple()
    // that avoids a copy.
    //
    // TODO(chasseur): Depending on the selectivity of the join predicate,
    // 'joined_tuple_ids' might be very long (up to the full cross-product of
    // tuples in a pair of blocks in the most extreme case). The temporary
    // result could then wind up taking a great deal of memory (significantly
    // more than what would fit in a typical StorageBlock). It may be
    // worthwhile to implement a heuristic where we only materialize temporary
    // results N tuples at a time instead of all at once (where N is some
    // number tuned high enough to good benefits from vectorized expression
    // evaluation and data movement, but low enough that temporary memory
    // requirements don't get out of hand).
    ColumnVectorsValueAccessor temp_result;
    for (vector<unique_ptr<const Scalar>>::const_iterator selection_cit = selection_.begin();
         selection_cit != selection_.end();
         ++selection_cit) {
      temp_result.addColumn((*selection_cit)->getAllValuesForJoin(left_input_relation_id,
                                                                  left_accessor.get(),
                                                                  right_input_relation_id,
                                                                  right_accessor.get(),
                                                                  joined_tuple_ids));
    }

    output_destination_->bulkInsertTuples(&temp_result);
  }
}

void NestedLoopsJoinWorkOrder::execute() {
  BlockReference left(
      storage_manager_->getBlock(left_block_id_, left_input_relation_));
  BlockReference right(
      storage_manager_->getBlock(right_block_id_, right_input_relation_));

  const TupleStorageSubBlock &left_store = left->getTupleStorageSubBlock();
  const TupleStorageSubBlock &right_store = right->getTupleStorageSubBlock();

  if (left_store.isPacked()) {
    if (right_store.isPacked()) {
      executeHelper<true, true>(left_store, right_store);
    } else {
      executeHelper<true, false>(left_store, right_store);
    }
  } else {
    if (right_store.isPacked()) {
      executeHelper<false, true>(left_store, right_store);
    } else {
      executeHelper<false, false>(left_store, right_store);
    }
  }
}

}  // namespace quickstep
