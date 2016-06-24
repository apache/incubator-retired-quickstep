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

#ifndef QUICKSTEP_QUERY_EXECUTION_LEARNER_HPP_
#define QUICKSTEP_QUERY_EXECUTION_LEARNER_HPP_

#include <algorithm>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

#include "query_execution/ExecutionStats.hpp"
#include "query_execution/ProbabilityStore.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_optimizer/QueryHandle.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

class Learner {
 public:
  /**
   * @brief Constructor.
   **/
  Learner() {
    probabilities_of_priority_levels_.reset(new ProbabilityStore());
  }

  void addCompletionFeedback(
      const serialization::NormalWorkOrderCompletionMessage
          &workorder_completion_proto);

  void addQuery(const QueryHandle &query_handle) {
    initializePriorityLevelIfNotPresent(query_handle.query_priority());
    initializeQuery(query_handle);
    relearn();
  }

  void removeQuery(const std::size_t query_id) {
    // Find the iterator to the query in execution_stats_.
    DCHECK(isQueryPresent(query_id));
    const std::size_t priority_level = getQueryPriority(query_id);
    auto stats_iter_mutable = getExecutionStatsIterMutable(query_id);
    execution_stats_[priority_level].erase(stats_iter_mutable);
    DCHECK(current_probabilities_.find(priority_level) !=
           current_probabilities_.end());
    if (current_probabilities_[priority_level]->hasObject(query_id)) {
      // We may have cases when a query doesn't produce any feedback message,
      // therefore we may not have an entry for this query in the
      // current_probabilities_[priority_level] ProbabilityStore.
      current_probabilities_[priority_level]->removeObject(query_id);
    }
    query_id_to_priority_lookup_.erase(query_id);
    checkAndRemovePriorityLevel(priority_level);
    relearn();
  }

  void removeOperator(const std::size_t query_id, const std::size_t operator_id) {
    ExecutionStats *stats = getExecutionStats(query_id);
    DCHECK(stats != nullptr);
    stats->removeOperator(operator_id);
  }

  void relearn() {
    if (hasActiveQueries()) {
      initializeDefaultProbabilitiesForAllQueries();
      initializeDefaultProbabilitiesForPriorityLevels();
    }
  }

  inline const bool hasActiveQueries() const {
    return !query_id_to_priority_lookup_.empty();
  }

  inline const std::size_t getNumActiveQueriesInPriorityLevel(
      const std::size_t priority_level) const {
    const auto it = execution_stats_.find(priority_level);
    if (it != execution_stats_.end()) {
      return it->second.size();
    } else {
      return 0;
    }
  }

  inline const std::size_t getTotalNumActiveQueries() const {
    return query_id_to_priority_lookup_.size();
  }

 private:
  /**
   * @brief Update the probabilities for queries in the given priority level.
   *
   * @note This function is called after the learner receives a completion
   *       feedback message from a given query.
   *
   * @param priority_level The priority level.
   * @param query_id The ID of the query for which a completion feedback message
   *        has been received.
   *
   **/
  void updateProbabilitiesForQueriesInPriorityLevel(
      const std::size_t priority_level, const std::size_t query_id);

  /**
   * @brief Update the probabilities of all the priority levels.
   *
   * TODO(harshad) - Cache internal results from previous invocation of this
   * function and reuse them. There's a lot of redundancy in computations
   * at this point.
   **/
  void updateProbabilitiesOfAllPriorityLevels();

  /**
   * @brief Initialize the default probabilities for the queries.
   **/
  void initializeDefaultProbabilitiesForAllQueries();

  /**
   * @brief Initialize the default probabilities for the priority levels.
   **/
  void initializeDefaultProbabilitiesForPriorityLevels();

  /**
   * @brief Initialize the data structures for a given priority level, if none
   *        exist. If there are already data structures for the given priority
   *        level, do nothing.
   *
   * @note This function should be followed by a relearn() call, to insert this
   *       priority levels in probabilities_of_priority_levels_.
   **/
  inline void initializePriorityLevelIfNotPresent(
      const std::size_t priority_level) {
    if (!isPriorityLevelPresent(priority_level)) {
      current_probabilities_[priority_level].reset(new ProbabilityStore());
      execution_stats_[priority_level];
    }
  }

  /**
   * @brief First check if the priority level needs to be removed from the local
   *        data structures and remove if needed.
   *
   * @param priority_level The priority level.
   **/
  inline void checkAndRemovePriorityLevel(const std::size_t priority_level) {
    DCHECK(isPriorityLevelPresent(priority_level));
    if (execution_stats_[priority_level].empty()) {
      execution_stats_.erase(priority_level);
      current_probabilities_.erase(priority_level);
      probabilities_of_priority_levels_->removeObject(priority_level);
      has_feedback_from_all_queries_.erase(priority_level);
    }
  }

  /**
   * @brief Check if the Learner has presence of the given priority level.
   *
   * @param priority_level The priority level.
   *
   * @return True if present, false otherwise.
   **/
  inline bool isPriorityLevelPresent(const std::size_t priority_level) const {
    DCHECK_EQ((current_probabilities_.find(priority_level) ==
               current_probabilities_.end()),
              execution_stats_.find(priority_level) == execution_stats_.end());
    return (execution_stats_.find(priority_level) != execution_stats_.end());
  }

  /**
   * @brief Check if the query is present in local data structures.
   **/
  inline bool isQueryPresent(const std::size_t query_id) const {
    return query_id_to_priority_lookup_.find(query_id) !=
           query_id_to_priority_lookup_.end();
  }

  /**
   * @brief Initialize all the data structures for a new query.
   *
   * @param query_handle The query handle for the new query.
   **/
  void initializeQuery(const QueryHandle &query_handle);

  /**
   * @brief Get the execution stats object for the given query.
   *
   * @return A pointer to the ExecutionStats for the query. If not present,
   *         returns NULL.
   **/
  inline ExecutionStats* getExecutionStats(const std::size_t query_id) {
    if (isQueryPresent(query_id)) {
      const auto stats_iter = getExecutionStatsIterMutable(query_id);
      DCHECK(stats_iter !=
             std::end(execution_stats_[getQueryPriority(query_id)]));
      return stats_iter->second.get();
    }
    return nullptr;
  }

  /**
   * @breif Get a mutable iterator to the execution stats for a given query.
   *
   * @note This function works well when the query and priority level exists
   *       in the data structures.
   **/
  inline std::vector<
      std::pair<std::size_t, std::unique_ptr<ExecutionStats>>>::const_iterator
      getExecutionStatsIterMutable(const std::size_t query_id) {
    const std::size_t priority_level = getQueryPriority(query_id);
    const std::vector<std::pair<std::size_t, std::unique_ptr<ExecutionStats>>>
        &stats_vector = execution_stats_[priority_level];
    // The following line uses std::find_if to reach to the desired element
    // in the stats_vector.
    auto stats_iter = std::find_if(
        stats_vector.begin(),
        stats_vector.end(),
        [&query_id](
            const std::pair<std::size_t, std::unique_ptr<ExecutionStats>> &p) {
          return p.first == query_id;
        });
    return stats_iter;
  }

  /**
   * @brief Get a query's priority level given its ID.
   **/
  inline const std::size_t getQueryPriority(const std::size_t query_id) const {
    const auto it = query_id_to_priority_lookup_.find(query_id);
    DCHECK(it != query_id_to_priority_lookup_.end());
    return it->second;
  }

  /**
   * @brief Check if we have received at least one feedback message from all the
   *        queries in the given priority level.
   **/
  inline bool hasFeedbackFromAllQueriesInPriorityLevel(
      const std::size_t priority_level) const {
    const std::vector<std::pair<std::size_t, std::unique_ptr<ExecutionStats>>>
        &stats_vector = execution_stats_.at(priority_level);
    for (std::size_t i = 0; i < stats_vector.size(); ++i) {
      DCHECK(stats_vector[i].second != nullptr);
      if (!stats_vector[i].second->hasStats()) {
        return false;
      }
    }
    return true;
  }

  inline void updateFeedbackFromQueriesInPriorityLevel(
      const std::size_t priority_level) {
    const std::vector<std::pair<std::size_t, std::unique_ptr<ExecutionStats>>>
        &stats_vector = execution_stats_.at(priority_level);
    for (std::size_t i = 0; i < stats_vector.size(); ++i) {
      DCHECK(stats_vector[i].second != nullptr);
      if (!stats_vector[i].second->hasStats()) {
        // At least one query has no statistics so far.
        return;
      }
    }
    // All the queries have at least one execution statistic.
    has_feedback_from_all_queries_[priority_level] = true;
  }

  /**
   * @brief Get the mean work order execution times for all the queries in
   *        a given priority level.
   *
   * @param priority_level The priority level.
   *
   * @return An unordered_map in which: Key = query ID.
   *         Value = Mean time per work order for that query.
   **/
  inline std::unordered_map<std::size_t, std::size_t>
  getMeanWorkOrderTimesForQueriesInPriorityLevel(
      const std::size_t priority_level) {
    DCHECK(isPriorityLevelPresent(priority_level));
    std::unordered_map<std::size_t, std::size_t> result;
    for (auto it = execution_stats_[priority_level].begin();
         it != execution_stats_[priority_level].end();
         ++it) {
      DCHECK(it->second.get() != nullptr);
      auto query_stats = it->second->getCurrentStats();
      result[it->first] =
          query_stats.second == 0 ? 0 : query_stats.first / query_stats.second;
    }
    return result;
  }

  /**
   * @param mean_workorder_per_query A vector of pairs in which:
   *        1st element is mean time per work order
   *        2nd element is the query ID.
   *
   * @note If any query has mean work order time as 0, we return 0 as the
   *       denominator.
   *
   * @return The denominator to be used for probability calculations.
   **/
  inline float calculateDenominator(std::unordered_map<std::size_t, std::size_t>
                                        &mean_workorder_per_query) const {
    float denominator = 0;
    for (const auto &element : mean_workorder_per_query) {
      if (element.second != 0) {
        denominator += 1/static_cast<float>(element.second);
      } else {
        return 0;
      }
    }
    return denominator;
  }

  inline bool hasFeedbackFromAllPriorityLevels() const {
    for (auto feedback : has_feedback_from_all_queries_) {
      if (!hasFeedbackFromAllQueriesInPriorityLevel(feedback.first)) {
        return false;
      }
    }
    return true;
  }

  // Key = Priority level, value = A vector of pairs.
  // Each pair:
  // 1st element: Query ID.
  // 2nd Element: Execution statistics for the query.
  std::unordered_map<
      std::size_t,
      std::vector<std::pair<std::size_t, std::unique_ptr<ExecutionStats>>>>
      execution_stats_;

  // Key = query ID, value = priority level for the query ID.
  std::unordered_map<std::size_t, std::size_t> query_id_to_priority_lookup_;

  // Key = priority level, value = ProbabilityStore for the queries belonging to
  // that priority level.
  std::unordered_map<std::size_t, std::unique_ptr<ProbabilityStore>>
      current_probabilities_;

  // Key = priority level, value = ProbabilityStore for the queries belonging to
  // that priority level.
  std::unordered_map<std::size_t, std::unique_ptr<ProbabilityStore>>
      default_probabilities_;

  // ProbabilityStrore for probabilities mapped to the priority levels.
  std::unique_ptr<ProbabilityStore> probabilities_of_priority_levels_;

  // Key = priority level. Value = A boolean that indicates if we have received
  // feedback from all the queries in the given priority level.
  std::unordered_map<std::size_t, bool> has_feedback_from_all_queries_;

  DISALLOW_COPY_AND_ASSIGN(Learner);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_LEARNER_HPP_
