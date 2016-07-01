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
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace serialization { class NormalWorkOrderCompletionMessage; }

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

class Learner {
 public:
  /**
   * @brief Constructor.
   **/
  Learner();

  void addCompletionFeedback(
      const serialization::NormalWorkOrderCompletionMessage
          &workorder_completion_proto);

  /**
   * @brief Add a query to the Learner.
   *
   * @param query_handle The query handle for the new query.
   **/
  void addQuery(const QueryHandle &query_handle) {
    initializePriorityLevelIfNotPresent(query_handle.query_priority());
    initializeQuery(query_handle);
    relearn();
  }

  /**
   * @brief Remove a query from the Learner.
   *
   * @param query_id The ID of the query to be removed.
   **/
  void removeQuery(const std::size_t query_id) {
    DCHECK(isQueryPresent(query_id));
    const std::size_t priority_level = getQueryPriorityUnsafe(query_id);
    // Find the iterator to the query in execution_stats_.
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
    // has_feedback_from_all_queries_[priority_level] = false;
    query_id_to_priority_lookup_.erase(query_id);
    checkAndRemovePriorityLevel(priority_level);
    relearn();
  }

  /**
   * @brief Remove the stats of a given operator in a given query.
   **/
  void removeOperator(const std::size_t query_id,
                      const std::size_t operator_id) {
    ExecutionStats *stats = getExecutionStats(query_id);
    DCHECK(stats != nullptr);
    stats->removeOperator(operator_id);
  }

  /**
   * @brief Reset the probabilities and start learning again.
   **/
  void relearn() {
    if (hasActiveQueries()) {
      initializeDefaultProbabilitiesForAllQueries();
      initializeDefaultProbabilitiesForPriorityLevels();
    }
  }

  /**
   * @brief Check if there are any active queries in the Learner.
   **/
  inline const bool hasActiveQueries() const {
    return !query_id_to_priority_lookup_.empty();
  }

  /**
   * @brief Get the number of active queries in the Learner for the given
   *        priority level.
   **/
  inline const std::size_t getNumActiveQueriesInPriorityLevel(
      const std::size_t priority_level) const {
    const auto it = execution_stats_.find(priority_level);
    if (it != execution_stats_.end()) {
      return it->second.size();
    } else {
      return 0;
    }
  }

  /**
   * @brief Get the total number of active queries in the Learner.
   **/
  inline const std::size_t getTotalNumActiveQueries() const {
    return query_id_to_priority_lookup_.size();
  }

  /**
   * @brief Get the highest priority level among the active queries.
   *
   * @return The highest priority level. If the system is empty it returns
   *         kInvalidPriorityLevel.
   **/
  inline const int getHighestPriorityLevel() const {
    return highest_priority_level_;
  }

  /**
   * @brief Randomly pick a priority level.
   *
   * @note We use uniform random distribution.
   *
   * @return A priority level. If no queries are present in the learner, return
   *         kInvalidPriorityLevel.
   **/
  inline const int pickRandomPriorityLevel() const {
    if (hasActiveQueries()) {
      const int result = static_cast<int>(
          probabilities_of_priority_levels_->pickRandomProperty());
      return result;
    } else {
      return kInvalidPriorityLevel;
    }
  }

  /**
   * @brief Randomly pick a query from any priority level in the learner.
   *
   * @note We use uniform random distribution.
   *
   * @return A query ID. If no queries are present in the learner, return
   *         kInvalidQueryID.
   **/
  inline const int pickRandomQuery() const {
    if (hasActiveQueries()) {
      const int random_priority_level = pickRandomPriorityLevel();
      // Note : The valid priority level values are non-zero.
      DCHECK_GT(random_priority_level, 0);
      const int result = pickRandomQueryFromPriorityLevel(
          static_cast<std::size_t>(random_priority_level));
      return result;
    } else {
      return kInvalidQueryID;
    }
  }

  /**
   * @brief Randomly pick a query from a given priority level in the learner.
   *
   * @note We use uniform random distribution.
   *
   * @return A query ID. If no queries are present for this priority level in
   *         the learner, return kInvalidQueryID.
   **/
  inline const int pickRandomQueryFromPriorityLevel(
      const std::size_t priority_level) const {
    DCHECK(isPriorityLevelPresent(priority_level));
    if (hasActiveQueries()) {
      if (hasFeedbackFromAllQueriesInPriorityLevel(priority_level)) {
        DCHECK(current_probabilities_.at(priority_level) != nullptr);
        const auto it = current_probabilities_.find(priority_level);
        if (it->second->getNumObjects() > 0) {
          return static_cast<int>(
              current_probabilities_.at(priority_level)->pickRandomProperty());
        }
      } else {
        DCHECK(default_probabilities_.at(priority_level) != nullptr);
        const auto it = default_probabilities_.find(priority_level);
        if (it->second->getNumObjects() > 0) {
          return static_cast<int>(
              default_probabilities_.at(priority_level)->pickRandomProperty());
        }
      }
    }
    return kInvalidQueryID;
  }

  /**
   * @brief Given a query ID, if the query exists in the learner, return its
   *        priority, otherwise return kInvalidPriorityLevel.
   **/
  inline int getQueryPriority(const std::size_t query_id) const {
    const auto it = query_id_to_priority_lookup_.find(query_id);
    if (it != query_id_to_priority_lookup_.end()) {
      return it->second;
    } else {
      return kInvalidPriorityLevel;
    }
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
    CHECK_GT(priority_level, 0) << "Priority level should be non-zero";
    if (!isPriorityLevelPresent(priority_level)) {
      current_probabilities_[priority_level].reset(new ProbabilityStore());
      execution_stats_.emplace(priority_level, std::vector<std::pair<std::size_t, std::unique_ptr<ExecutionStats>>>());
      if (static_cast<int>(priority_level) > highest_priority_level_) {
        highest_priority_level_ = priority_level;
      }
    }
  }

  /**
   * @brief First check if the priority level needs to be removed from the local
   *        data structures and remove if needed.
   *
   * @param priority_level The priority level.
   **/
  void checkAndRemovePriorityLevel(const std::size_t priority_level);

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
             std::end(execution_stats_[getQueryPriorityUnsafe(query_id)]));
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
    const std::size_t priority_level = getQueryPriorityUnsafe(query_id);
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
   *
   * @note This version assumes that the given query ID exists in data
   *       structures.
   **/
  inline const std::size_t getQueryPriorityUnsafe(const std::size_t query_id) const {
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
    // NOTE(harshad) : Not using this cache as it gets confusing.
    // return has_feedback_from_all_queries_.at(priority_level);
    DCHECK(isPriorityLevelPresent(priority_level));
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
        &stats_vector = execution_stats_[priority_level];
    for (std::size_t i = 0; i < stats_vector.size(); ++i) {
      DCHECK(stats_vector[i].second != nullptr);
      if (!stats_vector[i].second->hasStats()) {
        // At least one query has no statistics so far.
        // NOTE(harshad) : Not using this cache as it gets confusing.
        // has_feedback_from_all_queries_[priority_level] = false;
        return;
      }
    }
    // All the queries have at least one execution statistic.
    // NOTE(harshad) : Not using this cache as it gets confusing.
    // has_feedback_from_all_queries_[priority_level] = true;
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
   * @param mean_workorder_per_query An unordered_map in which:
   *        1st element is the query ID.
   *        2nd element is mean time per work order
   *
   * @note If any query has mean work order time as 0, we return 0 as the
   *       denominator.
   *
   * @return The denominator to be used for probability calculations.
   **/
  inline float calculateDenominator(
      const std::unordered_map<std::size_t, std::size_t>
          &mean_workorder_per_query) const {
    float denominator = 0;
    for (const auto &element : mean_workorder_per_query) {
      if (element.second != 0) {
        denominator += 1/static_cast<float>(element.second);
      }
    }
    return denominator;
  }

  inline bool hasFeedbackFromAllPriorityLevels() const {
    // for (auto feedback : has_feedback_from_all_queries_) {
    // NOTE(harshad) : Not using this cache as it gets confusing.
    for (auto priority_iter = default_probabilities_.cbegin();
         priority_iter != default_probabilities_.cend();
         ++priority_iter) {
      if (!hasFeedbackFromAllQueriesInPriorityLevel(priority_iter->first)) {
        return false;
      }
    }
    return true;
  }

  void printProbabilitiesForPriorityLevel(const std::size_t priority_level);

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

  // NOTE(harshad) : Not using this cache as it gets confusing.
  // Key = priority level. Value = A boolean that indicates if we have received
  // feedback from all the queries in the given priority level.
  // std::unordered_map<std::size_t, bool> has_feedback_from_all_queries_;

  int highest_priority_level_;

  DISALLOW_COPY_AND_ASSIGN(Learner);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_LEARNER_HPP_
