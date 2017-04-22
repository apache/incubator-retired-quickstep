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

#ifndef QUICKSTEP_STORAGE_THREAD_PRIVATE_COMPACT_KEY_HASH_TABLE_HPP_
#define QUICKSTEP_STORAGE_THREAD_PRIVATE_COMPACT_KEY_HASH_TABLE_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregationHandle;
class StorageManager;
class Type;

/**
 * @brief Specialized aggregation hash table that is preferable for two-phase
 *        aggregation with small-cardinality group-by keys. To use this hash
 *        table, it also requires that the group-by keys have fixed-length types
 *        with total byte size no greater than 8 (so that the keys can be packed
 *        into a 64-bit QWORD).
 */
class ThreadPrivateCompactKeyHashTable : public AggregationStateHashTableBase {
 public:
  /**
   * @brief Constructor.
   *
   * @param key_types A vector of one or more types (>1 indicates a composite
   *        key).
   * @param num_entries The estimated number of entries this hash table will
   *        hold.
   * @param handles The aggregation handles.
   * @param storage_manager The StorageManager to use (a StorageBlob will be
   *        allocated to hold this hash table's contents).
   **/
  ThreadPrivateCompactKeyHashTable(
      const std::vector<const Type*> &key_types,
      const std::size_t num_entries,
      const std::vector<AggregationHandle*> &handles,
      StorageManager *storage_manager);

  ~ThreadPrivateCompactKeyHashTable() override;

  HashTableImplType getImplType() const override {
    return HashTableImplType::kThreadPrivateCompactKey;
  }

  void destroyPayload() override {}

  std::size_t getMemoryConsumptionBytes() const override {
    return blob_->size();
  }

  /**
   * @return The number of entries in this HashTable.
   **/
  inline std::size_t numEntries() const {
    return buckets_allocated_;
  }

  bool upsertValueAccessorCompositeKey(
      const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
      const std::vector<MultiSourceAttributeId> &key_attr_ids,
      const ValueAccessorMultiplexer &accessor_mux) override;

  /**
   * @brief Merge the states of \p source into this hash table.
   *
   * @param source The source hash table from which the states are to be merged
   *        into this hash table.
   */
  void mergeFrom(const ThreadPrivateCompactKeyHashTable &source);

  /**
   * @brief Finalize all the aggregation state vectors and add the result column
   *        vectors into the output ColumnVectorsValueAccessor.
   *
   * @param output The ColumnVectorsValueAccessor to add all the result column
   *        vectors into.
   */
  void finalize(ColumnVectorsValueAccessor *output) const;

 private:
  // Compact key as a 64-bit QWORD.
  using KeyCode = std::uint64_t;
  static constexpr std::size_t kKeyCodeSize = sizeof(KeyCode);

  using BucketIndex = std::uint32_t;

  inline static std::size_t CacheLineAlignedBytes(const std::size_t actual_bytes) {
    return (actual_bytes + kCacheLineBytes - 1) / kCacheLineBytes * kCacheLineBytes;
  }

  // Grow the size of this hash table by a factor of 2.
  void resize();

  template <std::size_t key_size>
  inline static void ConstructKeyCode(const std::size_t offset,
                                      const attribute_id attr_id,
                                      ValueAccessor *accessor,
                                      void *key_code_start) {
    InvokeOnAnyValueAccessor(
        accessor,
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      char *key_code_ptr = static_cast<char*>(key_code_start) + offset;
      accessor->beginIteration();
      while (accessor->next()) {
        std::memcpy(key_code_ptr,
                    accessor->template getUntypedValue<false>(attr_id),
                    key_size);
        key_code_ptr += kKeyCodeSize;
      }
    });
  }

  inline void upsertValueAccessorCount(const std::vector<BucketIndex> &bucket_indices,
                                       void *state_vec) {
    std::int64_t *states = static_cast<std::int64_t*>(state_vec);
    for (const BucketIndex idx : bucket_indices) {
      states[idx] += 1;
    }
  }

  template <typename ArgumentT, typename StateT>
  inline void upsertValueAccessorSum(const std::vector<BucketIndex> &bucket_indices,
                                     const attribute_id attr_id,
                                     ValueAccessor *accessor,
                                     void *state_vec) {
    InvokeOnAnyValueAccessor(
        accessor,
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      accessor->beginIteration();

      StateT *states = static_cast<StateT*>(state_vec);
      for (const BucketIndex idx : bucket_indices) {
        accessor->next();
        states[idx] += *static_cast<const ArgumentT*>(
            accessor->template getUntypedValue<false>(attr_id));
      }
    });
  }

  template <typename StateT>
  inline void mergeStateSum(const std::vector<BucketIndex> &dst_bucket_indices,
                            const void *src_state_vec,
                            void *dst_state_vec) {
    StateT *dst_states = static_cast<StateT*>(dst_state_vec);
    const StateT* src_states = static_cast<const StateT*>(src_state_vec);
    for (std::size_t i = 0; i < dst_bucket_indices.size(); ++i) {
      dst_states[dst_bucket_indices[i]] += src_states[i];
    }
  }

  template <std::size_t key_size>
  inline void finalizeKey(const std::size_t offset,
                          NativeColumnVector *output_cv) const {
    const char *key_ptr = reinterpret_cast<const char*>(keys_) + offset;
    for (std::size_t i = 0; i < buckets_allocated_; ++i) {
      std::memcpy(output_cv->getPtrForDirectWrite(),
                  key_ptr,
                  key_size);
      key_ptr += kKeyCodeSize;
    }
  }

  template <typename StateT, typename ResultT>
  inline void finalizeStateSum(const void *state_vec,
                               NativeColumnVector *output_cv) const {
    const StateT *states = static_cast<const StateT*>(state_vec);
    for (std::size_t i = 0; i < buckets_allocated_; ++i) {
      *static_cast<ResultT*>(output_cv->getPtrForDirectWrite()) = states[i];
    }
  }

  const std::vector<const Type*> key_types_;
  const std::vector<AggregationHandle *> handles_;

  std::vector<std::size_t> key_sizes_;
  std::vector<std::size_t> state_sizes_;
  std::size_t total_state_size_;

  std::size_t num_buckets_;
  std::size_t buckets_allocated_;

  // Maps a compact-key to its bucket location.
  std::unordered_map<KeyCode, BucketIndex> index_;

  // Compact-key array where keys_[i] holds the compact-key for bucket i.
  KeyCode *keys_;

  // Use a column-wise layout for aggregation states.
  std::vector<void*> state_vecs_;

  StorageManager *storage_manager_;
  MutableBlobReference blob_;

  DISALLOW_COPY_AND_ASSIGN(ThreadPrivateCompactKeyHashTable);
};

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_THREAD_PRIVATE_COMPACT_KEY_HASH_TABLE_HPP_
