/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
#include "query_execution/AdmitRequestMessage.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/PolicyEnforcerBase.hpp"
#include "query_execution/PolicyEnforcerDistributed.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/ShiftbossDirectory.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "threading/ThreadUtil.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

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
    const BlockLocator::BlockDomainToShiftbossIndex &block_domain_to_shiftboss_index,
    const BlockLocator::BlockLocation &block_locations,
    MessageBus *bus,
    CatalogDatabaseLite *catalog_database,
    const int cpu_id)
    : ForemanBase(bus, cpu_id),
      block_domain_to_shiftboss_index_(block_domain_to_shiftboss_index),
      block_locations_(block_locations),
      catalog_database_(DCHECK_NOTNULL(catalog_database)) {
  const std::vector<QueryExecutionMessageType> sender_message_types{
      kShiftbossRegistrationResponseMessage,
      kQueryInitiateMessage,
      kWorkOrderMessage,
      kInitiateRebuildMessage,
      kQueryTeardownMessage,
      kSaveQueryResultMessage,
      kQueryExecutionSuccessMessage,
      kPoisonMessage};

  for (const auto message_type : sender_message_types) {
    bus_->RegisterClientAsSender(foreman_client_id_, message_type);
  }

  const std::vector<QueryExecutionMessageType> receiver_message_types{
      kShiftbossRegistrationMessage,
      kAdmitRequestMessage,
      kQueryInitiateResponseMessage,
      kCatalogRelationNewBlockMessage,
      kDataPipelineMessage,
      kInitiateRebuildResponseMessage,
      kWorkOrderCompleteMessage,
      kRebuildWorkOrderCompleteMessage,
      kWorkOrderFeedbackMessage,
      kSaveQueryResultResponseMessage,
      kPoisonMessage};

  for (const auto message_type : receiver_message_types) {
    bus_->RegisterClientAsReceiver(foreman_client_id_, message_type);
  }

  policy_enforcer_ = std::make_unique<PolicyEnforcerDistributed>(
      foreman_client_id_,
      catalog_database_,
      &shiftboss_directory_,
      bus_);
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
    DLOG(INFO) << "ForemanDistributed received typed '" << tagged_message.message_type()
               << "' message from client " << annotated_message.sender;

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
    DLOG(INFO) << "ForemanDistributed received typed '" << message_type
               << "' message from client " << annotated_message.sender;
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
        CHECK(policy_enforcer_->existQuery(proto.query_id()));
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
      case kSaveQueryResultResponseMessage: {
        S::SaveQueryResultResponseMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const std::size_t query_id = proto.query_id();
        query_result_saved_shiftbosses_[query_id].insert(proto.shiftboss_index());

        // TODO(quickstep-team): Dynamically scale-up/down Shiftbosses.
        if (query_result_saved_shiftbosses_[query_id].size() == shiftboss_directory_.size()) {
          processSaveQueryResultResponseMessage(proto.cli_id(), proto.relation_id());
          query_result_saved_shiftbosses_.erase(query_id);
        }
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

  switch (work_order_proto.work_order_type()) {
    case S::BUILD_HASH:
      join_hash_table_index = work_order_proto.GetExtension(S::BuildHashWorkOrder::join_hash_table_index);
      break;
    case S::HASH_JOIN:
      join_hash_table_index = work_order_proto.GetExtension(S::HashJoinWorkOrder::join_hash_table_index);
      break;
    case S::DESTROY_HASH:
      join_hash_table_index = work_order_proto.GetExtension(S::DestroyHashWorkOrder::join_hash_table_index);
      break;
    default:
      return false;
  }

  static_cast<PolicyEnforcerDistributed*>(policy_enforcer_.get())->getShiftbossIndexForHashJoin(
      proto.query_id(), join_hash_table_index, next_shiftboss_index_to_schedule, shiftboss_index_for_hash_join);

  return true;
}

bool ForemanDistributed::isNestedLoopsJoinWorkOrder(const serialization::WorkOrder &work_order_proto,
                                                    std::size_t *shiftboss_index_for_join) {
  if (work_order_proto.work_order_type() != S::NESTED_LOOP_JOIN) {
    return false;
  }

  const block_id left_block = work_order_proto.GetExtension(S::NestedLoopsJoinWorkOrder::left_block_id);
  if (hasBlockLocalityInfoHelper(left_block, shiftboss_index_for_join)) {
    return true;
  }

  const block_id right_block = work_order_proto.GetExtension(S::NestedLoopsJoinWorkOrder::right_block_id);
  return hasBlockLocalityInfoHelper(right_block, shiftboss_index_for_join);
}

bool ForemanDistributed::hasBlockLocalityInfo(const S::WorkOrder &work_order_proto,
                                              size_t *shiftboss_index_for_block) {
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
  return hasBlockLocalityInfoHelper(block, shiftboss_index_for_block);
}

bool ForemanDistributed::hasBlockLocalityInfoHelper(const block_id block,
                                                    size_t *shiftboss_index_for_block) {
  const auto cit = block_locations_.find(block);
  if (cit != block_locations_.end()) {
    for (const block_id_domain block_domain : cit->second) {
      // TODO(quickstep-team): choose the best node, instead of the first.
      const auto cit_shiftboss_index = block_domain_to_shiftboss_index_.find(block_domain);
      if (cit_shiftboss_index != block_domain_to_shiftboss_index_.end()) {
        *shiftboss_index_for_block = cit_shiftboss_index->second;
        return true;
      }
    }
  }

  return false;
}

namespace {
constexpr size_t kDefaultShiftbossIndex = 0u;
}  // namespace

void ForemanDistributed::dispatchWorkOrderMessages(const vector<unique_ptr<S::WorkOrderMessage>> &messages) {
  static size_t shiftboss_index = kDefaultShiftbossIndex;

  PolicyEnforcerDistributed* policy_enforcer_dist = static_cast<PolicyEnforcerDistributed*>(policy_enforcer_.get());
  for (const auto &message : messages) {
    DCHECK(message != nullptr);
    const S::WorkOrderMessage &proto = *message;
    size_t shiftboss_index_for_particular_work_order_type;
    if (policy_enforcer_dist->isSingleNodeQuery(proto.query_id())) {
      // Always schedule the single-node query to the same Shiftboss.
      shiftboss_index_for_particular_work_order_type = kDefaultShiftbossIndex;
    } else if (isAggregationRelatedWorkOrder(proto, shiftboss_index, &shiftboss_index_for_particular_work_order_type)) {
    } else if (isHashJoinRelatedWorkOrder(proto, shiftboss_index, &shiftboss_index_for_particular_work_order_type)) {
    } else if (hasBlockLocalityInfo(proto.work_order(), &shiftboss_index_for_particular_work_order_type)) {
    } else if (isNestedLoopsJoinWorkOrder(proto.work_order(), &shiftboss_index_for_particular_work_order_type)) {
    } else {
      shiftboss_index_for_particular_work_order_type = shiftboss_index;
    }

    sendWorkOrderMessage(shiftboss_index_for_particular_work_order_type, proto);
    shiftboss_directory_.incrementNumQueuedWorkOrders(shiftboss_index_for_particular_work_order_type);

    if (shiftboss_index == shiftboss_index_for_particular_work_order_type) {
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
  DLOG(INFO) << "ForemanDistributed sent WorkOrderMessage (typed '" << kWorkOrderMessage
             << "') to Shiftboss with TMB client ID " << shiftboss_client_id;
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

  DLOG(INFO) << "ForemanDistributed sent ShiftbossRegistrationResponseMessage (typed '"
             << kShiftbossRegistrationResponseMessage
             << "') to Shiftboss with TMB client id " << shiftboss_client_id;
  const MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         shiftboss_client_id,
                                         move(message));
  CHECK(send_status == MessageBus::SendStatus::kOK);
}

void ForemanDistributed::processSaveQueryResultResponseMessage(const client_id cli_id,
                                                               const relation_id result_relation_id) {
  S::QueryExecutionSuccessMessage proto;
  proto.mutable_result_relation()->MergeFrom(
      static_cast<CatalogDatabase*>(catalog_database_)->getRelationById(result_relation_id)->getProto());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kQueryExecutionSuccessMessage);
  free(proto_bytes);

  // Notify the CLI regarding the query result.
  DLOG(INFO) << "ForemanDistributed sent QueryExecutionSuccessMessage (typed '"
             << kQueryExecutionSuccessMessage
             << "') to CLI with TMB client id " << cli_id;
  const MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         cli_id,
                                         move(message));
  CHECK(send_status == MessageBus::SendStatus::kOK);
}

}  // namespace quickstep
