/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
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

// TODO(chasseur): This test is FAR from complete. For now it just does the
// bare minimum of making sure a ThreadSafeQueue can be created and
// pushed/popped from a single thread.

#include "gtest/gtest.h"
#include "utility/ThreadSafeQueue.hpp"

namespace quickstep {

TEST(ThreadSafeQueueTest, IntPushPopTest) {
  ThreadSafeQueue<int> int_queue;
  EXPECT_TRUE(int_queue.empty());
  EXPECT_EQ(0u, int_queue.size());

  int pop_destination = 0;
  EXPECT_FALSE(int_queue.popOneIfAvailable(&pop_destination));
  EXPECT_EQ(0, pop_destination);

  int_queue.push(42);
  EXPECT_FALSE(int_queue.empty());
  EXPECT_EQ(1u, int_queue.size());

  int_queue.push(2001);
  EXPECT_FALSE(int_queue.empty());
  EXPECT_EQ(2u, int_queue.size());

  int popped = int_queue.popOne();
  EXPECT_EQ(42, popped);
  EXPECT_FALSE(int_queue.empty());
  EXPECT_EQ(1u, int_queue.size());

  EXPECT_TRUE(int_queue.popOneIfAvailable(&pop_destination));
  EXPECT_EQ(2001, pop_destination);
  EXPECT_TRUE(int_queue.empty());
  EXPECT_EQ(0u, int_queue.size());
}

}  // namespace quickstep
