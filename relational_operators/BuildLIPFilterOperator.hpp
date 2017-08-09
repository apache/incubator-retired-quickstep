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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_BUILD_LIP_FILTER_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_BUILD_LIP_FILTER_OPERATOR_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionScheme.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"
#include "utility/lip_filter/LIPFilterBuilder.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class Predicate;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which builds LIPFilters on one relation.
 **/
class BuildLIPFilterOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @note The LIPFilters' information are not passed explicitly as parameters
   *       to this constructor, but attached later via RelationalOperator::deployLIPFilters().
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param input_relation The relation to build LIP filters on.
   * @param build_side_predicate_index The index of the predicate in QueryContext
   *        where the predicate is to be applied to the input relation before
   *        building the LIP filters (or kInvalidPredicateId if no predicate is
   *        to be applied).
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   **/
  BuildLIPFilterOperator(const std::size_t query_id,
                         const CatalogRelation &input_relation,
                         const QueryContext::predicate_id build_side_predicate_index,
                         const bool input_relation_is_stored)
    : RelationalOperator(query_id, input_relation.getNumPartitions()),
      input_relation_(input_relation),
      build_side_predicate_index_(build_side_predicate_index),
      input_relation_is_stored_(input_relation_is_stored),
      input_relation_block_ids_(num_partitions_),
      num_workorders_generated_(num_partitions_),
      started_(false) {
    if (input_relation_is_stored) {
      if (input_relation.hasPartitionScheme()) {
        const PartitionScheme &part_scheme = *input_relation.getPartitionScheme();
        for (partition_id part_id = 0; part_id < num_partitions_; ++part_id) {
          input_relation_block_ids_[part_id] = part_scheme.getBlocksInPartition(part_id);
        }
      } else {
        // No partitions.
        input_relation_block_ids_[0] = input_relation.getBlocksSnapshot();
      }
    }
  }

  ~BuildLIPFilterOperator() override {}

  /**
   * @return The input relation to build LIP filters on.
   */
  const CatalogRelation& input_relation() const {
    return input_relation_;
  }

  OperatorType getOperatorType() const override {
    return kBuildLIPFilter;
  }

  std::string getName() const override {
    return "BuildLIPFilterOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id,
                      const partition_id part_id) override {
    input_relation_block_ids_[part_id].push_back(input_block_id);
  }

 private:
  /**
   * @brief Create Work Order proto.
   *
   * @param part_id The partition id of 'input_relation_'.
   * @param block The block id used in the Work Order.
   **/
  serialization::WorkOrder* createWorkOrderProto(const partition_id part_id, const block_id block);

  const CatalogRelation &input_relation_;
  const QueryContext::predicate_id build_side_predicate_index_;
  const bool input_relation_is_stored_;

  // The index is the partition id.
  std::vector<BlocksInPartition> input_relation_block_ids_;
  std::vector<std::size_t> num_workorders_generated_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(BuildLIPFilterOperator);
};

/**
 * @brief A WorkOrder produced by BuildLIPFilterOperator.
 **/
class BuildLIPFilterWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param input_relation The relation to build LIP filters on.
   * @param part_id The partition id of 'input_relation'.
   * @param build_block_id The block id.
   * @param build_side_predicate The predicate to be applied to filter the input
   *        relation before building the LIP filters (or nullptr if no predicate
   *        is to be applied).
   * @param storage_manager The StorageManager to use.
   * @param lip_filter_adaptive_prober The attached LIP filter prober.
   * @param lip_filter_builder The attached LIP filter builder.
   **/
  BuildLIPFilterWorkOrder(const std::size_t query_id,
                          const CatalogRelationSchema &input_relation,
                          const partition_id part_id,
                          const block_id build_block_id,
                          const Predicate *build_side_predicate,
                          StorageManager *storage_manager,
                          LIPFilterAdaptiveProber *lip_filter_adaptive_prober,
                          LIPFilterBuilder *lip_filter_builder)
      : WorkOrder(query_id, part_id),
        input_relation_(input_relation),
        build_block_id_(build_block_id),
        build_side_predicate_(build_side_predicate),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        lip_filter_adaptive_prober_(lip_filter_adaptive_prober),
        lip_filter_builder_(DCHECK_NOTNULL(lip_filter_builder)) {}

  ~BuildLIPFilterWorkOrder() override {}

  /**
   * @return The input relation to build LIP filters on.
   */
  const CatalogRelationSchema& input_relation() const {
    return input_relation_;
  }

  void execute() override;

 private:
  const CatalogRelationSchema &input_relation_;
  const block_id build_block_id_;
  const Predicate *build_side_predicate_;

  StorageManager *storage_manager_;

  std::unique_ptr<LIPFilterAdaptiveProber> lip_filter_adaptive_prober_;
  std::unique_ptr<LIPFilterBuilder> lip_filter_builder_;

  DISALLOW_COPY_AND_ASSIGN(BuildLIPFilterWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_BUILD_LIP_FILTER_OPERATOR_HPP_
