/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_CONCRETE_HANDLE_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_CONCRETE_HANDLE_HPP_

#include <cstddef>
#include <vector>
#include <utility>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/HashTable.hpp"
#include "storage/HashTableBase.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class StorageManager;
class Type;
class ValueAccessor;

/** \addtogroup Expressions
 *  @{
 */


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
  /**
   * @brief Default implementaion for AggregationHandle::accumulateNullary().
   */
  AggregationState* accumulateNullary(
      const std::size_t num_tuples) const override {
    LOG(FATAL) << "Called accumulateNullary on an AggregationHandle that "
               << "takes at least one argument.";
  }

  /**
   * @brief Implementaion for AggregationHandle::createDistinctifyHashTable()
   *        that creates a new HashTable for the distinctify step for
   *        DISTINCT aggregation.
   */
  AggregationStateHashTableBase* createDistinctifyHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type*> &key_types,
      const std::size_t estimated_num_distinct_keys,
      StorageManager *storage_manager) const override;

  /**
   * @brief Implementaion for AggregationHandle::insertValueAccessorIntoDistinctifyHashTable()
   *        that inserts the GROUP BY expressions and aggregation arguments together
   *        as keys into the distinctify hash table.
   */
  void insertValueAccessorIntoDistinctifyHashTable(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &key_ids,
      AggregationStateHashTableBase *distinctify_hash_table) const override;

 protected:
  AggregationConcreteHandle() {
  }

  template <typename HandleT,
            typename StateT,
            typename HashTableT>
  void aggregateValueAccessorIntoHashTableNullaryHelper(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &group_by_key_ids,
      const StateT &default_state,
      AggregationStateHashTableBase *hash_table) const;

  template <typename HandleT,
            typename StateT,
            typename HashTableT>
  void aggregateValueAccessorIntoHashTableUnaryHelper(
      ValueAccessor *accessor,
      const attribute_id argument_id,
      const std::vector<attribute_id> &group_by_key_ids,
      const StateT &default_state,
      AggregationStateHashTableBase *hash_table) const;

  template <typename HandleT,
            typename StateT>
  StateT* aggregateOnDistinctifyHashTableForSingleUnaryHelper(
      const AggregationStateHashTableBase &distinctify_hash_table) const;

  template <typename HandleT,
            typename StateT,
            typename HashTableT>
  void aggregateOnDistinctifyHashTableForGroupByUnaryHelper(
      const AggregationStateHashTableBase &distinctify_hash_table,
      const StateT &default_state,
      AggregationStateHashTableBase *hash_table) const;

  template <typename HandleT,
            typename HashTableT>
  ColumnVector* finalizeHashTableHelper(
      const Type &result_type,
      const AggregationStateHashTableBase &hash_table,
      std::vector<std::vector<TypedValue>> *group_by_keys) const;

  template <typename HandleT, typename HashTableT>
  inline TypedValue finalizeGroupInHashTable(
      const AggregationStateHashTableBase &hash_table,
      const std::vector<TypedValue> &group_key) const {
    const AggregationState *group_state
        = static_cast<const HashTableT&>(hash_table).getSingleCompositeKey(group_key);
    DCHECK(group_state != nullptr)
        << "Could not find entry for specified group_key in HashTable";
    return static_cast<const HandleT*>(this)->finalizeHashTableEntry(*group_state);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(AggregationConcreteHandle);
};

/**
 * @brief Templated class to implement value-accessor-based upserter for each
 *        aggregation state payload type. This version is for nullary
 *        aggregates (those that take no arguments).
 **/
template <typename HandleT, typename StateT>
class NullaryAggregationStateValueAccessorUpserter {
 public:
  explicit NullaryAggregationStateValueAccessorUpserter(const HandleT &handle)
      : handle_(handle) {
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor, StateT *state) {
    handle_.iterateNullaryInl(state);
  }

 private:
  const HandleT &handle_;
};

/**
 * @brief Templated class to implement value-accessor-based upserter for each
 *        aggregation state payload type. This version is for unary aggregates
 *        (those that take a single argument).
 **/
template <typename HandleT, typename StateT>
class UnaryAggregationStateValueAccessorUpserter {
 public:
  UnaryAggregationStateValueAccessorUpserter(const HandleT &handle,
                                             attribute_id value_id)
    : handle_(handle),
      value_id_(value_id) {
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor, StateT *state) {
    handle_.iterateUnaryInl(state, accessor.getTypedValue(value_id_));
  }

 private:
  const HandleT &handle_;
  const attribute_id value_id_;
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
  HashTableAggregateFinalizer(const HandleT &handle,
                              std::vector<std::vector<TypedValue>> *group_by_keys,
                              ColumnVectorT *output_column_vector)
      : handle_(handle),
        group_by_keys_(group_by_keys),
        output_column_vector_(output_column_vector) {
  }

  inline void operator()(const std::vector<TypedValue> &group_by_key,
                         const AggregationState &group_state) {
    group_by_keys_->emplace_back(group_by_key);
    output_column_vector_->appendTypedValue(handle_.finalizeHashTableEntry(group_state));
  }

 private:
  const HandleT &handle_;
  std::vector<std::vector<TypedValue>> *group_by_keys_;
  ColumnVectorT *output_column_vector_;
};

/** @} */

// ----------------------------------------------------------------------------
// Implementations of templated methods follow:

template <typename HandleT,
          typename StateT,
          typename HashTableT>
void AggregationConcreteHandle::aggregateValueAccessorIntoHashTableNullaryHelper(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &group_by_key_ids,
    const StateT &default_state,
    AggregationStateHashTableBase *hash_table) const {
  NullaryAggregationStateValueAccessorUpserter<HandleT, StateT>
      upserter(static_cast<const HandleT&>(*this));
  static_cast<HashTableT*>(hash_table)->upsertValueAccessorCompositeKey(
      accessor,
      group_by_key_ids,
      true,
      default_state,
      &upserter);
}

template <typename HandleT,
          typename StateT,
          typename HashTableT>
void AggregationConcreteHandle::aggregateValueAccessorIntoHashTableUnaryHelper(
    ValueAccessor *accessor,
    const attribute_id argument_id,
    const std::vector<attribute_id> &group_by_key_ids,
    const StateT &default_state,
    AggregationStateHashTableBase *hash_table) const {
  UnaryAggregationStateValueAccessorUpserter<HandleT, StateT>
      upserter(static_cast<const HandleT&>(*this), argument_id);
  static_cast<HashTableT*>(hash_table)->upsertValueAccessorCompositeKey(
      accessor,
      group_by_key_ids,
      true,
      default_state,
      &upserter);
}

template <typename HandleT,
          typename StateT>
StateT* AggregationConcreteHandle::aggregateOnDistinctifyHashTableForSingleUnaryHelper(
    const AggregationStateHashTableBase &distinctify_hash_table) const {
  const HandleT& handle = static_cast<const HandleT&>(*this);
  StateT *state = static_cast<StateT*>(createInitialState());

  // A lambda function which will be called on each key from the distinctify
  // hash table.
  const auto aggregate_functor = [&handle, &state](const TypedValue &key,
                                                   const bool &dumb_placeholder) {
    // For each (unary) key in the distinctify hash table, aggregate the key
    // into "state".
    handle.iterateUnaryInl(state, key);
  };

  const AggregationStateHashTable<bool> &hash_table =
      static_cast<const AggregationStateHashTable<bool>&>(distinctify_hash_table);
  // Invoke the lambda function "aggregate_functor" on each key from the distinctify
  // hash table.
  hash_table.forEach(&aggregate_functor);

  return state;
}

template <typename HandleT,
          typename StateT,
          typename HashTableT>
void AggregationConcreteHandle::aggregateOnDistinctifyHashTableForGroupByUnaryHelper(
    const AggregationStateHashTableBase &distinctify_hash_table,
    const StateT &default_state,
    AggregationStateHashTableBase *aggregation_hash_table) const {
  const HandleT& handle = static_cast<const HandleT&>(*this);
  HashTableT *target_hash_table = static_cast<HashTableT*>(aggregation_hash_table);

  // A lambda function which will be called on each key-value pair from the
  // distinctify hash table.
  const auto aggregate_functor = [&handle, &target_hash_table, &default_state](
      std::vector<TypedValue> &key,
      const bool &dumb_placeholder) {
    // For each (composite) key vector in the distinctify hash table with size N.
    // The first N-1 entries are GROUP BY columns and the last entry is the argument
    // to be aggregated on.
    const TypedValue argument(std::move(key.back()));
    key.pop_back();

    // An upserter as lambda function for aggregating the argument into its
    // GROUP BY group's entry inside aggregation_hash_table.
    const auto upserter = [&handle, &argument](StateT *state) {
      handle.iterateUnaryInl(state, argument);
    };

    target_hash_table->upsertCompositeKey(key, default_state, &upserter);
  };

  const AggregationStateHashTable<bool> &source_hash_table =
      static_cast<const AggregationStateHashTable<bool>&>(distinctify_hash_table);
  // Invoke the lambda function "aggregate_functor" on each composite key vector
  // from the distinctify hash table.
  source_hash_table.forEachCompositeKey(&aggregate_functor);
}

template <typename HandleT,
          typename HashTableT>
ColumnVector* AggregationConcreteHandle::finalizeHashTableHelper(
    const Type &result_type,
    const AggregationStateHashTableBase &hash_table,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  const HandleT &handle = static_cast<const HandleT&>(*this);
  const HashTableT &hash_table_concrete = static_cast<const HashTableT&>(hash_table);

  if (group_by_keys->empty()) {
    if (NativeColumnVector::UsableForType(result_type)) {
      NativeColumnVector *result = new NativeColumnVector(result_type,
                                                          hash_table_concrete.numEntries());
      HashTableAggregateFinalizer<HandleT, NativeColumnVector> finalizer(
          handle,
          group_by_keys,
          result);
      hash_table_concrete.forEachCompositeKey(&finalizer);
      return result;
    } else {
      IndirectColumnVector *result = new IndirectColumnVector(result_type,
                                                              hash_table_concrete.numEntries());
      HashTableAggregateFinalizer<HandleT, IndirectColumnVector> finalizer(
          handle,
          group_by_keys,
          result);
      hash_table_concrete.forEachCompositeKey(&finalizer);
      return result;
    }
  } else {
    if (NativeColumnVector::UsableForType(result_type)) {
      NativeColumnVector *result = new NativeColumnVector(result_type,
                                                          group_by_keys->size());
      for (const std::vector<TypedValue> &group_by_key : *group_by_keys) {
        result->appendTypedValue(finalizeGroupInHashTable<HandleT, HashTableT>(hash_table,
                                                                               group_by_key));
      }
      return result;
    } else {
      IndirectColumnVector *result = new IndirectColumnVector(result_type,
                                                              hash_table_concrete.numEntries());
      for (const std::vector<TypedValue> &group_by_key : *group_by_keys) {
        result->appendTypedValue(finalizeGroupInHashTable<HandleT, HashTableT>(hash_table,
                                                                               group_by_key));
      }
      return result;
    }
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_CONCRETE_HANDLE_HPP_
