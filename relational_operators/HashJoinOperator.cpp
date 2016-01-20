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

#include "relational_operators/HashJoinOperator.hpp"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/HashTable.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleReference.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"

#include "glog/logging.h"

using std::unique_ptr;
using std::vector;

namespace quickstep {

namespace {

class JoinedTupleCollector {
 public:
  JoinedTupleCollector() {
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor,
                         const TupleReference &tref) {
    joined_tuples_[tref.block].emplace_back(tref.tuple, accessor.getCurrentPosition());
  }

  inline std::unordered_map<block_id, std::vector<std::pair<tuple_id, tuple_id>>>*
      getJoinedTupleMap() {
    return &joined_tuples_;
  }

 private:
  // NOTE(chasseur): It would also be possible to represent joined tuples for a
  // particular pair of blocks as a TupleIdSequence/BitVector over the
  // cross-product of all tuples from both blocks, but simply using pairs of
  // tuple-IDs is expected to be more space efficient if the result set is less
  // than 1/64 the cardinality of the cross-product.
  std::unordered_map<block_id, std::vector<std::pair<tuple_id, tuple_id>>> joined_tuples_;
};

}  // namespace

bool HashJoinOperator::getAllWorkOrders(WorkOrdersContainer *container) {
  // We wait until the building of global hash table is complete.
  if (blocking_dependencies_met_) {
    if (probe_relation_is_stored_) {
      if (!started_) {
        for (const block_id probe_block_id : probe_relation_block_ids_) {
          container->addNormalWorkOrder(
              new HashJoinWorkOrder(build_relation_.getID(),
                                    probe_relation_.getID(),
                                    join_key_attributes_,
                                    any_join_key_attributes_nullable_,
                                    output_destination_index_,
                                    hash_table_index_,
                                    residual_predicate_index_,
                                    selection_index_,
                                    probe_block_id),
              op_index_);
        }
        started_ = true;
      }
      return started_;
    } else {
      while (num_workorders_generated_ < probe_relation_block_ids_.size()) {
        container->addNormalWorkOrder(
            new HashJoinWorkOrder(
                build_relation_.getID(),
                probe_relation_.getID(),
                join_key_attributes_,
                any_join_key_attributes_nullable_,
                output_destination_index_,
                hash_table_index_,
                residual_predicate_index_,
                selection_index_,
                probe_relation_block_ids_[num_workorders_generated_]),
            op_index_);
        ++num_workorders_generated_;
      }  // end while
      return done_feeding_input_relation_;
    }  // end else (input_relation_is_stored is false)
  }  // end if (blocking_dependencies_met)
  return false;
}

void HashJoinWorkOrder::execute(QueryContext *query_context,
                                CatalogDatabase *database,
                                StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  JoinHashTable *hash_table = query_context->getJoinHashTable(hash_table_index_);
  DCHECK(hash_table != nullptr);

  BlockReference probe_block(
      storage_manager->getBlock(block_id_,
                                *database->getRelationById(probe_relation_id_)));
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();

  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());
  JoinedTupleCollector collector;
  if (join_key_attributes_.size() == 1) {
    hash_table->getAllFromValueAccessor(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table->getAllFromValueAccessorCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  const Predicate *residual_predicate = query_context->getPredicate(residual_predicate_index_);

  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);
  DCHECK(output_destination != nullptr);

  const vector<unique_ptr<const Scalar>> &selection =
      query_context->getScalarGroup(selection_index_);

  for (std::pair<const block_id, std::vector<std::pair<tuple_id, tuple_id>>>
           &build_block_entry : *collector.getJoinedTupleMap()) {
    BlockReference build_block(
        storage_manager->getBlock(build_block_entry.first,
                                  *database->getRelationById(build_relation_id_)));
    const TupleStorageSubBlock &build_store = build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(build_store.createValueAccessor());

    // Evaluate '*residual_predicate', if any.
    //
    // TODO(chasseur): We might consider implementing true vectorized
    // evaluation for join predicates that are not equijoins (although in
    // general that would require evaluating and materializing some expressions
    // over the cross-product of all tuples in a pair of blocks in order to
    // evaluate the predicate). We could use a heuristic where we only do the
    // vectorized materialization and evaluation if the set of matches from the
    // hash join is below a reasonable threshold so that we don't blow up
    // temporary memory requirements to an unreasonable degree.
    if (residual_predicate != nullptr) {
      std::vector<std::pair<tuple_id, tuple_id>> filtered_matches;

      for (const std::pair<tuple_id, tuple_id> &hash_match
           : build_block_entry.second) {
        if (residual_predicate->matchesForJoinedTuples(*build_accessor,
                                                       build_relation_id_,
                                                       hash_match.first,
                                                       *probe_accessor,
                                                       probe_relation_id_,
                                                       hash_match.second)) {
          filtered_matches.emplace_back(hash_match);
        }
      }

      build_block_entry.second = std::move(filtered_matches);
    }

    // TODO(chasseur): If all the output expressions are ScalarAttributes,
    // we could implement a similar fast-path to StorageBlock::selectSimple()
    // that avoids a copy.
    //
    // TODO(chasseur): See TODO in NestedLoopsJoinOperator.cpp about limiting
    // the size of materialized temporary results. In common usage, this
    // probably won't be an issue for hash-joins, but in the worst case a hash
    // join can still devolve into a cross-product.
    //
    // NOTE(chasseur): We could also create one big ColumnVectorsValueAccessor
    // and accumulate all the results across multiple block pairs into it
    // before inserting anything into output blocks, but this would require
    // some significant API extensions to the expressions system for a dubious
    // benefit (probably only a real performance win when there are very few
    // matching tuples in each individual inner block but very many inner
    // blocks with at least one match).
    ColumnVectorsValueAccessor temp_result;
    for (vector<unique_ptr<const Scalar>>::const_iterator selection_cit = selection.begin();
         selection_cit != selection.end();
         ++selection_cit) {
      temp_result.addColumn((*selection_cit)->getAllValuesForJoin(build_relation_id_,
                                                                  build_accessor.get(),
                                                                  probe_relation_id_,
                                                                  probe_accessor.get(),
                                                                  build_block_entry.second));
    }

    // NOTE(chasseur): calling the bulk-insert method of InsertDestination once
    // for each pair of joined blocks incurs some extra overhead that could be
    // avoided by keeping checked-out MutableBlockReferences across iterations
    // of this loop, but that would get messy when combined with partitioning.
    output_destination->bulkInsertTuples(&temp_result);
  }
}

}  // namespace quickstep
