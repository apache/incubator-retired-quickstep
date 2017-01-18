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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"
#include "utility/lip_filter/LIPFilterBuilder.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

struct TupleReference;

template <typename, bool, bool, bool, bool> class HashTable;
typedef HashTable<TupleReference, true, false, false, true> JoinHashTable;

namespace serialization { class WorkOrder; }

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
   * @param query_id The ID of the query to which this operator belongs.
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
  BuildHashOperator(const std::size_t query_id,
                    const CatalogRelation &input_relation,
                    const bool input_relation_is_stored,
                    const std::vector<attribute_id> &join_key_attributes,
                    const bool any_join_key_attributes_nullable,
                    const QueryContext::join_hash_table_id hash_table_index)
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

  const CatalogRelation& input_relation() const {
    return input_relation_;
  }

  std::string getName() const override {
    return "BuildHashOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id) override {
    input_relation_block_ids_.push_back(input_block_id);
  }

 private:
  /**
   * @brief Create Work Order proto.
   *
   * @param block The block id used in the Work Order.
   **/
  serialization::WorkOrder* createWorkOrderProto(const block_id block);

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
   * @param query_id The ID of the query.
   * @param input_relation The relation to build hash table on.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param build_block_id The block id.
   * @param hash_table The JoinHashTable to use.
   * @param storage_manager The StorageManager to use.
   * @param lip_filter_builder The attached LIP filter builer.
   **/
  BuildHashWorkOrder(const std::size_t query_id,
                     const CatalogRelationSchema &input_relation,
                     const std::vector<attribute_id> &join_key_attributes,
                     const bool any_join_key_attributes_nullable,
                     const block_id build_block_id,
                     JoinHashTable *hash_table,
                     StorageManager *storage_manager,
                     LIPFilterBuilder *lip_filter_builder)
      : WorkOrder(query_id),
        input_relation_(input_relation),
        join_key_attributes_(join_key_attributes),
        any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
        build_block_id_(build_block_id),
        hash_table_(DCHECK_NOTNULL(hash_table)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        lip_filter_builder_(lip_filter_builder) {}

  /**
   * @brief Constructor for the distributed version.
   *
   * @param query_id The ID of the query.
   * @param input_relation The relation to build hash table on.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param build_block_id The block id.
   * @param hash_table The JoinHashTable to use.
   * @param storage_manager The StorageManager to use.
   * @param lip_filter_builder The attached LIP filter builer.
   **/
  BuildHashWorkOrder(const std::size_t query_id,
                     const CatalogRelationSchema &input_relation,
                     std::vector<attribute_id> &&join_key_attributes,
                     const bool any_join_key_attributes_nullable,
                     const block_id build_block_id,
                     JoinHashTable *hash_table,
                     StorageManager *storage_manager,
                     LIPFilterBuilder *lip_filter_builder)
      : WorkOrder(query_id),
        input_relation_(input_relation),
        join_key_attributes_(std::move(join_key_attributes)),
        any_join_key_attributes_nullable_(any_join_key_attributes_nullable),
        build_block_id_(build_block_id),
        hash_table_(DCHECK_NOTNULL(hash_table)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        lip_filter_builder_(lip_filter_builder) {}

  ~BuildHashWorkOrder() override {}

  const CatalogRelationSchema& input_relation() const {
    return input_relation_;
  }

  void execute() override;

 private:
  const CatalogRelationSchema &input_relation_;
  const std::vector<attribute_id> join_key_attributes_;
  const bool any_join_key_attributes_nullable_;
  const block_id build_block_id_;

  JoinHashTable *hash_table_;
  StorageManager *storage_manager_;

  std::unique_ptr<LIPFilterBuilder> lip_filter_builder_;

  DISALLOW_COPY_AND_ASSIGN(BuildHashWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_BUILD_HASH_OPERATOR_HPP_
