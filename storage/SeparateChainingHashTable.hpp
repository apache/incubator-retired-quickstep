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

#ifndef QUICKSTEP_STORAGE_SEPARATE_CHAINING_HASH_TABLE_HPP_
#define QUICKSTEP_STORAGE_SEPARATE_CHAINING_HASH_TABLE_HPP_

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "storage/HashTable.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/HashTableKeyManager.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/Alignment.hpp"
#include "utility/Macros.hpp"
#include "utility/PrimeNumber.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A hash table implementation which uses separate chaining for buckets.
 **/
template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
class SeparateChainingHashTable : public HashTable<ValueT,
                                                   resizable,
                                                   serializable,
                                                   force_key_copy,
                                                   allow_duplicate_keys> {
 public:
  SeparateChainingHashTable(const std::vector<const Type*> &key_types,
                            const std::size_t num_entries,
                            StorageManager *storage_manager);

  SeparateChainingHashTable(const std::vector<const Type*> &key_types,
                            void *hash_table_memory,
                            const std::size_t hash_table_memory_size,
                            const bool new_hash_table,
                            const bool hash_table_memory_zeroed);

  // Delegating constructors for single scalar keys.
  SeparateChainingHashTable(const Type &key_type,
                            const std::size_t num_entries,
                            StorageManager *storage_manager)
      : SeparateChainingHashTable(std::vector<const Type*>(1, &key_type),
                                  num_entries,
                                  storage_manager) {
  }

  SeparateChainingHashTable(const Type &key_type,
                            void *hash_table_memory,
                            const std::size_t hash_table_memory_size,
                            const bool new_hash_table,
                            const bool hash_table_memory_zeroed)
      : SeparateChainingHashTable(std::vector<const Type*>(1, &key_type),
                                  hash_table_memory,
                                  hash_table_memory_size,
                                  new_hash_table,
                                  hash_table_memory_zeroed) {
  }

  ~SeparateChainingHashTable() override {
    DestroyValues(buckets_,
                  header_->buckets_allocated.load(std::memory_order_relaxed),
                  bucket_size_);
  }

  void clear() override;

  std::size_t numEntries() const override {
    return header_->buckets_allocated.load(std::memory_order_relaxed);
  }

  const ValueT* getSingle(const TypedValue &key) const override;
  const ValueT* getSingleCompositeKey(const std::vector<TypedValue> &key) const override;

  void getAll(const TypedValue &key,
              std::vector<const ValueT*> *values) const override;
  void getAllCompositeKey(const std::vector<TypedValue> &key,
                          std::vector<const ValueT*> *values) const override;

  std::size_t getHashTableMemorySizeBytes() const override {
    return sizeof(Header) + numEntries() * bucket_size_;
  }

 protected:
  HashTablePutResult putInternal(const TypedValue &key,
                                 const std::size_t variable_key_size,
                                 const ValueT &value,
                                 HashTablePreallocationState *prealloc_state) override;
  HashTablePutResult putCompositeKeyInternal(const std::vector<TypedValue> &key,
                                             const std::size_t variable_key_size,
                                             const ValueT &value,
                                             HashTablePreallocationState *prealloc_state) override;

  ValueT* upsertInternal(const TypedValue &key,
                         const std::size_t variable_key_size,
                         const ValueT &initial_value) override;
  ValueT* upsertCompositeKeyInternal(const std::vector<TypedValue> &key,
                                     const std::size_t variable_key_size,
                                     const ValueT &initial_value) override;

  bool getNextEntry(TypedValue *key,
                    const ValueT **value,
                    std::size_t *entry_num) const override;
  bool getNextEntryCompositeKey(std::vector<TypedValue> *key,
                                const ValueT **value,
                                std::size_t *entry_num) const override;

  bool getNextEntryForKey(const TypedValue &key,
                          const std::size_t hash_code,
                          const ValueT **value,
                          std::size_t *entry_num) const override;
  bool getNextEntryForCompositeKey(const std::vector<TypedValue> &key,
                                   const std::size_t hash_code,
                                   const ValueT **value,
                                   std::size_t *entry_num) const override;

  bool hasKey(const TypedValue &key) const override;
  bool hasCompositeKey(const std::vector<TypedValue> &key) const override;

  void resize(const std::size_t extra_buckets,
              const std::size_t extra_variable_storage,
              const std::size_t retry_num = 0) override;

  bool preallocateForBulkInsert(const std::size_t total_entries,
                                const std::size_t total_variable_key_size,
                                HashTablePreallocationState *prealloc_state) override;

 private:
  struct Header {
    std::size_t num_slots;
    std::size_t num_buckets;
    alignas(kCacheLineBytes)
        std::atomic<std::size_t> buckets_allocated;
    alignas(kCacheLineBytes)
        std::atomic<std::size_t> variable_length_bytes_allocated;
  };

  // Bucket size always rounds up to the alignment requirement of the atomic
  // size_t "next" pointer at the front or a ValueT, whichever is larger.
  //
  // Make sure that the larger of the two alignment requirements also satisfies
  // the smaller.
  static_assert(alignof(std::atomic<std::size_t>) < alignof(ValueT)
                    ? alignof(ValueT) % alignof(std::atomic<std::size_t>) == 0
                    : alignof(std::atomic<std::size_t>) % alignof(ValueT) == 0,
                "Alignment requirement of std::atomic<std::size_t> does not "
                "evenly divide with alignment requirement of ValueT.");

  static constexpr std::size_t kBucketAlignment
      = alignof(std::atomic<std::size_t>) < alignof(ValueT) ? alignof(ValueT)
                                                            : alignof(std::atomic<std::size_t>);

  // Value's offset in a bucket is the first alignof(ValueT) boundary after the
  // next pointer and hash code.
  static constexpr std::size_t kValueOffset
      = (((sizeof(std::atomic<std::size_t>) + sizeof(std::size_t) - 1) / alignof(ValueT)) + 1) * alignof(ValueT);

  // Round bucket size up to a multiple of kBucketAlignment.
  static constexpr std::size_t ComputeBucketSize(const std::size_t fixed_key_size) {
    return (((kValueOffset + sizeof(ValueT) + fixed_key_size - 1) / kBucketAlignment) + 1)
           * kBucketAlignment;
  }

  // If ValueT is not trivially destructible, invoke its destructor for all
  // values held in the specified buckets (including those in "empty" buckets
  // that were default constructed). If ValueT is trivially destructible, this
  // is a no-op.
  static void DestroyValues(void *buckets,
                            const std::size_t num_buckets,
                            const std::size_t bucket_size);

  // Attempt to find an empty bucket to insert 'hash_code' into, starting after
  // '*bucket' in the chain (or, if '*bucket' is NULL, starting from the slot
  // array). Returns true and stores SIZE_T_MAX in '*pending_chain_ptr' if an
  // empty bucket is found. Returns false if 'allow_duplicate_keys' is false
  // and a hash collision is found (caller should then check whether there is a
  // genuine key collision or the hash collision is spurious). Returns false
  // and sets '*bucket' to NULL if there are no more empty buckets in the hash
  // table. If 'variable_key_allocation_required' is nonzero, this method will
  // attempt to allocate storage for a variable-length key BEFORE allocating a
  // bucket, so that no bucket number below 'header_->num_buckets' is ever
  // deallocated after being allocated.
  inline bool locateBucketForInsertion(const std::size_t hash_code,
                                       const std::size_t variable_key_allocation_required,
                                       void **bucket,
                                       std::atomic<std::size_t> **pending_chain_ptr,
                                       std::size_t *pending_chain_ptr_finish_value,
                                       HashTablePreallocationState *prealloc_state);

  // Write a scalar 'key' and its 'hash_code' into the '*bucket', which was
  // found by locateBucketForInsertion(). Assumes that storage for a
  // variable-length key copy (if any) was already allocated by a successful
  // call to allocateVariableLengthKeyStorage().
  inline void writeScalarKeyToBucket(const TypedValue &key,
                                     const std::size_t hash_code,
                                     void *bucket,
                                     HashTablePreallocationState *prealloc_state);

  // Write a composite 'key' and its 'hash_code' into the '*bucket', which was
  // found by locateBucketForInsertion(). Assumes that storage for
  // variable-length key copies (if any) was already allocated by a successful
  // call to allocateVariableLengthKeyStorage().
  inline void writeCompositeKeyToBucket(const std::vector<TypedValue> &key,
                                        const std::size_t hash_code,
                                        void *bucket,
                                        HashTablePreallocationState *prealloc_state);

  // Determine whether it is actually necessary to resize this hash table.
  // Checks that there are sufficient unallocated buckets, and that there are
  // at least 'extra_variable_storage' bytes of variable-length storage free.
  bool isFull(const std::size_t extra_buckets,
              const std::size_t extra_variable_storage) const;

  // Helper object to manage key storage.
  HashTableKeyManager<serializable, force_key_copy> key_manager_;

  // In-memory structure is as follows:
  //   - SeparateChainingHashTable::Header
  //   - Array of slots, interpreted as follows:
  //       - 0 = Points to nothing (empty)
  //       - SIZE_T_MAX = Pending (some thread is starting a chain from this
  //         slot and will overwrite it soon)
  //       - Anything else = The number of the first bucket in the chain for
  //         this slot PLUS ONE (i.e. subtract one to get the actual bucket
  //         number).
  //   - Array of buckets, each of which is:
  //       - atomic size_t "next" pointer, interpreted the same as slots above.
  //       - size_t hash value
  //       - possibly some unused bytes as needed so that ValueT's alignment
  //         requirement is met
  //       - ValueT value slot
  //       - fixed-length key storage (which may include pointers to external
  //         memory or offsets of variable length keys stored within this hash
  //         table)
  //       - possibly some additional unused bytes so that bucket size is a
  //         multiple of both alignof(std::atomic<std::size_t>) and
  //         alignof(ValueT)
  //   - Variable-length key storage region (referenced by offsets stored in
  //     fixed-length keys).
  Header *header_;

  std::atomic<std::size_t> *slots_;
  void *buckets_;
  const std::size_t bucket_size_;

  DISALLOW_COPY_AND_ASSIGN(SeparateChainingHashTable);
};

/** @} */

// ----------------------------------------------------------------------------
// Implementations of template class methods follow.

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::SeparateChainingHashTable(const std::vector<const Type*> &key_types,
                                const std::size_t num_entries,
                                StorageManager *storage_manager)
        : HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>(
              key_types,
              num_entries,
              storage_manager,
              false,
              false,
              true),
          key_manager_(this->key_types_, kValueOffset + sizeof(ValueT)),
          bucket_size_(ComputeBucketSize(key_manager_.getFixedKeySize())) {
  // Give base HashTable information about what key components are stored
  // inline from 'key_manager_'.
  this->setKeyInline(key_manager_.getKeyInline());

  // Pick out a prime number of slots and calculate storage requirements.
  std::size_t num_slots_tmp = get_next_prime_number(num_entries * kHashTableLoadFactor);
  std::size_t required_memory = sizeof(Header)
                                + num_slots_tmp * sizeof(std::atomic<std::size_t>)
                                + (num_slots_tmp / kHashTableLoadFactor)
                                    * (bucket_size_ + key_manager_.getEstimatedVariableKeySize());
  std::size_t num_storage_slots = this->storage_manager_->SlotsNeededForBytes(required_memory);
  if (num_storage_slots == 0) {
    FATAL_ERROR("Storage requirement for SeparateChainingHashTable "
                "exceeds maximum allocation size.");
  }

  // Get a StorageBlob to hold the hash table.
  const block_id blob_id = this->storage_manager_->createBlob(num_storage_slots);
  this->blob_ = this->storage_manager_->getBlobMutable(blob_id);

  void *aligned_memory_start = this->blob_->getMemoryMutable();
  std::size_t available_memory = num_storage_slots * kSlotSizeBytes;
  if (align(alignof(Header),
            sizeof(Header),
            aligned_memory_start,
            available_memory)
          == nullptr) {
    // With current values from StorageConstants.hpp, this should be
    // impossible. A blob is at least 1 MB, while a Header has alignment
    // requirement of just kCacheLineBytes (64 bytes).
    FATAL_ERROR("StorageBlob used to hold resizable "
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
  header_ = static_cast<Header*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start) + sizeof(Header);
  available_memory -= sizeof(Header);

  // Recompute the number of slots & buckets using the actual available memory.
  // Most likely, we got some extra free bucket space due to "rounding up" to
  // the storage blob's size. It's also possible (though very unlikely) that we
  // will wind up with fewer buckets than we initially wanted because of screwy
  // alignment requirements for ValueT.
  std::size_t num_buckets_tmp
      = available_memory / (kHashTableLoadFactor * sizeof(std::atomic<std::size_t>)
                            + bucket_size_
                            + key_manager_.getEstimatedVariableKeySize());
  num_slots_tmp = get_previous_prime_number(num_buckets_tmp * kHashTableLoadFactor);
  num_buckets_tmp = num_slots_tmp / kHashTableLoadFactor;
  DEBUG_ASSERT(num_slots_tmp > 0);
  DEBUG_ASSERT(num_buckets_tmp > 0);

  // Locate the slot array.
  slots_ = static_cast<std::atomic<std::size_t>*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start)
                         + sizeof(std::atomic<std::size_t>) * num_slots_tmp;
  available_memory -= sizeof(std::atomic<std::size_t>) * num_slots_tmp;

  // Locate the buckets.
  buckets_ = aligned_memory_start;
  // Extra-paranoid: If ValueT has an alignment requirement greater than that
  // of std::atomic<std::size_t>, we may need to adjust the start of the bucket
  // array.
  if (align(kBucketAlignment,
            bucket_size_,
            buckets_,
            available_memory)
          == nullptr) {
    FATAL_ERROR("StorageBlob used to hold resizable "
                "SeparateChainingHashTable is too small to meet "
                "alignment requirements of buckets.");
  } else if (buckets_ != aligned_memory_start) {
    DEV_WARNING("Bucket array start position adjusted to meet alignment "
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
      static_cast<char*>(buckets_) + header_->num_buckets * bucket_size_,
      available_memory,
      &(header_->variable_length_bytes_allocated));
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::SeparateChainingHashTable(const std::vector<const Type*> &key_types,
                                void *hash_table_memory,
                                const std::size_t hash_table_memory_size,
                                const bool new_hash_table,
                                const bool hash_table_memory_zeroed)
        : HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>(
              key_types,
              hash_table_memory,
              hash_table_memory_size,
              new_hash_table,
              hash_table_memory_zeroed,
              false,
              false,
              true),
          key_manager_(this->key_types_, kValueOffset + sizeof(ValueT)),
          bucket_size_(ComputeBucketSize(key_manager_.getFixedKeySize())) {
  // Give base HashTable information about what key components are stored
  // inline from 'key_manager_'.
  this->setKeyInline(key_manager_.getKeyInline());

  // FIXME(chasseur): If we are reconstituting a HashTable using a block of
  // memory whose start was aligned differently than the memory block that was
  // originally used (modulo alignof(Header)), we could wind up with all of our
  // data structures misaligned. If memory is inside a
  // StorageBlock/StorageBlob, this will never occur, since the StorageManager
  // always allocates slots aligned to kCacheLineBytes. Similarly, this isn't
  // a problem for memory inside any other allocation aligned to at least
  // alignof(Header) == kCacheLineBytes.

  void *aligned_memory_start = this->hash_table_memory_;
  std::size_t available_memory = this->hash_table_memory_size_;

  if (align(alignof(Header),
            sizeof(Header),
            aligned_memory_start,
            available_memory)
          == nullptr) {
    FATAL_ERROR("Attempted to create a non-resizable "
                << "SeparateChainingHashTable with "
                << available_memory << " bytes of memory at "
                << aligned_memory_start << " which either can not fit a "
                << "SeparateChainingHashTable::Header or meet its alignement "
                << "requirement.");
  } else if (aligned_memory_start != this->hash_table_memory_) {
    // In general, we could get memory of any alignment, although at least
    // cache-line aligned would be nice.
    DEV_WARNING("StorageBlob memory adjusted by "
                << (this->hash_table_memory_size_ - available_memory)
                << " bytes to meet alignment requirement for "
                << "SeparateChainingHashTable::Header.");
  }

  header_ = static_cast<Header*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start) + sizeof(Header);
  available_memory -= sizeof(Header);

  if (new_hash_table) {
    std::size_t estimated_bucket_capacity
        = available_memory / (kHashTableLoadFactor * sizeof(std::atomic<std::size_t>)
                              + bucket_size_
                              + key_manager_.getEstimatedVariableKeySize());
    std::size_t num_slots = get_previous_prime_number(estimated_bucket_capacity * kHashTableLoadFactor);

    // Fill in the header.
    header_->num_slots = num_slots;
    header_->num_buckets = num_slots / kHashTableLoadFactor;
    header_->buckets_allocated.store(0, std::memory_order_relaxed);
    header_->variable_length_bytes_allocated.store(0, std::memory_order_relaxed);
  }

  // Locate the slot array.
  slots_ = static_cast<std::atomic<std::size_t>*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start)
                         + sizeof(std::atomic<std::size_t>) * header_->num_slots;
  available_memory -= sizeof(std::atomic<std::size_t>) * header_->num_slots;

  if (new_hash_table && !hash_table_memory_zeroed) {
    std::memset(slots_, 0x0, sizeof(std::atomic<std::size_t>) * header_->num_slots);
  }

  // Locate the buckets.
  buckets_ = aligned_memory_start;
  // Extra-paranoid: sizeof(Header) should almost certainly be a multiple of
  // kBucketAlignment, unless ValueT has some members with seriously big
  // (> kCacheLineBytes) alignment requirements specified using alignas().
  if (align(kBucketAlignment,
            bucket_size_,
            buckets_,
            available_memory)
          == nullptr) {
    FATAL_ERROR("Attempted to create a non-resizable "
                << "SeparateChainingHashTable with "
                << this->hash_table_memory_size_ << " bytes of memory at "
                << this->hash_table_memory_ << ", which can hold an aligned "
                << "SeparateChainingHashTable::Header but does not have "
                << "enough remaining space for even a single hash bucket.");
  } else if (buckets_ != aligned_memory_start) {
    DEV_WARNING("Bucket array start position adjusted to meet alignment "
                "requirement for SeparateChainingHashTable's value type.");
    if (header_->num_buckets * bucket_size_ > available_memory) {
      DEBUG_ASSERT(new_hash_table);
      --(header_->num_buckets);
    }
  }
  available_memory -= bucket_size_ * header_->num_buckets;

  // Make sure "next" pointers in buckets are zeroed-out.
  if (new_hash_table && !hash_table_memory_zeroed) {
    std::memset(buckets_, 0x0, header_->num_buckets * bucket_size_);
  }

  // Locate variable-length key storage region.
  key_manager_.setVariableLengthStorageInfo(
      static_cast<char*>(buckets_) + header_->num_buckets * bucket_size_,
      available_memory,
      &(header_->variable_length_bytes_allocated));
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::clear() {
  const std::size_t used_buckets = header_->buckets_allocated.load(std::memory_order_relaxed);
  // Destroy existing values, if necessary.
  DestroyValues(buckets_,
                used_buckets,
                bucket_size_);

  // Zero-out slot array.
  std::memset(slots_, 0x0, sizeof(std::atomic<std::size_t>) * header_->num_slots);

  // Zero-out used buckets.
  std::memset(buckets_, 0x0, used_buckets * bucket_size_);

  header_->buckets_allocated.store(0, std::memory_order_relaxed);
  header_->variable_length_bytes_allocated.store(0, std::memory_order_relaxed);
  key_manager_.zeroNextVariableLengthKeyOffset();
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
const ValueT* SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getSingle(const TypedValue &key) const {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = key.getHash();
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Match located.
      return reinterpret_cast<const ValueT*>(bucket + kValueOffset);
    }
    bucket_ref = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
  }

  // Reached the end of the chain and didn't find a match.
  return nullptr;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
const ValueT* SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getSingleCompositeKey(const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->hashCompositeKey(key);
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      return reinterpret_cast<const ValueT*>(bucket + kValueOffset);
    }
    bucket_ref = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
  }

  // Reached the end of the chain and didn't find a match.
  return nullptr;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAll(const TypedValue &key, std::vector<const ValueT*> *values) const {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = key.getHash();
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Match located.
      values->push_back(reinterpret_cast<const ValueT*>(bucket + kValueOffset));
      if (!allow_duplicate_keys) {
        return;
      }
    }
    bucket_ref = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAllCompositeKey(const std::vector<TypedValue> &key, std::vector<const ValueT*> *values) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->hashCompositeKey(key);
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      values->push_back(reinterpret_cast<const ValueT*>(bucket + kValueOffset));
      if (!allow_duplicate_keys) {
        return;
      }
    }
    bucket_ref = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
HashTablePutResult
    SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
        ::putInternal(const TypedValue &key,
                      const std::size_t variable_key_size,
                      const ValueT &value,
                      HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  if (prealloc_state == nullptr) {
    // Early check for a free bucket.
    if (header_->buckets_allocated.load(std::memory_order_relaxed) >= header_->num_buckets) {
      return HashTablePutResult::kOutOfSpace;
    }

    // TODO(chasseur): If allow_duplicate_keys is true, avoid storing more than
    // one copy of the same variable-length key.
    if (!key_manager_.allocateVariableLengthKeyStorage(variable_key_size)) {
      // Ran out of variable-length key storage space.
      return HashTablePutResult::kOutOfSpace;
    }
  }

  const std::size_t hash_code = key.getHash();
  void *bucket = nullptr;
  std::atomic<std::size_t> *pending_chain_ptr;
  std::size_t pending_chain_ptr_finish_value;
  for (;;) {
    if (locateBucketForInsertion(hash_code,
                                 0,
                                 &bucket,
                                 &pending_chain_ptr,
                                 &pending_chain_ptr_finish_value,
                                 prealloc_state)) {
      // Found an empty bucket.
      break;
    } else if (bucket == nullptr) {
      // Ran out of buckets. Deallocate any variable space that we were unable
      // to use.
      DEBUG_ASSERT(prealloc_state == nullptr);
      key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
      return HashTablePutResult::kOutOfSpace;
    } else {
      // Hash collision found, and duplicates aren't allowed.
      DEBUG_ASSERT(!allow_duplicate_keys);
      DEBUG_ASSERT(prealloc_state == nullptr);
      if (key_manager_.scalarKeyCollisionCheck(key, bucket)) {
        // Duplicate key. Deallocate any variable storage space and return.
        key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
        return HashTablePutResult::kDuplicateKey;
      }
    }
  }

  // Write the key and hash.
  writeScalarKeyToBucket(key, hash_code, bucket, prealloc_state);

  // Store the value by using placement new with ValueT's copy constructor.
  new(static_cast<char*>(bucket) + kValueOffset) ValueT(value);

  // Update the previous chain pointer to point to the new bucket.
  pending_chain_ptr->store(pending_chain_ptr_finish_value, std::memory_order_release);

  // We're all done.
  return HashTablePutResult::kOK;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
HashTablePutResult
    SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
        ::putCompositeKeyInternal(const std::vector<TypedValue> &key,
                                  const std::size_t variable_key_size,
                                  const ValueT &value,
                                  HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  if (prealloc_state == nullptr) {
    // Early check for a free bucket.
    if (header_->buckets_allocated.load(std::memory_order_relaxed) >= header_->num_buckets) {
      return HashTablePutResult::kOutOfSpace;
    }

    // TODO(chasseur): If allow_duplicate_keys is true, avoid storing more than
    // one copy of the same variable-length key.
    if (!key_manager_.allocateVariableLengthKeyStorage(variable_key_size)) {
      // Ran out of variable-length key storage space.
      return HashTablePutResult::kOutOfSpace;
    }
  }

  const std::size_t hash_code = this->hashCompositeKey(key);
  void *bucket = nullptr;
  std::atomic<std::size_t> *pending_chain_ptr;
  std::size_t pending_chain_ptr_finish_value;
  for (;;) {
    if (locateBucketForInsertion(hash_code,
                                 0,
                                 &bucket,
                                 &pending_chain_ptr,
                                 &pending_chain_ptr_finish_value,
                                 prealloc_state)) {
      // Found an empty bucket.
      break;
    } else if (bucket == nullptr) {
      // Ran out of buckets. Deallocate any variable space that we were unable
      // to use.
      DEBUG_ASSERT(prealloc_state == nullptr);
      key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
      return HashTablePutResult::kOutOfSpace;
    } else {
      // Hash collision found, and duplicates aren't allowed.
      DEBUG_ASSERT(!allow_duplicate_keys);
      DEBUG_ASSERT(prealloc_state == nullptr);
      if (key_manager_.compositeKeyCollisionCheck(key, bucket)) {
        // Duplicate key. Deallocate any variable storage space and return.
        key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
        return HashTablePutResult::kDuplicateKey;
      }
    }
  }

  // Write the key and hash.
  writeCompositeKeyToBucket(key, hash_code, bucket, prealloc_state);

  // Store the value by using placement new with ValueT's copy constructor.
  new(static_cast<char*>(bucket) + kValueOffset) ValueT(value);

  // Update the previous chain pointer to point to the new bucket.
  pending_chain_ptr->store(pending_chain_ptr_finish_value, std::memory_order_release);

  // We're all done.
  return HashTablePutResult::kOK;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
ValueT* SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsertInternal(const TypedValue &key,
                     const std::size_t variable_key_size,
                     const ValueT &initial_value) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  if (variable_key_size > 0) {
    // Don't allocate yet, since the key may already be present. However, we
    // do check if either the allocated variable storage space OR the free
    // space is big enough to hold the key (at least one must be true: either
    // the key is already present and allocated, or we need to be able to
    // allocate enough space for it).
    std::size_t allocated_bytes = header_->variable_length_bytes_allocated.load(std::memory_order_relaxed);
    if ((allocated_bytes < variable_key_size)
        && (allocated_bytes + variable_key_size > key_manager_.getVariableLengthKeyStorageSize())) {
      return nullptr;
    }
  }

  const std::size_t hash_code = key.getHash();
  void *bucket = nullptr;
  std::atomic<std::size_t> *pending_chain_ptr;
  std::size_t pending_chain_ptr_finish_value;
  for (;;) {
    if (locateBucketForInsertion(hash_code,
                                 variable_key_size,
                                 &bucket,
                                 &pending_chain_ptr,
                                 &pending_chain_ptr_finish_value,
                                 nullptr)) {
      // Found an empty bucket.
      break;
    } else if (bucket == nullptr) {
      // Ran out of buckets or variable-key space.
      return nullptr;
    } else if (key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Found an already-existing entry for this key.
      return reinterpret_cast<ValueT*>(static_cast<char*>(bucket) + kValueOffset);
    }
  }

  // We are now writing to an empty bucket.
  // Write the key and hash.
  writeScalarKeyToBucket(key, hash_code, bucket, nullptr);

  // Copy the supplied 'initial_value' into place.
  ValueT *value = new(static_cast<char*>(bucket) + kValueOffset) ValueT(initial_value);

  // Update the previous chain pointer to point to the new bucket.
  pending_chain_ptr->store(pending_chain_ptr_finish_value, std::memory_order_release);

  // Return the value.
  return value;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
ValueT* SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsertCompositeKeyInternal(const std::vector<TypedValue> &key,
                                 const std::size_t variable_key_size,
                                 const ValueT &initial_value) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  if (variable_key_size > 0) {
    // Don't allocate yet, since the key may already be present. However, we
    // do check if either the allocated variable storage space OR the free
    // space is big enough to hold the key (at least one must be true: either
    // the key is already present and allocated, or we need to be able to
    // allocate enough space for it).
    std::size_t allocated_bytes = header_->variable_length_bytes_allocated.load(std::memory_order_relaxed);
    if ((allocated_bytes < variable_key_size)
        && (allocated_bytes + variable_key_size > key_manager_.getVariableLengthKeyStorageSize())) {
      return nullptr;
    }
  }

  const std::size_t hash_code = this->hashCompositeKey(key);
  void *bucket = nullptr;
  std::atomic<std::size_t> *pending_chain_ptr;
  std::size_t pending_chain_ptr_finish_value;
  for (;;) {
    if (locateBucketForInsertion(hash_code,
                                 variable_key_size,
                                 &bucket,
                                 &pending_chain_ptr,
                                 &pending_chain_ptr_finish_value,
                                 nullptr)) {
      // Found an empty bucket.
      break;
    } else if (bucket == nullptr) {
      // Ran out of buckets or variable-key space.
      return nullptr;
    } else if (key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Found an already-existing entry for this key.
      return reinterpret_cast<ValueT*>(static_cast<char*>(bucket) + kValueOffset);
    }
  }

  // We are now writing to an empty bucket.
  // Write the key and hash.
  writeCompositeKeyToBucket(key, hash_code, bucket, nullptr);

  // Copy the supplied 'initial_value' into place.
  ValueT *value = new(static_cast<char*>(bucket) + kValueOffset) ValueT(initial_value);

  // Update the previous chaing pointer to point to the new bucket.
  pending_chain_ptr->store(pending_chain_ptr_finish_value, std::memory_order_release);

  // Return the value.
  return value;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntry(TypedValue *key, const ValueT **value, std::size_t *entry_num) const {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  if (*entry_num < header_->buckets_allocated.load(std::memory_order_relaxed)) {
    const char *bucket = static_cast<const char*>(buckets_) + (*entry_num) * bucket_size_;
    *key = key_manager_.getKeyComponentTyped(bucket, 0);
    *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);
    ++(*entry_num);
    return true;
  } else {
    return false;
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntryCompositeKey(std::vector<TypedValue> *key,
                               const ValueT **value,
                               std::size_t *entry_num) const {
  if (*entry_num < header_->buckets_allocated.load(std::memory_order_relaxed)) {
    const char *bucket = static_cast<const char*>(buckets_) + (*entry_num) * bucket_size_;
    for (std::vector<const Type*>::size_type key_idx = 0;
         key_idx < this->key_types_.size();
         ++key_idx) {
      key->emplace_back(key_manager_.getKeyComponentTyped(bucket, key_idx));
    }
    *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);
    ++(*entry_num);
    return true;
  } else {
    return false;
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntryForKey(const TypedValue &key,
                         const std::size_t hash_code,
                         const ValueT **value,
                         std::size_t *entry_num) const {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  if (*entry_num == 0) {
    *entry_num = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  } else if (*entry_num == std::numeric_limits<std::size_t>::max()) {
    return false;
  }

  while (*entry_num != 0) {
    DEBUG_ASSERT(*entry_num != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (*entry_num - 1) * bucket_size_;
    *entry_num = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Match located.
      *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);
      if (*entry_num == 0) {
        // If this is the last bucket in the chain, prevent the next call from
        // starting over again.
        *entry_num = std::numeric_limits<std::size_t>::max();
      }
      return true;
    }
  }

  // Reached the end of the chain.
  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntryForCompositeKey(const std::vector<TypedValue> &key,
                                  const std::size_t hash_code,
                                  const ValueT **value,
                                  std::size_t *entry_num) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  if (*entry_num == 0) {
    *entry_num = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  } else if (*entry_num == std::numeric_limits<std::size_t>::max()) {
    return false;
  }

  while (*entry_num != 0) {
    DEBUG_ASSERT(*entry_num != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (*entry_num - 1) * bucket_size_;
    *entry_num = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);
      if (*entry_num == 0) {
        // If this is the last bucket in the chain, prevent the next call from
        // starting over again.
        *entry_num = std::numeric_limits<std::size_t>::max();
      }
      return true;
    }
  }

  // Reached the end of the chain.
  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::hasKey(const TypedValue &key) const {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = key.getHash();
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Find a match.
      return true;
    }
    bucket_ref = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
  }
  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::hasCompositeKey(const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->hashCompositeKey(key);
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket = static_cast<const char*>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t*>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Find a match.
      return true;
    }
    bucket_ref = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
  }
  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::resize(const std::size_t extra_buckets,
             const std::size_t extra_variable_storage,
             const std::size_t retry_num) {
  DEBUG_ASSERT(resizable);

  // A retry should never be necessary with this implementation of HashTable.
  // Separate chaining ensures that any resized hash table with more buckets
  // than the original table will be able to hold more entries than the
  // original.
  DEBUG_ASSERT(retry_num == 0);

  SpinSharedMutexExclusiveLock<true> write_lock(this->resize_shared_mutex_);

  // Recheck whether the hash table is still full. Note that multiple threads
  // might wait to rebuild this hash table simultaneously. Only the first one
  // should do the rebuild.
  if (!isFull(extra_buckets, extra_variable_storage)) {
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
  std::size_t variable_storage_required
      = (resized_num_slots / kHashTableLoadFactor) * key_manager_.getEstimatedVariableKeySize();
  const std::size_t original_variable_storage_used
      = header_->variable_length_bytes_allocated.load(std::memory_order_relaxed);
  // If this resize was triggered by a too-large variable-length key, bump up
  // the variable-length storage requirement.
  if ((extra_variable_storage > 0)
      && (extra_variable_storage + original_variable_storage_used
          > key_manager_.getVariableLengthKeyStorageSize())) {
    variable_storage_required += extra_variable_storage;
  }

  const std::size_t resized_memory_required
      = sizeof(Header)
        + resized_num_slots * sizeof(std::atomic<std::size_t>)
        + (resized_num_slots / kHashTableLoadFactor) * bucket_size_
        + variable_storage_required;
  const std::size_t resized_storage_slots
      = this->storage_manager_->SlotsNeededForBytes(resized_memory_required);
  if (resized_storage_slots == 0) {
    FATAL_ERROR("Storage requirement for resized SeparateChainingHashTable "
                "exceeds maximum allocation size.");
  }

  // Get a new StorageBlob to hold the resized hash table.
  const block_id resized_blob_id = this->storage_manager_->createBlob(resized_storage_slots);
  MutableBlobReference resized_blob = this->storage_manager_->getBlobMutable(resized_blob_id);

  // Locate data structures inside the new StorageBlob.
  void *aligned_memory_start = resized_blob->getMemoryMutable();
  std::size_t available_memory = resized_storage_slots * kSlotSizeBytes;
  if (align(alignof(Header),
            sizeof(Header),
            aligned_memory_start,
            available_memory)
          == nullptr) {
    // Should be impossible, as noted in constructor.
    FATAL_ERROR("StorageBlob used to hold resized SeparateChainingHashTable "
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

  Header *resized_header = static_cast<Header*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start) + sizeof(Header);
  available_memory -= sizeof(Header);

  // As in constructor, recompute the number of slots and buckets using the
  // actual available memory.
  std::size_t resized_num_buckets
      = (available_memory - extra_variable_storage)
        / (kHashTableLoadFactor * sizeof(std::atomic<std::size_t>)
           + bucket_size_
           + key_manager_.getEstimatedVariableKeySize());
  resized_num_slots = get_previous_prime_number(resized_num_buckets * kHashTableLoadFactor);
  resized_num_buckets = resized_num_slots / kHashTableLoadFactor;

  // Locate slot array.
  std::atomic<std::size_t> *resized_slots = static_cast<std::atomic<std::size_t>*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start)
                         + sizeof(std::atomic<std::size_t>) * resized_num_slots;
  available_memory -= sizeof(std::atomic<std::size_t>) * resized_num_slots;

  // As in constructor, we will be extra paranoid and use align() to locate the
  // start of the array of buckets, as well.
  void *resized_buckets = aligned_memory_start;
  if (align(kBucketAlignment,
            bucket_size_,
            resized_buckets,
            available_memory)
          == nullptr) {
    FATAL_ERROR("StorageBlob used to hold resized SeparateChainingHashTable "
                "is too small to meet alignment requirements of buckets.");
  } else if (resized_buckets != aligned_memory_start) {
    DEV_WARNING("Bucket array start position adjusted to meet alignment "
                "requirement for SeparateChainingHashTable's value type.");
    if (resized_num_buckets * bucket_size_ + variable_storage_required > available_memory) {
      --resized_num_buckets;
    }
  }
  aligned_memory_start = static_cast<char*>(aligned_memory_start)
                         + resized_num_buckets * bucket_size_;
  available_memory -= resized_num_buckets * bucket_size_;

  void *resized_variable_length_key_storage = aligned_memory_start;
  const std::size_t resized_variable_length_key_storage_size = available_memory;

  const std::size_t original_buckets_used = header_->buckets_allocated.load(std::memory_order_relaxed);

  // Initialize the header.
  resized_header->num_slots = resized_num_slots;
  resized_header->num_buckets = resized_num_buckets;
  resized_header->buckets_allocated.store(original_buckets_used, std::memory_order_relaxed);
  resized_header->variable_length_bytes_allocated.store(
      original_variable_storage_used,
      std::memory_order_relaxed);

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
  std::memcpy(resized_buckets, buckets_, original_buckets_used * bucket_size_);

  // TODO(chasseur): std::is_trivially_copyable is not yet implemented in
  // GCC 4.8.3, so we assume we need to invoke ValueT's copy or move
  // constructor, even though the plain memcpy above could suffice for many
  // possible ValueTs.
  void *current_value_original = static_cast<char*>(buckets_) + kValueOffset;
  void *current_value_resized = static_cast<char*>(resized_buckets) + kValueOffset;
  for (std::size_t bucket_num = 0; bucket_num < original_buckets_used; ++bucket_num) {
    // Use a move constructor if available to avoid a deep-copy, since resizes
    // always succeed.
    new (current_value_resized) ValueT(std::move(*static_cast<ValueT*>(current_value_original)));
    current_value_original = static_cast<char*>(current_value_original) + bucket_size_;
    current_value_resized = static_cast<char*>(current_value_resized) + bucket_size_;
  }

  // Copy over variable-length key components, if any.
  if (original_variable_storage_used > 0) {
    DEBUG_ASSERT(original_variable_storage_used
                 == key_manager_.getNextVariableLengthKeyOffset());
    DEBUG_ASSERT(original_variable_storage_used <= resized_variable_length_key_storage_size);
    std::memcpy(resized_variable_length_key_storage,
                key_manager_.getVariableLengthKeyStorage(),
                original_variable_storage_used);
  }

  // Destroy values in the original hash table, if neccesary,
  DestroyValues(buckets_,
                original_buckets_used,
                bucket_size_);

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
  for (std::size_t bucket_num = 0; bucket_num < original_buckets_used; ++bucket_num) {
    std::atomic<std::size_t> *next_ptr
        = static_cast<std::atomic<std::size_t>*>(current_bucket);
    const std::size_t hash_code = *reinterpret_cast<const std::size_t*>(
        static_cast<const char*>(current_bucket) + sizeof(std::atomic<std::size_t>));

    const std::size_t slot_number = hash_code % header_->num_slots;
    std::size_t slot_ptr_value = 0;
    if (slots_[slot_number].compare_exchange_strong(slot_ptr_value,
                                                    bucket_num + 1,
                                                    std::memory_order_relaxed)) {
      // This bucket is the first in the chain for this block, so reset its
      // next pointer to 0.
      next_ptr->store(0, std::memory_order_relaxed);
    } else {
      // A chain already exists starting from this slot, so put this bucket at
      // the head.
      next_ptr->store(slot_ptr_value, std::memory_order_relaxed);
      slots_[slot_number].store(bucket_num + 1, std::memory_order_relaxed);
    }
    current_bucket = static_cast<char*>(current_bucket) + bucket_size_;
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::preallocateForBulkInsert(const std::size_t total_entries,
                               const std::size_t total_variable_key_size,
                               HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT(allow_duplicate_keys);
  if (!key_manager_.allocateVariableLengthKeyStorage(total_variable_key_size)) {
    return false;
  }

  // We use load then compare-exchange here instead of simply fetch-add,
  // because if multiple threads are simultaneously trying to allocate more
  // than one bucket and exceed 'header_->num_buckets', their respective
  // rollbacks might happen in such an order that some bucket ranges get
  // skipped, while others might get double-allocated later.
  std::size_t original_buckets_allocated = header_->buckets_allocated.load(std::memory_order_relaxed);
  std::size_t buckets_post_allocation = original_buckets_allocated + total_entries;
  while ((buckets_post_allocation <= header_->num_buckets)
         && !header_->buckets_allocated.compare_exchange_weak(original_buckets_allocated,
                                                              buckets_post_allocation,
                                                              std::memory_order_relaxed)) {
    buckets_post_allocation = original_buckets_allocated + total_entries;
  }

  if (buckets_post_allocation > header_->num_buckets) {
    key_manager_.deallocateVariableLengthKeyStorage(total_variable_key_size);
    return false;
  }

  prealloc_state->bucket_position = original_buckets_allocated;
  if (total_variable_key_size != 0) {
    prealloc_state->variable_length_key_position
        = key_manager_.incrementNextVariableLengthKeyOffset(total_variable_key_size);
  }
  return true;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::DestroyValues(void *hash_buckets,
                    const std::size_t num_buckets,
                    const std::size_t bucket_size) {
  if (!std::is_trivially_destructible<ValueT>::value) {
    void *value_ptr = static_cast<char*>(hash_buckets) + kValueOffset;
    for (std::size_t bucket_num = 0;
         bucket_num < num_buckets;
         ++bucket_num) {
      static_cast<ValueT*>(value_ptr)->~ValueT();
      value_ptr = static_cast<char*>(value_ptr) + bucket_size;
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::locateBucketForInsertion(const std::size_t hash_code,
                               const std::size_t variable_key_allocation_required,
                               void **bucket,
                               std::atomic<std::size_t> **pending_chain_ptr,
                               std::size_t *pending_chain_ptr_finish_value,
                               HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT((prealloc_state == nullptr) || allow_duplicate_keys);
  if (*bucket == nullptr) {
    *pending_chain_ptr = &(slots_[hash_code % header_->num_slots]);
  } else {
    *pending_chain_ptr = static_cast<std::atomic<std::size_t>*>(*bucket);
  }
  for (;;) {
    std::size_t existing_chain_ptr = 0;
    if ((*pending_chain_ptr)->compare_exchange_strong(existing_chain_ptr,
                                                      std::numeric_limits<std::size_t>::max(),
                                                      std::memory_order_acq_rel)) {
      // Got to the end of the chain. Allocate a new bucket.

      // First, allocate variable-length key storage, if needed (i.e. if this
      // is an upsert and we didn't allocate up-front).
      if ((prealloc_state == nullptr)
          && !key_manager_.allocateVariableLengthKeyStorage(variable_key_allocation_required)) {
        // Ran out of variable-length storage.
        (*pending_chain_ptr)->store(0, std::memory_order_release);
        *bucket = nullptr;
        return false;
      }

      const std::size_t allocated_bucket_num
          = (prealloc_state == nullptr)
              ? header_->buckets_allocated.fetch_add(1, std::memory_order_relaxed)
              : (prealloc_state->bucket_position)++;
      if (allocated_bucket_num >= header_->num_buckets) {
        // Ran out of buckets.
        DEBUG_ASSERT(prealloc_state == nullptr);
        header_->buckets_allocated.fetch_sub(1, std::memory_order_relaxed);
        (*pending_chain_ptr)->store(0, std::memory_order_release);
        *bucket = nullptr;
        return false;
      } else {
        *bucket = static_cast<char*>(buckets_) + allocated_bucket_num * bucket_size_;
        *pending_chain_ptr_finish_value = allocated_bucket_num + 1;
        return true;
      }
    }
    // Spin until the real "next" pointer is available.
    while (existing_chain_ptr == std::numeric_limits<std::size_t>::max()) {
      existing_chain_ptr = (*pending_chain_ptr)->load(std::memory_order_acquire);
    }
    if (existing_chain_ptr == 0) {
      // Other thread had to roll back, so try again.
      continue;
    }
    // Chase the next pointer.
    *bucket = static_cast<char*>(buckets_) + (existing_chain_ptr - 1) * bucket_size_;
    *pending_chain_ptr = static_cast<std::atomic<std::size_t>*>(*bucket);
    if (!allow_duplicate_keys) {
      const std::size_t hash_in_bucket
          = *reinterpret_cast<const std::size_t*>(static_cast<const char*>(*bucket)
                                                  + sizeof(std::atomic<std::size_t>));
      if (hash_in_bucket == hash_code) {
        return false;
      }
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline void SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::writeScalarKeyToBucket(const TypedValue &key,
                             const std::size_t hash_code,
                             void *bucket,
                             HashTablePreallocationState *prealloc_state) {
  *reinterpret_cast<std::size_t*>(static_cast<char*>(bucket) + sizeof(std::atomic<std::size_t>))
      = hash_code;
  key_manager_.writeKeyComponentToBucket(key, 0, bucket, prealloc_state);
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline void SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::writeCompositeKeyToBucket(const std::vector<TypedValue> &key,
                                const std::size_t hash_code,
                                void *bucket,
                                HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT(key.size() == this->key_types_.size());
  *reinterpret_cast<std::size_t*>(static_cast<char*>(bucket) + sizeof(std::atomic<std::size_t>))
      = hash_code;
  for (std::size_t idx = 0;
       idx < this->key_types_.size();
       ++idx) {
    key_manager_.writeKeyComponentToBucket(key[idx], idx, bucket, prealloc_state);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SeparateChainingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::isFull(const std::size_t extra_buckets,
             const std::size_t extra_variable_storage) const {
  if (header_->buckets_allocated.load(std::memory_order_relaxed)
          + extra_buckets >= header_->num_buckets) {
    // Not enough buckets.
    return true;
  }

  if (extra_variable_storage > 0) {
    if (extra_variable_storage
            + header_->variable_length_bytes_allocated.load(std::memory_order_relaxed)
        > key_manager_.getVariableLengthKeyStorageSize()) {
      // Not enough variable-length key storage space.
      return true;
    }
  }

  return false;
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_SEPARATE_CHAINING_HASH_TABLE_HPP_
