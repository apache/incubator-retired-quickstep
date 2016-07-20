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

#include <algorithm>
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
#include "storage/StorageManager.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

DEFINE_uint64(max_msgs_per_dispatch_round, 40, "Maximum number of messages that"
              " can be allocated in a single round of dispatch of messages to"
              " the workers.");

DEFINE_bool(highest_priority_first, false, "Pick queries from the highest priority level first");

DEFINE_bool(dynamic_probabilities_in_learner, true, "Whether the learner should have dynamic probabilities or static probabilities");

PriorityPolicyEnforcer::PriorityPolicyEnforcer(const tmb::client_id foreman_client_id,
                       const std::size_t num_numa_nodes,
                       CatalogDatabaseLite *catalog_database,
                       StorageManager *storage_manager,
                       WorkerDirectory *worker_directory,
                       tmb::MessageBus *bus,
                       const bool profile_individual_workorders)
    : foreman_client_id_(foreman_client_id),
      num_numa_nodes_(num_numa_nodes),
      catalog_database_(catalog_database),
      storage_manager_(storage_manager),
      worker_directory_(worker_directory),
      bus_(bus),
      profile_individual_workorders_(profile_individual_workorders),
      committed_memory_(0),
      suspended_memory_(0) {
  learner_.reset(new Learner());
}

bool PriorityPolicyEnforcer::admitQuery(QueryHandle *query_handle) {
  // Find a victim query to be suspended.
  bool memory_available = admissionMemoryCheck(query_handle);
  if (!memory_available) {
    while (!memory_available) {
      std::pair<int, std::size_t> victim_query = getQueryWithHighestMemoryFootprint();
      if (victim_query.first != kInvalidQueryID) {
        // We need to suspend this query - move it from admitted to suspended.
        suspendQuery(victim_query.first);
        memory_available = admissionMemoryCheck(query_handle);
      } else {
        std::cout << "No victim found, okay to admit query " << query_handle->query_id() << "\n";
        break;
      }
    }
  }
  // Ok to admit the query.
  const std::size_t query_id = query_handle->query_id();
  if (admitted_queries_.find(query_id) == admitted_queries_.end()) {
    // Query with the same ID not present, ok to admit.
    admitted_queries_[query_id].reset(
        new QueryManager(foreman_client_id_, num_numa_nodes_, query_handle,
                         catalog_database_, storage_manager_, bus_));
    std::cout << "Admitted query with ID: " << query_handle->query_id()
               << " priority: " << query_handle->query_priority() << "\n";
    priority_query_ids_[query_handle->query_priority()].emplace_back(query_id);
    learner_->addQuery(*query_handle);
    query_handle->setAdmissionTime();
    query_id_to_handle_[query_handle->query_id()] = query_handle;
    LOG(INFO) << "Query " << query_handle->query_id() << " mem estimate: " << query_handle->getEstimatedMaxMemoryInBytes() << " bytes";
    return true;
  } else {
    LOG(ERROR) << "Query with the same ID " << query_id << " exists";
    return false;
  }
}

bool PriorityPolicyEnforcer::admitSuspendedQuery(QueryHandle *query_handle) {
 if (admissionMemoryCheck(query_handle)) {
    // Ok to admit the query.
    const std::size_t query_id = query_handle->query_id();
    // As we deducted the current query footprint from the committed memory in
    // suspendQuery() we need to add it back.
    const std::size_t curr_query_footprint = getMemoryForQueryInBytes(query_id);
    suspended_memory_ -= curr_query_footprint;
    if (admitted_queries_.find(query_id) == admitted_queries_.end()) {
      // Query with the same ID not present, ok to admit.
      // Don't create a new QueryManager instance, it has already been created.
      // Just move it from suspended_query_managers_ to admitted_queries_.
      DCHECK(suspended_query_managers_.find(query_id) != suspended_query_managers_.end());
      DCHECK(suspended_query_managers_.at(query_id) != nullptr);
      admitted_queries_[query_id].reset(suspended_query_managers_[query_id].release());
      std::cout << "Admitted suspended query with ID: " << query_handle->query_id()
                 << " priority: " << query_handle->query_priority() << "\n";
      priority_query_ids_[query_handle->query_priority()].emplace_back(query_id);
      learner_->addQuery(*query_handle);
      // query_handle->setAdmissionTime();
      query_id_to_handle_[query_handle->query_id()] = query_handle;
      LOG(INFO) << "Query " << query_handle->query_id() << " mem estimate: " << query_handle->getEstimatedMaxMemoryInBytes() << " bytes";
      return true;
    } else {
      LOG(ERROR) << "Query with the same ID " << query_id << " exists";
      return false;
    }
  } else {
    // Let the query be in the suspended mode.
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
      if (!hasQuerySuspended(query_id)) {
        // Add completion feedback for non-suspended queries.
        if (FLAGS_dynamic_probabilities_in_learner) {
          learner_->addCompletionFeedback<true>(proto);
        } else {
          learner_->addCompletionFeedback<false>(proto);
        }
      }
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
  QueryManager::QueryStatusCode return_code = QueryManager::QueryStatusCode::kNone;
  if (!hasQuerySuspended(query_id)) {
    DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
       return_code = admitted_queries_[query_id]->processMessage(tagged_message);
  } else {
    DCHECK(suspended_query_managers_.find(query_id) != suspended_query_managers_.end());
    return_code = suspended_query_managers_[query_id]->processMessage(tagged_message);
  }
  // NOTE: kQueryExecuted takes precedence over kOperatorExecuted.
  if (return_code == QueryManager::QueryStatusCode::kQueryExecuted) {
    removeQuery(query_id);
    if (!suspended_queries_.empty()) {
      // Admit a suspended query.
      QueryHandle *suspended_query = suspended_queries_.back();
      while (admitSuspendedQuery(suspended_query)) {
        // Until we can admit more suspended queries ...
        std::cout << "Admitting suspended query " << suspended_query->query_id() << " back\n";
        suspended_queries_.pop_back();
        suspended_query_managers_.erase(suspended_query->query_id());
        if (!suspended_queries_.empty()) {
          suspended_query = suspended_queries_.back();
        } else {
          break;
        }
      }
    } else if (!waiting_queries_.empty()) {
      // Admit the earliest waiting query.
      QueryHandle *new_query = waiting_queries_.front();
      // waiting_queries_.pop();
      if (admitQuery(new_query)) {
        std::cout << "Removing Q " << new_query->query_id() << " from waitlist\n";
        waiting_queries_.pop();
      }
    }
  } else if (return_code == QueryManager::QueryStatusCode::kOperatorExecuted) {
    learner_->removeOperator(query_id, operator_id);
  }
}

void PriorityPolicyEnforcer::getWorkerMessages(
    std::vector<std::unique_ptr<WorkerMessage>> *worker_messages) {
  if (!FLAGS_highest_priority_first) {
    // Iterate over admitted queries until either there are no more
    // messages available, or the maximum number of messages have
    // been collected.
    DCHECK(worker_messages->empty());
    std::unordered_map<std::size_t, bool> finished_queries_ids;

    if (learner_->hasActiveQueries()) {
      // Key = priority level. Value = Whether we have already checked the
      std::unordered_map<std::size_t, bool> checked_priority_levels;
      // While there are more priority levels to be checked ..
      while (checked_priority_levels.size() < priority_query_ids_.size() && worker_messages->size() < FLAGS_max_msgs_per_dispatch_round) {
        const int chosen_priority_level = learner_->pickRandomPriorityLevel();
        if (chosen_priority_level == kInvalidPriorityLevel) {
          DLOG(INFO) << "No valid priority level chosen";
          break;
        } else if (checked_priority_levels.find(static_cast<std::size_t>(
                       chosen_priority_level)) != checked_priority_levels.end()) {
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
  } else {
    getWorkerMessagesHPF(worker_messages);
  }
}

void PriorityPolicyEnforcer::getWorkerMessagesHPF(std::vector<std::unique_ptr<WorkerMessage>> *worker_messages) {
  // Iterate over admitted queries until either there are no more
  // messages available, or the maximum number of messages have
  // been collected.
  DCHECK(worker_messages->empty());
  if (learner_->hasActiveQueries()) {
    const std::set<std::size_t> &priority_set = learner_->getSetOfPriorityLevels();
    std::unordered_map<std::size_t, bool> finished_queries_ids;
    auto priority_set_reverse_it = priority_set.rbegin();
    if (priority_set.empty()) {
      DLOG(INFO) << "No priority level available";
      return;
    }
    while (worker_messages->size() < FLAGS_max_msgs_per_dispatch_round && priority_set_reverse_it != priority_set.rend()) {
      WorkerMessage *next_worker_message = getNextWorkerMessageFromPriorityLevel(*priority_set_reverse_it, &finished_queries_ids);
      if (next_worker_message != nullptr) {
        worker_messages->push_back(std::unique_ptr<WorkerMessage>(next_worker_message));
      } else {
        // No work orders available in the current priority level, check the
        // next largest priority level.
        ++priority_set_reverse_it;
      }
    }
    for (auto finished_qid_pair : finished_queries_ids) {
      removeQuery(finished_qid_pair.first);
    }
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
  const std::size_t estimated_memory_bytes = query_id_to_handle_[query_id]->getEstimatedMaxMemoryInBytes();
  committed_memory_ -= estimated_memory_bytes;
  // Remove the query from the learner.
  learner_->removeQuery(query_id);
  // TODO(harshad) - Admit waiting queries, if any.
  query_id_to_handle_.erase(query_id);
  DLOG(INFO) << "Removed query: " << query_id << " with priority: " << query_priority;
}

void PriorityPolicyEnforcer::suspendQuery(const std::size_t query_id) {
  suspended_query_managers_[query_id].reset(admitted_queries_[query_id].release());
  suspended_queries_.push_back(query_id_to_handle_[query_id]);
  DCHECK(admitted_queries_.find(query_id) != admitted_queries_.end());
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
  const std::size_t estimated_memory_bytes = query_id_to_handle_[query_id]->getEstimatedMaxMemoryInBytes();
  // TODO(harshad) - Support actually evicting the memory used up by the suspended query.
  committed_memory_ -= estimated_memory_bytes;
  const std::size_t curr_query_footprint = getMemoryForQueryInBytes(query_id);
  suspended_memory_ += curr_query_footprint;
  // Remove the query from the learner.
  learner_->removeQuery(query_id);
  std::cout << "Suspended query: " << query_id << " with priority: " << query_priority << "\n";
}

bool PriorityPolicyEnforcer::admitQueries(
    const std::vector<QueryHandle*> &query_handles) {
  bool result = true;
  for (QueryHandle *curr_query : query_handles) {
    if (!admitQuery(curr_query)) {
      result = false;
    }
  }
  return result;
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
    int chosen_query_id;
    if (FLAGS_dynamic_probabilities_in_learner) {
      chosen_query_id = learner_->pickRandomQueryFromPriorityLevel<true>(priority_level);
    } else {
      chosen_query_id = learner_->pickRandomQueryFromPriorityLevel<false>(priority_level);
    }
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

bool PriorityPolicyEnforcer::admissionMemoryCheck(const QueryHandle *query_handle) {
  if (admitted_queries_.empty()) {
    // No query running in the system, let the query in.
    const std::size_t estimated_memory_requirement_bytes = query_handle->getEstimatedMaxMemoryInBytes();
    committed_memory_ += estimated_memory_requirement_bytes;
    return true;
  }
  const std::size_t estimated_memory_requirement_bytes = query_handle->getEstimatedMaxMemoryInBytes();
  const std::size_t estimated_slots = StorageManager::SlotsNeededForBytes(estimated_memory_requirement_bytes);
  const std::size_t current_slots = StorageManager::SlotsNeededForBytes(storage_manager_->getMemorySize() - ((suspended_memory_ > 0) ? suspended_memory_ : 0));
  const std::size_t committed_slots = StorageManager::SlotsNeededForBytes(committed_memory_);
  /*std::cout << "Requested: " << std::max(committed_slots, current_slots) + estimated_slots << " Current: " << current_slots << " Limit: " << 0.8 * float(storage_manager_->getMaxBufferPoolSlots()) << "\n";*/
  if (std::max(committed_slots, current_slots) + estimated_slots < (0.8 * float(storage_manager_->getMaxBufferPoolSlots()))) {
    committed_memory_ += estimated_memory_requirement_bytes;
    return true;
  }
  return false;
}

const std::size_t PriorityPolicyEnforcer::getMemoryForQueryInBytes(const std::size_t query_id) {
  DCHECK(query_id_to_handle_.find(query_id) != query_id_to_handle_.end());
  QueryHandle *query_handle = query_id_to_handle_[query_id];
  std::size_t memory = query_handle->getMemoryTempRelationsBytes();
  if (!hasQuerySuspended(query_id)) {
    memory += admitted_queries_[query_id]->getMemoryBytes();
  } else {
    memory += suspended_query_managers_[query_id]->getMemoryBytes();
  }
  return memory;
}

const std::pair<int, std::size_t> PriorityPolicyEnforcer::getQueryWithHighestMemoryFootprint() {
  std::size_t max_memory_footprint = 0;
  int query_id_with_max_memory = kInvalidQueryID;
  for (auto it = admitted_queries_.begin(); it != admitted_queries_.end(); ++it) {
    const std::size_t curr_query_footprint = getMemoryForQueryInBytes(it->first);
    if (curr_query_footprint > max_memory_footprint) {
      max_memory_footprint = curr_query_footprint;
      query_id_with_max_memory = static_cast<int>(it->first);
    }
  }
  return std::make_pair(query_id_with_max_memory, max_memory_footprint);
}

bool PriorityPolicyEnforcer::hasQuerySuspended(const std::size_t query_id) const {
  auto it = std::find_if(suspended_queries_.begin(), suspended_queries_.end(), [query_id] (const QueryHandle *handle) {return handle->query_id() == query_id; });
  return it != suspended_queries_.end();
}

}  // namespace quickstep
