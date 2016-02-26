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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_HASH_JOIN_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_HASH_JOIN_OPERATOR_HPP_

#include <cstddef>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabase;
class CatalogRelationSchema;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which performs a hash-join on two
 *        relations.
 **/
class HashJoinOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @note This operator can be constructed with an optional parameter
   *       residual_predicate_index that applies an additional filter besides
   *       the key-equality predicate. If the residual_predicate_index is not
   *       kInvalidPredicateId, then the key-equality predicate will still be
   *       evaluated using vectorized probing of the hash-table, but the
   *       residual predicate will be checked using non-vectorized
   *       tuple-at-a-time evaluation (similar to NestedLoopsJoinOperator).
   *       Alternatively, a HashJoinOperator can be used to evaluate just the
   *       key-equality predicate, and its output can be piped to a subsequent
   *       SelectOperator that can evaluate the residual predicate on the
   *       materialized output of the hash join with full vectorization support
   *       (although at the cost of making an additional intermediate data
   *       copy). Which approach performs best in practice is likely to be
   *       dependent on the selectivity of the predicates involved. The decision
   *       is left to the query optimizer.
   *
   * @param build_relation The relation that the hash table was originally
   *        built on (i.e. the inner relation in the join).
   * @param probe_relation The relation to probe the hash table with (i.e. the
   *        outer relation in the join).
   * @param probe_relation_is_stored If probe_relation is a stored relation or
   *        not. False if probe_relation is the output of some operator in the
   *        query plan DAG. True if probe_relation is a stored relation.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        probe_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the join results.
   * @param hash_table_index The index of the JoinHashTable in QueryContext.
   * @param residual_predicate_index If not kInvalidPredicateId, apply as an
   *        additional filter to pairs of tuples that match the hash-join (i.e.
   *        key equality) predicate. Effectively, this makes the join predicate
   *        the conjunction of the key-equality predicate and residual predicate.
   * @param selection_index The group index of Scalars in QueryContext,
   *        corresponding to the attributes of the relation referred by
   *        output_relation_id. Each Scalar is evaluated for the joined tuples,
   *        and the resulting value is inserted into the join result.
   **/
  HashJoinOperator(const CatalogRelation &build_relation,
                   const CatalogRelation &probe_relation,
                   const bool probe_relation_is_stored,
                   const std::vector<attribute_id> &join_key_attributes,
                   const bool any_join_key_attributes_nullable,
                   const CatalogRelation &output_relation,
                   const QueryContext::insert_destination_id output_destination_index,
                   const QueryContext::join_hash_table_id hash_table_index,
                   const QueryContext::predicate_id residual_predicate_index,
                   const QueryContext::scalar_group_id selection_index)
    : build_relation_(build_relation),
      probe_relation_(probe_relation),
      probe_relation_is_stored_(probe_relation_is_stored),
      join_key_attributes_(join_key_attributes),
      any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
      output_relation_(output_relation),
      output_destination_index_(output_destination_index),
      hash_table_index_(hash_table_index),
      residual_predicate_index_(residual_predicate_index),
      selection_index_(selection_index),
      probe_relation_block_ids_(probe_relation_is_stored ? probe_relation.getBlocksSnapshot()
                                                         : std::vector<block_id>()),
      num_workorders_generated_(0),
      started_(false) {}

  ~HashJoinOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        CatalogDatabase *catalog_database,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        tmb::MessageBus *bus) override;

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id) override {
    DCHECK(input_relation_id == probe_relation_.getID());
    probe_relation_block_ids_.push_back(input_block_id);
  }

  void feedInputBlocks(const relation_id rel_id,
                       std::vector<block_id> *partially_filled_blocks) override {
    DCHECK(rel_id == probe_relation_.getID());
    probe_relation_block_ids_.insert(probe_relation_block_ids_.end(),
                                     partially_filled_blocks->begin(),
                                     partially_filled_blocks->end());
  }

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

  void doneFeedingInputBlocks(const relation_id rel_id) override {
    // The HashJoinOperator depends on BuildHashOperator too, but it
    // should ignore a doneFeedingInputBlocks() message that comes
    // after completion of BuildHashOperator. Therefore we need this check.
    if (probe_relation_.getID() == rel_id) {
      done_feeding_input_relation_ = true;
    }
  }

 private:
  const CatalogRelation &build_relation_;
  const CatalogRelation &probe_relation_;
  const bool probe_relation_is_stored_;
  const std::vector<attribute_id> join_key_attributes_;
  const bool any_join_key_attributes_nullable_;
  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;
  const QueryContext::join_hash_table_id hash_table_index_;
  const QueryContext::predicate_id residual_predicate_index_;
  const QueryContext::scalar_group_id selection_index_;

  std::vector<block_id> probe_relation_block_ids_;
  std::size_t num_workorders_generated_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(HashJoinOperator);
};

/**
 * @brief A WorkOrder produced by HashJoinOperator.
 **/
class HashJoinWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param build_relation The relation that the hash table was originally built
   *        on (i.e. the inner relation in the join).
   * @param probe_relation The relation to probe the hash table with (i.e. the
   *        outer relation in the join).
   * @param join_key_attributes The IDs of equijoin attributes in \c
   *        probe_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param lookup_block_id The block id of the probe_relation.
   * @param residual_predicate If non-null, apply as an additional filter to
   *        pairs of tuples that match the hash-join (i.e. key equality)
   *        predicate. Effectively, this makes the join predicate the
   *        conjunction of the key-equality predicate and residual_predicate.
   * @param selection A list of Scalars corresponding to the relation attributes
   *        in \c output_destination. Each Scalar is evaluated for the joined
   *        tuples, and the resulting value is inserted into the join result.
   * @param output_destination The InsertDestination to insert the join results.
   * @param hash_table The JoinHashTable to use.
   * @param storage_manager The StorageManager to use.
   **/
  HashJoinWorkOrder(const CatalogRelationSchema &build_relation,
                    const CatalogRelationSchema &probe_relation,
                    const std::vector<attribute_id> &join_key_attributes,
                    const bool any_join_key_attributes_nullable,
                    const block_id lookup_block_id,
                    const Predicate *residual_predicate,
                    const std::vector<std::unique_ptr<const Scalar>> &selection,
                    InsertDestination *output_destination,
                    JoinHashTable *hash_table,
                    StorageManager *storage_manager)
      : build_relation_(build_relation),
        probe_relation_(probe_relation),
        join_key_attributes_(join_key_attributes),
        any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
        block_id_(lookup_block_id),
        residual_predicate_(residual_predicate),
        selection_(selection),
        output_destination_(output_destination),
        hash_table_(hash_table),
        storage_manager_(storage_manager) {
    DCHECK(output_destination_ != nullptr);
    DCHECK(hash_table_ != nullptr);
    DCHECK(storage_manager_ != nullptr);
  }

  ~HashJoinWorkOrder() override {}

  /**
   * @exception TupleTooLargeForBlock A tuple produced by this join was too
   *            large to insert into an empty block provided by
   *            output_destination_index_ in query_context. Join may be
   *            partially complete (with some tuples inserted into the
   *            destination) when this exception is thrown, causing potential
   *            inconsistency.
   **/
  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  template <typename CollectorT>
  void executeWithCollectorType(QueryContext *query_context,
                                CatalogDatabase *catalog_database,
                                StorageManager *storage_manager);

  const CatalogRelationSchema &build_relation_, &probe_relation_;
  const std::vector<attribute_id> join_key_attributes_;
  const bool any_join_key_attributes_nullable_;
  const block_id block_id_;
  const Predicate *residual_predicate_;
  const std::vector<std::unique_ptr<const Scalar>> &selection_;

  InsertDestination *output_destination_;
  JoinHashTable *hash_table_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(HashJoinWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_HASH_JOIN_OPERATOR_HPP_
