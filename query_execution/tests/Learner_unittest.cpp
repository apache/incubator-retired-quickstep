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

TEST(LearnerTest, AddQueryTest) {
  Learner learner;
  std::unique_ptr<QueryHandle> handle;
  handle.reset(new QueryHandle(1, 1));

  EXPECT_FALSE(learner.hasActiveQueries());
  learner.addQuery(*handle);
  EXPECT_TRUE(learner.hasActiveQueries());
}

TEST(LearnerTest, RemoveQueryTest) {
  Learner learner;
  std::unique_ptr<QueryHandle> handle;
  handle.reset(new QueryHandle(1, 1));

  EXPECT_FALSE(learner.hasActiveQueries());
  learner.addQuery(*handle);
  EXPECT_TRUE(learner.hasActiveQueries());
  learner.removeQuery(handle->query_id());
  EXPECT_FALSE(learner.hasActiveQueries());

  handle.reset(new QueryHandle(2, 1));
  learner.addQuery(*handle);
  EXPECT_TRUE(learner.hasActiveQueries());
  learner.removeQuery(handle->query_id());
  EXPECT_FALSE(learner.hasActiveQueries());
}

}  // namespace quickstep
