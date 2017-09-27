/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_QUERY_EXECUTION_EXECUTION_STATS_HPP_
#define QUICKSTEP_QUERY_EXECUTION_EXECUTION_STATS_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory>
#include <unordered_map>
#include <utility>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief Record the execution stats of a query.
 *
 * @note The time is measured in microseconds.
 **/
class ExecutionStats {
 public:
  /**
   * @brief Constructor
   *
   * @param max_entries The maximum number of entries we remember for each
   *        operator.
   **/
  explicit ExecutionStats(const std::size_t max_entries)
      : max_entries_(max_entries) {}

  /**
   * @brief Get the number of active operators in stats.
   **/
  std::size_t getNumActiveOperators() const {
    return active_operators_.size();
  }

  /**
   * @brief Check if there are stats present for at least one active operator.
   **/
  inline bool hasStats() const {
    for (auto it = active_operators_.begin(); it != active_operators_.end(); ++it) {
      if (it->second->hasStatsForOperator()) {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief Get the current stats for the query.
   *
   * @return A pair - 1st element is total time, 2nd element is total number of
   *         WorkOrders for the whole query.
   **/
  std::pair<std::uint64_t, std::uint64_t> getCurrentStatsForQuery() const {
    std::uint64_t total_time = 0;
    std::uint64_t num_workorders = 0;
    for (auto it = active_operators_.begin(); it != active_operators_.end(); ++it) {
      auto operator_stats = getCurrentStatsForOperator(it->first);
      total_time += operator_stats.first;
      num_workorders += operator_stats.second;
    }
    return std::make_pair(total_time, num_workorders);
  }

  /**
   * @brief Get the average work order time for the query.
   */
  double getAverageWorkOrderTimeForQuery() const {
    auto result = getCurrentStatsForQuery();
    if (result.second != 0) {
      return result.first / static_cast<double>(result.second);
    }
    return 0.0;
  }

  /**
   * @brief Get the current stats for the given operator.
   * @param operator_id The ID of the operator.
   * @return A pair - 1st element is total time, 2nd element is total number of
   *         WorkOrders for the operator.
   */
  std::pair<std::uint64_t, std::uint64_t> getCurrentStatsForOperator(const std::size_t operator_id) const {
    if (hasOperator(operator_id)) {
      DCHECK(active_operators_.at(operator_id) != nullptr);
      return active_operators_.at(operator_id)->getStats();
    }
    return std::make_pair(0, 0);
  }

  double getAverageWorkOrderTimeForOperator(const std::size_t operator_id) const {
    auto result = getCurrentStatsForOperator(operator_id);
    if (result.second != 0) {
      return result.first / static_cast<double>(result.second);
    }
    return 0.0;
  }

  /**
   * @brief Add a new entry to stats.
   *
   * @param value The value to be added.
   * @param operator_index The operator index which the value belongs to.
   **/
  void addEntry(const std::size_t value, const std::size_t operator_index) {
    if (!hasOperator(operator_index)) {
      // This is the first entry for the given operator.
      // Create the OperatorStats object for this operator.
      active_operators_[operator_index] =
          std::make_unique<OperatorStats>(max_entries_);
    }
    active_operators_[operator_index]->addEntry(value);
  }

  /**
   * @brief Remove the operator with given index. This should be called only
   *        when the given operator finishes its execution.
   **/
  void removeOperator(const std::size_t operator_index) {
    DCHECK(hasOperator(operator_index));
    active_operators_.erase(operator_index);
  }

 private:
  /**
   * @brief Stats for an operator within the query.
   *
   * @note We remember only the last N entries for the operator.
   **/
  class OperatorStats {
   public:
    /**
     * @brief Constructor.
     *
     * @param max_entries The maximum number of entries we remember. Typically
     *        these are the last N (=max_entries) entries.
     **/
    explicit OperatorStats(const std::size_t max_entries) : max_entries_(max_entries) {}

    inline std::pair<std::uint64_t, std::size_t> getStats() const {
      return std::make_pair(std::accumulate(times_.begin(), times_.end(), 0),
                            times_.size());
    }

    inline void addEntry(const std::uint64_t time_value) {
      if (times_.size() == max_entries_) {
        times_.pop_front();
      }
      times_.push_back(time_value);
      DCHECK_LE(times_.size(), max_entries_);
    }

    inline bool hasStatsForOperator() const {
      return !times_.empty();
    }

   private:
    const std::size_t max_entries_;
    // Times are measured in microseconds.
    std::deque<std::uint64_t> times_;

    DISALLOW_COPY_AND_ASSIGN(OperatorStats);
  };

  /**
   * @brief Check if the operator with given index is present in the stats.
   **/
  inline bool hasOperator(const std::size_t operator_index) const {
    return active_operators_.find(operator_index) != active_operators_.end();
  }

  const std::size_t max_entries_;

  std::unordered_map<std::size_t, std::unique_ptr<OperatorStats>>
      active_operators_;

  DISALLOW_COPY_AND_ASSIGN(ExecutionStats);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_EXECUTION_STATS_HPP_
