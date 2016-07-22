/**
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

#include "query_execution/Shiftboss.hpp"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "relational_operators/RebuildWorkOrder.hpp"
#include "relational_operators/WorkOrderFactory.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadUtil.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;
using std::size_t;
using std::string;
using std::unique_ptr;
using std::vector;

using tmb::TaggedMessage;

namespace quickstep {

class WorkOrder;

void Shiftboss::run() {
  if (cpu_id_ >= 0) {
    // We can pin the shiftboss thread to a CPU if specified.
    ThreadUtil::BindToCPU(cpu_id_);
  }

  for (;;) {
    // Receive() is a blocking call, causing this thread to sleep until next
    // message is received.
    AnnotatedMessage annotated_message(bus_->Receive(shiftboss_client_id_, 0, true));
    LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
              << "') received the typed '" << annotated_message.tagged_message.message_type()
              << "' message from client " << annotated_message.sender;
    switch (annotated_message.tagged_message.message_type()) {
      case kShiftbossRegistrationResponseMessage: {
        foreman_client_id_ = annotated_message.sender;
        break;
      }
      case kQueryInitiateMessage: {
        const TaggedMessage &tagged_message = annotated_message.tagged_message;

        serialization::QueryInitiateMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processQueryInitiateMessage(proto.query_id(), proto.catalog_database_cache(), proto.query_context());
        break;
      }
      case kWorkOrderMessage: {
        const TaggedMessage &tagged_message = annotated_message.tagged_message;

        serialization::WorkOrderMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const std::size_t query_id = proto.query_id();
        DCHECK_EQ(1u, query_contexts_.count(query_id));

        WorkOrder *work_order = WorkOrderFactory::ReconstructFromProto(proto.work_order(),
                                                                       &database_cache_,
                                                                       query_contexts_[query_id].get(),
                                                                       storage_manager_,
                                                                       shiftboss_client_id_,
                                                                       bus_);

        unique_ptr<WorkerMessage> worker_message(
            WorkerMessage::WorkOrderMessage(work_order, proto.operator_index()));

        TaggedMessage worker_tagged_message(worker_message.get(),
                                            sizeof(*worker_message),
                                            kWorkOrderMessage);

        const size_t worker_index = getSchedulableWorker();
        LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
                  << "') forwarded WorkOrderMessage (typed '" << kWorkOrderMessage
                  << "') from Foreman to worker " << worker_index;

        QueryExecutionUtil::SendTMBMessage(bus_,
                                           shiftboss_client_id_,
                                           workers_->getClientID(worker_index),
                                           move(worker_tagged_message));
        break;
      }
      case kInitiateRebuildMessage: {
        // Construct rebuild work orders, and send back their number to
        // 'ForemanDistributed'.
        const TaggedMessage &tagged_message = annotated_message.tagged_message;

        serialization::InitiateRebuildMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processInitiateRebuildMessage(proto.query_id(),
                                      proto.operator_index(),
                                      proto.insert_destination_index(),
                                      proto.relation_id());
        break;
      }
      case kWorkOrderCompleteMessage:  // Fall through.
      case kRebuildWorkOrderCompleteMessage:
      case kDataPipelineMessage:
      case kWorkOrdersAvailableMessage:
      case kWorkOrderFeedbackMessage: {
        LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
                  << "') forwarded typed '" << annotated_message.tagged_message.message_type()
                  << "' message from worker (client " << annotated_message.sender << ") to Foreman";

        DCHECK_NE(foreman_client_id_, tmb::kClientIdNone);
        QueryExecutionUtil::SendTMBMessage(bus_,
                                           shiftboss_client_id_,
                                           foreman_client_id_,
                                           move(annotated_message.tagged_message));
        break;
      }
      case kQueryResultRelationMessage: {
        // TODO(zuyu): Rename to kSaveQueryResultMessage.
        const TaggedMessage &tagged_message = annotated_message.tagged_message;

        serialization::QueryResultRelationMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        for (int i = 0; i < proto.blocks_size(); ++i) {
          const block_id block = proto.blocks(i);
          storage_manager_->saveBlockOrBlob(block);
          if (storage_manager_->blockOrBlobIsLoaded(block)) {
            // NOTE(zuyu): eviction is required to avoid accesses to the query
            // result relation schema in CatalogDatabaseCache, for all query
            // optimizer execution generator unit tests and the single-process
            // Quickstep CLI.
            storage_manager_->evictBlockOrBlob(block);
          }
        }

        serialization::QueryResultRelationResponseMessage ack_proto;
        ack_proto.set_relation_id(proto.relation_id());

        const size_t ack_proto_length = ack_proto.ByteSize();
        char *ack_proto_bytes = static_cast<char*>(malloc(ack_proto_length));
        CHECK(ack_proto.SerializeToArray(ack_proto_bytes, ack_proto_length));

        TaggedMessage ack_message(static_cast<const void*>(ack_proto_bytes),
                                  ack_proto_length,
                                  kQueryResultRelationResponseMessage);
        free(ack_proto_bytes);

        LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
                  << "') sent QueryResultRelationResponseMessage (typed '" << kQueryResultRelationResponseMessage
                  << ") to Foreman";
        QueryExecutionUtil::SendTMBMessage(bus_,
                                           shiftboss_client_id_,
                                           foreman_client_id_,
                                           move(ack_message));
        break;
      }
      case kPoisonMessage: {
        LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
                  << "') forwarded PoisonMessage (typed '" << kPoisonMessage
                  << "') from Foreman to all workers";

        tmb::MessageStyle broadcast_style;
        broadcast_style.Broadcast(true);

        tmb::MessageBus::SendStatus send_status =
            bus_->Send(shiftboss_client_id_,
                       worker_addresses_,
                       broadcast_style,
                       move(annotated_message.tagged_message));
        DCHECK(send_status == tmb::MessageBus::SendStatus::kOK);
        return;
      }
      default: {
        LOG(FATAL) << "Unknown TMB message type";
      }
    }
  }
}

size_t Shiftboss::getSchedulableWorker() {
  const size_t num_workers = workers_->getNumWorkers();

  size_t curr_worker = start_worker_index_;
  for (;;) {
    if (workers_->getNumQueuedWorkOrders(curr_worker) < max_msgs_per_worker_) {
      start_worker_index_ = (curr_worker + 1) % num_workers;
      // TODO(zuyu): workers_->incrementNumQueuedWorkOrders(curr_worker);
      // But we need a WorkOrder queue first.
      return curr_worker;
    }

    curr_worker = (curr_worker + 1) % num_workers;
  }
}

void Shiftboss::registerWithForeman() {
  LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
            << "') sent ShiftbossRegistrationMessage (typed '" << kShiftbossRegistrationMessage
            << "') to all";

  tmb::Address all_addresses;
  all_addresses.All(true);

  tmb::MessageStyle style;

  serialization::ShiftbossRegistrationMessage proto;
  proto.set_work_order_capacity(getWorkOrderCapacity());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kShiftbossRegistrationMessage);
  free(proto_bytes);

  tmb::MessageBus::SendStatus send_status =
      bus_->Send(shiftboss_client_id_, all_addresses, style, move(message));
  DCHECK(send_status == tmb::MessageBus::SendStatus::kOK);
}

void Shiftboss::processQueryInitiateMessage(
    const std::size_t query_id,
    const serialization::CatalogDatabase &catalog_database_cache_proto,
    const serialization::QueryContext &query_context_proto) {
  database_cache_.update(catalog_database_cache_proto);

  unique_ptr<QueryContext> query_context(
      new QueryContext(query_context_proto,
                       database_cache_,
                       storage_manager_,
                       shiftboss_client_id_,
                       bus_));
  query_contexts_.emplace(query_id, move(query_context));

  LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
            << "') sent QueryInitiateResponseMessage (typed '" << kQueryInitiateResponseMessage
            << "') to Foreman";

  serialization::QueryInitiateResponseMessage proto;
  proto.set_query_id(query_id);

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage ack_message(static_cast<const void*>(proto_bytes),
                            proto_length,
                            kQueryInitiateResponseMessage);
  free(proto_bytes);

  QueryExecutionUtil::SendTMBMessage(bus_,
                                     shiftboss_client_id_,
                                     foreman_client_id_,
                                     move(ack_message));
}

void Shiftboss::processInitiateRebuildMessage(const std::size_t query_id,
                                              const std::size_t op_index,
                                              const QueryContext::insert_destination_id dest_index,
                                              const relation_id rel_id) {
  DCHECK_NE(foreman_client_id_, tmb::kClientIdNone);

  DCHECK_EQ(1u, query_contexts_.count(query_id));
  InsertDestination *insert_destination = query_contexts_[query_id]->getInsertDestination(dest_index);
  DCHECK(insert_destination != nullptr);

  vector<MutableBlockReference> partially_filled_block_refs;
  insert_destination->getPartiallyFilledBlocks(&partially_filled_block_refs);

  LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
            << "') sent InitiateRebuildResponseMessage (typed '" << kInitiateRebuildResponseMessage
            << "') to Foreman";

  serialization::InitiateRebuildResponseMessage proto;
  proto.set_query_id(query_id);
  proto.set_operator_index(op_index);
  proto.set_num_rebuild_work_orders(partially_filled_block_refs.size());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage ack_message(static_cast<const void*>(proto_bytes),
                            proto_length,
                            kInitiateRebuildResponseMessage);
  free(proto_bytes);

  QueryExecutionUtil::SendTMBMessage(bus_,
                                     shiftboss_client_id_,
                                     foreman_client_id_,
                                     move(ack_message));

  for (size_t i = 0; i < partially_filled_block_refs.size(); ++i) {
    // NOTE(zuyu): Worker releases the memory after the execution of
    // RebuildWorkOrder on the Worker.
    WorkOrder *rebuild_work_order =
        new RebuildWorkOrder(query_id,
                             move(partially_filled_block_refs[i]),
                             op_index,
                             rel_id,
                             shiftboss_client_id_,
                             bus_);

    unique_ptr<WorkerMessage> worker_message(
        WorkerMessage::RebuildWorkOrderMessage(rebuild_work_order, op_index));

    TaggedMessage worker_tagged_message(worker_message.get(),
                                        sizeof(*worker_message),
                                        kRebuildWorkOrderMessage);

    const size_t worker_index = getSchedulableWorker();
    LOG(INFO) << "Shiftboss (id '" << shiftboss_client_id_
              << "') sent RebuildWorkOrderMessage (typed '" << kRebuildWorkOrderMessage
              << "') to worker " << worker_index;

    QueryExecutionUtil::SendTMBMessage(bus_,
                                       shiftboss_client_id_,
                                       workers_->getClientID(worker_index),
                                       move(worker_tagged_message));
  }
}

}  // namespace quickstep
