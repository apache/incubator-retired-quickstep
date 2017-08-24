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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATOR_HPP_

#include <cstddef>
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

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class AggregationOperationState;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which performs aggregation over a relation.
 **/
class AggregationOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for aggregating with arbitrary expressions in projection
   *        list.
   *
   * @param query_id The ID of this query.
   * @param input_relation The relation to perform aggregation over.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   * @param aggr_state_index The index of the AggregationState in QueryContext.
   * @param num_partitions The number of partitions in 'input_relation'. If no
   *        partitions, it is one.
   **/
  AggregationOperator(const std::size_t query_id,
                      const CatalogRelation &input_relation,
                      bool input_relation_is_stored,
                      const QueryContext::aggregation_state_id aggr_state_index,
                      const std::size_t num_partitions)
      : RelationalOperator(query_id),
        input_relation_(input_relation),
        input_relation_is_stored_(input_relation_is_stored),
        aggr_state_index_(aggr_state_index),
        num_partitions_(num_partitions),
        input_relation_block_ids_(num_partitions),
        num_workorders_generated_(num_partitions),
        started_(false) {
    if (input_relation_is_stored) {
      if (input_relation.hasPartitionScheme()) {
        const PartitionScheme &part_scheme = *input_relation.getPartitionScheme();
        for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
          input_relation_block_ids_[part_id] = part_scheme.getBlocksInPartition(part_id);
        }
      } else {
        // No partition.
        input_relation_block_ids_[0] = input_relation.getBlocksSnapshot();
      }
    }
  }

  ~AggregationOperator() override {}

  OperatorType getOperatorType() const override {
    return kAggregation;
  }

  std::string getName() const override {
    return "AggregationOperator";
  }

  const CatalogRelation& input_relation() const {
    return input_relation_;
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id,
                      const partition_id part_id) override {
    input_relation_block_ids_[part_id].push_back(input_block_id);
  }

 private:
  /**
   * @brief Create Work Order proto.
   *
   * @param block The block id used in the Work Order.
   * @param part_id The partition id of 'block'.
   **/
  serialization::WorkOrder* createWorkOrderProto(const block_id block, const partition_id part_id);

  const CatalogRelation &input_relation_;
  const bool input_relation_is_stored_;
  const QueryContext::aggregation_state_id aggr_state_index_;
  const std::size_t num_partitions_;

  // The index is the partition id.
  std::vector<BlocksInPartition> input_relation_block_ids_;
  std::vector<std::size_t> num_workorders_generated_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(AggregationOperator);
};

/**
 * @brief A WorkOrder produced by AggregationOperator.
 **/
class AggregationWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor
   *
   * @param query_id The ID of this query.
   * @param part_id The partition id.
   * @param input_block_id The block id.
   * @param state The AggregationState to use.
   * @param lip_filter_adaptive_prober The attached LIP filter prober.
   **/
  AggregationWorkOrder(const std::size_t query_id,
                       const partition_id part_id,
                       const block_id input_block_id,
                       AggregationOperationState *state,
                       LIPFilterAdaptiveProber *lip_filter_adaptive_prober)
      : WorkOrder(query_id, part_id),
        input_block_id_(input_block_id),
        state_(DCHECK_NOTNULL(state)),
        lip_filter_adaptive_prober_(lip_filter_adaptive_prober) {}

  ~AggregationWorkOrder() override {}

  void execute() override;

 private:
  const block_id input_block_id_;
  AggregationOperationState *state_;

  std::unique_ptr<LIPFilterAdaptiveProber> lip_filter_adaptive_prober_;

  DISALLOW_COPY_AND_ASSIGN(AggregationWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_AGGREGATION_OPERATOR_HPP_
