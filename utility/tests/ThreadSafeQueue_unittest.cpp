/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
