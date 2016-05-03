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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_

#include <utility>
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

class CatalogRelationSchema;
class StorageManager;
class WorkOrdersContainer;

struct TupleReference;

template <typename, bool, bool, bool, bool> class HashTable;
typedef HashTable<TupleReference, true, false, false, true> JoinHashTable;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which builds a shared hash table on one
 *        relation.
 **/
class BuildHashOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param input_relation The relation to build hash table on.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param hash_table_index The index of the JoinHashTable in QueryContext.
   *        The HashTable's key Type(s) should be the Type(s) of the
   *        join_key_attributes in input_relation.
   * @param The ID of the query to which this operator belongs.
   **/
  BuildHashOperator(const CatalogRelation &input_relation,
                    const bool input_relation_is_stored,
                    const std::vector<attribute_id> &join_key_attributes,
                    const bool any_join_key_attributes_nullable,
                    const QueryContext::join_hash_table_id hash_table_index,
                    const std::size_t query_id)
    : RelationalOperator(query_id),
      input_relation_(input_relation),
      input_relation_is_stored_(input_relation_is_stored),
      join_key_attributes_(join_key_attributes),
      any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
      hash_table_index_(hash_table_index),
      input_relation_block_ids_(input_relation_is_stored ? input_relation.getBlocksSnapshot()
                                                         : std::vector<block_id>()),
      num_workorders_generated_(0),
      started_(false) {}

  ~BuildHashOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id) override {
    input_relation_block_ids_.push_back(input_block_id);
  }

  void feedInputBlocks(const relation_id rel_id,
                       std::vector<block_id> *partially_filled_blocks) override {
    input_relation_block_ids_.insert(input_relation_block_ids_.end(),
                                     partially_filled_blocks->begin(),
                                     partially_filled_blocks->end());
  }

 private:
  const CatalogRelation &input_relation_;
  const bool input_relation_is_stored_;
  const std::vector<attribute_id> join_key_attributes_;
  const bool any_join_key_attributes_nullable_;
  const QueryContext::join_hash_table_id hash_table_index_;

  std::vector<block_id> input_relation_block_ids_;
  std::vector<block_id>::size_type num_workorders_generated_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(BuildHashOperator);
};

/**
 * @brief A WorkOrder produced by BuildHashOperator
 **/
class BuildHashWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param input_relation The relation to build hash table on.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param build_block_id The block id.
   * @param query_id The ID of the query.
   * @param hash_table The JoinHashTable to use.
   * @param storage_manager The StorageManager to use.
   **/
  BuildHashWorkOrder(const CatalogRelationSchema &input_relation,
                     const std::vector<attribute_id> &join_key_attributes,
                     const bool any_join_key_attributes_nullable,
                     const block_id build_block_id,
                     const std::size_t query_id,
                     JoinHashTable *hash_table,
                     StorageManager *storage_manager)
      : WorkOrder(query_id),
        input_relation_(input_relation),
        join_key_attributes_(join_key_attributes),
        any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
        build_block_id_(build_block_id),
        hash_table_(DCHECK_NOTNULL(hash_table)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  /**
   * @brief Constructor for the distributed version.
   *
   * @param input_relation The relation to build hash table on.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param query_id The ID of the query.
   * @param build_block_id The block id.
   * @param hash_table The JoinHashTable to use.
   * @param storage_manager The StorageManager to use.
   **/
  BuildHashWorkOrder(const CatalogRelationSchema &input_relation,
                     std::vector<attribute_id> &&join_key_attributes,
                     const bool any_join_key_attributes_nullable,
                     const block_id build_block_id,
                     const std::size_t query_id,
                     JoinHashTable *hash_table,
                     StorageManager *storage_manager)
      : WorkOrder(query_id),
        input_relation_(input_relation),
        join_key_attributes_(std::move(join_key_attributes)),
        any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
        build_block_id_(build_block_id),
        hash_table_(DCHECK_NOTNULL(hash_table)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  ~BuildHashWorkOrder() override {}

  void execute() override;

 private:
  const CatalogRelationSchema &input_relation_;
  const std::vector<attribute_id> join_key_attributes_;
  const bool any_join_key_attributes_nullable_;
  const block_id build_block_id_;

  JoinHashTable *hash_table_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(BuildHashWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_
