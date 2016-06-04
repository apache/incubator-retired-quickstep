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

#include "transaction/TransactionTable.hpp"

#include "transaction/AccessMode.hpp"
#include "transaction/ResourceId.hpp"
#include "transaction/Transaction.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace transaction {

class TransactionTableTest : public ::testing::Test {
 protected:
  TransactionTableTest()
      : tid_1_(1),
        tid_2_(2),
        tid_3_(3) {
    }

  TransactionTable transaction_table_;
  const transaction_id tid_1_;
  const transaction_id tid_2_;
  const transaction_id tid_3_;
};

TEST_F(TransactionTableTest, NormalOperations) {
  const AccessMode is_lock_mode = AccessMode::IsLockMode();
  const AccessMode x_lock_mode = AccessMode::XLockMode();

  EXPECT_EQ(TransactionTableResult::kPlacedInOwned,
            transaction_table_.putOwnEntry(tid_1_,
                                           ResourceId(3),
                                           is_lock_mode));


  EXPECT_EQ(TransactionTableResult::kPlacedInPending,
            transaction_table_.putPendingEntry(tid_1_,
                                               ResourceId(5),
                                               x_lock_mode));
}

TEST_F(TransactionTableTest, DeleteEntryOperations) {
  const AccessMode s_lock_mode = AccessMode::SLockMode();
  const AccessMode x_lock_mode = AccessMode::XLockMode();

  EXPECT_EQ(TransactionTableResult::kDelError,
            transaction_table_.deleteOwnEntry(tid_2_,
                                              ResourceId(5),
                                              s_lock_mode));

  EXPECT_EQ(TransactionTableResult::kPlacedInOwned,
            transaction_table_.putOwnEntry(tid_2_,
                                           ResourceId(5),
                                           s_lock_mode));

  // Tring to delete a lock with different acces mode on same resource id
  // will result in an error.
  EXPECT_EQ(TransactionTableResult::kDelError,
            transaction_table_.deleteOwnEntry(tid_2_,
                                              ResourceId(5),
                                              x_lock_mode));

  // Transaction 3 does not have a lock on this resource id.
  EXPECT_EQ(TransactionTableResult::kDelError,
            transaction_table_.deleteOwnEntry(tid_3_,
                                              ResourceId(5),
                                              s_lock_mode));

  // This will result in success since transaction 2 have acquired the lock on
  // this resource with the corresponding mode.
  EXPECT_EQ(TransactionTableResult::kDelFromOwned,
            transaction_table_.deleteOwnEntry(tid_2_,
                                              ResourceId(5),
                                              s_lock_mode));

  // Repeat the previous sequence, with pending list.
  EXPECT_EQ(TransactionTableResult::kDelError,
            transaction_table_.deletePendingEntry(tid_2_,
                                                  ResourceId(5),
                                                  s_lock_mode));

  EXPECT_EQ(TransactionTableResult::kPlacedInPending,
            transaction_table_.putPendingEntry(tid_2_,
                                               ResourceId(5),
                                               s_lock_mode));


  EXPECT_EQ(TransactionTableResult::kDelError,
            transaction_table_.deletePendingEntry(tid_2_,
                                                  ResourceId(5),
                                                  x_lock_mode));

  EXPECT_EQ(TransactionTableResult::kDelError,
            transaction_table_.deletePendingEntry(tid_3_,
                                                  ResourceId(5),
                                                  s_lock_mode));

  EXPECT_EQ(TransactionTableResult::kDelFromPending,
            transaction_table_.deletePendingEntry(tid_2_,
                                                  ResourceId(5),
                                                  s_lock_mode));
}

TEST_F(TransactionTableTest, TransactionEntries) {
  const AccessMode s_lock_mode = AccessMode::SLockMode();

  EXPECT_EQ(TransactionTableResult::kTransactionDeleteError,
            transaction_table_.deleteTransaction(tid_1_));

  EXPECT_EQ(TransactionTableResult::kPlacedInOwned,
            transaction_table_.putOwnEntry(tid_1_,
                                           ResourceId(4),
                                           s_lock_mode));

  EXPECT_EQ(TransactionTableResult::kTransactionDeleteOk,
            transaction_table_.deleteTransaction(tid_1_));

  EXPECT_EQ(TransactionTableResult::kDelError,
            transaction_table_.deleteOwnEntry(tid_1_,
                                              ResourceId(4),
                                              s_lock_mode));
}

}  // namespace transaction
}  // namespace quickstep
