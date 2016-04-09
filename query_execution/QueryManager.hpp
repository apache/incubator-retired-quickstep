/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "utility/DAG.hpp"
#include "utility/Macros.hpp"

#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {

class CatalogDatabaseLite;
class ForemanMessage;
class QueryHandle;
class StorageManager;
class WorkerMessage;

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class that manages the execution of a query including generation
 *        of new work orders, keeping track of the query exection state.
 **/
class QueryManager {
 public:
  typedef DAG<RelationalOperator, bool>::size_type_nodes dag_node_index;

  /**
   * @brief Return codes for processMessage() function.
   *
   * @note When both operator and query get executed, kQueryExecuted takes
   *       precedence over kOperatorExecuted.
   **/
  enum class QueryStatusCode {
    kOperatorExecuted = 0,  // An operator in the query finished execution.
    kQueryExecuted,         // The query got executed.
    kNone                   // None of the above.
  };

  /**
   * @brief Constructor.
   *
   * @param foreman_client_id The TMB client ID of the foreman thread.
   * @param num_numa_nodes The number of NUMA nodes used by the system.
   * @param query_handle The QueryHandle object for this query.
   * @param catalog_database The CatalogDatabse used by the query.
   * @param storage_manager The StorageManager used by the query.
   * @param bus The TMB used for communication.
   **/
  QueryManager(const tmb::client_id foreman_client_id,
               const int num_numa_nodes,
               QueryHandle *query_handle,
               CatalogDatabaseLite *catalog_database,
               StorageManager *storage_manager,
               tmb::MessageBus *bus);

 /**
   * @brief Get the next workorder to be excuted, wrapped in a WorkerMessage.
   *
   * @param start_operator_index Begin the search for the schedulable WorkOrder
   *        with the operator at this index.
   * @param numa_node The next WorkOrder should preferably have its input(s)
   *        from this numa_node. This is a hint and not a binding requirement.
   *
   * @return A pointer to the WorkerMessage. If there's no WorkOrder to be
   *         executed, return NULL.
   **/
  WorkerMessage* getNextWorkerMessage(
      const dag_node_index start_operator_index, const int numa_node = -1);

  /**
   * @brief Process a message sent to the QueryManager.
   *
   * @param tagged_message TaggedMessage sent to the QueryManager.
   *
   * @return QueryStatusCode as determined after the message is processed.
   **/
  QueryStatusCode processMessage(const TaggedMessage &tagged_message);

  /**
   * @brief Get the QueryExecutionState for this query.
   **/
  inline const QueryExecutionState& getQueryExecutionState() const {
    return *(query_exec_state_.get());
  }

  /**
   * @brief Get a pointer to the QueryContext.
   **/
  inline QueryContext* getQueryContextMutable() {
    return query_context_.get();
  }

 private:
  /**
   * @brief Process the received WorkOrder complete message.
   *
   * @param node_index The index of the specified operator node in the query DAG
   *        for the completed WorkOrder.
   **/
  void processWorkOrderCompleteMessage(const dag_node_index op_index);

  /**
   * @brief Process the received RebuildWorkOrder complete message.
   *
   * @param node_index The index of the specified operator node in the query DAG
   *        for the completed RebuildWorkOrder.
   **/
  void processRebuildWorkOrderCompleteMessage(const dag_node_index op_index);

  /**
   * @brief Process a current relational operator: Get its workorders and store
   *        them in the WorkOrdersContainer for this query. If the operator can
   *        be marked as done, do so.
   *
   * @param index The index of the relational operator to be processed in the
   *        query plan DAG.
   * @param recursively_check_dependents If an operator is done, should we
   *        call processOperator on its dependents recursively.
   **/
  void processOperator(const dag_node_index index,
                       const bool recursively_check_dependents);

  /**
   * @brief Process the received data pipeline message.
   *
   * @param node_index The index of the specified operator node in the query DAG
   *        for the pipelining block.
   * @param block The block id.
   * @param rel_id The ID of the relation that produced 'block'.
   **/
  void processDataPipelineMessage(const dag_node_index op_index,
                                  const block_id block,
                                  const relation_id rel_id);

  /**
   * @brief Process the received work order feedback message and notify
   *        relational operator.
   *
   * @param message Feedback message from work order.
   **/
  void processFeedbackMessage(const WorkOrder::FeedbackMessage &message);

  /**
   * @brief Fetch all work orders currently available in relational operator and
   *        store them internally.
   *
   * @param index The index of the relational operator to be processed in the
   *        query plan DAG.
   *
   * @return Whether any work order was generated by op.
   **/
  bool fetchNormalWorkOrders(const dag_node_index index);

  /**
   * @brief This function does the following things:
   *        1. Mark the given relational operator as "done".
   *        2. For all the dependents of this operator, check if all of their
   *        blocking dependencies are met. If so inform them that the blocking
   *        dependencies are met.
   *        3. Check if the given operator is done producing output. If it's
   *        done, inform the dependents that they won't receive input anymore
   *        from the given operator.
   *
   * @param index The index of the given relational operator in the DAG.
   **/
  void markOperatorFinished(const dag_node_index index);

  /**
   * @brief Check if all the dependencies of the node at specified index have
   *        finished their execution.
   *
   * @note This function's true return value is a pre-requisite for calling
   *       getRebuildWorkOrders()
   *
   * @param node_index The index of the specified node in the query DAG.
   *
   * @return True if all the dependencies have finished their execution. False
   *         otherwise.
   **/
  inline bool checkAllDependenciesMet(const dag_node_index node_index) const {
    for (const dag_node_index dependency_index :
         query_dag_->getDependencies(node_index)) {
      // If at least one of the dependencies is not met, return false.
      if (!query_exec_state_->hasExecutionFinished(dependency_index)) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Check if all the blocking dependencies of the node at specified
   *        index have finished their execution.
   *
   * @note A blocking dependency is the one which is pipeline breaker. Output of
   *       a dependency can't be streamed to its dependent if the link between
   *       them is pipeline breaker.
   *
   * @param node_index The index of the specified node in the query DAG.
   *
   * @return True if all the blocking dependencies have finished their
   *         execution. False otherwise.
   **/
  inline bool checkAllBlockingDependenciesMet(
      const dag_node_index node_index) const {
    for (const dag_node_index blocking_dependency_index :
         blocking_dependencies_[node_index]) {
      if (!query_exec_state_->hasExecutionFinished(
              blocking_dependency_index)) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Check if the execution of the given operator is over.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the execution of the given operator is over, false
   *         otherwise.
   **/
  inline bool checkOperatorExecutionOver(const dag_node_index index) const {
    if (checkRebuildRequired(index)) {
      return (checkNormalExecutionOver(index) && checkRebuildOver(index));
    } else {
      return checkNormalExecutionOver(index);
    }
  }

  /**
   * @brief Check if the given operator's normal execution is over.
   *
   * @note The conditions for a given operator's normal execution to get over:
   *       1. All of its  normal (i.e. non rebuild) WorkOrders have finished
   *       execution.
   *       2. The operator is done generating work orders.
   *       3. All of the dependencies of the given operator have been met.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the normal execution of the given operator is over, false
   *         otherwise.
   **/
  inline bool checkNormalExecutionOver(const dag_node_index index) const {
    return (checkAllDependenciesMet(index) &&
            !workorders_container_->hasNormalWorkOrder(index) &&
            query_exec_state_->getNumQueuedWorkOrders(index) == 0 &&
            query_exec_state_->hasDoneGenerationWorkOrders(index));
  }

  /**
   * @brief Check if the rebuild operation is required for a given operator.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the rebuild operation is required, false otherwise.
   **/
  inline bool checkRebuildRequired(const dag_node_index index) const {
    return query_exec_state_->isRebuildRequired(index);
  }

  /**
   * @brief Check if the rebuild operation for a given operator is over.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the rebuild operation is over, false otherwise.
   **/
  inline bool checkRebuildOver(const dag_node_index index) const {
    return query_exec_state_->hasRebuildInitiated(index) &&
           !workorders_container_->hasRebuildWorkOrder(index) &&
           (query_exec_state_->getNumRebuildWorkOrders(index) == 0);
  }

  /**
   * @brief Check if the rebuild operation for a given operator has been
   *        initiated.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the rebuild operation has been initiated, false otherwise.
   **/
  inline bool checkRebuildInitiated(const dag_node_index index) const {
    return query_exec_state_->hasRebuildInitiated(index);
  }

  /**
   * @brief Initiate the rebuild process for partially filled blocks generated
   *        during the execution of the given operator.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the rebuild is over immediately, i.e. the operator didn't
   *         generate any rebuild WorkOrders, false otherwise.
   **/
  bool initiateRebuild(const dag_node_index index);

  /**
   * @brief Get the rebuild WorkOrders for an operator.
   *
   * @note This function should be called only once, when all the normal
   *       WorkOrders generated by an operator finish their execution.
   *
   * @param index The index of the operator in the query plan DAG.
   * @param container A pointer to a WorkOrdersContainer to be used to store the
   *        generated WorkOrders.
   **/
  void getRebuildWorkOrders(const dag_node_index index,
                            WorkOrdersContainer *container);

  const tmb::client_id foreman_client_id_;
  const std::size_t query_id_;

  CatalogDatabaseLite *catalog_database_;
  StorageManager *storage_manager_;
  tmb::MessageBus *bus_;

  DAG<RelationalOperator, bool> *query_dag_;

  std::unique_ptr<QueryContext> query_context_;

  // For all nodes, store their receiving dependents.
  std::vector<std::vector<dag_node_index>> output_consumers_;

  // For all nodes, store their pipeline breaking dependencies (if any).
  std::vector<std::vector<dag_node_index>> blocking_dependencies_;

  std::unique_ptr<QueryExecutionState> query_exec_state_;

  std::unique_ptr<WorkOrdersContainer> workorders_container_;

  DISALLOW_COPY_AND_ASSIGN(QueryManager);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_MANAGER_HPP_
