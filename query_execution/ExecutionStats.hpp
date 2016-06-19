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
      : max_entries_(max_entries), cached_stats_(std::make_pair(0, 0)) {}

  /**
   * @brief Get the number of active operators in stats.
   **/
  const std::size_t getNumActiveOperators() const {
    return active_operators_.size();
  }

  /**
   * @brief Get the current stats.
   *
   * @note This function updates the cache, hence it can't be const. We are lazy
   *       in updating the cache, instead of eagerly updating the cache upon
   *       each update.
   *
   * @return A pair - 1st element is total time, 2nd element is total number of
   *         WorkOrders for the whole query.
   **/
  std::pair<std::uint64_t, std::uint64_t> getCurrentStats() {
    if (active_operators_.empty()) {
      return cached_stats_;
    } else {
      std::pair<std::uint64_t, std::uint64_t> result = std::make_pair(0, 0);
      for (auto it = active_operators_.begin(); it != active_operators_.end(); ++it) {
        DCHECK(it->second.get() != nullptr);
        std::pair<std::uint64_t, std::size_t> op_stats = it->second->getStats();
        result.first += op_stats.first;
        result.second += op_stats.second;
      }
      if (result.first == 0 || result.second == 0) {
        // If one of the element in the pair is 0, use old result.
        return cached_stats_;
      } else if (result.first != 0 && result.second != 0) {
        cached_stats_ = result;
      }
      return result;
    }
  }

  /**
   * @brief Add a new entry to stats.
   *
   * @param value The value to be added.
   * @param operator_index The operator index which the value belongs to.
   **/
  void addEntry(std::size_t value, std::size_t operator_index) {
    if (hasOperator(operator_index)) {
      // This is not the first entry for the given operator.
      active_operators_[operator_index]->addEntry(value);
    } else {
      // Create the OperatorStats object for this operator.
      active_operators_[operator_index] =
          std::unique_ptr<OperatorStats>(new OperatorStats(max_entries_));
    }
  }

  /**
   * @brief Remove the operator with given index. This should be called only
   *        when the given operator finishes its execution.
   **/
  void removeOperator(std::size_t operator_index) {
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
    explicit OperatorStats(const std::size_t max_entries) : max_entries_(max_entries) {
      DCHECK_GE(max_entries, 0);
    }

    inline std::pair<std::uint64_t, std::size_t> getStats() const {
      return std::make_pair(std::accumulate(times_.begin(), times_.end(), 0),
                            times_.size());
    }

    inline void addEntry(std::uint64_t time_value) {
      if (times_.size() == max_entries_) {
        times_.pop_front();
      }
      times_.push_back(time_value);
      DCHECK_LE(times_.size(), max_entries_);
    }

   private:
    const std::size_t max_entries_;
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

  // Cached stats for the whole query.
  std::pair<std::uint64_t, std::uint64_t> cached_stats_;

  DISALLOW_COPY_AND_ASSIGN(ExecutionStats);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_EXECUTION_STATS_HPP_
