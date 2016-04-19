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

#include "relational_operators/HashJoinOperator.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

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
#include "storage/SubBlocksReference.hpp"
#include "storage/TupleReference.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::unique_ptr;
using std::vector;

namespace quickstep {

namespace {

DEFINE_bool(vector_based_joined_tuple_collector, true,
            "If true, use simple vector-based joined tuple collector in "
            "hash join, with a final sort pass to group joined tuple pairs "
            "by inner block. If false, use unordered_map based collector that "
            "keeps joined pairs grouped by inner block as they are found "
            "(this latter option has exhibited performance/scaling problems, "
            "particularly in NUMA contexts).");

// Functor passed to HashTable::getAllFromValueAccessor() to collect matching
// tuples from the inner relation. This version stores matching tuple ID pairs
// in an unordered_map keyed by inner block ID.
//
// NOTE(chasseur): Performance testing has shown that this particular
// implementation has problems scaling in a multisocket NUMA machine.
// Additional benchmarking revealed problems using the STL unordered_map class
// in a NUMA system (at least for the implementation in GNU libstdc++), even
// though instances of this class and the internal unordered_map are private to
// a single thread. Because of this, VectorBasedJoinedTupleCollector is used by
// default instead.
class MapBasedJoinedTupleCollector {
 public:
  MapBasedJoinedTupleCollector() {
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor,
                         const TupleReference &tref) {
    joined_tuples_[tref.block].emplace_back(tref.tuple, accessor.getCurrentPosition());
  }

  // Consolidation is a no-op for this version, but we provide this trivial
  // call so that MapBasedJoinedTupleCollector and
  // VectorBasedJoinedTupleCollector have the same interface and can both be
  // used in the templated HashInnerJoinWorkOrder::executeWithCollectorType() method.
  inline void consolidate() const {
  }

  // Get a mutable pointer to the collected map of joined tuple ID pairs. The
  // key is inner block_id, values are vectors of joined tuple ID pairs with
  // tuple ID from the inner block on the left and the outer block on the
  // right.
  inline std::unordered_map<block_id, std::vector<std::pair<tuple_id, tuple_id>>>*
      getJoinedTuples() {
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

// Compare std::pair instances based on their first element only.
template <typename PairT>
inline bool CompareFirst(const PairT &left, const PairT &right) {
  return left.first < right.first;
}

// Functor passed to HashTable::getAllFromValueAccessor() to collect matching
// tuples from the inner relation. This version stores inner block ID and pairs
// of joined tuple IDs in an unsorted vector, which should then be sorted with
// a call to consolidate() before materializing join output.
//
// NOTE(chasseur): Because of NUMA scaling issues for
// MapBasedJoinedTupleCollector noted above, this implementation is the
// default.
class VectorBasedJoinedTupleCollector {
 public:
  VectorBasedJoinedTupleCollector() {
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor,
                         const TupleReference &tref) {
    joined_tuples_.emplace_back(tref.block,
                                std::make_pair(tref.tuple, accessor.getCurrentPosition()));
  }

  // Sorts joined tuple pairs by inner block ID. Must be called before
  // getJoinedTuples().
  void consolidate() {
    if (joined_tuples_.empty()) {
      return;
    }

    // Sort joined tuple_id pairs by inner block_id.
    std::sort(joined_tuples_.begin(),
              joined_tuples_.end(),
              CompareFirst<std::pair<block_id, std::pair<tuple_id, tuple_id>>>);

    // Make a single vector of joined block_id pairs for each inner block for
    // compatibility with other join-related APIs.
    consolidated_joined_tuples_.emplace_back(joined_tuples_.front().first,
                                             std::vector<std::pair<tuple_id, tuple_id>>());

    for (const std::pair<block_id, std::pair<tuple_id, tuple_id>> &match_entry
         : joined_tuples_) {
      if (match_entry.first == consolidated_joined_tuples_.back().first) {
        consolidated_joined_tuples_.back().second.emplace_back(match_entry.second);
      } else {
        consolidated_joined_tuples_.emplace_back(
            match_entry.first,
            std::vector<std::pair<tuple_id, tuple_id>>(1, match_entry.second));
      }
    }
  }

  // Get a mutable pointer to the collected joined tuple ID pairs. The returned
  // vector has a single entry for each inner block where there are matching
  // joined tuples (the inner block's ID is the first element of the pair). The
  // second element of each pair is another vector consisting of pairs of
  // joined tuple IDs (tuple ID from inner block on the left, from outer block
  // on the right).
  inline std::vector<std::pair<const block_id, std::vector<std::pair<tuple_id, tuple_id>>>>*
      getJoinedTuples() {
    return &consolidated_joined_tuples_;
  }

 private:
  // Unsorted vector of join matches that is appended to by call operator().
  std::vector<std::pair<block_id, std::pair<tuple_id, tuple_id>>> joined_tuples_;

  // Joined tuples sorted by inner block_id. consolidate() populates this from
  // 'joined_tuples_'.
  std::vector<std::pair<const block_id, std::vector<std::pair<tuple_id, tuple_id>>>>
      consolidated_joined_tuples_;
};

class SemiAntiJoinTupleCollector {
 public:
  explicit SemiAntiJoinTupleCollector(const TupleStorageSubBlock &tuple_store) {
    filter_.reset(tuple_store.getExistenceMap());
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor) {
    filter_->set(accessor.getCurrentPosition(), false);
  }

  const TupleIdSequence* filter() const {
    return filter_.get();
  }

 private:
  std::unique_ptr<TupleIdSequence> filter_;
};

}  // namespace

bool HashJoinOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  switch (join_type_) {
    case JoinType::kInnerJoin:
      return getAllNonOuterJoinWorkOrders<HashInnerJoinWorkOrder>(
          container, query_context, storage_manager);
    case JoinType::kLeftSemiJoin:
      return getAllNonOuterJoinWorkOrders<HashSemiJoinWorkOrder>(
          container, query_context, storage_manager);
    case JoinType::kLeftAntiJoin:
      return getAllNonOuterJoinWorkOrders<HashAntiJoinWorkOrder>(
          container, query_context, storage_manager);
    default:
      LOG(FATAL) << "Unknown join type in HashJoinOperator::getAllWorkOrders()";
  }
}

template <class JoinWorkOrderClass>
bool HashJoinOperator::getAllNonOuterJoinWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager) {
  // We wait until the building of global hash table is complete.
  if (blocking_dependencies_met_) {
    DCHECK(query_context != nullptr);

    const Predicate *residual_predicate =
        query_context->getPredicate(residual_predicate_index_);
    const vector<unique_ptr<const Scalar>> &selection =
        query_context->getScalarGroup(selection_index_);
    InsertDestination *output_destination =
        query_context->getInsertDestination(output_destination_index_);
    JoinHashTable &hash_table =
        *(query_context->getJoinHashTable(hash_table_index_));

    if (attr_id_vector_.size() > 0) {
      DCHECK_EQ(attr_id_vector_.size(), bloom_filter_vector_.size());
      hash_table.attr_id_vector_ptr = &attr_id_vector_;
      hash_table.bloom_filter_vector_ptr = &bloom_filter_vector_;
    } else {
      hash_table.attr_id_vector_ptr = nullptr;
      hash_table.bloom_filter_vector_ptr = nullptr;
    }

    if (probe_relation_is_stored_) {
      if (!started_) {
        for (const block_id probe_block_id : probe_relation_block_ids_) {
          container->addNormalWorkOrder(
              new JoinWorkOrderClass(build_relation_,
                                     probe_relation_,
                                     join_key_attributes_,
                                     any_join_key_attributes_nullable_,
                                     probe_block_id,
                                     residual_predicate,
                                     selection,
                                     hash_table,
                                     output_destination,
                                     storage_manager),
              op_index_);
        }
        started_ = true;
      }
      return started_;
    } else {
      while (num_workorders_generated_ < probe_relation_block_ids_.size()) {
        container->addNormalWorkOrder(
            new JoinWorkOrderClass(build_relation_,
                                   probe_relation_,
                                   join_key_attributes_,
                                   any_join_key_attributes_nullable_,
                                   probe_relation_block_ids_[num_workorders_generated_],
                                   residual_predicate,
                                   selection,
                                   hash_table,
                                   output_destination,
                                   storage_manager),
            op_index_);
        ++num_workorders_generated_;
      }  // end while
      return done_feeding_input_relation_;
    }  // end else (probe_relation_is_stored_)
  }  // end if (blocking_dependencies_met_)
  return false;
}

void HashInnerJoinWorkOrder::execute() {
  if (FLAGS_vector_based_joined_tuple_collector) {
    executeWithCollectorType<VectorBasedJoinedTupleCollector>();
  } else {
    executeWithCollectorType<MapBasedJoinedTupleCollector>();
  }
}

template <typename CollectorT>
void HashInnerJoinWorkOrder::executeWithCollectorType() {
  BlockReference probe_block(
      storage_manager_->getBlock(block_id_, probe_relation_));
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();

  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());
  CollectorT collector;
  if (join_key_attributes_.size() == 1) {
    LOG(ERROR) << build_relation_.getName() << "," << probe_relation_.getName();
    hash_table_.getAllFromValueAccessor(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }
  collector.consolidate();

  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  for (std::pair<const block_id, std::vector<std::pair<tuple_id, tuple_id>>>
           &build_block_entry : *collector.getJoinedTuples()) {
    BlockReference build_block =
        storage_manager_->getBlock(build_block_entry.first, build_relation_);
    const TupleStorageSubBlock &build_store = build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(build_store.createValueAccessor());

    // Evaluate '*residual_predicate_', if any.
    //
    // TODO(chasseur): We might consider implementing true vectorized
    // evaluation for join predicates that are not equijoins (although in
    // general that would require evaluating and materializing some expressions
    // over the cross-product of all tuples in a pair of blocks in order to
    // evaluate the predicate). We could use a heuristic where we only do the
    // vectorized materialization and evaluation if the set of matches from the
    // hash join is below a reasonable threshold so that we don't blow up
    // temporary memory requirements to an unreasonable degree.
    if (residual_predicate_ != nullptr) {
      std::vector<std::pair<tuple_id, tuple_id>> filtered_matches;

      for (const std::pair<tuple_id, tuple_id> &hash_match
           : build_block_entry.second) {
        if (residual_predicate_->matchesForJoinedTuples(*build_accessor,
                                                        build_relation_id,
                                                        hash_match.first,
                                                        *probe_accessor,
                                                        probe_relation_id,
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
    for (vector<unique_ptr<const Scalar>>::const_iterator selection_cit = selection_.begin();
         selection_cit != selection_.end();
         ++selection_cit) {
      temp_result.addColumn((*selection_cit)->getAllValuesForJoin(build_relation_id,
                                                                  build_accessor.get(),
                                                                  probe_relation_id,
                                                                  probe_accessor.get(),
                                                                  build_block_entry.second));
    }

    // NOTE(chasseur): calling the bulk-insert method of InsertDestination once
    // for each pair of joined blocks incurs some extra overhead that could be
    // avoided by keeping checked-out MutableBlockReferences across iterations
    // of this loop, but that would get messy when combined with partitioning.
    output_destination_->bulkInsertTuples(&temp_result);
  }
}

void HashSemiJoinWorkOrder::execute() {
  if (residual_predicate_ == nullptr) {
    executeWithoutResidualPredicate();
  } else {
    executeWithResidualPredicate();
  }
}

void HashSemiJoinWorkOrder::executeWithResidualPredicate() {
  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();

  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());

  // TODO(harshad) - Make this function work with both types of collectors.

  // We collect all the matching probe relation tuples, as there's a residual
  // preidcate that needs to be applied after collecting these matches.
  MapBasedJoinedTupleCollector collector;
  if (join_key_attributes_.size() == 1) {
    hash_table_.getAllFromValueAccessor(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  // Get a filter for tuples in the given probe block.
  TupleIdSequence filter(probe_store.getMaxTupleID() + 1);
  filter.setRange(0, filter.length(), false);
  for (const std::pair<const block_id,
                       std::vector<std::pair<tuple_id, tuple_id>>>
           &build_block_entry : *collector.getJoinedTuples()) {
    // First element of the pair build_block_entry is the build block ID
    // 2nd element of the pair is a vector of pairs, in each of which -
    // 1st element is a matching tuple ID from the inner (build) relation.
    // 2nd element is a matching tuple ID from the outer (probe) relation.

    // Get the block from the build relation for this pair of matched tuples.
    BlockReference build_block =
        storage_manager_->getBlock(build_block_entry.first, build_relation_);
    const TupleStorageSubBlock &build_store =
        build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(
        build_store.createValueAccessor());
    for (const std::pair<tuple_id, tuple_id> &hash_match
         : build_block_entry.second) {
      // For each pair, 1st element is a tuple ID from the build relation in the
      // given build block, 2nd element is a tuple ID from the probe relation.
      if (filter.get(hash_match.second)) {
        // We have already found matches for this tuple that belongs to the
        // probe side, skip it.
        continue;
      }
      if (residual_predicate_->matchesForJoinedTuples(*build_accessor,
                                                      build_relation_id,
                                                      hash_match.first,
                                                      *probe_accessor,
                                                      probe_relation_id,
                                                      hash_match.second)) {
        filter.set(hash_match.second, true);
      }
    }
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_store.createValueAccessor(&filter));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end();
       ++selection_it) {
    temp_result.addColumn((*selection_it)->getAllValues(
        probe_accessor_with_filter.get(), &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

void HashSemiJoinWorkOrder::executeWithoutResidualPredicate() {
  DCHECK(residual_predicate_ == nullptr);

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();

  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());
  SemiAntiJoinTupleCollector collector(probe_store);
  // We collect all the probe relation tuples which have at least one matching
  // tuple in the build relation. As a performance optimization, the hash table
  // just looks for the existence of the probing key in the hash table and sets
  // the bit for the probing key in the collector. The optimization works
  // because there is no residual predicate in this case, unlike
  // executeWithResidualPredicate().
  if (join_key_attributes_.size() == 1u) {
    // Call the collector to set the bit to 0 for every key without a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchNotFound(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    // Call the collector to set the bit to 0 for every key without a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchNotFoundCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_store.createValueAccessor(collector.filter()));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end(); ++selection_it) {
    temp_result.addColumn((*selection_it)->getAllValues(
        probe_accessor_with_filter.get(), &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

void HashAntiJoinWorkOrder::executeWithoutResidualPredicate() {
  DCHECK(residual_predicate_ == nullptr);

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();

  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());
  SemiAntiJoinTupleCollector collector(probe_store);
  // We probe the hash table to find the keys which have an entry in the
  // hash table.
  if (join_key_attributes_.size() == 1) {
    // Call the collector to set the bit to 0 for every key with a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchFound(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    // Call the collector to set the bit to 0 for every key with a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchFoundCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_store.createValueAccessor(collector.filter()));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end(); ++selection_it) {
    temp_result.addColumn((*selection_it)->getAllValues(
        probe_accessor_with_filter.get(), &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

void HashAntiJoinWorkOrder::executeWithResidualPredicate() {
  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();

  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());
  // TODO(harshad) - Make the following code work with both types of collectors.
  MapBasedJoinedTupleCollector collector;
  // We probe the hash table and get all the matches. Unlike
  // executeWithoutResidualPredicate(), we have to collect all the matching
  // tuples, because after this step we still have to evalute the residual
  // predicate.
  if (join_key_attributes_.size() == 1) {
    hash_table_.getAllFromValueAccessor(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  // Create a filter for all the tuples from the given probe block.
  std::unique_ptr<TupleIdSequence> filter(probe_store.getExistenceMap());
  for (const std::pair<const block_id, std::vector<std::pair<tuple_id, tuple_id>>>
           &build_block_entry : *collector.getJoinedTuples()) {
    // First element of the pair build_block_entry is the build block ID
    // 2nd element of the pair is a vector of pairs, in each of which -
    // 1st element is a matching tuple ID from the inner (build) relation.
    // 2nd element is a matching tuple ID from the outer (probe) relation.
    BlockReference build_block = storage_manager_->getBlock(build_block_entry.first,
                                                            build_relation_);
    const TupleStorageSubBlock &build_store = build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(build_store.createValueAccessor());
    for (const std::pair<tuple_id, tuple_id> &hash_match
         : build_block_entry.second) {
      if (!filter->get(hash_match.second)) {
        // We have already seen this tuple, skip it.
        continue;
      }
      if (residual_predicate_->matchesForJoinedTuples(*build_accessor,
                                                      build_relation_id,
                                                      hash_match.first,
                                                      *probe_accessor,
                                                      probe_relation_id,
                                                      hash_match.second)) {
        // Note that the filter marks a match as false, as needed by the anti
        // join definition.
        filter->set(hash_match.second, false);
      }
    }
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_store.createValueAccessor(filter.get()));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end();
       ++selection_it) {
    temp_result.addColumn((*selection_it)->getAllValues(probe_accessor_with_filter.get(),
                                                     &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

}  // namespace quickstep
