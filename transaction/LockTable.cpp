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

#include <list>
#include <utility>

#include "transaction/AccessMode.hpp"
#include "transaction/Lock.hpp"
#include "transaction/Transaction.hpp"

namespace quickstep {
namespace transaction {

LockTableResult
LockTable::putLock(const transaction_id tid,
                   const ResourceId &rid,
                   const AccessMode &access_mode) {
  // TODO(hakan): Lock upgrade is not supported.
  lock_list_pair &lock_list_pair = internal_map_[rid];

  // Each resource id entry has own list and pending list.
  lock_own_list &lock_own_list = lock_list_pair.first;
  lock_pending_list &lock_pending_list = lock_list_pair.second;

  // Check this resource id already has the same lock request from
  // the same transaction in the own list.
  for (lock_own_list::const_iterator it = lock_own_list.cbegin();
       it != lock_own_list.cend(); ++it) {
    if (it->first == tid && it->second.getAccessMode() == access_mode) {
      return LockTableResult::kAlreadyInOwned;
    }
  }

  // Check this resource id already has the same lock request from
  // the same transaction in the pending list.
  for (lock_pending_list::const_iterator it = lock_pending_list.cbegin();
       it != lock_pending_list.cend(); ++it) {
    if (it->first == tid && it->second.getAccessMode() == access_mode) {
      return LockTableResult::kAlreadyInPending;
    }
  }

  // If the execution can reach this point, it means the resource id
  // does not have duplicate lock record (for both in owned and pending).
  if (lock_pending_list.empty()) {
    for (lock_own_list::const_iterator it = lock_own_list.cbegin();
         it != lock_own_list.cend(); ++it) {
      if (!access_mode.isCompatible(it->second.getAccessMode())) {
        lock_pending_list.push_back(std::make_pair(tid,
                                                   Lock(rid, access_mode)));
        return LockTableResult::kPlacedInPending;
      }
    }

    lock_own_list.push_back(std::make_pair(tid, Lock(rid, access_mode)));
    return LockTableResult::kPlacedInOwned;
  } else {
    // If the pending list is not empty, even if the lock request is compatible
    // with other owned lock entries, we put the new request into the pending
    // list to eliminate starvation.
    lock_pending_list.push_back(std::make_pair(tid, Lock(rid, access_mode)));
    return LockTableResult::kPlacedInPending;
  }
}

LockTableResult
LockTable::deleteLock(const transaction_id tid,
                      const ResourceId &rid) {
  lock_list_pair &lock_list_pair = internal_map_[rid];

  // Each resource id has its own and pending locks list.
  lock_own_list &lock_own_list = lock_list_pair.first;
  lock_pending_list &lock_pending_list = lock_list_pair.second;

  // Iterate over owned locks list to see the lock entry of the transaction
  // on the resource id exists.
  for (lock_own_list::const_iterator it = lock_own_list.begin();
       it != lock_own_list.cend(); ++it) {
    if (it->first == tid) {
      // If it exists, erase it from the owned list.
      lock_own_list.erase(it);

      // Since we erased a lock entry from owned list, the first entries
      // in the pending list can be pushed to owned list if they are
      // compatible with the remaining owned entries.
      movePendingToOwned(rid);

      return LockTableResult::kDeleteFromOwned;
    }
  }

  // Iterate over pending locks list to check the lock entry of the transaction
  // on this resource id exists.
  for (lock_pending_list::const_iterator it = lock_pending_list.begin();
         it != lock_pending_list.cend(); ++it) {
    if (it->first == tid) {
      // If it exists, erase it from pending list.
      lock_pending_list.erase(it);
      return LockTableResult::kDeleteFromPending;
    }
  }

  // Execution reaches here, if we cannot find the corresponding lock entry
  // in the both list.
  return LockTableResult::kDeleteError;
}

void LockTable::movePendingToOwned(const ResourceId &rid) {
  lock_list_pair &lock_list_pair = internal_map_[rid];
  lock_own_list &lock_own_list = lock_list_pair.first;
  lock_pending_list &lock_pending_list = lock_list_pair.second;

  // Iterate over pending list to pending requests compatible with the
  // all entries in the resource ids owned lock list.
  for (lock_pending_list::const_iterator pending_it = lock_pending_list.cbegin();
         pending_it != lock_pending_list.cend(); ++pending_it) {
    transaction_id pending_tid = pending_it->first;
    AccessMode pending_mode = pending_it->second.getAccessMode();
    bool is_compatible_with_own_list = true;

    // Now compare against the all entries in the owned lock list.
    for (lock_own_list::const_iterator owned_it = lock_own_list.cbegin();
           owned_it != lock_pending_list.cend(); ++owned_it) {
      AccessMode owned_mode = owned_it->second.getAccessMode();
      if (!pending_mode.isCompatible(owned_mode)) {
        // If it is not compatible, we will not move this entry.
        is_compatible_with_own_list = false;
        break;
      }
    }

    // If this pending lock entry is compatible with the all entries in the
    // owned lock list, we should move it from pending list to owned list.
    if (is_compatible_with_own_list) {
      // Erase the entry from the list. Get the new iterator.
      pending_it = lock_pending_list.erase(pending_it);

      // Put the corresponding entry to the own list.
      lock_own_list.emplace_back(pending_tid, Lock(rid, pending_mode));

      // Move iterator one step backward because erasing an element moves the
      // iterator to the next element.
      --pending_it;
    } else {
      // There is no need to iterate pending list anymore since
      // we found first incompatible one. Checking, and accepting other pending
      // entries may cause starvation.
      break;
    }
  }
}

LockTable::iterator LockTable::begin() {
  return internal_map_.begin();
}

LockTable::iterator LockTable::end() {
  return internal_map_.end();
}

LockTable::const_iterator LockTable::begin() const {
  return internal_map_.begin();
}

LockTable::const_iterator LockTable::end() const {
  return internal_map_.end();
}

void LockTable::latchShared() {
  mutex_.lockShared();
}

void LockTable::unlatchShared() {
  mutex_.unlockShared();
}

void LockTable::latchExclusive() {
  mutex_.lock();
}

void LockTable::unlatchExclusive() {
  mutex_.unlock();
}

}  // namespace transaction
}  // namespace quickstep
