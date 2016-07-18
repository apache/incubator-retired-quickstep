/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_STORAGE_LINEAR_OPEN_ADDRESSING_HASH_TABLE_HPP_
#define QUICKSTEP_STORAGE_LINEAR_OPEN_ADDRESSING_HASH_TABLE_HPP_

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

#include "storage/HashTable.hpp"
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
 * @brief A hash table implementation which uses open addressing with linear
 *        (distance 1) probing.
 **/
template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
class LinearOpenAddressingHashTable : public HashTable<ValueT,
                                                       resizable,
                                                       serializable,
                                                       force_key_copy,
                                                       allow_duplicate_keys> {
 public:
  static std::size_t GetEstimatedMemoryInBytes(const std::size_t num_entries) {
    const std::size_t num_slots_tmp = get_next_prime_number(num_entries * kHashTableLoadFactor);
    const std::size_t bucket_size = LinearOpenAddressingHashTable::ComputeBucketSize(sizeof(std::size_t) * 2);
    // TODO(harshad) - Refine the bucket_size below with the help of KeyManager.
    // NOTE(harshad) - We ignore the estimated variable key size term below
    // assuming that we don't have variable length keys in SSB queries.
    const std::size_t required_memory = LinearOpenAddressingHashTable::ComputeBucketSize(sizeof(std::size_t) * 2)
                                  + (num_slots_tmp / kHashTableLoadFactor)
                                      * (bucket_size /*+ *key_manager_.getEstimatedVariableKeySize()*/);
    return required_memory;
  }

  // Bring in constants from HashTable.
  static constexpr unsigned char kEmptyHashByte
      = HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>::kEmptyHashByte;
  static constexpr std::size_t kEmptyHash
      = HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>::kEmptyHash;
  static constexpr std::size_t kPendingHash
      = HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>::kPendingHash;

  LinearOpenAddressingHashTable(const std::vector<const Type*> &key_types,
                                const std::size_t num_entries,
                                StorageManager *storage_manager);

  LinearOpenAddressingHashTable(const std::vector<const Type*> &key_types,
                                void *hash_table_memory,
                                const std::size_t hash_table_memory_size,
                                const bool new_hash_table,
                                const bool hash_table_memory_zeroed);

  // Delegating constructors for single scalar keys.
  LinearOpenAddressingHashTable(const Type &key_type,
                                const std::size_t num_entries,
                                StorageManager *storage_manager)
      : LinearOpenAddressingHashTable(std::vector<const Type*>(1, &key_type),
                                      num_entries,
                                      storage_manager) {
  }

  LinearOpenAddressingHashTable(const Type &key_type,
                                void *hash_table_memory,
                                const std::size_t hash_table_memory_size,
                                const bool new_hash_table,
                                const bool hash_table_memory_zeroed)
      : LinearOpenAddressingHashTable(std::vector<const Type*>(1, &key_type),
                                      hash_table_memory,
                                      hash_table_memory_size,
                                      new_hash_table,
                                      hash_table_memory_zeroed) {
  }

  ~LinearOpenAddressingHashTable() override {
    DestroyValues(hash_buckets_,
                  header_->num_buckets + header_->num_overflow_buckets,
                  bucket_size_);
  }

  void clear() override;

  std::size_t numEntries() const override;

  const ValueT* getSingle(const TypedValue &key) const override;
  const ValueT* getSingleCompositeKey(const std::vector<TypedValue> &key) const override;

  void getAll(const TypedValue &key,
              std::vector<const ValueT*> *values) const override;
  void getAllCompositeKey(const std::vector<TypedValue> &key,
                          std::vector<const ValueT*> *values) const override;

  const std::size_t getHashTableMemorySizeBytes() const override {
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

 private:
  struct Header {
    // Number of ordinary (non-overflow) buckets in the hash table.
    std::size_t num_buckets;

    // Number of extra overflow buckets.
    std::size_t num_overflow_buckets;

    // Bytes actually allocated to hold variable length keys (or key segments).
    // This is placed on its own cache line to avoid false sharing.
    alignas(kCacheLineBytes)
        std::atomic<std::size_t> variable_length_bytes_allocated;
  };

  // Write kEmptyHash to every bucket. Typically 'num_buckets' will be the
  // TOTAL number of buckets (i.e. header_->num_buckets
  // + header_->num_overflow_buckets). If 'memory_already_zeroed' is true,
  // assume that bucket memory has already been zeroed out.
  static void InitializeBuckets(void *buckets,
                                const std::size_t num_buckets,
                                const std::size_t bucket_size,
                                const bool memory_already_zeroed);

  // If ValueT is not trivially destructible, invoke its destructor for all
  // values held in the specified buckets (including those in "empty" buckets
  // that were default constructed). If ValueT is trivially destructible, this
  // is a no-op.
  static void DestroyValues(void *buckets,
                            const std::size_t num_buckets,
                            const std::size_t bucket_size);

  // Bucket size always rounds up to the alignment requirement of the atomic
  // size_t hash at the front or a ValueT, whichever is larger.
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
  // hash code.
  static constexpr std::size_t kValueOffset
      = (((sizeof(std::atomic<std::size_t>) - 1) / alignof(ValueT)) + 1) * alignof(ValueT);

  // Round bucket size up to a multiple of kBucketAlignment.
  static constexpr std::size_t ComputeBucketSize(const std::size_t fixed_key_size) {
    return (((kValueOffset + sizeof(ValueT) + fixed_key_size - 1) / kBucketAlignment) + 1)
           * kBucketAlignment;
  }

  // Attempt to find an empty bucket to insert 'hash_code' into, starting from
  // '*bucket_num'. Returns true and stores kPendingHash in bucket if an empty
  // bucket is found. Returns false if 'allow_duplicate_keys' is false and a
  // hash collision is found (caller should then check whether there is a
  // genuine key collision or the hash collision is spurious). Returns false if
  // no empty buckets are available past the initial position of '*bucket_num'.
  inline bool locateBucketForInsertion(const std::size_t hash_code,
                                       std::size_t *bucket_num,
                                       void **bucket);

  // Write a scalar 'key' and its 'hash_code' into the '*bucket', which was
  // found by locateBucketForInsertion(). Assumes that storage for a
  // variable-length key copy (if any) was already allocated by a successful
  // call to allocateVariableLengthKeyStorage().
  inline void writeScalarKeyToBucket(const TypedValue &key,
                                     const std::size_t hash_code,
                                     void *bucket);

  // Write a composite 'key' and its 'hash_code' into the '*bucket', which was
  // found by locateBucketForInsertion(). Assumes that storage for
  // variable-length key copies (if any) was already allocated by a successful
  // call to allocateVariableLengthKeyStorage().
  inline void writeCompositeKeyToBucket(const std::vector<TypedValue> &key,
                                        const std::size_t hash_code,
                                        void *bucket);

  // Determine whether it is actually necessary to resize this hash table.
  // Checks that there is at least one empty overflow bucket, and that there is
  // at least 'extra_variable_storage' bytes of variable-length storage free.
  bool isFull(const std::size_t extra_variable_storage) const;

  // Helper object to manage key storage.
  HashTableKeyManager<serializable, force_key_copy> key_manager_;

  // In-memory structure is as follows:
  //   - LinearOpenAddressingHashTable::Header
  //   - Array of hash buckets, each of which is:
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

  void *hash_buckets_;
  const std::size_t bucket_size_;

  DISALLOW_COPY_AND_ASSIGN(LinearOpenAddressingHashTable);
};

/** @} */

// ----------------------------------------------------------------------------
// Implementations of template class methods follow.

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::LinearOpenAddressingHashTable(const std::vector<const Type*> &key_types,
                                    const std::size_t num_entries,
                                    StorageManager *storage_manager)
        : HashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>(
              key_types,
              num_entries,
              storage_manager,
              true,
              false,
              false),
          key_manager_(this->key_types_, kValueOffset + sizeof(ValueT)),
          bucket_size_(ComputeBucketSize(key_manager_.getFixedKeySize())) {
  // Give base HashTable information about what key components are stored
  // inline from 'key_manager_'.
  this->setKeyInline(key_manager_.getKeyInline());

  // Pick out a prime number of buckets and calculate storage requirements.
  std::size_t num_buckets_tmp = get_next_prime_number(num_entries * kHashTableLoadFactor);
  std::size_t required_memory = sizeof(Header)
                                + (num_buckets_tmp + kLinearOpenAddressingHashTableNumOverflowBuckets)
                                    * (bucket_size_ + key_manager_.getEstimatedVariableKeySize());
  std::size_t num_slots = this->storage_manager_->SlotsNeededForBytes(required_memory);
  if (num_slots == 0) {
    FATAL_ERROR("Storage requirement for LinearOpenAddressingHashTable "
                "exceeds maximum allocation size.");
  }

  // Get a StorageBlob to hold the hash table.
  const block_id blob_id = this->storage_manager_->createBlob(num_slots);
  this->blob_ = this->storage_manager_->getBlobMutable(blob_id);

  void *aligned_memory_start = this->blob_->getMemoryMutable();
  std::size_t available_memory = num_slots * kSlotSizeBytes;
  if (align(alignof(Header),
            sizeof(Header),
            aligned_memory_start,
            available_memory)
          == nullptr) {
    // With current values from StorageConstants.hpp, this should be
    // impossible. A blob is at least 1 MB, while a Header has alignment
    // requirement of just kCacheLineBytes (64 bytes).
    FATAL_ERROR("StorageBlob used to hold resizable "
                "LinearOpenAddressingHashTable is too small to meet alignment "
                "requirements of LinearOpenAddressingHashTableHeader.");
  } else if (aligned_memory_start != this->blob_->getMemoryMutable()) {
    // This should also be impossible, since the StorageManager allocates slots
    // aligned to kCacheLineBytes.
    DEV_WARNING("StorageBlob memory adjusted by "
                << (num_slots * kSlotSizeBytes - available_memory)
                << " bytes to meet alignment requirement for "
                << "LinearOpenAddressingHashTable::Header.");
  }

  // Locate the header.
  header_ = static_cast<Header*>(aligned_memory_start);
  available_memory -= sizeof(Header);

  hash_buckets_ = static_cast<char*>(aligned_memory_start)
                  + sizeof(Header);
  // Extra-paranoid: sizeof(Header) should almost certainly be a multiple of
  // kBucketAlignment, unless ValueT has some members with seriously big
  // (> kCacheLineBytes) alignment requirements specified using alignas().
  if (align(kBucketAlignment,
            bucket_size_,
            hash_buckets_,
            available_memory)
          == nullptr) {
    FATAL_ERROR("StorageBlob used to hold resizable "
                "LinearOpenAddressingHashTable is too small to meet "
                "alignment requirements of buckets.");
  } else if (hash_buckets_
             != reinterpret_cast<const char*>(header_) + sizeof(Header)) {
    DEV_WARNING("Bucket array start position adjusted to meet alignment "
                "requirement for LinearOpenAddressingHashTable's value type.");
  }

  // Recompute the number of buckets using the actual available memory. Most
  // likely, we got some extra free bucket space due to "rounding up" to the
  // storage blob's size. It's also possible (though very unlikely) that we
  // will wind up with fewer buckets than we initially wanted because of screwy
  // alignment requirements for ValueT, as noted above.
  num_buckets_tmp
      = get_previous_prime_number((available_memory / (bucket_size_ + key_manager_.getEstimatedVariableKeySize()))
                                  - kLinearOpenAddressingHashTableNumOverflowBuckets);
  DEBUG_ASSERT(num_buckets_tmp > 0);

  // Fill in the header and initialize buckets.
  header_->num_buckets = num_buckets_tmp;
  header_->num_overflow_buckets = kLinearOpenAddressingHashTableNumOverflowBuckets;
  available_memory -= bucket_size_ * (header_->num_buckets + header_->num_overflow_buckets);
  InitializeBuckets(hash_buckets_,
                    header_->num_buckets + header_->num_overflow_buckets,
                    bucket_size_,
                    true);

  // There may be some leftover memory from going down to a prime number of
  // ordinary buckets. This will be used as extra storage for variable-length
  // key segments. Although we could fill it with additional overflow buckets,
  // we don't, because it is preferable to resize a HashTable rather than let
  // its overflow chain grow very long.

  // Locate variable-length key storage region, and give it all the remaining
  // bytes in the blob.
  header_->variable_length_bytes_allocated.store(0, std::memory_order_relaxed);
  key_manager_.setVariableLengthStorageInfo(
      static_cast<char*>(hash_buckets_)
          + (header_->num_buckets + header_->num_overflow_buckets) * bucket_size_,
      available_memory,
      &(header_->variable_length_bytes_allocated));
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::LinearOpenAddressingHashTable(const std::vector<const Type*> &key_types,
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
              true,
              false,
              false),
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
                << "LinearOpenAddressingHashTable with "
                << available_memory << " bytes of memory at "
                << aligned_memory_start << " which either can not fit a "
                << "LinearOpenAddressingHashTable::Header or meet its "
                << "alignment requirement.");
  } else if (aligned_memory_start != this->hash_table_memory_) {
    // In general, we could get memory of any alignment, although at least
    // cache-line aligned would be nice.
    DEV_WARNING("StorageBlob memory adjusted by "
                << (this->hash_table_memory_size_ - available_memory)
                << " bytes to meet alignment requirement for "
                << "LinearOpenAddressingHashTable::Header.");
  }

  header_ = static_cast<Header*>(aligned_memory_start);

  hash_buckets_ = static_cast<char*>(aligned_memory_start) + sizeof(Header);
  // Extra-paranoid: sizeof(Header) should almost certainly be a multiple of
  // kBucketAlignment, unless ValueT has some members with seriously big
  // (> kCacheLineBytes) alignment requirements specified using alignas().
  if (align(kBucketAlignment,
            bucket_size_,
            hash_buckets_,
            available_memory)
          == nullptr) {
    FATAL_ERROR("Attempted to create a non-resizable "
                << "LinearOpenAddressingHashTable with "
                << this->hash_table_memory_size_ << " bytes of memory at "
                << this->hash_table_memory_ << ", which can hold an aligned "
                << "LinearOpenAddressingHashTable::Header but does not have "
                << "enough remaining space for even a single hash bucket.");
  } else if (hash_buckets_ != reinterpret_cast<const char*>(header_) + sizeof(Header)) {
    DEV_WARNING("Bucket array start position adjusted to meet alignment "
                "requirement for LinearOpenAddressingHashTable's value type.");
  }

  if (new_hash_table) {
    std::size_t estimated_bucket_capacity
        = (available_memory - sizeof(Header))
          / (bucket_size_ + key_manager_.getEstimatedVariableKeySize());
    std::size_t estimated_overflow_buckets
        = estimated_bucket_capacity * kFixedSizeLinearOpenAddressingHashTableOverflowFactor
            > kLinearOpenAddressingHashTableNumOverflowBuckets
                ? kLinearOpenAddressingHashTableNumOverflowBuckets
                : estimated_bucket_capacity * kFixedSizeLinearOpenAddressingHashTableOverflowFactor;

    std::size_t regular_buckets = get_next_prime_number(
        estimated_bucket_capacity - estimated_overflow_buckets);
    if (regular_buckets > estimated_bucket_capacity) {
      // Next prime was too large. Try the previous prime instead.
      regular_buckets = get_previous_prime_number(
          estimated_bucket_capacity - estimated_overflow_buckets);
    }

    // Initialize the header.
    header_->num_buckets = regular_buckets;
    header_->num_overflow_buckets = estimated_bucket_capacity - regular_buckets;
    header_->variable_length_bytes_allocated.store(0, std::memory_order_relaxed);

    InitializeBuckets(hash_buckets_,
                      header_->num_buckets + header_->num_overflow_buckets,
                      bucket_size_,
                      hash_table_memory_zeroed);
  }
  available_memory -= bucket_size_ * (header_->num_buckets + header_->num_overflow_buckets);

  // Locate variable-length key storage region.
  key_manager_.setVariableLengthStorageInfo(
      static_cast<char*>(hash_buckets_)
          + (header_->num_buckets + header_->num_overflow_buckets) * bucket_size_,
      available_memory,
      &(header_->variable_length_bytes_allocated));
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::clear() {
  header_->variable_length_bytes_allocated.store(0, std::memory_order_relaxed);
  key_manager_.zeroNextVariableLengthKeyOffset();

  DestroyValues(hash_buckets_,
                header_->num_buckets + header_->num_overflow_buckets,
                bucket_size_);
  InitializeBuckets(hash_buckets_,
                    header_->num_buckets + header_->num_overflow_buckets,
                    bucket_size_,
                    false);
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
std::size_t LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::numEntries() const {
  std::size_t count = 0;

  const char *endpos = static_cast<const char*>(hash_buckets_)
                       + (header_->num_buckets + header_->num_overflow_buckets) * bucket_size_;
  for (const char *bucket = static_cast<const char*>(hash_buckets_);
       bucket < endpos;
       bucket += bucket_size_) {
    count += (reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed)
              != kEmptyHash);
  }

  return count;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
const ValueT* LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getSingle(const TypedValue &key) const {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = this->AdjustHash(key.getHash());
  for (std::size_t bucket_num = hash_code % header_->num_buckets;
       bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++bucket_num) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + bucket_num * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket without finding 'key'.
      return nullptr;
    }

    // None of the get methods should be called while inserts are still taking
    // place.
    DEBUG_ASSERT(bucket_hash != kPendingHash);

    if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Match located.
      return reinterpret_cast<const ValueT*>(bucket + kValueOffset);
    }
  }

  // Reached the end of buckets and didn't find a match.
  return nullptr;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
const ValueT* LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getSingleCompositeKey(const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->AdjustHash(this->hashCompositeKey(key));
  for (std::size_t bucket_num = hash_code % header_->num_buckets;
       bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++bucket_num) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + bucket_num * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket without finding 'key'.
      return nullptr;
    }

    // None of the get methods should be called while inserts are still taking
    // place.
    DEBUG_ASSERT(bucket_hash != kPendingHash);

    if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      return reinterpret_cast<const ValueT*>(bucket + kValueOffset);
    }
  }

  // Reached the end of buckets and didn't find a match.
  return nullptr;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAll(const TypedValue &key, std::vector<const ValueT*> *values) const {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = this->AdjustHash(key.getHash());
  for (std::size_t bucket_num = hash_code % header_->num_buckets;
       bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++bucket_num) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + bucket_num * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket, so we're finished.
      return;
    }

    // None of the get methods should be called while inserts are still taking
    // place.
    DEBUG_ASSERT(bucket_hash != kPendingHash);

    if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Match located.
      values->push_back(reinterpret_cast<const ValueT*>(bucket + kValueOffset));
      if (!allow_duplicate_keys) {
        return;
      }
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getAllCompositeKey(const std::vector<TypedValue> &key, std::vector<const ValueT*> *values) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->AdjustHash(this->hashCompositeKey(key));
  for (std::size_t bucket_num = hash_code % header_->num_buckets;
       bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++bucket_num) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + bucket_num * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket, so we're finished.
      return;
    }

    // None of the get methods should be called while inserts are still taking
    // place.
    DEBUG_ASSERT(bucket_hash != kPendingHash);

    if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      values->push_back(reinterpret_cast<const ValueT*>(bucket + kValueOffset));
      if (!allow_duplicate_keys) {
        return;
      }
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
HashTablePutResult
    LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
        ::putInternal(const TypedValue &key,
                      const std::size_t variable_key_size,
                      const ValueT &value,
                      HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));
  DEBUG_ASSERT(prealloc_state == nullptr);

  // TODO(chasseur): If allow_duplicate_keys is true, avoid storing more than
  // one copy of the same variable-length key.
  if (!key_manager_.allocateVariableLengthKeyStorage(variable_key_size)) {
    // Ran out of variable-length key storage space.
    return HashTablePutResult::kOutOfSpace;
  }

  const std::size_t hash_code = this->AdjustHash(key.getHash());
  std::size_t bucket_num =  hash_code % header_->num_buckets;
  void *bucket = nullptr;
  while (bucket_num < header_->num_buckets + header_->num_overflow_buckets) {
    if (locateBucketForInsertion(hash_code, &bucket_num, &bucket)) {
      // Found an empty bucket.
      break;
    }

    // Found at least a hash collision. Check for duplicate keys if necessary.
    if (!allow_duplicate_keys
        && (bucket_num < header_->num_buckets + header_->num_overflow_buckets)
        && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Duplicate key. Deallocate any variable storage space and return.
      key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
      return HashTablePutResult::kDuplicateKey;
    } else {
      // Duplicates are allowed, or the hash collision was spurious.
      ++bucket_num;
    }
  }
  if (bucket_num >= header_->num_buckets + header_->num_overflow_buckets) {
    // Ran out of buckets. Deallocate any variable space that we were unable to
    // use.
    key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
    return HashTablePutResult::kOutOfSpace;
  }

  writeScalarKeyToBucket(key, hash_code, bucket);

  // Store the value by using placement new with ValueT's copy constructor.
  new(static_cast<char*>(bucket) + kValueOffset) ValueT(value);

  // We're all done.
  return HashTablePutResult::kOK;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
HashTablePutResult
    LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
        ::putCompositeKeyInternal(const std::vector<TypedValue> &key,
                                  const std::size_t variable_key_size,
                                  const ValueT &value,
                                  HashTablePreallocationState *prealloc_state) {
  DEBUG_ASSERT(this->key_types_.size() == key.size());
  DEBUG_ASSERT(prealloc_state == nullptr);

  // TODO(chasseur): If allow_duplicate_keys is true, avoid storing more than
  // one copy of the same variable-length key.
  if (!key_manager_.allocateVariableLengthKeyStorage(variable_key_size)) {
    // Ran out of variable-length key storage space.
    return HashTablePutResult::kOutOfSpace;
  }

  const std::size_t hash_code = this->AdjustHash(this->hashCompositeKey(key));
  std::size_t bucket_num =  hash_code % header_->num_buckets;
  void *bucket = nullptr;
  while (bucket_num < header_->num_buckets + header_->num_overflow_buckets) {
    if (locateBucketForInsertion(hash_code, &bucket_num, &bucket)) {
      // Found an empty bucket.
      break;
    }

    // Found at least a hash collision. Check for duplicate keys if
    // necessary.
    if (!allow_duplicate_keys
        && (bucket_num < header_->num_buckets + header_->num_overflow_buckets)
        && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Duplicate key. Deallocate any variable storage space and return.
      key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
      return HashTablePutResult::kDuplicateKey;
    } else {
      // Duplicates are allowed, or the hash collision was spurious.
      ++bucket_num;
    }
  }
  if (bucket_num >= header_->num_buckets + header_->num_overflow_buckets) {
    // Ran out of buckets. Deallocate any variable space that we were unable to
    // use.
    key_manager_.deallocateVariableLengthKeyStorage(variable_key_size);
    return HashTablePutResult::kOutOfSpace;
  }

  writeCompositeKeyToBucket(key, hash_code, bucket);

  // Store the value by using placement new with ValueT's copy constructor.
  new(static_cast<char*>(bucket) + kValueOffset) ValueT(value);

  // We're all done.
  return HashTablePutResult::kOK;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
ValueT* LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsertInternal(const TypedValue &key,
                     const std::size_t variable_key_size,
                     const ValueT &initial_value) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  // Block is do/while(false) so we can use break.
  do {
    if (variable_key_size > 0) {
      // Don't allocate yet, since the key may already be present. However, we
      // do check if either the allocated variable storage space OR the free
      // space is big enough to hold the key (at least one must be true: either
      // the key is already present and allocated, or we need to be able to
      // allocate enough space for it).
      std::size_t allocated_bytes = header_->variable_length_bytes_allocated.load(std::memory_order_relaxed);
      if ((allocated_bytes < variable_key_size)
          && (allocated_bytes + variable_key_size > key_manager_.getVariableLengthKeyStorageSize())) {
        break;
      }
    }

    const std::size_t hash_code = this->AdjustHash(key.getHash());
    std::size_t bucket_num =  hash_code % header_->num_buckets;
    void *bucket = nullptr;
    while (bucket_num < header_->num_buckets + header_->num_overflow_buckets) {
      if (locateBucketForInsertion(hash_code, &bucket_num, &bucket)) {
        // Found an empty bucket.
        break;
      } else if ((bucket_num < header_->num_buckets + header_->num_overflow_buckets)
                 && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
        // Found an already-existing entry for this key.
        return reinterpret_cast<ValueT*>(static_cast<char*>(bucket) + kValueOffset);
      } else {
        ++bucket_num;
      }
    }
    if (bucket_num >= header_->num_buckets + header_->num_overflow_buckets) {
      // Ran out of buckets.
      break;
    }

    // We are now writing to an empty bucket. Allocate storage for
    // variable-length key, if needed.
    if (!key_manager_.allocateVariableLengthKeyStorage(variable_key_size)) {
      // Allocation failed. Abandon this insert.
      static_cast<std::atomic<std::size_t>*>(bucket)->store(kEmptyHash,
                                                            std::memory_order_release);
      break;
    }

    // Copy the supplied 'initial_value' into place.
    ValueT *value = new(static_cast<char*>(bucket) + kValueOffset) ValueT(initial_value);

    // Write the key.
    writeScalarKeyToBucket(key, hash_code, bucket);

    // Return the value.
    return value;
  } while (false);

  // If we have reached this point without returning, the key was not already
  // present and we either failed to find an empty bucket to insert into or
  // we failed to allocate enough space to store a variable-length key.
  return nullptr;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
ValueT* LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::upsertCompositeKeyInternal(const std::vector<TypedValue> &key,
                                 const std::size_t variable_key_size,
                                 const ValueT &initial_value) {
  DEBUG_ASSERT(!allow_duplicate_keys);
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  // Block is do/while(false) so we can use break.
  do {
    if (variable_key_size > 0) {
      // Don't allocate yet, since the key may already be present. However, we
      // do check if either the allocated variable storage space OR the free
      // space is big enough to hold the key (at least one must be true: either
      // the key is already present and allocated, or we need to be able to
      // allocate enough space for it).
      std::size_t allocated_bytes = header_->variable_length_bytes_allocated.load(std::memory_order_relaxed);
      if ((allocated_bytes < variable_key_size)
          && (allocated_bytes + variable_key_size > key_manager_.getVariableLengthKeyStorageSize())) {
        break;
      }
    }

    const std::size_t hash_code = this->AdjustHash(this->hashCompositeKey(key));
    std::size_t bucket_num =  hash_code % header_->num_buckets;
    void *bucket = nullptr;
    while (bucket_num < header_->num_buckets + header_->num_overflow_buckets) {
      if (locateBucketForInsertion(hash_code, &bucket_num, &bucket)) {
        // Found an empty bucket.
        break;
      } else if ((bucket_num < header_->num_buckets + header_->num_overflow_buckets)
                 && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
        // Found an already-existing entry for this key.
        return reinterpret_cast<ValueT*>(static_cast<char*>(bucket) + kValueOffset);
      } else {
        ++bucket_num;
      }
    }
    if (bucket_num >= header_->num_buckets + header_->num_overflow_buckets) {
      // Ran out of buckets.
      break;
    }

    // We are now writing to an empty bucket. Allocate storage for
    // variable-length key, if needed.
    if (!key_manager_.allocateVariableLengthKeyStorage(variable_key_size)) {
      // Allocation failed. Abandon this insert.
      static_cast<std::atomic<std::size_t>*>(bucket)->store(kEmptyHash,
                                                            std::memory_order_release);
      break;
    }

    // Copy the supplied 'initial_value' into place.
    ValueT *value = new(static_cast<char*>(bucket) + kValueOffset) ValueT(initial_value);

    // Write the key.
    writeCompositeKeyToBucket(key, hash_code, bucket);

    // Return the value.
    return value;
  } while (false);

  // If we have reached this point without returning, the key was not already
  // present and we either failed to find an empty bucket to insert into or
  // we failed to allocate enough space to store a variable-length key.
  return nullptr;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntry(TypedValue *key, const ValueT **value, std::size_t *entry_num) const {
  DEBUG_ASSERT(this->key_types_.size() == 1);

  while (*entry_num < header_->num_buckets + header_->num_overflow_buckets) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + (*entry_num) * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash != kEmptyHash) {
      DEBUG_ASSERT(bucket_hash != kPendingHash);
      *key = key_manager_.getKeyComponentTyped(bucket, 0);
      *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);

      // Increment '*entry_num' before returning so that the next call will
      // start looking at the next bucket.
      ++(*entry_num);
      return true;
    } else {
      ++(*entry_num);
    }
  }

  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntryCompositeKey(std::vector<TypedValue> *key,
                               const ValueT **value,
                               std::size_t *entry_num) const {
  while (*entry_num < header_->num_buckets + header_->num_overflow_buckets) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + (*entry_num) * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash != kEmptyHash) {
      DEBUG_ASSERT(bucket_hash != kPendingHash);
      for (std::vector<const Type*>::size_type key_idx = 0;
           key_idx < this->key_types_.size();
           ++key_idx) {
        key->emplace_back(key_manager_.getKeyComponentTyped(bucket, key_idx));
      }
      *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);

      // Increment '*entry_num' before returning so that the next call will
      // start looking at the next bucket.
      ++(*entry_num);
      return true;
    } else {
      ++(*entry_num);
    }
  }

  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntryForKey(const TypedValue &key,
                         const std::size_t hash_code,
                         const ValueT **value,
                         std::size_t *entry_num) const {
  DEBUG_ASSERT(this->key_types_.size() == 1);
  DEBUG_ASSERT(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  if (*entry_num == 0) {
    *entry_num = hash_code % header_->num_buckets;
  }

  while (*entry_num < header_->num_buckets + header_->num_overflow_buckets) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + (*entry_num) * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket, so we are all done with this key.
      return false;
    } else if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Match located.
      *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);

      // Increment '*entry_num' before returning so that the next call will
      // start looking at the next bucket.
      ++(*entry_num);
      return true;
    } else {
      ++(*entry_num);
    }
  }

  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::getNextEntryForCompositeKey(const std::vector<TypedValue> &key,
                                  const std::size_t hash_code,
                                  const ValueT **value,
                                  std::size_t *entry_num) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  if (*entry_num == 0) {
    *entry_num = hash_code % header_->num_buckets;
  }

  while (*entry_num < header_->num_buckets + header_->num_overflow_buckets) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + (*entry_num) * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket, so we are all done with this key.
      return false;
    } else if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      *value = reinterpret_cast<const ValueT*>(bucket + kValueOffset);

      // Increment '*entry_num' before returning so that the next call will
      // start looking at the next bucket.
      ++(*entry_num);
      return true;
    } else {
      ++(*entry_num);
    }
  }

  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::hasKey(const TypedValue &key) const {
  DCHECK_EQ(1u, this->key_types_.size());
  DCHECK(key.isPlausibleInstanceOf(this->key_types_.front()->getSignature()));

  const std::size_t hash_code = this->AdjustHash(key.getHash());
  for (std::size_t bucket_num = hash_code % header_->num_buckets;
       bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++bucket_num) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + bucket_num * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket, so the search is finished
      // without finding any match.
      return false;
    }

    // None of the get methods should be called while inserts are still taking
    // place.
    DCHECK(bucket_hash != kPendingHash);

    if ((bucket_hash == hash_code) && key_manager_.scalarKeyCollisionCheck(key, bucket)) {
      // Match located.
      return true;
    }
  }
  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::hasCompositeKey(const std::vector<TypedValue> &key) const {
  DEBUG_ASSERT(this->key_types_.size() == key.size());

  const std::size_t hash_code = this->AdjustHash(this->hashCompositeKey(key));
  for (std::size_t bucket_num = hash_code % header_->num_buckets;
       bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++bucket_num) {
    const char *bucket = static_cast<const char*>(hash_buckets_) + bucket_num * bucket_size_;
    const std::size_t bucket_hash
        = reinterpret_cast<const std::atomic<std::size_t>*>(bucket)->load(std::memory_order_relaxed);
    if (bucket_hash == kEmptyHash) {
      // Hit an empty bucket, so the search is finished
      // without finding any match.
      return false;
    }

    // None of the get methods should be called while inserts are still taking
    // place.
    DEBUG_ASSERT(bucket_hash != kPendingHash);

    if ((bucket_hash == hash_code) && key_manager_.compositeKeyCollisionCheck(key, bucket)) {
      // Match located.
      return true;
    }
  }
  return false;
}

// TODO(chasseur): Smarter heuristics that are more selective about whether
// to grow hash buckets, variable-length storage, or both, and to what degree.
template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::resize(const std::size_t extra_buckets,
             const std::size_t extra_variable_storage,
             const std::size_t retry_num) {
  DEBUG_ASSERT(resizable);

  // Only actually lock on the first call to resize. Recursive calls will
  // inherit the lock. This is necessary because exclusive locks on a
  // SpinSharedMutex do not support recursion.
  DynamicConditionalSpinSharedMutexExclusiveLock<true> write_lock(this->resize_shared_mutex_,
                                                                  retry_num == 0);

  // Recheck whether the hash table is still full. Note that multiple threads
  // might wait to rebuild this hash table simultaneously. Only the first one
  // should do the rebuild.
  if ((retry_num == 0) && !isFull(extra_variable_storage)) {
    return;
  }

  // Approximately double the number of buckets (or increase by more if we are
  // retrying a failed resize).
  //
  // TODO(chasseur): It may be worth it to more than double the number of
  // buckets here so that we can maintain a good, sparse fill factor for a
  // longer time as more values are inserted. Such behavior should take into
  // account kHashTableLoadFactor.
  std::size_t resized_regular_buckets
      = get_next_prime_number((header_->num_buckets + header_->num_overflow_buckets)
                              * (2 + retry_num));

  std::size_t variable_storage_required
      = (resized_regular_buckets + kLinearOpenAddressingHashTableNumOverflowBuckets)
        * key_manager_.getEstimatedVariableKeySize();
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
        + (resized_regular_buckets + kLinearOpenAddressingHashTableNumOverflowBuckets)
            * bucket_size_
        + variable_storage_required;
  const std::size_t resized_num_slots
      = this->storage_manager_->SlotsNeededForBytes(resized_memory_required);
  if (resized_num_slots == 0) {
    FATAL_ERROR("Storage requirement for resized LinearOpenAddressingHashTable "
                "exceeds maximum allocation size.");
  }

  // Get a new StorageBlob to hold the resized hash table.
  const block_id resized_blob_id = this->storage_manager_->createBlob(resized_num_slots);
  MutableBlobReference resized_blob = this->storage_manager_->getBlobMutable(resized_blob_id);

  // Locate data structures inside the new StorageBlob.
  void *aligned_memory_start = resized_blob->getMemoryMutable();
  std::size_t available_memory = resized_num_slots * kSlotSizeBytes;
  if (align(alignof(Header),
            sizeof(Header),
            aligned_memory_start,
            available_memory)
          == nullptr) {
    // Should be impossible, as noted in constructor.
    FATAL_ERROR("StorageBlob used to hold resized LinearOpenAddressingHashTable "
                "is too small to meet alignment requirements of "
                "LinearOpenAddressingHashTable::Header.");
  } else if (aligned_memory_start != resized_blob->getMemoryMutable()) {
    // Again, should be impossible.
    DEV_WARNING("In LinearOpenAddressingHashTable::resize(), StorageBlob "
                << "memory adjusted by "
                << (resized_num_slots * kSlotSizeBytes - available_memory)
                << " bytes to meet alignment requirement for "
                << "LinearOpenAddressingHashTable::Header.");
  }

  Header *resized_header = static_cast<Header*>(aligned_memory_start);
  available_memory -= sizeof(Header);

  // As in constructor, we will be extra paranoid and use align() to locate the
  // start of the array of hash buckets, as well.
  void *resized_hash_buckets = static_cast<char*>(aligned_memory_start) + sizeof(Header);
  if (align(kBucketAlignment,
            bucket_size_,
            resized_hash_buckets,
            available_memory)
          == nullptr) {
    FATAL_ERROR("StorageBlob used to hold resized LinearOpenAddressingHashTable "
                "is too small to meet alignment requirements of buckets.");
  } else if (resized_hash_buckets
             != reinterpret_cast<const char*>(resized_header) + sizeof(Header)) {
    DEV_WARNING("Bucket array start position adjusted to meet alignment "
                "requirement for LinearOpenAddressingHashTable's value type.");
  }

  // As in constructor, recompute the number of regular buckets using the
  // actual available memory.
  resized_regular_buckets = get_previous_prime_number(
      ((available_memory - extra_variable_storage)
          / (bucket_size_ + key_manager_.getEstimatedVariableKeySize()))
      - kLinearOpenAddressingHashTableNumOverflowBuckets);
  DEBUG_ASSERT(resized_regular_buckets > header_->num_buckets);
  available_memory -= bucket_size_
                      * (resized_regular_buckets + kLinearOpenAddressingHashTableNumOverflowBuckets);

  void *resized_variable_length_key_storage
      = static_cast<char*>(resized_hash_buckets)
        + (resized_regular_buckets + kLinearOpenAddressingHashTableNumOverflowBuckets) * bucket_size_;
  const std::size_t resized_variable_length_key_storage_size = available_memory;

  // Initialize the header.
  resized_header->num_buckets = resized_regular_buckets;
  resized_header->num_overflow_buckets = kLinearOpenAddressingHashTableNumOverflowBuckets;
  resized_header->variable_length_bytes_allocated.store(
      original_variable_storage_used,
      std::memory_order_relaxed);

  // Initialize buckets in the resized hash table.
  InitializeBuckets(resized_hash_buckets,
                    resized_header->num_buckets + resized_header->num_overflow_buckets,
                    bucket_size_,
                    true);

  // Copy non-empty buckets into the resized hash table.
  for (std::size_t original_bucket_num = 0;
       original_bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++original_bucket_num) {
    const void *original_bucket = static_cast<const char*>(hash_buckets_)
                                  + original_bucket_num * bucket_size_;
    const std::size_t hash_code
        = static_cast<const std::atomic<std::size_t>*>(original_bucket)->load(std::memory_order_relaxed);
    // No other thread should be inserting while we hold the exclusive lock on
    // 'resize_shared_mutex_'.
    DEBUG_ASSERT(hash_code != kPendingHash);

    if (hash_code != kEmptyHash) {
      // Locate an empty bucket in the resized hash table to copy into.
      bool copied = false;
      for (std::size_t resized_bucket_num = hash_code % (resized_header->num_buckets);
           resized_bucket_num < resized_header->num_buckets + resized_header->num_overflow_buckets;
           ++resized_bucket_num) {
        void *resized_bucket = static_cast<char*>(resized_hash_buckets)
                               + resized_bucket_num * bucket_size_;
        if (static_cast<const std::atomic<std::size_t>*>(resized_bucket)->load(std::memory_order_relaxed)
            == kEmptyHash) {
          // Empty bucket found, now we copy. Note that a byte-for-byte copy is
          // safe because:
          //     1. The hash code will stay the same.
          //     2. For key components:
          //       a. Inline keys will stay exactly the same.
          //       b. Offsets into variable-length storage will remain valid,
          //          because we also do a byte-for-byte copy of
          //          variable-length storage below.
          //       c. Absolute external pointers will still point to the same
          //          address.
          //       d. Relative pointers are not used with resizable hash
          //          tables.
          //     3. If values are not trivially copyable, then we invoke
          //        ValueT's copy constructor with placement new.
          //
          // TODO(chasseur): std::is_trivially_copyable is not yet implemented
          // in GCC 4.8.3, so we assume we need to invoke ValueT's copy
          // constructor, even though memcpy could suffice for many possible
          // ValueTs.
          std::memcpy(resized_bucket, original_bucket, bucket_size_);
          const ValueT *original_value = reinterpret_cast<const ValueT*>(
              static_cast<const char*>(original_bucket) + kValueOffset);
          void *resized_value_ptr = static_cast<char*>(resized_bucket) + kValueOffset;
          // TODO(chasseur): We could use std::move here to try and avoid a
          // deep-copy of value, but that would require that we move values
          // BACK to the original buckets in the case of a failed resize.
          new(resized_value_ptr) ValueT(*original_value);

          copied = true;
          break;
        }
      }
      if (!copied) {
        // We ran out of buckets. Free the blob we allocated and try again with
        // even more memory.
        DestroyValues(resized_hash_buckets,
                      resized_header->num_buckets + resized_header->num_overflow_buckets,
                      bucket_size_);
        resized_blob.release();
        this->storage_manager_->deleteBlockOrBlobFile(resized_blob_id);
        resize(extra_variable_storage, retry_num + 1);
        return;
      }
    }
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
  DestroyValues(hash_buckets_,
                header_->num_buckets + header_->num_overflow_buckets,
                bucket_size_);

  // Resized hash table is fully built, now make it active.
  std::swap(this->blob_, resized_blob);
  header_ = resized_header;
  hash_buckets_ = resized_hash_buckets;
  key_manager_.setVariableLengthStorageInfo(
      resized_variable_length_key_storage,
      resized_variable_length_key_storage_size,
      &(resized_header->variable_length_bytes_allocated));

  // Drop the old blob.
  const block_id old_blob_id = resized_blob->getID();
  resized_blob.release();
  this->storage_manager_->deleteBlockOrBlobFile(old_blob_id);
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::InitializeBuckets(void *buckets,
                        const std::size_t num_buckets,
                        const std::size_t bucket_size,
                        const bool memory_already_zeroed) {
  // Initialize hash buckets to special "empty" hash value. Writing key storage
  // as well is harmless, while values will be initialized on their first use.
  if ((kEmptyHashByte != 0x0) || !memory_already_zeroed) {
    std::memset(buckets,
                kEmptyHashByte,
                num_buckets * bucket_size);
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::DestroyValues(void *hash_buckets,
                    const std::size_t num_buckets,
                    const std::size_t bucket_size) {
  if (!std::is_trivially_destructible<ValueT>::value) {
    void *hash_ptr = hash_buckets;
    void *value_ptr = static_cast<char*>(hash_buckets) + kValueOffset;
    for (std::size_t bucket_num = 0;
         bucket_num < num_buckets;
         ++bucket_num) {
      if (static_cast<std::atomic<std::size_t>*>(hash_ptr)->load(
              std::memory_order_relaxed) != kEmptyHash) {
        static_cast<ValueT*>(value_ptr)->~ValueT();
      }
      hash_ptr = static_cast<char*>(hash_ptr) + bucket_size;
      value_ptr = static_cast<char*>(value_ptr) + bucket_size;
    }
  }
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::locateBucketForInsertion(const std::size_t hash_code,
                               std::size_t *bucket_num,
                               void **bucket) {
  std::atomic<std::size_t> *hash_in_bucket = reinterpret_cast<std::atomic<std::size_t>*>(
      static_cast<char*>(hash_buckets_) + (*bucket_num) * bucket_size_);
  for (;
       *bucket_num < header_->num_buckets + header_->num_overflow_buckets;
       ++(*bucket_num)) {
    std::size_t existing_hash = kEmptyHash;
    if (hash_in_bucket->compare_exchange_strong(existing_hash,
                                                kPendingHash,
                                                std::memory_order_acq_rel)) {
      *bucket = hash_in_bucket;
      return true;
    } else if (!allow_duplicate_keys) {
      // Spin until the real hash value is available.
      while (existing_hash == kPendingHash) {
        existing_hash = hash_in_bucket->load(std::memory_order_acquire);
      }
      if (existing_hash == hash_code) {
        // This method only checks for a hash collision. It is the caller's
        // responsibility to check whether there is a genuine key collision or
        // a spurious hash collision.
        *bucket = hash_in_bucket;
        return false;
      } else if (existing_hash == kEmptyHash) {
        // Edge case: hash was pending, but another thread failed to allocate
        // variable-length storage and had to abandon its insert in
        // upsert()/upsertCompositeKey(). We will try to use the same
        // bucket again. Note that we do NOT need to check for this edge case
        // if 'allow_duplicate_keys' is true, because then the upsert methods
        // will not be usable, and we will always check that there is enough
        // variable-length storage for an insert up-front before locating a
        // bucket to insert into (i.e. once a bucket changes from kEmptyHash,
        // it will never become empty again).
        --(*bucket_num);
        continue;
      }
    }
    hash_in_bucket = reinterpret_cast<std::atomic<std::size_t>*>(
        reinterpret_cast<char*>(hash_in_bucket) + bucket_size_);
  }

  *bucket = hash_in_bucket;
  return false;
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::writeScalarKeyToBucket(const TypedValue &key,
                             const std::size_t hash_code,
                             void *bucket) {
  key_manager_.writeKeyComponentToBucket(key, 0, bucket, nullptr);

  // Store the real hash.
  static_cast<std::atomic<std::size_t>*>(bucket)->store(
      hash_code,
      std::memory_order_release);
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
inline void LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::writeCompositeKeyToBucket(const std::vector<TypedValue> &key,
                                const std::size_t hash_code,
                                void *bucket) {
  DEBUG_ASSERT(key.size() == this->key_types_.size());
  for (std::size_t idx = 0;
       idx < this->key_types_.size();
       ++idx) {
    key_manager_.writeKeyComponentToBucket(key[idx], idx, bucket, nullptr);
  }

  // Store the real hash.
  static_cast<std::atomic<std::size_t>*>(bucket)->store(
      hash_code,
      std::memory_order_release);
}

template <typename ValueT,
          bool resizable,
          bool serializable,
          bool force_key_copy,
          bool allow_duplicate_keys>
bool LinearOpenAddressingHashTable<ValueT, resizable, serializable, force_key_copy, allow_duplicate_keys>
    ::isFull(const std::size_t extra_variable_storage) const {
  const char *last_bucket
      = static_cast<const char*>(hash_buckets_)
        + (header_->num_buckets + header_->num_overflow_buckets - 1) * bucket_size_;
  const std::size_t last_hash
      = reinterpret_cast<const std::atomic<std::size_t>*>(last_bucket)->load(
          std::memory_order_acquire);
  if (last_hash != kEmptyHash) {
    // Last bucket is non-empty.
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

#endif  // QUICKSTEP_STORAGE_LINEAR_OPEN_ADDRESSING_HASH_TABLE_HPP_
