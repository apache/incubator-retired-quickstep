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

#include "transaction/LockRequest.hpp"

#include "transaction/AccessMode.hpp"
#include "transaction/ResourceId.hpp"
#include "transaction/Transaction.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace transaction {

class LockRequestTest : public ::testing::Test {
 protected:
  LockRequestTest()
      : lock_request_(transaction_id(3),
                      ResourceId(5),
                      AccessMode(AccessModeType::kSLock),
                      RequestType::kAcquireLock) {
  }

  const LockRequest lock_request_;
};

TEST_F(LockRequestTest, CheckGetters) {
  EXPECT_EQ(transaction_id(3), lock_request_.getTransactionId());
  EXPECT_EQ(ResourceId(5), lock_request_.getResourceId());
  EXPECT_EQ(AccessMode(AccessModeType::kSLock), lock_request_.getAccessMode());
  EXPECT_EQ(RequestType::kAcquireLock, lock_request_.getRequestType());
}

}  // namespace transaction
}  // namespace quickstep
