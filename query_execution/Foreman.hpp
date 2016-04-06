/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_EXECUTION_FOREMAN_HPP_
#define QUICKSTEP_QUERY_EXECUTION_FOREMAN_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/ForemanLite.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/DAG.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest_prod.h"

#include "tmb/message_bus.h"

namespace quickstep {

class CatalogDatabaseLite;
class StorageManager;
class WorkerDirectory;

namespace serialization { class QueryContext; }

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief The Foreman scans the query DAG, requests each operator to produce
 *        workorders. It also pipelines the intermediate output it receives to
 *        the relational operators which need it.
 **/
class Foreman final : public ForemanLite {
 public:
  /**
   * @brief Constructor.
   *
   * @param bus A pointer to the TMB.
   * @param catalog_database The catalog database where this query is executed.
   * @param storage_manager The StorageManager to use.
   * @param cpu_id The ID of the CPU to which the Foreman thread can be pinned.
   * @param num_numa_nodes The number of NUMA nodes in the system.
   *
   * @note If cpu_id is not specified, Foreman thread can be possibly moved
   *       around on different CPUs by the OS.
  **/
  Foreman(tmb::MessageBus *bus,
          CatalogDatabaseLite *catalog_database,
          StorageManager *storage_manager,
          const int cpu_id = -1,
          const int num_numa_nodes = 1)
      : ForemanLite(bus, cpu_id),
        catalog_database_(DCHECK_NOTNULL(catalog_database)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        max_msgs_per_worker_(1),
        num_numa_nodes_(num_numa_nodes) {
    bus_->RegisterClientAsSender(foreman_client_id_, kWorkOrderMessage);
    bus_->RegisterClientAsSender(foreman_client_id_, kRebuildWorkOrderMessage);
    // NOTE : Right now, foreman thread doesn't send poison messages. In the
    // future if foreman needs to abort a worker thread, this registration
    // should be useful.
    bus_->RegisterClientAsSender(foreman_client_id_, kPoisonMessage);

    bus_->RegisterClientAsReceiver(foreman_client_id_,
                                   kWorkOrderCompleteMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_,
                                   kRebuildWorkOrderCompleteMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kCatalogRelationNewBlockMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kDataPipelineMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_,
                                   kWorkOrdersAvailableMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_,
                                   kWorkOrderFeedbackMessage);

    agent_client_id_ = bus_->Connect();
    bus_->RegisterClientAsSender(agent_client_id_, kCatalogRelationNewBlockMessage);
    bus_->RegisterClientAsSender(agent_client_id_, kDataPipelineMessage);
    bus_->RegisterClientAsSender(agent_client_id_, kWorkOrderFeedbackMessage);
    bus_->RegisterClientAsSender(agent_client_id_, kWorkOrdersAvailableMessage);
  }

  ~Foreman() override {}

  /**
   * @brief Set the Query plan DAG for the query to be executed.
   *
   * @param query_plan_dag A pointer to the query plan DAG.
   **/
  inline void setQueryPlan(DAG<RelationalOperator, bool> *query_plan_dag) {
    query_dag_ = query_plan_dag;
  }

  /**
   * @brief Reconstruct the QueryContext for the query to be executed.
   *
   * @param proto The serialized QueryContext.
   **/
  inline void reconstructQueryContextFromProto(const serialization::QueryContext &proto) {
    query_context_.reset(
        new QueryContext(proto, *catalog_database_, storage_manager_, foreman_client_id_, agent_client_id_, bus_));
  }

  /**
   * @brief Set the WorkerDirectory pointer.
   *
   * @param workers A pointer to the WorkerDirectory.
   **/
  void setWorkerDirectory(WorkerDirectory *workers) {
    workers_ = workers;
  }

  /**
   * @brief Set the maximum number of messages that should be allocated to each
   *        worker during a single round of WorkOrder dispatch.
   *
   * @param max_msgs_per_worker Maximum number of messages.
   **/
  void setMaxMessagesPerWorker(const std::size_t max_msgs_per_worker) {
    max_msgs_per_worker_ = max_msgs_per_worker;
  }

 protected:
  /**
   * @brief The foreman receives a DAG of relational operators, asks relational
   *        operators to produce the workorders and based on the response it gets
   *        pipelines the intermediate output to dependent relational operators.
   *
   * @note  The workers who get the messages from the Foreman execute and
   *        subsequently delete the WorkOrder contained in the message.
   **/
  void run() override;

 private:
  typedef DAG<RelationalOperator, bool>::size_type_nodes dag_node_index;

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
    for (const dag_node_index dependency_index : query_dag_->getDependencies(node_index)) {
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
  inline bool checkAllBlockingDependenciesMet(const dag_node_index node_index) const {
    for (const dag_node_index blocking_dependency_index : blocking_dependencies_[node_index]) {
      if (!query_exec_state_->hasExecutionFinished(blocking_dependency_index)) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Dispatch schedulable WorkOrders, wrapped in WorkerMessages to the
   *        worker threads.
   *
   * @param start_worker_index The dispatch of WorkOrders preferably begins with
   *        the worker at this index.
   * @param start_operator_index The search for a schedulable WorkOrder
   *        begins with the WorkOrders generated by this operator.
   **/
  void dispatchWorkerMessages(const std::size_t start_worker_index,
                              const dag_node_index start_operator_index);

  /**
   * @brief Initialize all the local vectors and maps. If the operator has an
   *        InsertDestination, pass the bus address and Foreman's TMB client ID
   *        to it.
   **/
  void initializeState();

  /**
   * @brief Initialize the Foreman before starting the event loop. This binds
   * the Foreman thread to configured CPU, and does initial processing of
   * operator before waiting for events from Workers.
   **/
  void initialize();

  /**
   * @brief Process the received WorkOrder complete message.
   *
   * @param node_index The index of the specified operator node in the query DAG
   *        for the completed WorkOrder.
   * @param worker_id The logical ID of the worker for the completed WorkOrder.
   **/
  void processWorkOrderCompleteMessage(const dag_node_index op_index,
                                       const std::size_t worker_id);

  /**
   * @brief Process the received RebuildWorkOrder complete message.
   *
   * @param node_index The index of the specified operator node in the query DAG
   *        for the completed RebuildWorkOrder.
   * @param worker_id The logical ID of the worker for the completed
   *        RebuildWorkOrder.
   **/
  void processRebuildWorkOrderCompleteMessage(const dag_node_index op_index,
                                              const std::size_t worker_id);

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
   * @brief Process the received work order feedback message and notify relational
   *        operator.
   *
   * @param message Feedback message from work order.
   **/
  void processFeedbackMessage(const WorkOrder::FeedbackMessage &message);

  /**
   * @brief Clear some of the vectors used for a single run of a query.
   **/
  void cleanUp() {
    output_consumers_.clear();
    blocking_dependencies_.clear();
  }

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
  void processOperator(const dag_node_index index, const bool recursively_check_dependents);

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
   * @brief Send the given message to the specified worker.
   *
   * @param worker_id The logical ID of the recipient worker.
   * @param message The WorkerMessage to be sent.
   **/
  void sendWorkerMessage(const std::size_t worker_id, const WorkerMessage &message);

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
  void getRebuildWorkOrders(const dag_node_index index, WorkOrdersContainer *container);

  CatalogDatabaseLite *catalog_database_;
  StorageManager *storage_manager_;

  // The sender agent to send messages to Foreman on behalf of
  // InsertDestinations and some WorkOrders.
  tmb::client_id agent_client_id_;

  DAG<RelationalOperator, bool> *query_dag_;

  std::unique_ptr<QueryContext> query_context_;

  // During a single round of WorkOrder dispatch, a Worker should be allocated
  // at most these many WorkOrders.
  std::size_t max_msgs_per_worker_;

  // For all nodes, store their receiving dependents.
  std::vector<std::vector<dag_node_index>> output_consumers_;

  // For all nodes, store their pipeline breaking dependencies (if any).
  std::vector<std::vector<dag_node_index>> blocking_dependencies_;

  std::unique_ptr<QueryExecutionState> query_exec_state_;

  std::unique_ptr<WorkOrdersContainer> workorders_container_;

  const int num_numa_nodes_;

  WorkerDirectory *workers_;

  friend class ForemanTest;
  FRIEND_TEST(ForemanTest, TwoNodesDAGPartiallyFilledBlocksTest);

  DISALLOW_COPY_AND_ASSIGN(Foreman);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_FOREMAN_HPP_
