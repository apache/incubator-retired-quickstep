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

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/HashTableBase.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

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
  AggregationState* accumulateNullary(
      const std::size_t num_tuples) const override {
    LOG(FATAL) << "Called accumulateNullary on an AggregationHandle that "
               << "takes at least one argument.";
  }

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
