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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORKER_DIRECTORY_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORKER_DIRECTORY_HPP_

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */
/**
 * @brief A class which keeps the metadata about the workers.
 *
 * @note The number of workorders per worker used in this class are as viewed by
 *       Foreman. An alternative implementation can use TMB to query the queue
 *       lengths of each worker.
 *
 * @note This class is intended to be used only by Foreman thread. Therefore
 *       none of the methods in this class are thread-safe.
 **/
class WorkerDirectory {
 public:
  /**
   * @brief Constructor.
   *
   * @param num_workers The number of workers.
   * @param client_ids A vector of TMB client IDs for the workers.
   * @param numa_node_ids A vector of NUMA node IDs where the workers are
   *        pinned. If a worker is not pinned to any NUMA node, the NUMA node ID
   *        for that worker is -1.
   **/
  WorkerDirectory(const std::size_t num_workers,
                  const std::vector<client_id> &client_ids,
                  const std::vector<int> &numa_node_ids)
    : num_workers_(num_workers),
      num_queued_workorders_(num_workers, 0),
      numa_node_ids_(numa_node_ids),
      client_ids_(client_ids) {
    DEBUG_ASSERT(num_workers > 0);
    DEBUG_ASSERT(client_ids.size() == num_workers);
    DEBUG_ASSERT(numa_node_ids.size() == num_workers);
  }

  /**
   * @brief Get the number of worker threads.
   *
   * @return The number of worker threads at the time the function is called.
   **/
  inline const std::size_t getNumWorkers() const {
    return num_workers_;
  }

  /**
   * @brief Get the number of queued workorders for the given worker.
   *
   * @note The queued number of workorders consist of the workorders waiting for
   *       execution and the workorder being executed by the specified worker
   *       at the time this function is called.
   *
   * @param worker_thread_index The logical ID of the given worker.
   *
   * @return The number of queued workorders.
   **/
  inline const std::size_t getNumQueuedWorkOrders(
      const std::size_t worker_thread_index) const {
    DEBUG_ASSERT(worker_thread_index < num_workers_);
    return num_queued_workorders_[worker_thread_index];
  }

  /**
   * @brief Increment the number of queued workorders for the given worker by 1.
   *
   * @param worker_thread_index The logical ID of the given worker.
   **/
  inline void incrementNumQueuedWorkOrders(const std::size_t worker_thread_index) {
    DEBUG_ASSERT(worker_thread_index < num_workers_);
    ++num_queued_workorders_[worker_thread_index];
  }

  /**
   * @brief Decrement the number of queued workorders for the given worker by 1.
   *
   * @param worker_thread_index The logical ID of the given worker.
   **/
  inline void decrementNumQueuedWorkOrders(const std::size_t worker_thread_index) {
    DEBUG_ASSERT(worker_thread_index < num_workers_);
    DEBUG_ASSERT(num_queued_workorders_[worker_thread_index] >= 1);
    --num_queued_workorders_[worker_thread_index];
  }

  /**
   * @brief Get the NUMA node where the specified worker is pinned to.
   *
   * @param worker_thread_index The logical ID of the given worker.
   *
   * @return The NUMA node ID where the given worker is pinned. If the worker
   *         hasn't been pinned to any NUMA node, this value is -1.
   **/
  inline int getNUMANode(const std::size_t worker_thread_index) const {
    DEBUG_ASSERT(worker_thread_index < num_workers_);
    return numa_node_ids_[worker_thread_index];
  }

  /**
   * @brief Get the TMB client ID of the specified worker.
   *
   * @param worker_thread_index The logical ID of the given worker.
   *
   * @return The TMB client ID of the given worker.
   **/
  inline const client_id getClientID(const std::size_t worker_thread_index) const {
    DEBUG_ASSERT(worker_thread_index < num_workers_);
    return client_ids_[worker_thread_index];
  }

  /**
   * @brief Generate address of a worker.
   *
   * @param worker_thread_index The logical ID of the given worker.
   *
   * @return TMB Address of the given worker.
   **/
  inline Address getWorkerAddress(std::size_t worker_thread_index) const {
    DEBUG_ASSERT(worker_thread_index < num_workers_);
    Address worker_address;
    worker_address.AddRecipient(client_ids_[worker_thread_index]);
    return worker_address;
  }

  /**
   * @brief Add metadata about a new worker.
   *
   * @param cid The TMB client ID of the new worker.
   * @param numa_node_id The NUMA node ID where the new worker is pinned.
   **/
  inline void addWorker(const client_id cid, const int numa_node_id) {
    ++num_workers_;
    num_queued_workorders_.push_back(0);
    numa_node_ids_.push_back(numa_node_id);
    client_ids_.push_back(cid);
  }

  /**
   * @brief Get information about the least loaded worker at the time the
   *        function is called.
   *
   * TODO(harshad) This method performs a linear search. Using a dynamic
   *               priority queue to track worker loads can help this method
   *               execute faster.
   * TODO(harshad) Extend this method to find least loaded workers on a given
   *               NUMA node.
   *
   * @note If there are multiple workers with the least amount of load, the
   *       worker with the smaller logical ID is returned.
   *
   * @return A pair: First element is the logical ID of the least loaded worker
   *         and second element is the number of queued workorders for this
   *         worker at the time the function is called.
   **/
  std::pair<std::size_t, std::size_t> getLeastLoadedWorker() const {
    std::vector<std::size_t>::const_iterator min_element_iter =
        std::min_element(std::begin(num_queued_workorders_),
                         std::end(num_queued_workorders_));
    DEBUG_ASSERT(min_element_iter != num_queued_workorders_.end());
    const std::size_t least_loaded_worker_thread_index =
        std::distance(num_queued_workorders_.begin(), min_element_iter);
    return std::make_pair(least_loaded_worker_thread_index, *min_element_iter);
  }

  /**
   * @brief Get information about the most loaded worker at the time the
   *        function is called.
   *
   * TODO(harshad) This method performs a linear search. Using a dynamic
   *               priority queue to track worker loads can help this method
   *               execute faster.
   * TODO(harshad) Extend this method to find most loaded workers on a given
   *               NUMA node.
   *
   * @note If there are multiple workers with the maximum amount of load, the
   *       worker with the smaller logical ID is returned.
   *
   * @return A pair: First element is the logical ID of the most loaded worker
   *         and second element is the number of queued workorders for this
   *         worker at the time the function is called.
   **/
  std::pair<std::size_t, std::size_t> getMostLoadedWorker() const {
    std::vector<std::size_t>::const_iterator max_element_iter =
        std::max_element(std::begin(num_queued_workorders_),
                         std::end(num_queued_workorders_));
    DEBUG_ASSERT(max_element_iter != num_queued_workorders_.end());
    const std::size_t most_loaded_worker_thread_index =
        std::distance(num_queued_workorders_.begin(), max_element_iter);
    return std::make_pair(most_loaded_worker_thread_index, *max_element_iter);
  }

 private:
  std::size_t num_workers_;

  // The number of WorkOrders queued for execution per worker.
  std::vector<std::size_t> num_queued_workorders_;

  // The NUMA node IDs where the workers are pinned to. If a worker is not
  // pinned, the ID is -1.
  std::vector<int> numa_node_ids_;

  // The vector of client IDs
  std::vector<client_id> client_ids_;

  DISALLOW_COPY_AND_ASSIGN(WorkerDirectory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORKER_DIRECTORY_HPP_
