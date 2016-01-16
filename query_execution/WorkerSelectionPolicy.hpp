/**
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
   * @param start_worker_id The ID of the worker which should be chosen
   *        initially.
   *
   * @warning If the number of workers change between the constructor and
   *          getNextWorkerID() invocations, the worker ID returned by
   *          getNextWorkerID() initially may not be the same as desired.
   **/
  RoundRobinWorkerSelectionPolicy(const WorkerDirectory &worker_directory,
                                  const std::size_t start_worker_id)
      : WorkerSelectionPolicy(worker_directory) {
    if (start_worker_id == 0) {
      previous_worker_id_ = worker_directory.getNumWorkers() - 1;
    } else {
      previous_worker_id_ = start_worker_id - 1;
    }
  }

  std::size_t getNextWorkerID() override {
    previous_worker_id_ =
        (previous_worker_id_ + 1) % worker_directory_.getNumWorkers();
    return previous_worker_id_;
  }

 private:
  std::size_t previous_worker_id_;

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
