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

#include "query_execution/ForemanDistributed.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "cli/Flags.hpp"
#include "query_execution/AdmitRequestMessage.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/BlockLocatorUtil.hpp"
#include "query_execution/PolicyEnforcerBase.hpp"
#include "query_execution/PolicyEnforcerDistributed.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

using std::make_unique;
using std::move;
using std::size_t;
using std::unique_ptr;
using std::vector;

using tmb::AnnotatedMessage;
using tmb::MessageBus;
using tmb::TaggedMessage;
using tmb::client_id;

namespace quickstep {

namespace S = serialization;

class QueryHandle;

ForemanDistributed::ForemanDistributed(
    const BlockLocator &block_locator,
    MessageBus *bus,
    CatalogDatabaseLite *catalog_database,
    QueryProcessor *query_processor,
    const int cpu_id)
    : ForemanBase(bus, cpu_id),
      block_locator_(block_locator),
      catalog_database_(DCHECK_NOTNULL(catalog_database)) {
  const std::vector<QueryExecutionMessageType> sender_message_types{
      kBlockDomainRegistrationMessage,
      kShiftbossRegistrationResponseMessage,
      kQueryInitiateMessage,
      kWorkOrderMessage,
      kInitiateRebuildMessage,
      kQueryTeardownMessage,
      kQueryExecutionSuccessMessage,
      kCommandResponseMessage,
      kPoisonMessage};

  for (const auto message_type : sender_message_types) {
    bus_->RegisterClientAsSender(foreman_client_id_, message_type);
  }

  const std::vector<QueryExecutionMessageType> receiver_message_types{
      kBlockDomainRegistrationResponseMessage,
      kShiftbossRegistrationMessage,
      kAdmitRequestMessage,
      kQueryInitiateResponseMessage,
      kCatalogRelationNewBlockMessage,
      kDataPipelineMessage,
      kInitiateRebuildResponseMessage,
      kWorkOrderCompleteMessage,
      kRebuildWorkOrderCompleteMessage,
      kWorkOrderFeedbackMessage,
      kPoisonMessage};

  for (const auto message_type : receiver_message_types) {
    bus_->RegisterClientAsReceiver(foreman_client_id_, message_type);
  }

  client_id locator_client_id;
  storage_manager_ = make_unique<StorageManager>(
      FLAGS_storage_path,
      block_locator::getBlockDomain(data_exchanger_.network_address(), foreman_client_id_, &locator_client_id, bus_),
      locator_client_id, bus_);

  data_exchanger_.set_storage_manager(storage_manager_.get());
  data_exchanger_.start();

  policy_enforcer_ = make_unique<PolicyEnforcerDistributed>(
      foreman_client_id_, catalog_database_, query_processor, storage_manager_.get(), &shiftboss_directory_, bus_);
}

void ForemanDistributed::run() {
  if (cpu_id_ >= 0) {
    // We can pin the foreman thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }

  // Ensure that at least one Shiftboss to register.
  if (shiftboss_directory_.empty()) {
    const AnnotatedMessage annotated_message = bus_->Receive(foreman_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    DCHECK_EQ(kShiftbossRegistrationMessage, tagged_message.message_type());
    DLOG(INFO) << "ForemanDistributed received ShiftbossRegistrationMessage from Client " << annotated_message.sender;

    S::ShiftbossRegistrationMessage proto;
    CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

    processShiftbossRegistrationMessage(annotated_message.sender, proto.work_order_capacity());
    DCHECK_EQ(1u, shiftboss_directory_.size());
  }

  // Event loop
  for (;;) {
    // Receive() causes this thread to sleep until next message is received.
    const AnnotatedMessage annotated_message =
        bus_->Receive(foreman_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    const tmb::message_type_id message_type = tagged_message.message_type();
    DLOG(INFO) << "ForemanDistributed received " << QueryExecutionUtil::MessageTypeToString(message_type)
               << " from Client " << annotated_message.sender;
    switch (message_type) {
      case kShiftbossRegistrationMessage: {
        S::ShiftbossRegistrationMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processShiftbossRegistrationMessage(annotated_message.sender, proto.work_order_capacity());
        break;
      }
      case kAdmitRequestMessage: {
        const AdmitRequestMessage *request_message =
            static_cast<const AdmitRequestMessage*>(tagged_message.message());

        const vector<QueryHandle *> &query_handles = request_message->getQueryHandles();
        DCHECK(!query_handles.empty());

        bool all_queries_admitted = true;
        if (query_handles.size() == 1u) {
          all_queries_admitted =
              policy_enforcer_->admitQuery(query_handles.front());
        } else {
          all_queries_admitted = policy_enforcer_->admitQueries(query_handles);
        }
        if (!all_queries_admitted) {
          LOG(WARNING) << "The scheduler could not admit all the queries";
          // TODO(harshad) - Inform the main thread about the failure.
        }
        break;
      }
      case kQueryInitiateResponseMessage: {
        S::QueryInitiateResponseMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));
        break;
      }
      case kCatalogRelationNewBlockMessage:  // Fall through
      case kDataPipelineMessage:
      case kRebuildWorkOrderCompleteMessage:
      case kWorkOrderCompleteMessage:
      case kWorkOrderFeedbackMessage: {
        policy_enforcer_->processMessage(tagged_message);
        break;
      }
      case kInitiateRebuildResponseMessage: {
        // A unique case in the distributed version.
        static_cast<PolicyEnforcerDistributed*>(policy_enforcer_.get())->
            processInitiateRebuildResponseMessage(tagged_message);
        break;
      }
      case kPoisonMessage: {
        if (policy_enforcer_->hasQueries()) {
          LOG(WARNING) << "ForemanDistributed thread exiting while some queries are "
                          "under execution or waiting to be admitted";
        }

        // Shutdown all Shiftbosses.
        tmb::Address shiftboss_addresses;
        for (std::size_t i = 0; i < shiftboss_directory_.size(); ++i) {
          shiftboss_addresses.AddRecipient(shiftboss_directory_.getClientId(i));
        }

        tmb::MessageStyle broadcast_style;
        broadcast_style.Broadcast(true);

        TaggedMessage poison_message(kPoisonMessage);

        const MessageBus::SendStatus send_status =
            bus_->Send(foreman_client_id_,
                       shiftboss_addresses,
                       broadcast_style,
                       move(poison_message));
        DCHECK(send_status == MessageBus::SendStatus::kOK);
        return;
      }
      default:
        LOG(FATAL) << "Unknown message type to ForemanDistributed";
    }

    if (canCollectNewMessages(message_type)) {
      vector<unique_ptr<S::WorkOrderMessage>> new_messages;
      static_cast<PolicyEnforcerDistributed*>(policy_enforcer_.get())->
          getWorkOrderProtoMessages(&new_messages);
      dispatchWorkOrderMessages(new_messages);
    }
  }
}

bool ForemanDistributed::canCollectNewMessages(const tmb::message_type_id message_type) {
  return !QUICKSTEP_EQUALS_ANY_CONSTANT(message_type,
                                        kCatalogRelationNewBlockMessage,
                                        kWorkOrderFeedbackMessage);
}

bool ForemanDistributed::isAggregationRelatedWorkOrder(const S::WorkOrderMessage &proto,
                                                       const size_t next_shiftboss_index_to_schedule,
                                                       size_t *shiftboss_index_for_aggregation) {
  const S::WorkOrder &work_order_proto = proto.work_order();
  QueryContext::aggregation_state_id aggr_state_index;

  switch (work_order_proto.work_order_type()) {
    case S::AGGREGATION:
      aggr_state_index = work_order_proto.GetExtension(S::AggregationWorkOrder::aggr_state_index);
      break;
    case S::FINALIZE_AGGREGATION:
      aggr_state_index = work_order_proto.GetExtension(S::FinalizeAggregationWorkOrder::aggr_state_index);
      break;
    case S::DESTROY_AGGREGATION_STATE:
      aggr_state_index = work_order_proto.GetExtension(S::DestroyAggregationStateWorkOrder::aggr_state_index);
      break;
    default:
      return false;
  }

  static_cast<PolicyEnforcerDistributed*>(policy_enforcer_.get())->getShiftbossIndexForAggregation(
      proto.query_id(), aggr_state_index, next_shiftboss_index_to_schedule, shiftboss_index_for_aggregation);

  return true;
}

bool ForemanDistributed::isHashJoinRelatedWorkOrder(const S::WorkOrderMessage &proto,
                                                    const size_t next_shiftboss_index_to_schedule,
                                                    size_t *shiftboss_index_for_hash_join) {
  const S::WorkOrder &work_order_proto = proto.work_order();
  QueryContext::join_hash_table_id join_hash_table_index;
  partition_id part_id;

  switch (work_order_proto.work_order_type()) {
    case S::BUILD_HASH:
      join_hash_table_index = work_order_proto.GetExtension(S::BuildHashWorkOrder::join_hash_table_index);
      part_id = work_order_proto.GetExtension(S::BuildHashWorkOrder::partition_id);
      break;
    case S::HASH_JOIN:
      join_hash_table_index = work_order_proto.GetExtension(S::HashJoinWorkOrder::join_hash_table_index);
      part_id = work_order_proto.GetExtension(S::HashJoinWorkOrder::partition_id);
      break;
    case S::DESTROY_HASH:
      join_hash_table_index = work_order_proto.GetExtension(S::DestroyHashWorkOrder::join_hash_table_index);
      part_id = work_order_proto.GetExtension(S::DestroyHashWorkOrder::partition_id);
      break;
    default:
      return false;
  }

  static_cast<PolicyEnforcerDistributed*>(policy_enforcer_.get())->getShiftbossIndexForHashJoin(
      proto.query_id(), join_hash_table_index, part_id, next_shiftboss_index_to_schedule,
      shiftboss_index_for_hash_join);

  return true;
}

namespace {

constexpr size_t kDefaultShiftbossIndex = 0u;

bool isNestedLoopsJoinWorkOrder(const serialization::WorkOrder &work_order_proto,
                                const BlockLocator &block_locator,
                                std::size_t *shiftboss_index_for_join) {
  if (work_order_proto.work_order_type() != S::NESTED_LOOP_JOIN) {
    return false;
  }

  const block_id left_block = work_order_proto.GetExtension(S::NestedLoopsJoinWorkOrder::left_block_id);
  if (block_locator.getBlockLocalityInfo(left_block, shiftboss_index_for_join)) {
    return true;
  }

  const block_id right_block = work_order_proto.GetExtension(S::NestedLoopsJoinWorkOrder::right_block_id);
  return block_locator.getBlockLocalityInfo(right_block, shiftboss_index_for_join);
}

bool hasBlockLocalityInfo(const serialization::WorkOrder &work_order_proto,
                          const BlockLocator &block_locator,
                          std::size_t *shiftboss_index_for_block) {
  block_id block = kInvalidBlockId;
  switch (work_order_proto.work_order_type()) {
    case S::SAVE_BLOCKS: {
      block = work_order_proto.GetExtension(S::SaveBlocksWorkOrder::block_id);
      break;
    }
    case S::SELECT: {
      block = work_order_proto.GetExtension(S::SelectWorkOrder::block_id);
      break;
    }
    case S::SORT_RUN_GENERATION: {
      block = work_order_proto.GetExtension(S::SortRunGenerationWorkOrder::block_id);
      break;
    }
    default:
      return false;
  }

  DCHECK_NE(block, kInvalidBlockId);
  return block_locator.getBlockLocalityInfo(block, shiftboss_index_for_block);
}

}  // namespace

void ForemanDistributed::dispatchWorkOrderMessages(const vector<unique_ptr<S::WorkOrderMessage>> &messages) {
  static size_t shiftboss_index = kDefaultShiftbossIndex;

  PolicyEnforcerDistributed* policy_enforcer_dist = static_cast<PolicyEnforcerDistributed*>(policy_enforcer_.get());
  for (const auto &message : messages) {
    DCHECK(message != nullptr);
    const S::WorkOrderMessage &proto = *message;
    const S::WorkOrder &work_order_proto = proto.work_order();
    size_t shiftboss_index_for_particular_work_order_type;
    if (policy_enforcer_dist->isSingleNodeQuery(proto.query_id())) {
      // Always schedule the single-node query to the same Shiftboss.
      shiftboss_index_for_particular_work_order_type = kDefaultShiftbossIndex;
    } else if (isAggregationRelatedWorkOrder(proto, shiftboss_index, &shiftboss_index_for_particular_work_order_type)) {
    } else if (isHashJoinRelatedWorkOrder(proto, shiftboss_index, &shiftboss_index_for_particular_work_order_type)) {
    } else if (hasBlockLocalityInfo(work_order_proto, block_locator_,
                                    &shiftboss_index_for_particular_work_order_type)) {
    } else if (isNestedLoopsJoinWorkOrder(work_order_proto, block_locator_,
                                          &shiftboss_index_for_particular_work_order_type)) {
    } else {
      shiftboss_index_for_particular_work_order_type = shiftboss_index;
    }

    sendWorkOrderMessage(shiftboss_index_for_particular_work_order_type, proto);
    shiftboss_directory_.incrementNumQueuedWorkOrders(shiftboss_index_for_particular_work_order_type);

    if (shiftboss_index == shiftboss_index_for_particular_work_order_type &&
        shiftboss_directory_.hasReachedCapacity(shiftboss_index)) {
      shiftboss_index = (shiftboss_index + 1) % shiftboss_directory_.size();
    } else {
      // NOTE(zuyu): This is not the exact round-robin scheduling, as in this case,
      // <shiftboss_index_for_particular_work_order_type> might be scheduled one
      // more WorkOrder for an Aggregation or a HashJoin.
    }
  }
}

void ForemanDistributed::sendWorkOrderMessage(const size_t shiftboss_index,
                                              const S::WorkOrderMessage &proto) {
  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kWorkOrderMessage);
  free(proto_bytes);

  const client_id shiftboss_client_id = shiftboss_directory_.getClientId(shiftboss_index);
  DLOG(INFO) << "ForemanDistributed sent WorkOrderMessage to Shiftboss with Client " << shiftboss_client_id;
  const MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         shiftboss_client_id,
                                         move(message));
  CHECK(send_status == MessageBus::SendStatus::kOK);
}

void ForemanDistributed::printWorkOrderProfilingResults(const std::size_t query_id,
                                                        std::FILE *out) const {
  const std::vector<WorkOrderTimeEntry> &recorded_times =
      policy_enforcer_->getProfilingResults(query_id);
  fputs("Query ID,Worker ID,Operator ID,Time (microseconds)\n", out);
  for (const auto &workorder_entry : recorded_times) {
    const std::size_t worker_id = workorder_entry.worker_id;
    fprintf(out,
            "%lu,%lu,%lu,%lu\n",
            query_id,
            worker_id,
            workorder_entry.operator_id,  // Operator ID.
            workorder_entry.end_time - workorder_entry.start_time);  // Time.
  }
}

void ForemanDistributed::processShiftbossRegistrationMessage(const client_id shiftboss_client_id,
                                                             const std::size_t work_order_capacity) {
  S::ShiftbossRegistrationResponseMessage proto;
  proto.set_shiftboss_index(shiftboss_directory_.size());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kShiftbossRegistrationResponseMessage);
  free(proto_bytes);

  shiftboss_directory_.addShiftboss(shiftboss_client_id, work_order_capacity);

  DLOG(INFO) << "ForemanDistributed sent ShiftbossRegistrationResponseMessage to Shiftboss with Client "
             << shiftboss_client_id;
  const MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         shiftboss_client_id,
                                         move(message));
  CHECK(send_status == MessageBus::SendStatus::kOK);
}

}  // namespace quickstep
