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

#include "query_execution/QueryManagerSingleNode.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "relational_operators/RebuildWorkOrder.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageManager.hpp"
#include "utility/DAG.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

class WorkOrder;

QueryManagerSingleNode::QueryManagerSingleNode(
    const tmb::client_id foreman_client_id,
    const std::size_t num_numa_nodes,
    QueryHandle *query_handle,
    CatalogDatabaseLite *catalog_database,
    StorageManager *storage_manager,
    tmb::MessageBus *bus)
    : QueryManagerBase(query_handle),
      foreman_client_id_(foreman_client_id),
      storage_manager_(DCHECK_NOTNULL(storage_manager)),
      bus_(DCHECK_NOTNULL(bus)),
      query_context_(new QueryContext(query_handle->getQueryContextProto(),
                                      *catalog_database,
                                      storage_manager_,
                                      foreman_client_id_,
                                      bus_)),
      workorders_container_(
          new WorkOrdersContainer(num_operators_in_dag_, num_numa_nodes)),
      estimated_memory_consumption_in_bytes_(0) {
  updateActiveOperators();
}

WorkerMessage* QueryManagerSingleNode::getNextWorkerMessage(
    const dag_node_index start_operator_index, const numa_node_id numa_node) {
  WorkerMessage *worker_message = getNextWorkerMessageFromActiveOperators(numa_node);
  if (worker_message != nullptr) {
    return worker_message;
  }
  // We didn't find a work order. Try activating new operators.
  updateActiveOperators();
  worker_message = getNextWorkerMessageFromActiveOperators(numa_node);
  if (worker_message == nullptr) {
    std::cout << "No workorder even after activating new operators\n";
  }
  return worker_message;
}

WorkerMessage* QueryManagerSingleNode::getNextWorkerMessageFromActiveOperators(
    const numa_node_id numa_node) {
  WorkOrder *work_order = nullptr;
  for (dag_node_index index : active_operators_) {
    if (checkAllBlockingDependenciesMet(index)) {
      if (numa_node != kAnyNUMANodeID) {
        // First try to get a normal WorkOrder from the specified NUMA node.
        work_order = workorders_container_->getNormalWorkOrderForNUMANode(index, numa_node);
        if (work_order != nullptr) {
          // A WorkOrder found on the given NUMA node.
          query_exec_state_->incrementNumQueuedWorkOrders(index);
          return WorkerMessage::WorkOrderMessage(work_order, index);
        } else {
          // Normal workorder not found on this node. Look for a rebuild workorder
          // on this NUMA node.
          work_order = workorders_container_->getRebuildWorkOrderForNUMANode(index, numa_node);
          if (work_order != nullptr) {
            return WorkerMessage::RebuildWorkOrderMessage(work_order, index);
          }
        }
      }
      // Either no workorder found on the given NUMA node, or numa_node is
      // 'kAnyNUMANodeID'.
      // Try to get a normal WorkOrder from other NUMA nodes.
      work_order = workorders_container_->getNormalWorkOrder(index);
      if (work_order != nullptr) {
        query_exec_state_->incrementNumQueuedWorkOrders(index);
        return WorkerMessage::WorkOrderMessage(work_order, index);
      } else {
        // Normal WorkOrder not found, look for a RebuildWorkOrder.
        work_order = workorders_container_->getRebuildWorkOrder(index);
        if (work_order != nullptr) {
          return WorkerMessage::RebuildWorkOrderMessage(work_order, index);
        }
      }
    }
  }
  return nullptr;
}

bool QueryManagerSingleNode::fetchNormalWorkOrders(const dag_node_index index) {
  bool generated_new_workorders = false;
  if (!query_exec_state_->hasDoneGenerationWorkOrders(index)) {
    // Do not fetch any work units until all blocking dependencies are met.
    // The releational operator is not aware of blocking dependencies for
    // uncorrelated scalar queries.
    if (!checkAllBlockingDependenciesMet(index)) {
      return false;
    }
    if (std::find(active_operators_.begin(), active_operators_.end(), index) == active_operators_.end()) {
      // This operator is not yet active.
      std::cout << "Operator " << index << " not active yet\n";
      return false;
    }
    const size_t num_pending_workorders_before =
        workorders_container_->getNumNormalWorkOrders(index);
    const bool done_generation =
        query_dag_->getNodePayloadMutable(index)->getAllWorkOrders(workorders_container_.get(),
                                                                   query_context_.get(),
                                                                   storage_manager_,
                                                                   foreman_client_id_,
                                                                   bus_);
    if (done_generation) {
      query_exec_state_->setDoneGenerationWorkOrders(index);
    }

    // TODO(shoban): It would be a good check to see if operator is making
    // useful progress, i.e., the operator either generates work orders to
    // execute or still has pending work orders executing. However, this will not
    // work if Foreman polls operators without feeding data. This check can be
    // enabled, if Foreman is refactored to call getAllWorkOrders() only when
    // pending work orders are completed or new input blocks feed.

    generated_new_workorders =
        (num_pending_workorders_before <
         workorders_container_->getNumNormalWorkOrders(index));
  }
  return generated_new_workorders;
}

bool QueryManagerSingleNode::initiateRebuild(const dag_node_index index) {
  DCHECK(!workorders_container_->hasRebuildWorkOrder(index));
  DCHECK(checkRebuildRequired(index));
  DCHECK(!checkRebuildInitiated(index));

  getRebuildWorkOrders(index, workorders_container_.get());

  query_exec_state_->setRebuildStatus(
      index, workorders_container_->getNumRebuildWorkOrders(index), true);

  return (query_exec_state_->getNumRebuildWorkOrders(index) == 0);
}

void QueryManagerSingleNode::getRebuildWorkOrders(const dag_node_index index,
                                                  WorkOrdersContainer *container) {
  const RelationalOperator &op = query_dag_->getNodePayload(index);
  const QueryContext::insert_destination_id insert_destination_index = op.getInsertDestinationID();

  if (insert_destination_index == QueryContext::kInvalidInsertDestinationId) {
    return;
  }

  std::vector<MutableBlockReference> partially_filled_block_refs;

  DCHECK(query_context_ != nullptr);
  InsertDestination *insert_destination = query_context_->getInsertDestination(insert_destination_index);
  DCHECK(insert_destination != nullptr);

  insert_destination->getPartiallyFilledBlocks(&partially_filled_block_refs);

  for (std::vector<MutableBlockReference>::size_type i = 0;
       i < partially_filled_block_refs.size();
       ++i) {
    container->addRebuildWorkOrder(
        new RebuildWorkOrder(query_id_,
                             std::move(partially_filled_block_refs[i]),
                             index,
                             op.getOutputRelationID(),
                             foreman_client_id_,
                             bus_),
        index);
  }
}

void QueryManagerSingleNode::activateOperator(const dag_node_index index) {
  // DCHECK(checkAllBlockingDependenciesMet(index));
  // It is okay to call the line below multiple times.
  query_dag_->getNodePayloadMutable(index)->informAllBlockingDependenciesMet();
  query_context_->activateOperator(
      query_handle()->getQueryContextProto(), index, storage_manager_);
  processOperator(index, false);
}

void QueryManagerSingleNode::updateActiveOperators() {
  // Mark the active operators in the DAG.
  for (dag_node_index index = 0; index < num_operators_in_dag_; ++index) {
    if (!query_exec_state_->hasExecutionFinished(index)) {
      if (!isOperatorActive(index)) {
        // This is an inactive operator.
        const std::size_t estimated_memory_for_operator = query_context_->getHashTableSize(index);
        // NOTE(harshad) - We might get in trouble in the following situation:
        // If there's a build operator in the leaf of the query plan DAG, that
        // has huge memory requirements (greater than the threshold). That
        // operator won't be activated thereby leading the query execution in a
        // deadlock.
        if (canActivateOperator(estimated_memory_for_operator)) {
          estimated_memory_consumption_in_bytes_ += estimated_memory_for_operator;
          active_operators_.push_back(index);
          activateOperator(index);
          if (estimated_memory_for_operator > 0) {
            std::cout << "Activated operator: " << index << " Total active: " << active_operators_.size() << "\n";
          }
        } else {
          std::cout << "operator: " << index << " not activated Total active: " << active_operators_.size() << "\n";
        }
      }
    }
  }
}

bool QueryManagerSingleNode::isOperatorActive(const dag_node_index index) const {
  auto iter = std::find(active_operators_.begin(), active_operators_.end(), index);
  return iter != active_operators_.end();
}

bool QueryManagerSingleNode::canActivateOperator(const std::size_t estimated_memory_for_operator) const {
  const std::size_t total_memory_consumption = estimated_memory_for_operator + estimated_memory_consumption_in_bytes_;
  const std::size_t estimated_num_slots = StorageManager::SlotsNeededForBytes(total_memory_consumption);
  const std::size_t threshold_num_slots = kAdmissionNumSlotsThreshold * storage_manager_->getMaxBufferPoolSlots();
  if (estimated_memory_for_operator > 0) {
    std::cout << "Memory estimate: " << estimated_memory_for_operator;
    std::cout << " estimated slots: " << estimated_num_slots << " threshold: " << threshold_num_slots << "\n";
  }
  return estimated_num_slots < threshold_num_slots;
}

void QueryManagerSingleNode::markOperatorFinished(const dag_node_index index) {
  query_exec_state_->setExecutionFinished(index);

  RelationalOperator *op = query_dag_->getNodePayloadMutable(index);
  op->updateCatalogOnCompletion();

  const relation_id output_rel = op->getOutputRelationID();
  for (const std::pair<dag_node_index, bool> &dependent_link : query_dag_->getDependents(index)) {
    const dag_node_index dependent_op_index = dependent_link.first;
    RelationalOperator *dependent_op = query_dag_->getNodePayloadMutable(dependent_op_index);
    // Signal dependent operator that current operator is done feeding input blocks.
    if (output_rel >= 0) {
      dependent_op->doneFeedingInputBlocks(output_rel);
    }
    if (checkAllBlockingDependenciesMet(dependent_op_index)) {
      dependent_op->informAllBlockingDependenciesMet();
    }
  }
  // Reduce the estimate for this operator.
  const std::size_t estimated_memory_for_operator = query_context_->getHashTableSize(index);
  estimated_memory_consumption_in_bytes_ -= estimated_memory_for_operator;
  std::cout << "Operator " << index << " over, decremented: " << estimated_memory_for_operator << " bytes Total active: " << active_operators_.size();
  std::cout << " Current slots: " << StorageManager::SlotsNeededForBytes(estimated_memory_consumption_in_bytes_) << "\n";
  // Remove this operator from the active operators list.
  auto iter =
      std::find(active_operators_.begin(), active_operators_.end(), index);
  if (iter != active_operators_.end()) {
    active_operators_.erase(iter);
  } else {
    DLOG(WARNING) << "Marking operator " << index << " as finished which wasn't active";
  }
}

}  // namespace quickstep
