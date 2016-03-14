/**
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

#ifndef QUICKSTEP_QUERY_EXECUTION_FOREMAN_DISTRIBUTED_HPP_
#define QUICKSTEP_QUERY_EXECUTION_FOREMAN_DISTRIBUTED_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/ForemanLite.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/DAG.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"

namespace quickstep {

class CatalogDatabase;

namespace serialization { class WorkOrderMessage; }

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief The Foreman scans the query DAG, requests each operator to produce
 *        workorders. It also pipelines the intermediate output it receives to
 *        the relational operators which need it.
 **/
class ForemanDistributed final : public ForemanLite {
 public:
  /**
   * @brief Constructor.
   *
   * @param bus A pointer to the TMB.
   * @param database The Database to resolve relation references in.
   * @param cpu_id The ID of the CPU to which the Foreman thread can be pinned.
   * @param num_numa_nodes The number of NUMA nodes in the system.
   *
   * @note If cpu_id is not specified, Foreman thread can be possibly moved
   *       around on different CPUs by the OS.
  **/
  ForemanDistributed(tmb::MessageBus *bus,
                     CatalogDatabase *database,
                     const int cpu_id = -1)
      : ForemanLite(bus, cpu_id),
        database_(DCHECK_NOTNULL(database)),
        num_operators_finished_(0) {
    DCHECK(bus != nullptr);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kShiftbossRegistrationMessage);
    bus_->RegisterClientAsSender(foreman_client_id_, kShiftbossRegistrationResponseMessage);

    bus_->RegisterClientAsSender(foreman_client_id_, kShiftbossInitiateMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kShiftbossInitiateResponseMessage);

    bus_->RegisterClientAsSender(foreman_client_id_, kWorkOrderMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kWorkOrderCompleteMessage);

    bus_->RegisterClientAsSender(foreman_client_id_, kInitiateRebuildMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kInitiateRebuildResponseMessage);

    bus_->RegisterClientAsReceiver(foreman_client_id_, kRebuildWorkOrderCompleteMessage);

    bus_->RegisterClientAsSender(foreman_client_id_, kQueryResultRelationMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kQueryResultRelationResponseMessage);

    // NOTE(zuyu): Foreman thread sends poison messages to stop Shiftbosses.
    bus_->RegisterClientAsSender(foreman_client_id_, kPoisonMessage);

    bus_->RegisterClientAsReceiver(foreman_client_id_, kCatalogRelationNewBlockMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kDataPipelineMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kWorkOrdersAvailableMessage);
    bus_->RegisterClientAsReceiver(foreman_client_id_, kWorkOrderFeedbackMessage);
  }

  ~ForemanDistributed() override {
  }

  /**
   * @brief Set QueryHandle for the query to be executed.
   *
   * @param query_handle A pointer to QueryHandle.
   **/
  inline void setQueryHandle(QueryHandle *query_handle) {
    query_handle_ = DCHECK_NOTNULL(query_handle);

    query_dag_ = query_handle->getQueryPlanMutable()->getQueryPlanDAGMutable();
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
   * @brief Check if the current query has finished its execution.
   *
   * @return True if the query has finished. Otherwise false.
   **/
  inline bool checkQueryExecutionFinished() const {
    return num_operators_finished_ == query_dag_->size();
  }

  /**
   * @brief Check if all the dependencies of the node at specified index have
   *        finished their execution.
   *
   * @param node_index The index of the specified node in the query DAG.
   *
   * @return True if all the dependencies have finished their execution. False
   *         otherwise.
   **/
  inline bool checkAllDependenciesMet(const dag_node_index node_index) const {
    for (dag_node_index dependency_index : query_dag_->getDependencies(node_index)) {
      // If at least one of the dependencies is not met, return false.
      if (!execution_finished_[dependency_index]) {
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
    for (dag_node_index blocking_dependency_index : blocking_dependencies_[node_index]) {
      if (!execution_finished_[blocking_dependency_index]) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Dispatch schedulable WorkOrders to the Shiftboss thread.
   *
   * @param start_shiftboss_index The dispatch of WorkOrders preferably begins with
   *        the Shiftboss at this index.
   * @param start_operator_index The search for a schedulable WorkOrder
   *        begins with the WorkOrders generated by this operator.
   **/
  void dispatchToShiftboss(const std::size_t start_shiftboss_index,
                           const dag_node_index start_operator_index);

  /**
   * @brief Initialize all the local vectors and maps. If the operator has an
   *        InsertDestination, pass the bus address and Foreman's TMB client ID
   *        to it.
   **/
  void initializeState();

  /**
   * @brief Initiate Shiftboss to prepare for the query execution, including
   *        constructing QueryContext and CatalogCache.
   **/
  void sendShiftbossInitiateMessage();

  /**
   * @brief Initialize the Foreman before starting the event loop. This binds
   * the Foreman thread to configured CPU, and does initial processing of
   * operator before waiting for events from Workers.
   **/
  void initialize();

  /**
   * @brief Process the Shiftboss registeration message and ack back.
   *
   * @param shiftboss_client_id The TMB client id of Shiftboss.
   * @param work_order_capacity The work order processing capacity of Shiftboss.
   **/
  void processShiftbossRegisterationMessage(const tmb::client_id shiftboss_client_id,
                                            const std::size_t work_order_capacity);

  /**
   * @brief Process the work order complete message.
   *
   * @param shiftboss_index The Shiftboss index for the completed work order.
   * @param op_index The index of the specified operator node in the query DAG
   *        for the completed work order.
   **/
  void processWorkOrderCompleteMessage(const std::size_t shiftboss_index,
                                       const dag_node_index op_index);

  /**
   * @brief Process the initiate rebuild work order ack message.
   *
   * @param shiftboss_index The Shiftboss index for the rebuild work orders.
   * @param op_index The index of the specified operator node in the query DAG
   *        for initiating the rebuild work order.
   * @param num_rebuild_work_orders The number of the rebuild work orders
   *        generated for the operator indexed by 'op_index'.
   **/
  void processInitiateRebuildResponseMessage(const std::size_t shiftboss_index,
                                             const dag_node_index op_index,
                                             const std::size_t num_rebuild_work_orders);
  /**
   * @brief Process the rebuild work order complete message.
   *
   * @param shiftboss_index The Shiftboss index for the completed rebuild work
   *        order.
   * @param op_index The index of the specified operator node in the query DAG
   *        for the completed rebuild work order.
   **/
  void processRebuildWorkOrderCompleteMessage(const std::size_t shiftboss_index,
                                              const dag_node_index op_index);

  /**
   * @brief Process the data pipeline message.
   *
   * @param shiftboss_index The Shiftboss index for the datapipeline message.
   * @param op_index The index of the specified operator node in the query DAG
   *        for the completed rebuild work order.
   * @param block The block id.
   * @param rel_id The ID of the relation that produced 'block'.
   **/
  void processDataPipelineMessage(const std::size_t shiftboss_index,
                                  const dag_node_index op_index,
                                  const block_id block,
                                  const relation_id rel_id);

  /**
   * @brief Process work order feedback message and notify relational operator.
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
    done_gen_.clear();
    execution_finished_.clear();
    rebuild_required_.clear();
    rebuild_status_.clear();
  }

  /**
   * @brief Process a current relational operator: Get its workorders and store
   *        them in the WorkOrdersContainer for this query. If the operator can
   *        be marked as done, do so.
   *
   * @param shiftboss_index The Shiftboss index for the normal work orders.
   * @param index The index of op in the query plan DAG.
   * @param recursively_check_dependents If an operator is done, should we
   *        call processOperator on its dependents recursively.
   **/
  void processOperator(const std::size_t shiftboss_index,
                       const dag_node_index index,
                       const bool recursively_check_dependents);

 /**
   * @brief Get the next normal work order to be excuted, wrapped in
   *        WorkOrderMessage proto.
   *
   * @param start_operator_index Begin the search for the schedulable WorkOrder
   *        with the operator at this index.
   *
   * @return A pointer to WorkOrderMessage proto. If there is no avaiable
   *         WorkOrder, return NULL.
   **/
  serialization::WorkOrderMessage* getWorkOrderMessage(const dag_node_index start_operator_index);

  /**
   * @brief Fetch all work orders currently available in relational operator and
   *        store them internally.
   *
   * @param index The index of op in the query plan DAG.
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
    return checkNormalExecutionOver(index)
      && (!checkRebuildRequired(index) || checkRebuildOver(index));
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
    return checkAllDependenciesMet(index)
        && !normal_workorder_protos_container_->hasWorkOrderProto(index)
        && queued_workorders_per_op_[index] == 0
        && done_gen_[index];
  }

  /**
   * @brief Check if the rebuild operation is required for a given operator.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the rebuild operation is required, false otherwise.
   **/
  inline bool checkRebuildRequired(const dag_node_index index) const {
    return rebuild_required_[index];
  }

  /**
   * @brief Check if the rebuild operation for a given operator is over.
   *
   * @param index The index of the given operator in the DAG.
   *
   * @return True if the rebuild operation is over, false otherwise.
   **/
  inline bool checkRebuildOver(const dag_node_index index) const {
    std::unordered_map<dag_node_index,
                       std::pair<bool, std::int64_t>>::const_iterator
        search_res = rebuild_status_.find(index);
    DCHECK(search_res != rebuild_status_.end());
    return checkRebuildInitiated(index)
        && search_res->second.second == 0;
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
    return rebuild_status_.at(index).first;
  }

  /**
   * @brief Initiate the rebuild process for partially filled blocks generated
   *        during the execution of the given operator.
   *
   * @param shiftboss_index The Shiftboss index for the rebuild work orders.
   * @param index The index of the given operator in the DAG.
   **/
  void initiateRebuild(const std::size_t shiftboss_index,
                       const dag_node_index index);

  ShiftbossDirectory shiftbosses_;

  CatalogDatabase *database_;

  QueryHandle *query_handle_;
  DAG<RelationalOperator, bool> *query_dag_;  // Owned by 'query_handle_'.

  // Number of operators who've finished their execution.
  std::size_t num_operators_finished_;

  // The ith bit denotes if the operator with ID = i has finished its execution.
  std::vector<bool> execution_finished_;

  // For all nodes, store their receiving dependents.
  std::vector<std::vector<dag_node_index>> output_consumers_;

  // For all nodes, store their pipeline breaking dependencies (if any).
  std::vector<std::vector<dag_node_index>> blocking_dependencies_;

  // The ith bit denotes if the operator with ID = i has finished generating
  // work orders.
  std::vector<bool> done_gen_;

  // The ith bit denotes if the operator with ID = i requires generation of
  // rebuild WorkOrders.
  std::vector<bool> rebuild_required_;

  // Key is dag_node_index of the operator for which rebuild is required. Value is
  // a pair - first element has a bool (whether rebuild for operator at index i
  // has been initiated) and if the boolean is true, the second element denotes
  // the number of pending rebuild workorders for the operator.
  std::unordered_map<dag_node_index, std::pair<bool, std::int64_t>> rebuild_status_;

  // A vector to track the number of workorders in execution, for each operator.
  std::vector<int> queued_workorders_per_op_;

  std::unique_ptr<WorkOrderProtosContainer> normal_workorder_protos_container_;

  DISALLOW_COPY_AND_ASSIGN(ForemanDistributed);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_FOREMAN_DISTRIBUTED_HPP_
