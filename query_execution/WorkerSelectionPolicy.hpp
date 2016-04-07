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

#ifndef QUICKSTEP_QUERY_EXECUTION_WORKER_SELECTION_POLICY_HPP_
#define QUICKSTEP_QUERY_EXECUTION_WORKER_SELECTION_POLICY_HPP_

#include <cstddef>
#include <random>
#include <utility>

#include "query_execution/WorkerDirectory.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief Base class for a policy to select workers for workorder execution.
 **/
class WorkerSelectionPolicy {
 public:
  /**
   * @brief Constructor.
   *
   * @param worker_directory The worker directory.
   **/
  explicit WorkerSelectionPolicy(const WorkerDirectory &worker_directory)
      : worker_directory_(worker_directory) {
    if (worker_directory.getNumWorkers() == 0) {
      FATAL_ERROR("WorkerSelectionPolicy needs at least one worker");
    }
  }

  /**
   * @brief Choose the worker for next workorder execution based on the policy.
   *
   * @return The logical ID of the worker chosen for next workorder execution.
   **/
  virtual std::size_t getNextWorkerID() = 0;

 protected:
  const WorkerDirectory &worker_directory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(WorkerSelectionPolicy);
};

/**
 * @brief Choose the next worker in a round robin manner.
 **/
class RoundRobinWorkerSelectionPolicy : public WorkerSelectionPolicy {
 public:
  /**
   * @brief Constructor.
   *
   * @param worker_directory The worker directory.
   * @param start_worker_thread_index The ID of the worker which should be chosen
   *        initially.
   *
   * @warning If the number of workers change between the constructor and
   *          getNextWorkerID() invocations, the worker ID returned by
   *          getNextWorkerID() initially may not be the same as desired.
   **/
  RoundRobinWorkerSelectionPolicy(const WorkerDirectory &worker_directory,
                                  const std::size_t start_worker_thread_index)
      : WorkerSelectionPolicy(worker_directory) {
    if (start_worker_thread_index == 0) {
      previous_worker_thread_index_ = worker_directory.getNumWorkers() - 1;
    } else {
      previous_worker_thread_index_ = start_worker_thread_index - 1;
    }
  }

  std::size_t getNextWorkerID() override {
    previous_worker_thread_index_ =
        (previous_worker_thread_index_ + 1) % worker_directory_.getNumWorkers();
    return previous_worker_thread_index_;
  }

 private:
  std::size_t previous_worker_thread_index_;

  DISALLOW_COPY_AND_ASSIGN(RoundRobinWorkerSelectionPolicy);
};

/**
 * @brief Choose the next worker so that the load among workers is balanced.
 **/
class LoadBalancingWorkerSelectionPolicy : public WorkerSelectionPolicy {
 public:
  explicit LoadBalancingWorkerSelectionPolicy(
      const WorkerDirectory &worker_directory)
      : WorkerSelectionPolicy(worker_directory) {}

  std::size_t getNextWorkerID() override {
    return worker_directory_.getLeastLoadedWorker().first;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(LoadBalancingWorkerSelectionPolicy);
};

/**
 * @brief Choose a (uniform) random worker.
 **/
class RandomWorkerSelectionPolicy : public WorkerSelectionPolicy {
 public:
  explicit RandomWorkerSelectionPolicy(const WorkerDirectory &worker_directory)
      : WorkerSelectionPolicy(worker_directory),
        mt_(std::random_device()()) {}

  std::size_t getNextWorkerID() override {
    std::uniform_int_distribution<std::size_t> dist(
        0, worker_directory_.getNumWorkers() - 1);
    return dist(mt_);
  }

 private:
  std::mt19937_64 mt_;

  DISALLOW_COPY_AND_ASSIGN(RandomWorkerSelectionPolicy);
};
/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_WORKER_SELECTION_POLICY_HPP_
