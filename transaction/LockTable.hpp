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

#ifndef QUICKSTEP_TRANSACTION_LOCK_TABLE_HPP_
#define QUICKSTEP_TRANSACTION_LOCK_TABLE_HPP_

#include <list>
#include <unordered_map>
#include <utility>

#include "threading/SharedMutex.hpp"
#include "transaction/Lock.hpp"
#include "transaction/ResourceId.hpp"
#include "transaction/Transaction.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace transaction {

class AccessMode;

/** \addtogroup Transaction
 * @{
 */

/**
 * @brief Represents different results for LockTable's methods.
 **/
enum class LockTableResult {
  kPlacedInOwned = 0,
  kPlacedInPending,
  kAlreadyInOwned,
  kAlreadyInPending,
  kDeleteFromOwned,
  kDeleteFromPending,
  kDeleteError,
  kPutError,
};

/**
 * @brief LockTable class represents the hash map for RID and
 *        list of locks on RID.
 **/
class LockTable {
 public:
  typedef std::pair<transaction_id, Lock> lock_entry;
  typedef std::list<lock_entry> lock_own_list;
  typedef std::list<lock_entry> lock_pending_list;
  typedef std::pair<lock_own_list, lock_pending_list> lock_list_pair;
  typedef std::unordered_map<ResourceId,
                             lock_list_pair,
                             ResourceId::ResourceIdHasher> internal_map_type;
  typedef internal_map_type::iterator iterator;
  typedef internal_map_type::const_iterator const_iterator;

  /**
   * @brief Constructor for LockTable.
   **/
  LockTable() {
  }

  /**
   * @brief Puts the lock entry into the lock table for corresponding resource.
   *
   * @param tid Id of the transaction that requests the lock.
   * @param rid Id of the resource to be locked.
   * @param access_mode Access mode of the lock.
   *
   * @return LockTableResult::kPlacedInOwned if lock is granted,
   *         LockTableResult::kPlacedInPending if lock is not granted,
   *         LockTableResult::kAlreadyInOwned if lock has been
   *         already granted,
   *         LockTableResult::kAlreadyInPending if lock has been
   *         already pending.
   **/
  LockTableResult putLock(const transaction_id tid,
                          const ResourceId &rid,
                          const AccessMode &access_mode);
  /**
   * @brief Deletes the lock entry.
   *
   * @param tid Id of the transaction that owns or awaits.
   * @param rid Id of resource that the lock covers.
   *
   * @return LockTableResult::kDelFromOwned if the lock is deleted from
   *         owned list,
   *         LockTableResult::kDelFromPending if the lock is deleted from
   *         pending list,
   *         LockTableResult::kDelError if the lock cannot be found
   **/
  LockTableResult deleteLock(const transaction_id tid,
                             const ResourceId &rid);

  /**
   * @brief Iterator for begin position.
   *
   * @return Non-const Iterator which points to begin point
   *         of the lock table.
   **/
  iterator begin();

  /**
   * @brief Iterator for end position.
   *
   * @return Non-const iterator which points to end point
   *         of the lock table.
   **/
  iterator end();

  /**
   * @brief Iterator for begin position.
   *
   * @return Const iterator which points to the begin
   *         point of the lock table.
   **/
  const_iterator begin() const;

  /**
   * @brief Iterator for end position.
   *
   * @return Const iterator which points to the end
   *         point of the lock table.
   **/
  const_iterator end() const;

  /**
   * @brief Latch mutex in shared mode. Multiple shared mode
   *        latch acquisition is compatible.
   **/
  void latchShared();

  /**
   * @brief Unlatch mutex in shared mode.
   **/
  void unlatchShared();

  /**
   * @brief Latch mutex in exclusive mode.
   */
  void latchExclusive();

  /**
   * @brief Unlatch mutex in exclusive mode.
   */
  void unlatchExclusive();

 private:
  // This method will be called after deletion of locks.
  // After delete, some pending locks might be acquired.
  void movePendingToOwned(const ResourceId &rid);

  internal_map_type internal_map_;

  // Mutex protects whole lock table.
  SharedMutex mutex_;

  DISALLOW_COPY_AND_ASSIGN(LockTable);
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif
