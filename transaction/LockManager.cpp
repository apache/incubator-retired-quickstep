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

#include "transaction/LockManager.hpp"

#include <cstdint>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "transaction/AccessMode.hpp"
#include "transaction/DeadLockDetector.hpp"
#include "transaction/LockRequest.hpp"
#include "transaction/LockTable.hpp"
#include "transaction/ResourceId.hpp"
#include "transaction/Transaction.hpp"
#include "transaction/TransactionTable.hpp"
#include "utility/ThreadSafeQueue.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {
namespace transaction {

DEFINE_uint64(max_try_incoming, 10000,
              "The maximum number of tries that lock manager checks incoming "
              "request buffer until the buffer is empty.");

DEFINE_uint64(max_try_inner, 6000,
              "The maximum number of tries that lock manager checks inner "
              "request buffer until the buffer is empty.");

LockManager::LockManager(ThreadSafeQueue<LockRequest> *incoming_requests,
                         ThreadSafeQueue<LockRequest> *permitted_requests)
    : lock_table_(std::make_unique<LockTable>()),
      transaction_table_(std::make_unique<TransactionTable>()),
      detector_status_(DeadLockDetectorStatus::kNotReady),
      deadlock_detector_(std::make_unique<DeadLockDetector>(lock_table_.get(),
                                                            &detector_status_,
                                                            &victim_result_)),
      incoming_requests_(*incoming_requests),
      permitted_requests_(*permitted_requests),
      inner_pending_requests_() {
}

LockManager::~LockManager() {
  deadlock_detector_->join();
}

void LockManager::run() {
  deadlock_detector_->start();

  const std::uint64_t kMaxTryIncoming =
      static_cast<std::uint64_t>(FLAGS_max_try_incoming);
  const std::uint64_t kMaxTryInner =
      static_cast<std::uint64_t>(FLAGS_max_try_incoming);

  while (true) {
    for (std::uint64_t tries = 0; tries < kMaxTryIncoming; ++tries) {
      if (!incoming_requests_.empty()) {
        const LockRequest request = incoming_requests_.popOne();
        if (request.getRequestType() == RequestType::kReleaseLocks) {
          CHECK(releaseAllLocks(request.getTransactionId()))
              << "Unexpected condition occured.";
        } else if (acquireLock(request.getTransactionId(),
                               request.getResourceId(),
                               request.getAccessMode())) {
          // Lock has been acquired.
          LOG(INFO) << "Transaction "
                    << std::to_string(request.getTransactionId())
                    << " acquired " + request.getResourceId().toString();

          permitted_requests_.push(request);
        } else {
          // We are unable to acquire lock at this point.
          LOG(INFO) << "Transaction "
                    << std::to_string(request.getTransactionId())
                    << " is waiting " + request.getResourceId().toString();

          inner_pending_requests_.push(request);
        }
      }
    }

    for (std::uint64_t tries = 0; tries < kMaxTryInner; ++tries) {
      if (!inner_pending_requests_.empty()) {
        const LockRequest request = inner_pending_requests_.front();

        if (acquireLock(request.getTransactionId(), request.getResourceId(),
                        request.getAccessMode())) {
          inner_pending_requests_.pop();
          permitted_requests_.push(request);
        }
      }
    }

    // Resolve deadlocks.
    killVictims();
  }
}

bool LockManager::acquireLock(const transaction_id tid,
                              const ResourceId &rid,
                              const AccessMode &access_mode) {
  std::stack<std::pair<ResourceId, AccessMode>> stack;
  ResourceId current_rid = rid;
  AccessMode current_access_mode = access_mode;
  stack.push(std::make_pair(current_rid, current_access_mode));

  while (current_rid.hasParent()) {
    current_rid = current_rid.getParentResourceId();
    current_access_mode = (current_access_mode.isShareLock() ||
                           current_access_mode.isIntentionShareLock())
                              ? AccessMode(AccessMode::IsLockMode())
                              : AccessMode(AccessMode::IxLockMode());

    stack.push(std::make_pair(current_rid, current_access_mode));
  }

  lock_table_->latchExclusive();

  while (!stack.empty()) {
    const std::pair<ResourceId, AccessMode> pair_to_pick = stack.top();
    const ResourceId rid_to_pick = pair_to_pick.first;
    const AccessMode access_mode_to_pick = pair_to_pick.second;

    if (!acquireLockInternal(tid, rid_to_pick, access_mode_to_pick)) {
      lock_table_->unlatchExclusive();
      return false;
    }
    stack.pop();
  }
  lock_table_->unlatchExclusive();
  return true;
}

bool LockManager::releaseAllLocks(const transaction_id tid,
                                  const bool latch_table) {
  const std::vector<ResourceId> resource_ids
      = transaction_table_->getResourceIdList(tid);
  const TransactionTableResult transaction_deleted
      = transaction_table_->deleteTransaction(tid);

  CHECK(transaction_deleted != TransactionTableResult::kTransactionDeleteError)
      << "In LockManager.releaseAllLocks: Transaction could not be deleted!";

  if (latch_table) {
    lock_table_->latchExclusive();
  }
  for (const auto &resource_id : resource_ids) {
    const LockTableResult lock_deleted = lock_table_->deleteLock(tid, resource_id);

    LOG(INFO) << "Transaction "
              << std::to_string(tid)
              << " released lock:"
              << resource_id.toString();
    CHECK(lock_deleted != LockTableResult::kDeleteError)
        << "In LockManager.releaseAllLock lock could not be deleted from "
           "LockTable";
  }
  if (latch_table) {
    lock_table_->unlatchExclusive();
  }
  return true;
}

bool LockManager::acquireLockInternal(const transaction_id tid,
                                      const ResourceId &rid,
                                      const AccessMode &access_mode) {
  const LockTableResult lock_result = lock_table_->putLock(tid, rid, access_mode);
  CHECK(lock_result != LockTableResult::kPutError)
      << "Unexpected result in LockManager.acquireLockInternal";

  switch (lock_result) {
  case LockTableResult::kAlreadyInOwned: {
    return true;
  }
  case LockTableResult::kPlacedInOwned: {
    const TransactionTableResult transaction_result
        = transaction_table_->putOwnEntry(tid, rid, access_mode);
    CHECK(transaction_result == TransactionTableResult::kPlacedInOwned)
        << "Unexpected result in LockManager.acquireLockInternal: "
           "Mismatch of table results: LockTable entry is owned, "
           "whereas TransactionTable entry is not owned.";
    return true;
  }
  case LockTableResult::kAlreadyInPending: {
    return false;
  }
  case LockTableResult::kPlacedInPending: {
    const TransactionTableResult transaction_result =
      transaction_table_->putPendingEntry(tid, rid, access_mode);
    CHECK(transaction_result == TransactionTableResult::kPlacedInPending)
        << "Unexpected result in LockManager.acquireLockInternal: "
           "Mismatch of table results: LockTable entry is pending, "
           "whereas TransactionTable entry is not pending";
    return false;
  }
  default: {
    return false;
  }
  }
}

void LockManager::killVictims() {
  if (detector_status_.load() == DeadLockDetectorStatus::kDone) {
    lock_table_->latchExclusive();
    for (const auto victim_transaction_id : victim_result_) {
      releaseAllLocks(victim_transaction_id, false);
      // TODO(Hakan): Find a way to kill transaction, so that requests with this
      //              tid should be ignored.
      LOG(INFO) << "Killed transaction "
                << std::to_string(victim_transaction_id);
    }
    lock_table_->unlatchExclusive();
  }
  victim_result_.clear();
  detector_status_.store(DeadLockDetectorStatus::kNotReady);
}

}  // namespace transaction
}  // namespace quickstep
