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

#ifndef QUICKSTEP_STORAGE_HASH_TABLE_POOL_HPP_
#define QUICKSTEP_STORAGE_HASH_TABLE_POOL_HPP_

#include <chrono>
#include <memory>
#include <utility>
#include <vector>

#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/HashTableBase.hpp"
#include "threading/SpinMutex.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

class StorageManager;
class Type;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A pool of HashTables used for a single aggregation handle. This class
 *        has similar functionality as InsertDestination, but for checking out
 *        HashTables. A worker thread can check out a hash table for insertion,
 *        perform the insertions and return the hash table to the pool. While
 *        one thread is using a hash table, no other thread can access it.
 **/
class HashTablePool {
 public:
  /**
   * @brief Constructor.
   *
   * @param estimated_num_entries The maximum number of entries in a hash table.
   * @param hash_table_impl_type The type of hash table implementation.
   * @param group_by_types A vector of pointer of types which form the group by
   *        key.
   * @param agg_handle The aggregation handle.
   * @param storage_manager A pointer to the storage manager.
   *
   * @note The estimate of number of entries is quite inaccurate at this time.
   *       If we go by the current estimate, each hash table demands much
   *       larger space than it actually needs, which causes the system to
   *       either trigger evictions or worse - run out of memory. To fix this
   *       issue, we divide the estimate by 100. The division will not affect
   *       correctness, however it may allocate some hash tables smaller space
   *       than their requirement, causing them to be resized during build
   *       phase, which has a performance penalty.
   **/
  HashTablePool(const std::size_t estimated_num_entries,
                const HashTableImplType hash_table_impl_type,
                const std::vector<const Type *> &group_by_types,
                AggregationHandle *agg_handle,
                StorageManager *storage_manager)
      : estimated_num_entries_(reduceEstimatedCardinality(estimated_num_entries)),
        hash_table_impl_type_(hash_table_impl_type),
        group_by_types_(group_by_types),
        agg_handle_(DCHECK_NOTNULL(agg_handle)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  /**
   * @brief Check out a hash table for insertion.
   *
   * @return A hash table pointer.
   **/
  AggregationStateHashTableBase* getHashTable() {
    {
      SpinMutexLock lock(mutex_);
      if (!hash_tables_.empty()) {
        std::unique_ptr<AggregationStateHashTableBase> ret_hash_table(
            std::move(hash_tables_.back()));
        hash_tables_.pop_back();
        DCHECK(ret_hash_table != nullptr);
        return ret_hash_table.release();
      }
    }
    return createNewHashTable();
  }

  /**
   * @brief Return a previously checked out hash table.
   *
   * @param hash_table A pointer to the checked out hash table.
   **/
  void returnHashTable(AggregationStateHashTableBase *hash_table) {
    SpinMutexLock lock(mutex_);
    hash_tables_.push_back(
        std::unique_ptr<AggregationStateHashTableBase>(hash_table));
  }

  /**
   * @brief Get all the hash tables from the pool.
   *
   * @warning The caller should ensure that this call is made when no hash table
   *          is being checked in or checked out from the pool. In other words
   *          the hash table pool is in read-only state.
   *
   * @param All the hash tables in the pool.
   *
   **/
  const std::vector<std::unique_ptr<AggregationStateHashTableBase>>*
      getAllHashTables() {
    return &hash_tables_;
  }

 private:
  AggregationStateHashTableBase* createNewHashTable() {
    return agg_handle_->createGroupByHashTable(hash_table_impl_type_,
                                               group_by_types_,
                                               estimated_num_entries_,
                                               storage_manager_);
  }

  inline std::size_t reduceEstimatedCardinality(
      const std::size_t original_estimate) const {
    if (original_estimate < kEstimateReductionFactor) {
      return original_estimate;
    } else {
      DCHECK_GT(kEstimateReductionFactor, 0u);
      return original_estimate / kEstimateReductionFactor;
    }
  }

  static constexpr std::size_t kEstimateReductionFactor = 100;

  std::vector<std::unique_ptr<AggregationStateHashTableBase>> hash_tables_;

  const std::size_t estimated_num_entries_;
  const HashTableImplType hash_table_impl_type_;

  const std::vector<const Type *> group_by_types_;

  AggregationHandle *agg_handle_;
  StorageManager *storage_manager_;

  SpinMutex mutex_;

  DISALLOW_COPY_AND_ASSIGN(HashTablePool);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_POOL_HPP_
