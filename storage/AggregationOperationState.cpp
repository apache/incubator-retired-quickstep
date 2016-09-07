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

#include "storage/AggregationOperationState.hpp"

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabaseLite.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/ExpressionFactories.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationHandleDistinct.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/AggregationOperationState.pb.h"
#include "storage/HashTable.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/HashTableFactory.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"

#include "glog/logging.h"

using std::unique_ptr;

namespace quickstep {

AggregationOperationState::AggregationOperationState(
    const CatalogRelationSchema &input_relation,
    const std::vector<const AggregateFunction *> &aggregate_functions,
    std::vector<std::vector<std::unique_ptr<const Scalar>>> &&arguments,
    std::vector<bool> &&is_distinct,
    std::vector<std::unique_ptr<const Scalar>> &&group_by,
    const Predicate *predicate,
    const std::size_t estimated_num_entries,
    const HashTableImplType hash_table_impl_type,
    const std::vector<HashTableImplType> &distinctify_hash_table_impl_types,
    StorageManager *storage_manager)
    : input_relation_(input_relation),
      predicate_(predicate),
      group_by_list_(std::move(group_by)),
      arguments_(std::move(arguments)),
      is_distinct_(std::move(is_distinct)),
      storage_manager_(storage_manager) {
  // Sanity checks: each aggregate has a corresponding list of arguments.
  DCHECK(aggregate_functions.size() == arguments_.size());

  // Get the types of GROUP BY expressions for creating HashTables below.
  std::vector<const Type *> group_by_types;
  for (const std::unique_ptr<const Scalar> &group_by_element : group_by_list_) {
    group_by_types.emplace_back(&group_by_element->getType());
  }

  std::vector<AggregationHandle *> group_by_handles;
  group_by_handles.clear();

  if (aggregate_functions.size() == 0) {
    // If there is no aggregation function, then it is a distinctify operation
    // on the group-by expressions.
    DCHECK_GT(group_by_list_.size(), 0u);

    handles_.emplace_back(new AggregationHandleDistinct());
    arguments_.push_back({});
    is_distinct_.emplace_back(false);
    group_by_hashtable_pool_.reset(new HashTablePool(estimated_num_entries,
                                                     hash_table_impl_type,
                                                     group_by_types,
                                                     {1},
                                                     handles_,
                                                     storage_manager));
  } else {
    // Set up each individual aggregate in this operation.
    std::vector<const AggregateFunction *>::const_iterator agg_func_it =
        aggregate_functions.begin();
    std::vector<std::vector<std::unique_ptr<const Scalar>>>::const_iterator
        args_it = arguments_.begin();
    std::vector<bool>::const_iterator is_distinct_it = is_distinct_.begin();
    std::vector<HashTableImplType>::const_iterator
        distinctify_hash_table_impl_types_it =
            distinctify_hash_table_impl_types.begin();
    std::vector<std::size_t> payload_sizes;
    for (; agg_func_it != aggregate_functions.end();
         ++agg_func_it, ++args_it, ++is_distinct_it) {
      // Get the Types of this aggregate's arguments so that we can create an
      // AggregationHandle.
      std::vector<const Type *> argument_types;
      for (const std::unique_ptr<const Scalar> &argument : *args_it) {
        argument_types.emplace_back(&argument->getType());
      }

      // Sanity checks: aggregate function exists and can apply to the specified
      // arguments.
      DCHECK(*agg_func_it != nullptr);
      DCHECK((*agg_func_it)->canApplyToTypes(argument_types));

      // Have the AggregateFunction create an AggregationHandle that we can use
      // to do actual aggregate computation.
      handles_.emplace_back((*agg_func_it)->createHandle(argument_types));

      if (!group_by_list_.empty()) {
        // Aggregation with GROUP BY: combined payload is partially updated in
        // the presence of DISTINCT.
        if (*is_distinct_it) {
          handles_.back()->blockUpdate();
        }
        group_by_handles.emplace_back(handles_.back());
        payload_sizes.emplace_back(group_by_handles.back()->getPayloadSize());
      } else {
        // Aggregation without GROUP BY: create a single global state.
        single_states_.emplace_back(handles_.back()->createInitialState());

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
        // See if all of this aggregate's arguments are attributes in the input
        // relation. If so, remember the attribute IDs so that we can do copy
        // elision when actually performing the aggregation.
        std::vector<attribute_id> local_arguments_as_attributes;
        local_arguments_as_attributes.reserve(args_it->size());
        for (const std::unique_ptr<const Scalar> &argument : *args_it) {
          const attribute_id argument_id =
              argument->getAttributeIdForValueAccessor();
          if (argument_id == -1) {
            local_arguments_as_attributes.clear();
            break;
          } else {
            DCHECK_EQ(input_relation_.getID(),
                      argument->getRelationIdForValueAccessor());
            local_arguments_as_attributes.push_back(argument_id);
          }
        }

        arguments_as_attributes_.emplace_back(
            std::move(local_arguments_as_attributes));
#endif
      }

      // Initialize the corresponding distinctify hash table if this is a
      // DISTINCT aggregation.
      if (*is_distinct_it) {
        std::vector<const Type *> key_types(group_by_types);
        key_types.insert(
            key_types.end(), argument_types.begin(), argument_types.end());
        // TODO(jianqiao): estimated_num_entries is quite inaccurate for
        // estimating
        // the number of entries in the distinctify hash table. We may estimate
        // for each distinct aggregation an estimated_num_distinct_keys value
        // during
        // query optimization, if it worths.
        distinctify_hashtables_.emplace_back(
            AggregationStateFastHashTableFactory::CreateResizable(
                *distinctify_hash_table_impl_types_it,
                key_types,
                estimated_num_entries,
                {0},
                {},
                storage_manager));
        ++distinctify_hash_table_impl_types_it;
      } else {
        distinctify_hashtables_.emplace_back(nullptr);
      }
    }

    if (!group_by_handles.empty()) {
      // Aggregation with GROUP BY: create a HashTable pool for per-group
      // states.
      group_by_hashtable_pool_.reset(new HashTablePool(estimated_num_entries,
                                                       hash_table_impl_type,
                                                       group_by_types,
                                                       payload_sizes,
                                                       group_by_handles,
                                                       storage_manager));
    }
  }
}

AggregationOperationState* AggregationOperationState::ReconstructFromProto(
    const serialization::AggregationOperationState &proto,
    const CatalogDatabaseLite &database,
    StorageManager *storage_manager) {
  DCHECK(ProtoIsValid(proto, database));

  // Rebuild contructor arguments from their representation in 'proto'.
  std::vector<const AggregateFunction *> aggregate_functions;
  std::vector<std::vector<std::unique_ptr<const Scalar>>> arguments;
  std::vector<bool> is_distinct;
  std::vector<HashTableImplType> distinctify_hash_table_impl_types;
  std::size_t distinctify_hash_table_impl_type_index = 0;
  for (int agg_idx = 0; agg_idx < proto.aggregates_size(); ++agg_idx) {
    const serialization::Aggregate &agg_proto = proto.aggregates(agg_idx);

    aggregate_functions.emplace_back(
        &AggregateFunctionFactory::ReconstructFromProto(agg_proto.function()));

    arguments.emplace_back();
    arguments.back().reserve(agg_proto.argument_size());
    for (int argument_idx = 0; argument_idx < agg_proto.argument_size();
         ++argument_idx) {
      arguments.back().emplace_back(ScalarFactory::ReconstructFromProto(
          agg_proto.argument(argument_idx), database));
    }

    is_distinct.emplace_back(agg_proto.is_distinct());

    if (agg_proto.is_distinct()) {
      distinctify_hash_table_impl_types.emplace_back(
          HashTableImplTypeFromProto(proto.distinctify_hash_table_impl_types(
              distinctify_hash_table_impl_type_index)));
      ++distinctify_hash_table_impl_type_index;
    }
  }

  std::vector<std::unique_ptr<const Scalar>> group_by_expressions;
  for (int group_by_idx = 0; group_by_idx < proto.group_by_expressions_size();
       ++group_by_idx) {
    group_by_expressions.emplace_back(ScalarFactory::ReconstructFromProto(
        proto.group_by_expressions(group_by_idx), database));
  }

  unique_ptr<Predicate> predicate;
  if (proto.has_predicate()) {
    predicate.reset(
        PredicateFactory::ReconstructFromProto(proto.predicate(), database));
  }

  return new AggregationOperationState(
      database.getRelationSchemaById(proto.relation_id()),
      aggregate_functions,
      std::move(arguments),
      std::move(is_distinct),
      std::move(group_by_expressions),
      predicate.release(),
      proto.estimated_num_entries(),
      HashTableImplTypeFromProto(proto.hash_table_impl_type()),
      distinctify_hash_table_impl_types,
      storage_manager);
}

bool AggregationOperationState::ProtoIsValid(
    const serialization::AggregationOperationState &proto,
    const CatalogDatabaseLite &database) {
  if (!proto.IsInitialized() ||
      !database.hasRelationWithId(proto.relation_id()) ||
      (proto.aggregates_size() < 0)) {
    return false;
  }

  std::size_t num_distinctify_hash_tables =
      proto.distinctify_hash_table_impl_types_size();
  std::size_t distinctify_hash_table_impl_type_index = 0;
  for (int i = 0; i < proto.aggregates_size(); ++i) {
    if (!AggregateFunctionFactory::ProtoIsValid(
            proto.aggregates(i).function())) {
      return false;
    }

    // TODO(chasseur): We may also want to check that the specified
    // AggregateFunction is applicable to the specified arguments, but that
    // requires partial deserialization and may be too heavyweight for this
    // method.
    for (int argument_idx = 0;
         argument_idx < proto.aggregates(i).argument_size();
         ++argument_idx) {
      if (!ScalarFactory::ProtoIsValid(
              proto.aggregates(i).argument(argument_idx), database)) {
        return false;
      }
    }

    if (proto.aggregates(i).is_distinct()) {
      if (distinctify_hash_table_impl_type_index >=
              num_distinctify_hash_tables ||
          !serialization::HashTableImplType_IsValid(
              proto.distinctify_hash_table_impl_types(
                  distinctify_hash_table_impl_type_index))) {
        return false;
      }
    }
  }

  for (int i = 0; i < proto.group_by_expressions_size(); ++i) {
    if (!ScalarFactory::ProtoIsValid(proto.group_by_expressions(i), database)) {
      return false;
    }
  }

  if (proto.group_by_expressions_size() > 0) {
    if (!proto.has_hash_table_impl_type() ||
        !serialization::HashTableImplType_IsValid(
            proto.hash_table_impl_type())) {
      return false;
    }
  }

  if (proto.has_predicate()) {
    if (!PredicateFactory::ProtoIsValid(proto.predicate(), database)) {
      return false;
    }
  }

  return true;
}

void AggregationOperationState::aggregateBlock(const block_id input_block,
                                               LIPFilterAdaptiveProber *lip_filter_adaptive_prober) {
  if (group_by_list_.empty()) {
    aggregateBlockSingleState(input_block);
  } else {
    aggregateBlockHashTable(input_block, lip_filter_adaptive_prober);
  }
}

void AggregationOperationState::finalizeAggregate(
    InsertDestination *output_destination) {
  if (group_by_list_.empty()) {
    finalizeSingleState(output_destination);
  } else {
    finalizeHashTable(output_destination);
  }
}

void AggregationOperationState::mergeSingleState(
    const std::vector<std::unique_ptr<AggregationState>> &local_state) {
  DEBUG_ASSERT(local_state.size() == single_states_.size());
  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    if (!is_distinct_[agg_idx]) {
      handles_[agg_idx]->mergeStates(*local_state[agg_idx],
                                     single_states_[agg_idx].get());
    }
  }
}

void AggregationOperationState::aggregateBlockSingleState(
    const block_id input_block) {
  // Aggregate per-block state for each aggregate.
  std::vector<std::unique_ptr<AggregationState>> local_state;

  BlockReference block(
      storage_manager_->getBlock(input_block, input_relation_));

  std::unique_ptr<TupleIdSequence> matches;
  if (predicate_ != nullptr) {
    std::unique_ptr<ValueAccessor> accessor(
        block->getTupleStorageSubBlock().createValueAccessor());
    matches.reset(block->getMatchesForPredicate(predicate_.get(), matches.get()));
  }

  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    const std::vector<attribute_id> *local_arguments_as_attributes = nullptr;
#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
    // If all arguments are attributes of the input relation, elide a copy.
    if (!arguments_as_attributes_[agg_idx].empty()) {
      local_arguments_as_attributes = &(arguments_as_attributes_[agg_idx]);
    }
#endif
    if (is_distinct_[agg_idx]) {
      // Call StorageBlock::aggregateDistinct() to put the arguments as keys
      // directly into the (threadsafe) shared global distinctify HashTable
      // for this aggregate.
      block->aggregateDistinct(*handles_[agg_idx],
                               arguments_[agg_idx],
                               local_arguments_as_attributes,
                               {}, /* group_by */
                               matches.get(),
                               distinctify_hashtables_[agg_idx].get(),
                               nullptr /* reuse_group_by_vectors */);
      local_state.emplace_back(nullptr);
    } else {
      // Call StorageBlock::aggregate() to actually do the aggregation.
      local_state.emplace_back(block->aggregate(*handles_[agg_idx],
                                                arguments_[agg_idx],
                                                local_arguments_as_attributes,
                                                matches.get()));
    }
  }

  // Merge per-block aggregation states back with global state.
  mergeSingleState(local_state);
}

void AggregationOperationState::aggregateBlockHashTable(
    const block_id input_block,
    LIPFilterAdaptiveProber *lip_filter_adaptive_prober) {
  BlockReference block(
      storage_manager_->getBlock(input_block, input_relation_));

  // Apply the predicate first, then the LIPFilters, to generate a TupleIdSequence
  // as the existence map for the tuples.
  std::unique_ptr<TupleIdSequence> matches;
  if (predicate_ != nullptr) {
    matches.reset(block->getMatchesForPredicate(predicate_.get()));
  }
  if (lip_filter_adaptive_prober != nullptr) {
    std::unique_ptr<ValueAccessor> accessor(
        block->getTupleStorageSubBlock().createValueAccessor(matches.get()));
    matches.reset(lip_filter_adaptive_prober->filterValueAccessor(accessor.get()));
  }

  // This holds values of all the GROUP BY attributes so that the can be reused
  // across multiple aggregates (i.e. we only pay the cost of evaluatin the
  // GROUP BY expressions once).
  std::vector<std::unique_ptr<ColumnVector>> reuse_group_by_vectors;

  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    if (is_distinct_[agg_idx]) {
      // Call StorageBlock::aggregateDistinct() to insert the GROUP BY
      // expression
      // values and the aggregation arguments together as keys directly into the
      // (threadsafe) shared global distinctify HashTable for this aggregate.
      block->aggregateDistinct(*handles_[agg_idx],
                               arguments_[agg_idx],
                               nullptr, /* arguments_as_attributes */
                               group_by_list_,
                               matches.get(),
                               distinctify_hashtables_[agg_idx].get(),
                               &reuse_group_by_vectors);
    }
  }

  // Call StorageBlock::aggregateGroupBy() to aggregate this block's values
  // directly into the (threadsafe) shared global HashTable for this
  // aggregate.
  DCHECK(group_by_hashtable_pool_ != nullptr);
  AggregationStateHashTableBase *agg_hash_table =
      group_by_hashtable_pool_->getHashTableFast();
  DCHECK(agg_hash_table != nullptr);
  block->aggregateGroupBy(arguments_,
                          group_by_list_,
                          matches.get(),
                          agg_hash_table,
                          &reuse_group_by_vectors);
  group_by_hashtable_pool_->returnHashTable(agg_hash_table);
}

void AggregationOperationState::finalizeSingleState(
    InsertDestination *output_destination) {
  // Simply build up a Tuple from the finalized values for each aggregate and
  // insert it in '*output_destination'.
  std::vector<TypedValue> attribute_values;

  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    if (is_distinct_[agg_idx]) {
      single_states_[agg_idx].reset(
          handles_[agg_idx]->aggregateOnDistinctifyHashTableForSingle(
              *distinctify_hashtables_[agg_idx]));
    }

    attribute_values.emplace_back(
        handles_[agg_idx]->finalize(*single_states_[agg_idx]));
  }

  output_destination->insertTuple(Tuple(std::move(attribute_values)));
}

void AggregationOperationState::mergeGroupByHashTables(
    AggregationStateHashTableBase *src, AggregationStateHashTableBase *dst) {
  HashTableMergerFast merger(dst);
  (static_cast<FastHashTable<true, false, true, false> *>(src))
      ->forEachCompositeKeyFast(&merger);
}

void AggregationOperationState::finalizeHashTable(
    InsertDestination *output_destination) {
  // Each element of 'group_by_keys' is a vector of values for a particular
  // group (which is also the prefix of the finalized Tuple for that group).
  std::vector<std::vector<TypedValue>> group_by_keys;

  // TODO(harshad) - The merge phase may be slower when each hash table contains
  // large number of entries. We should find ways in which we can perform a
  // parallel merge.

  // TODO(harshad) - Find heuristics for faster merge, even in a single thread.
  // e.g. Keep merging entries from smaller hash tables to larger.

  auto *hash_tables = group_by_hashtable_pool_->getAllHashTables();
  if (hash_tables->size() > 1) {
    for (int hash_table_index = 0;
         hash_table_index < static_cast<int>(hash_tables->size() - 1);
         ++hash_table_index) {
      // Merge each hash table to the last hash table.
      mergeGroupByHashTables((*hash_tables)[hash_table_index].get(),
                             hash_tables->back().get());
    }
  }

  // Collect per-aggregate finalized values.
  std::vector<std::unique_ptr<ColumnVector>> final_values;
  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    if (is_distinct_[agg_idx]) {
      DCHECK(group_by_hashtable_pool_ != nullptr);
      auto *hash_tables = group_by_hashtable_pool_->getAllHashTables();
      DCHECK(hash_tables != nullptr);
      if (hash_tables->empty()) {
        // We may have a case where hash_tables is empty, e.g. no input blocks.
        // However for aggregateOnDistinctifyHashTableForGroupBy to work
        // correctly, we should create an empty group by hash table.
        AggregationStateHashTableBase *new_hash_table =
            group_by_hashtable_pool_->getHashTableFast();
        group_by_hashtable_pool_->returnHashTable(new_hash_table);
        hash_tables = group_by_hashtable_pool_->getAllHashTables();
      }
      DCHECK(hash_tables->back() != nullptr);
      AggregationStateHashTableBase *agg_hash_table = hash_tables->back().get();
      DCHECK(agg_hash_table != nullptr);
      handles_[agg_idx]->allowUpdate();
      handles_[agg_idx]->aggregateOnDistinctifyHashTableForGroupBy(
          *distinctify_hashtables_[agg_idx], agg_hash_table, agg_idx);
    }

    auto *hash_tables = group_by_hashtable_pool_->getAllHashTables();
    DCHECK(hash_tables != nullptr);
    if (hash_tables->empty()) {
      // We may have a case where hash_tables is empty, e.g. no input blocks.
      // However for aggregateOnDistinctifyHashTableForGroupBy to work
      // correctly, we should create an empty group by hash table.
      AggregationStateHashTableBase *new_hash_table =
          group_by_hashtable_pool_->getHashTableFast();
      group_by_hashtable_pool_->returnHashTable(new_hash_table);
      hash_tables = group_by_hashtable_pool_->getAllHashTables();
    }
    AggregationStateHashTableBase *agg_hash_table = hash_tables->back().get();
    DCHECK(agg_hash_table != nullptr);
    ColumnVector *agg_result_col = handles_[agg_idx]->finalizeHashTable(
        *agg_hash_table, &group_by_keys, agg_idx);
    if (agg_result_col != nullptr) {
      final_values.emplace_back(agg_result_col);
    }
  }

  // Reorganize 'group_by_keys' in column-major order so that we can make a
  // ColumnVectorsValueAccessor to bulk-insert results.
  //
  // TODO(chasseur): Shuffling around the GROUP BY keys like this is suboptimal
  // if there is only one aggregate. The need to do this should hopefully go
  // away when we work out storing composite structures for multiple aggregates
  // in a single HashTable.
  std::vector<std::unique_ptr<ColumnVector>> group_by_cvs;
  std::size_t group_by_element_idx = 0;
  for (const std::unique_ptr<const Scalar> &group_by_element : group_by_list_) {
    const Type &group_by_type = group_by_element->getType();
    if (NativeColumnVector::UsableForType(group_by_type)) {
      NativeColumnVector *element_cv =
          new NativeColumnVector(group_by_type, group_by_keys.size());
      group_by_cvs.emplace_back(element_cv);
      for (std::vector<TypedValue> &group_key : group_by_keys) {
        element_cv->appendTypedValue(
            std::move(group_key[group_by_element_idx]));
      }
    } else {
      IndirectColumnVector *element_cv =
          new IndirectColumnVector(group_by_type, group_by_keys.size());
      group_by_cvs.emplace_back(element_cv);
      for (std::vector<TypedValue> &group_key : group_by_keys) {
        element_cv->appendTypedValue(
            std::move(group_key[group_by_element_idx]));
      }
    }
    ++group_by_element_idx;
  }

  // Stitch together a ColumnVectorsValueAccessor combining the GROUP BY keys
  // and the finalized aggregates.
  ColumnVectorsValueAccessor complete_result;
  for (std::unique_ptr<ColumnVector> &group_by_cv : group_by_cvs) {
    complete_result.addColumn(group_by_cv.release());
  }
  for (std::unique_ptr<ColumnVector> &final_value_cv : final_values) {
    complete_result.addColumn(final_value_cv.release());
  }

  // Bulk-insert the complete result.
  output_destination->bulkInsertTuples(&complete_result);
}

void AggregationOperationState::destroyAggregationHashTablePayload() {
  if (group_by_hashtable_pool_ != nullptr) {
    auto all_hash_tables = group_by_hashtable_pool_->getAllHashTables();
    DCHECK(all_hash_tables != nullptr);
    for (std::size_t ht_index = 0; ht_index < all_hash_tables->size(); ++ht_index) {
      (*all_hash_tables)[ht_index]->destroyPayload();
    }
  }
}

}  // namespace quickstep
