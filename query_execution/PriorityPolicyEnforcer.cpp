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

#include "query_execution/PriorityPolicyEnforcer.hpp"

#include <cstddef>
#include <memory>
#include <queue>
#include <utility>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/Learner.hpp"
#include "query_execution/ProbabilityStore.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryManager.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "relational_operators/WorkOrder.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

DEFINE_uint64(max_msgs_per_dispatch_round, 20, "Maximum number of messages that"
              " can be allocated in a single round of dispatch of messages to"
              " the workers.");

bool PriorityPolicyEnforcer::admitQuery(QueryHandle *query_handle) {
  if (admitted_queries_.size() < kMaxConcurrentQueries) {
    // Ok to admit the query.
    const std::size_t query_id = query_handle->query_id();
    if (admitted_queries_.find(query_id) == admitted_queries_.end()) {
      // Query with the same ID not present, ok to admit.
      admitted_queries_[query_id].reset(
          new QueryManager(foreman_client_id_, num_numa_nodes_, query_handle,
                           catalog_database_, storage_manager_, bus_));
      LOG(INFO) << "Admitted query with ID: " << query_handle->query_id() << " priority: " << query_handle->query_priority();
      priority_query_ids_[query_handle->query_priority()].emplace_back(query_id);
      learner_->addQuery(*query_handle);
      query_handle->setAdmissionTime();
      query_id_to_handle_[query_handle->query_id()] = query_handle;
      return true;
    } else {
      LOG(ERROR) << "Query with the same ID " << query_id << " exists";
      return false;
    }
  } else {
    // This query will have to wait.
    LOG(INFO) << "Query " << query_handle->query_id() << " waitlisted";
    query_id_to_handle_[query_handle->query_id()] = query_handle;
    waiting_queries_.push(query_handle);
    return false;
  }
}

void PriorityPolicyEnforcer::processMessage(const TaggedMessage &tagged_message) {
  // TODO(harshad) : Provide processXMessage() public functions in
  // QueryManager, so that we need to extract message from the
  // TaggedMessage only once.
  std::size_t query_id;
  std::size_t operator_id;
  switch (tagged_message.message_type()) {
    case kWorkOrderCompleteMessage: {
      serialization::NormalWorkOrderCompletionMessage proto;
      // Note: This proto message contains the time it took to execute the
      // WorkOrder. It can be accessed in this scope.
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      operator_id = proto.operator_index();
      worker_directory_->decrementNumQueuedWorkOrders(
          proto.worker_thread_index());
      learner_->addCompletionFeedback(proto);
      if (profile_individual_workorders_) {
        recordTimeForWorkOrder(proto);
      }
      break;
    }
    case kRebuildWorkOrderCompleteMessage: {
      serialization::RebuildWorkOrderCompletionMessage proto;
      // Note: This proto message contains the time it took to execute the
      // rebuild WorkOrder. It can be accessed in this scope.
      CHECK(proto.ParseFromArray(tagged_message.message(),
                                 tagged_message.message_bytes()));
      query_id = proto.query_id();
      operator_id = proto.operator_index();
      worker_directory_->decrementNumQueuedWorkOrders(
          proto.worker_thread_index());
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
      WorkOrder::FeedbackMessage msg(
          const_cast<void *>(tagged_message.message()),
          tagged_message.message_bytes());
      query_id = msg.header().query_id;
      break;
    }
    default:
      LOG(FATAL) << "Unknown message type found in PriorityPolicyEnforcer";
  }
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
  const QueryManager::QueryStatusCode return_code =
      admitted_queries_[query_id]->processMessage(tagged_message);
  //NOTE: kQueryExecuted takes precedence over kOperatorExecuted.
  if (return_code == QueryManager::QueryStatusCode::kQueryExecuted) {
    removeQuery(query_id);
    if (!waiting_queries_.empty()) {
      // Admit the earliest waiting query.
      QueryHandle *new_query = waiting_queries_.front();
      waiting_queries_.pop();
      admitQuery(new_query);
    }
  } else if (return_code == QueryManager::QueryStatusCode::kOperatorExecuted) {
    learner_->removeOperator(query_id, operator_id);
  }
}

void PriorityPolicyEnforcer::getWorkerMessages(
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
  std::unordered_map<std::size_t, bool> finished_queries_ids;

  if (learner_->hasActiveQueries()) {
    // Key = priority level. Value = Whether we have already checked the
    std::unordered_map<std::size_t, bool> checked_priority_levels;
    // While there are more priority levels to be checked ..
    while (checked_priority_levels.size() < priority_query_ids_.size()) {
      const int chosen_priority_level = learner_->pickRandomPriorityLevel();
      if (chosen_priority_level == kInvalidPriorityLevel) {
        LOG(INFO) << "No valid priority level chosen";
        break;
      } else if (checked_priority_levels.find(static_cast<std::size_t>(
                     chosen_priority_level)) != checked_priority_levels.end()) {
        DLOG(INFO) << "The chosen priority level " << chosen_priority_level << " was checked already";
        continue;
      } else {
        WorkerMessage *next_worker_message =
            getNextWorkerMessageFromPriorityLevel(chosen_priority_level,
                                                  &finished_queries_ids);
        if (next_worker_message != nullptr) {
          worker_messages->push_back(std::unique_ptr<WorkerMessage>(next_worker_message));
        } else {
          checked_priority_levels[static_cast<std::size_t>(chosen_priority_level)] = true;
        }
      }
    }
  } else {
    DLOG(INFO) << "No active queries in the learner at this point.";
    return;
  }
  for (auto finished_qid_pair : finished_queries_ids) {
    removeQuery(finished_qid_pair.first);
  }
}

void PriorityPolicyEnforcer::removeQuery(const std::size_t query_id) {
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
  if (!admitted_queries_[query_id]->getQueryExecutionState().hasQueryExecutionFinished()) {
    LOG(WARNING) << "Removing query with ID " << query_id
                 << " that hasn't finished its execution";
  }
  admitted_queries_.erase(query_id);
  // Remove the query from priority_query_ids_ structure.
  const int query_priority = learner_->getQueryPriority(query_id);
  DCHECK(query_priority != kInvalidPriorityLevel);
  const std::size_t query_priority_unsigned =
      static_cast<std::size_t>(query_priority);
  std::vector<std::size_t> *query_ids_for_priority_level =
      &priority_query_ids_[query_priority_unsigned];
  query_ids_for_priority_level->erase(
      std::remove(query_ids_for_priority_level->begin(),
                  query_ids_for_priority_level->end(),
                  query_id));
  if (query_ids_for_priority_level->empty()) {
    // No more queries for the given priority level. Remove the entry.
    priority_query_ids_.erase(query_priority_unsigned);
  }
  query_id_to_handle_[query_id]->setCompletionTime();
  // Remove the query from the learner.
  learner_->removeQuery(query_id);
  // TODO(harshad) - Admit waiting queries, if any.
  LOG(INFO) << "Removed query: " << query_id << " with priority: " << query_priority;
}

bool PriorityPolicyEnforcer::admitQueries(
    const std::vector<QueryHandle*> &query_handles) {
  for (QueryHandle *curr_query : query_handles) {
    if (!admitQuery(curr_query)) {
      return false;
    }
  }
  return true;
}

void PriorityPolicyEnforcer::recordTimeForWorkOrder(
    const serialization::NormalWorkOrderCompletionMessage &proto) {
  const std::size_t query_id = proto.query_id();
  if (workorder_time_recorder_.find(query_id) == workorder_time_recorder_.end()) {
    workorder_time_recorder_[query_id];
  }
  workorder_time_recorder_[query_id].emplace_back(
      proto.worker_thread_index(),
      proto.operator_index(),
      proto.execution_time_in_microseconds());
}

WorkerMessage* PriorityPolicyEnforcer::getNextWorkerMessageFromPriorityLevel(
    const std::size_t priority_level,
    std::unordered_map<std::size_t, bool> *finished_queries_ids) {
  // Key = query ID from the given priority level, value = whether we have
  // checked this query earlier.
  std::unordered_map<std::size_t, bool> checked_query_ids;
  // While there are more queries to be checked ..
  while (checked_query_ids.size() < priority_query_ids_[priority_level].size()) {
    const int chosen_query_id = learner_->pickRandomQueryFromPriorityLevel(priority_level);
    if (chosen_query_id == kInvalidQueryID) {
      // No query available at this time in this priority level.
      return nullptr;
    } else if (checked_query_ids.find(static_cast<std::size_t>(chosen_query_id)) != checked_query_ids.end()) {
      // Find a query from the same priority level, but not present in the
      // checked_query_ids map.
      for (const std::size_t qid : priority_query_ids_[priority_level]) {
        if (checked_query_ids.find(qid) == checked_query_ids.end() &&
            finished_queries_ids->find(qid) == finished_queries_ids->end()) {
          // Query not seen already.
          QueryManager *chosen_query_manager = admitted_queries_[static_cast<std::size_t>(qid)].get();
          DCHECK(chosen_query_manager != nullptr);
          std::unique_ptr<WorkerMessage> next_worker_message(
              chosen_query_manager->getNextWorkerMessage(0, kAnyNUMANodeID));
          if (next_worker_message != nullptr) {
            // LOG(INFO) << "Selecting a work order from query " << qid << " instead";
            return next_worker_message.release();
          } else {
            // This query doesn't have any WorkerMessage right now. Mark as checked.
            checked_query_ids[qid] = true;
            if (chosen_query_manager->getQueryExecutionState().hasQueryExecutionFinished()) {
              (*finished_queries_ids)[static_cast<std::size_t>(qid)] = true;
            }
          }
        }
      }
    } else {
      // We haven't seen this query earlier. Check if it has any schedulable
      // WorkOrder.
      QueryManager *chosen_query_manager = admitted_queries_[static_cast<std::size_t>(chosen_query_id)].get();
      DCHECK(chosen_query_manager != nullptr);
      std::unique_ptr<WorkerMessage> next_worker_message(chosen_query_manager->getNextWorkerMessage(0, kAnyNUMANodeID));
      if (next_worker_message != nullptr) {
        return next_worker_message.release();
      } else {
        // This query doesn't have any WorkerMessage right now. Mark as checked.
        checked_query_ids[chosen_query_id] = true;
        if (chosen_query_manager->getQueryExecutionState().hasQueryExecutionFinished()) {
          (*finished_queries_ids)[static_cast<std::size_t>(chosen_query_id)] = true;
        }
      }
    }
  }
  return nullptr;
}

}  // namespace quickstep
