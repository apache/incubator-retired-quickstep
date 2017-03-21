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

#ifndef QUICKSTEP_STORAGE_PARTITIONED_HASH_TABLE_POOL_HPP_
#define QUICKSTEP_STORAGE_PARTITIONED_HASH_TABLE_POOL_HPP_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "storage/HashTableBase.hpp"
#include "storage/HashTableFactory.hpp"
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
 * @brief A pool of HashTables used for a single aggregation handle. Each
 *        HashTable represents values from a given partition, which is
 *        determined by the keys in the group by clause.
 **/
class PartitionedHashTablePool {
 public:
  /**
   * @brief Constructor.
   *
   * @note This constructor is relevant for the HashTable specialized for
   *       aggregation.
   *
   * @param estimated_num_entries The maximum number of entries in a hash table.
   * @param num_partitions The number of partitions (i.e. number of HashTables)
   * @param hash_table_impl_type The type of hash table implementation.
   * @param group_by_types A vector of pointer of types which form the group by
   *        key.
   * @param handles The aggregation handles.
   * @param storage_manager A pointer to the storage manager.
   **/
  PartitionedHashTablePool(const std::size_t estimated_num_entries,
                           const std::size_t num_partitions,
                           const HashTableImplType hash_table_impl_type,
                           const std::vector<const Type *> &group_by_types,
                           const std::vector<AggregationHandle *> &handles,
                           StorageManager *storage_manager)
      : estimated_num_entries_(
            setHashTableSize(estimated_num_entries, num_partitions)),
        num_partitions_(num_partitions),
        hash_table_impl_type_(hash_table_impl_type),
        group_by_types_(group_by_types),
        handles_(handles),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {
    initializeAllHashTables();
  }

  /**
   * @brief Check out a hash table for insertion.
   *
   * @param partition_id The ID of the partitioned HashTable.
   *
   * @return A hash table pointer for the given HashTable.
   **/
  AggregationStateHashTableBase* getHashTable(const std::size_t partition_id) {
    DCHECK_LT(partition_id, num_partitions_);
    DCHECK_LT(partition_id, hash_tables_.size());
    return hash_tables_[partition_id].get();
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
   * @brief Get the number of partitions used for the aggregation.
   **/
  inline std::size_t getNumPartitions() const {
    return num_partitions_;
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
  void initializeAllHashTables() {
    for (std::size_t part_num = 0; part_num < num_partitions_; ++part_num) {
      AggregationStateHashTableBase *part_hash_table = createNewHashTable();
      hash_tables_.push_back(
          std::unique_ptr<AggregationStateHashTableBase>(part_hash_table));
    }
  }

  AggregationStateHashTableBase* createNewHashTable() {
    return AggregationStateHashTableFactory::CreateResizable(
                hash_table_impl_type_,
                group_by_types_,
                estimated_num_entries_,
                handles_,
                storage_manager_);
  }

  inline std::size_t setHashTableSize(const std::size_t overall_estimate,
                                      const std::size_t num_partitions) const {
    CHECK_NE(num_partitions, 0Lu);
    // The minimum size of the hash table is set to 100.
    return std::max(static_cast<std::size_t>(overall_estimate / num_partitions),
                    100Lu);
  }

  std::vector<std::unique_ptr<AggregationStateHashTableBase>> hash_tables_;

  const std::size_t estimated_num_entries_;
  const std::size_t num_partitions_;

  const HashTableImplType hash_table_impl_type_;

  const std::vector<const Type *> group_by_types_;
  const std::vector<AggregationHandle *> handles_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(PartitionedHashTablePool);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_POOL_HPP_
