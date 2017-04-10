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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_RELATIONAL_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_RELATIONAL_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator in a relational query plan. The query plan is
 *        a directed acyclic graph of RelationalOperators.
 **/
class RelationalOperator {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~RelationalOperator() {}

  /**
   * @brief Enumeration of the operator types.
   **/
  enum OperatorType {
    kAggregation = 0,
    kBuildAggregationExistenceMap,
    kBuildHash,
    kBuildLIPFilter,
    kCreateIndex,
    kCreateTable,
    kDelete,
    kDestroyAggregationState,
    kDestroyHash,
    kDropTable,
    kFinalizeAggregation,
    kInitializeAggregation,
    kInnerJoin,
    kInsert,
    kLeftAntiJoin,
    kLeftOuterJoin,
    kLeftSemiJoin,
    kNestedLoopsJoin,
    kSample,
    kSaveBlocks,
    kSelect,
    kSortMergeRun,
    kSortRunGeneration,
    kTableGenerator,
    kTextScan,
    kUnionAll,
    kUpdate,
    kWindowAggregation,
    kMockOperator
  };

  /**
   * @brief Get the type of the operator.
   **/
  virtual OperatorType getOperatorType() const = 0;

  /**
   * @brief Get the name of this relational operator.
   *
   * @return The name of this relational operator.
   */
  virtual std::string getName() const = 0;

  /**
   * @brief Generate all the next WorkOrders for this RelationalOperator.
   *
   * @note If a RelationalOperator has blocking dependencies, it should not
   *       generate workorders unless all of the blocking dependencies have been
   *       met.
   *
   * @note If a RelationalOperator is not parallelizeable on a block-level, then
   *       only one WorkOrder consisting of all the work for this
   *       RelationalOperator should be generated.
   *
   * @param container A pointer to a WorkOrdersContainer to be used to store the
   *        generated WorkOrders.
   * @param query_context The QueryContext that stores query execution states.
   * @param storage_manager The StorageManager to use.
   * @param scheduler_client_id The TMB client ID of the scheduler thread.
   * @param bus A pointer to the TMB.
   *
   * @return Whether the operator has finished generating work orders. If \c
   *         false, the execution engine will invoke this method after at least
   *         one pending work order has finished executing.
   **/
  virtual bool getAllWorkOrders(WorkOrdersContainer *container,
                                QueryContext *query_context,
                                StorageManager *storage_manager,
                                const tmb::client_id scheduler_client_id,
                                tmb::MessageBus *bus) = 0;

  /**
    * @brief For the distributed version, generate all the next WorkOrder protos
    *        for this RelationalOperator
    *
    * @note If a RelationalOperator has blocking dependencies, it should not
    *       generate workorders unless all of the blocking dependencies have been
    *       met.
    *
    * @note If a RelationalOperator is not parallelizeable on a block-level, then
    *       only one WorkOrder consisting of all the work for this
    *       RelationalOperator should be generated.
    *
    * @param container A pointer to a WorkOrderProtosContainer to be used to
    *        store the generated WorkOrder protos.
    *
    * @return Whether the operator has finished generating work order protos. If
    *         \c false, the execution engine will invoke this method after at
    *         least one pending work order has finished executing.
    **/
  virtual bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) = 0;

  /**
   * @brief Update Catalog upon the completion of this RelationalOperator, if
   *        necessary.
   *
   **/
  virtual void updateCatalogOnCompletion() {
  }

  /**
   * @brief Inform this RelationalOperator that ALL the dependencies which break
   *        the pipeline have been met.
   *
   * @note This function is only relevant in certain operators like HashJoin
   *       which have a pipeline breaking dependency on BuildHash operator.
   *       Such operators can start generating WorkOrders when all the pipeline
   *       breaking dependencies are met.
   **/
  inline void informAllBlockingDependenciesMet() {
    blocking_dependencies_met_ = true;
  }

  /**
   * @brief Receive input blocks for this RelationalOperator.
   *
   * @param input_block_id The ID of the input block.
   * @param relation_id The ID of the relation that produced this input_block.
   * @param part_id The partition ID of 'input_block_id'.
   **/
  virtual void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id,
                              const partition_id part_id) {}

  /**
   * @brief Signal the end of feeding of input blocks for this
   * RelationalOperator.
   *
   * @param rel_id ID of the relation that finished producing blocks.
   *
   * @note A RelationalOperator that does not override this method can use \c
   *       done_feeding_input_relation_ member variable to figure out that this
   *       event was received. However, this does not distinguish between input
   *       relations. If the operator uses more than one input relation, it will
   *       need to override to this method to figure when the event is called
   *       for each of its relations.
   *
   * @note This method is invoke for both streaming and non-streaming operators.
   *       In the streaming case, this corresponds to end input to specified
   *       relation. In both the cases, this corresponds to event of the end of
   *       execution of downstream operator feeding input to this operator by
   *       specified relation.
   **/
  virtual void doneFeedingInputBlocks(const relation_id rel_id) {
    done_feeding_input_relation_ = true;
  }

  /**
   * @brief Get the InsertDestination index in the QueryContext, if used by
   *        this RelationalOperator for storing the output of the workorders'
   *        execution.
   *
   * @return If there is an InsertDestination for this RelationalOperator,
   *         return the non-negative id, otherwise return
   *         kInvalidInsertDestinationId.
   **/
  virtual QueryContext::insert_destination_id getInsertDestinationID() const {
    return QueryContext::kInvalidInsertDestinationId;
  }

  /**
   * @brief Get the relation ID of the output relation.
   *
   * @note If a derived RelationalOperator produces a output relation using
   *       InsertDestination, it should override this virtual method to return
   *       its relation_id.
   *
   * @note If a derived RelationalOperator (e.g., DeleteOperator) does not use
   *       InsertDestination to write blocks to the output, it should also
   *       override this virtual method to return its relation_id.
   **/
  virtual const relation_id getOutputRelationID() const {
    return -1;
  }

  /**
   * @brief Callback to receive feedback messages from work orders.
   *
   * @param message Feedback message received by the Foreman from work order on
   *                behalf of the relational operator.
   * @note The relational operator needs to override this default
   *       implementation to receive messages. Most operators don't need to use
   *       this API. Only multi-pass operators like the sort-merge-runs
   *       operator and the text-scan operator (with parallelized loads) will
   *       need to use this API to convey information from work orders to the
   *       relational operator.
   **/
  virtual void receiveFeedbackMessage(
      const WorkOrder::FeedbackMessage &message) {
    LOG(FATAL) << "Received a feedback message on default interface. "
               << "Operator has no implementation.";
  }

  /**
   * @brief Set the index of this operator in the query plan DAG.
   *
   * @param operator_index The index of this operator in the query plan DAG.
   **/
  void setOperatorIndex(const std::size_t operator_index) {
    op_index_ = operator_index;
  }

  /**
   * @brief Get the index of this operator in the query plan DAG.
   *
   * @return The index of this operator in the query plan DAG.
   */
  std::size_t getOperatorIndex() const {
    return op_index_;
  }

  /**
   * @brief Deploy a group of LIPFilters to this operator.
   */
  void deployLIPFilters(const QueryContext::lip_deployment_id lip_deployment_index) {
    lip_deployment_index_ = lip_deployment_index;
  }

 protected:
  /**
   * @brief Constructor
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param blocking_dependencies_met If those dependencies which break the
   *        pipeline have been met.
   **/
  explicit RelationalOperator(const std::size_t query_id,
                              const bool blocking_dependencies_met = false)
      : query_id_(query_id),
        blocking_dependencies_met_(blocking_dependencies_met),
        done_feeding_input_relation_(false),
        lip_deployment_index_(QueryContext::kInvalidLIPDeploymentId) {}

  const std::size_t query_id_;

  bool blocking_dependencies_met_;
  bool done_feeding_input_relation_;
  std::size_t op_index_;

  QueryContext::lip_deployment_id lip_deployment_index_;

 private:
  DISALLOW_COPY_AND_ASSIGN(RelationalOperator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_RELATIONAL_OPERATOR_HPP_
