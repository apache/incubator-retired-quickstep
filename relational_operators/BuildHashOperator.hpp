/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_

#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class StorageManager;
class WorkOrdersContainer;

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
   **/
  BuildHashOperator(const CatalogRelation &input_relation,
                    const bool input_relation_is_stored,
                    const std::vector<attribute_id> &join_key_attributes,
                    const bool any_join_key_attributes_nullable,
                    const QueryContext::join_hash_table_id hash_table_index)
    : input_relation_(input_relation),
      input_relation_is_stored_(input_relation_is_stored),
      join_key_attributes_(join_key_attributes),
      any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
      hash_table_index_(hash_table_index),
      input_relation_block_ids_(input_relation_is_stored ? input_relation.getBlocksSnapshot()
                                                         : std::vector<block_id>()),
      num_workorders_generated_(0),
      started_(false) {}

  ~BuildHashOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

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
   * @param rel_id The id of the relation to build hash table on.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param build_block_id The block id.
   * @param hash_table_index The index of the JoinHashTable in QueryContext.
   **/
  BuildHashWorkOrder(const relation_id rel_id,
                     const std::vector<attribute_id> &join_key_attributes,
                     const bool any_join_key_attributes_nullable,
                     const block_id build_block_id,
                     const QueryContext::join_hash_table_id hash_table_index)
      : rel_id_(rel_id),
        join_key_attributes_(join_key_attributes),
        any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
        build_block_id_(build_block_id),
        hash_table_index_(hash_table_index) {}

  ~BuildHashWorkOrder() override {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const relation_id rel_id_;
  const std::vector<attribute_id> join_key_attributes_;
  const bool any_join_key_attributes_nullable_;
  const block_id build_block_id_;
  const QueryContext::join_hash_table_id hash_table_index_;

  DISALLOW_COPY_AND_ASSIGN(BuildHashWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_
