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

#ifndef QUICKSTEP_STORAGE_SIMPLE_SCALAR_SEPARATE_CHAINING_HASH_TABLE_HPP_
#define QUICKSTEP_STORAGE_SIMPLE_SCALAR_SEPARATE_CHAINING_HASH_TABLE_HPP_

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstring>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include "storage/HashTable.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/Alignment.hpp"
#include "utility/Macros.hpp"
#include "utility/PrimeNumber.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A lean simplified version of SeparateChainingHashTable that only
 *        works for scalar (non-composite) keys that have a reversible hash
 *        function.
 *
 * The simplified code in this version of HashTable is mostly made possible by
 * the fact that the key Type has a reversible hash function. i.e. The original
 * key can always be recovered from the hash value, and the hash-function is
 * collision-free (although it is still possible for different hashes to have
 * the same remainder modulo the number of slots in the HashTable and wind up
 * in the same slot). So, this HashTable implementation only stores hash codes
 * and not copies of the original keys. Besides saving space, this also means
 * that all code for managing variable-length and/or composite keys can can be
 * omitted, and that exact equality of hash codes (a simple integer comparison)
 * can always be used to detect if keys collide.
 **/
template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
class SimpleScalarSeparateChainingHashTable : public HashTable<ValueT,
                                                               resizable,
                                                               serializable,
                                                               force_key_copy,
                                                               allow_duplicate_keys> {
 public:
  SimpleScalarSeparateChainingHashTable(const std::vector<const Type*> &key_types,
                                        const std::size_t num_entries,
                                        StorageManager *storage_manager);

  SimpleScalarSeparateChainingHashTable(const std::vector<const Type*> &key_types,
                                        void *hash_table_memory,
                                        const std::size_t hash_table_memory_size,
                                        const bool new_hash_table,
                                        const bool hash_table_memory_zeroed);

  // Delegating constructors for single scalar keys.
  SimpleScalarSeparateChainingHashTable(const Type &key_type,
                                        const std::size_t num_entries,
                                        StorageManager *storage_manager)
      : SimpleScalarSeparateChainingHashTable(std::vector<const Type*>(1, &key_type),
                                              num_entries,
                                              storage_manager) {
  }

  SimpleScalarSeparateChainingHashTable(const Type &key_type,
                                        void *hash_table_memory,
                                        const std::size_t hash_table_memory_size,
                                        const bool new_hash_table,
                                        const bool hash_table_memory_zeroed)
      : SimpleScalarSeparateChainingHashTable(std::vector<const Type*>(1, &key_type),
                                              hash_table_memory,
                                              hash_table_memory_size,
                                              new_hash_table,
                                              hash_table_memory_zeroed) {
  }

  ~SimpleScalarSeparateChainingHashTable() override {
    DestroyValues(buckets_,
                  header_->buckets_allocated.load(std::memory_order_relaxed));
  }

  void clear() override;

  std::size_t numEntries() const override {
    return header_->buckets_allocated.load(std::memory_order_relaxed);
  }

  const ValueT* getSingle(const TypedValue &key) const override;

  const ValueT* getSingleCompositeKey(const std::vector<TypedValue> &key) const override {
    DCHECK_EQ(1u, key.size());
    return getSingle(key.front());
  }

  void getAll(const TypedValue &key,
              std::vector<const ValueT*> *values) const override;

  void getAllCompositeKey(const std::vector<TypedValue> &key,
                          std::vector<const ValueT*> *values) const override {
    DCHECK_EQ(1u, key.size());
    return getAll(key.front(), values);
  }

 protected:
  HashTablePutResult putInternal(const TypedValue &key,
                                 const std::size_t variable_key_size,
                                 const ValueT &value,
                                 HashTablePreallocationState *prealloc_state) override;

  HashTablePutResult putCompositeKeyInternal(
      const std::vector<TypedValue> &key,
      const std::size_t variable_key_size,
      const ValueT &value,
      HashTablePreallocationState *prealloc_state) override {
    DCHECK_EQ(1u, key.size());
    return putInternal(key.front(), variable_key_size, value, prealloc_state);
  }

  ValueT* upsertInternal(const TypedValue &key,
                         const std::size_t variable_key_size,
                         const ValueT &initial_value) override;

  ValueT* upsertCompositeKeyInternal(const std::vector<TypedValue> &key,
                                     const std::size_t variable_key_size,
                                     const ValueT &initial_value) override {
    DCHECK_EQ(1u, key.size());
    return upsertInternal(key.front(), variable_key_size, initial_value);
  }

  bool getNextEntry(TypedValue *key,
                    const ValueT **value,
                    std::size_t *entry_num) const override;

  bool getNextEntryCompositeKey(std::vector<TypedValue> *key,
                                const ValueT **value,
                                std::size_t *entry_num) const override {
    key->resize(1);
    return getNextEntry(&(key->front()), value, entry_num);
  }

  bool getNextEntryForKey(const TypedValue &key,
                          const std::size_t hash_code,
                          const ValueT **value,
                          std::size_t *entry_num) const override;

  bool getNextEntryForCompositeKey(const std::vector<TypedValue> &key,
                                   const std::size_t hash_code,
                                   const ValueT **value,
                                   std::size_t *entry_num) const override {
    DCHECK_EQ(1u, key.size());
    return getNextEntryForKey(key.front(), hash_code, value, entry_num);
  }

  bool hasKey(const TypedValue &key) const override;

  bool hasCompositeKey(const std::vector<TypedValue> &key) const override {
    return false;
  }

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
  };

  struct Bucket {
    std::atomic<std::size_t> next;
    std::size_t hash;
    ValueT value;
  };

  // If ValueT is not trivially destructible, invoke its destructor for all
  // values held in the specified buckets (including those in "empty" buckets
  // that were default constructed). If ValueT is trivially destructible, this
  // is a no-op.
  static void DestroyValues(Bucket *buckets,
                            const std::size_t num_buckets);

  // Attempt to find an empty bucket to insert 'hash_code' into, starting after
  // '*bucket' in the chain (or, if '*bucket' is NULL, starting from the slot
  // array). Returns true and stores SIZE_T_MAX in '*pending_chain_ptr' if an
  // empty bucket is found. Returns false if 'allow_duplicate_keys' is false
  // and a hash collision is found. Returns false and sets '*bucket' to NULL if
  // there are no more empty buckets in the hash table.
  inline bool locateBucketForInsertion(const std::size_t hash_code,
                                       Bucket **bucket,
                                       std::atomic<std::size_t> **pending_chain_ptr,
                                       std::size_t *pending_chain_ptr_finish_value,
                                       HashTablePreallocationState *prealloc_state);

  // Determine whether it is actually necessary to resize this hash table.
  // Checks that there are at least ``extra_buckets`` unallocated buckets.
  inline bool isFull(const std::size_t extra_buckets) const {
    return (header_->buckets_allocated.load(std::memory_order_relaxed) +
            extra_buckets) >= header_->num_buckets;
  }

  // Cache the TypeID of the key.
  const TypeID key_type_id_;

  // In-memory structure is as follows:
  //   - SimpleScalarSeparateChainingHashTable::Header
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
  //       - ValueT value slot
  Header *header_;

  std::atomic<std::size_t> *slots_;
  Bucket *buckets_;

  DISALLOW_COPY_AND_ASSIGN(SimpleScalarSeparateChainingHashTable);
};

/** @} */

namespace simple_scalar_separate_chaining_hash_table_detail {

// Always has a single element 'true'.
extern const std::vector<bool> kKeyInlineGlobal;

}  // namespace simple_scalar_separate_chaining_hash_table_detail

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
SimpleScalarSeparateChainingHashTable<ValueT,
                                      resizable,
                                      serializable,
                                      force_key_copy,
                                      allow_duplicate_keys>
    ::SimpleScalarSeparateChainingHashTable(const std::vector<const Type*> &key_types,
                                            const std::size_t num_entries,
                                            StorageManager *storage_manager)
        : HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>(
              key_types,
              num_entries,
              storage_manager,
              false,
              true,
              true),
          key_type_id_(key_types.front()->getTypeID()) {
  DCHECK_EQ(1u, key_types.size());
  DCHECK(TypedValue::HashIsReversible(key_type_id_));

  // Give base HashTable information about what key components are stored
  // inline (SimpleScalarSeparateChainingHashTable ALWAYS stores keys inline).
  this->setKeyInline(&simple_scalar_separate_chaining_hash_table_detail::kKeyInlineGlobal);

  // Pick out a prime number of slots and calculate storage requirements.
  std::size_t num_slots_tmp = get_next_prime_number(num_entries * kHashTableLoadFactor);
  std::size_t required_memory = sizeof(Header)
                                + num_slots_tmp * sizeof(std::atomic<std::size_t>)
                                + (num_slots_tmp / kHashTableLoadFactor) * sizeof(Bucket);
  std::size_t num_storage_slots = this->storage_manager_->SlotsNeededForBytes(required_memory);
  if (num_storage_slots == 0) {
    LOG(FATAL)
        << "Storage requirement for SimpleScalarSeparateChainingHashTable "
           "exceeds maximum allocation size.";
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
    LOG(FATAL)
        << "StorageBlob used to hold resizable "
           "SimpleScalarSeparateChainingHashTable is too small to meet "
           "requirements of SimpleScalarSeparateChainingHashTable::Header.";
  } else if (aligned_memory_start != this->blob_->getMemoryMutable()) {
    // This should also be impossible, since the StorageManager allocates slots
    // aligned to kCacheLineBytes.
    LOG(WARNING)
        << "StorageBlob memory adjusted by "
        << (num_storage_slots * kSlotSizeBytes - available_memory)
        << " bytes to meet alignment requirement for "
        << "SimpleScalarSeparateChainingHashTable::Header.";
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
                            + sizeof(Bucket));
  num_slots_tmp = get_previous_prime_number(num_buckets_tmp * kHashTableLoadFactor);
  num_buckets_tmp = num_slots_tmp / kHashTableLoadFactor;
  DCHECK_GT(num_slots_tmp, 0u);
  DCHECK_GT(num_buckets_tmp, 0u);

  // Locate the slot array.
  slots_ = static_cast<std::atomic<std::size_t>*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start)
                         + sizeof(std::atomic<std::size_t>) * num_slots_tmp;
  available_memory -= sizeof(std::atomic<std::size_t>) * num_slots_tmp;

  // Locate the buckets.
  void* buckets_addr = aligned_memory_start;
  // Extra-paranoid: If ValueT has an alignment requirement greater than that
  // of std::atomic<std::size_t>, we may need to adjust the start of the bucket
  // array.
  if (align(alignof(Bucket),
            sizeof(Bucket),
            buckets_addr,
            available_memory)
          == nullptr) {
    LOG(FATAL) << "StorageBlob used to hold resizable "
                  "SimpleScalarSeparateChainingHashTable is too small to meet "
                  "alignment requirements of buckets.";
  } else if (buckets_ != aligned_memory_start) {
    LOG(WARNING) << "Bucket array start position adjusted to meet alignment "
                    "requirement for SimpleScalarSeparateChainingHashTable's "
                    "value type.";
    if (num_buckets_tmp * sizeof(Bucket) > available_memory) {
      --num_buckets_tmp;
    }
  }
  buckets_ = static_cast<Bucket*>(buckets_addr);

  // Fill in the header.
  header_->num_slots = num_slots_tmp;
  header_->num_buckets = num_buckets_tmp;
  header_->buckets_allocated.store(0, std::memory_order_relaxed);

  // There is probably some left over available memory in the blob at this
  // point. Although we could allocate some extra buckets from that space,
  // doing so would cause us to violate kHashTableLoadFactor. Instead, we
  // respect kHashTableLoadFactor and prefer to resize when we run out of
  // allocated buckets.
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
SimpleScalarSeparateChainingHashTable<ValueT,
                                      resizable,
                                      serializable,
                                      force_key_copy,
                                      allow_duplicate_keys>
    ::SimpleScalarSeparateChainingHashTable(const std::vector<const Type*> &key_types,
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
              true,
              true),
          key_type_id_(key_types.front()->getTypeID()) {
  DCHECK_EQ(1u, key_types.size());
  DCHECK(TypedValue::HashIsReversible(key_type_id_));

  // Give base HashTable information about what key components are stored
  // inline (SimpleScalarSeparateChainingHashTable ALWAYS stores keys inline).
  this->setKeyInline(&simple_scalar_separate_chaining_hash_table_detail::kKeyInlineGlobal);

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
    LOG(FATAL) << "Attempted to create a non-resizable "
               << "SimpleScalarSeparateChainingHashTable with "
               << available_memory << " bytes of memory at "
               << aligned_memory_start << " which either can not fit a "
               << "SimpleScalarSeparateChainingHashTable::Header or meet its "
               << "alignment requirement.";
  } else if (aligned_memory_start != this->hash_table_memory_) {
    // In general, we could get memory of any alignment, although at least
    // cache-line aligned would be nice.
    LOG(WARNING) << "StorageBlob memory adjusted by "
                 << (this->hash_table_memory_size_ - available_memory)
                 << " bytes to meet alignment requirement for "
                 << "SimpleScalarSeparateChainingHashTable::Header.";
  }

  header_ = static_cast<Header*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start) + sizeof(Header);
  available_memory -= sizeof(Header);

  if (new_hash_table) {
    std::size_t estimated_bucket_capacity
        = available_memory / (kHashTableLoadFactor * sizeof(std::atomic<std::size_t>)
                              + sizeof(Bucket));
    std::size_t num_slots = get_previous_prime_number(estimated_bucket_capacity * kHashTableLoadFactor);

    // Fill in the header.
    header_->num_slots = num_slots;
    header_->num_buckets = num_slots / kHashTableLoadFactor;
    header_->buckets_allocated.store(0, std::memory_order_relaxed);
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
  void* buckets_addr = aligned_memory_start;
  // Extra-paranoid: sizeof(Header) should almost certainly be a multiple of
  // alignof(Bucket), unless ValueT has some members with seriously big
  // (> kCacheLineBytes) alignment requirements specified using alignas().
  if (align(alignof(Bucket),
            sizeof(Bucket),
            buckets_addr,
            available_memory)
          == nullptr) {
    LOG(FATAL)
        << "Attempted to create a non-resizable "
        << "SimpleScalarSeparateChainingHashTable with "
        << this->hash_table_memory_size_ << " bytes of memory at "
        << this->hash_table_memory_ << ", which can hold an aligned "
        << "SimpleScalarSeparateChainingHashTable::Header but does not have "
        << "enough remaining space for even a single hash bucket.";
  } else if (buckets_addr != aligned_memory_start) {
    LOG(WARNING)
        << "Bucket array start position adjusted to meet alignment requirement "
        << "for SimpleScalarSeparateChainingHashTable's value type.";
    if (header_->num_buckets * sizeof(Bucket) > available_memory) {
      DCHECK(new_hash_table);
      --(header_->num_buckets);
    }
  }
  buckets_ = static_cast<Bucket*>(buckets_addr);

  // Make sure "next" pointers in buckets are zeroed-out.
  if (new_hash_table && !hash_table_memory_zeroed) {
    std::memset(buckets_, 0x0, header_->num_buckets * sizeof(Bucket));
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                           allow_duplicate_keys>
    ::clear() {
  const std::size_t used_buckets = header_->buckets_allocated.load(std::memory_order_relaxed);
  // Destroy existing values, if necessary.
  DestroyValues(buckets_, used_buckets);

  // Zero-out slot array.
  std::memset(slots_, 0x0, sizeof(std::atomic<std::size_t>) * header_->num_slots);

  // Zero-out used buckets.
  std::memset(static_cast<void*>(buckets_), 0x0, sizeof(Bucket) * used_buckets);

  header_->buckets_allocated.store(0, std::memory_order_relaxed);
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
const ValueT* SimpleScalarSeparateChainingHashTable<ValueT,
                                                    resizable,
                                                    serializable,
                                                    force_key_copy,
                                                    allow_duplicate_keys>
    ::getSingle(const TypedValue &key) const {
  DCHECK(!allow_duplicate_keys);
  DCHECK(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = key.getHashScalarLiteral();
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DCHECK_NE(bucket_ref, std::numeric_limits<std::size_t>::max());

    const Bucket &bucket = buckets_[bucket_ref - 1];
    if (bucket.hash == hash_code) {
      // Match located.
      return &(bucket.value);
    }
    bucket_ref = bucket.next.load(std::memory_order_relaxed);
  }

  // Reached the end of the chain and didn't find a match.
  return nullptr;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                           allow_duplicate_keys>
    ::getAll(const TypedValue &key, std::vector<const ValueT*> *values) const {
  DCHECK(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = key.getHashScalarLiteral();
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DCHECK_NE(bucket_ref, std::numeric_limits<std::size_t>::max());
    const Bucket &bucket = buckets_[bucket_ref - 1];
    if (bucket.hash == hash_code) {
      // Match located.
      values->push_back(&(bucket.value));
      if (!allow_duplicate_keys) {
        return;
      }
    }
    bucket_ref = bucket.next.load(std::memory_order_relaxed);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
HashTablePutResult
    SimpleScalarSeparateChainingHashTable<ValueT,
                                          resizable,
                                          serializable,
                                          force_key_copy,
                                          allow_duplicate_keys>
        ::putInternal(const TypedValue &key,
                      const std::size_t variable_key_size,
                      const ValueT &value,
                      HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  if (prealloc_state == nullptr) {
    // Early check for a free bucket.
    if (header_->buckets_allocated.load(std::memory_order_relaxed) >= header_->num_buckets) {
      return HashTablePutResult::kOutOfSpace;
    }
  }

  const std::size_t hash_code = key.getHashScalarLiteral();
  Bucket *bucket = nullptr;
  std::atomic<std::size_t> *pending_chain_ptr;
  
  const std::size_t allocated_bucket_num
    = (prealloc_state == nullptr)
    ? header_->buckets_allocated.fetch_add(1, std::memory_order_relaxed)
    : (prealloc_state->bucket_position)++;
  
  if (allocated_bucket_num >= header_->num_buckets) {
    header_->buckets_allocated.fetch_sub(1, std::memory_order_relaxed);
    return HashTablePutResult::kOutOfSpace;
  }
  
  bucket = buckets_ + allocated_bucket_num;
  bucket->hash = hash_code;
  new(&(bucket->value)) ValueT(value);

  std::atomic<std::size_t> *buckets_next_ptr = &(bucket->next);

  pending_chain_ptr = &(slots_[hash_code % header_->num_slots]);
  for (;;) {
    // Save the old address;
    std::size_t existing_chain_ptr = pending_chain_ptr->load(std::memory_order_release);

    if (pending_chain_ptr->compare_exchange_strong(existing_chain_ptr,
						   allocated_bucket_num + 1,
						   std::memory_order_acq_rel)) {
      buckets_next_ptr->store(existing_chain_ptr, std::memory_order_release);
      break;
    }
  }
  return HashTablePutResult::kOK;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
ValueT* SimpleScalarSeparateChainingHashTable<ValueT,
                                              resizable,
                                              serializable,
                                              force_key_copy,
                                              allow_duplicate_keys>
    ::upsertInternal(const TypedValue &key,
                     const std::size_t variable_key_size,
                     const ValueT &initial_value) {
  DCHECK(!allow_duplicate_keys);
  DCHECK(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));
  DCHECK_EQ(0u, variable_key_size);

  const std::size_t hash_code = key.getHashScalarLiteral();
  Bucket *bucket = nullptr;
  std::atomic<std::size_t> *pending_chain_ptr;
  std::size_t pending_chain_ptr_finish_value;
  if (locateBucketForInsertion(hash_code,
                               &bucket,
                               &pending_chain_ptr,
                               &pending_chain_ptr_finish_value,
                               nullptr)) {
    // Inserting into an empty bucket.
    // Write the hash, which can be reversed to recover the key.
    bucket->hash = hash_code;

    // Copy the supplied 'initial_value into place.
    new(&(bucket->value)) ValueT(initial_value);

    // Update the previous chain pointer to point to the new bucket.
    pending_chain_ptr->store(pending_chain_ptr_finish_value, std::memory_order_release);

    // Return the value.
    return &(bucket->value);
  } else if (bucket == nullptr) {
    // Ran out of buckets.
    return nullptr;
  } else {
    // Found an already-existing entry for this key.
    return &(bucket->value);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                           allow_duplicate_keys>
    ::getNextEntry(TypedValue *key, const ValueT **value, std::size_t *entry_num) const {
  if (*entry_num < header_->buckets_allocated.load(std::memory_order_relaxed)) {
    const Bucket &bucket = buckets_[*entry_num];
    // Reconstruct key by reversing hash.
    *key = TypedValue(key_type_id_, bucket.hash);
    *value = &(bucket.value);
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
bool SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                             allow_duplicate_keys>
    ::getNextEntryForKey(const TypedValue &key,
                         const std::size_t hash_code,
                         const ValueT **value,
                         std::size_t *entry_num) const {
  DCHECK(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  if (*entry_num == 0) {
    *entry_num = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  } else if (*entry_num == std::numeric_limits<std::size_t>::max()) {
    return false;
  }

  while (*entry_num != 0) {
    DCHECK_NE(*entry_num, std::numeric_limits<std::size_t>::max());
    const Bucket &bucket = buckets_[*entry_num - 1];
    *entry_num = bucket.next.load(std::memory_order_relaxed);
    if (bucket.hash == hash_code) {
      // Match located.
      *value = &(bucket.value);
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
bool SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                           allow_duplicate_keys>
    ::hasKey(const TypedValue &key) const {
  DCHECK_EQ(1u, this->key_types_.size());
  DCHECK(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = key.getHashScalarLiteral();
  std::size_t bucket_ref = slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DCHECK_NE(bucket_ref, std::numeric_limits<std::size_t>::max());

    const Bucket &bucket = buckets_[bucket_ref - 1];
    if (bucket.hash == hash_code) {
      // Match located.
      return true;
    }
    bucket_ref = bucket.next.load(std::memory_order_relaxed);
  }

  // Reached the end of the chain and didn't find a match.
  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                             allow_duplicate_keys>
    ::resize(const std::size_t extra_buckets,
             const std::size_t extra_variable_storage,
             const std::size_t retry_num) {
  DCHECK(resizable);
  DCHECK_EQ(0u, extra_variable_storage);

  // A retry should never be necessary with this implementation of HashTable.
  // Separate chaining ensures that any resized hash table with more buckets
  // than the original table will be able to hold more entries than the
  // original.
  DCHECK_EQ(0u, retry_num);

  SpinSharedMutexExclusiveLock<true> write_lock(this->resize_shared_mutex_);

  // Recheck whether the hash table is still full. Note that multiple threads
  // might wait to rebuild this hash table simultaneously. Only the first one
  // should do the rebuild.
  if (!isFull(extra_buckets)) {
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

  const std::size_t resized_memory_required
      = sizeof(Header)
        + resized_num_slots * sizeof(std::atomic<std::size_t>)
        + (resized_num_slots / kHashTableLoadFactor) * sizeof(Bucket);
  const std::size_t resized_storage_slots
      = this->storage_manager_->SlotsNeededForBytes(resized_memory_required);
  if (resized_storage_slots == 0) {
    LOG(FATAL) << "Storage requirement for resized "
                  "SimpleScalarSeparateChainingHashTable exceeds maximum "
                  "allocation size.";
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
    LOG(FATAL) << "StorageBlob used to hold resized "
                  "SimpleScalarSeparateChainingHashTable is too small to meet "
                  "alignment requirements of Header.";
  } else if (aligned_memory_start != resized_blob->getMemoryMutable()) {
    // Again, should be impossible.
    LOG(WARNING) << "StorageBlob memory adjusted by "
                 << (resized_num_slots * kSlotSizeBytes - available_memory)
                 << " bytes to meet alignment requirement for "
                 << "SimpleScalarSeparateChainingHashTable::Header.";
  }

  Header *resized_header = static_cast<Header*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start) + sizeof(Header);
  available_memory -= sizeof(Header);

  // As in constructor, recompute the number of slots and buckets using the
  // actual available memory.
  std::size_t resized_num_buckets
      = (available_memory - extra_variable_storage)
        / (kHashTableLoadFactor * sizeof(std::atomic<std::size_t>)+ sizeof(Bucket));
  resized_num_slots = get_previous_prime_number(resized_num_buckets * kHashTableLoadFactor);
  resized_num_buckets = resized_num_slots / kHashTableLoadFactor;

  // Locate slot array.
  std::atomic<std::size_t> *resized_slots = static_cast<std::atomic<std::size_t>*>(aligned_memory_start);
  aligned_memory_start = static_cast<char*>(aligned_memory_start)
                         + sizeof(std::atomic<std::size_t>) * resized_num_slots;
  available_memory -= sizeof(std::atomic<std::size_t>) * resized_num_slots;

  // As in constructor, we will be extra paranoid and use align() to locate the
  // start of the array of buckets, as well.
  void *resized_buckets_addr = aligned_memory_start;
  if (align(alignof(Bucket),
            sizeof(Bucket),
            resized_buckets_addr,
            available_memory)
          == nullptr) {
    LOG(FATAL)
        << "StorageBlob used to hold resized "
           "SimpleScalarSeparateChainingHashTable is too small to meet "
           "alignment requirements of buckets.";
  } else if (resized_buckets_addr != aligned_memory_start) {
    LOG(WARNING)
        << "Bucket array start position adjusted from " << aligned_memory_start
        << " to " << resized_buckets_addr
        << " to meet requirement for SimpleScalarSeparateChainingHashTable's "
        << "value type.";
    if (resized_num_buckets * sizeof(Bucket) > available_memory) {
      --resized_num_buckets;
    }
  }
  Bucket *resized_buckets = static_cast<Bucket*>(resized_buckets_addr);

  const std::size_t original_buckets_used = header_->buckets_allocated.load(std::memory_order_relaxed);

  // Initialize the header.
  resized_header->num_slots = resized_num_slots;
  resized_header->num_buckets = resized_num_buckets;
  resized_header->buckets_allocated.store(original_buckets_used, std::memory_order_relaxed);

  // Bulk-copy buckets. This is safe because:
  //     1. The "next" pointers will be adjusted when rebuilding chains below.
  //     2. The hash codes will stay the same.
  //     3. If values are not trivially copyable, then we invoke ValueT's copy
  //        or move constructor with placement new.
  std::memcpy(static_cast<void*>(resized_buckets),
              static_cast<const void*>(buckets_),
              original_buckets_used * sizeof(Bucket));

  // TODO(chasseur): std::is_trivially_copyable is not yet implemented in
  // GCC 4.8.3, so we assume we need to invoke ValueT's copy or move
  // constructor, even though the plain memcpy above could suffice for many
  // possible ValueTs.
  for (std::size_t bucket_num = 0; bucket_num < original_buckets_used; ++bucket_num) {
    // Use a move constructor if available to avoid a deep-copy, since resizes
    // always succeed.
    new (&(resized_buckets[bucket_num].value)) ValueT(std::move(buckets_[bucket_num].value));
  }

  // Destroy values in the original hash table, if neccesary,
  DestroyValues(buckets_,
                original_buckets_used);

  // Make resized structures active.
  std::swap(this->blob_, resized_blob);
  header_ = resized_header;
  slots_ = resized_slots;
  buckets_ = resized_buckets;

  // Drop the old blob.
  const block_id old_blob_id = resized_blob->getID();
  resized_blob.release();
  this->storage_manager_->deleteBlockOrBlobFile(old_blob_id);

  // Rebuild chains.
  for (std::size_t bucket_num = 0; bucket_num < original_buckets_used; ++bucket_num) {
    Bucket &bucket = buckets_[bucket_num];
    const std::size_t slot_number = bucket.hash % header_->num_slots;
    std::size_t slot_ptr_value = 0;
    if (slots_[slot_number].compare_exchange_strong(slot_ptr_value,
                                                    bucket_num + 1,
                                                    std::memory_order_relaxed)) {
      // This bucket is the first in the chain for this block, so reset its
      // next pointer to zero.
      bucket.next.store(0, std::memory_order_relaxed);
    } else {
      // A chain already exists starting from this slot, so put this bucket at
      // the head.
      bucket.next.store(slot_ptr_value, std::memory_order_relaxed);
      slots_[slot_number].store(bucket_num + 1, std::memory_order_relaxed);
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                           allow_duplicate_keys>
    ::preallocateForBulkInsert(const std::size_t total_entries,
                               const std::size_t total_variable_key_size,
                               HashTablePreallocationState *prealloc_state) {
  DCHECK(allow_duplicate_keys);
  DCHECK_EQ(0u, total_variable_key_size);

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
    return false;
  }

  prealloc_state->bucket_position = original_buckets_allocated;
  return true;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void SimpleScalarSeparateChainingHashTable<ValueT,
                                           resizable,
                                           serializable,
                                           force_key_copy,
                                           allow_duplicate_keys>
    ::DestroyValues(Bucket *hash_buckets,
                    const std::size_t num_buckets) {
  if (!std::is_trivially_destructible<ValueT>::value) {
    for (std::size_t bucket_num = 0;
         bucket_num < num_buckets;
         ++bucket_num) {
      hash_buckets[bucket_num].value.~ValueT();
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline bool SimpleScalarSeparateChainingHashTable<ValueT,
                                                  resizable,
                                                  serializable,
                                                  force_key_copy,
                                                  allow_duplicate_keys>
    ::locateBucketForInsertion(const std::size_t hash_code,
                               Bucket **bucket,
                               std::atomic<std::size_t> **pending_chain_ptr,
                               std::size_t *pending_chain_ptr_finish_value,
                               HashTablePreallocationState *prealloc_state) {
  DCHECK((prealloc_state == nullptr) || allow_duplicate_keys);
  if (*bucket == nullptr) {
    *pending_chain_ptr = &(slots_[hash_code % header_->num_slots]);
  } else {
    *pending_chain_ptr = &((*bucket)->next);
  }
  for (;;) {
    std::size_t existing_chain_ptr = 0;
    if ((*pending_chain_ptr)->compare_exchange_strong(existing_chain_ptr,
                                                      std::numeric_limits<std::size_t>::max(),
                                                      std::memory_order_acq_rel)) {
      // Got to the end of the chain. Allocate a new bucket.
      const std::size_t allocated_bucket_num
          = (prealloc_state == nullptr)
              ? header_->buckets_allocated.fetch_add(1, std::memory_order_relaxed)
              : (prealloc_state->bucket_position)++;
      if (allocated_bucket_num >= header_->num_buckets) {
        // Ran out of buckets.
        DCHECK(prealloc_state == nullptr);
        header_->buckets_allocated.fetch_sub(1, std::memory_order_relaxed);
        (*pending_chain_ptr)->store(0, std::memory_order_release);
        *bucket = nullptr;
        return false;
      } else {
        *bucket = buckets_ + allocated_bucket_num;
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
    *bucket = buckets_ + (existing_chain_ptr - 1);
    *pending_chain_ptr = &((*bucket)->next);
    if (!allow_duplicate_keys) {
      if ((*bucket)->hash == hash_code) {
        return false;
      }
    }
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_SIMPLE_SCALAR_SEPARATE_CHAINING_HASH_TABLE_HPP_
