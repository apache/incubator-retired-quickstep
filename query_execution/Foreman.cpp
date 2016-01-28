/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "relational_operators/RebuildWorkOrder.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/Macros.hpp"

#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

#include "glog/logging.h"

using std::move;
using std::pair;
using std::vector;

namespace quickstep {

bool Foreman::initialize() {
  if (cpu_id_ >= 0) {
    // We can pin the foreman thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }
  DEBUG_ASSERT(query_dag_ != nullptr);

  const dag_node_index dag_size = query_dag_->size();
  initializeState();

  // Collect all the workorders from all the relational operators in the DAG.
  for (dag_node_index index = 0; index < dag_size; ++index) {
    RelationalOperator *curr_op = query_dag_->getNodePayloadMutable(index);
    if (checkAllBlockingDependenciesMet(index)) {
      curr_op->informAllBlockingDependenciesMet();
      processOperator(curr_op, index, false);
    }
  }

  // Dispatch the WorkOrders generated so far.
  dispatchWorkerMessages(0, 0);
  return num_operators_finished_ == dag_size;
}

// TODO(harshad) - There is duplication in terms of functionality provided by
// TMB and ForemanMessage class with respect to determining the message types.
// Try to use TMB message types for infering the messsage types consistently.
bool Foreman::processMessage(const ForemanMessage &message) {
  const dag_node_index dag_size = query_dag_->size();
  // Get the relational operator that caused this message to be sent.
  dag_node_index response_op_index = message.getRelationalOpIndex();
  const int worker_id = message.getWorkerID();

  switch (message.getType()) {
    case ForemanMessage::kWorkOrderCompletion:
      {
        // Completion of a regular WorkOrder.
        DEBUG_ASSERT(worker_id >= 0);
        --queued_workorders_per_op_[response_op_index];
        // As the given worker finished executing a WorkOrder, decrement its
        // number of queued WorkOrders.
        workers_->decrementNumQueuedWorkOrders(worker_id);

        // Check if new work orders are available and fetch them if so.
        fetchNormalWorkOrders(
            query_dag_->getNodePayloadMutable(response_op_index),
            response_op_index);

        if (checkRebuildRequired(response_op_index)) {
          if (checkNormalExecutionOver(response_op_index)) {
            if (!checkRebuildInitiated(response_op_index)) {
              if (initiateRebuild(response_op_index)) {
                // Rebuild initiated and completed right away.
                markOperatorFinished(response_op_index);
              } else {
                // Rebuild under progress.
              }
            } else if (checkRebuildOver(response_op_index)) {
              // Rebuild was under progress and now it is over.
              markOperatorFinished(response_op_index);
            }
          } else {
            // Normal execution under progress for this operator.
          }
        } else if (checkOperatorExecutionOver(response_op_index)) {
          // Rebuild not required for this operator and its normal execution is
          // complete.
          markOperatorFinished(response_op_index);
        }

        for (pair<dag_node_index, bool> dependent_link :
             query_dag_->getDependents(response_op_index)) {
          RelationalOperator *dependent_op =
              query_dag_->getNodePayloadMutable(dependent_link.first);
          if (checkAllBlockingDependenciesMet(dependent_link.first)) {
            // Process the dependent operator (of the operator whose WorkOrder
            // was just executed) for which all the dependencies have been met.
            processOperator(dependent_op, dependent_link.first, true);
          }
        }
      }
      break;
    case ForemanMessage::kRebuildCompletion:
      {
        DEBUG_ASSERT(worker_id >= 0);
        // Completion of a rebuild WorkOrder.
        --rebuild_status_[response_op_index].second;
        workers_->decrementNumQueuedWorkOrders(worker_id);

        if (checkRebuildOver(response_op_index)) {
          markOperatorFinished(response_op_index);
          for (pair<dag_node_index, bool> dependent_link :
               query_dag_->getDependents(response_op_index)) {
            RelationalOperator *dependent_op =
                query_dag_->getNodePayloadMutable(dependent_link.first);
            if (checkAllBlockingDependenciesMet(dependent_link.first)) {
              processOperator(dependent_op, dependent_link.first, true);
            }
          }
        }
      }
      break;
    case ForemanMessage::kDataPipeline:
      {
        // Data streaming message. Possible senders of this message include
        // InsertDestination and some operators which modify existing blocks.
        for (dag_node_index consumer_index :
             output_consumers_[response_op_index]) {
          RelationalOperator *consumer_op =
              query_dag_->getNodePayloadMutable(consumer_index);
          // Feed the streamed block to the consumer. Note that
          // output_consumers_ only contain those dependents of operator with
          // index = response_op_index which are eligible to receive streamed
          // input.
          consumer_op->feedInputBlock(message.getOutputBlockID(),
                                      message.getRelationID());
          // Because of the streamed input just fed, check if there are any new
          // WorkOrders available and if so, fetch them.
          fetchNormalWorkOrders(consumer_op, consumer_index);
        }  // end for (feeding input to dependents)
      }
      break;
    case ForemanMessage::kWorkOrdersAvailable: {
      // Check if new work orders are available.
      fetchNormalWorkOrders(
          query_dag_->getNodePayloadMutable(response_op_index),
          response_op_index);
      break;
    }
    default:
      FATAL_ERROR("Unknown ForemanMessage type");
  }

  // Dispatch the WorkerMessages to the workers. We prefer to start the search
  // for the schedulable WorkOrders beginning from response_op_index. The first
  // candidate worker to receive the next WorkOrder is the one that sent the
  // response message to Foreman.
  dispatchWorkerMessages(((worker_id >= 0) ? worker_id : 0), response_op_index);
  return num_operators_finished_ == dag_size;
}

void Foreman::processFeedbackMessage(const WorkOrder::FeedbackMessage &msg) {
  RelationalOperator *op =
      query_dag_->getNodePayloadMutable(msg.header().rel_op_index);
  op->receiveFeedbackMessage(msg);
}

void Foreman::run() {
  // Initialize before for Foreman eventloop.
  bool done = initialize();

  // Event loop
  while (!done) {
    // Receive() causes this thread to sleep until next message is received.
    AnnotatedMessage annotated_msg = bus_->Receive(foreman_client_id_, 0, true);
    // Message is either workorder feedback message or foreman message.
    if (annotated_msg.tagged_message.message_type() == kWorkOrderFeedbackMessage) {
      WorkOrder::FeedbackMessage msg(
          const_cast<void *>(annotated_msg.tagged_message.message()),
          annotated_msg.tagged_message.message_bytes());
      processFeedbackMessage(msg);
    } else {
      done = processMessage(*static_cast<const ForemanMessage *>(
                                annotated_msg.tagged_message.message()));
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

WorkerMessage* Foreman::generateWorkerMessage(
    WorkOrder *workorder,
    dag_node_index index,
    WorkerMessage::WorkerMessageType type) {
  std::unique_ptr<WorkerMessage> worker_message;
  switch (type) {
    case WorkerMessage::kWorkOrder :
      return new WorkerMessage(WorkerMessage::WorkOrderMessage(workorder, index));
    case WorkerMessage::kRebuildWorkOrder:
      return new WorkerMessage(
          WorkerMessage::RebuildWorkOrderMessage(workorder, index));
    default:
      FATAL_ERROR("Called Foreman::generateWorkerMessage() with invalid "
                  "WorkerMesageType argument");
  }
}

void Foreman::initializeState() {
  const dag_node_index dag_size = query_dag_->size();
  num_operators_finished_ = 0;

  done_gen_.assign(dag_size, false);

  execution_finished_.assign(dag_size, false);

  output_consumers_.resize(dag_size);
  blocking_dependencies_.resize(dag_size);

  rebuild_required_.assign(dag_size, false);

  queued_workorders_per_op_.assign(dag_size, 0);

  workorders_container_.reset(new WorkOrdersContainer(dag_size, num_numa_nodes_));

  for (dag_node_index node_index = 0; node_index < dag_size; ++node_index) {
    const QueryContext::insert_destination_id insert_destination_index =
        query_dag_->getNodePayload(node_index).getInsertDestinationID();
    if (insert_destination_index != QueryContext::kInvalidInsertDestinationId) {
      // Rebuild is necessary whenever InsertDestination is present.
      rebuild_required_[node_index] = true;
      rebuild_status_[node_index] = std::make_pair(false, 0);
    }

    for (pair<dag_node_index, bool> dependent_link :
         query_dag_->getDependents(node_index)) {
      if (!query_dag_->getLinkMetadata(node_index, dependent_link.first)) {
        // The link is not a pipeline-breaker. Streaming of blocks is possible
        // between these two operators.
        output_consumers_[node_index].push_back(dependent_link.first);
      } else {
        // The link is a pipeline-breaker. Streaming of blocks is not possible
        // between these two operators.
        blocking_dependencies_[dependent_link.first].push_back(node_index);
      }
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
  std::unique_ptr<WorkerMessage> worker_message;
  WorkOrder *work_order = nullptr;
  size_t num_operators_checked = 0;
  for (dag_node_index index = start_operator_index;
       num_operators_checked < query_dag_->size();
       index = (index + 1) % query_dag_->size(), ++num_operators_checked) {
    if (execution_finished_[index]) {
      continue;
    }
    if (numa_node != -1) {
      // First try to get a normal WorkOrder from the specified NUMA node.
      work_order = workorders_container_->getNormalWorkOrderForNUMANode(index, numa_node);
      if (work_order != nullptr) {
        // A WorkOrder found on the given NUMA node.
        ++queued_workorders_per_op_[index];
        worker_message.reset(generateWorkerMessage(
            work_order, index, WorkerMessage::kWorkOrder));
        return worker_message.release();
      } else {
        // Normal workorder not found on this node. Look for a rebuild workorder
        // on this NUMA node.
        work_order = workorders_container_->getRebuildWorkOrderForNUMANode(index,
                                                                 numa_node);
        if (work_order != nullptr) {
          worker_message.reset(generateWorkerMessage(
              work_order, index, WorkerMessage::kRebuildWorkOrder));
          return worker_message.release();
        }
      }
    }
    // Either no workorder found on the given NUMA node, or numa_node is -1.
    // Try to get a normal WorkOrder from other NUMA nodes.
    work_order = workorders_container_->getNormalWorkOrder(index);
    if (work_order != nullptr) {
      ++queued_workorders_per_op_[index];
      worker_message.reset(generateWorkerMessage(
          work_order, index, WorkerMessage::kWorkOrder));
      return worker_message.release();
    } else {
      // Normal WorkOrder not found, look for a RebuildWorkOrder.
      work_order = workorders_container_->getRebuildWorkOrder(index);
      if (work_order != nullptr) {
        worker_message.reset(generateWorkerMessage(
            work_order, index, WorkerMessage::kRebuildWorkOrder));
        return worker_message.release();
      }
    }
  }
  // No WorkOrders available right now.
  return nullptr;
}

void Foreman::sendWorkerMessage(std::size_t worker_id,
                                const WorkerMessage &message) {
  message_type_id type;
  if (message.getType() == WorkerMessage::kRebuildWorkOrder) {
    type = kRebuildWorkOrderMessage;
  } else if (message.getType() == WorkerMessage::kWorkOrder) {
    type = kWorkOrderMessage;
  } else {
    FATAL_ERROR("Invalid WorkerMessageType");
  }
  TaggedMessage worker_tagged_message;
  worker_tagged_message.set_message(&message, sizeof(message), type);

  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     workers_->getClientID(worker_id),
                                     move(worker_tagged_message));
}

bool Foreman::fetchNormalWorkOrders(RelationalOperator *op, dag_node_index index) {
  bool generated_new_workorders = false;
  if (!done_gen_[index]) {
    const size_t num_pending_workorders_before =
        workorders_container_->getNumNormalWorkOrders(index);
    done_gen_[index] = op->getAllWorkOrders(workorders_container_.get());

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

void Foreman::processOperator(RelationalOperator *op,
                              dag_node_index index,
                              bool recursively_check_dependents) {
  if (fetchNormalWorkOrders(op, index)) {
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
      for (pair<dag_node_index, bool> dependent_link :
           query_dag_->getDependents(index)) {
        if (checkAllBlockingDependenciesMet(dependent_link.first)) {
          processOperator(
              query_dag_->getNodePayloadMutable(dependent_link.first),
              dependent_link.first, true);
        }
      }
    }
  }
}

void Foreman::markOperatorFinished(dag_node_index index) {
  execution_finished_[index] = true;
  ++num_operators_finished_;
  const relation_id output_rel = query_dag_->getNodePayload(index).getOutputRelationID();
  for (pair<dag_node_index, bool> dependent_link : query_dag_->getDependents(index)) {
    RelationalOperator *dependent_op = query_dag_->getNodePayloadMutable(dependent_link.first);
    // Signal dependent operator that current operator is done feeding input blocks.
    if (output_rel >= 0) {
      dependent_op->doneFeedingInputBlocks(output_rel);
    }
    if (checkAllBlockingDependenciesMet(dependent_link.first)) {
      dependent_op->informAllBlockingDependenciesMet();
    }
  }
}

bool Foreman::initiateRebuild(dag_node_index index) {
  DEBUG_ASSERT(!workorders_container_->hasRebuildWorkOrder(index));
  DEBUG_ASSERT(checkRebuildRequired(index));
  DEBUG_ASSERT(!checkRebuildInitiated(index));

  getRebuildWorkOrders(index, workorders_container_.get());

  rebuild_status_[index] = std::make_pair(
      true, workorders_container_->getNumRebuildWorkOrders(index));

  return (rebuild_status_[index].second == 0);
}

void Foreman::getRebuildWorkOrders(dag_node_index index, WorkOrdersContainer *container) {
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

}  // namespace quickstep
