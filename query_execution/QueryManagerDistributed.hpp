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

#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryManagerBase.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class QueryHandle;
class ShiftbossDirectory;

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
   * @param shiftboss_directory The ShiftbossDirectory to use.
   * @param foreman_client_id The TMB client ID of the foreman thread.
   * @param bus The TMB used for communication.
   **/
  QueryManagerDistributed(QueryHandle *query_handle,
                          const ShiftbossDirectory *shiftboss_directory,
                          const tmb::client_id foreman_client_id,
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
           (query_exec_state_->getNumRebuildWorkOrders(index) == 0);
  }

  const ShiftbossDirectory *shiftboss_directory_;

  const tmb::client_id foreman_client_id_;
  tmb::MessageBus *bus_;

  std::unique_ptr<WorkOrderProtosContainer> normal_workorder_protos_container_;

  DISALLOW_COPY_AND_ASSIGN(QueryManagerDistributed);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_DISTRIBUTED_HPP_
