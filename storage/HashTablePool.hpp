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

#ifndef QUICKSTEP_STORAGE_HASH_TABLE_POOL_HPP_
#define QUICKSTEP_STORAGE_HASH_TABLE_POOL_HPP_

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "storage/HashTableBase.hpp"
#include "storage/HashTableFactory.hpp"
#include "threading/SpinMutex.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class AggregationHandle;
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
   * @note This constructor is relevant for HashTables specialized for
   *       aggregation.
   *
   * @param estimated_num_entries The maximum number of entries in a hash table.
   * @param hash_table_impl_type The type of hash table implementation.
   * @param group_by_types A vector of pointer of types which form the group by
   *        key.
   * @param handles The AggregationHandles in this query.
   * @param storage_manager A pointer to the storage manager.
   **/
  HashTablePool(const std::size_t estimated_num_entries,
                const HashTableImplType hash_table_impl_type,
                const std::vector<const Type *> &group_by_types,
                const std::vector<AggregationHandle *> &handles,
                StorageManager *storage_manager)
      : estimated_num_entries_(reduceEstimatedCardinality(estimated_num_entries)),
        hash_table_impl_type_(hash_table_impl_type),
        group_by_types_(group_by_types),
        handles_(handles),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  /**
   * @brief Get the type of hash table implementation for this pool.
   *
   * @return The type of hash table implementation for this pool.
   */
  HashTableImplType getHashTableImplType() const {
    return hash_table_impl_type_;
  }

  /**
   * @brief Check out a hash table for insertion.
   *
   * @note This method is relevant for specialized (for aggregation)
   *       hash table implementation.
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
  std::vector<std::unique_ptr<AggregationStateHashTableBase>>*
      getAllHashTables() {
    return &hash_tables_;
  }

  /**
   * @brief Get the total memory consumed by the hash tables in this pool.
   **/
  std::size_t getMemoryConsumptionPoolBytes() const {
    std::size_t memory = 0;
    for (std::size_t ht_id = 0; ht_id <  hash_tables_.size(); ++ht_id) {
      if (hash_tables_[ht_id] != nullptr) {
        memory += hash_tables_[ht_id]->getMemoryConsumptionBytes();
      }
    }
    return memory;
  }

 private:
  AggregationStateHashTableBase* createNewHashTable() {
    return AggregationStateHashTableFactory::CreateResizable(
                hash_table_impl_type_,
                group_by_types_,
                estimated_num_entries_,
                handles_,
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
  const std::vector<AggregationHandle *> handles_;
  StorageManager *storage_manager_;

  SpinMutex mutex_;

  DISALLOW_COPY_AND_ASSIGN(HashTablePool);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_POOL_HPP_
