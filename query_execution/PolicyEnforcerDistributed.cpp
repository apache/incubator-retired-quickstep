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
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/QueryManagerBase.hpp"
#include "query_execution/QueryManagerDistributed.hpp"
#include "query_optimizer/QueryHandle.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

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

DEFINE_uint64(max_msgs_per_dispatch_round, 20, "Maximum number of messages that "
              "can be allocated in a single round of dispatch of messages to "
              "Shiftbosses.");

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
    removeQuery(finished_qid);
  }
}

bool PolicyEnforcerDistributed::admitQuery(QueryHandle *query_handle) {
  if (admitted_queries_.size() >= PolicyEnforcerBase::kMaxConcurrentQueries) {
    // This query will have to wait.
    waiting_queries_.push(query_handle);
    return false;
  }

  // Ok to admit the query.
  const std::size_t query_id = query_handle->query_id();
  if (admitted_queries_.find(query_id) != admitted_queries_.end()) {
    LOG(ERROR) << "Query with the same ID " << query_id << " exists";
    return false;
  }

  // NOTE(zuyu): Should call before constructing a 'QueryManager'.
  // Otherwise, an InitiateRebuildMessage may be sent before 'QueryContext'
  // initializes.
  initiateQueryInShiftboss(query_handle);

  // Query with the same ID not present, ok to admit.
  admitted_queries_[query_id].reset(
      new QueryManagerDistributed(query_handle, shiftboss_directory_, foreman_client_id_, bus_));
  return true;
}

void PolicyEnforcerDistributed::initiateQueryInShiftboss(QueryHandle *query_handle) {
  serialization::QueryInitiateMessage proto;
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

  LOG(INFO) << "ForemanDistributed sent QueryInitiateMessage (typed '" << kQueryInitiateMessage
            << "') to Shiftboss 0";

  // TODO(zuyu): Multiple Shiftbosses support.
  QueryExecutionUtil::SendTMBMessage(bus_,
                                     foreman_client_id_,
                                     shiftboss_directory_->getClientId(0),
                                     move(message));

  // Wait Shiftboss for QueryInitiateResponseMessage.
  const tmb::AnnotatedMessage annotated_message = bus_->Receive(foreman_client_id_, 0, true);
  const TaggedMessage &tagged_message = annotated_message.tagged_message;
  DCHECK_EQ(kQueryInitiateResponseMessage, tagged_message.message_type());
  LOG(INFO) << "ForemanDistributed received typed '" << tagged_message.message_type()
            << "' message from client " << annotated_message.sender;

  serialization::QueryInitiateResponseMessage proto_response;
  CHECK(proto_response.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));
}

}  // namespace quickstep
