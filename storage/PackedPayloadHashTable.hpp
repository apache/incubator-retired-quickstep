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

#ifndef QUICKSTEP_STORAGE_PACKED_PAYLOAD_HASH_TABLE_HPP_
#define QUICKSTEP_STORAGE_PACKED_PAYLOAD_HASH_TABLE_HPP_

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/HashTableKeyManager.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "threading/SpinMutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/HashPair.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class StorageManager;
class Type;
class ValueAccessor;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Aggregation hash table implementation in which the payload can be just
 *        a bunch of bytes. This implementation is suitable for aggregation with
 *        multiple aggregation handles (e.g. SUM, MAX, MIN etc).
 *
 * At present the hash table uses separate chaining to resolve collisions, i.e.
 * Keys/values are stored in a separate region of memory from the base hash
 * table slot array. Every bucket has a "next" pointer so that entries that
 * collide (i.e. map to the same base slot) form chains of pointers with each
 * other.
 **/
class PackedPayloadHashTable : public AggregationStateHashTableBase {
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
  PackedPayloadHashTable(
      const std::vector<const Type *> &key_types,
      const std::size_t num_entries,
      const std::vector<AggregationHandle *> &handles,
      StorageManager *storage_manager);

  ~PackedPayloadHashTable() override;

  /**
   * @brief Erase all entries in this hash table.
   *
   * @warning This method is not guaranteed to be threadsafe.
   **/
  void clear();

  void destroyPayload() override;

  /**
   * @brief Use aggregation handles to update (multiple) aggregation states in
   *        this hash table, with group-by keys and arguments drawn from the
   *        given ValueAccessors. New states are first inserted if not already
   *        present.
   *
   * @note This method is threadsafe with regard to other calls to
   *       upsertCompositeKey() and upsertValueAccessorCompositeKey().
   *
   * @param argument_ids The multi-source attribute IDs of each argument
   *        component to be read from \p accessor_mux.
   * @param key_ids The multi-source attribute IDs of each group-by key
   *        component to be read from \p accessor_mux.
   * @param accessor_mux A ValueAccessorMultiplexer object that contains the
   *        ValueAccessors which will be used to access keys. beginIteration()
   *        should be called on the accessors before calling this method.
   * @return True on success, false if upsert failed because there was not
   *         enough space to insert new entries for all the keys in accessor
   *         (note that some entries may still have been upserted, and
   *         accessors' iterations will be left on the first tuple which could
   *         not be inserted).
   **/
  bool upsertValueAccessorCompositeKey(
      const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
      const std::vector<MultiSourceAttributeId> &key_ids,
      const ValueAccessorMultiplexer &accessor_mux) override;

  /**
   * @return The ID of the StorageBlob used to store this hash table.
   **/
  inline block_id getBlobId() const {
    return blob_->getID();
  }

  /**
   * @warning This method assumes that no concurrent calls to
   *          upsertCompositeKey() or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call).
   *          Concurrent calls to getSingleCompositeKey(), forEach(), and
   *          forEachCompositeKey() are safe.
   *
   * @return The number of entries in this HashTable.
   **/
  inline std::size_t numEntries() const {
    return header_->buckets_allocated.load(std::memory_order_relaxed);
  }

  /**
   * @brief Use aggregation handles to merge the given aggregation states into
   *        the aggregation states mapped to the given key. New states are first
   *        inserted if not already present.
   *
   * @warning The key must not be null.
   * @note This method is threadsafe with regard to other calls to
   *       upsertCompositeKey() and upsertValueAccessorCompositeKey().
   *
   * @param key The key.
   * @param source_state The source aggregation states to be merged into this
   *        hash table.
   * @return True on success, false if upsert failed because there was not
   *         enough space to insert a new entry in this hash table.
   **/
  inline bool upsertCompositeKey(const std::vector<TypedValue> &key,
                                 const std::uint8_t *source_state);

  /**
   * @brief Apply a functor to an aggregation state mapped to the given key.
   *        First inserting a new state if one is not already present.
   *
   * @warning The key must not be null.
   * @note This method is threadsafe with regard to other calls to
   *       upsertCompositeKey() and upsertValueAccessorCompositeKey().
   *
   * @param key The key.
   * @param functor A pointer to a functor, which should provide a call
   *        operator which takes an aggregation state (of type std::uint8_t *)
   *        as an argument.
   * @param index The index of the target aggregation state among those states
   *        mapped to \p key.
   * @return True on success, false if upsert failed because there was not
   *         enough space to insert a new entry in this hash table.
   **/
  template <typename FunctorT>
  inline bool upsertCompositeKey(const std::vector<TypedValue> &key,
                                 FunctorT *functor,
                                 const std::size_t index);

  /**
   * @brief Lookup a composite key against this hash table to find a matching
   *        entry.
   *
   * @warning The key must not be null.
   * @warning This method assumes that no concurrent calls to
   *          upsertCompositeKey() or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingleCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   *
   * @param key The key to look up.
   * @return The value of a matched entry if a matching key is found.
   *         Otherwise, return NULL.
   **/
  inline const std::uint8_t* getSingleCompositeKey(
      const std::vector<TypedValue> &key) const;

  /**
   * @brief Lookup a composite key against this hash table to find a matching
   *        entry. Then return the aggregation state component with the
   *        specified index.
   *
   * @warning The key must not be null.
   * @warning This method assumes that no concurrent calls to
   *          upsertCompositeKey() or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingleCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   *
   * @param key The key to look up.
   * @param index The index of the target aggregation state among those states
   *        mapped to \p key.
   * @return The aggregation state of the specified index if a matching key is
   *         found. Otherwise, return NULL.
   **/
  inline const std::uint8_t* getSingleCompositeKey(
      const std::vector<TypedValue> &key,
      const std::size_t index) const;

  /**
   * @brief Apply a functor to each (key, value) pair in this hash table.
   *
   * @warning This method assumes that no concurrent calls to
   *          upsertCompositeKey() or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingleCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   *
   * @param functor A pointer to a functor, which should provide a call operator
   *        which takes 2 arguments: const TypedValue&, const std::uint8_t*.
   *        The call operator will be invoked once on each key, value pair in
   *        this hash table.
   * @return The number of key-value pairs visited.
   **/
  template <typename FunctorT>
  inline std::size_t forEach(FunctorT *functor) const;

  /**
   * @brief Apply a functor to each (key, aggregation state) pair in this hash
   *        table, where the aggregation state is retrieved from the value
   *        that maps to the corresponding key with the specified index.
   *
   * @warning This method assumes that no concurrent calls to
   *          upsertCompositeKey() or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingleCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   *
   * @param functor A pointer to a functor, which should provide a call operator
   *        which takes 2 arguments: const TypedValue&, const std::uint8_t*.
   *        The call operator will be invoked once on each (key, aggregation state)
   *        pair in this hash table.
   * @param index The index of the target aggregation state among those states
   *        mapped to \p key.
   * @return The number of key-value pairs visited.
   **/
  template <typename FunctorT>
  inline std::size_t forEach(FunctorT *functor, const int index) const;

  /**
   * @brief Apply a functor to each key, value pair in this hash table.
   *        Composite key version.
   *
   * @warning This method assumes that no concurrent calls to
   *          upsertCompositeKey() or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingleCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   *
   * @param functor A pointer to a functor, which should provide a call operator
   *        which takes 2 arguments: const TypedValue&, const std::uint8_t*.
   *        The call operator will be invoked once on each key, value pair in
   *        this hash table.
   * @return The number of key-value pairs visited.
   **/
  template <typename FunctorT>
  inline std::size_t forEachCompositeKey(FunctorT *functor) const;

  /**
   * @brief Apply a functor to each (key, aggregation state) pair in this hash
   *        table, where the aggregation state is retrieved from the value
   *        that maps to the corresponding key with the specified index.
   *        Composite key version.
   *
   * @warning This method assumes that no concurrent calls to
   *          upsertCompositeKey() or upsertValueAccessorCompositeKey() are
   *          taking place (i.e. that this HashTable is immutable for the
   *          duration of the call and as long as the returned pointer may be
   *          dereferenced). Concurrent calls to getSingleCompositeKey(),
   *          forEach(), and forEachCompositeKey() are safe.
   *
   * @param functor A pointer to a functor, which should provide a call operator
   *        which takes 2 arguments: const TypedValue&, const std::uint8_t*.
   *        The call operator will be invoked once on each (key, aggregation state)
   *        pair in this hash table.
   * @param index The index of the target aggregation state among those states
   *        mapped to \p key.
   * @return The number of key-value pairs visited.
   **/
  template <typename FunctorT>
  inline std::size_t forEachCompositeKey(FunctorT *functor,
                                         const std::size_t index) const;

  std::size_t getMemoryConsumptionBytes() const override {
    return sizeof(Header) + numEntries() * bucket_size_;
  }

 private:
  void resize(const std::size_t extra_buckets,
              const std::size_t extra_variable_storage,
              const std::size_t retry_num = 0);

  inline std::size_t calculateVariableLengthCompositeKeyCopySize(
      const std::vector<TypedValue> &key) const {
    std::size_t total = 0;
    for (std::vector<TypedValue>::size_type idx = 0; idx < key.size(); ++idx) {
      if (!(*key_inline_)[idx]) {
        total += key[idx].getDataSize();
      }
    }
    return total;
  }

  inline bool getNextEntry(TypedValue *key,
                           const std::uint8_t **value,
                           std::size_t *entry_num) const;

  inline bool getNextEntryCompositeKey(std::vector<TypedValue> *key,
                                       const std::uint8_t **value,
                                       std::size_t *entry_num) const;

  template <bool key_only = false>
  inline std::uint8_t* upsertCompositeKeyInternal(
      const std::vector<TypedValue> &key,
      const std::size_t variable_key_size);

  template <bool use_two_accessors, bool key_only, bool has_variable_size,
            typename ValueAccessorT>
  inline bool upsertValueAccessorCompositeKeyInternal(
      const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
      const std::vector<MultiSourceAttributeId> &key_ids,
      ValueAccessor *base_accessor,
      ValueAccessorT *derived_accessor);

  // Generate a hash for a composite key by hashing each component of 'key' and
  // mixing their bits with CombineHashes().
  inline std::size_t hashCompositeKey(const std::vector<TypedValue> &key) const;

  // Set information about which key components are stored inline. This usually
  // comes from a HashTableKeyManager, and is set by the constructor of a
  // subclass of HashTable.
  inline void setKeyInline(const std::vector<bool> *key_inline) {
    key_inline_ = key_inline;
    all_keys_inline_ = std::accumulate(key_inline_->begin(), key_inline_->end(),
                                       true, std::logical_and<bool>());
  }

  inline static std::size_t ComputeTotalPayloadSize(
      const std::vector<AggregationHandle *> &handles) {
    std::size_t total_payload_size = sizeof(SpinMutex);
    for (const auto *handle : handles) {
      total_payload_size += handle->getPayloadSize();
    }
    return total_payload_size;
  }

  // Assign '*key_vector' with the attribute values specified by 'key_ids' at
  // the current position of 'accessor'. If 'check_for_null_keys' is true, stops
  // and returns true if any of the values is null, otherwise returns false.
  template <bool use_two_accessors,
            bool check_for_null_keys,
            typename BaseAccessorT,
            typename DerivedAccessorT>
  inline static bool GetCompositeKeyFromValueAccessor(
      const std::vector<MultiSourceAttributeId> &key_ids,
      const BaseAccessorT *accessor,
      const DerivedAccessorT *derived_accessor,
      std::vector<TypedValue> *key_vector) {
    for (std::size_t key_idx = 0; key_idx < key_ids.size(); ++key_idx) {
      const MultiSourceAttributeId &key_id = key_ids[key_idx];
      if (use_two_accessors && key_id.source == ValueAccessorSource::kDerived) {
        (*key_vector)[key_idx] = derived_accessor->getTypedValue(key_id.attr_id);
      } else {
        (*key_vector)[key_idx] = accessor->getTypedValue(key_id.attr_id);
      }
      if (check_for_null_keys && (*key_vector)[key_idx].isNull()) {
        return true;
      }
    }
    return false;
  }

  struct Header {
    std::size_t num_slots;
    std::size_t num_buckets;
    alignas(kCacheLineBytes) std::atomic<std::size_t> buckets_allocated;
    alignas(kCacheLineBytes)
        std::atomic<std::size_t> variable_length_bytes_allocated;
  };

  // Type(s) of keys.
  const std::vector<const Type *> key_types_;

  // Information about whether key components are stored inline or in a
  // separate variable-length storage region. This is usually determined by a
  // HashTableKeyManager and set by calling setKeyInline().
  bool all_keys_inline_;
  const std::vector<bool> *key_inline_;

  const std::size_t num_handles_;
  const std::vector<AggregationHandle *> handles_;

  std::size_t total_payload_size_;
  std::vector<std::size_t> payload_offsets_;
  std::uint8_t *init_payload_;

  StorageManager *storage_manager_;
  MutableBlobReference blob_;

  // Locked in shared mode for most operations, exclusive mode during resize.
  // Not locked at all for non-resizable HashTables.
  alignas(kCacheLineBytes) SpinSharedMutex<true> resize_shared_mutex_;

  std::size_t kBucketAlignment;

  // Value's offset in a bucket is the first alignof(ValueT) boundary after the
  // next pointer and hash code.
  std::size_t kValueOffset;

  // Round bucket size up to a multiple of kBucketAlignment.
  inline std::size_t ComputeBucketSize(const std::size_t fixed_key_size) {
    return (((kValueOffset + this->total_payload_size_ + fixed_key_size - 1) /
             kBucketAlignment) +
            1) *
           kBucketAlignment;
  }

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
  inline bool locateBucketForInsertion(
      const std::size_t hash_code,
      const std::size_t variable_key_allocation_required,
      void **bucket,
      std::atomic<std::size_t> **pending_chain_ptr,
      std::size_t *pending_chain_ptr_finish_value);

  // Write a scalar 'key' and its 'hash_code' into the '*bucket', which was
  // found by locateBucketForInsertion(). Assumes that storage for a
  // variable-length key copy (if any) was already allocated by a successful
  // call to allocateVariableLengthKeyStorage().
  inline void writeScalarKeyToBucket(
      const TypedValue &key,
      const std::size_t hash_code,
      void *bucket);

  // Write a composite 'key' and its 'hash_code' into the '*bucket', which was
  // found by locateBucketForInsertion(). Assumes that storage for
  // variable-length key copies (if any) was already allocated by a successful
  // call to allocateVariableLengthKeyStorage().
  inline void writeCompositeKeyToBucket(
      const std::vector<TypedValue> &key,
      const std::size_t hash_code,
      void *bucket);

  // Determine whether it is actually necessary to resize this hash table.
  // Checks that there is at least one unallocated bucket, and that there is
  // at least 'extra_variable_storage' bytes of variable-length storage free.
  inline bool isFull(const std::size_t extra_variable_storage) const;

  // Helper object to manage key storage.
  HashTableKeyManager<false, true> key_manager_;

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

  DISALLOW_COPY_AND_ASSIGN(PackedPayloadHashTable);
};

/** @} */

// ----------------------------------------------------------------------------
// Implementations of template class methods follow.

class HashTableMerger {
 public:
  /**
   * @brief Constructor
   *
   * @param handle The Aggregation handle being used.
   * @param destination_hash_table The destination hash table to which other
   *        hash tables will be merged.
   **/
  explicit HashTableMerger(PackedPayloadHashTable *destination_hash_table)
      : destination_hash_table_(destination_hash_table) {}

  /**
   * @brief The operator for the functor.
   *
   * @param group_by_key The group by key being merged.
   * @param source_state The aggregation state for the given key in the source
   *        aggregation hash table.
   **/
  inline void operator()(const std::vector<TypedValue> &group_by_key,
                         const std::uint8_t *source_state) {
    destination_hash_table_->upsertCompositeKey(group_by_key, source_state);
  }

 private:
  PackedPayloadHashTable *destination_hash_table_;

  DISALLOW_COPY_AND_ASSIGN(HashTableMerger);
};

inline std::size_t PackedPayloadHashTable::hashCompositeKey(
    const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(!key.empty());
  DEBUG_ASSERT(key.size() == key_types_.size());
  std::size_t hash = key.front().getHash();
  for (std::vector<TypedValue>::const_iterator key_it = key.begin() + 1;
       key_it != key.end();
       ++key_it) {
    hash = CombineHashes(hash, key_it->getHash());
  }
  return hash;
}

inline bool PackedPayloadHashTable::getNextEntry(TypedValue *key,
                                                 const std::uint8_t **value,
                                                 std::size_t *entry_num) const {
  if (*entry_num < header_->buckets_allocated.load(std::memory_order_relaxed)) {
    const char *bucket =
        static_cast<const char *>(buckets_) + (*entry_num) * bucket_size_;
    *key = key_manager_.getKeyComponentTyped(bucket, 0);
    *value = reinterpret_cast<const std::uint8_t *>(bucket + kValueOffset);
    ++(*entry_num);
    return true;
  } else {
    return false;
  }
}

inline bool PackedPayloadHashTable::getNextEntryCompositeKey(
    std::vector<TypedValue> *key,
    const std::uint8_t **value,
    std::size_t *entry_num) const {
  if (*entry_num < header_->buckets_allocated.load(std::memory_order_relaxed)) {
    const char *bucket =
        static_cast<const char *>(buckets_) + (*entry_num) * bucket_size_;
    for (std::vector<const Type *>::size_type key_idx = 0;
         key_idx < this->key_types_.size();
         ++key_idx) {
      key->emplace_back(key_manager_.getKeyComponentTyped(bucket, key_idx));
    }
    *value = reinterpret_cast<const std::uint8_t *>(bucket + kValueOffset);
    ++(*entry_num);
    return true;
  } else {
    return false;
  }
}

inline bool PackedPayloadHashTable::locateBucketForInsertion(
    const std::size_t hash_code,
    const std::size_t variable_key_allocation_required,
    void **bucket,
    std::atomic<std::size_t> **pending_chain_ptr,
    std::size_t *pending_chain_ptr_finish_value) {
  if (*bucket == nullptr) {
    *pending_chain_ptr = &(slots_[hash_code % header_->num_slots]);
  } else {
    *pending_chain_ptr = static_cast<std::atomic<std::size_t> *>(*bucket);
  }
  for (;;) {
    std::size_t existing_chain_ptr = 0;
    if ((*pending_chain_ptr)
            ->compare_exchange_strong(existing_chain_ptr,
                                      std::numeric_limits<std::size_t>::max(),
                                      std::memory_order_acq_rel)) {
      // Got to the end of the chain. Allocate a new bucket.

      // First, allocate variable-length key storage, if needed (i.e. if this
      // is an upsert and we didn't allocate up-front).
      if (!key_manager_.allocateVariableLengthKeyStorage(
              variable_key_allocation_required)) {
        // Ran out of variable-length storage.
        (*pending_chain_ptr)->store(0, std::memory_order_release);
        *bucket = nullptr;
        return false;
      }

      const std::size_t allocated_bucket_num =
          header_->buckets_allocated.fetch_add(1, std::memory_order_relaxed);
      if (allocated_bucket_num >= header_->num_buckets) {
        // Ran out of buckets.
        header_->buckets_allocated.fetch_sub(1, std::memory_order_relaxed);
        (*pending_chain_ptr)->store(0, std::memory_order_release);
        *bucket = nullptr;
        return false;
      } else {
        *bucket =
            static_cast<char *>(buckets_) + allocated_bucket_num * bucket_size_;
        *pending_chain_ptr_finish_value = allocated_bucket_num + 1;
        return true;
      }
    }
    // Spin until the real "next" pointer is available.
    while (existing_chain_ptr == std::numeric_limits<std::size_t>::max()) {
      existing_chain_ptr =
          (*pending_chain_ptr)->load(std::memory_order_acquire);
    }
    if (existing_chain_ptr == 0) {
      // Other thread had to roll back, so try again.
      continue;
    }
    // Chase the next pointer.
    *bucket =
        static_cast<char *>(buckets_) + (existing_chain_ptr - 1) * bucket_size_;
    *pending_chain_ptr = static_cast<std::atomic<std::size_t> *>(*bucket);
    const std::size_t hash_in_bucket = *reinterpret_cast<const std::size_t *>(
        static_cast<const char *>(*bucket) +
        sizeof(std::atomic<std::size_t>));
    if (hash_in_bucket == hash_code) {
      return false;
    }
  }
}

inline const std::uint8_t* PackedPayloadHashTable::getSingleCompositeKey(
    const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->hashCompositeKey(key);
  std::size_t bucket_ref =
      slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket =
        static_cast<const char *>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t *>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) &&
        key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      return reinterpret_cast<const std::uint8_t *>(bucket + kValueOffset);
    }
    bucket_ref =
        reinterpret_cast<const std::atomic<std::size_t> *>(bucket)->load(
            std::memory_order_relaxed);
  }

  // Reached the end of the chain and didn't find a match.
  return nullptr;
}

inline const std::uint8_t* PackedPayloadHashTable::getSingleCompositeKey(
    const std::vector<TypedValue> &key,
    const std::size_t index) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->hashCompositeKey(key);
  std::size_t bucket_ref =
      slots_[hash_code % header_->num_slots].load(std::memory_order_relaxed);
  while (bucket_ref != 0) {
    DEBUG_ASSERT(bucket_ref != std::numeric_limits<std::size_t>::max());
    const char *bucket =
        static_cast<const char *>(buckets_) + (bucket_ref - 1) * bucket_size_;
    const std::size_t bucket_hash = *reinterpret_cast<const std::size_t *>(
        bucket + sizeof(std::atomic<std::size_t>));
    if ((bucket_hash == hash_code) &&
        key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      return reinterpret_cast<const std::uint8_t *>(bucket + kValueOffset) +
             this->payload_offsets_[index];
    }
    bucket_ref =
        reinterpret_cast<const std::atomic<std::size_t> *>(bucket)->load(
            std::memory_order_relaxed);
  }

  // Reached the end of the chain and didn't find a match.
  return nullptr;
}

inline bool PackedPayloadHashTable::upsertCompositeKey(
    const std::vector<TypedValue> &key,
    const std::uint8_t *source_state) {
  const std::size_t variable_size =
      calculateVariableLengthCompositeKeyCopySize(key);
  for (;;) {
    {
      SpinSharedMutexSharedLock<true> resize_lock(resize_shared_mutex_);
      std::uint8_t *value =
          upsertCompositeKeyInternal(key, variable_size);
      if (value != nullptr) {
        SpinMutexLock lock(*(reinterpret_cast<SpinMutex *>(value)));
        for (unsigned int k = 0; k < num_handles_; ++k) {
          handles_[k]->mergeStates(source_state + payload_offsets_[k],
                                   value + payload_offsets_[k]);
        }
        return true;
      }
    }
    resize(0, variable_size);
  }
}

template <typename FunctorT>
inline bool PackedPayloadHashTable::upsertCompositeKey(
    const std::vector<TypedValue> &key,
    FunctorT *functor,
    const std::size_t index) {
  const std::size_t variable_size =
      calculateVariableLengthCompositeKeyCopySize(key);
  for (;;) {
    {
      SpinSharedMutexSharedLock<true> resize_lock(resize_shared_mutex_);
      std::uint8_t *value =
          upsertCompositeKeyInternal(key, variable_size);
      if (value != nullptr) {
        (*functor)(value + payload_offsets_[index]);
        return true;
      }
    }
    resize(0, variable_size);
  }
}

template <bool key_only>
inline std::uint8_t* PackedPayloadHashTable::upsertCompositeKeyInternal(
    const std::vector<TypedValue> &key,
    const std::size_t variable_key_size) {
  if (variable_key_size > 0) {
    // Don't allocate yet, since the key may already be present. However, we
    // do check if either the allocated variable storage space OR the free
    // space is big enough to hold the key (at least one must be true: either
    // the key is already present and allocated, or we need to be able to
    // allocate enough space for it).
    std::size_t allocated_bytes = header_->variable_length_bytes_allocated.load(
        std::memory_order_relaxed);
    if ((allocated_bytes < variable_key_size) &&
        (allocated_bytes + variable_key_size >
         key_manager_.getVariableLengthKeyStorageSize())) {
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
                                 &pending_chain_ptr_finish_value)) {
      // Found an empty bucket.
      break;
    } else if (bucket == nullptr) {
      // Ran out of buckets or variable-key space.
      return nullptr;
    } else if (key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Found an already-existing entry for this key.
      return reinterpret_cast<std::uint8_t *>(static_cast<char *>(bucket) +
                                              kValueOffset);
    }
  }

  // We are now writing to an empty bucket.
  // Write the key and hash.
  writeCompositeKeyToBucket(key, hash_code, bucket);

  std::uint8_t *value = static_cast<unsigned char *>(bucket) + kValueOffset;
  if (!key_only) {
    std::memcpy(value, init_payload_, this->total_payload_size_);
  }

  // Update the previous chaing pointer to point to the new bucket.
  pending_chain_ptr->store(pending_chain_ptr_finish_value,
                           std::memory_order_release);

  // Return the value.
  return value;
}

template <bool use_two_accessors, bool key_only, bool has_variable_size,
          typename ValueAccessorT>
inline bool PackedPayloadHashTable::upsertValueAccessorCompositeKeyInternal(
    const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
    const std::vector<MultiSourceAttributeId> &key_ids,
    ValueAccessor *base_accessor,
    ValueAccessorT *derived_accessor) {
  std::size_t variable_size = 0;
  std::vector<TypedValue> key_vector;
  key_vector.resize(key_ids.size());

  return InvokeOnAnyValueAccessor(
      base_accessor,
      [&](auto *accessor) -> bool {  // NOLINT(build/c++11)
    bool continuing = true;
    while (continuing) {
      {
        continuing = false;
        SpinSharedMutexSharedLock<true> lock(resize_shared_mutex_);
        while (accessor->next()) {
          if (use_two_accessors) {
            derived_accessor->next();
          }
          if (this->GetCompositeKeyFromValueAccessor<use_two_accessors, true>(
                  key_ids,
                  accessor,
                  derived_accessor,
                  &key_vector)) {
            continue;
          }
          if (has_variable_size) {
            variable_size =
                this->calculateVariableLengthCompositeKeyCopySize(key_vector);
          }
          std::uint8_t *value =
              this->template upsertCompositeKeyInternal<key_only>(
                  key_vector, variable_size);
          if (value == nullptr) {
            continuing = true;
            break;
          } else if (!key_only) {
            SpinMutexLock lock(*(reinterpret_cast<SpinMutex *>(value)));
            for (unsigned int k = 0; k < num_handles_; ++k) {
              const auto &ids = argument_ids[k];
              if (ids.empty()) {
                handles_[k]->updateStateNullary(value + payload_offsets_[k]);
              } else {
                const MultiSourceAttributeId &arg_id = ids.front();
                if (use_two_accessors && arg_id.source == ValueAccessorSource::kDerived) {
                  DCHECK_NE(arg_id.attr_id, kInvalidAttributeID);
                  handles_[k]->updateStateUnary(derived_accessor->getTypedValue(arg_id.attr_id),
                                                value + payload_offsets_[k]);
                } else {
                  handles_[k]->updateStateUnary(accessor->getTypedValue(arg_id.attr_id),
                                                value + payload_offsets_[k]);
                }
              }
            }
          }
        }
      }
      if (continuing) {
        this->resize(0, variable_size);
        accessor->previous();
        if (use_two_accessors) {
          derived_accessor->previous();
        }
      }
    }
    return true;
  });
}

inline void PackedPayloadHashTable::writeScalarKeyToBucket(
    const TypedValue &key,
    const std::size_t hash_code,
    void *bucket) {
  *reinterpret_cast<std::size_t *>(static_cast<char *>(bucket) +
                                   sizeof(std::atomic<std::size_t>)) =
      hash_code;
  key_manager_.writeKeyComponentToBucket(key, 0, bucket, nullptr);
}

inline void PackedPayloadHashTable::writeCompositeKeyToBucket(
    const std::vector<TypedValue> &key,
    const std::size_t hash_code,
    void *bucket) {
  DEBUG_ASSERT(key.size() == this->key_types_.size());
  *reinterpret_cast<std::size_t *>(static_cast<char *>(bucket) +
                                   sizeof(std::atomic<std::size_t>)) =
      hash_code;
  for (std::size_t idx = 0; idx < this->key_types_.size(); ++idx) {
    key_manager_.writeKeyComponentToBucket(key[idx], idx, bucket, nullptr);
  }
}

inline bool PackedPayloadHashTable::isFull(
    const std::size_t extra_variable_storage) const {
  if (header_->buckets_allocated.load(std::memory_order_relaxed) >=
      header_->num_buckets) {
    // All buckets are allocated.
    return true;
  }

  if (extra_variable_storage > 0) {
    if (extra_variable_storage +
            header_->variable_length_bytes_allocated.load(
                std::memory_order_relaxed) >
        key_manager_.getVariableLengthKeyStorageSize()) {
      // Not enough variable-length key storage space.
      return true;
    }
  }

  return false;
}

template <typename FunctorT>
inline std::size_t PackedPayloadHashTable::forEach(FunctorT *functor) const {
  std::size_t entries_visited = 0;
  std::size_t entry_num = 0;
  TypedValue key;
  const std::uint8_t *value_ptr;
  while (getNextEntry(&key, &value_ptr, &entry_num)) {
    ++entries_visited;
    (*functor)(key, value_ptr);
  }
  return entries_visited;
}

template <typename FunctorT>
inline std::size_t PackedPayloadHashTable::forEach(
    FunctorT *functor, const int index) const {
  std::size_t entries_visited = 0;
  std::size_t entry_num = 0;
  TypedValue key;
  const std::uint8_t *value_ptr;
  while (getNextEntry(&key, &value_ptr, &entry_num)) {
    ++entries_visited;
    (*functor)(key, value_ptr + payload_offsets_[index]);
    key.clear();
  }
  return entries_visited;
}

template <typename FunctorT>
inline std::size_t PackedPayloadHashTable::forEachCompositeKey(
    FunctorT *functor) const {
  std::size_t entries_visited = 0;
  std::size_t entry_num = 0;
  std::vector<TypedValue> key;
  const std::uint8_t *value_ptr;
  while (getNextEntryCompositeKey(&key, &value_ptr, &entry_num)) {
    ++entries_visited;
    (*functor)(key, value_ptr);
    key.clear();
  }
  return entries_visited;
}

template <typename FunctorT>
inline std::size_t PackedPayloadHashTable::forEachCompositeKey(
    FunctorT *functor,
    const std::size_t index) const {
  std::size_t entries_visited = 0;
  std::size_t entry_num = 0;
  std::vector<TypedValue> key;
  const std::uint8_t *value_ptr;
  while (getNextEntryCompositeKey(&key, &value_ptr, &entry_num)) {
    ++entries_visited;
    (*functor)(key, value_ptr + payload_offsets_[index]);
    key.clear();
  }
  return entries_visited;
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_PACKED_PAYLOAD_HASH_TABLE_HPP_
