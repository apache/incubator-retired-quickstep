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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATION_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATION_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/AggregationOperationState.pb.h"
#include "storage/HashTableBase.hpp"
#include "storage/HashTablePool.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/BloomFilter.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregateFunction;
class CatalogDatabaseLite;
class CatalogRelationSchema;
class InsertDestination;
class StorageManager;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Helper class for maintaining state during aggregation operation.
 *        If a GROUP BY list was provided, this class maintains a hash table
 *        for each aggregate computed where the key is the GROUP BY expression
 *        values and payload is each group's corresponding running aggregation
 *        state. Without GROUP BY, this class maintains a single aggregation
 *        state for each aggregate computed.
 * @note See also AggregationHandle, which encapsulates logic for actually
 *       computing aggregates, and AggregateFunction, which represents a
 *       particular SQL aggregate in the abstract sense.
 *
 * This class represents the common state for an instance of
 * AggregationOperator, and also encapsulates the high-level logic for
 * aggregating over blocks and generating final results.
 * AggregationWorkOrder::execute() is mainly just a call to aggregateBlock(),
 * while FinalizeAggregationWorkOrder::execute() is mainly just a call to
 * finalizeAggregate().
 **/
class AggregationOperationState {
 public:
  /**
   * @brief Constructor for aggregation operation state.
   * @note The order of some of the parameters to this constructor (or the
   *       corresponding fields when reconstructing from a protobuf) determines
   *       the schema of tuples written out by finalizeAggregate(). If group_by
   *       is nonempty, the first attribute(s) will be the group-by values, in
   *       order. Following that will be the values for each aggregate
   *       specified by aggregate_functions (with arguments specified by
   *       attributes), in order.
   *
   * @param input_relation Input relation on which the aggregates are computed.
   * @param aggregate_functions A list of the aggregate functions to be
   *        computed.
   * @param arguments For each entry in aggregate_functions, a corresponding
   *        list of argument expressions to that aggregate. This is moved-from,
   *        with AggregationOperationState taking ownership.
   * @param is_distinct For each entry in aggregate_functions, whether DISTINCT
   *        should be applied to the entry's arguments.
   * @param group_by A list of expressions to compute the GROUP BY values. If
   *        empty, no grouping is used. This is moved-from, with
   *        AggregationOperationState taking ownership.
   * @param predicate The predicate to be applied prior to aggregation. nullptr
   *        indicates no predicate to be applied. This object takes ownership
   *        of predicate.
   * @param estimated_num_entries Estimated of number of entries in the hash
   *        table. A good estimate would be a fraction of total number of tuples
   *        in the input relation.
   * @param hash_table_impl_type The HashTable implementation to use for
   *        GROUP BY. Ignored if group_by is empty.
   * @param distinctify_hash_table_impl_type The HashTable implementation to use
   *        for the distinctify phase of each DISTINCT aggregation.
   * @param storage_manager The StorageManager to use for allocating hash
   *        tables. Single aggregation state (when GROUP BY list is not
   *        specified) is not allocated using memory from storage manager.
   */
  AggregationOperationState(const CatalogRelationSchema &input_relation,
                            const std::vector<const AggregateFunction*> &aggregate_functions,
                            std::vector<std::vector<std::unique_ptr<const Scalar>>> &&arguments,
                            std::vector<bool> &&is_distinct,
                            std::vector<std::unique_ptr<const Scalar>> &&group_by,
                            const Predicate *predicate,
                            std::vector<const BloomFilter *> &&bloom_filters,
                            std::vector<attribute_id> &&bloom_filter_attribute_ids,
                            const std::size_t estimated_num_entries,
                            const HashTableImplType hash_table_impl_type,
                            const std::vector<HashTableImplType> &distinctify_hash_table_impl_types,
                            StorageManager *storage_manager);

  ~AggregationOperationState() {}

  /**
   * @brief Generate the aggregation operation state from the serialized
   *        Protocol Buffer representation.
   * @note The order of some repeated fields in the proto representation
   *       determines the schema of tuples written out by finalizeAggregate().
   *       See the note for the constructor for details.
   *
   * @param proto A serialized Protocol Buffer representation of an
   *        AggregationOperationState, originally generated by the optimizer.
   * @param database The Database to resolve relation and attribute references
   *        in.
   * @param storage_manager The StorageManager to use.
   **/
  static AggregationOperationState* ReconstructFromProto(
      const serialization::AggregationOperationState &proto,
      const CatalogDatabaseLite &database,
      StorageManager *storage_manager,
      const std::vector<std::unique_ptr<BloomFilter>> &bloom_filters);

  /**
   * @brief Check whether a serialization::AggregationOperationState is
   *        fully-formed and all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of an
   *        AggregationOperationState, originally generated by the optimizer.
   * @param database The Database to resolve relation and attribute references
   *        in.
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::AggregationOperationState &proto,
                           const CatalogDatabaseLite &database);

  /**
   * @brief Compute aggregates on the tuples of the given storage block,
   *        updating the running state maintained by this
   *        AggregationOperationState.
   *
   * @param input_block The block ID of the storage block where the aggreates
   *        are going to be computed.
   **/
  void aggregateBlock(const block_id input_block);

  /**
   * @brief Generate the final results for the aggregates managed by this
   *        AggregationOperationState and write them out to StorageBlock(s).
   *
   * @param output_destination An InsertDestination where the finalized output
   *        tuple(s) from this aggregate are to be written.
   **/
  void finalizeAggregate(InsertDestination *output_destination);

 private:
  // Merge locally (per storage block) aggregated states with global aggregation
  // states.
  void mergeSingleState(const std::vector<std::unique_ptr<AggregationState>> &local_state);

  // Aggregate on input block.
  void aggregateBlockSingleState(const block_id input_block);
  void aggregateBlockHashTable(const block_id input_block);

  void finalizeSingleState(InsertDestination *output_destination);
  void finalizeHashTable(InsertDestination *output_destination);

  // Common state for all aggregates in this operation: the input relation, the
  // filter predicate (if any), and the list of GROUP BY expressions (if any).
  const CatalogRelationSchema &input_relation_;
  std::unique_ptr<const Predicate> predicate_;

  std::vector<const BloomFilter*> bloom_filters_;
  std::vector<attribute_id> bloom_filter_attribute_ids_;

  std::vector<std::unique_ptr<const Scalar>> group_by_list_;

  // Each individual aggregate in this operation has an AggregationHandle and
  // some number of Scalar arguments.
  std::vector<std::unique_ptr<AggregationHandle>> handles_;
  std::vector<std::vector<std::unique_ptr<const Scalar>>> arguments_;

  // For each aggregate, whether DISTINCT should be applied to the aggregate's
  // arguments.
  std::vector<bool> is_distinct_;

  // Hash table for obtaining distinct (i.e. unique) arguments.
  std::vector<std::unique_ptr<AggregationStateHashTableBase>> distinctify_hashtables_;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  // If all an aggregate's argument expressions are simply attributes in
  // 'input_relation_', then this caches the attribute IDs of those arguments.
  std::vector<std::vector<attribute_id>> arguments_as_attributes_;
#endif

  // Per-aggregate global states for aggregation without GROUP BY.
  std::vector<std::unique_ptr<AggregationState>> single_states_;

  // Per-aggregate HashTables for aggregation with GROUP BY.
  //
  // TODO(shoban): We should ideally store the aggregation state together in one
  // hash table to prevent multiple lookups.
  std::vector<std::unique_ptr<AggregationStateHashTableBase>> group_by_hashtables_;

  // A vector of group by hash table pools, one for each group by clause.
  std::vector<std::unique_ptr<HashTablePool>> group_by_hashtable_pools_;

  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(AggregationOperationState);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATION_HPP_
