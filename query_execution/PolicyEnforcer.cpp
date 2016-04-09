/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_execution/PolicyEnforcer.hpp"

#include <cstddef>
#include <memory>
#include <queue>
#include <utility>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryManager.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "relational_operators/WorkOrder.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

DEFINE_uint64(max_msgs_per_dispatch_round, 20, "Maximum number of messages that"
              " can be allocated in a single round of dispatch of messages to"
              " the workers.");

bool PolicyEnforcer::admitQuery(QueryHandle *query_handle) {
  if (admitted_queries_.size() < kMaxConcurrentQueries) {
    // Ok to admit the query.
    const std::size_t query_id = query_handle->query_id();
    if (admitted_queries_.find(query_id) == admitted_queries_.end()) {
      // Query with the same ID not present, ok to admit.
      admitted_queries_[query_id].reset(
          new QueryManager(foreman_client_id_, num_numa_nodes_, query_handle,
                           catalog_database_, storage_manager_, bus_));
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

void PolicyEnforcer::processMessage(const TaggedMessage &tagged_message) {
  // TODO(harshad) : Provide processXMessage() public functions in
  // QueryManager, so that we need to extract message from the
  // TaggedMessage only once.
  std::size_t query_id;
  switch (tagged_message.message_type()) {
    case kWorkOrderCompleteMessage:  // Fall through.
    case kRebuildWorkOrderCompleteMessage: {
      serialization::WorkOrderCompletionMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      break;
    }
    case kCatalogRelationNewBlockMessage: {
      serialization::CatalogRelationNewBlockMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      break;
    }
    case kDataPipelineMessage: {
      serialization::DataPipelineMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      break;
    }
    case kWorkOrdersAvailableMessage: {
      serialization::WorkOrdersAvailableMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      break;
    }
    case kWorkOrderFeedbackMessage: {
      WorkOrder::FeedbackMessage msg(const_cast<void *>(tagged_message.message()), tagged_message.message_bytes());
      query_id = msg.header().query_id;
      break;
    }
    default:
      LOG(FATAL) << "Unknown message type found in PolicyEnforcer";
  }
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
  const QueryManager::QueryStatusCode return_code =
      admitted_queries_[query_id]->processMessage(tagged_message);
  if (return_code == QueryManager::QueryStatusCode::kQueryExecuted) {
    removeQuery(query_id);
    if (!waiting_queries_.empty()) {
      // Admit the earliest waiting query.
      QueryHandle *new_query = waiting_queries_.front();
      waiting_queries_.pop();
      admitQuery(new_query);
    }
  }
}

void PolicyEnforcer::getWorkerMessages(
    std::vector<std::unique_ptr<WorkerMessage>> *worker_messages) {
  // Iterate over admitted queries until either there are no more
  // messages available, or the maximum number of messages have
  // been collected.
  DCHECK(worker_messages->empty());
  // TODO(harshad) - Make this function generic enough so that it
  // works well when multiple queries are getting executed.
  std::size_t per_query_share = 0;
  if (!admitted_queries_.empty()) {
    per_query_share = FLAGS_max_msgs_per_dispatch_round / admitted_queries_.size();
  } else {
    LOG(WARNING) << "Requesting WorkerMessages when no query is running";
    return;
  }
  DCHECK_GT(per_query_share, 0u);
  std::vector<std::size_t> finished_queries_ids;

  for (const auto &admitted_query_info : admitted_queries_) {
    QueryManager *curr_query_manager = admitted_query_info.second.get();
    DCHECK(curr_query_manager != nullptr);
    std::size_t messages_collected_curr_query = 0;
    while (messages_collected_curr_query < per_query_share) {
      WorkerMessage *next_worker_message =
          curr_query_manager->getNextWorkerMessage(0, kAnyNUMANodeID);
      if (next_worker_message != nullptr) {
        ++messages_collected_curr_query;
        worker_messages->push_back(std::unique_ptr<WorkerMessage>(next_worker_message));
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

void PolicyEnforcer::removeQuery(const std::size_t query_id) {
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
  if (!admitted_queries_[query_id]->getQueryExecutionState().hasQueryExecutionFinished()) {
    LOG(WARNING) << "Removing query with ID " << query_id
                 << " that hasn't finished its execution";
  }
  admitted_queries_.erase(query_id);
}

bool PolicyEnforcer::admitQueries(
    const std::vector<QueryHandle*> &query_handles) {
  for (QueryHandle *curr_query : query_handles) {
    if (!admitQuery(curr_query)) {
      return false;
    }
  }
  return true;
}

}  // namespace quickstep
