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

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_STATE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_STATE_HPP_

#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class that tracks the state of the execution of a query which
 *        includes status of operators, number of dispatched work orders etc.
 **/
class QueryExecutionState {
 public:
  /**
   * @brief Constructor.
   *
   * @param num_operators Number of relational operators in the query.
   **/
  explicit QueryExecutionState(const std::size_t num_operators)
      : num_operators_(num_operators),
        num_operators_finished_(0),
        queued_workorders_per_op_(num_operators, 0),
        rebuild_required_(num_operators, false),
        done_gen_(num_operators, false),
        execution_finished_(num_operators, false) {}

  /**
   * @brief Get the number of operators in the query.
   **/
  inline const std::size_t getNumOperators() const {
    return num_operators_;
  }

  /**
   * @brief Get the number of operators who have finished their execution.
   **/
  inline const std::size_t getNumOperatorsFinished() const {
    return num_operators_finished_;
  }

  /**
   * @brief Check if the query has finished its execution.
   *
   * @return True if the query has finished its execution, false otherwise.
   **/
  inline bool hasQueryExecutionFinished() const {
    return num_operators_finished_ == num_operators_;
  }

  /**
   * @brief Set the rebuild status of the given operator that includes the
   *        flag for whether the rebuild has been initiated and if so, the
   *        number of pending rebuild work orders.
   *
   * @param operator_index The index of the given operator.
   * @param num_rebuild_workorders The number of rebuild workorders of the given
   *        operator.
   * @param rebuild_initiated True if the rebuild has been initiated, false
   *        otherwise.
   **/
  inline void setRebuildStatus(const std::size_t operator_index,
                               const std::size_t num_rebuild_workorders,
                               const bool rebuild_initiated) {
    DCHECK(operator_index < num_operators_);
    rebuild_status_[operator_index].second = num_rebuild_workorders;
    rebuild_status_[operator_index].first = rebuild_initiated;
  }

  /**
   * @brief Check if the rebuild has been initiated for the given operator.
   *
   * @param operator_index The index of the given operator.
   *
   * @return True if the rebuild has been initiated, false otherwise.
   **/
  inline bool hasRebuildInitiated(const std::size_t operator_index) const {
    DCHECK(operator_index < num_operators_);
    const auto search_res = rebuild_status_.find(operator_index);
    if (search_res != rebuild_status_.end()) {
      return search_res->second.first;
    }
    return false;
  }

  /**
   * @brief Get the number of pending rebuild workorders for the given operator.
   *
   * @param operator_index The index of the given operator.
   *
   * @return The number of pending rebuild workorders for the given operator.
   **/
  inline const std::size_t getNumRebuildWorkOrders(
      const std::size_t operator_index) const {
    DCHECK(operator_index < num_operators_);
    const auto search_res = rebuild_status_.find(operator_index);
    if (search_res != rebuild_status_.end()) {
      return search_res->second.second;
    }
    LOG(WARNING) << "Called QueryExecutionState::getNumRebuildWorkOrders() "
                    "for an operator whose rebuild entry doesn't exist.";
    return 0;
  }

  /**
   * @brief Decrement the number of rebuild WorkOrders for the given operator.
   *
   * @param operator_index The index of the given operator.
   **/
  inline void decrementNumRebuildWorkOrders(const std::size_t operator_index) {
    DCHECK(operator_index < num_operators_);
    const auto search_res = rebuild_status_.find(operator_index);
    if (search_res != rebuild_status_.end()) {
      DCHECK(search_res->second.first);
      DCHECK_GE(search_res->second.second, 1u);
      --rebuild_status_[operator_index].second;
    } else {
      LOG(FATAL) <<
          "Called QueryExecutionState::decrementNumRebuildWorkOrders() for an "
          "operator whose rebuild entry doesn't exist.";
    }
  }

  /**
   * @brief Increment the number of queued (normal) WorkOrders for the given
   *        operator.
   *
   * @param operator_index The index of the given operator.
   **/
  inline void incrementNumQueuedWorkOrders(const std::size_t operator_index) {
    DCHECK(operator_index < num_operators_);
    ++queued_workorders_per_op_[operator_index];
  }

  /**
   * @brief Decrement the number of queued (normal) WorkOrders for the given
   *        operator.
   *
   * @param operator_index The index of the given operator.
   **/
  inline void decrementNumQueuedWorkOrders(const std::size_t operator_index) {
    DCHECK(operator_index < num_operators_);
    DCHECK_GT(queued_workorders_per_op_[operator_index], 0u);
    --queued_workorders_per_op_[operator_index];
  }

  /**
   * @brief Get the number of queued (normal) WorkOrders for the given operator.
   *
   * @note Queued WorkOrders mean those WorkOrders which have been dispatched
   *       for execution by the Foreman and haven't yet completed. These are
   *       different from pending WorkOrders which mean the WorkOrders that
   *       haven't been dispatched for execution yet.
   *
   * @param operator_index The index of the given operator.
   *
   * @return The number of queued (normal) WorkOrders for the given operators.
   **/
  inline const std::size_t getNumQueuedWorkOrders(
      const std::size_t operator_index) const {
    DCHECK(operator_index < num_operators_);
    return queued_workorders_per_op_[operator_index];
  }

  /**
   * @brief Set the rebuild required flag as true for the given operator.
   *
   * @param operator_index The index of the given operator.
   **/
  inline void setRebuildRequired(const std::size_t operator_index) {
    DCHECK(operator_index < num_operators_);
    rebuild_required_[operator_index] = true;
  }

  /**
   * @brief Get the rebuild required flag for the given operator.
   *
   * @param operator_index The index of the given operator.
   **/
  inline bool isRebuildRequired(const std::size_t operator_index) const {
    DCHECK(operator_index < num_operators_);
    return rebuild_required_[operator_index];
  }

  /**
   * @brief Set the execution finished flag for the given operator as true.
   *
   * @note By default this flag is false.
   *
   * @param operator_index The index of the given operator.
   **/
  inline void setExecutionFinished(const std::size_t operator_index) {
    DCHECK(operator_index < num_operators_);
    execution_finished_[operator_index] = true;
    ++num_operators_finished_;
  }

  /**
   * @brief Get the execution finished flag for the given operator.
   *
   * @param operator_index The index of the given operator.
   **/
  inline bool hasExecutionFinished(const std::size_t operator_index) const {
    DCHECK(operator_index < num_operators_);
    return execution_finished_[operator_index];
  }

  /**
   * @brief Set the "done generation of workorders" flag as true for the given
   *        operator.
   *
   * @note By default this flag is false.
   *
   * @param operator_index The index of the given operator.
   **/
  inline void setDoneGenerationWorkOrders(const std::size_t operator_index) {
    DCHECK(operator_index < num_operators_);
    done_gen_[operator_index] = true;
  }

  /**
   * @brief Get the "done generation of workorders" flag for the given operator.
   *
   * @param operator_index The index of the given operator.
   **/
  inline bool hasDoneGenerationWorkOrders(const std::size_t operator_index)
      const {
    DCHECK(operator_index < num_operators_);
    return done_gen_[operator_index];
  }

 private:
  // Total number of operators in the query.
  const std::size_t num_operators_;

  // Number of operators who've finished their execution.
  std::size_t num_operators_finished_;

  // A vector to track the number of normal WorkOrders in execution.
  std::vector<std::size_t> queued_workorders_per_op_;

  // The ith bit denotes if the operator with ID = i requires generation of
  // rebuild WorkOrders.
  std::vector<bool> rebuild_required_;

  // The ith bit denotes if the operator with ID = i has finished generating
  // work orders.
  std::vector<bool> done_gen_;

  // The ith bit denotes if the operator with ID = i has finished its execution.
  std::vector<bool> execution_finished_;

  // Key is dag_node_index for which rebuild is required. Value is a pair -
  // first element is a bool (whether rebuild for operator at index i has been
  // initiated) and if the boolean is true, the second element denotes the
  // number of pending rebuild workorders for the operator.
  std::unordered_map<std::size_t, std::pair<bool, std::size_t>> rebuild_status_;

  DISALLOW_COPY_AND_ASSIGN(QueryExecutionState);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_STATE_HPP_
