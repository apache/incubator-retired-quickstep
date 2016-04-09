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

#include "transaction/LockTable.hpp"

#include "transaction/AccessMode.hpp"
#include "transaction/ResourceId.hpp"
#include "transaction/Transaction.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace transaction {

class LockTableTest : public ::testing::Test {
 protected:
  LockTableTest()
      : tid_1_(1),
        tid_2_(2),
        tid_3_(3) {
  }

  LockTable lock_table_;
  const transaction_id tid_1_;
  const transaction_id tid_2_;
  const transaction_id tid_3_;
};

TEST_F(LockTableTest, CompatibleRequestsFromDifferentTransactions) {
  EXPECT_EQ(lock_table_.putLock(tid_1_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kIsLock)),
            LockTableResult::kPLACED_IN_OWNED);

  // Acquire the same lock mode on same resource.
  EXPECT_EQ(lock_table_.putLock(tid_1_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kIsLock)),
            LockTableResult::kALREADY_IN_OWNED);

  // Another transaction acquires compatible lock on the same resource.
  EXPECT_EQ(lock_table_.putLock(tid_2_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kSLock)),
            LockTableResult::kPLACED_IN_OWNED);
}

TEST_F(LockTableTest, IncompatibleRequestsFromDifferentTransactions) {
  EXPECT_EQ(lock_table_.putLock(tid_1_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kIsLock)),
            LockTableResult::kPLACED_IN_OWNED);

  // Acquire the same lock mode on same resource.
  EXPECT_EQ(lock_table_.putLock(tid_1_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kIsLock)),
            LockTableResult::kALREADY_IN_OWNED);

  // Another transaction acquires incompatible lock on the same resource.
  EXPECT_EQ(lock_table_.putLock(tid_2_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kXLock)),
            LockTableResult::kPLACED_IN_PENDING);
}

TEST_F(LockTableTest, StarvationProtection) {
  EXPECT_EQ(lock_table_.putLock(tid_1_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kIsLock)),
            LockTableResult::kPLACED_IN_OWNED);

  // Another transaction requests incompatible lock on the same resource.
  // It should wait for the previous transaction.
  EXPECT_EQ(lock_table_.putLock(tid_2_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kXLock)),
            LockTableResult::kPLACED_IN_PENDING);

  // Another third transaction requests a compatible lock on the same resource.
  // Normally, it should acquire the lock, however, there is a pending
  // transaction waiting on the same resource. To prevent starvation, we should
  // put in the pending list.
  EXPECT_EQ(lock_table_.putLock(tid_3_,
                                ResourceId(2),
                                AccessMode(AccessModeType::kIsLock)),
            LockTableResult::kPLACED_IN_PENDING);
}

}  // namespace transaction
}  // namespace quickstep
