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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_INITIALIZE_AGGREGATION_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_INITIALIZE_AGGREGATION_OPERATOR_HPP_

#include <string>

#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/Macros.hpp"

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
 * @brief An operator which initializes an AggregationOperationState.
 **/
class InitializeAggregationOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of this query.
   * @param aggr_state_index The index of the AggregationOperationState in QueryContext.
   * @param num_partitions The number of partitions in 'input_relation'. If no
   *        partitions, it is one.
   **/
  InitializeAggregationOperator(const std::size_t query_id,
                                const QueryContext::aggregation_state_id aggr_state_index,
                                const std::size_t num_partitions)
      : RelationalOperator(query_id),
        aggr_state_index_(aggr_state_index),
        num_partitions_(num_partitions),
        started_(false) {}

  ~InitializeAggregationOperator() override {}

  OperatorType getOperatorType() const override {
    return kInitializeAggregation;
  }

  std::string getName() const override {
    return "InitializeAggregationOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

 private:
  const QueryContext::aggregation_state_id aggr_state_index_;
  const std::size_t num_partitions_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(InitializeAggregationOperator);
};

/**
 * @brief A WorkOrder produced by InitializeAggregationOperator.
 **/
class InitializeAggregationWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param state_partition_id The partition ID for which the work order is issued.
   * @param state The AggregationOperationState to be initialized.
   */
  InitializeAggregationWorkOrder(const std::size_t query_id,
                                 const std::size_t state_partition_id,
                                 AggregationOperationState *state)
      : WorkOrder(query_id),
        state_partition_id_(state_partition_id),
        state_(DCHECK_NOTNULL(state)) {}

  ~InitializeAggregationWorkOrder() override {}

  void execute() override;

 private:
  const std::size_t state_partition_id_;

  AggregationOperationState *state_;

  DISALLOW_COPY_AND_ASSIGN(InitializeAggregationWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_INITIALIZE_AGGREGATION_OPERATOR_HPP_
