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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_BUILD_GENERALIZED_HASH_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_BUILD_GENERALIZED_HASH_OPERATOR_HPP_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "relational_operators/BuildHashOperator.hpp"

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionScheme.hpp"
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
class BuildGeneralizedHashOperator : public BuildHashOperator {
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
   * @param num_partitions The number of partitions in 'probe_relation'. If no
   *        partitions, it is one.
   * @param hash_table_index The index of the JoinHashTable in QueryContext.
   *        The HashTable's key Type(s) should be the Type(s) of the
   *        join_key_attributes in input_relation.
   * @param build_predicate_index The index of the build_predicate in QueryContext.
   **/
  BuildGeneralizedHashOperator(const std::size_t query_id,
                               const CatalogRelation &input_relation,
                               const bool input_relation_is_stored,
                               const std::vector<attribute_id> &join_key_attributes,
                               const bool any_join_key_attributes_nullable,
                               const CatalogRelation &second_input_relation,
                               const bool second_input_relation_is_stored,
                               const std::vector<attribute_id> &second_join_key_attributes,
                               const bool any_second_join_key_attributes_nullable,
                               const std::size_t num_partitions,
                               const QueryContext::join_hash_table_id hash_table_index,
                               const QueryContext::join_hash_table_id second_hash_table_index,
                               const QueryContext::predicate_id build_predicate_index)
      : BuildHashOperator(query_id, input_relation, input_relation_is_stored, join_key_attributes,
                          any_join_key_attributes_nullable, num_partitions, hash_table_index,
                          build_predicate_index),
        second_input_relation_(second_input_relation),
        second_input_relation_is_stored_(second_input_relation_is_stored),
        second_join_key_attributes_(second_join_key_attributes),
        any_second_join_key_attributes_nullable_(any_second_join_key_attributes_nullable),
        second_hash_table_index_(second_hash_table_index),
        second_input_relation_block_ids_(num_partitions) {
          if (second_input_relation_is_stored) {
            if (second_input_relation.hasPartitionScheme()) {
              DCHECK_EQ(num_partitions_, second_input_relation.getNumPartitions());

              const PartitionScheme &part_scheme = *second_input_relation.getPartitionScheme();
              for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
                second_input_relation_block_ids_[part_id] = part_scheme.getBlocksInPartition(part_id);
              }
            } else {
              // Broadcast hash join if build has no partitions.
              for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
                second_input_relation_block_ids_[part_id] = second_input_relation.getBlocksSnapshot();
              }
            }
          }
        }

  ~BuildGeneralizedHashOperator() override {}

  const CatalogRelation& second_input_relation() const {
    return second_input_relation_;
  }

  OperatorType getOperatorType() const override {
    return kBuildGeneralizedHash;
  }

  std::string getName() const override {
    return "BuildGeneralizedHashOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

 private:
  /**
   * @brief Create Work Order proto.
   *
   * @param block The block id used in the Work Order.
   * @param part_id The partition id of 'input_relation_'.
   **/
  serialization::WorkOrder* createWorkOrderProto(const block_id block, const block_id second_block, const partition_id part_id);

  const CatalogRelation &second_input_relation_;
  const bool second_input_relation_is_stored_;
  const std::vector<attribute_id> second_join_key_attributes_;
  const bool any_second_join_key_attributes_nullable_;
  const QueryContext::join_hash_table_id second_hash_table_index_;

  // The index is the partition id.
  std::vector<BlocksInPartition> input_relation_block_ids_;
  std::vector<BlocksInPartition> second_input_relation_block_ids_;
  std::vector<std::size_t> num_workorders_generated_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(BuildGeneralizedHashOperator);
};

/**
 * @brief A WorkOrder produced by BuildHashOperator
 **/
class BuildGeneralizedHashWorkOrder : public BuildHashWorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query.
   * @param input_relation The relation to build hash table on.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param part_id The partition id of 'input_relation'.
   * @param build_block_id The block id.
   * @param predicate The Predicate to use.
   * @param hash_table The JoinHashTable to use.
   * @param storage_manager The StorageManager to use.
   * @param lip_filter_builder The attached LIP filter builer.
   **/
  BuildGeneralizedHashWorkOrder(const std::size_t query_id,
                     const CatalogRelationSchema &input_relation,
                     const CatalogRelationSchema &second_input_relation,
                     const std::vector<attribute_id> &join_key_attributes,
                     const std::vector<attribute_id> &second_join_key_attributes,
                     const bool any_join_key_attributes_nullable,
                     const bool any_second_join_key_attributes_nullable,
                     const partition_id part_id,
                     const block_id build_block_id,
                     const block_id second_build_block_id,
                     const Predicate *predicate,
                     JoinHashTable *hash_table,
                     JoinHashTable *second_hash_table,
                     StorageManager *storage_manager,
                     LIPFilterBuilder *lip_filter_builder)
      : BuildHashWorkOrder(query_id, input_relation, join_key_attributes, any_join_key_attributes_nullable,
                           part_id, build_block_id, predicate, hash_table, storage_manager, lip_filter_builder),
        second_input_relation_(second_input_relation),
        second_join_key_attributes_(second_join_key_attributes),
        any_second_join_key_attributes_nullable_(any_second_join_key_attributes_nullable),
        second_build_block_id_(second_build_block_id),
        second_hash_table_(second_hash_table) {}

  /**
   * @brief Constructor for the distributed version.
   *
   * @param query_id The ID of the query.
   * @param input_relation The relation to build hash table on.
   * @param join_key_attributes The IDs of equijoin attributes in
   *        input_relation.
   * @param any_join_key_attributes_nullable If any attribute is nullable.
   * @param part_id The partition id of 'input_relation'.
   * @param build_block_id The block id.
   * @param predicate The Predicate to use.
   * @param hash_table The JoinHashTable to use.
   * @param storage_manager The StorageManager to use.
   * @param lip_filter_builder The attached LIP filter builer.
   **/
  BuildGeneralizedHashWorkOrder(const std::size_t query_id,
                     const CatalogRelationSchema &input_relation,
                     const CatalogRelationSchema &second_input_relation,
                     std::vector<attribute_id> &&join_key_attributes,
                     std::vector<attribute_id> &&second_join_key_attributes,
                     const bool any_join_key_attributes_nullable,
                     const bool any_second_join_key_attributes_nullable,
                     const partition_id part_id,
                     const block_id build_block_id,
                     const block_id second_build_block_id,
                     const Predicate *predicate,
                     JoinHashTable *hash_table,
                     JoinHashTable *second_hash_table,
                     StorageManager *storage_manager,
                     LIPFilterBuilder *lip_filter_builder)
      : BuildHashWorkOrder(query_id, input_relation, join_key_attributes,
                           any_join_key_attributes_nullable, part_id, build_block_id, predicate,
                           hash_table, storage_manager, lip_filter_builder),
        second_input_relation_(second_input_relation),
        second_join_key_attributes_(std::move(second_join_key_attributes)),
        any_second_join_key_attributes_nullable_(any_second_join_key_attributes_nullable),
        second_build_block_id_(second_build_block_id),
        second_hash_table_(second_hash_table) {}

  ~BuildGeneralizedHashWorkOrder() override {}

  const CatalogRelationSchema& second_input_relation() const {
    return second_input_relation_;
  }

  void execute() override;

 private:
  const CatalogRelationSchema &second_input_relation_;
  const std::vector<attribute_id> second_join_key_attributes_;
  const bool any_second_join_key_attributes_nullable_;
  const block_id second_build_block_id_;

  JoinHashTable *second_hash_table_;

  DISALLOW_COPY_AND_ASSIGN(BuildGeneralizedHashWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_BUILD_GENRALIZED_HASH_OPERATOR_HPP_
