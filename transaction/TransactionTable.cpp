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

#include <cstddef>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

#include "transaction/AccessMode.hpp"
#include "transaction/ResourceId.hpp"
#include "transaction/Transaction.hpp"

namespace quickstep {
namespace transaction {

TransactionTableResult
TransactionTable::putOwnEntry(const transaction_id tid,
                              const ResourceId &rid,
                              const AccessMode &access_mode) {
  transaction_list_pair &transaction_list_pair = internal_map_[tid];
  transaction_own_list &transaction_own_list = transaction_list_pair.first;

  transaction_own_list.push_back(std::make_pair(rid, Lock(rid, access_mode)));

  return TransactionTableResult::kPlacedInOwned;
}

TransactionTableResult
TransactionTable::putPendingEntry(const transaction_id tid,
                                  const ResourceId &rid,
                                  const AccessMode &access_mode) {
  transaction_list_pair &transaction_list_pair = internal_map_[tid];
  transaction_pending_list &transaction_pending_list
      = transaction_list_pair.second;

  transaction_pending_list.push_back(std::make_pair(rid,
                                                    Lock(rid, access_mode)));

  return TransactionTableResult::kPlacedInPending;
}

TransactionTableResult
TransactionTable::deleteOwnEntry(const transaction_id tid,
                                 const ResourceId &rid,
                                 const AccessMode &access_mode) {
  transaction_list_pair &transaction_list_pair = internal_map_[tid];
  transaction_own_list &transaction_own_list = transaction_list_pair.first;

  std::size_t original_size = transaction_own_list.size();
  transaction_own_list.remove_if(
      [&rid, &access_mode](transaction_entry &entry) {
        return entry.second.getResourceId() == rid
        && entry.second.getAccessMode() == access_mode;
      });
  if (transaction_own_list.size() == original_size) {
    return TransactionTableResult::kDelError;
  } else {
    return TransactionTableResult::kDelFromOwned;
  }
}

TransactionTableResult
TransactionTable::deletePendingEntry(const transaction_id tid,
                                     const ResourceId &rid,
                                     const AccessMode &access_mode) {
  transaction_list_pair &transaction_list_pair = internal_map_[tid];
  transaction_pending_list &transaction_pending_list
      = transaction_list_pair.second;

  std::size_t original_size = transaction_pending_list.size();
  transaction_pending_list.remove_if(
     [&rid, &access_mode] (transaction_entry &entry) {
       return entry.second.getResourceId() == rid
         && entry.second.getAccessMode() == access_mode;
     });

  if (transaction_pending_list.size() == original_size) {
    return TransactionTableResult::kDelError;
  } else {
    return TransactionTableResult::kDelFromPending;
  }
}

std::vector<ResourceId>
TransactionTable::getResourceIdList(const transaction_id tid) {
  std::vector<ResourceId> result;
  const transaction_list_pair &transaction_list_pair = internal_map_[tid];
  const transaction_own_list &transaction_own_list =
      transaction_list_pair.second;
  const transaction_pending_list
      &transaction_pending_list = transaction_list_pair.first;

  for (transaction_own_list::const_iterator it = transaction_own_list.begin();
       it != transaction_own_list.end();
       ++it) {
    result.push_back(it->first);
  }

  for (transaction_pending_list::const_iterator
           it = transaction_pending_list.begin();
       it != transaction_pending_list.end();
       ++it) {
    result.push_back(it->first);
  }

  return result;
}

TransactionTableResult
TransactionTable::deleteTransaction(const transaction_id tid) {
  std::size_t original_size = internal_map_.size();
  internal_map_.erase(tid);
  std::size_t size_after_delete = internal_map_.size();

  if (original_size == size_after_delete) {
    return  TransactionTableResult::kTransactionDeleteError;
  }

  return TransactionTableResult::kTransactionDeleteOk;
}

}  // namespace transaction
}  // namespace quickstep
