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

#include "transaction/Lock.hpp"

#include <cstddef>
#include <vector>

#include "transaction/AccessMode.hpp"
#include "transaction/ResourceId.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace transaction {

class LockTest : public ::testing::Test {
 protected:
  LockTest()
      : modes_({AccessMode(AccessModeType::kNoLock),
                AccessMode(AccessModeType::kIsLock),
                AccessMode(AccessModeType::kIxLock),
                AccessMode(AccessModeType::kSLock),
                AccessMode(AccessModeType::kSixLock),
                AccessMode(AccessModeType::kXLock)}),
        resource_a_(3, 10, 2, 5),
        resource_b_(4, 5, 3, 2),
        locks_on_resource_a_({Lock(resource_a_, modes_[0]),
                              Lock(resource_a_, modes_[1]),
                              Lock(resource_a_, modes_[2]),
                              Lock(resource_a_, modes_[3]),
                              Lock(resource_a_, modes_[4]),
                              Lock(resource_a_, modes_[5])}),
        locks_on_resource_b_({Lock(resource_b_, modes_[0]),
                             Lock(resource_b_, modes_[1]),
                             Lock(resource_b_, modes_[2]),
                             Lock(resource_b_, modes_[3]),
                             Lock(resource_b_, modes_[4]),
                             Lock(resource_b_, modes_[5])}) {
  }

  const std::vector<AccessMode> modes_;
  const ResourceId resource_a_;
  const ResourceId resource_b_;
  const std::vector<Lock> locks_on_resource_a_;
  const std::vector<Lock> locks_on_resource_b_;
};

TEST_F(LockTest, LockEquality) {
  // Locks are equal if they are on the same resource with same
  // access mode.
  for (std::size_t i = 0; i < locks_on_resource_a_.size(); ++i) {
    for (std::size_t j = 0; j < locks_on_resource_a_.size(); ++j) {
      if (i == j) {
        EXPECT_EQ(locks_on_resource_a_[i], locks_on_resource_a_[j]);
      } else {
        EXPECT_NE(locks_on_resource_a_[i], locks_on_resource_a_[j]);
      }
    }
    // Locks are nver equal if they have different resource ids.
    EXPECT_NE(locks_on_resource_a_[i], locks_on_resource_b_[i]);
  }
}

TEST_F(LockTest, GetResourceId) {
  for (std::size_t i = 0; i < locks_on_resource_a_.size(); ++i) {
    EXPECT_EQ(resource_a_, locks_on_resource_a_[i].getResourceId());
  }
}

TEST_F(LockTest, GetAccessMode) {
  for (std::size_t i = 0; i < locks_on_resource_a_.size(); ++i) {
    EXPECT_EQ(modes_[i], locks_on_resource_a_[i].getAccessMode());
  }
}

}  // namespace transaction
}  // namespace quickstep
