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

#ifndef QUICKSTEP_TRANSACTION_LOCK_MANAGER_HPP_
#define QUICKSTEP_TRANSACTION_LOCK_MANAGER_HPP_

#include <atomic>
#include <memory>
#include <queue>
#include <vector>

#include "threading/Thread.hpp"
#include "transaction/DeadLockDetector.hpp"
#include "transaction/LockRequest.hpp"
#include "transaction/Transaction.hpp"

namespace quickstep {

template <typename T> class ThreadSafeQueue;

namespace transaction {

class AccessMode;
class LockTable;
class ResourceId;
class TransactionTable;

/** \addtogroup Transaction
 *  @{
 **/

/**
 * @brief Class for centralized location of acquisition and releasing
 *        of resource locks.
 **/
class LockManager : public Thread {
 public:
  /**
   * @brief Constructor
   *
   * @param incoming_requests Queue for the lock requests that are waiting
   *        for the permission.
   * @param permitted_requests Queue for the lock requests that are granted
   *        permission.
   **/
  LockManager(ThreadSafeQueue<LockRequest> *incoming_requests,
              ThreadSafeQueue<LockRequest> *permitted_requests);

  /**
   * @brief Destructor for LockManager. It handles the thread
   *        joins that it owns.
   **/
  ~LockManager();

  /**
   * @brief Method for defining the LockManager's thread main logic.
   *
   * @warning Users must not use this function directly. Instead use
   *          start() method inherited from the Thread class.
   **/
  void run() override;

  /**
   * @brief Acquires the lock on resource with specified access mode.
   *
   * @param tid Id of the transaction which the resource lock is acquired for.
   * @param rid Id of the resource on which the resource lock is acquired.
   * @param access_mode Permissible access mode on resource.
   *
   * @return True if it can acquire the lock from root to leaf lock hierarchy,
   *         false otherwise.
   **/
  bool acquireLock(const transaction_id tid,
                   const ResourceId &rid,
                   const AccessMode &access_mode);

  /**
   * @brief Releases all locks hold by the transaction.
   *
   * @param tid Id of the transaction whose locks will be released.
   * @param latch_table If it is true, the method latch the whole
   *        lock table, which is default.
   * @return True if tid releases all of its locks.
   **/
  bool releaseAllLocks(const transaction_id tid,
                       const bool latch_table = true);

  /**
   * @brief Release the locks acquired by the transactions contained
   *        in victim buffer to break the deadlock.
   **/
  void killVictims();

 private:
  bool acquireLockInternal(const transaction_id tid,
                           const ResourceId &rid,
                           const AccessMode &access_mode);

  std::unique_ptr<LockTable> lock_table_;
  std::unique_ptr<TransactionTable> transaction_table_;
  std::atomic<DeadLockDetectorStatus> detector_status_;
  std::vector<transaction_id> victim_result_;
  std::unique_ptr<DeadLockDetector> deadlock_detector_;
  ThreadSafeQueue<LockRequest> &incoming_requests_;
  ThreadSafeQueue<LockRequest> &permitted_requests_;
  std::queue<LockRequest> inner_pending_requests_;
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_LOCK_MANAGER_HPP_
