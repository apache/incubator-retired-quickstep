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

#ifndef QUICKSTEP_STORAGE_COMPACT_KEY_SEPARATE_CHAINING_HASH_TABLE_HPP_
#define QUICKSTEP_STORAGE_COMPACT_KEY_SEPARATE_CHAINING_HASH_TABLE_HPP_

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "cli/Flags.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/StorageConstants.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "utility/Macros.hpp"
#include "utility/Range.hpp"
#include "utility/ScopedArray.hpp"

#include "glog/logging.h"

namespace quickstep {

class AggregationHandle;
class ColumnVectorsValueAccessor;
class StorageManager;

class CompactKeySeparateChainingHashTable : public AggregationStateHashTableBase {
 public:
  CompactKeySeparateChainingHashTable(
      const std::vector<const Type*> &key_types,
      const std::size_t num_entries,
      const std::vector<AggregationHandle *> &handles,
      StorageManager *storage_manager);

  bool upsertValueAccessorCompositeKey(
      const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
      const std::vector<MultiSourceAttributeId> &key_attr_ids,
      const ValueAccessorMultiplexer &accessor_mux) override;

  void destroyPayload() override {}

  HashTableImplType getImplType() const override {
    return HashTableImplType::kCompactKeySeparateChaining;
  }

  std::size_t getMemoryConsumptionBytes() const override {
    return kSlotDataSize * num_slots_ + kKeyBucketDataSize * num_key_buckets_;
  }

  inline std::size_t getNumInitializationPartitions() const {
    return slots_init_splitter_->getNumPartitions();
  }

  inline std::size_t getNumFinalizationPartitions() const {
    if (final_splitter_ == nullptr) {
      final_splitter_ = std::make_unique<RangeSplitter>(
          RangeSplitter::CreateWithPartitionLength(
              0, buckets_allocated_.load(std::memory_order_relaxed),
              kFinalMinPartitionLength, FLAGS_num_workers * 2));
    }
    return final_splitter_->getNumPartitions();
  }

  void initialize(const std::size_t partition_id) {
    const Range slots_range = slots_init_splitter_->getPartition(partition_id);
    std::memset(slots_.get() + slots_range.begin(),
                0,
                slots_range.size() * kSlotDataSize);

    const Range key_buckets_range =
        key_buckets_init_splitter_->getPartition(partition_id);
    std::memset(key_buckets_.get() + key_buckets_range.begin(),
                0,
                key_buckets_range.size() * kKeyBucketDataSize);
  }

  void finalizeKeys(const std::size_t partition_id,
                    ColumnVectorsValueAccessor *output) const;

 private:
  inline static std::size_t CacheLineAlignedBytes(const std::size_t actual_bytes) {
    return (actual_bytes + kCacheLineBytes - 1) / kCacheLineBytes * kCacheLineBytes;
  }

  inline static std::size_t CalculateNumInitializationPartitions(
      const std::size_t memory_size) {
    // Set initialization memory block size as 4MB.
    constexpr std::size_t kInitBlockSize = 4uL * 1024u * 1024u;

    // At least 1 partition, at most 80 partitions.
    return std::max(1uL, std::min(memory_size / kInitBlockSize, 80uL));
  }

  inline static std::size_t CalculateNumFinalizationPartitions(
      const std::size_t num_entries) {
    // Set finalization segment size as 4096 entries.
    constexpr std::size_t kFinalizeSegmentSize = 4uL * 1024L;

    // At least 1 partition, at most 80 partitions.
    return std::max(1uL, std::min(num_entries / kFinalizeSegmentSize, 80uL));
  }

  using KeyCode = std::uint64_t;
  using BucketIndex = std::uint32_t;

  inline BucketIndex locateBucketInternal(const KeyCode key_code);

  template <typename ValueAccessorT>
  inline void constructCompactKeyCodeComponent(const std::size_t num_tuples,
                                               const std::size_t offset,
                                               const std::size_t key_size,
                                               ValueAccessorT *accessor,
                                               const attribute_id attr,
                                               KeyCode *key_codes);

  static constexpr std::size_t kInitMinPartitionLength = 1024uL * 256uL;
  static constexpr std::size_t kFinalMinPartitionLength = 1024uL * 4uL;

  struct KeyBucket {
    KeyCode key_code;
    std::atomic<BucketIndex> next;
  };

  static constexpr std::size_t kSlotDataSize = sizeof(std::atomic<BucketIndex>);
  static constexpr std::size_t kKeyBucketDataSize = sizeof(KeyBucket);
  static constexpr BucketIndex kExclusiveState = std::numeric_limits<BucketIndex>::max();

  const std::vector<const Type*> key_types_;
  std::vector<std::size_t> key_sizes_;

  ScopedArray<std::atomic<BucketIndex>> slots_;
  ScopedArray<KeyBucket> key_buckets_;

  std::size_t num_slots_;
  std::size_t num_key_buckets_;
  std::atomic<std::size_t> buckets_allocated_;

  std::unique_ptr<RangeSplitter> slots_init_splitter_;
  std::unique_ptr<RangeSplitter> key_buckets_init_splitter_;
  mutable std::unique_ptr<RangeSplitter> final_splitter_;

  DISALLOW_COPY_AND_ASSIGN(CompactKeySeparateChainingHashTable);
};

// ----------------------------------------------------------------------------
// Implementations of class methods follow.

inline CompactKeySeparateChainingHashTable::BucketIndex
    CompactKeySeparateChainingHashTable::locateBucketInternal(const KeyCode key_code) {
  std::atomic<BucketIndex> *pending_chain = &slots_[key_code % num_slots_];

  for (;;) {
    BucketIndex existing_chain = 0;

    // Check if current node is the end of the chain.
    if (pending_chain->compare_exchange_strong(existing_chain,
                                               kExclusiveState,
                                               std::memory_order_acq_rel)) {
      const BucketIndex bucket_index =
          buckets_allocated_.fetch_add(1, std::memory_order_relaxed);

      // TODO(jianqiao): Resize.
      if (bucket_index > num_key_buckets_) {
        LOG(FATAL) << "Need resize, not handled";
      }

      // Store key code into key bucket.
      key_buckets_[bucket_index].key_code = key_code;

      // Update the chaing pointer to point to the new node.
      pending_chain->store(bucket_index + 1, std::memory_order_release);

      return bucket_index;
    }

    // Spin until the pointer is available.
    while (existing_chain == kExclusiveState) {
      existing_chain = pending_chain->load(std::memory_order_acquire);
    }

    if (existing_chain == 0) {
      // Other thread had to roll back, so try again.
      continue;
    }

    const BucketIndex bucket_index = existing_chain - 1;
    KeyBucket &key_bucket = key_buckets_[bucket_index];
    if (key_bucket.key_code == key_code) {
      return bucket_index;
    } else {
      pending_chain = &key_bucket.next;
    }
  }
}

template <typename ValueAccessorT>
inline void CompactKeySeparateChainingHashTable
    ::constructCompactKeyCodeComponent(const std::size_t num_tuples,
                                       const std::size_t offset,
                                       const std::size_t key_size,
                                       ValueAccessorT *accessor,
                                       const attribute_id attr,
                                       KeyCode *key_codes) {
  accessor->beginIteration();
  for (std::size_t i = 0; i < num_tuples; ++i) {
    accessor->next();
    std::memcpy(reinterpret_cast<char*>(key_codes + i) + offset,
                accessor->template getUntypedValue<false>(attr),
                key_size);
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_COMPACT_KEY_SEPARATE_CHAINING_HASH_TABLE_HPP_
