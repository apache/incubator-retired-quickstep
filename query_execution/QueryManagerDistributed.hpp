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

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_DISTRIBUTED_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_DISTRIBUTED_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryManagerBase.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "utility/Macros.hpp"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class QueryHandle;

namespace serialization { class WorkOrderMessage; }

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class that manages the execution of a query including generation
 *        of new work orders, keeping track of the query exection state.
 **/
class QueryManagerDistributed final : public QueryManagerBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_handle The QueryHandle object for this query.
   * @param foreman_client_id The TMB client ID of the foreman thread.
   * @param num_shiftbosses The number of Shiftbosses for rebuild.
   * @param shiftboss_addresses The TMB Address of Shiftbosses for rebuild.
   * @param bus The TMB used for communication.
   **/
  QueryManagerDistributed(QueryHandle *query_handle,
                          const tmb::client_id foreman_client_id,
                          const std::size_t num_shiftbosses,
                          tmb::Address &&shiftboss_addresses,
                          tmb::MessageBus *bus);

  ~QueryManagerDistributed() override {}

  bool fetchNormalWorkOrders(const dag_node_index index) override;

  /**
   * @brief Process the initiate rebuild work order response message.
   *
   * @param op_index The index of the specified operator node in the query DAG
   *        for initiating the rebuild work order.
   * @param num_rebuild_work_orders The number of the rebuild work orders
   *        generated for the operator indexed by 'op_index'.
   * @param shiftboss_index The index of the Shiftboss that sends the message.
   **/
  void processInitiateRebuildResponseMessage(const dag_node_index op_index,
                                             const std::size_t num_rebuild_work_orders,
                                             const std::size_t shiftboss_index);

  /**
   * @brief Get the next normal workorder to be excuted, wrapped in a
   *        WorkOrderMessage proto.
   *
   * @param start_operator_index Begin the search for the schedulable WorkOrder
   *        with the operator at this index.
   *
   * @return A pointer to the WorkOrderMessage proto. If there is no WorkOrder
   *         to be executed, return NULL.
   **/
  serialization::WorkOrderMessage* getNextWorkOrderMessage(
      const dag_node_index start_operator_index);

  /**
   * @brief Get the index of Shiftboss for an Aggregation related WorkOrder. If
   * the Shiftboss index is not found, set using <next_shiftboss_index_to_schedule>.
   *
   * @param aggr_state_index The Hash Table for the Aggregation.
   * @param next_shiftboss_index The index of Shiftboss to schedule a next WorkOrder.
   * @param shiftboss_index The index of Shiftboss to schedule the WorkOrder.
   **/
  void getShiftbossIndexForAggregation(const QueryContext::aggregation_state_id aggr_state_index,
                                       const std::size_t next_shiftboss_index_to_schedule,
                                       std::size_t *shiftboss_index) {
    DCHECK_LT(aggr_state_index, shiftboss_indexes_for_aggrs_.size());
    if (shiftboss_indexes_for_aggrs_[aggr_state_index] == kInvalidShiftbossIndex) {
      shiftboss_indexes_for_aggrs_[aggr_state_index] = next_shiftboss_index_to_schedule;
    }

    *shiftboss_index = shiftboss_indexes_for_aggrs_[aggr_state_index];
  }

  /**
   * @brief Get the index of Shiftboss for a HashJoin related WorkOrder. If the
   * Shiftboss index is not found, set using <next_shiftboss_index_to_schedule>.
   *
   * @param join_hash_table_index The Hash Table for the Join.
   * @param part_id The partition ID.
   * @param next_shiftboss_index The index of Shiftboss to schedule a next WorkOrder.
   * @param shiftboss_index The index of Shiftboss to schedule the WorkOrder.
   **/
  void getShiftbossIndexForHashJoin(const QueryContext::join_hash_table_id join_hash_table_index,
                                    const partition_id part_id,
                                    const std::size_t next_shiftboss_index_to_schedule,
                                    std::size_t *shiftboss_index) {
    DCHECK_LT(join_hash_table_index, shiftboss_indexes_for_hash_joins_.size());
    DCHECK_LT(part_id, shiftboss_indexes_for_hash_joins_[join_hash_table_index].size());

    if (shiftboss_indexes_for_hash_joins_[join_hash_table_index][part_id] == kInvalidShiftbossIndex) {
      shiftboss_indexes_for_hash_joins_[join_hash_table_index][part_id] = next_shiftboss_index_to_schedule;
    }

    *shiftboss_index = shiftboss_indexes_for_hash_joins_[join_hash_table_index][part_id];
  }

 private:
  bool checkNormalExecutionOver(const dag_node_index index) const override {
    return (checkAllDependenciesMet(index) &&
            !normal_workorder_protos_container_->hasWorkOrderProto(index) &&
            query_exec_state_->getNumQueuedWorkOrders(index) == 0 &&
            query_exec_state_->hasDoneGenerationWorkOrders(index));
  }

  bool initiateRebuild(const dag_node_index index) override;

  bool checkRebuildOver(const dag_node_index index) const override {
    return query_exec_state_->hasRebuildInitiated(index) &&
           query_exec_state_->hasRebuildFinished(index, num_shiftbosses_);
  }

  const tmb::client_id foreman_client_id_;

  // TODO(quickstep-team): deal with Shiftboss failure.
  const std::size_t num_shiftbosses_;
  const tmb::Address shiftboss_addresses_;

  tmb::MessageBus *bus_;

  std::unique_ptr<WorkOrderProtosContainer> normal_workorder_protos_container_;

  // From an aggregation id (QueryContext::aggregation_state_id) to its
  // scheduled Shiftboss index.
  std::vector<std::size_t> shiftboss_indexes_for_aggrs_;

  // Get the scheduled Shiftboss index given
  // [QueryContext::join_hash_table_id][partition_id].
  std::vector<std::vector<std::size_t>> shiftboss_indexes_for_hash_joins_;

  DISALLOW_COPY_AND_ASSIGN(QueryManagerDistributed);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_DISTRIBUTED_HPP_
