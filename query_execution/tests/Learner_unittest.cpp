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

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <memory>
#include <random>
#include <vector>

#include "gtest/gtest.h"

#include "query_execution/Learner.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/QueryHandle.hpp"

namespace quickstep {

class LearnerTest : public ::testing::Test {
 protected:
  serialization::NormalWorkOrderCompletionMessage createMockCompletionMessage(
      const std::size_t query_id, const std::size_t operator_id) {
    serialization::NormalWorkOrderCompletionMessage mock_proto_message;
    mock_proto_message.set_operator_index(operator_id);
    mock_proto_message.set_query_id(query_id);
    mock_proto_message.set_worker_thread_index(0);
    mock_proto_message.set_execution_time_in_microseconds(10);

    return mock_proto_message;
  }
};

TEST_F(LearnerTest, AddAndRemoveQueryTest) {
  Learner learner;
  std::unique_ptr<QueryHandle> handle;
  const std::size_t kPriorityLevel = 1;
  handle.reset(new QueryHandle(1, kPriorityLevel));

  EXPECT_FALSE(learner.hasActiveQueries());
  learner.addQuery(*handle);
  EXPECT_EQ(1u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));
  EXPECT_TRUE(learner.hasActiveQueries());

  learner.removeQuery(handle->query_id());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));
  EXPECT_FALSE(learner.hasActiveQueries());

  handle.reset(new QueryHandle(2, kPriorityLevel));
  learner.addQuery(*handle);
  EXPECT_EQ(1u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));
  EXPECT_TRUE(learner.hasActiveQueries());

  learner.removeQuery(handle->query_id());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));
  EXPECT_FALSE(learner.hasActiveQueries());
}

TEST_F(LearnerTest, MultipleQueriesSamePriorityAddRemoveTest) {
  Learner learner;
  std::unique_ptr<QueryHandle> handle1, handle2;
  const std::size_t kPriorityLevel = 1;
  handle1.reset(new QueryHandle(1, kPriorityLevel));
  handle2.reset(new QueryHandle(2, kPriorityLevel));

  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));

  learner.addQuery(*handle1);
  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(1u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));
  learner.addQuery(*handle2);
  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(2u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(2u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));

  learner.removeQuery(handle1->query_id());
  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(1u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));

  learner.removeQuery(handle2->query_id());

  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));
}

TEST_F(LearnerTest, MultipleQueriesDifferentPrioritiesAddRemoveTest) {
  Learner learner;
  std::unique_ptr<QueryHandle> handle1, handle2;
  const std::size_t kPriorityLevel1 = 1;
  const std::size_t kPriorityLevel2 = 2;
  handle1.reset(new QueryHandle(1, kPriorityLevel1));
  handle2.reset(new QueryHandle(2, kPriorityLevel2));

  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel1));
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel2));

  learner.addQuery(*handle1);
  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(1u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel1));
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel2));

  learner.addQuery(*handle2);
  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(2u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel1));
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel2));

  learner.removeQuery(handle2->query_id());
  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(1u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel2));
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel1));

  learner.removeQuery(handle1->query_id());
  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel1));
  EXPECT_EQ(0u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel2));
}

TEST_F(LearnerTest, AddCompletionFeedbackSamePriorityLevelTest) {
  Learner learner;
  std::unique_ptr<QueryHandle> handle1, handle2;
  const std::size_t kPriorityLevel = 1;
  handle1.reset(new QueryHandle(1, kPriorityLevel));

  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  learner.addQuery(*handle1);
  serialization::NormalWorkOrderCompletionMessage completion_message =
      createMockCompletionMessage(handle1->query_id(), 0);

  learner.addCompletionFeedback(completion_message);
  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(1u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));

  handle2.reset(new QueryHandle(2, kPriorityLevel));
  learner.addQuery(*handle2);
  completion_message = createMockCompletionMessage(handle2->query_id(), 0);
  learner.addCompletionFeedback(completion_message);

  EXPECT_TRUE(learner.hasActiveQueries());
  EXPECT_EQ(2u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(2u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel));
}

TEST_F(LearnerTest, AddCompletionFeedbackMultiplePriorityLevelsTest) {
  Learner learner;
  std::unique_ptr<QueryHandle> handle1, handle2;
  const std::size_t kPriorityLevel1 = 1;
  const std::size_t kPriorityLevel2 = 2;
  handle1.reset(new QueryHandle(1, kPriorityLevel1));

  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(0u, learner.getTotalNumActiveQueries());
  learner.addQuery(*handle1);

  handle2.reset(new QueryHandle(2, kPriorityLevel2));
  learner.addQuery(*handle2);

  EXPECT_EQ(2u, learner.getTotalNumActiveQueries());
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel1));
  EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel2));

  const std::size_t kNumIterations = 10;
  std::vector<QueryHandle*> handles;
  handles.emplace_back(handle1.get());
  handles.emplace_back(handle2.get());
  for (std::size_t iter_num = 0; iter_num < kNumIterations; ++iter_num) {
    for (std::size_t index = 0; index < handles.size(); ++index) {
      EXPECT_TRUE(learner.hasActiveQueries());
      serialization::NormalWorkOrderCompletionMessage completion_message =
        createMockCompletionMessage(handles[index]->query_id(), 0);
      learner.addCompletionFeedback(completion_message);
      EXPECT_EQ(2u, learner.getTotalNumActiveQueries());
      EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel1));
      EXPECT_EQ(1u, learner.getNumActiveQueriesInPriorityLevel(kPriorityLevel2));
    }
  }
}

TEST_F(LearnerTest, HighestPriorityLevelTest) {
  std::vector<std::size_t> priorities_insertion_order;
  std::vector<std::size_t> priorities_removal_order;
  const std::size_t kNumPrioritiesToTest = 20;
  for (std::size_t priority_num = 1;
       priority_num <= kNumPrioritiesToTest;
       ++priority_num) {
    // Note: Priority level should be non-zero, hence we begin from 1.
    priorities_insertion_order.emplace_back(priority_num);
    priorities_removal_order.emplace_back(priority_num);
  }

  // Randomize the orders.
  std::random_device rd;
  std::mt19937 g1(rd());
  std::mt19937 g2(rd());

  std::shuffle(priorities_insertion_order.begin(),
               priorities_insertion_order.end(),
               g1);

  std::shuffle(priorities_removal_order.begin(),
               priorities_removal_order.end(),
               g2);

  Learner learner;
  EXPECT_EQ(kInvalidPriorityLevel, learner.getHighestPriorityLevel());

  std::unique_ptr<QueryHandle> handle;
  // First insert the queries in the order of priorities as defined by
  // priorities_insertion_order.
  for (auto it = priorities_insertion_order.begin();
       it != priorities_insertion_order.end();
       ++it) {
    // Note that the query ID is kept the same as priority level for simplicity.
    handle.reset(new QueryHandle(*it, *it));
    learner.addQuery(*handle);
    const std::size_t max_priority_so_far =
        *(std::max_element(priorities_insertion_order.begin(), it + 1));
    EXPECT_EQ(static_cast<int>(max_priority_so_far),
              learner.getHighestPriorityLevel());
  }
  // Now remove the queries in the order of priorities as defined by
  // priorities_removal_order.
  for (auto it = priorities_removal_order.begin();
       it != priorities_removal_order.end();
       ++it) {
    // Recall that the query ID is the same as priority level.
    const std::size_t max_priority_so_far =
        *(std::max_element(it, priorities_removal_order.end()));
    EXPECT_EQ(static_cast<int>(max_priority_so_far),
              learner.getHighestPriorityLevel());
    learner.removeQuery(*it);
  }
  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(kInvalidPriorityLevel, learner.getHighestPriorityLevel());
}

TEST_F(LearnerTest, PickRandomPriorityLevelTest) {
  std::vector<std::size_t> priorities_insertion_order;
  std::vector<std::size_t> priorities_removal_order;
  const std::size_t kNumPrioritiesToTest = 20;
  for (std::size_t priority_num = 1;
       priority_num <= kNumPrioritiesToTest;
       ++priority_num) {
    // Note: Priority level should be non-zero, hence we begin from 1.
    priorities_insertion_order.emplace_back(priority_num);
    priorities_removal_order.emplace_back(priority_num);
  }

  // Randomize the orders.
  std::random_device rd;
  std::mt19937 g1(rd());
  std::mt19937 g2(rd());

  std::shuffle(priorities_insertion_order.begin(),
               priorities_insertion_order.end(),
               g1);

  std::shuffle(priorities_removal_order.begin(),
               priorities_removal_order.end(),
               g2);

  Learner learner;
  EXPECT_EQ(kInvalidPriorityLevel, learner.pickRandomPriorityLevel());

  std::unique_ptr<QueryHandle> handle;
  // First insert the queries in the order of priorities as defined by
  // priorities_insertion_order.
  for (auto it = priorities_insertion_order.begin();
       it != priorities_insertion_order.end();
       ++it) {
    // Note that the query ID is kept the same as priority level for simplicity.
    handle.reset(new QueryHandle(*it, *it));
    learner.addQuery(*handle);
    const std::size_t picked_priority_level = learner.pickRandomPriorityLevel();
    // Try to find the randomly picked priority level in the
    // priorities_insertion_order vector.
    auto find_priority_level_it = std::find(
        priorities_insertion_order.begin(), it + 1, picked_priority_level);
    // We expect the search to be successful.
    EXPECT_TRUE(find_priority_level_it != priorities_insertion_order.end());
  }

  // Repeat the tests a few more times.
  const std::size_t kNumTests = 200;
  for (std::size_t test_num = 0; test_num < kNumTests; ++test_num) {
    const std::size_t picked_priority_level = learner.pickRandomPriorityLevel();
    // Try to find the randomly picked priority level in the
    // priorities_insertion_order vector.
    auto find_priority_level_it = std::find(priorities_insertion_order.begin(),
                                            priorities_insertion_order.end(),
                                            picked_priority_level);
    // We expect the search to be successful.
    EXPECT_TRUE(find_priority_level_it != priorities_insertion_order.end());
  }

  // Now remove the queries in the order of priorities as defined by
  // priorities_removal_order.
  for (auto it = priorities_removal_order.begin();
       it != priorities_removal_order.end();
       ++it) {
    // Recall that the query ID is the same as priority level.
    const std::size_t picked_priority_level = learner.pickRandomPriorityLevel();
    // Try to find the randomly picked priority level in the
    // priorities_removal_order vector.
    auto find_priority_level_it = std::find(
        it, priorities_removal_order.end(), picked_priority_level);
    // We expect the search to be successful.
    EXPECT_TRUE(find_priority_level_it != priorities_removal_order.end());
    learner.removeQuery(*it);
  }
  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(kInvalidPriorityLevel, learner.pickRandomPriorityLevel());
}

TEST_F(LearnerTest, PickRandomQueryDefaultProbabilitiesTest) {
  // We use a set of unique query IDs. For each query ID, we assign a priority
  // level. The set of priority levels is smaller than the set of query IDs, so
  // that we can have more than one queries for a given priority level.

  // Also, in this test we don't send any completion feedback message to the
  // learner. Therefore it always refers to the default probabilities set for
  // the queries.
  std::vector<std::size_t> query_ids_insertion_order;
  std::vector<std::size_t> query_ids_removal_order;
  const std::size_t kNumQueriesToTest = 20;
  for (std::size_t query_num = 0;
       query_num < kNumQueriesToTest;
       ++query_num) {
    query_ids_insertion_order.emplace_back(query_num);
    query_ids_removal_order.emplace_back(query_num);
  }

  // Randomize the orders.
  std::random_device rd;
  std::mt19937 g1(rd());
  std::mt19937 g2(rd());

  std::shuffle(query_ids_insertion_order.begin(),
               query_ids_insertion_order.end(),
               g1);

  std::shuffle(query_ids_removal_order.begin(),
               query_ids_removal_order.end(),
               g2);

  Learner learner;
  EXPECT_EQ(kInvalidQueryID, learner.pickRandomQuery());

  std::vector<std::size_t> priority_levels {1, 3, 5, 9};
  std::size_t priority_level_index = 0;
  std::unique_ptr<QueryHandle> handle;
  // Insert the queries in the order as defined in query_ids_insertion_order.
  for (auto it = query_ids_insertion_order.begin();
       it != query_ids_insertion_order.end();
       ++it) {
    handle.reset(new QueryHandle(*it, priority_levels[priority_level_index]));
    priority_level_index = (priority_level_index + 1) % priority_levels.size();
    learner.addQuery(*handle);
    const int picked_query_id = learner.pickRandomQuery();
    // Try to find the randomly picked query ID in query_ids_insertion_order.
    auto find_query_it = std::find(
        query_ids_insertion_order.begin(), it + 1, picked_query_id);
    // We expect the search to be successful.
    EXPECT_TRUE(find_query_it != query_ids_insertion_order.end());
  }

  // Repeat the tests a few more times.
  for (auto it = query_ids_insertion_order.begin();
       it != query_ids_insertion_order.end();
       ++it) {
    const int picked_query_id = learner.pickRandomQuery();
    // Try to find the randomly picked query ID in query_ids_insertion_order.
    auto find_query_it = std::find(query_ids_insertion_order.begin(),
                                   query_ids_insertion_order.end(),
                                   picked_query_id);
    // We expect the search to be successful.
    EXPECT_TRUE(find_query_it != query_ids_insertion_order.end());
  }

  // Remove the queries in the order as defined in query_ids_removal_order.
  for (auto it = query_ids_removal_order.begin();
       it != query_ids_removal_order.end();
       ++it) {
    const int picked_query_id = learner.pickRandomQuery();
    // Try to find the randomly picked query ID in query_ids_removal_order.
    auto find_query_it = std::find(
        it, query_ids_removal_order.end(), picked_query_id);
    // We expect the search to be successful.
    EXPECT_TRUE(find_query_it != query_ids_removal_order.end());
    learner.removeQuery(*it);
  }

  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(kInvalidQueryID, learner.pickRandomQuery());
}

TEST_F(LearnerTest, PickRandomQueryCurrentProbabilitiesTest) {
  // We use a set of unique query IDs. For each query ID, we assign a priority
  // level. The set of priority levels is smaller than the set of query IDs, so
  // that we can have more than one queries for a given priority level.

  // In this test we send completion feedback messages for all the queries
  // to the learner. Therefore it refers to the current probabilities set for
  // the queries.
  std::vector<std::size_t> query_ids_insertion_order;
  std::vector<std::size_t> query_ids_removal_order;
  const std::size_t kNumQueriesToTest = 20;
  for (std::size_t query_num = 0;
       query_num < kNumQueriesToTest;
       ++query_num) {
    query_ids_insertion_order.emplace_back(query_num);
    query_ids_removal_order.emplace_back(query_num);
  }

  // Randomize the orders.
  std::random_device rd;
  std::mt19937 g1(rd());
  std::mt19937 g2(rd());

  std::shuffle(query_ids_insertion_order.begin(),
               query_ids_insertion_order.end(),
               g1);

  std::shuffle(query_ids_removal_order.begin(),
               query_ids_removal_order.end(),
               g2);

  Learner learner;
  EXPECT_EQ(kInvalidQueryID, learner.pickRandomQuery());

  std::vector<std::size_t> priority_levels {1, 3, 5, 9};
  std::size_t priority_level_index = 0;
  std::unique_ptr<QueryHandle> handle;
  // Insert the queries in the order as defined in query_ids_insertion_order.
  for (auto it = query_ids_insertion_order.begin();
       it != query_ids_insertion_order.end();
       ++it) {
    handle.reset(new QueryHandle(*it, priority_levels[priority_level_index]));
    priority_level_index = (priority_level_index + 1) % priority_levels.size();
    learner.addQuery(*handle);
    const int picked_query_id = learner.pickRandomQuery();
    // Try to find the randomly picked query ID in query_ids_insertion_order.
    auto find_query_it = std::find(
        query_ids_insertion_order.begin(), it + 1, picked_query_id);
    // We expect the search to be successful.
    EXPECT_TRUE(find_query_it != query_ids_insertion_order.end());
  }

  // Now send one completion feedback message per query to the learner.
  const std::size_t kOperatorID = 0;
  for (auto it = query_ids_insertion_order.begin();
       it != query_ids_insertion_order.end();
       ++it) {
    // LOG(INFO) << "Completion message for query : " << *it;
    learner.addCompletionFeedback(createMockCompletionMessage(*it, kOperatorID));
  }

  // Repeat the tests a few more times.
  for (auto it = query_ids_insertion_order.begin();
       it != query_ids_insertion_order.end();
       ++it) {
    const int picked_query_id = learner.pickRandomQuery();
    // Try to find the randomly picked query ID in query_ids_insertion_order.
    auto find_query_it = std::find(query_ids_insertion_order.begin(),
                                   query_ids_insertion_order.end(),
                                   picked_query_id);
    // We expect the search to be successful.
    EXPECT_TRUE(find_query_it != query_ids_insertion_order.end());
  }

  // Remove the queries in the order as defined in query_ids_removal_order.
  for (auto it = query_ids_removal_order.begin();
       it != query_ids_removal_order.end();
       ++it) {
    const int picked_query_id = learner.pickRandomQuery();
    // Try to find the randomly picked query ID in query_ids_removal_order.
    auto find_query_it = std::find(
        it, query_ids_removal_order.end(), picked_query_id);
    // We expect the search to be successful.
    // LOG(INFO) << "Picked query ID: " << picked_query_id << "\n";
    EXPECT_TRUE(find_query_it != query_ids_removal_order.end());
    learner.removeQuery(*it);
    // LOG(INFO) << "Removed query ID: " << *it;
  }

  EXPECT_FALSE(learner.hasActiveQueries());
  EXPECT_EQ(kInvalidQueryID, learner.pickRandomQuery());
}
}  // namespace quickstep
