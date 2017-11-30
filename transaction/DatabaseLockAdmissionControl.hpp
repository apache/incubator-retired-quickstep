/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_TRANSACTION_DATABASE_LOCK_ADMISSION_CONTROL_HPP_
#define QUICKSTEP_TRANSACTION_DATABASE_LOCK_ADMISSION_CONTROL_HPP_

#include <queue>
#include <utility>
#include <vector>

#include "transaction/AdmissionControl.hpp"
#include "utility/Macros.hpp"
#include "utility/ThreadSafeQueue.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace transaction {

/**
 * @brief An admission control mechanism that grabs an exclusive lock on the
 *        entire database for any query.
 *
 */
class DatabaseLockAdmissionControl : public AdmissionControl {
 public:
  DatabaseLockAdmissionControl() : AdmissionControl() {
  }

  ~DatabaseLockAdmissionControl() override {
  }

  bool admitTransaction(const transaction_id tid,
                        const std::vector<std::pair<ResourceId, AccessMode>> &resource_requests) override {
    waiting_transactions_.push(tid);
    if (waiting_transactions_.size() == 1u) {
      // Don't bother about the requested resource IDs and their access modes,
      // as we always lock the CatalogDatabase.
      // The system currently supports only one CatalogDatabase.
      return true;
    }
    // Other transactions are waitlisted ahead of the current transaction.
    return false;
  }

  bool admitWaitingTransaction(const transaction_id tid) override {
    // TODO(harshad) - Safety check if the transaction is already waiting.
    waiting_transactions_.push(tid);
    return true;
  }

  void signalTransactionCompletion(const transaction_id tid) {
    // TODO(harshad) - Safety check if the transaction was indeed admitted and
    // was at the front of the queue.
    waiting_transactions_.popOne();
  }

  std::size_t getTotalTransactionsCount() const {
    return waiting_transactions_.size();
  }

 private:
  ThreadSafeQueue<transaction_id> waiting_transactions_;

  DISALLOW_COPY_AND_ASSIGN(DatabaseLockAdmissionControl);
};

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_DATABASE_LOCK_ADMISSION_CONTROL_HPP_
