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

#include "storage/PackedPayloadHashTable.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/HashTableKeyManager.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "threading/SpinMutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "types/Type.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/Alignment.hpp"
#include "utility/Macros.hpp"
#include "utility/PrimeNumber.hpp"
#include "utility/TemplateUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

PackedPayloadHashTable::PackedPayloadHashTable(
    const std::vector<const Type *> &key_types,
    const std::size_t num_entries,
    const std::vector<AggregationHandle *> &handles,
    StorageManager *storage_manager)
    : key_types_(key_types),
      num_handles_(handles.size()),
      handles_(handles),
      total_payload_size_(ComputeTotalPayloadSize(handles)),
      storage_manager_(storage_manager),
      kBucketAlignment(alignof(std::atomic<std::size_t>)),
      kValueOffset(sizeof(std::atomic<std::size_t>) + sizeof(std::size_t)),
      key_manager_(key_types_, kValueOffset + total_payload_size_),
      bucket_size_(ComputeBucketSize(key_manager_.getFixedKeySize())) {
  std::size_t payload_offset_running_sum = sizeof(SpinMutex);
  for (const auto *handle : handles) {
    payload_offsets_.emplace_back(payload_offset_running_sum);
    payload_offset_running_sum += handle->getPayloadSize();
  }

  // NOTE(jianqiao): Potential memory leak / double freeing by copying from
  // init_payload to buckets if payload contains out of line data.
  init_payload_ =
      static_cast<std::uint8_t *>(calloc(this->total_payload_size_, 1));
  DCHECK(init_payload_ != nullptr);

  for (std::size_t i = 0; i < num_handles_; ++i) {
    handles_[i]->initPayload(init_payload_ + payload_offsets_[i]);
  }

  // Bucket size always rounds up to the alignment requirement of the atomic
  // size_t "next" pointer at the front or a ValueT, whichever is larger.
  //
  // Give base HashTable information about what key components are stored
  // inline from 'key_manager_'.
  setKeyInline(key_manager_.getKeyInline());

  // Pick out a prime number of slots and calculate storage requirements.
  std::size_t num_slots_tmp =
      get_next_prime_number(num_entries * kHashTableLoadFactor);
  std::size_t required_memory =
      sizeof(Header) + num_slots_tmp * sizeof(std::atomic<std::size_t>) +
      (num_slots_tmp / kHashTableLoadFactor) *
          (bucket_size_ + key_manager_.getEstimatedVariableKeySize());
  std::size_t num_storage_slots =
      this->storage_manager_->SlotsNeededForBytes(required_memory);
  if (num_storage_slots == 0) {
    FATAL_ERROR(
        "Storage requirement for SeparateChainingHashTable "
        "exceeds maximum allocation size.");
  }

  // Get a StorageBlob to hold the hash table.
  const block_id blob_id =
      this->storage_manager_->createBlob(num_storage_slots);
  this->blob_ = this->storage_manager_->getBlobMutable(blob_id);

  void *aligned_memory_start = this->blob_->getMemoryMutable();
  std::size_t available_memory = num_storage_slots * kSlotSizeBytes;
  if (align(alignof(Header),
            sizeof(Header),
            aligned_memory_start,
            available_memory) == nullptr) {
    // With current values from StorageConstants.hpp, this should be
    // impossible. A blob is at least 1 MB, while a Header has alignment
    // requirement of just kCacheLineBytes (64 bytes).
    FATAL_ERROR(
        "StorageBlob used to hold resizable "
        "SeparateChainingHashTable is too small to meet alignment "
        "requirements of SeparateChainingHashTable::Header.");
  } else if (aligned_memory_start != this->blob_->getMemoryMutable()) {
    // This should also be impossible, since the StorageManager allocates slots
    // aligned to kCacheLineBytes.
    DEV_WARNING("StorageBlob memory adjusted by "
                << (num_storage_slots * kSlotSizeBytes - available_memory)
                << " bytes to meet alignment requirement for "
                << "SeparateChainingHashTable::Header.");
  }

  // Locate the header.
  header_ = static_cast<Header *>(aligned_memory_start);
  aligned_memory_start =
      static_cast<char *>(aligned_memory_start) + sizeof(Header);
  available_memory -= sizeof(Header);

  // Recompute the number of slots & buckets using the actual available memory.
  // Most likely, we got some extra free bucket space due to "rounding up" to
  // the storage blob's size. It's also possible (though very unlikely) that we
  // will wind up with fewer buckets than we initially wanted because of screwy
  // alignment requirements for ValueT.
  std::size_t num_buckets_tmp =
      available_memory /
      (kHashTableLoadFactor * sizeof(std::atomic<std::size_t>) + bucket_size_ +
       key_manager_.getEstimatedVariableKeySize());
  num_slots_tmp =
      get_previous_prime_number(num_buckets_tmp * kHashTableLoadFactor);
  num_buckets_tmp = num_slots_tmp / kHashTableLoadFactor;
  DEBUG_ASSERT(num_slots_tmp > 0);
  DEBUG_ASSERT(num_buckets_tmp > 0);

  // Locate the slot array.
  slots_ = static_cast<std::atomic<std::size_t> *>(aligned_memory_start);
  aligned_memory_start = static_cast<char *>(aligned_memory_start) +
                         sizeof(std::atomic<std::size_t>) * num_slots_tmp;
  available_memory -= sizeof(std::atomic<std::size_t>) * num_slots_tmp;

  // Locate the buckets.
  buckets_ = aligned_memory_start;
  // Extra-paranoid: If ValueT has an alignment requirement greater than that
  // of std::atomic<std::size_t>, we may need to adjust the start of the bucket
  // array.
  if (align(kBucketAlignment, bucket_size_, buckets_, available_memory) ==
      nullptr) {
    FATAL_ERROR(
        "StorageBlob used to hold resizable "
        "SeparateChainingHashTable is too small to meet "
        "alignment requirements of buckets.");
  } else if (buckets_ != aligned_memory_start) {
    DEV_WARNING(
        "Bucket array start position adjusted to meet alignment "
        "requirement for SeparateChainingHashTable's value type.");
    if (num_buckets_tmp * bucket_size_ > available_memory) {
      --num_buckets_tmp;
    }
  }

  // Fill in the header.
  header_->num_slots = num_slots_tmp;
  header_->num_buckets = num_buckets_tmp;
  header_->buckets_allocated.store(0, std::memory_order_relaxed);
  header_->variable_length_bytes_allocated.store(0, std::memory_order_relaxed);
  available_memory -= bucket_size_ * (header_->num_buckets);

  // Locate variable-length key storage region, and give it all the remaining
  // bytes in the blob.
  key_manager_.setVariableLengthStorageInfo(
      static_cast<char *>(buckets_) + header_->num_buckets * bucket_size_,
      available_memory,
      &(header_->variable_length_bytes_allocated));
}

PackedPayloadHashTable::~PackedPayloadHashTable() {
  if (blob_.valid()) {
    const block_id blob_id = blob_->getID();
    blob_.release();
    storage_manager_->deleteBlockOrBlobFile(blob_id);
  }
  std::free(init_payload_);
}

void PackedPayloadHashTable::clear() {
  const std::size_t used_buckets =
      header_->buckets_allocated.load(std::memory_order_relaxed);
  // Destroy existing values, if necessary.
  destroyPayload();

  // Zero-out slot array.
  std::memset(
      slots_, 0x0, sizeof(std::atomic<std::size_t>) * header_->num_slots);

  // Zero-out used buckets.
  std::memset(buckets_, 0x0, used_buckets * bucket_size_);

  header_->buckets_allocated.store(0, std::memory_order_relaxed);
  header_->variable_length_bytes_allocated.store(0, std::memory_order_relaxed);
  key_manager_.zeroNextVariableLengthKeyOffset();
}

void PackedPayloadHashTable::destroyPayload() {
  const std::size_t num_buckets =
      header_->buckets_allocated.load(std::memory_order_relaxed);
  void *bucket_ptr = static_cast<char *>(buckets_) + kValueOffset;
  for (std::size_t bucket_num = 0; bucket_num < num_buckets; ++bucket_num) {
    for (std::size_t handle_id = 0; handle_id < num_handles_; ++handle_id) {
      void *value_internal_ptr =
          static_cast<char *>(bucket_ptr) + this->payload_offsets_[handle_id];
      handles_[handle_id]->destroyPayload(static_cast<std::uint8_t *>(value_internal_ptr));
    }
    bucket_ptr = static_cast<char *>(bucket_ptr) + bucket_size_;
  }
}

bool PackedPayloadHashTable::upsertValueAccessorCompositeKey(
    const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
    const std::vector<MultiSourceAttributeId> &key_attr_ids,
    const ValueAccessorMultiplexer &accessor_mux) {
  ValueAccessor *base_accessor = accessor_mux.getBaseAccessor();
  ValueAccessor *derived_accessor = accessor_mux.getDerivedAccessor();

  const bool has_derived_accessor = (derived_accessor != nullptr);

  base_accessor->beginIterationVirtual();
  if (has_derived_accessor) {
    DCHECK(derived_accessor->getImplementationType()
               == ValueAccessor::Implementation::kColumnVectors);
    derived_accessor->beginIterationVirtual();
  }

  return InvokeOnBools(
      has_derived_accessor,
      handles_.empty(),
      !all_keys_inline_,
      [&](auto use_two_accessors,  // NOLINT(build/c++11)
          auto key_only,
          auto has_variable_size) -> bool {
    return this->upsertValueAccessorCompositeKeyInternal<
        decltype(use_two_accessors)::value,
        decltype(key_only)::value,
        decltype(has_variable_size)::value>(
            argument_ids,
            key_attr_ids,
            base_accessor,
            static_cast<ColumnVectorsValueAccessor *>(derived_accessor));
  });
}

void PackedPayloadHashTable::resize(const std::size_t extra_buckets,
                                    const std::size_t extra_variable_storage,
                                    const std::size_t retry_num) {
  // A retry should never be necessary with this implementation of HashTable.
  // Separate chaining ensures that any resized hash table with more buckets
  // than the original table will be able to hold more entries than the
  // original.
  DEBUG_ASSERT(retry_num == 0);

  SpinSharedMutexExclusiveLock<true> write_lock(this->resize_shared_mutex_);

  // Recheck whether the hash table is still full. Note that multiple threads
  // might wait to rebuild this hash table simultaneously. Only the first one
  // should do the rebuild.
  if (!isFull(extra_variable_storage)) {
    return;
  }

  // Approximately double the number of buckets and slots.
  //
  // TODO(chasseur): It may be worth it to more than double the number of
  // buckets here so that we can maintain a good, sparse fill factor for a
  // longer time as more values are inserted. Such behavior should take into
  // account kHashTableLoadFactor.
  std::size_t resized_num_slots = get_next_prime_number(
      (header_->num_buckets + extra_buckets / 2) * kHashTableLoadFactor * 2);
  std::size_t variable_storage_required =
      (resized_num_slots / kHashTableLoadFactor) *
      key_manager_.getEstimatedVariableKeySize();
  const std::size_t original_variable_storage_used =
      header_->variable_length_bytes_allocated.load(std::memory_order_relaxed);
  // If this resize was triggered by a too-large variable-length key, bump up
  // the variable-length storage requirement.
  if ((extra_variable_storage > 0) &&
      (extra_variable_storage + original_variable_storage_used >
       key_manager_.getVariableLengthKeyStorageSize())) {
    variable_storage_required += extra_variable_storage;
  }

  const std::size_t resized_memory_required =
      sizeof(Header) + resized_num_slots * sizeof(std::atomic<std::size_t>) +
      (resized_num_slots / kHashTableLoadFactor) * bucket_size_ +
      variable_storage_required;
  const std::size_t resized_storage_slots =
      this->storage_manager_->SlotsNeededForBytes(resized_memory_required);
  if (resized_storage_slots == 0) {
    FATAL_ERROR(
        "Storage requirement for resized SeparateChainingHashTable "
        "exceeds maximum allocation size.");
  }

  // Get a new StorageBlob to hold the resized hash table.
  const block_id resized_blob_id =
      this->storage_manager_->createBlob(resized_storage_slots);
  MutableBlobReference resized_blob =
      this->storage_manager_->getBlobMutable(resized_blob_id);

  // Locate data structures inside the new StorageBlob.
  void *aligned_memory_start = resized_blob->getMemoryMutable();
  std::size_t available_memory = resized_storage_slots * kSlotSizeBytes;
  if (align(alignof(Header),
            sizeof(Header),
            aligned_memory_start,
            available_memory) == nullptr) {
    // Should be impossible, as noted in constructor.
    FATAL_ERROR(
        "StorageBlob used to hold resized SeparateChainingHashTable "
        "is too small to meet alignment requirements of "
        "LinearOpenAddressingHashTable::Header.");
  } else if (aligned_memory_start != resized_blob->getMemoryMutable()) {
    // Again, should be impossible.
    DEV_WARNING("In SeparateChainingHashTable::resize(), StorageBlob "
                << "memory adjusted by "
                << (resized_num_slots * kSlotSizeBytes - available_memory)
                << " bytes to meet alignment requirement for "
                << "LinearOpenAddressingHashTable::Header.");
  }

  Header *resized_header = static_cast<Header *>(aligned_memory_start);
  aligned_memory_start =
      static_cast<char *>(aligned_memory_start) + sizeof(Header);
  available_memory -= sizeof(Header);

  // As in constructor, recompute the number of slots and buckets using the
  // actual available memory.
  std::size_t resized_num_buckets =
      (available_memory - extra_variable_storage) /
      (kHashTableLoadFactor * sizeof(std::atomic<std::size_t>) + bucket_size_ +
       key_manager_.getEstimatedVariableKeySize());
  resized_num_slots =
      get_previous_prime_number(resized_num_buckets * kHashTableLoadFactor);
  resized_num_buckets = resized_num_slots / kHashTableLoadFactor;

  // Locate slot array.
  std::atomic<std::size_t> *resized_slots =
      static_cast<std::atomic<std::size_t> *>(aligned_memory_start);
  aligned_memory_start = static_cast<char *>(aligned_memory_start) +
                         sizeof(std::atomic<std::size_t>) * resized_num_slots;
  available_memory -= sizeof(std::atomic<std::size_t>) * resized_num_slots;

  // As in constructor, we will be extra paranoid and use align() to locate the
  // start of the array of buckets, as well.
  void *resized_buckets = aligned_memory_start;
  if (align(
          kBucketAlignment, bucket_size_, resized_buckets, available_memory) ==
      nullptr) {
    FATAL_ERROR(
        "StorageBlob used to hold resized SeparateChainingHashTable "
        "is too small to meet alignment requirements of buckets.");
  } else if (resized_buckets != aligned_memory_start) {
    DEV_WARNING(
        "Bucket array start position adjusted to meet alignment "
        "requirement for SeparateChainingHashTable's value type.");
    if (resized_num_buckets * bucket_size_ + variable_storage_required >
        available_memory) {
      --resized_num_buckets;
    }
  }
  aligned_memory_start = static_cast<char *>(aligned_memory_start) +
                         resized_num_buckets * bucket_size_;
  available_memory -= resized_num_buckets * bucket_size_;

  void *resized_variable_length_key_storage = aligned_memory_start;
  const std::size_t resized_variable_length_key_storage_size = available_memory;

  const std::size_t original_buckets_used =
      header_->buckets_allocated.load(std::memory_order_relaxed);

  // Initialize the header.
  resized_header->num_slots = resized_num_slots;
  resized_header->num_buckets = resized_num_buckets;
  resized_header->buckets_allocated.store(original_buckets_used,
                                          std::memory_order_relaxed);
  resized_header->variable_length_bytes_allocated.store(
      original_variable_storage_used, std::memory_order_relaxed);

  // Bulk-copy buckets. This is safe because:
  //     1. The "next" pointers will be adjusted when rebuilding chains below.
  //     2. The hash codes will stay the same.
  //     3. For key components:
  //       a. Inline keys will stay exactly the same.
  //       b. Offsets into variable-length storage will remain valid, because
  //          we also do a byte-for-byte copy of variable-length storage below.
  //       c. Absolute external pointers will still point to the same address.
  //       d. Relative pointers are not used with resizable hash tables.
  //     4. If values are not trivially copyable, then we invoke ValueT's copy
  //        or move constructor with placement new.
  // NOTE(harshad) - Regarding point 4 above, as this is a specialized
  // hash table implemented for aggregation, the values are trivially copyable,
  // therefore we don't need to invoke payload values' copy/move constructors.
  std::memcpy(resized_buckets, buckets_, original_buckets_used * bucket_size_);

  // Copy over variable-length key components, if any.
  if (original_variable_storage_used > 0) {
    DEBUG_ASSERT(original_variable_storage_used ==
                 key_manager_.getNextVariableLengthKeyOffset());
    DEBUG_ASSERT(original_variable_storage_used <=
                 resized_variable_length_key_storage_size);
    std::memcpy(resized_variable_length_key_storage,
                key_manager_.getVariableLengthKeyStorage(),
                original_variable_storage_used);
  }

  destroyPayload();

  // Make resized structures active.
  std::swap(this->blob_, resized_blob);
  header_ = resized_header;
  slots_ = resized_slots;
  buckets_ = resized_buckets;
  key_manager_.setVariableLengthStorageInfo(
      resized_variable_length_key_storage,
      resized_variable_length_key_storage_size,
      &(resized_header->variable_length_bytes_allocated));

  // Drop the old blob.
  const block_id old_blob_id = resized_blob->getID();
  resized_blob.release();
  this->storage_manager_->deleteBlockOrBlobFile(old_blob_id);

  // Rebuild chains.
  void *current_bucket = buckets_;
  for (std::size_t bucket_num = 0; bucket_num < original_buckets_used;
       ++bucket_num) {
    std::atomic<std::size_t> *next_ptr =
        static_cast<std::atomic<std::size_t> *>(current_bucket);
    const std::size_t hash_code = *reinterpret_cast<const std::size_t *>(
        static_cast<const char *>(current_bucket) +
        sizeof(std::atomic<std::size_t>));

    const std::size_t slot_number = hash_code % header_->num_slots;
    std::size_t slot_ptr_value = 0;
    if (slots_[slot_number].compare_exchange_strong(
            slot_ptr_value, bucket_num + 1, std::memory_order_relaxed)) {
      // This bucket is the first in the chain for this block, so reset its
      // next pointer to 0.
      next_ptr->store(0, std::memory_order_relaxed);
    } else {
      // A chain already exists starting from this slot, so put this bucket at
      // the head.
      next_ptr->store(slot_ptr_value, std::memory_order_relaxed);
      slots_[slot_number].store(bucket_num + 1, std::memory_order_relaxed);
    }
    current_bucket = static_cast<char *>(current_bucket) + bucket_size_;
  }
}

}  // namespace quickstep
