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

#ifndef QUICKSTEP_STORAGE_HASH_TABLE_KEY_MANAGER_HPP_
#define QUICKSTEP_STORAGE_HASH_TABLE_KEY_MANAGER_HPP_

#include <atomic>
#include <cstddef>
#include <vector>

#include "storage/HashTableBase.hpp"
#include "storage/StorageConstants.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Helper object that manages key storage on behalf of a HashTable
 *        implementation.
 *
 * @tparam serializable Whether the HashTable can safely be saved to and
 *         loaded from disk. If false, out of band memory may be used to store
 *         variable length keys.
 * @tparam force_key_copy If true, inserted keys are always copied into
 *         HashTable memory. If false, pointers to external values may be
 *         stored instead. force_key_copy should be true if the hash table will
 *         outlive the external key values which are inserted into it. Note
 *         that if serializable is true and force_key_copy is false, then
 *         relative offsets will be used instead of absolute pointers to keys,
 *         meaning that the pointed-to keys must be serialized and deserialized
 *         in exactly the same relative byte order (e.g. as part of the same
 *         StorageBlock), and keys must not change position relative to the
 *         HashTable (beware TupleStorageSubBlocks that may self-reorganize
 *         when modified).
 **/
template <bool serializable, bool force_key_copy>
class HashTableKeyManager {
 public:
  /**
   * @brief Constructor.
   *
   * @param key_types A vector of pointers to the Types of key components for
   *        a HashTable. This is stored as a reference, not copied, so it must
   *        remain valid as long as this HashTableKeyManager will be used.
   * @param key_start_in_bucket The position in the HashTable's buckets where
   *        storage for keys begins, measured as the number of bytes from the
   *        start of the bucket.
   **/
  HashTableKeyManager(const std::vector<const Type*> &key_types,
                      const std::size_t key_start_in_bucket);

  /**
   * @return The size (in bytes) of fixed (in-bucket) key storage.
   **/
  inline std::size_t getFixedKeySize() const {
    return fixed_key_size_;
  }

  /**
   * @return The total estimated size (in bytes) of variable-length key
   *         components stored out-of-line.
   **/
  inline std::size_t getEstimatedVariableKeySize() const {
    return estimated_variable_key_size_;
  }

  /**
   * @return A vector of bools indicating which key components are stored
   *         inline in buckets.
   **/
  inline const std::vector<bool>* getKeyInline() const {
    return &key_inline_;
  }

  /**
   * @brief Set information about out-of-line variable length key storage for
   *        a HashTable.
   *
   * @param variable_length_key_storage The location in memory of
   *        variable-length key storage for the HashTable.
   * @param variable_length_key_storage_size The size (in bytes) of the region
   *        at variable_length_key_storage.
   * @param variable_length_bytes_allocated A pointer to an atomic variable
   *        that counts how many bytes of variable-length storage have actually
   *        been allocated to store keys.
   **/
  void setVariableLengthStorageInfo(
      void *variable_length_key_storage,
      const std::size_t variable_length_key_storage_size,
      std::atomic<std::size_t> *variable_length_bytes_allocated) {
    variable_length_key_storage_ = variable_length_key_storage;
    variable_length_key_storage_size_ = variable_length_key_storage_size;
    variable_length_bytes_allocated_ = variable_length_bytes_allocated;
    next_variable_length_key_offset_.store(
        variable_length_bytes_allocated_->load(std::memory_order_relaxed),
        std::memory_order_relaxed);
  }

  /**
   * @return A pointer to the variable-length key storage region for the
   *         HashTable.
   **/
  inline const void* getVariableLengthKeyStorage() const {
    return variable_length_key_storage_;
  }

  /**
   * @return The size (in bytes) of the HashTable's variable-length key storage
   *         region.
   **/
  inline std::size_t getVariableLengthKeyStorageSize() const {
    return variable_length_key_storage_size_;
  }

  /**
   * @brief Get a pointer to a component of a key in a particular bucket.
   *
   * @param bucket A pointer to a bucket in the HashTable.
   * @param component_idx The index of the desired key component (0 for single
   *        scalar keys).
   * @return A pointer to the key component specified by component_idx in
   *         bucket.
   **/
  inline const void* getKeyComponent(
      const void *bucket,
      const std::size_t component_idx) const;

  /**
   * @brief Get a component of a key in a particular bucket as a TypedValue.
   *
   * @note Unlike getKeyComponent(), this method also determines the size in
   *       bytes of the key component in order to construct the TypedValue.
   *
   * @param bucket A pointer to a bucket in the HashTable.
   * @param component_idx The index of the desired key component (0 for single
   *        scalar keys).
   * @return The key component specified by component_idx in bucket as a
   *         TypedValue.
   **/
  inline TypedValue getKeyComponentTyped(
      const void *bucket,
      const std::size_t component_idx) const;

  /**
   * @brief Check whether a key is actually equal to a scalar key stored in a
   *        HashTable bucket.
   *
   * @param key The key to check.
   * @param bucket The bucket whose key should be checked.
   * @return Whether the keys are equal.
   **/
  inline bool scalarKeyCollisionCheck(const TypedValue &key,
                                      const void *bucket) const;

  /**
   * @brief Check whether a composite key is actually equal to the composite
   *        key stored in a HashTable bucket.
   *
   * @param key The composite key to check.
   * @param bucket The bucket whose key should be checked.
   * @return Whether all the key components are equal.
   **/
  inline bool compositeKeyCollisionCheck(const std::vector<TypedValue> &key,
                                         const void *bucket) const;

  /**
   * @brief Write a key component into a bucket.
   *
   * @note If the key component is variable-length, then
   *       allocateVariableLengthKeyStorage() should have been called first
   *       (either directly or by HashTable::preallocateForBulkInsert()) to
   *       reserve out-of-line variable storage.
   *
   * @param component The key component to write.
   * @param component_idx The index of the key component (0 for single scalar
   *        keys).
   * @param bucket A pointer to the bucket in the HashTable to write the key
   *        to.
   * @param prealloc_state If non-null, variable length key components will be
   *        located in preallocated variable-length storage.
   **/
  inline void writeKeyComponentToBucket(
      const TypedValue &component,
      const std::size_t component_idx,
      void *bucket,
      HashTablePreallocationState *prealloc_state);

  /**
   * @brief Reserve storage for variable-length key components.
   *
   * @note This can be undone by calling deallocateVariableLengthKeyStorage().
   * @note Allocated storage is actually used by calling
   *       writeKeyComponentToBucket() for variable-length key components.
   *
   * @param required_bytes The size of the variable-length key storage to
   *        reserve.
   * @return true if allocation was successful, false if not enough unreserved
   *         space was available.
   **/
  inline bool allocateVariableLengthKeyStorage(
      const std::size_t required_bytes);

  /**
   * @brief Release (undo) a reservation of storage for variable-length key
   *        components previously made by allocateVariableLengthKeyStorage().
   *
   * @param bytes The size of the variable-length key storage reservation to
   *        release.
   **/
  inline void deallocateVariableLengthKeyStorage(const std::size_t bytes);

  /**
   * @return The current "next" offset to be used when actually writing
   *         variable-length keys.
   **/
  inline std::size_t getNextVariableLengthKeyOffset() const {
    return next_variable_length_key_offset_.load(std::memory_order_relaxed);
  }

  /**
   * @brief Increment the "next" offset to be used when writing variable-length
   *        keys.
   *
   * @note This is intended mainly for use by preallocateForBulkInsert() to
   *       allocate a contiguous region of variable-length storage all in one
   *       go.
   *
   * @param bytes The number of bytes to increment the offset by.
   * @return The original value of the "next" offset.
   **/
  inline std::size_t incrementNextVariableLengthKeyOffset(
      const std::size_t bytes) {
    return next_variable_length_key_offset_.fetch_add(
        bytes,
        std::memory_order_relaxed);
  }

  /**
   * @brief Reset the "next" offset for writing variable-length keys to zero.
   *
   * @warning This should only be used when clearing a HashTable, as it will
   *          cause any previously written variable-length keys to be
   *          overwritten.
   **/
  inline void zeroNextVariableLengthKeyOffset() {
    next_variable_length_key_offset_.store(0, std::memory_order_relaxed);
  }

 private:
  const std::vector<const Type*> &key_types_;
  std::size_t fixed_key_size_;
  std::size_t estimated_variable_key_size_;
  std::vector<std::size_t> key_offsets_;
  std::vector<bool> key_inline_;

  void *variable_length_key_storage_;
  std::size_t variable_length_key_storage_size_;
  std::atomic<std::size_t> *variable_length_bytes_allocated_;
  // This is separate from '*variable_length_bytes_allocated_', because we
  // allocate storage for variable-length keys up-front but defer actually
  // copying them until we find an empty bucket.
  alignas(kCacheLineBytes) std::atomic<std::size_t>
      next_variable_length_key_offset_;

  DISALLOW_COPY_AND_ASSIGN(HashTableKeyManager);
};

/** @} */

// ----------------------------------------------------------------------------
// Implementations of template class methods follow.

template <bool serializable, bool force_key_copy>
HashTableKeyManager<serializable, force_key_copy>::HashTableKeyManager(
    const std::vector<const Type*> &key_types,
    const std::size_t key_start_in_bucket)
    : key_types_(key_types),
      fixed_key_size_(0),
      estimated_variable_key_size_(0),
      variable_length_key_storage_(nullptr),
      variable_length_key_storage_size_(0),
      variable_length_bytes_allocated_(nullptr),
      next_variable_length_key_offset_(0) {
  DCHECK(!key_types_.empty());

  for (const Type *subkey_type : key_types_) {
    key_offsets_.push_back(key_start_in_bucket + fixed_key_size_);

    if (force_key_copy) {
      if (subkey_type->isVariableLength()) {
        // An offset into the variable length storage region, paired with a length.
        fixed_key_size_ += sizeof(std::size_t) * 2;
        estimated_variable_key_size_ += subkey_type->estimateAverageByteLength();
        key_inline_.push_back(false);
      } else {
        fixed_key_size_ += subkey_type->maximumByteLength();
        key_inline_.push_back(true);
      }
    } else {
      constexpr std::size_t ptr_size = serializable ? sizeof(std::ptrdiff_t)
                                                    : sizeof(const void*);
      if ((!subkey_type->isVariableLength())
          && (subkey_type->maximumByteLength() <= ptr_size + sizeof(std::size_t))) {
        fixed_key_size_ += subkey_type->maximumByteLength();
        key_inline_.push_back(true);
      } else {
        // A pointer to external memory, paired with a length.
        fixed_key_size_ += ptr_size + sizeof(std::size_t);
        key_inline_.push_back(false);
      }
    }
  }
}

template <bool serializable, bool force_key_copy>
inline const void* HashTableKeyManager<serializable, force_key_copy>
    ::getKeyComponent(const void *bucket,
                      const std::size_t component_idx) const {
  DCHECK_LT(component_idx, key_offsets_.size());

  const void *base_key_ptr = static_cast<const char*>(bucket)
                             + key_offsets_[component_idx];
  if (key_inline_[component_idx]) {
    // Component is inline.
    return base_key_ptr;
  } else if (force_key_copy) {
    // Component is copied into variable-length region.
    DCHECK(key_types_[component_idx]->isVariableLength());
    return static_cast<const char*>(variable_length_key_storage_)
           + *static_cast<const std::size_t*>(base_key_ptr);
  } else if (serializable) {
    // Relative pointer.
    return static_cast<const char*>(base_key_ptr)
           + *static_cast<const std::ptrdiff_t*>(base_key_ptr);
  } else {
    // Absolute pointer.
    return *static_cast<const void* const*>(base_key_ptr);
  }
}

template <bool serializable, bool force_key_copy>
inline TypedValue HashTableKeyManager<serializable, force_key_copy>
    ::getKeyComponentTyped(const void *bucket,
                           const std::size_t component_idx) const {
  DCHECK_LT(component_idx, key_offsets_.size());

  const Type &key_type = *(key_types_[component_idx]);
  const void *base_key_ptr = static_cast<const char*>(bucket)
                             + key_offsets_[component_idx];
  if (key_inline_[component_idx]) {
    // Component is inline.
    DCHECK(!key_type.isVariableLength());
    return key_type.makeValue(base_key_ptr,
                              key_type.maximumByteLength());
  } else if (force_key_copy) {
    // Component is copied into variable-length region.
    DCHECK(key_type.isVariableLength());
    return key_type.makeValue(static_cast<const char*>(variable_length_key_storage_)
                                  + *static_cast<const std::size_t*>(base_key_ptr),
                              *(static_cast<const std::size_t*>(base_key_ptr) + 1));
  } else if (serializable) {
    // Relative pointer.
    return key_type.makeValue(
        static_cast<const char*>(base_key_ptr)
            + *static_cast<const std::ptrdiff_t*>(base_key_ptr),
        key_type.isVariableLength() ? *(static_cast<const std::size_t*>(base_key_ptr) + 1)
                                    : key_type.maximumByteLength());
  } else {
    // Absolute pointer.
    return key_type.makeValue(
        *static_cast<const void* const*>(base_key_ptr),
        key_type.isVariableLength() ? *(static_cast<const std::size_t*>(base_key_ptr) + 1)
                                    : key_type.maximumByteLength());
  }
}

template <bool serializable, bool force_key_copy>
inline bool HashTableKeyManager<serializable, force_key_copy>
    ::scalarKeyCollisionCheck(const TypedValue &key,
                              const void *bucket) const {
  return CheckUntypedValuesEqual(*key_types_.front(),
                                 key.getDataPtr(),
                                 getKeyComponent(bucket, 0));
}

template <bool serializable, bool force_key_copy>
inline bool HashTableKeyManager<serializable, force_key_copy>
    ::compositeKeyCollisionCheck(const std::vector<TypedValue> &key,
                                 const void *bucket) const {
  DCHECK_EQ(key_types_.size(), key.size());

  // Check key components one-by-one.
  for (std::size_t idx = 0;
       idx < key_types_.size();
       ++idx) {
    if (!CheckUntypedValuesEqual(*key_types_[idx],
                                 key[idx].getDataPtr(),
                                 getKeyComponent(bucket, idx))) {
      return false;
    }
  }

  return true;
}

template <bool serializable, bool force_key_copy>
inline void HashTableKeyManager<serializable, force_key_copy>
    ::writeKeyComponentToBucket(const TypedValue &component,
                                const std::size_t component_idx,
                                void *bucket,
                                HashTablePreallocationState *prealloc_state) {
  void *key_ptr = static_cast<char*>(bucket) + key_offsets_[component_idx];
  if (key_inline_[component_idx]) {
    // Copy key component directly into bucket.
    component.copyInto(key_ptr);
  } else if (force_key_copy) {
    // Copy key into variable-length storage region and store its offset and
    // length.
    DCHECK(key_types_[component_idx]->isVariableLength());
    const std::size_t component_size = component.getDataSize();
    const std::size_t variable_length_key_pos
        = (prealloc_state == nullptr)
            ? next_variable_length_key_offset_.fetch_add(component_size,
                                                         std::memory_order_relaxed)
            : prealloc_state->variable_length_key_position;
    if (prealloc_state != nullptr) {
      prealloc_state->variable_length_key_position += component_size;
    }
    DCHECK_LE(variable_length_key_pos + component_size,
              variable_length_key_storage_size_);

    component.copyInto(static_cast<char*>(variable_length_key_storage_)
                       + variable_length_key_pos);
    *static_cast<std::size_t*>(key_ptr) = variable_length_key_pos;
    *(static_cast<std::size_t*>(key_ptr) + 1) = component_size;
  } else if (serializable) {
    // Store a relative pointer. We assume that we are pointing to memory
    // that is serialized together with the HashTable, i.e. as part of the same
    // StorageBlock. See comments regarding the 'force_key_copy' parameter in
    // storage/HashTable.hpp for more details.
    DCHECK(component.isReference());
    *static_cast<std::ptrdiff_t*>(key_ptr) = static_cast<const char*>(component.getDataPtr())
                                             - static_cast<const char*>(key_ptr);
    if (key_types_[component_idx]->isVariableLength()) {
      // Also store length if variable-length.
      *(static_cast<std::size_t*>(key_ptr) + 1) = component.getDataSize();
    }
  } else {
    // Store an absolute pointer.
    *static_cast<const void**>(key_ptr) = component.getDataPtr();
    if (key_types_[component_idx]->isVariableLength()) {
      // Also store length if variable-length.
      *(static_cast<std::size_t*>(key_ptr) + 1) = component.getDataSize();
    }
  }
}

template <bool serializable, bool force_key_copy>
inline bool HashTableKeyManager<serializable, force_key_copy>
    ::allocateVariableLengthKeyStorage(const std::size_t required_bytes) {
  if (required_bytes == 0) {
    return true;
  }

  const std::size_t original_variable_length_bytes_allocated
      = variable_length_bytes_allocated_->fetch_add(required_bytes,
                                                    std::memory_order_relaxed);
  if (original_variable_length_bytes_allocated + required_bytes
      > variable_length_key_storage_size_) {
    // Release the memory we tried to allocate.
    variable_length_bytes_allocated_->fetch_sub(required_bytes,
                                                std::memory_order_relaxed);
    return false;
  } else {
    return true;
  }
}

template <bool serializable, bool force_key_copy>
inline void HashTableKeyManager<serializable, force_key_copy>
    ::deallocateVariableLengthKeyStorage(const std::size_t bytes) {
  if (bytes == 0) {
    return;
  }
  variable_length_bytes_allocated_->fetch_sub(bytes,
                                              std::memory_order_relaxed);
}

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_HASH_TABLE_KEY_MANAGER_HPP_
