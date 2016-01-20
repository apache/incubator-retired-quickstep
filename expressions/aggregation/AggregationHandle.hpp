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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
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
 * @brief Abstract base class for aggregation state.
 **/
class AggregationState {
 public:
  /**
   * @brief Default constructor.
   **/
  AggregationState() {}

  /**
   * @brief Pure virtual destructor.
   **/
  virtual ~AggregationState() = 0;
};

// Destructor should be defined. This will be called when derived class
// destructor is called.
inline AggregationState::~AggregationState() {}

/**
 * @brief AggregationHandle encapsulates logic for actually computing
 *        aggregates with particular argument(s).
 * @note See also AggregateFunction, which represents a SQL aggregate function
 *       in the abstract sense.
 *
 * An AggregationHandle is created by calling
 * AggregateFunction::createHandle(). The AggregationHandle object provides
 * methods that are used to actually compute the aggregate, storing
 * intermediate results in AggregationState objects.
 *
 * The work-flow for computing an aggregate without GROUP BY is as follows:
 *     1. Create a global state for the aggregate with createInitialState().
 *     2. For each block in a relation (parallelizable):
 *        a. Call StorageBlock::aggregate() to accumulate results from the
 *           block (under the covers, this calls either
 *           accumulateColumnVectors() or accumulateValueAccessor() to do the
 *           actual per-block aggregation in a vectorized fashion).
 *        b. Merge the per-block results back with the global state by calling
 *           mergeStates() (this is threadsafe).
 *     3. Generate the final result by calling finalize() on the global state.
 *
 * The work-flow for computing an aggregate with GROUP BY is as follows:
 *     1. Create a HashTable to hold per-group states by calling
 *        createGroupByHashTable().
 *     2. For each block in a relation (parallelizable):
 *        a. Call StorageBlock::aggregateGroupBy() to update the states in the
 *           HashTable according to the values in the block (under the covers,
 *           this calls aggregateValueAccessorIntoHashTable() to aggregate over
 *           all the values/groups in a block in one shot; this is threadsafe).
 *     3. Generate the final set of groups and their corresponding results by
 *        calling finalizeHashTable().
 *
 * See also AggregationOperationState, which holds 1 or more global states or
 * HashTables for an aggregate query, and has some logic to re-use common
 * information across multiple aggregates being computed on the same block
 * (e.g. the set of matches for a predicate, or the values of computed GROUP BY
 * expressions). AggregationOperationState also has a method to write out
 * finalized aggregate values to an InsertDestination.
 **/
class AggregationHandle {
 public:
  /**
   * @brief Virtual destructor.
   *
   **/
  virtual ~AggregationHandle() {
  }

  /**
   * @brief Create an initial "blank" state for this aggregation.
   *
   * @return An initial "blank" state for this particular aggregation.
   **/
  virtual AggregationState* createInitialState() const = 0;

  /**
   * @brief Create a new HashTable for aggregation with GROUP BY.
   *
   * @param hash_table_impl The choice of which concrete HashTable
   *        implementation to use.
   * @param group_by_types The types of the GROUP BY columns/expressions. These
   *        correspond to the (composite) key type for the HashTable.
   * @param estimated_num_groups The estimated number of distinct groups for
   *        the GROUP BY aggregation. This is used to size the initial
   *        HashTable. This is an estimate only, and the HashTable will be
   *        resized if it becomes over-full.
   * @param storage_manager The StorageManager to use to create the HashTable.
   *        A StorageBlob will be allocated to serve as the HashTable's
   *        in-memory storage.
   * @return A new HashTable instance with the appropriate state type for this
   *         aggregate as the ValueT.
   **/
  virtual AggregationStateHashTableBase* createGroupByHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type*> &group_by_types,
      const std::size_t estimated_num_groups,
      StorageManager *storage_manager) const = 0;

  /**
   * @brief Accumulate over tuples for a nullary aggregate function (one that
   *        has zero arguments, i.e. COUNT(*)).
   *
   * @param num_tuples The number of tuples to "accumulate". No actual tuple
   *        data is accessed, the only thing that a nullary aggeregate can know
   *        about input is its cardinality.
   * @return A new AggregationState which contains the accumulated results from
   *         applying the (nullary) aggregate to the specified number of
   *         tuples.
   **/
  virtual AggregationState* accumulateNullary(
      const std::size_t num_tuples) const {
    LOG(FATAL) << "Called accumulateNullary on an AggregationHandle that "
               << "takes at least one argument.";
  }

  /**
   * @brief Accumulate (iterate over) all values in one or more ColumnVectors
   *        and return a new AggregationState which can be merged with other
   *        states or finalized.
   *
   * @param column_vectors One or more ColumnVectors that the aggregate will be
   *        applied to. These correspond to the aggregate function's arguments,
   *        in order.
   * @return A new AggregationState which contains the accumulated results from
   *         applying the aggregate to column_vectors. Caller is responsible
   *         for deleting the returned AggregationState.
   **/
  virtual AggregationState* accumulateColumnVectors(
      const std::vector<std::unique_ptr<ColumnVector>> &column_vectors) const = 0;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  /**
   * @brief Accumulate (iterate over) all values in columns accessible through
   *        a ValueAccessor and return a new AggregationState which can be
   *        merged with other states or finalized.
   *
   * @param accessor A ValueAccessor that the columns to be aggregated can be
   *        accessed through.
   * @param accessor_ids The attribute_ids that correspond to the columns in
   *        accessor to aggeregate. These correspond to the aggregate
   *        function's arguments, in order.
   * @return A new AggregationState which contains the accumulated results from
   *         applying the aggregate to the specified columns in accessor.
   *         Caller is responsible for deleting the returned AggregationState.
   **/
  virtual AggregationState* accumulateValueAccessor(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &accessor_ids) const = 0;
#endif

  /**
   * @brief Perform an aggregation with GROUP BY over all the tuples accessible
   *        through a ValueAccessor, upserting states in a HashTable.
   *
   * @note Implementations of this method are threadsafe with respect to
   *       hash_table, and can be called concurrently from multiple threads
   *       with the same HashTable object.
   *
   * @param accessor The ValueAccessor that will be iterated over to read
   *        tuples.
   * @param argument_ids The attribute_ids of the arguments to this aggregate
   *        in accessor, in order.
   * @param group_by_key_ids The attribute_ids of the group-by
   *        columns/expressions in accessor.
   * @param hash_table The HashTable to upsert AggregationStates in. This
   *        should have been created by calling createGroupByHashTable() on
   *        this same AggregationHandle.
   **/
  virtual void aggregateValueAccessorIntoHashTable(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &argument_ids,
      const std::vector<attribute_id> &group_by_key_ids,
      AggregationStateHashTableBase *hash_table) const = 0;

  /**
   * @brief Merge two AggregationStates, updating one in-place. This computes a
   *        new intermediate result and stores it in the destination
   *        AggregationState.
   *
   * @note The order of arguments (i.e. which AggregationState is source and
   *       which is destination) is not important for the result of the merging
   *       process, but it does determine which of the two states is
   *       overwritten (destination) and which remains unchanged (source).
   * @note Implementations of this method are threadsafe with respect to the
   *       destination state, and can be called concurrently from multiple
   *       threads with the same state object.
   *
   * @param source The AggregationState to merge "from".
   * @param destination The AggregationState to merge "to". The internal state
   *        will be overwritten with the merged result.
   **/
  virtual void mergeStates(const AggregationState &source,
                           AggregationState *destination) const = 0;

  /**
   * @brief Computes and returns the resulting aggregate by using intermediate
   *        result saved in this handle.
   *
   * @note Except for count, SQL89 aggeregates return NULL when no rows are
   *       selected.
   * @warning It is dangerous to assume that calling mergeStates() or some
   *          iterate method after previous finalize call will work correctly
   *          when the aggregate function is not one of SQL89 aggregates (SUM,
   *          COUNT, MIN, MAX, AVG).
   *
   * @return The result of this aggregation.
   **/
  virtual TypedValue finalize(const AggregationState &state) const = 0;

  /**
   * @brief Compute and return finalized aggregates for all groups in a
   *        HashTable.
   *
   * @param hash_table The HashTable to finalize states from. This should have
   *        have been created by calling createGroupByHashTable() on this same
   *        AggregationHandle.
   * @param group_by_keys A pointer to a vector of vectors of GROUP BY keys. If
   *        this is initially empty, it will be filled in with the GROUP BY
   *        keys visited by this method in the same order as the finalized
   *        values returned in the ColumnVector. If this is already filled in,
   *        then this method will visit the GROUP BY keys in the exact order
   *        specified.
   * @return A ColumnVector containing each group's finalized aggregate value.
   **/
  virtual ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      std::vector<std::vector<TypedValue>> *group_by_keys) const = 0;

 protected:
  AggregationHandle() {
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
  DISALLOW_COPY_AND_ASSIGN(AggregationHandle);
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
void AggregationHandle::aggregateValueAccessorIntoHashTableNullaryHelper(
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
void AggregationHandle::aggregateValueAccessorIntoHashTableUnaryHelper(
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
ColumnVector* AggregationHandle::finalizeHashTableHelper(
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

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_HPP_
