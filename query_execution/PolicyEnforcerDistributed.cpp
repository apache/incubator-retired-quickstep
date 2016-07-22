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

#include "query_execution/PolicyEnforcerDistributed.hpp"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <queue>
#include <utility>
#include <unordered_map>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/QueryManagerBase.hpp"
#include "query_execution/QueryManagerDistributed.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;
using std::size_t;
using std::unique_ptr;
using std::vector;

using tmb::TaggedMessage;

namespace quickstep {

namespace S = serialization;

DEFINE_uint64(max_msgs_per_dispatch_round, 20, "Maximum number of messages that"
              " can be allocated in a single round of dispatch of messages to"
              " the workers.");

void PolicyEnforcerDistributed::getWorkOrderMessages(
    vector<unique_ptr<S::WorkOrderMessage>> *work_order_messages) {
  // Iterate over admitted queries until either there are no more
  // messages available, or the maximum number of messages have
  // been collected.
  DCHECK(work_order_messages->empty());
  // TODO(harshad) - Make this function generic enough so that it
  // works well when multiple queries are getting executed.
  if (admitted_queries_.empty()) {
    LOG(WARNING) << "Requesting WorkerMessages when no query is running";
    return;
  }

  const std::size_t per_query_share =
      FLAGS_max_msgs_per_dispatch_round / admitted_queries_.size();
  DCHECK_GT(per_query_share, 0u);

  vector<std::size_t> finished_queries_ids;

  for (const auto &admitted_query_info : admitted_queries_) {
    QueryManagerBase *curr_query_manager = admitted_query_info.second.get();
    DCHECK(curr_query_manager != nullptr);
    std::size_t messages_collected_curr_query = 0;
    while (messages_collected_curr_query < per_query_share) {
      S::WorkOrderMessage *next_work_order_message =
          static_cast<QueryManagerDistributed*>(curr_query_manager)->getNextWorkOrderMessage(0);
      if (next_work_order_message != nullptr) {
        ++messages_collected_curr_query;
        work_order_messages->push_back(unique_ptr<S::WorkOrderMessage>(next_work_order_message));
      } else {
        // No more work ordes from the current query at this time.
        // Check if the query's execution is over.
        if (curr_query_manager->getQueryExecutionState().hasQueryExecutionFinished()) {
          // If the query has been executed, remove it.
          finished_queries_ids.push_back(admitted_query_info.first);
        }
        break;
      }
    }
  }
  for (const std::size_t finished_qid : finished_queries_ids) {
    onQueryCompletion(admitted_queries_[finished_qid]->query_handle());
    removeQuery(finished_qid);
  }
}

bool PolicyEnforcerDistributed::admitQuery(QueryHandle *query_handle) {
  if (admitted_queries_.size() < PolicyEnforcerBase::kMaxConcurrentQueries) {
    // Ok to admit the query.
    const std::size_t query_id = query_handle->query_id();
    if (admitted_queries_.find(query_id) == admitted_queries_.end()) {
      // NOTE(zuyu): Should call before constructing a 'QueryManager'.
      // Otherwise, an InitiateRebuildMessage may be sent before 'QueryContext'
      // initializes.
      initiateQueryInShiftboss(query_handle);

      // Query with the same ID not present, ok to admit.
      admitted_queries_[query_id].reset(
          new QueryManagerDistributed(query_handle, shiftboss_directory_, foreman_client_id_, bus_));
      return true;
    } else {
      LOG(ERROR) << "Query with the same ID " << query_id << " exists";
      return false;
    }
  } else {
    // This query will have to wait.
    waiting_queries_.push(query_handle);
    return false;
  }
}

void PolicyEnforcerDistributed::processInitiateRebuildResponseMessage(const tmb::TaggedMessage &tagged_message) {
  S::InitiateRebuildResponseMessage proto;
  CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

  const std::size_t query_id = proto.query_id();
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());

  QueryManagerBase *query_manager = admitted_queries_[query_id].get();

  const std::size_t num_rebuild_work_orders = proto.num_rebuild_work_orders();
  query_manager->processInitiateRebuildResponseMessage(proto.operator_index(), num_rebuild_work_orders);
  shiftboss_directory_->addNumQueuedWorkOrders(proto.shiftboss_index(), num_rebuild_work_orders);

  if (query_manager->getQueryExecutionState().hasQueryExecutionFinished()) {
    onQueryCompletion(query_manager->query_handle());

    removeQuery(query_id);
    if (!waiting_queries_.empty()) {
      // Admit the earliest waiting query.
      QueryHandle *new_query = waiting_queries_.front();
      waiting_queries_.pop();
      admitQuery(new_query);
    }
  }
}

void PolicyEnforcerDistributed::initiateQueryInShiftboss(QueryHandle *query_handle) {
  S::QueryInitiateMessage proto;
  proto.set_query_id(query_handle->query_id());
  proto.mutable_catalog_database_cache()->MergeFrom(query_handle->getCatalogDatabaseCacheProto());
  proto.mutable_query_context()->MergeFrom(query_handle->getQueryContextProto());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kQueryInitiateMessage);
  free(proto_bytes);

  LOG(INFO) << "PolicyEnforcerDistributed sent QueryInitiateMessage (typed '" << kQueryInitiateMessage
            << "') to Shiftboss 0";

  // TODO(zuyu): Multiple Shiftbosses support.
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         shiftboss_directory_->getClientId(0),
                                         move(message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
      << "Message could not be sent from Foreman with TMB client ID " << foreman_client_id_
      << " to Shiftboss with TMB client ID " << shiftboss_directory_->getClientId(0);

  // Wait Shiftboss for QueryInitiateResponseMessage.
  const tmb::AnnotatedMessage annotated_message = bus_->Receive(foreman_client_id_, 0, true);
  const TaggedMessage &tagged_message = annotated_message.tagged_message;
  DCHECK_EQ(kQueryInitiateResponseMessage, tagged_message.message_type());
  LOG(INFO) << "PolicyEnforcerDistributed received typed '" << tagged_message.message_type()
            << "' message from client " << annotated_message.sender;

  S::QueryInitiateResponseMessage proto_response;
  CHECK(proto_response.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));
}

void PolicyEnforcerDistributed::onQueryCompletion(QueryHandle *query_handle) {
  const CatalogRelation *query_result = query_handle->getQueryResultRelation();
  if (query_result == nullptr) {
    // TODO(zuyu): notify Shiftboss to remove QueryContext.
    TaggedMessage message(kQueryExecutionSuccessMessage);

    const tmb::client_id cli_id = query_handle->getClientId();

    // Notify the CLI regarding the query execution result.
    LOG(INFO) << "PolicyEnforcerDistributed sent QueryExecutionSuccessMessage (typed '" << kQueryExecutionSuccessMessage
              << "') to CLI with TMB client id " << cli_id;
    const tmb::MessageBus::SendStatus send_status =
        QueryExecutionUtil::SendTMBMessage(bus_,
                                           foreman_client_id_,
                                           cli_id,
                                           move(message));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
        << "Message could not be sent from Foreman with TMB client ID " << foreman_client_id_
        << " to CLI with TMB client ID " << cli_id;
    return;
  }

  // SaveQueryResultMessage implies QueryContext clean up in Shiftboss.
  S::SaveQueryResultMessage proto;
  proto.set_query_id(query_handle->query_id());
  proto.set_relation_id(query_result->getID());

  const vector<block_id> blocks(query_result->getBlocksSnapshot());
  for (const block_id block : blocks) {
    proto.add_blocks(block);
  }

  proto.set_cli_id(query_handle->getClientId());

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kSaveQueryResultMessage);
  free(proto_bytes);

  LOG(INFO) << "PolicyEnforcerDistributed sent SaveQueryResultMessage (typed '" << kSaveQueryResultMessage
            << "') to Shiftboss 0";
  // TODO(zuyu): Support multiple shiftbosses.
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         foreman_client_id_,
                                         shiftboss_directory_->getClientId(0),
                                         move(message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK)
      << "Message could not be sent from Foreman with TMB client ID " << foreman_client_id_
      << " to Shiftboss with TMB client ID " << shiftboss_directory_->getClientId(0);
}

}  // namespace quickstep
