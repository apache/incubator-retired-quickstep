/**
 *   Copyright 2015 Pivotal Software, Inc.
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

#include <cstddef>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_execution/WorkerSelectionPolicy.hpp"

namespace quickstep {

class WorkerSelectionPolicyTest : public ::testing::Test {
 protected:
  WorkerDirectory* getWorkerDirectory() {
    return directory_.get();
  }

  const std::size_t getRandomWorkerID() const {
    std::uniform_int_distribution<std::size_t> dist(
        0, directory_->getNumWorkers() - 1);
    return dist(*mt_);
  }

  void SetUp() {
    std::vector<int> numa_nodes;
    std::vector<client_id> client_ids;

    numa_nodes.reserve(kNumWorkers);
    client_ids.reserve(kNumWorkers);

    std::random_device rd;
    mt_.reset(new std::mt19937_64(rd()));
    for (std::size_t worker_id = 0; worker_id < kNumWorkers; ++worker_id) {
      // NUMA node id = worker_id % 4
      // Client ID = worker_id * 2 + 1
      numa_nodes.push_back(worker_id % 4);
      client_ids.push_back(worker_id * 2 + 1);
    }
    directory_.reset(new WorkerDirectory(kNumWorkers, client_ids, numa_nodes));
  }

 private:
  static const std::size_t kNumWorkers = 100;
  std::unique_ptr<std::mt19937_64> mt_;
  std::unique_ptr<WorkerDirectory> directory_;
};

TEST_F(WorkerSelectionPolicyTest, RoundRobinTest) {
  // Check if the round robin policy selects the next worker correctly.
  const std::size_t kStartWorkerID = getRandomWorkerID();
  RoundRobinWorkerSelectionPolicy rr_policy(*getWorkerDirectory(),
                                            kStartWorkerID);

  const std::size_t kNumIterations =
      (getWorkerDirectory()->getNumWorkers()) * 2;
  std::size_t expected_next_worker_id = kStartWorkerID;
  for (std::size_t iteration = 0; iteration < kNumIterations; ++iteration) {
    EXPECT_EQ(expected_next_worker_id, rr_policy.getNextWorkerID());
    expected_next_worker_id = (expected_next_worker_id + 1) %
                              (getWorkerDirectory()->getNumWorkers());
  }
}

TEST_F(WorkerSelectionPolicyTest, RoundRobinAddWorkerTest) {
  // Add a worker to the worker directory dynamically. Check if this worker
  // gets selected by the Round Robin policy.
  const std::size_t kStartWorkerID = getWorkerDirectory()->getNumWorkers() - 1;
  RoundRobinWorkerSelectionPolicy rr_policy(*getWorkerDirectory(),
                                            kStartWorkerID);

  EXPECT_EQ(kStartWorkerID, rr_policy.getNextWorkerID());

  // Add a new worker.
  const std::size_t new_worker_id = getWorkerDirectory()->getNumWorkers();
  getWorkerDirectory()->addWorker(new_worker_id * 2 + 1 /** client_id **/,
                                  0 /** NUMA node ID **/);

  EXPECT_EQ(new_worker_id, rr_policy.getNextWorkerID());
}

TEST_F(WorkerSelectionPolicyTest, LoadBalancingTest) {
  // Create a random set of worker load values (i.e. number of queued workorders)
  const std::size_t kMaxLoad = 100;
  std::vector<std::size_t> worker_loads;
  worker_loads.reserve(getWorkerDirectory()->getNumWorkers());

  std::random_device rd;
  std::mt19937_64 mt(rd());
  std::uniform_int_distribution<std::size_t> dist(0, kMaxLoad);

  for (std::size_t worker_id = 0;
       worker_id < getWorkerDirectory()->getNumWorkers();
       ++worker_id) {
    // For each worker ..
    const std::size_t worker_load = dist(mt);
    // assign a random load.
    worker_loads.push_back(worker_load);
    for (std::size_t workorder_count = 0;
         workorder_count < worker_load;
         ++workorder_count) {
      getWorkerDirectory()->incrementNumQueuedWorkOrders(worker_id);
    }
  }

  LoadBalancingWorkerSelectionPolicy lb_policy(*getWorkerDirectory());
  // At this point, all the workers have been assigned a random amount of load.
  // Keep decrementing one workorder from every worker and check if the policy
  // correctly chooses the next worker.
  std::size_t zero_load_counts = 0;
  for (std::size_t worker_load_index = 0;
       zero_load_counts < worker_loads.size();
       worker_load_index = (worker_load_index + 1) % worker_loads.size()) {
    EXPECT_EQ(getWorkerDirectory()->getLeastLoadedWorker().first,
              lb_policy.getNextWorkerID());
    if (worker_loads[worker_load_index] > 0) {
      getWorkerDirectory()->decrementNumQueuedWorkOrders(worker_load_index);
      --worker_loads[worker_load_index];
    } else {
      ++zero_load_counts;
    }
  }
  EXPECT_EQ(getWorkerDirectory()->getLeastLoadedWorker().first,
            lb_policy.getNextWorkerID());
}

TEST_F(WorkerSelectionPolicyTest, RandomWorkerPolicyTest) {
  // Expect that the randomly chosen worker falls in the limits of worker IDs.
  RandomWorkerSelectionPolicy rand_policy(*getWorkerDirectory());
  const std::size_t kNumIterations = 100;

  for (std::size_t iter = 0; iter < kNumIterations; ++iter) {
    const std::size_t chosen_worker_id = rand_policy.getNextWorkerID();
    EXPECT_GE(getWorkerDirectory()->getNumWorkers(), chosen_worker_id);
  }

  // Add a new worker and repeat the test above.
  const std::size_t new_worker_id = getWorkerDirectory()->getNumWorkers();
  getWorkerDirectory()->addWorker(new_worker_id * 2 + 1 /** client_id **/,
                                  0 /** NUMA node ID **/);

  for (std::size_t iter = 0; iter < kNumIterations; ++iter) {
    const std::size_t chosen_worker_id = rand_policy.getNextWorkerID();
    EXPECT_GE(getWorkerDirectory()->getNumWorkers(), chosen_worker_id);
  }
}

}  // namespace quickstep
