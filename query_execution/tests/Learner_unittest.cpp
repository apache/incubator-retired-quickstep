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

#include <memory>

#include "gtest/gtest.h"

#include "query_execution/Learner.hpp"
#include "query_optimizer/QueryHandle.hpp"

namespace quickstep {

TEST(LearnerTest, AddAndRemoveQueryTest) {
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

TEST(LearnerTest, MultipleQueriesSamePriorityAddRemoveTest) {
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

TEST(LearnerTest, MultipleQueriesDifferentPrioritiesAddRemoveTest) {
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

}  // namespace quickstep
