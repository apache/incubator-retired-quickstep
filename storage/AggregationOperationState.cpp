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

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
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
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/AggregationOperationState.pb.h"
#include "storage/CollisionFreeVectorTable.hpp"
#include "storage/HashTable.pb.h"
#include "storage/HashTableBase.hpp"
#include "storage/HashTableFactory.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/PackedPayloadHashTable.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/SubBlocksReference.hpp"
#include "storage/ThreadPrivateCompactKeyHashTable.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/ColumnVectorCache.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"

#include "glog/logging.h"

using std::size_t;
using std::vector;

namespace quickstep {

namespace S = serialization;

AggregationOperationState::AggregationOperationState(
    const CatalogRelationSchema &input_relation,
    const std::vector<const AggregateFunction *> &aggregate_functions,
    std::vector<std::vector<std::unique_ptr<const Scalar>>> &&arguments,
    std::vector<bool> &&is_distinct,
    std::vector<std::unique_ptr<const Scalar>> &&group_by,
    const Predicate *predicate,
    const std::size_t estimated_num_entries,
    const bool is_partitioned,
    const std::size_t num_partitions,
    const HashTableImplType hash_table_impl_type,
    const std::vector<HashTableImplType> &distinctify_hash_table_impl_types,
    StorageManager *storage_manager,
    const size_t collision_free_vector_memory_size,
    const size_t collision_free_vector_num_init_partitions,
    const vector<size_t> &collision_free_vector_state_offsets)
    : input_relation_(input_relation),
      is_aggregate_collision_free_(
          group_by.empty() ? false
                           : hash_table_impl_type == HashTableImplType::kCollisionFreeVector),
      is_aggregate_partitioned_(is_partitioned),
      predicate_(predicate),
      is_distinct_(std::move(is_distinct)),
      all_distinct_(std::accumulate(is_distinct_.begin(), is_distinct_.end(),
                                    !is_distinct_.empty(), std::logical_and<bool>())),
      storage_manager_(storage_manager) {
  // Sanity checks: each aggregate has a corresponding list of arguments.
  DCHECK(aggregate_functions.size() == arguments.size());

  // Get the types of GROUP BY expressions for creating HashTables below.
  for (const std::unique_ptr<const Scalar> &group_by_element : group_by) {
    group_by_types_.emplace_back(&group_by_element->getType());
  }

  // Prepare group-by key ids and non-trivial expressions.
  for (std::unique_ptr<const Scalar> &group_by_element : group_by) {
    const attribute_id attr_id =
        group_by_element->getAttributeIdForValueAccessor();
    if (attr_id != kInvalidAttributeID) {
      group_by_key_ids_.emplace_back(ValueAccessorSource::kBase, attr_id);
    } else {
      group_by_key_ids_.emplace_back(ValueAccessorSource::kDerived,
                                     non_trivial_expressions_.size());
      non_trivial_expressions_.emplace_back(group_by_element.release());
    }
  }

  std::vector<AggregationHandle *> group_by_handles;

  // Set up each individual aggregate in this operation.
  std::vector<const AggregateFunction *>::const_iterator agg_func_it =
      aggregate_functions.begin();
  std::vector<std::vector<std::unique_ptr<const Scalar>>>::iterator
      args_it = arguments.begin();
  std::vector<bool>::const_iterator is_distinct_it = is_distinct_.begin();
  std::vector<HashTableImplType>::const_iterator
      distinctify_hash_table_impl_types_it =
          distinctify_hash_table_impl_types.begin();
  for (; agg_func_it != aggregate_functions.end();
       ++agg_func_it, ++args_it, ++is_distinct_it) {
    // Get the Types of this aggregate's arguments so that we can create an
    // AggregationHandle.
    std::vector<const Type *> argument_types;
    for (const std::unique_ptr<const Scalar> &argument : *args_it) {
      argument_types.emplace_back(&argument->getType());
    }

    // Prepare argument attribute ids and non-trivial expressions.
    std::vector<MultiSourceAttributeId> argument_ids;
    for (std::unique_ptr<const Scalar> &argument : *args_it) {
      const attribute_id attr_id =
          argument->getAttributeIdForValueAccessor();
      if (attr_id != kInvalidAttributeID) {
        argument_ids.emplace_back(ValueAccessorSource::kBase, attr_id);
      } else {
        argument_ids.emplace_back(ValueAccessorSource::kDerived,
                                  non_trivial_expressions_.size());
        non_trivial_expressions_.emplace_back(argument.release());
      }
    }
    argument_ids_.emplace_back(std::move(argument_ids));

    // Sanity checks: aggregate function exists and can apply to the specified
    // arguments.
    DCHECK(*agg_func_it != nullptr);
    DCHECK((*agg_func_it)->canApplyToTypes(argument_types));

    // Have the AggregateFunction create an AggregationHandle that we can use
    // to do actual aggregate computation.
    handles_.emplace_back((*agg_func_it)->createHandle(argument_types));

    if (!group_by_key_ids_.empty()) {
      group_by_handles.emplace_back(handles_.back().get());
    } else {
      // Aggregation without GROUP BY: create a single global state.
      single_states_.emplace_back(handles_.back()->createInitialState());
    }

    // Initialize the corresponding distinctify hash table if this is a
    // DISTINCT aggregation.
    if (*is_distinct_it) {
      std::vector<const Type *> key_types(group_by_types_);
      key_types.insert(
          key_types.end(), argument_types.begin(), argument_types.end());

      // TODO(jianqiao): estimated_num_entries is quite inaccurate for
      // estimating the number of entries in the distinctify hash table.
      // We need to estimate for each distinct aggregation an
      // estimated_num_distinct_keys value during query optimization.
      if (is_aggregate_partitioned_) {
        DCHECK(partitioned_group_by_hashtable_pool_ == nullptr);
        partitioned_group_by_hashtable_pool_.reset(
            new PartitionedHashTablePool(estimated_num_entries,
                                         num_partitions,
                                         *distinctify_hash_table_impl_types_it,
                                         key_types,
                                         {},
                                         storage_manager));
      } else {
        distinctify_hashtables_.emplace_back(
            AggregationStateHashTableFactory::CreateResizable(
                *distinctify_hash_table_impl_types_it,
                key_types,
                estimated_num_entries,
                {} /* handles */,
                storage_manager));

        // Combined payload is partially updated in the presence of DISTINCT.
        handles_.back()->blockUpdate();
      }
      ++distinctify_hash_table_impl_types_it;
    } else {
      distinctify_hashtables_.emplace_back(nullptr);
    }
  }

  if (!group_by_key_ids_.empty()) {
    // Aggregation with GROUP BY: create the hash table (pool).
    if (is_aggregate_collision_free_) {
      collision_free_hashtable_.reset(
          AggregationStateHashTableFactory::CreateResizable(
              hash_table_impl_type,
              group_by_types_,
              estimated_num_entries,
              group_by_handles,
              storage_manager,
              num_partitions,
              collision_free_vector_memory_size,
              collision_free_vector_num_init_partitions,
              collision_free_vector_state_offsets));
    } else if (is_aggregate_partitioned_) {
      if (all_distinct_) {
        DCHECK_EQ(1u, group_by_handles.size());
        DCHECK(partitioned_group_by_hashtable_pool_ != nullptr);
        group_by_hashtable_pool_.reset(
            new HashTablePool(estimated_num_entries,
                              hash_table_impl_type,
                              group_by_types_,
                              group_by_handles,
                              storage_manager));
      } else {
        partitioned_group_by_hashtable_pool_.reset(
            new PartitionedHashTablePool(estimated_num_entries,
                                         num_partitions,
                                         hash_table_impl_type,
                                         group_by_types_,
                                         group_by_handles,
                                         storage_manager));
      }
    } else {
      group_by_hashtable_pool_.reset(
          new HashTablePool(estimated_num_entries,
                            hash_table_impl_type,
                            group_by_types_,
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

  std::unique_ptr<Predicate> predicate;
  if (proto.has_predicate()) {
    predicate.reset(
        PredicateFactory::ReconstructFromProto(proto.predicate(), database));
  }

  size_t collision_free_vector_memory_size = 0;
  size_t collision_free_vector_num_init_partitions = 0;
  vector<size_t> collision_free_vector_state_offsets;
  if (proto.has_collision_free_vector_info()) {
    const serialization::CollisionFreeVectorInfo &collision_free_vector_info =
        proto.collision_free_vector_info();
    collision_free_vector_memory_size = collision_free_vector_info.memory_size();
    collision_free_vector_num_init_partitions = collision_free_vector_info.num_init_partitions();
    for (int i = 0; i < collision_free_vector_info.state_offsets_size(); ++i) {
      collision_free_vector_state_offsets.push_back(collision_free_vector_info.state_offsets(i));
    }
  }

  return new AggregationOperationState(
      database.getRelationSchemaById(proto.relation_id()),
      aggregate_functions,
      std::move(arguments),
      std::move(is_distinct),
      std::move(group_by_expressions),
      predicate.release(),
      proto.estimated_num_entries(),
      proto.is_partitioned(),
      proto.num_partitions(),
      HashTableImplTypeFromProto(proto.hash_table_impl_type()),
      distinctify_hash_table_impl_types,
      storage_manager,
      collision_free_vector_memory_size,
      collision_free_vector_num_init_partitions,
      collision_free_vector_state_offsets);
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

  const int group_by_expressions_size = proto.group_by_expressions_size();
  for (int i = 0; i < group_by_expressions_size; ++i) {
    if (!ScalarFactory::ProtoIsValid(proto.group_by_expressions(i), database)) {
      return false;
    }
  }

  if (group_by_expressions_size > 0) {
    if (!proto.has_hash_table_impl_type() ||
        !serialization::HashTableImplType_IsValid(
            proto.hash_table_impl_type())) {
      return false;
    }

    if (proto.hash_table_impl_type() == S::HashTableImplType::COLLISION_FREE_VECTOR) {
      if (!proto.has_collision_free_vector_info()) {
        return false;
      }

      const S::CollisionFreeVectorInfo &proto_collision_free_vector_info = proto.collision_free_vector_info();
      if (!proto_collision_free_vector_info.IsInitialized() ||
          proto_collision_free_vector_info.state_offsets_size() != group_by_expressions_size) {
        return false;
      }
    }
  }

  if (proto.has_predicate()) {
    if (!PredicateFactory::ProtoIsValid(proto.predicate(), database)) {
      return false;
    }
  }

  return true;
}

CollisionFreeVectorTable* AggregationOperationState
    ::getCollisionFreeVectorTable() const {
  return static_cast<CollisionFreeVectorTable *>(
      collision_free_hashtable_.get());
}

void AggregationOperationState::initialize(const std::size_t partition_id) {
  if (is_aggregate_collision_free_) {
    static_cast<CollisionFreeVectorTable *>(
        collision_free_hashtable_.get())->initialize(partition_id);
  } else {
    LOG(FATAL) << "AggregationOperationState::initialize() "
               << "is not supported by this aggregation";
  }
}

void AggregationOperationState::aggregateBlock(const block_id input_block,
                                               LIPFilterAdaptiveProber *lip_filter_adaptive_prober) {
  BlockReference block(
      storage_manager_->getBlock(input_block, input_relation_));
  const auto &tuple_store = block->getTupleStorageSubBlock();
  std::unique_ptr<ValueAccessor> base_accessor(tuple_store.createValueAccessor());
  std::unique_ptr<ValueAccessor> shared_accessor;
  ValueAccessor *accessor = base_accessor.get();

  // Apply the predicate first, then the LIPFilters, to generate a TupleIdSequence
  // as the existence map for the tuples.
  std::unique_ptr<TupleIdSequence> matches;
  if (predicate_ != nullptr) {
    matches.reset(block->getMatchesForPredicate(predicate_.get()));
    shared_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*matches));
    accessor = shared_accessor.get();
  }
  if (lip_filter_adaptive_prober != nullptr) {
    matches.reset(lip_filter_adaptive_prober->filterValueAccessor(accessor));
    shared_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*matches));
    accessor = shared_accessor.get();
  }

  std::unique_ptr<ColumnVectorsValueAccessor> non_trivial_results;
  if (!non_trivial_expressions_.empty()) {
    non_trivial_results.reset(new ColumnVectorsValueAccessor());
    SubBlocksReference sub_blocks_ref(tuple_store,
                                      block->getIndices(),
                                      block->getIndicesConsistent());
    ColumnVectorCache cv_cache;
    for (const auto &expression : non_trivial_expressions_) {
      non_trivial_results->addColumn(
          expression->getAllValues(accessor, &sub_blocks_ref, &cv_cache));
    }
  }

  accessor->beginIterationVirtual();

  ValueAccessorMultiplexer accessor_mux(accessor, non_trivial_results.get());
  if (group_by_key_ids_.empty()) {
    aggregateBlockSingleState(accessor_mux);
  } else {
    aggregateBlockHashTable(accessor_mux);
  }
}

void AggregationOperationState::aggregateBlockSingleState(
    const ValueAccessorMultiplexer &accessor_mux) {
  // Aggregate per-block state for each aggregate.
  std::vector<std::unique_ptr<AggregationState>> local_state;

  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    const auto &argument_ids = argument_ids_[agg_idx];
    const auto &handle = handles_[agg_idx];

    AggregationState *state = nullptr;
    if (is_distinct_[agg_idx]) {
      handle->insertValueAccessorIntoDistinctifyHashTable(
          argument_ids,
          {},
          accessor_mux,
          distinctify_hashtables_[agg_idx].get());
    } else {
      if (argument_ids.empty()) {
        // Special case. This is a nullary aggregate (i.e. COUNT(*)).
        ValueAccessor *base_accessor = accessor_mux.getBaseAccessor();
        DCHECK(base_accessor != nullptr);
        state = handle->accumulateNullary(base_accessor->getNumTuplesVirtual());
      } else {
        // Have the AggregationHandle actually do the aggregation.
        state = handle->accumulateValueAccessor(argument_ids, accessor_mux);
      }
    }
    local_state.emplace_back(state);
  }

  // Merge per-block aggregation states back with global state.
  mergeSingleState(local_state);
}

void AggregationOperationState::mergeSingleState(
    const std::vector<std::unique_ptr<AggregationState>> &local_state) {
  DCHECK_EQ(local_state.size(), single_states_.size());
  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    if (!is_distinct_[agg_idx]) {
      handles_[agg_idx]->mergeStates(*local_state[agg_idx],
                                     single_states_[agg_idx].get());
    }
  }
}

void AggregationOperationState::mergeGroupByHashTables(
    AggregationStateHashTableBase *src,
    AggregationStateHashTableBase *dst) const {
  HashTableMerger merger(static_cast<PackedPayloadHashTable *>(dst));
  static_cast<PackedPayloadHashTable *>(src)->forEachCompositeKey(&merger);
}

void AggregationOperationState::aggregateBlockHashTable(
    const ValueAccessorMultiplexer &accessor_mux) {
  if (is_aggregate_collision_free_) {
    aggregateBlockHashTableImplCollisionFree(accessor_mux);
  } else if (is_aggregate_partitioned_) {
    aggregateBlockHashTableImplPartitioned(accessor_mux);
  } else {
    aggregateBlockHashTableImplThreadPrivate(accessor_mux);
  }
}

void AggregationOperationState::aggregateBlockHashTableImplCollisionFree(
    const ValueAccessorMultiplexer &accessor_mux) {
  DCHECK(collision_free_hashtable_ != nullptr);

  collision_free_hashtable_->upsertValueAccessorCompositeKey(argument_ids_,
                                                             group_by_key_ids_,
                                                             accessor_mux);
}

void AggregationOperationState::aggregateBlockHashTableImplPartitioned(
    const ValueAccessorMultiplexer &accessor_mux) {
  DCHECK(partitioned_group_by_hashtable_pool_ != nullptr);

  std::vector<attribute_id> group_by_key_ids;
  for (const MultiSourceAttributeId &key_id : group_by_key_ids_) {
    DCHECK(key_id.source == ValueAccessorSource::kBase);
    group_by_key_ids.emplace_back(key_id.attr_id);
  }

  InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor_mux.getBaseAccessor(),
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    // TODO(jianqiao): handle the situation when keys in non_trivial_results
    const std::size_t num_partitions = partitioned_group_by_hashtable_pool_->getNumPartitions();

    // Compute the partitions for the tuple formed by group by values.
    std::vector<std::unique_ptr<TupleIdSequence>> partition_membership;
    partition_membership.resize(num_partitions);

    // Create a tuple-id sequence for each partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      partition_membership[partition].reset(
          new TupleIdSequence(accessor->getEndPosition()));
    }

    // Iterate over ValueAccessor for each tuple,
    // set a bit in the appropriate TupleIdSequence.
    while (accessor->next()) {
      // We need a unique_ptr because getTupleWithAttributes() uses "new".
      std::unique_ptr<Tuple> curr_tuple(
          accessor->getTupleWithAttributes(group_by_key_ids));
      const std::size_t curr_tuple_partition_id =
          curr_tuple->getTupleHash() % num_partitions;
      partition_membership[curr_tuple_partition_id]->set(accessor->getCurrentPosition());
    }

    // Aggregate each partition.
    for (std::size_t partition = 0; partition < num_partitions; ++partition) {
      std::unique_ptr<ValueAccessor> base_adapter(
          accessor->createSharedTupleIdSequenceAdapter(
              *partition_membership[partition]));

      std::unique_ptr<ValueAccessor> derived_adapter;
      if (accessor_mux.getDerivedAccessor() != nullptr) {
        derived_adapter.reset(
            accessor_mux.getDerivedAccessor()->createSharedTupleIdSequenceAdapterVirtual(
                *partition_membership[partition]));
      }

      ValueAccessorMultiplexer local_mux(base_adapter.get(), derived_adapter.get());
      if (all_distinct_) {
        DCHECK_EQ(1u, handles_.size());
        handles_.front()->insertValueAccessorIntoDistinctifyHashTable(
            argument_ids_.front(),
            group_by_key_ids_,
            local_mux,
            partitioned_group_by_hashtable_pool_->getHashTable(partition));
      } else {
        partitioned_group_by_hashtable_pool_->getHashTable(partition)
            ->upsertValueAccessorCompositeKey(argument_ids_,
                                              group_by_key_ids_,
                                              local_mux);
      }
    }
  });
}

void AggregationOperationState::aggregateBlockHashTableImplThreadPrivate(
    const ValueAccessorMultiplexer &accessor_mux) {
  DCHECK(group_by_hashtable_pool_ != nullptr);

  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    if (is_distinct_[agg_idx]) {
      handles_[agg_idx]->insertValueAccessorIntoDistinctifyHashTable(
          argument_ids_[agg_idx],
          group_by_key_ids_,
          accessor_mux,
          distinctify_hashtables_[agg_idx].get());
    }
  }

  if (!all_distinct_) {
    AggregationStateHashTableBase *agg_hash_table =
        group_by_hashtable_pool_->getHashTable();

    agg_hash_table->upsertValueAccessorCompositeKey(argument_ids_,
                                                    group_by_key_ids_,
                                                    accessor_mux);
    group_by_hashtable_pool_->returnHashTable(agg_hash_table);
  }
}

void AggregationOperationState::finalizeAggregate(
    const std::size_t partition_id,
    InsertDestination *output_destination) {
  if (group_by_key_ids_.empty()) {
    DCHECK_EQ(0u, partition_id);
    finalizeSingleState(output_destination);
  } else {
    finalizeHashTable(partition_id, output_destination);
  }
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

void AggregationOperationState::finalizeHashTable(
    const std::size_t partition_id,
    InsertDestination *output_destination) {
  if (is_aggregate_collision_free_) {
    finalizeHashTableImplCollisionFree(partition_id, output_destination);
  } else if (is_aggregate_partitioned_) {
    finalizeHashTableImplPartitioned(partition_id, output_destination);
  } else {
    DCHECK_EQ(0u, partition_id);
    DCHECK(group_by_hashtable_pool_ != nullptr);
    switch (group_by_hashtable_pool_->getHashTableImplType()) {
      case HashTableImplType::kSeparateChaining:
        finalizeHashTableImplThreadPrivatePackedPayload(output_destination);
        break;
      case HashTableImplType::kThreadPrivateCompactKey:
        finalizeHashTableImplThreadPrivateCompactKey(output_destination);
        break;
      default:
        LOG(FATAL) << "Unexpected hash table type in "
                   << "AggregationOperationState::finalizeHashTable()";
    }
  }
}

void AggregationOperationState::finalizeHashTableImplCollisionFree(
    const std::size_t partition_id,
    InsertDestination *output_destination) {
  std::vector<std::unique_ptr<ColumnVector>> final_values;
  CollisionFreeVectorTable *hash_table =
      static_cast<CollisionFreeVectorTable *>(collision_free_hashtable_.get());

  const std::size_t max_length =
      hash_table->getNumTuplesInFinalizationPartition(partition_id);
  ColumnVectorsValueAccessor complete_result;

  DCHECK_EQ(1u, group_by_types_.size());
  const Type *key_type = group_by_types_.front();
  DCHECK(NativeColumnVector::UsableForType(*key_type));

  std::unique_ptr<NativeColumnVector> key_cv(
      std::make_unique<NativeColumnVector>(*key_type, max_length));
  hash_table->finalizeKey(partition_id, key_cv.get());
  complete_result.addColumn(key_cv.release());

  for (std::size_t i = 0; i < handles_.size(); ++i) {
    const Type *result_type = handles_[i]->getResultType();
    DCHECK(NativeColumnVector::UsableForType(*result_type));

    std::unique_ptr<NativeColumnVector> result_cv(
        std::make_unique<NativeColumnVector>(*result_type, max_length));
    hash_table->finalizeState(partition_id, i, result_cv.get());
    complete_result.addColumn(result_cv.release());
  }

  // Bulk-insert the complete result.
  output_destination->bulkInsertTuples(&complete_result);
}

void AggregationOperationState::finalizeHashTableImplPartitioned(
    const std::size_t partition_id,
    InsertDestination *output_destination) {
  PackedPayloadHashTable *partitioned_hash_table =
      static_cast<PackedPayloadHashTable *>(
          partitioned_group_by_hashtable_pool_->getHashTable(partition_id));

  PackedPayloadHashTable *hash_table;
  if (all_distinct_) {
    DCHECK_EQ(1u, handles_.size());
    DCHECK(group_by_hashtable_pool_ != nullptr);

    hash_table = static_cast<PackedPayloadHashTable *>(
        group_by_hashtable_pool_->getHashTable());
    handles_.front()->aggregateOnDistinctifyHashTableForGroupBy(
        *partitioned_hash_table, 0, hash_table);
    partitioned_hash_table->destroyPayload();
  } else {
    hash_table = partitioned_hash_table;
  }

  // Each element of 'group_by_keys' is a vector of values for a particular
  // group (which is also the prefix of the finalized Tuple for that group).
  std::vector<std::vector<TypedValue>> group_by_keys;

  if (handles_.empty()) {
    const auto keys_retriever = [&group_by_keys](std::vector<TypedValue> &group_by_key,
                                                 const std::uint8_t *dumb_placeholder) -> void {
      group_by_keys.emplace_back(std::move(group_by_key));
    };

    hash_table->forEachCompositeKey(&keys_retriever);
  }

  // Collect per-aggregate finalized values.
  std::vector<std::unique_ptr<ColumnVector>> final_values;
  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    ColumnVector *agg_result_col = handles_[agg_idx]->finalizeHashTable(
        *hash_table, agg_idx, &group_by_keys);
    if (agg_result_col != nullptr) {
      final_values.emplace_back(agg_result_col);
    }
  }
  hash_table->destroyPayload();

  // Reorganize 'group_by_keys' in column-major order so that we can make a
  // ColumnVectorsValueAccessor to bulk-insert results.
  //
  // TODO(chasseur): Shuffling around the GROUP BY keys like this is suboptimal
  // if there is only one aggregate. The need to do this should hopefully go
  // away when we work out storing composite structures for multiple aggregates
  // in a single HashTable.
  std::vector<std::unique_ptr<ColumnVector>> group_by_cvs;
  std::size_t group_by_element_idx = 0;
  for (const Type *group_by_type : group_by_types_) {
    if (NativeColumnVector::UsableForType(*group_by_type)) {
      NativeColumnVector *element_cv =
          new NativeColumnVector(*group_by_type, group_by_keys.size());
      group_by_cvs.emplace_back(element_cv);
      for (std::vector<TypedValue> &group_key : group_by_keys) {
        element_cv->appendTypedValue(std::move(group_key[group_by_element_idx]));
      }
    } else {
      IndirectColumnVector *element_cv =
          new IndirectColumnVector(*group_by_type, group_by_keys.size());
      group_by_cvs.emplace_back(element_cv);
      for (std::vector<TypedValue> &group_key : group_by_keys) {
        element_cv->appendTypedValue(std::move(group_key[group_by_element_idx]));
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

void AggregationOperationState::finalizeHashTableImplThreadPrivatePackedPayload(
    InsertDestination *output_destination) {
  // TODO(harshad) - The merge phase may be slower when each hash table contains
  // large number of entries. We should find ways in which we can perform a
  // parallel merge.

  // TODO(harshad) - Find heuristics for faster merge, even in a single thread.
  // e.g. Keep merging entries from smaller hash tables to larger.

  std::unique_ptr<AggregationStateHashTableBase> final_hash_table_ptr;

  if (all_distinct_) {
    final_hash_table_ptr.reset(group_by_hashtable_pool_->getHashTable());
  } else {
    auto *hash_tables = group_by_hashtable_pool_->getAllHashTables();
    DCHECK(hash_tables != nullptr);
    if (hash_tables->empty()) {
      return;
    }

    final_hash_table_ptr.reset(hash_tables->back().release());
    for (std::size_t i = 0; i < hash_tables->size() - 1; ++i) {
      std::unique_ptr<AggregationStateHashTableBase> hash_table(
          hash_tables->at(i).release());
      mergeGroupByHashTables(hash_table.get(), final_hash_table_ptr.get());
      hash_table->destroyPayload();
    }
  }

  PackedPayloadHashTable *final_hash_table =
      static_cast<PackedPayloadHashTable *>(final_hash_table_ptr.get());

  // Each element of 'group_by_keys' is a vector of values for a particular
  // group (which is also the prefix of the finalized Tuple for that group).
  std::vector<std::vector<TypedValue>> group_by_keys;

  if (handles_.empty()) {
    const auto keys_retriever = [&group_by_keys](std::vector<TypedValue> &group_by_key,
                                                 const std::uint8_t *dumb_placeholder) -> void {
      group_by_keys.emplace_back(std::move(group_by_key));
    };

    final_hash_table->forEachCompositeKey(&keys_retriever);
  }


  // Collect per-aggregate finalized values.
  std::vector<std::unique_ptr<ColumnVector>> final_values;
  for (std::size_t agg_idx = 0; agg_idx < handles_.size(); ++agg_idx) {
    if (is_distinct_[agg_idx]) {
      handles_[agg_idx]->allowUpdate();
      handles_[agg_idx]->aggregateOnDistinctifyHashTableForGroupBy(
          *distinctify_hashtables_[agg_idx], agg_idx, final_hash_table);
    }

    ColumnVector *agg_result_col =
        handles_[agg_idx]->finalizeHashTable(
            *final_hash_table, agg_idx, &group_by_keys);
    DCHECK(agg_result_col != nullptr);

    final_values.emplace_back(agg_result_col);
  }
  final_hash_table->destroyPayload();

  // Reorganize 'group_by_keys' in column-major order so that we can make a
  // ColumnVectorsValueAccessor to bulk-insert results.
  //
  // TODO(chasseur): Shuffling around the GROUP BY keys like this is suboptimal
  // if there is only one aggregate. The need to do this should hopefully go
  // away when we work out storing composite structures for multiple aggregates
  // in a single HashTable.
  std::vector<std::unique_ptr<ColumnVector>> group_by_cvs;
  std::size_t group_by_element_idx = 0;
  for (const Type *group_by_type : group_by_types_) {
    if (NativeColumnVector::UsableForType(*group_by_type)) {
      NativeColumnVector *element_cv =
          new NativeColumnVector(*group_by_type, group_by_keys.size());
      group_by_cvs.emplace_back(element_cv);
      for (std::vector<TypedValue> &group_key : group_by_keys) {
        element_cv->appendTypedValue(
            std::move(group_key[group_by_element_idx]));
      }
    } else {
      IndirectColumnVector *element_cv =
          new IndirectColumnVector(*group_by_type, group_by_keys.size());
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

void AggregationOperationState::finalizeHashTableImplThreadPrivateCompactKey(
    InsertDestination *output_destination) {
  auto *hash_tables = group_by_hashtable_pool_->getAllHashTables();
  DCHECK(hash_tables != nullptr);
  if (hash_tables->empty()) {
    return;
  }

  // Merge all hash tables into one.
  std::unique_ptr<ThreadPrivateCompactKeyHashTable> final_hash_table(
      static_cast<ThreadPrivateCompactKeyHashTable*>(hash_tables->back().release()));
  for (std::size_t i = 0; i < hash_tables->size() - 1; ++i) {
    std::unique_ptr<AggregationStateHashTableBase> hash_table(
        hash_tables->at(i).release());
    final_hash_table->mergeFrom(
        static_cast<const ThreadPrivateCompactKeyHashTable&>(*hash_table));
  }

  ColumnVectorsValueAccessor complete_result;
  final_hash_table->finalize(&complete_result);

  // Bulk-insert the complete result.
  output_destination->bulkInsertTuples(&complete_result);
}

std::size_t AggregationOperationState::getMemoryConsumptionBytes() const {
  std::size_t memory = getMemoryConsumptionBytesHelper(distinctify_hashtables_);
  memory += getMemoryConsumptionBytesHelper(group_by_hashtables_);
  if (collision_free_hashtable_ != nullptr) {
    memory += collision_free_hashtable_->getMemoryConsumptionBytes();
  }
  if (group_by_hashtable_pool_ != nullptr) {
    memory += group_by_hashtable_pool_->getMemoryConsumptionPoolBytes();
  }
  if (partitioned_group_by_hashtable_pool_ != nullptr) {
    memory += partitioned_group_by_hashtable_pool_->getMemoryConsumptionPoolBytes();
  }
  return memory;
}

std::size_t AggregationOperationState::getMemoryConsumptionBytesHelper(
    const std::vector<std::unique_ptr<AggregationStateHashTableBase>>
        &hashtables) const {
  std::size_t memory = 0;
  for (std::size_t ht_id = 0; ht_id < hashtables.size(); ++ht_id) {
    if (hashtables[ht_id] != nullptr) {
      memory += hashtables[ht_id]->getMemoryConsumptionBytes();
    }
  }
  return memory;
}

}  // namespace quickstep
