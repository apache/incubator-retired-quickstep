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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_HPP_

#include <cstddef>
#include <vector>

#include "expressions/aggregation/AggregationID.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class StorageManager;
class Type;

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
 * I. The work-flow for computing an aggregate without GROUP BY is as follows:
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
 * II. The work-flow for computing an aggregate with GROUP BY is as follows:
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
  virtual ~AggregationHandle() {}

  /**
   * @brief Get the ID of this aggregation.
   *
   * @return The AggregationID of this AggregationHandle.
   **/
  inline AggregationID getAggregationID() const {
    return agg_id_;
  }

  /**
   * @brief Get the list of Types (in order) for arguments to this aggregation.
   *
   * @return The list of Types for arguments to this aggregation.
   */
  virtual std::vector<const Type *> getArgumentTypes() const = 0;

  /**
   * @brief Get the result Type of this aggregation.
   *
   * @return The result Type of this aggregation.
   */
  virtual const Type* getResultType() const = 0;

  /**
   * @brief Create an initial "blank" state for this aggregation.
   *
   * @return An initial "blank" state for this particular aggregation.
   **/
  virtual AggregationState* createInitialState() const = 0;

  /**
   * @brief Accumulate over tuples for a nullary aggregate function (one that
   *        has zero arguments, i.e. COUNT(*)).
   *
   * @param num_tuples The number of tuples to "accumulate". No actual tuple
   *        data is accessed, the only thing that a nullary aggeregate can know
   *        about input is its cardinality.
   * @return A new AggregationState which contains the accumulated results from
   *         applying the (nullary) aggregate to the specified number of tuples.
   **/
  virtual AggregationState* accumulateNullary(
      const std::size_t num_tuples) const {
    LOG(FATAL) << "Called accumulateNullary on an AggregationHandle that "
               << "takes at least one argument.";
  }

  /**
   * @brief Accumulate (iterate over) all values in columns accessible through
   *        the ValueAccessors from a ValueAccessorMultiplexer and return a new
   *        AggregationState which can be merged with other states or finalized.
   *
   * @param argument_ids The multi-source attribute ids that correspond to the
   *        columns in \p accessor_mux to aggeregate. These correspond to the
   *        aggregate function's arguments, in order.
   * @param accessor_mux A ValueAccessorMultiplexer object that contains the
   *        ValueAccessors.
   * @return A new AggregationState which contains the accumulated results from
   *         applying the aggregate to the specified columns.
   *         Caller is responsible for deleting the returned AggregationState.
   **/
  virtual AggregationState* accumulateValueAccessor(
      const std::vector<MultiSourceAttributeId> &argument_ids,
      const ValueAccessorMultiplexer &accessor_mux) const = 0;

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
   * @param index The index of the AggregationHandle to be finalized.
   * @param group_by_keys A pointer to a vector of vectors of GROUP BY keys. If
   *        this is initially empty, it will be filled in with the GROUP BY
   *        keys visited by this method in the same order as the finalized
   *        values returned in the ColumnVector. If this is already filled in,
   *        then this method will visit the GROUP BY keys in the exact order
   *        specified.
   *
   * @return A ColumnVector containing each group's finalized aggregate value.
   **/
  virtual ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      const std::size_t index,
      std::vector<std::vector<TypedValue>> *group_by_keys) const = 0;

  /**
   * @brief Create a new HashTable for the distinctify step for DISTINCT
   *        aggregation.
   *
   * Distinctify is the first step for DISTINCT aggregation. This step inserts
   * the GROUP BY expression values and aggregation arguments together as keys
   * into the distinctify hash table, so that arguments are distinctified within
   * each GROUP BY group. Later, a second-round aggregation on the distinctify
   * hash table will be performed to actually compute the aggregated result for
   * each GROUP BY group.
   *
   * In the case of single aggregation where there is no GROUP BY expressions,
   * we simply treat it as a special GROUP BY case that the GROUP BY expression
   * vector is empty.
   *
   * @param hash_table_impl The choice of which concrete HashTable implementation
   *        to use.
   * @param key_types The types of the GROUP BY expressions together with the
   *        types of the aggregation arguments.
   * @param estimated_num_distinct_keys The estimated number of distinct keys
   *        (i.e. GROUP BY expressions together with aggregation arguments) for
   *        the distinctify step. This is used to size the initial HashTable.
   *        This is an estimate only, and the HashTable will be resized if it
   *        becomes over-full.
   * @param storage_manager The StorageManager to use to create the HashTable.
   *        A StorageBlob will be allocated to serve as the HashTable's
   *        in-memory storage.
   *
   * @return A new HashTable instance with the appropriate state type for this
   *         aggregate.
   */
  virtual AggregationStateHashTableBase* createDistinctifyHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type *> &key_types,
      const std::size_t estimated_num_distinct_keys,
      StorageManager *storage_manager) const = 0;

  /**
   * @brief Inserts the GROUP BY expressions and aggregation arguments together
   *        as keys into the distinctify hash table.
   *
   * @param argument_ids The argument ids that correspond to the columns in
   *        \p accessor_mux.
   * @param key_ids The group-by key ids that correspond to the columns in
   *        \p accessor_mux.
   * @param accessor_mux A ValueAccessorMultiplexer object that contains the
   *        ValueAccessors to be iterated over to read tuples.
   * @param distinctify_hash_table The HashTable to store the GROUP BY
   *        expressions and the aggregation arguments together as hash table
   *        keys and a bool constant \c true as hash table value (So the hash
   *        table actually serves as a hash set). This should have been created
   *        by calling createDistinctifyHashTable();
   */
  virtual void insertValueAccessorIntoDistinctifyHashTable(
      const std::vector<MultiSourceAttributeId> &argument_ids,
      const std::vector<MultiSourceAttributeId> &key_ids,
      const ValueAccessorMultiplexer &accessor_mux,
      AggregationStateHashTableBase *distinctify_hash_table) const = 0;

  /**
   * @brief Perform single (i.e. without GROUP BY) aggregation on the keys from
   *        the distinctify hash table to actually compute the aggregated results.
   *
   * @param distinctify_hash_table Hash table which stores the distinctified
   *        aggregation arguments as hash table keys. This should have been
   *        created by calling createDistinctifyHashTable();
   * @return A new AggregationState which contains the aggregated results from
   *         applying the aggregate to the distinctify hash table.
   *         Caller is responsible for deleting the returned AggregationState.
   */
  virtual AggregationState* aggregateOnDistinctifyHashTableForSingle(
      const AggregationStateHashTableBase &distinctify_hash_table) const = 0;

  /**
   * @brief Perform GROUP BY aggregation on the keys from the distinctify hash
   * table and upserts states into the aggregation hash table.
   *
   * @param distinctify_hash_table Hash table which stores the GROUP BY
   *        expression values and aggregation arguments together as hash table
   *        keys.
   * @param index The index of the AggregationHandle to perform aggregation.
   * @param aggregation_hash_table The HashTable to upsert AggregationStates in.
   *        This should have been created by calling createGroupByHashTable() on
   *        this same AggregationHandle.
   */
  virtual void aggregateOnDistinctifyHashTableForGroupBy(
      const AggregationStateHashTableBase &distinctify_hash_table,
      const std::size_t index,
      AggregationStateHashTableBase *aggregation_hash_table) const = 0;

  /**
   * @brief Get the number of bytes needed to store the aggregation handle's
   *        state.
   **/
  virtual std::size_t getPayloadSize() const {
    return 1u;
  }

  /**
   * @brief Update the aggregation state for nullary aggregation function, e.g.
   *        COUNT(*).
   *
   * @note This function should be overloaded by those aggregation function
   *       which can perform nullary operations, e.g. COUNT.
   *
   * @param byte_ptr The pointer where the aggregation state is stored.
   **/
  virtual void updateStateNullary(std::uint8_t *byte_ptr) const {
    LOG(FATAL) << "Called updateStateNullary on an AggregationHandle that "
               << "takes at least one argument.";
  }

  /**
   * @brief Update the aggregation state for unary aggregation function e.g.
   *        SUM(a).
   *
   * @param argument The argument which will be used to update the state of the
   *        aggregation function.
   * @param byte_ptr The pointer where the aggregation state is stored.
   **/
  virtual void updateStateUnary(const TypedValue &argument,
                                std::uint8_t *byte_ptr) const = 0;

  /**
   * @brief Merge two aggregation states for this aggregation handle.
   *
   * @note This function should be used with the hash table specifically meant
   *       for aggregations only.
   *
   * @param src A pointer to the source aggregation state.
   * @param dst A pointer to the destination aggregation state.
   **/
  virtual void mergeStates(const std::uint8_t *src,
                           std::uint8_t *dst) const = 0;

  /**
   * @brief Initialize the payload (in the aggregation hash table) for the given
   *        aggregation handle.
   *
   * @param byte_ptr The pointer to the aggregation state in the hash table.
   **/
  virtual void initPayload(std::uint8_t *byte_ptr) const = 0;

  /**
   * @brief Destroy the payload (in the aggregation hash table) for the given
   *        aggregation handle.
   *
   * @param byte_ptr The pointer to the aggregation state in the hash table.
   **/
  virtual void destroyPayload(std::uint8_t *byte_ptr) const = 0;

  /**
   * @brief Inform the aggregation handle to block (prohibit) updates on the
   *        aggregation state.
   **/
  virtual void blockUpdate() = 0;

  /**
   * @brief Inform the aggregation handle to allow updates on the aggregation
   *        state.
   **/
  virtual void allowUpdate() = 0;

 protected:
  explicit AggregationHandle(const AggregationID agg_id)
      : agg_id_(agg_id) {}

  const AggregationID agg_id_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AggregationHandle);
};

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_HPP_
