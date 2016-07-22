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

#include "query_execution/Learner.hpp"

#include <algorithm>
#include <cstddef>
#include <chrono>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "query_execution/ExecutionStats.hpp"
#include "query_execution/ProbabilityStore.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_optimizer/QueryHandle.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

DEFINE_uint64(max_past_entries_learner,
              10,
              "The maximum number of past WorkOrder execution statistics"
              " entries for a query");

Learner::Learner() {
  probabilities_of_priority_levels_.reset(new ProbabilityStore());
  // Format: Query ID, Operator ID, Worker ID, Time in micros, WO execution end timestamp.
  LOG(INFO) << "Query ID|Operator ID|Worker ID|Time in microseconds|Workorder end timestamp";
}

void Learner::updateProbabilitiesForQueriesInPriorityLevel(
    const std::size_t priority_level, const std::size_t query_id) {
  DCHECK(isPriorityLevelPresent(priority_level));
  if (execution_stats_[priority_level].empty()) {
    LOG(WARNING) << "Updating probabilities for query ID: " << query_id
                 << " and priority level: " << priority_level
                 << " that has no queries";
    return;
  } else if (execution_stats_[priority_level].size() == 1u) {
    DCHECK(current_probabilities_[priority_level] != nullptr);
    // As we want the probability of the lone query in this priority level as
    // 1, we set the numerator same as denominator.
    // TODO(harshad) - Get the mean work order times here too and use that as
    // the numerator.
    ExecutionStats *stats = getExecutionStats(query_id);
    auto query_stats = stats->getCurrentStats();
    /*const std::size_t numerator =
        current_probabilities_[priority_level]->getDenominator();*/
    if (query_stats.second != 0) {
      const float mean_workorder_time =
          query_stats.first / static_cast<float>(query_stats.second);
      if (mean_workorder_time != 0) {
        current_probabilities_[priority_level]->addOrUpdateObjectNewDenominator(
            query_id, 1 / mean_workorder_time, 1 / mean_workorder_time);
      }
    }
    return;
  }
  // Else, there are more than one queries for the given priority level.
  std::unordered_map<std::size_t, float>
      mean_workorders_per_query =
          getMeanWorkOrderTimesForQueriesInPriorityLevel(priority_level);
  const float denominator = calculateDenominator(mean_workorders_per_query);
  if (denominator != 0) {
    // Update the numerator for the given query and denominator for all the
    // queries.
    DCHECK(mean_workorders_per_query.find(query_id) !=
           mean_workorders_per_query.end());
    if (mean_workorders_per_query[query_id] != 0) {
      current_probabilities_[priority_level]->addOrUpdateObjectNewDenominator(
          query_id,
          1 / static_cast<float>(mean_workorders_per_query[query_id]),
          denominator);
    }
  } else {
    // At least one of the queries has predicted time for next work order as 0.
    // In such a case, we don't update the probabilities and continue to use
    // the older probabilities.
    return;
  }
}

void Learner::updateProbabilitiesOfAllPriorityLevels() {
  if (!hasFeedbackFromAllPriorityLevels()) {
      // has_feedback_from_all_queries_.empty()) {
      // NOTE(harshad) : Not using this cache as it gets confusing.
    // Either we don't have enough feedback messages from all the priority
    // levels OR there are no active queries in the system.
    return;
  }
  // Compute the predicted work order execution times for all the level.
  std::unordered_map<std::size_t, float> predicted_time_for_level;
  std::size_t sum_active_priorities = 0;
  for (auto priority_iter = execution_stats_.begin();
       priority_iter != execution_stats_.end();
       ++priority_iter) {
    float total_time_curr_level = 0;
    const std::size_t curr_priority_level = priority_iter->first;
    sum_active_priorities += curr_priority_level;
    // For each query, find its predicted work order execution time.
    const std::unordered_map<std::size_t, float>
        mean_workorders_all_queries_curr_level =
            getMeanWorkOrderTimesForQueriesInPriorityLevel(
                curr_priority_level);
    for (auto mean_workorder_entry : mean_workorders_all_queries_curr_level) {
      total_time_curr_level += mean_workorder_entry.second;
    }
    const std::size_t num_queries_in_priority_level =
        execution_stats_[curr_priority_level].size();
    DCHECK_GT(num_queries_in_priority_level, 0u);
    predicted_time_for_level[curr_priority_level] =
        total_time_curr_level / num_queries_in_priority_level;
  }
  DCHECK_GT(sum_active_priorities, 0u);
  // Now compute the allowable number of work orders for each priority level
  // that can be executed given a unit total time.
  // Key = priority level, value = the # of WO mentioned above.
  std::unordered_map<std::size_t, float> num_workorders_for_level;
  float total_num_workorders = 0;
  for (auto predicted_time_iter : predicted_time_for_level) {
    const std::size_t curr_priority_level = predicted_time_iter.first;
    const float num_workorders_for_curr_level =
        (predicted_time_iter.second == 0)
            ? 0
            : static_cast<float>(curr_priority_level) /
                  sum_active_priorities /
                  static_cast<float>(predicted_time_iter.second);
    num_workorders_for_level[curr_priority_level] = num_workorders_for_curr_level;
    total_num_workorders += num_workorders_for_curr_level;
  }
  if (total_num_workorders == 0) {
    // No priority level can be selected at this point.
    return;
  }
  // Finally, compute the probabilities.
  std::vector<std::size_t> priority_levels;
  std::vector<float> numerators;
  for (auto num_workorders_iter : num_workorders_for_level) {
    priority_levels.emplace_back(num_workorders_iter.first);
    numerators.emplace_back(num_workorders_iter.second);
  }
  probabilities_of_priority_levels_->addOrUpdateObjectsNewDenominator(
      priority_levels, numerators, total_num_workorders);
}

void Learner::initializeDefaultProbabilitiesForAllQueries() {
  for (auto queries_same_priority_level_iter = execution_stats_.begin();
       queries_same_priority_level_iter != execution_stats_.end();
       ++queries_same_priority_level_iter) {
    std::vector<std::size_t> query_ids;
    const auto &queries_vector = queries_same_priority_level_iter->second;
    DCHECK(!queries_vector.empty());
    for (auto query_iter = queries_vector.cbegin();
         query_iter != queries_vector.cend();
         ++query_iter) {
      query_ids.emplace_back(query_iter->first);
    }
    // Numerator for each query is 1.0
    // The common denominator is number of queries with the given priority level.
    std::vector<float> numerators(query_ids.size(), 1.0);
    // Reset the probability store for this level.
    const std::size_t curr_priority_level =
        queries_same_priority_level_iter->first;
    default_probabilities_[curr_priority_level].reset(new ProbabilityStore());
    default_probabilities_[curr_priority_level]
        ->addOrUpdateObjectsNewDenominator(
            query_ids, numerators, query_ids.size());
  }
}

void Learner::initializeDefaultProbabilitiesForPriorityLevels() {
  probabilities_of_priority_levels_.reset(new ProbabilityStore());
  std::vector<std::size_t> priority_levels;
  std::vector<float> numerators;
  float sum_priority_levels = 0;
  for (auto priority_iter = execution_stats_.cbegin();
       priority_iter != execution_stats_.cend();
       ++priority_iter) {
    DCHECK(!priority_iter->second.empty());
    const std::size_t curr_priority_level = priority_iter->first;
    sum_priority_levels += curr_priority_level;
    priority_levels.emplace_back(curr_priority_level);
    numerators.emplace_back(curr_priority_level);
  }
  if (sum_priority_levels > 0) {
    probabilities_of_priority_levels_->addOrUpdateObjectsNewDenominator(
        priority_levels, numerators, sum_priority_levels);
  }
}

void Learner::initializeQuery(const QueryHandle &query_handle) {
  const std::size_t priority_level = query_handle.query_priority();
  const std::size_t query_id = query_handle.query_id();
  DCHECK(isPriorityLevelPresent(priority_level));
  query_id_to_priority_lookup_[query_id] = priority_level;
  // TODO(harshad) - Create a gflag for max_past_entries_learner.
  execution_stats_[priority_level].emplace_back(
      query_id,
      std::unique_ptr<ExecutionStats>(
          new ExecutionStats(FLAGS_max_past_entries_learner)));
  // As we are initializing the query, we obviously haven't gotten any
  // feedback message for this query. Hence mark the following field as false.
  // has_feedback_from_all_queries_[priority_level] = false;
  // NOTE(harshad) : Not using this cache as it gets confusing.
}

void Learner::checkAndRemovePriorityLevel(const std::size_t priority_level) {
  DCHECK(isPriorityLevelPresent(priority_level));
  if (execution_stats_[priority_level].empty()) {
    execution_stats_.erase(priority_level);
    current_probabilities_.erase(priority_level);
    probabilities_of_priority_levels_->removeObject(priority_level);
    default_probabilities_.erase(priority_level);
    // NOTE(harshad) : Not using this cache as it gets confusing.
    // has_feedback_from_all_queries_.erase(priority_level);
    priority_levels_set_.erase(priority_level);
  }
}

void Learner::printProbabilitiesForPriorityLevel(const std::size_t priority_level) {
  DCHECK(isPriorityLevelPresent(priority_level));
  if (hasFeedbackFromAllQueriesInPriorityLevel(priority_level)) {
    DCHECK(current_probabilities_.at(priority_level) != nullptr);
    const auto it = current_probabilities_.find(priority_level);
    if (it->second->getNumObjects() > 0) {
      it->second->printIndividualProbabilities();
    }
  } else {
    DCHECK(default_probabilities_.at(priority_level) != nullptr);
    const auto it = default_probabilities_.find(priority_level);
    if (it->second->getNumObjects() > 0) {
      it->second->printIndividualProbabilities();
    }
  }
}

void Learner::printWorkOrderDetails(
    const serialization::NormalWorkOrderCompletionMessage &proto) const {
  // Format: Query ID, Operator ID, Worker ID, Time in micros, WO execution end timestamp.
  std::string result = "";
  result.reserve(30);
  result += std::to_string(proto.query_id());  // 2 chars
  result += "|";
  result += std::to_string(proto.operator_index());  // 2 chars
  result += "|";
  result += std::to_string(proto.worker_thread_index());  // 2 chars
  result += "|";
  result += std::to_string(proto.execution_time_in_microseconds());  // 5 chars
  result += "|";
  result += std::to_string(proto.execution_end_timestamp());  // 12 chars
  LOG(INFO) << result;
}

void Learner::printPredictedWorkOrderTimes() {
  std::string output = "";
  output.reserve(50);
  for (auto qid_priority_pair : query_id_to_priority_lookup_) {
    ExecutionStats *stats = getExecutionStats(qid_priority_pair.first);
    auto query_stats = stats->getCurrentStats();
    output += std::to_string(qid_priority_pair.first);
    output += ",";
    if (query_stats.second != 0) {
      const float mean_workorder_time =
          query_stats.first / static_cast<float>(query_stats.second);
      output += std::to_string(mean_workorder_time);
    } else {
      output += std::to_string(0.0);
    }
    output += ",";
  }
  if (!output.empty()) {
    // Add the timestamp for the prediction in the end.
    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    const uint64_t execution_end_timestamp =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end.time_since_epoch())
            .count();
    output += std::to_string(execution_end_timestamp);
    LOG(INFO) << output;
  }
}

}  // namespace quickstep
