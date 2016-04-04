/**
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_EXECUTION_SHIFTBOSS_DIRECTORY_HPP_
#define QUICKSTEP_QUERY_EXECUTION_SHIFTBOSS_DIRECTORY_HPP_

#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class which keeps the metadata about the shiftbosses.
 *
 * @note This class is intended to be used only by ForemanDistributed thread.
 *       Therefore, none of the methods in this class are thread-safe.
 **/
class ShiftbossDirectory {
 public:
  /**
   * @brief Constructor.
  **/
  ShiftbossDirectory() = default;

  /**
   * @brief Add the Shiftboss.
   *
   * @param shiftboss_id the TMB client ID of Shiftboss thread.
   * @param capacity The Work Order processing capacity of Shiftboss.
   **/
  void addShiftboss(const tmb::client_id shiftboss_id,
                    const std::size_t capacity) {
    client_id_indices_.emplace(shiftboss_id, client_ids_.size());
    client_ids_.push_back(shiftboss_id);
    work_order_capacities_.push_back(capacity);
    num_queued_work_orders_.push_back(0u);
  }

  /**
   * @brief Whether the ShiftbossDirectory has any Shiftboss.
   *
   * @return True if no Shiftboss in ShiftbossDirectory. Otherwise false.
   **/
  bool empty() const {
    DCHECK_EQ(client_ids_.empty(), client_id_indices_.empty());
    DCHECK_EQ(client_ids_.empty(), work_order_capacities_.empty());
    DCHECK_EQ(client_ids_.empty(), num_queued_work_orders_.empty());

    return client_ids_.empty();
  }

  /**
   * @brief Get the number of Shiftboss in ShiftbossDirectory.
   *
   * @return The number of Shiftboss in ShiftbossDirectory.
   **/
  std::size_t size() const {
    DCHECK_EQ(client_ids_.size(), client_id_indices_.size());
    DCHECK_EQ(client_ids_.size(), work_order_capacities_.size());
    DCHECK_EQ(client_ids_.size(), num_queued_work_orders_.size());

    return client_ids_.size();
  }

  /**
   * @brief Get the TMB client ID of the specified Shiftboss.
   *
   * @param shiftboss_index The index of Shiftboss.
   *
   * @return The TMB client ID of the given Shiftboss.
   **/
  tmb::client_id getClientId(const std::size_t shiftboss_index) const {
    DCHECK_LT(shiftboss_index, size());
    return client_ids_[shiftboss_index];
  }

  /**
   * @brief Get the Shiftboss index from the specified client id.
   *
   * @param shiftboss_id The TMB client ID of Shiftboss.
   *
   * @return The index of the given Shiftboss.
   **/
  std::size_t getShiftbossIndex(const tmb::client_id shiftboss_id) const {
    const auto it = client_id_indices_.find(shiftboss_id);
    DCHECK(it != client_id_indices_.end());

    return it->second;
  }

  /**
   * @brief Whether the given Shiftboss has reached its capacity.
   *
   * @param shiftboss_index The index of Shiftboss.
   *
   * @return True if reached the capacity. Otherwise false.
   **/
  bool hasReachedCapacity(const std::size_t shiftboss_index) const {
    DCHECK_LT(shiftboss_index, size());
    return num_queued_work_orders_[shiftboss_index] >= work_order_capacities_[shiftboss_index];
  }

  /**
   * @brief Add the number of new work orders for the given Shiftboss.
   *
   * @param shiftboss_index The index of Shiftboss.
   * @param num_new_work_orders The number of the new work orders will be
   *        executed on Shiftboss indexed by 'shiftboss_index'.
   **/
  void addNumQueuedWorkOrders(const std::size_t shiftboss_index,
                              const std::size_t num_new_work_orders) {
    num_queued_work_orders_[shiftboss_index] += num_new_work_orders;
  }

  /**
   * @brief Increase the number of queued workorders for the given Shiftboss by 1.
   *
   * @param shiftboss_index The index of Shiftboss.
   **/
  void incrementNumQueuedWorkOrders(const std::size_t shiftboss_index) {
    DCHECK_LT(shiftboss_index, size());
    ++num_queued_work_orders_[shiftboss_index];
  }

  /**
   * @brief Decrease the number of queued workorders for the given Shiftboss by 1.
   *
   * @param shiftboss_index The index of Shiftboss.
   **/
  void decrementNumQueuedWorkOrders(const std::size_t shiftboss_index) {
    DCHECK_LT(shiftboss_index, size());
    DCHECK_GE(num_queued_work_orders_[shiftboss_index], 1u);
    --num_queued_work_orders_[shiftboss_index];
  }

 private:
  // The TMB client IDs of Shiftbosses.
  // TODO(zuyu): Support deletions, as Shiftbosses go down.
  std::vector<tmb::client_id> client_ids_;

  // The map from the TMB client ID of Shiftboss to its index in 'client_ids_'.
  std::unordered_map<tmb::client_id, std::size_t> client_id_indices_;

  // The max number of WorkOrders per Shiftboss.
  std::vector<std::size_t> work_order_capacities_;

  // The number of WorkOrders queued for execution per Shiftboss, and the value
  // should be not greater than that of work_order_capacities_.
  std::vector<std::size_t> num_queued_work_orders_;

  DISALLOW_COPY_AND_ASSIGN(ShiftbossDirectory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_SHIFTBOSS_DIRECTORY_HPP_
