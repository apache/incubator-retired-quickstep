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

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <random>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkerDirectory.hpp"

namespace quickstep {

class WorkerDirectoryTest : public ::testing::Test {
 protected:
  // First element is the index of the min element, second element is the
  // value of the min element.
  static std::pair<std::size_t, std::size_t> getMinElement(
      const std::vector<std::size_t> &input_vector) {
    std::vector<std::size_t>::const_iterator min_element_iter =
            std::min_element(std::begin(input_vector), std::end(input_vector));
    const std::size_t min_element_id =
        std::distance(input_vector.begin(), min_element_iter);
    return std::make_pair(min_element_id, *min_element_iter);
  }

  // First element is the index of the max element, second element is the
  // value of the max element.
  static std::pair<std::size_t, std::size_t> getMaxElement(
      const std::vector<std::size_t> &input_vector) {
    std::vector<std::size_t>::const_iterator max_element_iter =
            std::max_element(std::begin(input_vector), std::end(input_vector));
    const std::size_t max_element_id =
        std::distance(input_vector.begin(), max_element_iter);
    return std::make_pair(max_element_id, *max_element_iter);
  }

  void SetUp() {
    std::vector<int> numa_nodes;
    std::vector<client_id> client_ids;

    numa_nodes.reserve(kNumWorkers);
    client_ids.reserve(kNumWorkers);

    for (std::size_t worker_thread_index = 0; worker_thread_index < kNumWorkers; ++worker_thread_index) {
      // NUMA node id = worker_thread_index % 4
      // Client ID = worker_thread_index * 2 + 1
      const client_id cid = worker_thread_index * 2 + 1;
      const int numa_node_id = worker_thread_index % 4;

      numa_nodes.push_back(numa_node_id);
      client_ids.push_back(cid);

      worker_thread_indexs_.push_back(worker_thread_index);
      actual_workers_[worker_thread_index] = std::make_pair(numa_node_id, cid);
    }
    wd_.reset(new WorkerDirectory(kNumWorkers, client_ids, numa_nodes));

    // Randomize the order of worker IDs.
    std::random_shuffle(worker_thread_indexs_.begin(), worker_thread_indexs_.end());
  }

  // First element is NUMA Node. Second element is client ID.
  // Input is the logical worker ID.
  std::pair<int, client_id> getNUMANodeAndClientIDForWorker(
      const std::size_t worker_thread_index) {
    return actual_workers_[worker_thread_index];
  }

  WorkerDirectory* getWorkerDirectory() {
    return wd_.get();
  }

  std::size_t getActualNumWorkers() const {
    return kNumWorkers;
  }

  std::vector<size_t>& getRandomizedWorkerIDs() {
    std::random_shuffle(worker_thread_indexs_.begin(), worker_thread_indexs_.end());
    return worker_thread_indexs_;
  }

 private:
  std::unique_ptr<WorkerDirectory> wd_;

  // Key = Worker's logical ID
  // Value = pair <NUMA node ID, client ID> for the worker.
  std::unordered_map<std::size_t, std::pair<int, client_id>> actual_workers_;

  std::vector<std::size_t> worker_thread_indexs_;

  const std::size_t kNumWorkers = 100;
};

TEST_F(WorkerDirectoryTest, NUMANodeAndClientIDTest) {
  // Check if the NUMA node ID and client IDs are set correctly and the number
  // of queued workorders is initialized correctly.
  WorkerDirectory *wd = getWorkerDirectory();

  EXPECT_EQ(getActualNumWorkers(), wd->getNumWorkers());

  for (std::size_t worker_thread_index : getRandomizedWorkerIDs()) {
    const std::pair<int, client_id> observed_ids =
        getNUMANodeAndClientIDForWorker(worker_thread_index);
    EXPECT_EQ(observed_ids.first, wd->getNUMANode(worker_thread_index));
    EXPECT_EQ(observed_ids.second, wd->getClientID(worker_thread_index));
    EXPECT_EQ(0u, wd->getNumQueuedWorkOrders(worker_thread_index));
  }
}

TEST_F(WorkerDirectoryTest, IncrementAndDecrementWorkOrdersTest) {
  // Increment and decrement the workorders for workers.
  // Check if the NUMA node ID and client IDs are set correctly and the number
  WorkerDirectory *wd = getWorkerDirectory();
  EXPECT_EQ(getActualNumWorkers(), wd->getNumWorkers());

  std::vector<std::size_t> actual_num_workorders;
  actual_num_workorders.resize(getActualNumWorkers(), 0);

  // We perform the increment and decrement in workorders as determined by a
  // pre-initialized sequence.
  // true = increment and false = decrement in number of workorders.
  std::vector<bool> increment_decrement_mini_sequence(
      {true, true, false, true, false, false, true, false});

  std::vector<bool> increment_decrement_sequence;

  // Insert the mini sequence into sequence kNumWorkers number of times.
  for (std::size_t i = 0; i < getActualNumWorkers(); ++i) {
    increment_decrement_sequence.insert(
        increment_decrement_sequence.end(),
        increment_decrement_mini_sequence.cbegin(),
        increment_decrement_mini_sequence.cend());
  }

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<std::size_t> dist(0, getActualNumWorkers() - 1);

  // Perform the increment or decrement operation as determined by the sequence
  // and check the correctness.
  std::stack<std::size_t> worker_thread_indexs_used;
  for (bool to_increment : increment_decrement_sequence) {
    if (to_increment) {
      // Pick a random worker ID and increment its number of workorders.
      const std::size_t chosen_worker_thread_index = dist(mt);
      worker_thread_indexs_used.push(chosen_worker_thread_index);
      EXPECT_EQ(actual_num_workorders[chosen_worker_thread_index],
                wd->getNumQueuedWorkOrders(chosen_worker_thread_index));
      wd->incrementNumQueuedWorkOrders(chosen_worker_thread_index);
      ++actual_num_workorders[chosen_worker_thread_index];
      EXPECT_EQ(actual_num_workorders[chosen_worker_thread_index],
                wd->getNumQueuedWorkOrders(chosen_worker_thread_index));
    } else {
      // For the worker with ID = top of stack, decrement a workorder.
      const std::size_t chosen_worker_thread_index = worker_thread_indexs_used.top();
      worker_thread_indexs_used.pop();
      EXPECT_EQ(actual_num_workorders[chosen_worker_thread_index],
                wd->getNumQueuedWorkOrders(chosen_worker_thread_index));
      wd->decrementNumQueuedWorkOrders(chosen_worker_thread_index);
      --actual_num_workorders[chosen_worker_thread_index];
      EXPECT_EQ(actual_num_workorders[chosen_worker_thread_index],
                wd->getNumQueuedWorkOrders(chosen_worker_thread_index));
    }
  }

  // Stack should be empty.
  EXPECT_TRUE(worker_thread_indexs_used.empty());
  // Expect no queued up workorders for any worker.
  for (const std::size_t random_worker_thread_index : getRandomizedWorkerIDs()) {
    EXPECT_EQ(0u, wd->getNumQueuedWorkOrders(random_worker_thread_index));
    EXPECT_EQ(actual_num_workorders[random_worker_thread_index],
              wd->getNumQueuedWorkOrders(random_worker_thread_index));
  }
}

TEST_F(WorkerDirectoryTest, AddWorkerTest) {
  // Add a worker to the worker directory after constructor call.
  WorkerDirectory *wd = getWorkerDirectory();
  EXPECT_EQ(getActualNumWorkers(), wd->getNumWorkers());

  const client_id new_worker_client_id = getActualNumWorkers() * 2 + 1;
  const int new_worker_numa_node = 4;
  wd->addWorker(new_worker_client_id, new_worker_numa_node);

  // The logical ID of the new worker.
  const std::size_t new_worker_thread_index = getActualNumWorkers();

  EXPECT_EQ(getActualNumWorkers() + 1, wd->getNumWorkers());
  // Check if the client ID is set correctly.
  EXPECT_EQ(new_worker_client_id, wd->getClientID(new_worker_thread_index));
  // Check if the NUMA node ID is set correctly.
  EXPECT_EQ(new_worker_numa_node, wd->getNUMANode(new_worker_thread_index));
  // Check if the new worker has no queued up workorders.
  EXPECT_EQ(0u, wd->getNumQueuedWorkOrders(new_worker_thread_index));
  // Increment a workorder for the new worker, check if the increment is
  // successful, then perform a decrement and check the correctness.
  wd->incrementNumQueuedWorkOrders(new_worker_thread_index);
  EXPECT_EQ(1u, wd->getNumQueuedWorkOrders(new_worker_thread_index));
  wd->decrementNumQueuedWorkOrders(new_worker_thread_index);
  EXPECT_EQ(0u, wd->getNumQueuedWorkOrders(new_worker_thread_index));
}

TEST_F(WorkerDirectoryTest, WorkerLoadTest) {
  // Assign load (in terms of number of workorders) to the workers and check if
  // the least and most loaded worker is retrieved correctly.
  WorkerDirectory *wd = getWorkerDirectory();
  EXPECT_EQ(getActualNumWorkers(), wd->getNumWorkers());

  std::vector<std::size_t> actual_num_workorders;
  actual_num_workorders.resize(getActualNumWorkers(), 0);

  // Loop over workers sequentially and increment workorder of all the workers.
  for (std::size_t worker_thread_index = 0; worker_thread_index < getActualNumWorkers();
       ++worker_thread_index) {
    const std::pair<std::size_t, std::size_t> &actual_min_loaded_worker =
        getMinElement(actual_num_workorders);
    const std::pair<std::size_t, std::size_t> &actual_max_loaded_worker =
        getMaxElement(actual_num_workorders);

    EXPECT_EQ(actual_min_loaded_worker, wd->getLeastLoadedWorker());
    EXPECT_EQ(actual_max_loaded_worker, wd->getMostLoadedWorker());

    wd->incrementNumQueuedWorkOrders(worker_thread_index);
    ++actual_num_workorders[worker_thread_index];
    EXPECT_EQ(actual_num_workorders[worker_thread_index],
              wd->getNumQueuedWorkOrders(worker_thread_index));
  }

  // At this time, every worker has exactly one workorder assigned to it.
  // Now increment workorders in a random order.
  for (const std::size_t random_worker_thread_index : getRandomizedWorkerIDs()) {
    const std::pair<std::size_t, std::size_t> actual_min_loaded_worker =
        getMinElement(actual_num_workorders);
    const std::pair<std::size_t, std::size_t> actual_max_loaded_worker =
        getMaxElement(actual_num_workorders);

    EXPECT_EQ(actual_min_loaded_worker, wd->getLeastLoadedWorker());
    EXPECT_EQ(actual_max_loaded_worker, wd->getMostLoadedWorker());

    wd->incrementNumQueuedWorkOrders(random_worker_thread_index);
    ++actual_num_workorders[random_worker_thread_index];
    EXPECT_EQ(actual_num_workorders[random_worker_thread_index],
              wd->getNumQueuedWorkOrders(random_worker_thread_index));
  }

  // At this time, every worker has two workorders assigned to it.
  // Now decrement workorders in a random order twice.
  for (std::size_t iteration = 0; iteration < 2; ++iteration) {
    for (const std::size_t random_worker_thread_index : getRandomizedWorkerIDs()) {
      const std::pair<std::size_t, std::size_t> actual_min_loaded_worker =
          getMinElement(actual_num_workorders);
      const std::pair<std::size_t, std::size_t> actual_max_loaded_worker =
          getMaxElement(actual_num_workorders);

      EXPECT_EQ(actual_min_loaded_worker, wd->getLeastLoadedWorker());
      EXPECT_EQ(actual_max_loaded_worker, wd->getMostLoadedWorker());

      wd->decrementNumQueuedWorkOrders(random_worker_thread_index);
      --actual_num_workorders[random_worker_thread_index];
      EXPECT_EQ(actual_num_workorders[random_worker_thread_index],
                wd->getNumQueuedWorkOrders(random_worker_thread_index));
    }
  }
  const std::pair<std::size_t, std::size_t> actual_min_loaded_worker =
      getMinElement(actual_num_workorders);
  const std::pair<std::size_t, std::size_t> actual_max_loaded_worker =
      getMaxElement(actual_num_workorders);

  EXPECT_EQ(actual_min_loaded_worker, wd->getLeastLoadedWorker());
  EXPECT_EQ(actual_max_loaded_worker, wd->getMostLoadedWorker());
}
}  // namespace quickstep
