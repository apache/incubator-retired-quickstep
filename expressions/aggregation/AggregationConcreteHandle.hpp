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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_CONCRETE_HANDLE_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_CONCRETE_HANDLE_HPP_

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/PackedPayloadHashTable.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "threading/SpinMutex.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class StorageManager;
class Type;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief An upserter class for modifying the destination hash table while
 *        merging two group by hash tables.
 **/
template <typename HandleT>
class HashTableStateUpserter {
 public:
  /**
   * @brief Constructor.
   *
   * @param handle The aggregation handle being used.
   * @param source_state The aggregation state in the source aggregation hash
   *        table. The corresponding state (for the same key) in the destination
   *        hash table will be upserted.
   **/
  HashTableStateUpserter(const HandleT &handle,
                         const std::uint8_t *source_state)
      : handle_(handle), source_state_(source_state) {}

  /**
   * @brief The operator for the functor required for the upsert.
   *
   * @param destination_state The aggregation state in the aggregation hash
   *        table that is being upserted.
   **/
  void operator()(std::uint8_t *destination_state) {
    handle_.mergeStates(source_state_, destination_state);
  }

 private:
  const HandleT &handle_;
  const std::uint8_t *source_state_;

  DISALLOW_COPY_AND_ASSIGN(HashTableStateUpserter);
};

/**
 * @brief The helper intermediate subclass of AggregationHandle that provides
 *        virtual method implementations as well as helper methods that are
 *        shared among all its subclasses.
 *
 * @note The reason that we have this intermediate class instead of putting
 *       everything inside AggregationHandle is to avoid cyclic dependency, e.g.
 *       when HashTable has to be used.
 **/
class AggregationConcreteHandle : public AggregationHandle {
 public:
  AggregationStateHashTableBase* createDistinctifyHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type *> &key_types,
      const std::size_t estimated_num_distinct_keys,
      StorageManager *storage_manager) const override;

  void insertValueAccessorIntoDistinctifyHashTable(
      const std::vector<MultiSourceAttributeId> &argument_ids,
      const std::vector<MultiSourceAttributeId> &key_ids,
      const ValueAccessorMultiplexer &accessor_mux,
      AggregationStateHashTableBase *distinctify_hash_table) const override;

  void blockUpdate() override {
    block_update_ = true;
  }

  void allowUpdate() override {
    block_update_ = false;
  }

 protected:
  explicit AggregationConcreteHandle(const AggregationID agg_id)
      : AggregationHandle(agg_id),
        block_update_(false) {}

  template <typename HandleT, typename StateT>
  StateT* aggregateOnDistinctifyHashTableForSingleUnaryHelper(
      const AggregationStateHashTableBase &distinctify_hash_table) const;

  template <typename HandleT>
  void aggregateOnDistinctifyHashTableForGroupByUnaryHelper(
      const AggregationStateHashTableBase &distinctify_hash_table,
      const std::size_t index,
      AggregationStateHashTableBase *hash_table) const;

  template <typename HandleT>
  ColumnVector* finalizeHashTableHelper(
      const Type &result_type,
      const AggregationStateHashTableBase &hash_table,
      const std::size_t index,
      std::vector<std::vector<TypedValue>> *group_by_keys) const;

  template <typename HandleT>
  inline TypedValue finalizeGroupInHashTable(
      const AggregationStateHashTableBase &hash_table,
      const std::size_t index,
      const std::vector<TypedValue> &group_key) const {
    const std::uint8_t *group_state =
        static_cast<const PackedPayloadHashTable &>(hash_table)
            .getSingleCompositeKey(group_key, index);
    DCHECK(group_state != nullptr)
        << "Could not find entry for specified group_key in HashTable";
    return static_cast<const HandleT *>(this)->finalizeHashTableEntry(group_state);
  }

  bool block_update_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AggregationConcreteHandle);
};

/**
 * @brief Templated helper class used to implement
 *        AggregationHandle::finalizeHashTable() by visiting each entry (i.e.
 *        GROUP) in a HashTable, finalizing the aggregation for the GROUP, and
 *        collecting the GROUP BY key values and the final aggregate values in
 *        a ColumnVector.
 **/
template <typename HandleT, typename ColumnVectorT>
class HashTableAggregateFinalizer {
 public:
  HashTableAggregateFinalizer(
      const HandleT &handle,
      std::vector<std::vector<TypedValue>> *group_by_keys,
      ColumnVectorT *output_column_vector)
      : handle_(handle),
        group_by_keys_(group_by_keys),
        output_column_vector_(output_column_vector) {}

  inline void operator()(const std::vector<TypedValue> &group_by_key,
                         const std::uint8_t *byte_ptr) {
    group_by_keys_->emplace_back(group_by_key);
    output_column_vector_->appendTypedValue(
        handle_.finalizeHashTableEntry(byte_ptr));
  }

 private:
  const HandleT &handle_;
  std::vector<std::vector<TypedValue>> *group_by_keys_;
  ColumnVectorT *output_column_vector_;
};

/** @} */

// ----------------------------------------------------------------------------
// Implementations of templated methods follow:

template <typename HandleT, typename StateT>
StateT* AggregationConcreteHandle::
    aggregateOnDistinctifyHashTableForSingleUnaryHelper(
        const AggregationStateHashTableBase &distinctify_hash_table) const {
  const HandleT &handle = static_cast<const HandleT &>(*this);
  StateT *state = static_cast<StateT *>(createInitialState());

  // A lambda function which will be called on each key from the distinctify
  // hash table.
  const auto aggregate_functor = [&handle, &state](
      const TypedValue &key, const std::uint8_t *dumb_placeholder) {
    // For each (unary) key in the distinctify hash table, aggregate the key
    // into "state".
    handle.iterateUnaryInl(state, key);
  };

  const auto &hash_table =
      static_cast<const PackedPayloadHashTable &>(distinctify_hash_table);
  // Invoke the lambda function "aggregate_functor" on each key from the
  // distinctify hash table.
  hash_table.forEach(&aggregate_functor);

  return state;
}

template <typename HandleT>
void AggregationConcreteHandle::
    aggregateOnDistinctifyHashTableForGroupByUnaryHelper(
        const AggregationStateHashTableBase &distinctify_hash_table,
        const std::size_t index,
        AggregationStateHashTableBase *aggregation_hash_table) const {
  const HandleT &handle = static_cast<const HandleT &>(*this);
  PackedPayloadHashTable *target_hash_table =
      static_cast<PackedPayloadHashTable *>(aggregation_hash_table);

  // A lambda function which will be called on each key-value pair from the
  // distinctify hash table.
  const auto aggregate_functor = [&handle, &target_hash_table, &index](
      std::vector<TypedValue> &key, const std::uint8_t *dumb_placeholder) {
    // For each (composite) key vector in the distinctify hash table with size N.
    // The first N-1 entries are GROUP BY columns and the last entry is the
    // argument to be aggregated on.
    const TypedValue argument(std::move(key.back()));
    key.pop_back();

    // An upserter as lambda function for aggregating the argument into its
    // GROUP BY group's entry inside aggregation_hash_table.
    const auto upserter = [&handle, &argument](std::uint8_t *state) {
      handle.iterateUnaryInl(argument, state);
    };

    target_hash_table->upsertCompositeKey(key, &upserter, index);
  };

  const PackedPayloadHashTable &source_hash_table =
      static_cast<const PackedPayloadHashTable &>(distinctify_hash_table);
  // Invoke the lambda function "aggregate_functor" on each composite key vector
  // from the distinctify hash table.
  source_hash_table.forEachCompositeKey(&aggregate_functor);
}

template <typename HandleT>
ColumnVector* AggregationConcreteHandle::finalizeHashTableHelper(
    const Type &result_type,
    const AggregationStateHashTableBase &hash_table,
    const std::size_t index,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  const HandleT &handle = static_cast<const HandleT &>(*this);
  const PackedPayloadHashTable &hash_table_concrete =
      static_cast<const PackedPayloadHashTable &>(hash_table);

  if (group_by_keys->empty()) {
    if (NativeColumnVector::UsableForType(result_type)) {
      NativeColumnVector *result =
          new NativeColumnVector(result_type, hash_table_concrete.numEntries());
      HashTableAggregateFinalizer<HandleT, NativeColumnVector> finalizer(
          handle, group_by_keys, result);
      hash_table_concrete.forEachCompositeKey(&finalizer, index);
      return result;
    } else {
      IndirectColumnVector *result = new IndirectColumnVector(
          result_type, hash_table_concrete.numEntries());
      HashTableAggregateFinalizer<HandleT, IndirectColumnVector> finalizer(
          handle, group_by_keys, result);
      hash_table_concrete.forEachCompositeKey(&finalizer, index);
      return result;
    }
  } else {
    if (NativeColumnVector::UsableForType(result_type)) {
      NativeColumnVector *result =
          new NativeColumnVector(result_type, group_by_keys->size());
      for (const std::vector<TypedValue> &group_by_key : *group_by_keys) {
        result->appendTypedValue(
            finalizeGroupInHashTable<HandleT>(
                hash_table, index, group_by_key));
      }
      return result;
    } else {
      IndirectColumnVector *result = new IndirectColumnVector(
          result_type, hash_table_concrete.numEntries());
      for (const std::vector<TypedValue> &group_by_key : *group_by_keys) {
        result->appendTypedValue(
            finalizeGroupInHashTable<HandleT>(
                hash_table, index, group_by_key));
      }
      return result;
    }
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_CONCRETE_HANDLE_HPP_
