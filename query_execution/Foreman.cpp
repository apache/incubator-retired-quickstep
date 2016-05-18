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

#include "query_execution/Foreman.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionScheme.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "relational_operators/RebuildWorkOrder.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::move;
using std::pair;
using std::size_t;
using std::vector;

namespace quickstep {

void Foreman::initialize() {
  if (cpu_id_ >= 0) {
    // We can pin the foreman thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }
  initializeState();

  DEBUG_ASSERT(query_dag_ != nullptr);
  const dag_node_index dag_size = query_dag_->size();

  // Collect all the workorders from all the relational operators in the DAG.
  for (dag_node_index index = 0; index < dag_size; ++index) {
    if (checkAllBlockingDependenciesMet(index)) {
      query_dag_->getNodePayloadMutable(index)->informAllBlockingDependenciesMet();
      processOperator(index, false);
    }
  }

  // Dispatch the WorkOrders generated so far.
  dispatchWorkerMessages(0, 0);
}

void Foreman::processWorkOrderCompleteMessage(const dag_node_index op_index,
                                              const size_t worker_thread_index) {
  query_exec_state_->decrementNumQueuedWorkOrders(op_index);

  // As the given worker finished executing a WorkOrder, decrement its number
  // of queued WorkOrders.
  workers_->decrementNumQueuedWorkOrders(worker_thread_index);

  // Check if new work orders are available and fetch them if so.
  fetchNormalWorkOrders(op_index);

  if (checkRebuildRequired(op_index)) {
    if (checkNormalExecutionOver(op_index)) {
      if (!checkRebuildInitiated(op_index)) {
        if (initiateRebuild(op_index)) {
          // Rebuild initiated and completed right away.
          markOperatorFinished(op_index);
        } else {
          // Rebuild under progress.
        }
      } else if (checkRebuildOver(op_index)) {
        // Rebuild was under progress and now it is over.
        markOperatorFinished(op_index);
      }
    } else {
      // Normal execution under progress for this operator.
    }
  } else if (checkOperatorExecutionOver(op_index)) {
    // Rebuild not required for this operator and its normal execution is
    // complete.
    markOperatorFinished(op_index);
  }

  for (const pair<dag_node_index, bool> &dependent_link :
       query_dag_->getDependents(op_index)) {
    const dag_node_index dependent_op_index = dependent_link.first;
    if (checkAllBlockingDependenciesMet(dependent_op_index)) {
      // Process the dependent operator (of the operator whose WorkOrder
      // was just executed) for which all the dependencies have been met.
      processOperator(dependent_op_index, true);
    }
  }

  // Dispatch the WorkerMessages to the workers. We prefer to start the search
  // for the schedulable WorkOrders beginning from 'op_index'. The first
  // candidate worker to receive the next WorkOrder is the one that sent the
  // response message to Foreman.
  dispatchWorkerMessages(worker_thread_index, op_index);
}

void Foreman::processRebuildWorkOrderCompleteMessage(const dag_node_index op_index,
                                                     const size_t worker_thread_index) {
  query_exec_state_->decrementNumRebuildWorkOrders(op_index);
  workers_->decrementNumQueuedWorkOrders(worker_thread_index);

  if (checkRebuildOver(op_index)) {
    markOperatorFinished(op_index);

    for (const pair<dag_node_index, bool> &dependent_link :
         query_dag_->getDependents(op_index)) {
      const dag_node_index dependent_op_index = dependent_link.first;
      if (checkAllBlockingDependenciesMet(dependent_op_index)) {
        processOperator(dependent_op_index, true);
      }
    }
  }

  // Dispatch the WorkerMessages to the workers. We prefer to start the search
  // for the schedulable WorkOrders beginning from 'op_index'. The first
  // candidate worker to receive the next WorkOrder is the one that sent the
  // response message to Foreman.
  dispatchWorkerMessages(worker_thread_index, op_index);
}

void Foreman::processDataPipelineMessage(const dag_node_index op_index,
                                         const block_id block,
                                         const relation_id rel_id) {
  for (const dag_node_index consumer_index :
       output_consumers_[op_index]) {
    // Feed the streamed block to the consumer. Note that 'output_consumers_'
    // only contain those dependents of operator with index = op_index which are
    // eligible to receive streamed input.
    query_dag_->getNodePayloadMutable(consumer_index)->feedInputBlock(block, rel_id);
    // Because of the streamed input just fed, check if there are any new
    // WorkOrders available and if so, fetch them.
    fetchNormalWorkOrders(consumer_index);
  }

  // Dispatch the WorkerMessages to the workers. We prefer to start the search
  // for the schedulable WorkOrders beginning from 'op_index'. The first
  // candidate worker to receive the next WorkOrder is the one that sent the
  // response message to Foreman.
  // TODO(zuyu): Improve the data locality for the next WorkOrder.
  dispatchWorkerMessages(0, op_index);
}

void Foreman::processFeedbackMessage(const WorkOrder::FeedbackMessage &msg) {
  RelationalOperator *op =
      query_dag_->getNodePayloadMutable(msg.header().rel_op_index);
  op->receiveFeedbackMessage(msg);
}

void Foreman::run() {
  // Initialize before for Foreman eventloop.
  initialize();

  // Event loop
  while (!query_exec_state_->hasQueryExecutionFinished()) {
    // Receive() causes this thread to sleep until next message is received.
    AnnotatedMessage annotated_msg = bus_->Receive(foreman_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_msg.tagged_message;
    switch (tagged_message.message_type()) {
      case kWorkOrderCompleteMessage: {
        serialization::WorkOrderCompletionMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processWorkOrderCompleteMessage(proto.operator_index(), proto.worker_thread_index());
        break;
      }
      case kRebuildWorkOrderCompleteMessage: {
        serialization::WorkOrderCompletionMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processRebuildWorkOrderCompleteMessage(proto.operator_index(), proto.worker_thread_index());
        break;
      }
      case kCatalogRelationNewBlockMessage: {
        serialization::CatalogRelationNewBlockMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const block_id block = proto.block_id();

        CatalogRelation *relation =
            static_cast<CatalogDatabase*>(catalog_database_)->getRelationByIdMutable(proto.relation_id());
        relation->addBlock(block);

        if (proto.has_partition_id()) {
          relation->getPartitionSchemeMutable()->addBlockToPartition(proto.partition_id(), block);
        }
        break;
      }
      case kDataPipelineMessage: {
        // Possible message senders include InsertDestinations and some
        // operators which modify existing blocks.
        serialization::DataPipelineMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processDataPipelineMessage(proto.operator_index(), proto.block_id(), proto.relation_id());
        break;
      }
      case kWorkOrdersAvailableMessage: {
        serialization::WorkOrdersAvailableMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const dag_node_index op_index = proto.operator_index();

        // Check if new work orders are available.
        fetchNormalWorkOrders(op_index);

        // Dispatch the WorkerMessages to the workers. We prefer to start the search
        // for the schedulable WorkOrders beginning from 'op_index'. The first
        // candidate worker to receive the next WorkOrder is the one that sent the
        // response message to Foreman.
        // TODO(zuyu): Improve the data locality for the next WorkOrder.
        dispatchWorkerMessages(0, op_index);
        break;
      }
      case kWorkOrderFeedbackMessage: {
        WorkOrder::FeedbackMessage msg(const_cast<void *>(tagged_message.message()),
                                       tagged_message.message_bytes());
        processFeedbackMessage(msg);
        break;
      }
      default:
        LOG(FATAL) << "Unknown message type to Foreman";
    }
  }

  // Clean up before exiting.
  cleanUp();
}

void Foreman::dispatchWorkerMessages(
    const size_t start_worker_index,
    const dag_node_index start_operator_index) {
  // Loop over all workers. Stopping criteria:
  // 1. Every worker has been assigned exactly max_msgs_per_worker_ workorders.
  // OR 2. No schedulable workorders at this time.
  size_t done_workers_count = 0;
  for (size_t curr_worker = start_worker_index;
       done_workers_count < workers_->getNumWorkers();
       curr_worker = (curr_worker + 1) % workers_->getNumWorkers()) {
    if (workers_->getNumQueuedWorkOrders(curr_worker) < max_msgs_per_worker_) {
      std::unique_ptr<WorkerMessage> msg;
      msg.reset(getNextWorkerMessage(
          start_operator_index, workers_->getNUMANode(curr_worker)));
      if (msg.get() != nullptr) {
        sendWorkerMessage(curr_worker, *msg);
        workers_->incrementNumQueuedWorkOrders(curr_worker);
      } else {
        // No schedulable workorder at this point.
        ++done_workers_count;
      }
    } else {
      // curr_worker already has been assigned max_msgs_per_worker workorders.
      ++done_workers_count;
    }
  }
}

void Foreman::initializeState() {
  const dag_node_index dag_size = query_dag_->size();

  output_consumers_.resize(dag_size);
  blocking_dependencies_.resize(dag_size);

  query_exec_state_.reset(new QueryExecutionState(dag_size));
  workorders_container_.reset(new WorkOrdersContainer(dag_size, num_numa_nodes_));

  for (dag_node_index node_index = 0; node_index < dag_size; ++node_index) {
    const QueryContext::insert_destination_id insert_destination_index =
        query_dag_->getNodePayload(node_index).getInsertDestinationID();
    if (insert_destination_index != QueryContext::kInvalidInsertDestinationId) {
      // Rebuild is necessary whenever InsertDestination is present.
      query_exec_state_->setRebuildRequired(node_index);
      query_exec_state_->setRebuildStatus(node_index, 0, false);
    }

    for (const pair<dag_node_index, bool> &dependent_link :
         query_dag_->getDependents(node_index)) {
      const dag_node_index dependent_op_index = dependent_link.first;
      if (!query_dag_->getLinkMetadata(node_index, dependent_op_index)) {
        // The link is not a pipeline-breaker. Streaming of blocks is possible
        // between these two operators.
        output_consumers_[node_index].push_back(dependent_op_index);
      } else {
        // The link is a pipeline-breaker. Streaming of blocks is not possible
        // between these two operators.
        blocking_dependencies_[dependent_op_index].push_back(node_index);
      }
    }
  }
  // Loop again to populate the active_operators_ vector.
  for (dag_node_index node_index = 0; node_index < dag_size; ++node_index) {
    if (checkAllBlockingDependenciesMet(node_index)) {
      // Add to active_operators_.
      active_operators_.emplace_back(node_index);
    }
  }
}

// TODO(harshad) : The default policy may execute remote WorkOrders for an
// operator with a lower index even when there are local WorkOrders available for
// an operator with higher index. We should examine if avoiding this behavior
// has any benefits with respect to execution time and/or memory pressure.
WorkerMessage* Foreman::getNextWorkerMessage(
    const dag_node_index start_operator_index, const int numa_node) {
  // Default policy: Operator with lowest index first.
  updateProbabilities();
  // We try few times.
  WorkOrder *work_order = nullptr;
  //while (work_order == nullptr) {
    // std::cout << "Look for workorder active operator count: " << operator_probabilities_.size() << "\n";
    int next_operator_index = chooseOperator();
    if (next_operator_index == -1) {
      return nullptr;
    }
    if (numa_node != -1) {
      work_order = workorders_container_->getNormalWorkOrderForNUMANode(next_operator_index, numa_node);
      if (work_order != nullptr) {
        // A WorkOrder found on the given NUMA node.
        query_exec_state_->incrementNumQueuedWorkOrders(next_operator_index);
        return WorkerMessage::WorkOrderMessage(work_order, next_operator_index);
      } else {
        // Normal workorder not found on this node. Look for a rebuild workorder
        // on this NUMA node.
        work_order = workorders_container_->getRebuildWorkOrderForNUMANode(next_operator_index, numa_node);
        if (work_order != nullptr) {
          return WorkerMessage::RebuildWorkOrderMessage(work_order, next_operator_index);
        }
      }
    }
    // Either no workorder found on the given NUMA node, or numa_node is -1.
    // Try to get a normal WorkOrder from other NUMA nodes.
    work_order = workorders_container_->getNormalWorkOrder(next_operator_index);
    if (work_order != nullptr) {
      /*if (!hasOperatorStarted(next_operator_index)) {
        operator_start_timestamp_[next_operator_index] = std::chrono::steady_clock::now();
      }*/
      query_exec_state_->incrementNumQueuedWorkOrders(next_operator_index);
      return WorkerMessage::WorkOrderMessage(work_order, next_operator_index);
    } else {
      // Normal WorkOrder not found, look for a RebuildWorkOrder.
      work_order = workorders_container_->getRebuildWorkOrder(next_operator_index);
      if (work_order != nullptr) {
        return WorkerMessage::RebuildWorkOrderMessage(work_order, next_operator_index);
      }
    }
  // }
  /*size_t num_operators_checked = 0;
  for (dag_node_index index = start_operator_index;
       num_operators_checked < query_dag_->size();
       index = (index + 1) % query_dag_->size(), ++num_operators_checked) {
    if (query_exec_state_->hasExecutionFinished(index)) {
      continue;
    }
    if (numa_node != -1) {
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
    // Either no workorder found on the given NUMA node, or numa_node is -1.
    // Try to get a normal WorkOrder from other NUMA nodes.
    work_order = workorders_container_->getNormalWorkOrder(index);
    if (work_order != nullptr) {
      if (!hasOperatorStarted(index)) {
        operator_start_timestamp_[index] = std::chrono::steady_clock::now();
      }
      query_exec_state_->incrementNumQueuedWorkOrders(index);
      return WorkerMessage::WorkOrderMessage(work_order, index);
    } else {
      // Normal WorkOrder not found, look for a RebuildWorkOrder.
      work_order = workorders_container_->getRebuildWorkOrder(index);
      if (work_order != nullptr) {
        return WorkerMessage::RebuildWorkOrderMessage(work_order, index);
      }
    }
  }*/
  // No WorkOrders available right now.
  return nullptr;
}

void Foreman::sendWorkerMessage(const std::size_t worker_thread_index,
                                const WorkerMessage &message) {
  message_type_id type;
  if (message.getType() == WorkerMessage::WorkerMessageType::kRebuildWorkOrder) {
    type = kRebuildWorkOrderMessage;
  } else if (message.getType() == WorkerMessage::WorkerMessageType::kWorkOrder) {
    type = kWorkOrderMessage;
  } else {
    FATAL_ERROR("Invalid WorkerMessageType");
  }
  TaggedMessage worker_tagged_message(&message, sizeof(message), type);

  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         workers_->getClientID(worker_thread_index),
                                         move(worker_tagged_message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK) <<
      "Message could not be sent from Foreman with TMB client ID "
      << foreman_client_id_ << " to Foreman with TMB client ID "
      << workers_->getClientID(worker_thread_index);
}

bool Foreman::fetchNormalWorkOrders(const dag_node_index index) {
  bool generated_new_workorders = false;
  if (!query_exec_state_->hasDoneGenerationWorkOrders(index)) {
    // Do not fetch any work units until all blocking dependencies are met.
    // The releational operator is not aware of blocking dependencies for
    // uncorrelated scalar queries.
    if (!checkAllBlockingDependenciesMet(index)) {
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
  if (generated_new_workorders) {
    if (std::find(active_operators_.begin(), active_operators_.end(), index) == active_operators_.end()) {
      // std::cout << "Added operator " << index << " in processOperator()\n";
      active_operators_.emplace_back(index);
      updateProbabilities();
    }
  }
  return generated_new_workorders;
}

void Foreman::processOperator(const dag_node_index index,
                              const bool recursively_check_dependents) {
  if (fetchNormalWorkOrders(index)) {
    // Fetched work orders. Return to wait for the generated work orders to
    // execute, and skip the execution-finished checks.
    return;
  }

  if (checkNormalExecutionOver(index)) {
    if (checkRebuildRequired(index)) {
      if (!checkRebuildInitiated(index)) {
        // Rebuild hasn't started, initiate it.
        if (initiateRebuild(index)) {
          // Rebuild initiated and completed right away.
          markOperatorFinished(index);
        } else {
          // Rebuild WorkOrders have been generated.
          return;
        }
      } else if (checkRebuildOver(index)) {
        // Rebuild had been initiated and it is over.
        markOperatorFinished(index);
      }
    } else {
      // Rebuild is not required and normal execution over, mark finished.
      markOperatorFinished(index);
    }
    // If we reach here, that means the operator has been marked as finished.
    if (recursively_check_dependents) {
      for (const pair<dag_node_index, bool> &dependent_link :
           query_dag_->getDependents(index)) {
        const dag_node_index dependent_op_index = dependent_link.first;
        if (checkAllBlockingDependenciesMet(dependent_op_index)) {
          processOperator(dependent_op_index, true);
        }
      }
    }
  }
}

void Foreman::markOperatorFinished(const dag_node_index index) {
  // std::cout << "Operator " << index << "finished\n";
  // operator_duration_[index] = std::chrono::steady_clock::now() - operator_start_timestamp_[index];
  // Remove this operator.
  active_operators_.erase(std::remove(active_operators_.begin(), active_operators_.end(), index), active_operators_.end());
  // Add to finished operators.
  // finished_operators_.emplace_back(index);
  updateProbabilities();

  query_exec_state_->setExecutionFinished(index);

  RelationalOperator *op = query_dag_->getNodePayloadMutable(index);
  op->updateCatalogOnCompletion();

  const relation_id output_rel = op->getOutputRelationID();
  for (const pair<dag_node_index, bool> &dependent_link : query_dag_->getDependents(index)) {
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
}

bool Foreman::initiateRebuild(const dag_node_index index) {
  DEBUG_ASSERT(!workorders_container_->hasRebuildWorkOrder(index));
  DEBUG_ASSERT(checkRebuildRequired(index));
  DEBUG_ASSERT(!checkRebuildInitiated(index));

  getRebuildWorkOrders(index, workorders_container_.get());

  query_exec_state_->setRebuildStatus(
      index, workorders_container_->getNumRebuildWorkOrders(index), true);

  if (query_exec_state_->getNumRebuildWorkOrders(index) > 0) {
    if (std::find(active_operators_.begin(), active_operators_.end(), index) == active_operators_.end()) {
      active_operators_.emplace_back(index);
    }
  }
  return (query_exec_state_->getNumRebuildWorkOrders(index) == 0);
}

void Foreman::getRebuildWorkOrders(const dag_node_index index, WorkOrdersContainer *container) {
  const RelationalOperator &op = query_dag_->getNodePayload(index);
  const QueryContext::insert_destination_id insert_destination_index = op.getInsertDestinationID();

  if (insert_destination_index == QueryContext::kInvalidInsertDestinationId) {
    return;
  }

  vector<MutableBlockReference> partially_filled_block_refs;

  DCHECK(query_context_ != nullptr);
  InsertDestination *insert_destination = query_context_->getInsertDestination(insert_destination_index);
  DCHECK(insert_destination != nullptr);

  insert_destination->getPartiallyFilledBlocks(&partially_filled_block_refs);

  for (vector<MutableBlockReference>::size_type i = 0;
       i < partially_filled_block_refs.size();
       ++i) {
    container->addRebuildWorkOrder(
        new RebuildWorkOrder(move(partially_filled_block_refs[i]),
                            index,
                            op.getOutputRelationID(),
                            foreman_client_id_,
                            bus_),
        index);
  }
}

void Foreman::updateProbabilities() {
  // std::unordered_map<dag_node_index, std::size_t> num_workorders;
  std::vector<std::pair<double, dag_node_index>> operator_probabilities;
  /*std::size_t total_num_workorders = 0;
  for (dag_node_index active_op_index : active_operators_) {
    if (checkAllBlockingDependenciesMet(active_op_index)) {
      std::size_t num_workorders_for_op = workorders_container_->getNumNormalWorkOrders(active_op_index);
      num_workorders_for_op += workorders_container_->getNumRebuildWorkOrders(active_op_index);
      if (num_workorders_for_op > 0) {
        num_workorders[active_op_index] = num_workorders_for_op;
        total_num_workorders += num_workorders_for_op;
      }
    }
  }*/
  std::unordered_map<dag_node_index, bool> schedulable_operators;
  for (dag_node_index active_op_index : active_operators_) {
    if (checkAllBlockingDependenciesMet(active_op_index)) {
      schedulable_operators[active_op_index] = true;
    }
  }
  /*if (total_num_workorders == 0) {
    operator_probabilities_.swap(operator_probabilities);
    return;
  }*/
  if (schedulable_operators.empty()) {
    operator_probabilities_.swap(operator_probabilities);
    return;
  }
  std::size_t last_operator_index = 0;
  // if (num_workorders.size() == 1u) {
  if (schedulable_operators.size() == 1u) {
    // Only one operator is active.
    // last_operator_index = num_workorders.begin()->first;
    last_operator_index = schedulable_operators.begin()->first;
    operator_probabilities.emplace_back(1.0, last_operator_index);
  } else {
    // More than one active operators.
    double cumulative_probability = 0.0;
    const double individual_probability = 1 / static_cast<double>(schedulable_operators.size());
    /*for (auto it = num_workorders.begin(); it != num_workorders.end(); ++it) {
      const double individual_probability = it->second / static_cast<double>(total_num_workorders);
      cumulative_probability += individual_probability;
      operator_probabilities.emplace_back(cumulative_probability, it->first);
      last_operator_index = it->first;
    }*/
    for (auto it = schedulable_operators.begin(); it != schedulable_operators.end(); ++it) {
      cumulative_probability += individual_probability;
      operator_probabilities.emplace_back(cumulative_probability, it->first);
      last_operator_index = it->first;
    }
    DCHECK(!operator_probabilities.empty());
    operator_probabilities.back().first = 1.0;
    operator_probabilities.back().second = last_operator_index;
  }
  // Round off the cumulative probability for the last element.
  operator_probabilities_.swap(operator_probabilities);
}

int Foreman::chooseOperator() {
  if (operator_probabilities_.empty()) {
    // std::cout << "No operator right now\n";
    return -1;
  } else if (operator_probabilities_.size() == 1u) {
    int operator_index = static_cast<int>(operator_probabilities_.front().second);
    if (workorders_container_->hasNormalWorkOrder(operator_index) || workorders_container_->hasRebuildWorkOrder(operator_index)) {
      // std::cout << "Single operator: " << operator_index << "\n";
      return operator_index;
    } else {
      // std::cout << "Single operator: " << operator_index << " but no workorder\n";
      return -1;
    }
  } else {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double chosen_probability = dist(mt_);
    std::pair<double, std::size_t> search_key = std::make_pair(chosen_probability, 0);
    auto chosen_operator_it = std::upper_bound(
          operator_probabilities_.begin(), operator_probabilities_.end(), search_key);
    DCHECK(chosen_operator_it != operator_probabilities_.end());
    // std::cout << "Operator: " << chosen_operator_it->second << "\n";
    return static_cast<int>(chosen_operator_it->second);
  }
}

}  // namespace quickstep
