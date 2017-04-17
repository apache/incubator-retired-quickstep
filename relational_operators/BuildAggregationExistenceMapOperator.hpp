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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_BUILD_AGGREGATION_EXISTENCE_MAP_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_BUILD_AGGREGATION_EXISTENCE_MAP_OPERATOR_HPP_

#include <cstddef>

#include <string>
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

class AggregationOperationState;
class CatalogRelationSchema;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which builds a bit vector on the input relation's one
 *        attribute where the bit vector serves as the existence map for an
 *        AggregationOperationState's CollisionFreeVectorTable.
 **/
class BuildAggregationExistenceMapOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param input_relation The relation to build the existence map on.
   * @param build_attribute The ID of the attribute to build the existence map on.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   * @param aggr_state_index The index of the AggregationState in QueryContext.
   **/
  BuildAggregationExistenceMapOperator(const std::size_t query_id,
                                       const CatalogRelation &input_relation,
                                       const attribute_id build_attribute,
                                       const bool input_relation_is_stored,
                                       const QueryContext::aggregation_state_id aggr_state_index)
      : RelationalOperator(query_id),
        input_relation_(input_relation),
        build_attribute_(build_attribute),
        input_relation_is_stored_(input_relation_is_stored),
        aggr_state_index_(aggr_state_index),
        input_relation_block_ids_(input_relation_is_stored ? input_relation.getBlocksSnapshot()
                                                           : std::vector<block_id>()),
        num_workorders_generated_(0),
        started_(false) {}

  ~BuildAggregationExistenceMapOperator() override {}

  OperatorType getOperatorType() const override {
    return kBuildAggregationExistenceMap;
  }

  std::string getName() const override {
    return "BuildAggregationExistenceMapOperator";
  }

  /**
   * @return The input relation.
   */
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
    input_relation_block_ids_.push_back(input_block_id);
  }

  QueryContext::aggregation_state_id getAggregationStateIndex() const {
    return aggr_state_index_;
  }

  attribute_id getBuildAttributeID() const {
    return build_attribute_;
  }

 private:
  serialization::WorkOrder* createWorkOrderProto(const block_id block);

  const CatalogRelation &input_relation_;
  const attribute_id build_attribute_;
  const bool input_relation_is_stored_;
  const QueryContext::aggregation_state_id aggr_state_index_;

  std::vector<block_id> input_relation_block_ids_;
  std::vector<block_id>::size_type num_workorders_generated_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(BuildAggregationExistenceMapOperator);
};

/**
 * @brief A WorkOrder produced by BuildAggregationExistenceMapOperator.
 **/
class BuildAggregationExistenceMapWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor
   *
   * @param query_id The ID of this query.
   * @param input_relation The relation to build the existence map on.
   * @param build_block_id The block id.
   * @param build_attribute The ID of the attribute to build on.
   * @param state The AggregationState to use.
   * @param storage_manager The StorageManager to use.
   **/
  BuildAggregationExistenceMapWorkOrder(const std::size_t query_id,
                                        const CatalogRelationSchema &input_relation,
                                        const block_id build_block_id,
                                        const attribute_id build_attribute,
                                        AggregationOperationState *state,
                                        StorageManager *storage_manager)
      : WorkOrder(query_id),
        input_relation_(input_relation),
        build_block_id_(build_block_id),
        build_attribute_(build_attribute),
        state_(DCHECK_NOTNULL(state)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  ~BuildAggregationExistenceMapWorkOrder() override {}

  void execute() override;

 private:
  const CatalogRelationSchema &input_relation_;
  const block_id build_block_id_;
  const attribute_id build_attribute_;

  AggregationOperationState *state_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(BuildAggregationExistenceMapWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_BUILD_AGGREGATION_EXISTENCE_MAP_OPERATOR_HPP_
