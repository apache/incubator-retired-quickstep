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
  EXPECT_EQ(transaction_table_.putOwnEntry(tid_1_,
                                           ResourceId(3),
                                           AccessMode(AccessModeType::kIsLock)),
            TransactionTableResult::kPlacedInOwned);

  EXPECT_EQ(transaction_table_.putPendingEntry(tid_1_,
                                               ResourceId(5),
                                               AccessMode(AccessModeType::kXLock)),
            TransactionTableResult::kPlacedInPending);
}

TEST_F(TransactionTableTest, DeleteEntryOperations) {
  EXPECT_EQ(transaction_table_.deleteOwnEntry(tid_2_,
                                              ResourceId(5),
                                              AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kDelError);

  EXPECT_EQ(transaction_table_.putOwnEntry(tid_2_,
                                           ResourceId(5),
                                           AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kPlacedInOwned);

  // Tring to delete a lock with different acces mode on same resource id
  // will result in an error.
  EXPECT_EQ(transaction_table_.deleteOwnEntry(tid_2_,
                                              ResourceId(5),
                                              AccessMode(AccessModeType::kXLock)),
            TransactionTableResult::kDelError);

  // Transaction 3 does not have a lock on this resource id.
  EXPECT_EQ(transaction_table_.deleteOwnEntry(tid_3_,
                                              ResourceId(5),
                                              AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kDelError);

  // This will result in success since transaction 2 have acquired the lock on
  // this resource with the corresponding mode.
  EXPECT_EQ(transaction_table_.deleteOwnEntry(tid_2_,
                                              ResourceId(5),
                                              AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kDelFromOwned);

  // Repeat the previous sequence, with pending list.
  EXPECT_EQ(transaction_table_.deletePendingEntry(tid_2_,
                                                  ResourceId(5),
                                                  AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kDelError);

  EXPECT_EQ(transaction_table_.putPendingEntry(tid_2_,
                                               ResourceId(5),
                                               AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kPlacedInPending);

  EXPECT_EQ(transaction_table_.deletePendingEntry(tid_2_,
                                                  ResourceId(5),
                                                  AccessMode(AccessModeType::kXLock)),
            TransactionTableResult::kDelError);

  EXPECT_EQ(transaction_table_.deletePendingEntry(tid_3_,
                                                  ResourceId(5),
                                                  AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kDelError);

  EXPECT_EQ(transaction_table_.deletePendingEntry(tid_2_,
                                                  ResourceId(5),
                                                  AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kDelFromPending);
}

TEST_F(TransactionTableTest, TransactionEntries) {
  EXPECT_EQ(transaction_table_.deleteTransaction(tid_1_),
            TransactionTableResult::kTransactionDeleteError);

  EXPECT_EQ(transaction_table_.putOwnEntry(tid_1_,
                                           ResourceId(4),
                                           AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kPlacedInOwned);

  EXPECT_EQ(transaction_table_.deleteTransaction(tid_1_),
            TransactionTableResult::kTransactionDeleteOk);

  EXPECT_EQ(transaction_table_.deleteOwnEntry(tid_1_,
                                              ResourceId(4),
                                              AccessMode(AccessModeType::kSLock)),
            TransactionTableResult::kDelError);
}

}  // namespace transaction
}  // namespace quickstep
