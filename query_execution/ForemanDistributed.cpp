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

#include "query_execution/ForemanDistributed.hpp"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "threading/ThreadUtil.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::free;
using std::make_pair;
using std::malloc;
using std::move;
using std::pair;
using std::size_t;
using std::unique_ptr;
using std::vector;

using tmb::AnnotatedMessage;
using tmb::TaggedMessage;
using tmb::client_id;

namespace quickstep {

void ForemanDistributed::initialize() {
  if (cpu_id_ >= 0) {
    // We can pin the foreman thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }

  // Ensure that at least one Shiftboss to register.
  if (shiftbosses_.empty()) {
    const AnnotatedMessage annotated_message(bus_->Receive(foreman_client_id_, 0, true));
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    LOG(INFO) << "ForemanDistributed received typed '" << tagged_message.message_type()
              << "' message from client " << annotated_message.sender;
    DCHECK_EQ(kShiftbossRegistrationMessage, tagged_message.message_type());

    serialization::ShiftbossRegistrationMessage proto;
    CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

    processShiftbossRegisterationMessage(annotated_message.sender, proto.work_order_capacity());
    DCHECK_EQ(1u, shiftbosses_.size());
  }

  initializeState();

  // NOTE(zuyu): Should call before processOperator(). Otherwise, an
  // InitiateRebuildMessage may be sent before QueryContext initializes.
  sendShiftbossInitiateMessage();

  // Wait Shiftboss for ShiftbossInitiateResponseMessage.
  const AnnotatedMessage annotated_message(bus_->Receive(foreman_client_id_, 0, true));
  const TaggedMessage &tagged_message = annotated_message.tagged_message;
  LOG(INFO) << "ForemanDistributed received typed '" << tagged_message.message_type()
            << "' message from client " << annotated_message.sender;
  DCHECK_EQ(kShiftbossInitiateResponseMessage, tagged_message.message_type());

  serialization::ShiftbossInitiateResponseMessage proto;
  CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

  DCHECK(query_dag_ != nullptr);
  const dag_node_index dag_size = query_dag_->size();

  // Collect all the workorders from all the relational operators in the DAG.
  for (dag_node_index index = 0; index < dag_size; ++index) {
    if (checkAllBlockingDependenciesMet(index)) {
      query_dag_->getNodePayloadMutable(index)->informAllBlockingDependenciesMet();
      processOperator(0 /* shiftboss_index */, index, false);
    }
  }

  // Dispatch the WorkOrder protos generated so far.
  dispatchToShiftboss(0 /* shiftboss_index */, 0 /* op_index */);
}

void ForemanDistributed::processShiftbossRegisterationMessage(const client_id shiftboss_client_id,
                                                              const std::size_t work_order_capacity) {
  shiftbosses_.addShiftboss(shiftboss_client_id, work_order_capacity);

  serialization::ShiftbossRegistrationResponseMessage proto;

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kShiftbossRegistrationResponseMessage);
  free(proto_bytes);

  LOG(INFO) << "ForemanDistributed sent ShiftbossRegistrationResponseMessage (typed '"
            << kShiftbossRegistrationResponseMessage
            << "') to Shiftboss with TMB client id " << shiftboss_client_id;
  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     shiftboss_client_id,
                                     move(message));
}

void ForemanDistributed::processWorkOrderCompleteMessage(const size_t shiftboss_index,
                                                         const dag_node_index op_index) {
  --queued_workorders_per_op_[op_index];
  // As the given worker finished executing a WorkOrder, decrement its
  // number of queued WorkOrders.
  shiftbosses_.decrementNumQueuedWorkOrders(shiftboss_index);

  // Check if new work orders are available and fetch them if so.
  fetchNormalWorkOrders(op_index);

  if (checkRebuildRequired(op_index)) {
    if (checkNormalExecutionOver(op_index)) {
      if (!checkRebuildInitiated(op_index)) {
        initiateRebuild(shiftboss_index, op_index);
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

  for (pair<dag_node_index, bool> dependent_link :
       query_dag_->getDependents(op_index)) {
    if (checkAllBlockingDependenciesMet(dependent_link.first)) {
      // Process the dependent operator (of the operator whose WorkOrder
      // was just executed) for which all the dependencies have been met.
      processOperator(shiftboss_index, dependent_link.first, true);
    }
  }

  // Dispatch WorkOrders to Shiftboss. We prefer to start the search for the
  // schedulable WorkOrders beginning from 'op_index'. The first candidate
  // Shiftboss to receive the next WorkOrder is the one that sent the response
  // message to Foreman.
  dispatchToShiftboss(shiftboss_index, op_index);
}

void ForemanDistributed::processInitiateRebuildResponseMessage(const size_t shiftboss_index,
                                                               const dag_node_index op_index,
                                                               const std::size_t num_rebuild_work_orders) {
  rebuild_status_[op_index].second = num_rebuild_work_orders;
  shiftbosses_.addNumQueuedWorkOrders(shiftboss_index, num_rebuild_work_orders);

  if (num_rebuild_work_orders == 0u) {
    markOperatorFinished(op_index);

    for (pair<dag_node_index, bool> dependent_link :
         query_dag_->getDependents(op_index)) {
      if (checkAllBlockingDependenciesMet(dependent_link.first)) {
        processOperator(shiftboss_index, dependent_link.first, true);
      }
    }
  }

  // Dispatch WorkOrders to Shiftboss. We prefer to start the search for the
  // schedulable WorkOrders beginning from 'op_index'. The first candidate
  // Shiftboss to receive the next WorkOrder is the one that sent the response
  // message to Foreman.
  dispatchToShiftboss(shiftboss_index, op_index);
}

void ForemanDistributed::processRebuildWorkOrderCompleteMessage(const size_t shiftboss_index,
                                                                const dag_node_index op_index) {
  DCHECK_GT(rebuild_status_[op_index].second, 0);
  --rebuild_status_[op_index].second;
  shiftbosses_.decrementNumQueuedWorkOrders(shiftboss_index);

  if (checkRebuildOver(op_index)) {
    markOperatorFinished(op_index);

    for (pair<dag_node_index, bool> dependent_link :
         query_dag_->getDependents(op_index)) {
      if (checkAllBlockingDependenciesMet(dependent_link.first)) {
        processOperator(shiftboss_index, dependent_link.first, true);
      }
    }
  }

  // Dispatch WorkOrders to Shiftboss. We prefer to start the search for the
  // schedulable WorkOrders beginning from 'op_index'. The first candidate
  // Shiftboss to receive the next WorkOrder is the one that sent the response
  // message to Foreman.
  dispatchToShiftboss(shiftboss_index, op_index);
}

void ForemanDistributed::processDataPipelineMessage(const size_t shiftboss_index,
                                                    const dag_node_index op_index,
                                                    const block_id block,
                                                    const relation_id rel_id) {
  for (dag_node_index consumer_index :
       output_consumers_[op_index]) {
    // Feed the streamed block to the consumer. Note that output_consumers_ only
    // contain those dependents of operator with index = op_index which are
    // eligible to receive streamed input.
    query_dag_->getNodePayloadMutable(consumer_index)->feedInputBlock(block, rel_id);

    // Because of the streamed input just fed, check if there are any new
    // WorkOrders available and if so, fetch them.
    fetchNormalWorkOrders(consumer_index);
  }

  // Dispatch WorkOrders to Shiftboss. We prefer to start the search for the
  // schedulable WorkOrders beginning from 'op_index'. The first candidate
  // Shiftboss to receive the next WorkOrder is the one that sent the response
  // message to Foreman.
  dispatchToShiftboss(shiftboss_index, op_index);
}

void ForemanDistributed::processFeedbackMessage(const WorkOrder::FeedbackMessage &msg) {
  RelationalOperator *op =
      query_dag_->getNodePayloadMutable(msg.header().rel_op_index);
  op->receiveFeedbackMessage(msg);
}

void ForemanDistributed::run() {
  // Initialize before for Foreman eventloop.
  initialize();

  // Event loop
  while (!checkQueryExecutionFinished()) {
    // Receive() causes this thread to sleep until next message is received.
    AnnotatedMessage annotated_message = bus_->Receive(foreman_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    LOG(INFO) << "ForemanDistributed received typed '" << tagged_message.message_type()
              << "' message from client " << annotated_message.sender;
    switch (tagged_message.message_type()) {
      case kShiftbossRegistrationMessage: {
        serialization::ShiftbossRegistrationMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processShiftbossRegisterationMessage(annotated_message.sender, proto.work_order_capacity());
        break;
      }
      case kShiftbossInitiateResponseMessage: {
        serialization::ShiftbossInitiateResponseMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        // TODO(quickstep-team): Support concurrent queries.
        DCHECK_EQ(query_handle_->query_id(), proto.query_id());
        break;
      }
      case kWorkOrderCompleteMessage: {
        const std::size_t shiftboss_index = shiftbosses_.getShiftbossIndex(annotated_message.sender);

        serialization::WorkOrderCompletionMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processWorkOrderCompleteMessage(shiftboss_index, proto.operator_index());
        break;
      }
      case kInitiateRebuildResponseMessage: {
        const std::size_t shiftboss_index = shiftbosses_.getShiftbossIndex(annotated_message.sender);

        serialization::InitiateRebuildResponseMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processInitiateRebuildResponseMessage(shiftboss_index,
                                              proto.operator_index(),
                                              proto.num_rebuild_work_orders());
        break;
      }
      case kRebuildWorkOrderCompleteMessage: {
        const std::size_t shiftboss_index = shiftbosses_.getShiftbossIndex(annotated_message.sender);

        serialization::WorkOrderCompletionMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processRebuildWorkOrderCompleteMessage(shiftboss_index, proto.operator_index());
        break;
      }
      case kCatalogRelationNewBlockMessage: {
        serialization::CatalogRelationNewBlockMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        CatalogRelation *relation = database_->getRelationByIdMutable(proto.relation_id());
        relation->addBlock(proto.block_id());
        break;
      }
      case kDataPipelineMessage: {
        const std::size_t shiftboss_index = shiftbosses_.getShiftbossIndex(annotated_message.sender);

        // Possible senders of this message include InsertDestination and some
        // operators which modify existing blocks.
        serialization::DataPipelineMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processDataPipelineMessage(shiftboss_index,
                                   proto.operator_index(),
                                   proto.block_id(),
                                   proto.relation_id());
        break;
      }
      case kWorkOrdersAvailableMessage: {
        const std::size_t shiftboss_index = shiftbosses_.getShiftbossIndex(annotated_message.sender);

        serialization::WorkOrdersAvailableMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const dag_node_index op_index = proto.operator_index();

        // Check if new work orders are available.
        fetchNormalWorkOrders(op_index);

        // Dispatch WorkOrders to Shiftboss. We prefer to start the search for the
        // schedulable WorkOrders beginning from 'op_index'. The first candidate
        // Shiftboss to receive the next WorkOrder is the one that sent the response
        // message to Foreman.
        dispatchToShiftboss(shiftboss_index, op_index);
        break;
      }
      case kWorkOrderFeedbackMessage: {
        WorkOrder::FeedbackMessage msg(
            const_cast<void *>(annotated_message.tagged_message.message()),
            annotated_message.tagged_message.message_bytes());
        processFeedbackMessage(msg);
        break;
      }
      default: {
        LOG(FATAL) << "Invalid message type";
      }
    }
  }

  const CatalogRelation *query_result_relation = query_handle_->getQueryResultRelation();
  if (query_result_relation != nullptr) {
    serialization::QueryResultRelationMessage proto;
    proto.set_relation_id(query_result_relation->getID());

    const vector<block_id> blocks(query_result_relation->getBlocksSnapshot());
    for (const block_id block : blocks) {
      proto.add_blocks(block);
    }

    const size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    TaggedMessage message(static_cast<const void*>(proto_bytes),
                          proto_length,
                          kQueryResultRelationMessage);
    free(proto_bytes);

    LOG(INFO) << "ForemanDistributed sent QueryResultRelationMessage (typed '" << kQueryResultRelationMessage
              << "') to Shiftboss 0";
    // TODO(zuyu): Support multiple shiftbosses.
    QueryExecutionUtil::SendTMBMessage(bus_,
                                       foreman_client_id_,
                                       shiftbosses_.getClientId(0),
                                       move(message));

    // Wait for Shiftboss to flush the query result blocks into disk.
    AnnotatedMessage annotated_message(bus_->Receive(foreman_client_id_, 0, true));
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    DCHECK_EQ(kQueryResultRelationResponseMessage, tagged_message.message_type());
    LOG(INFO) << "ForemanDistributed received QueryResultRelationResponseMessage from Shiftboss 0";

    serialization::QueryResultRelationResponseMessage ack_proto;
    CHECK(ack_proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));
    DCHECK_EQ(query_result_relation->getID(), ack_proto.relation_id());
  }

  // Clean up before exiting.
  cleanUp();
}

void ForemanDistributed::dispatchToShiftboss(const size_t start_shiftboss_index,
                                             const dag_node_index start_operator_index) {
  // Loop over all Shiftbosses. Stopping criteria:
  // 1. Every Shiftboss has been assigned exactly the number of workorders that
  //    equals to its capacity,
  // OR 2. No schedulable workorders at this time.
  size_t done_shiftbosses_count = 0u;
  for (size_t curr_shiftboss = start_shiftboss_index;
       done_shiftbosses_count < shiftbosses_.size();
       curr_shiftboss = (curr_shiftboss + 1u) % shiftbosses_.size()) {
    if (shiftbosses_.hasReachedCapacity(curr_shiftboss)) {
      // The current Shiftboss is full of queued WorkOrder for execution.
      ++done_shiftbosses_count;
      continue;
    }

    unique_ptr<serialization::WorkOrderMessage> proto(getWorkOrderMessage(start_operator_index));
    if (proto) {
      const size_t proto_length = proto->ByteSize();
      char *proto_bytes = static_cast<char*>(malloc(proto_length));
      CHECK(proto->SerializeToArray(proto_bytes, proto_length));

      TaggedMessage tagged_message(static_cast<const void*>(proto_bytes),
                                   proto_length,
                                   kWorkOrderMessage);
      free(proto_bytes);

      LOG(INFO) << "ForemanDistributed sent WorkOrderMessage (typed '" << kWorkOrderMessage
                << "') to Shiftboss " << curr_shiftboss;
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         shiftbosses_.getClientId(curr_shiftboss),
                                         move(tagged_message));

      shiftbosses_.incrementNumQueuedWorkOrders(curr_shiftboss);
    } else {
      // No schedulable normal workorder at this point.
      ++done_shiftbosses_count;
    }
  }
}

void ForemanDistributed::initializeState() {
  const dag_node_index dag_size = query_dag_->size();
  num_operators_finished_ = 0;

  done_gen_.assign(dag_size, false);

  execution_finished_.assign(dag_size, false);

  output_consumers_.resize(dag_size);
  blocking_dependencies_.resize(dag_size);

  rebuild_required_.assign(dag_size, false);

  queued_workorders_per_op_.assign(dag_size, 0);

  normal_workorder_protos_container_.reset(new WorkOrderProtosContainer(dag_size));

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

void ForemanDistributed::sendShiftbossInitiateMessage() {
  serialization::ShiftbossInitiateMessage proto;
  proto.set_query_id(query_handle_->query_id());
  proto.mutable_catalog_database_cache()->MergeFrom(query_handle_->getCatalogDatabaseCacheProto());
  proto.mutable_query_context()->MergeFrom(query_handle_->getQueryContextProto());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kShiftbossInitiateMessage);
  free(proto_bytes);

  LOG(INFO) << "ForemanDistributed sent ShiftbossInitiateMessage (typed '" << kShiftbossInitiateMessage
            << "') to Shiftboss 0";

  // TODO(zuyu): Multiple Shiftbosses support.
  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     shiftbosses_.getClientId(0),
                                     move(message));
}

// TODO(harshad) : The default policy may execute remote WorkOrders for an
// operator with a lower index even when there are local WorkOrders available for
// an operator with higher index. We should examine if avoiding this behavior
// has any benefits with respect to execution time and/or memory pressure.
serialization::WorkOrderMessage*
ForemanDistributed::getWorkOrderMessage(const dag_node_index start_operator_index) {
  size_t num_operators_checked = 0u;
  for (dag_node_index index = start_operator_index;
       num_operators_checked < query_dag_->size();
       index = (index + 1) % query_dag_->size(), ++num_operators_checked) {
    if (execution_finished_[index]) {
      continue;
    }

    unique_ptr<serialization::WorkOrder> work_order_proto(
        normal_workorder_protos_container_->getWorkOrderProto(index));
    if (work_order_proto) {
      ++queued_workorders_per_op_[index];

      unique_ptr<serialization::WorkOrderMessage> message_proto(new serialization::WorkOrderMessage);
      message_proto->mutable_work_order()->MergeFrom(*work_order_proto);
      message_proto->set_operator_index(index);

      return message_proto.release();
    }
  }

  // No normal WorkOrders available right now.
  return nullptr;
}

bool ForemanDistributed::fetchNormalWorkOrders(const dag_node_index index) {
  bool generated_new_workorders = false;
  if (!done_gen_[index]) {
    const size_t num_pending_workorders_before =
        normal_workorder_protos_container_->getNumWorkOrderProtos(index);
    done_gen_[index] =
        query_dag_->getNodePayloadMutable(index)
            ->getAllWorkOrderProtos(normal_workorder_protos_container_.get());

    // TODO(shoban): It would be a good check to see if operator is making
    // useful progress, i.e., the operator either generates work orders to
    // execute or still has pending work orders executing. However, this will not
    // work if Foreman polls operators without feeding data. This check can be
    // enabled, if Foreman is refactored to call getAllWorkOrders() only when
    // pending work orders are completed or new input blocks feed.

    generated_new_workorders =
        (num_pending_workorders_before <
         normal_workorder_protos_container_->getNumWorkOrderProtos(index));
  }
  return generated_new_workorders;
}

void ForemanDistributed::processOperator(const size_t shiftboss_index,
                                         const dag_node_index index,
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
        initiateRebuild(shiftboss_index, index);

        // Wait Shiftboss to generate RebuildWorkOrders.
        return;
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
          processOperator(shiftboss_index, dependent_link.first, true);
        }
      }
    }
  }
}

void ForemanDistributed::markOperatorFinished(const dag_node_index index) {
  execution_finished_[index] = true;
  ++num_operators_finished_;

  RelationalOperator *op = query_dag_->getNodePayloadMutable(index);
  op->updateCatalogOnCompletion();

  const relation_id output_rel = op->getOutputRelationID();
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

void ForemanDistributed::initiateRebuild(const size_t shiftboss_index,
                                         const dag_node_index index) {
  DCHECK(checkRebuildRequired(index));
  DCHECK(!checkRebuildInitiated(index));

  const RelationalOperator &op = query_dag_->getNodePayload(index);
  DCHECK_NE(op.getInsertDestinationID(), QueryContext::kInvalidInsertDestinationId);

  serialization::InitiateRebuildMessage proto;
  proto.set_operator_index(index);
  proto.set_insert_destination_index(op.getInsertDestinationID());
  proto.set_relation_id(op.getOutputRelationID());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage tagged_msg(static_cast<const void *>(proto_bytes),
                           proto_length,
                           kInitiateRebuildMessage);
  free(proto_bytes);

  LOG(INFO) << "ForemanDistributed sent InitiateRebuildMessage (typed '" << kInitiateRebuildMessage
            << "') to Shiftboss with TMB client id " << shiftboss_index;
  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     shiftbosses_.getClientId(shiftboss_index),
                                     move(tagged_msg));

  // The negative value indicates that the number of rebuild work orders is to be
  // determined.
  rebuild_status_[index] = make_pair(true, -1);
}

}  // namespace quickstep
