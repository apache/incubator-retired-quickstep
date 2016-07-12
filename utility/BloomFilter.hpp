/**
*   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
*     University of Wisconsin—Madison.
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
*
*   The hashing function used by the simple Bloom Filter implementation below
*   is a slightly modified version of the original hashing function, which was
*   written by Arash Partow and placed in public domain. The Quickstep team
*   hereby disclaims the credits to this part of the code.
**/

#ifndef QUICKSTEP_UTILITY_BLOOM_FILTER_HPP
#define QUICKSTEP_UTILITY_BLOOM_FILTER_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "storage/StorageConstants.hpp"
#include "threading/Mutex.hpp"
#include "threading/SharedMutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "utility/BloomFilter.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
/** \addtogroup Utility
 *  @{
 */

class BloomFilterOriginal;
class BloomFilterBlocked;
typedef BloomFilterBlocked BloomFilter;

#define PREV_CACHE_LINE_SIZE_MULTIPLE(n)  ((n)/kCacheLineBytes) * kCacheLineBytes

/**
 * @brief A "blocked" version of Bloom Filter based on this paper:
 *        Putze, Felix, Peter Sanders, and Johannes Singler.
 *        "Cache-, hash-and space-efficient bloom filters."
 *        International Workshop on Experimental and Efficient Algorithms.
 *        Springer Berlin Heidelberg, 2007.
 **/
class BloomFilterBlocked {
 public:
  static const std::uint8_t kNumBitsPerByte = 8;
  static const std::uint8_t kMaxNumHashFns = 8;

  // This union allows us to read/write position in convenient fashion,
  // through nested structs and their bitfield members
  //
  // A position can simply be a 32-bit hash
  // Or it can be a cache line (block of 512 bits) and position within it
  // Or it can be a byte (block of 8 bits) and position within it
  union Position {
    std::uint32_t hash;
    struct CacheLinePosition {
      unsigned index_in_line : 9;
      unsigned line_num : 23;
    } cache_line_pos;
    struct BytePosition {
      unsigned index_in_byte : 3;
      unsigned byte_num : 29;
    } byte_pos;
  };

  /**
   * @brief Constructor.
   * @note When no bit_array is being passed to the constructor,
   *       then the bit_array is owned and managed by this class.
   *
   * @param hash_fn_count The number of hash functions used by this bloom filter.
   * @param bit_array_size_in_bytes Size of the bit array.
   **/
  BloomFilterBlocked(const std::uint8_t hash_fn_count,
              const std::uint64_t bit_array_size_in_bytes)
      : hash_fn_count_(hash_fn_count),
        array_size_in_bytes_(PREV_CACHE_LINE_SIZE_MULTIPLE(bit_array_size_in_bytes)),
        is_bit_array_owner_(true),
        bit_array_(new std::uint8_t[array_size_in_bytes_]) {
    reset();
  }

  /**
   * @brief Constructor.
   * @note When a bit_array is passed as an argument to the constructor,
   *       then the ownership of the bit array lies with the caller.
   *
   * @param hash_fn_count The number of hash functions used by this bloom filter.
   * @param bit_array_size_in_bytes Size of the bit array.
   * @param bit_array A pointer to the memory region that is used to store bit array.
   * @param is_initialized A boolean that indicates whether to zero-out the region
   *                       before use or not.
   **/
  BloomFilterBlocked(const std::uint8_t hash_fn_count,
              const std::uint64_t bit_array_size_in_bytes,
              std::uint8_t *bit_array,
              const bool is_initialized)
      : hash_fn_count_(hash_fn_count),
        array_size_in_bytes_(PREV_CACHE_LINE_SIZE_MULTIPLE(bit_array_size_in_bytes)),
        is_bit_array_owner_(false),
        bit_array_(bit_array) {  // Owned by the calling method.
    if (!is_initialized) {
      reset();
    }
  }

  /**
   * @brief Constructor.
   * @note When a bloom filter proto is passed as an initializer,
   *       then the bit_array is owned and managed by this class.
   *
   * @param bloom_filter_proto The protobuf representation of a
   *        bloom filter configuration.
   **/
  explicit BloomFilterBlocked(const serialization::BloomFilter &bloom_filter_proto)
      : hash_fn_count_(bloom_filter_proto.number_of_hashes()),
        array_size_in_bytes_(bloom_filter_proto.bloom_filter_size()),
        is_bit_array_owner_(true),
        bit_array_(new std::uint8_t[array_size_in_bytes_]) {
    reset();
  }

  /**
   * @brief Destructor.
   **/
  ~BloomFilterBlocked() {
    if (is_bit_array_owner_) {
      bit_array_.reset();
    } else {
      bit_array_.release();
    }
  }

  static bool ProtoIsValid(const serialization::BloomFilter &bloom_filter_proto) {
    return bloom_filter_proto.IsInitialized();
  }

  /**
   * @brief Zeros out the contents of the bit array.
   **/
  inline void reset() {
    // Initialize the bit_array with all zeros.
    std::fill_n(bit_array_.get(), array_size_in_bytes_, 0x00);
    inserted_element_count_ = 0;
  }

  /**
   * @brief Get the number of hash functions used in this bloom filter.
   *
   * @return Returns the number of hash functions.
   **/
  inline std::uint8_t getNumberOfHashes() const {
    return hash_fn_count_;
  }

  /**
   * @brief Get the size of the bit array in bytes for this bloom filter.
   *
   * @return Returns the bit array size (in bytes).
   **/
  inline std::uint64_t getBitArraySize() const {
    return array_size_in_bytes_;
  }

  /**
   * @brief Get the constant pointer to the bit array for this bloom filter
   *
   * @return Returns constant pointer to the bit array.
   **/
  inline const std::uint8_t* getBitArray() const {
    return bit_array_.get();
  }

  template <typename T>
  void insert(const T value) {
    insert(reinterpret_cast<const std::uint8_t *>(&value), sizeof(T));
  }

  /**
   * @brief Inserts a given value into the bloom filter in a thread-safe manner.
   *
   * @param key_begin A pointer to the value being inserted.
   * @param length Size of the value being inserted in bytes.
   */
  inline void insert(const std::uint8_t *key_begin, const std::size_t length) {
    Position positions[kMaxNumHashFns];
    getPositions(key_begin, length, positions);
    {
      SpinSharedMutexExclusiveLock<false> exclusive_writer_lock(bloom_filter_insert_mutex_);
      for (std::uint8_t i = 0; i < hash_fn_count_; ++i)
        setBitAtPosition(positions[i]);
    }
    ++inserted_element_count_;
  }

  template <typename T>
  void insertUnSafe(const T value) {
    insertUnSafe(reinterpret_cast<const std::uint8_t *>(&value), sizeof(T));
  }

  /**
   * @brief Inserts a given value into the bloom filter.
   * @Warning This is a faster thread-unsafe version of the insert() function.
   *          The caller needs to ensure the thread safety.
   *
   * @param key_begin A pointer to the value being inserted.
   * @param length Size of the value being inserted in bytes.
   */
  inline void insertUnSafe(const std::uint8_t *key_begin, const std::size_t length) {
    Position positions[kMaxNumHashFns];
    getPositions(key_begin, length, positions);
    for (std::uint8_t i = 0; i < hash_fn_count_; ++i)
      setBitAtPosition(positions[i]);
    ++inserted_element_count_;
  }

  template <typename T>
  bool contains(const T value) {
    return contains(reinterpret_cast<const std::uint8_t *>(&value), sizeof(T));
  }

  /**
   * @brief Test membership of a given value in the bloom filter.
   *        If true is returned, then a value may or may not be present in the bloom filter.
   *        If false is returned, a value is certainly not present in the bloom filter.
   *
   * @note The membersip test does not require any locks, because the assumption is that
   *       the bloom filter will only be used after it has been built.
   *
   * @param key_begin A pointer to the value being tested for membership.
   * @param length Size of the value being inserted in bytes.
   */
  inline bool contains(const std::uint8_t *key_begin, const std::size_t length) const {
    Position positions[kMaxNumHashFns];
    getPositions(key_begin, length, positions);
    for (std::uint8_t i = 0; i < hash_fn_count_; ++i)
      if (!getBitAtPosition(positions[i]))
        return false;
    return true;
  }

  /**
   * @brief Perform a bitwise-OR of the given Bloom filter with this bloom filter.
   *        Essentially, it does a union of this bloom filter with the passed bloom filter.
   *
   * @param bloom_filter A const pointer to the bloom filter object to do bitwise-OR with.
   */
  inline void bitwiseOr(const BloomFilterBlocked *bloom_filter) {
    SpinSharedMutexExclusiveLock<false> exclusive_writer_lock(bloom_filter_insert_mutex_);
    for (std::size_t byte_index = 0; byte_index < bloom_filter->getBitArraySize(); ++byte_index) {
      (bit_array_.get())[byte_index] |= bloom_filter->getBitArray()[byte_index];
    }
  }

  /**
   * @brief Return the number of elements currently inserted into bloom filter.
   *
   * @return The number of elements inserted into bloom filter.
   **/
  inline std::uint32_t element_count() const {
    return inserted_element_count_;
  }

 protected:
  void getPositions(
      const std::uint8_t *begin,
      std::size_t length,
      Position positions[]) const {
    positions[0].hash = hash_multiplicative(begin, length, hash_fn_[0]);
    for (std::size_t i = 1; i < hash_fn_count_; ++i) {
      positions[i].hash = hash_multiplicative(begin, length, hash_fn_[i]);
      positions[i].cache_line_pos.line_num = positions[0].cache_line_pos.line_num;
    }
  }

  void setBitAtPosition(const Position &pos) {
    (bit_array_.get())[pos.byte_pos.byte_num] |= (1 << pos.byte_pos.index_in_byte);
  }

  bool getBitAtPosition(const Position &pos) const {
    return (bit_array_.get())[pos.byte_pos.byte_num] & (1 << pos.byte_pos.index_in_byte);
  }

  inline std::uint32_t hash_multiplicative(
      const std::uint8_t *begin,
      std::size_t length,
      const std::uint64_t multiplier) const {
    std::uint32_t hash = 0;
    std::size_t bytes_hashed = 0;
    if (length >= 4) {
      while (bytes_hashed < length) {
        auto val = *reinterpret_cast<const std::uint32_t *>(begin + bytes_hashed);
        hash += (multiplier * val) >> 24;
        bytes_hashed += 4;
      }
    }
    while (bytes_hashed < length) {
      std::uint8_t val = *(begin + bytes_hashed);
      hash += (multiplier * val) >> 32;
      bytes_hashed++;
    }
    return hash % (array_size_in_bytes_ * kNumBitsPerByte);
  }

 private:
  const std::uint32_t hash_fn_count_;
  const std::uint64_t array_size_in_bytes_;
  std::uint32_t inserted_element_count_;
  const bool is_bit_array_owner_;

  static constexpr std::uint64_t kKnuthGoldenRatioNumber = 2654435761;
  const std::uint64_t hash_fn_[kMaxNumHashFns] = { // hash_fn_[i] is 2**(i+1) - 1
    0x00000001 * kKnuthGoldenRatioNumber, // 0x00000003, 0x00000007, 0x0000000f,
    0x0000001f * kKnuthGoldenRatioNumber, // 0x0000003f, 0x0000007f, 0x000000ff,
    0x000001ff * kKnuthGoldenRatioNumber, // 0x000003ff, 0x000007ff, 0x00000fff,
    0x00001fff * kKnuthGoldenRatioNumber, // 0x00003fff, 0x00007fff, 0x0000ffff,
    0x0001ffff * kKnuthGoldenRatioNumber, // 0x0003ffff, 0x0007ffff, 0x000fffff,
    0x001fffff * kKnuthGoldenRatioNumber, // 0x003fffff, 0x007fffff, 0x00ffffff,
    0x01ffffff * kKnuthGoldenRatioNumber, // 0x03ffffff, 0x07ffffff, 0x0fffffff,
    0x1fffffff * kKnuthGoldenRatioNumber  // 0x3fffffff, 0x7fffffff, 0xffffffff
    };

  alignas(kCacheLineBytes) std::unique_ptr<std::uint8_t> bit_array_;
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> bloom_filter_insert_mutex_;

  DISALLOW_COPY_AND_ASSIGN(BloomFilterBlocked);
};

/**
 * @brief A simple Bloom Filter implementation with basic primitives
 *        based on Partow's Bloom Filter implementation.
 **/
class BloomFilterOriginal {
 public:
  static const uint32_t kNumBitsPerByte = 8;

  /**
   * @brief Constructor.
   * @note When no bit_array is being passed to the constructor,
   *       then the bit_array is owned and managed by this class.
   *
   * @param hash_fn_count The number of hash functions used by this bloom filter.
   * @param bit_array_size_in_bytes Size of the bit array.
   **/
  BloomFilterOriginal(const std::size_t hash_fn_count,
              const std::uint64_t bit_array_size_in_bytes)
      : hash_fn_count_(hash_fn_count),
        array_size_in_bytes_(bit_array_size_in_bytes),
        array_size_(array_size_in_bytes_ * kNumBitsPerByte),
        bit_array_(new std::uint8_t[array_size_in_bytes_]),
        is_bit_array_owner_(true) {
    reset();
  }

  /**
   * @brief Constructor.
   * @note When a bit_array is passed as an argument to the constructor,
   *       then the ownership of the bit array lies with the caller.
   *
   * @param hash_fn_count The number of hash functions used by this bloom filter.
   * @param bit_array_size_in_bytes Size of the bit array.
   * @param bit_array A pointer to the memory region that is used to store bit array.
   * @param is_initialized A boolean that indicates whether to zero-out the region
   *                       before use or not.
   **/
  BloomFilterOriginal(const std::size_t hash_fn_count,
              const std::uint64_t bit_array_size_in_bytes,
              std::uint8_t *bit_array,
              const bool is_initialized)
      : hash_fn_count_(hash_fn_count),
        array_size_in_bytes_(bit_array_size_in_bytes),
        array_size_(bit_array_size_in_bytes * kNumBitsPerByte),
        bit_array_(bit_array),  // Owned by the calling method.
        is_bit_array_owner_(false) {
    if (!is_initialized) {
      reset();
    }
  }

  /**
   * @brief Constructor.
   * @note When a bloom filter proto is passed as an initializer,
   *       then the bit_array is owned and managed by this class.
   *
   * @param bloom_filter_proto The protobuf representation of a
   *        bloom filter configuration.
   **/
  explicit BloomFilterOriginal(const serialization::BloomFilter &bloom_filter_proto)
      : hash_fn_count_(bloom_filter_proto.number_of_hashes()),
        array_size_in_bytes_(bloom_filter_proto.bloom_filter_size()),
        array_size_(array_size_in_bytes_ * kNumBitsPerByte),
        bit_array_(new std::uint8_t[array_size_in_bytes_]),
        is_bit_array_owner_(true) {
    reset();
  }

  /**
   * @brief Destructor.
   **/
  ~BloomFilterOriginal() {
    if (is_bit_array_owner_) {
      bit_array_.reset();
    } else {
      bit_array_.release();
    }
  }

  static bool ProtoIsValid(const serialization::BloomFilter &bloom_filter_proto) {
    return bloom_filter_proto.IsInitialized();
  }

  /**
   * @brief Zeros out the contents of the bit array.
   **/
  inline void reset() {
    // Initialize the bit_array with all zeros.
    std::fill_n(bit_array_.get(), array_size_in_bytes_, 0x00);
    inserted_element_count_ = 0;
  }

  /**
   * @brief Get the number of hash functions used in this bloom filter.
   *
   * @return Returns the number of hash functions.
   **/
  inline std::uint32_t getNumberOfHashes() const {
    return hash_fn_count_;
  }

  /**
   * @brief Get the size of the bit array in bytes for this bloom filter.
   *
   * @return Returns the bit array size (in bytes).
   **/
  inline std::uint64_t getBitArraySize() const {
    return array_size_in_bytes_;
  }

  /**
   * @brief Get the constant pointer to the bit array for this bloom filter
   *
   * @return Returns constant pointer to the bit array.
   **/
  inline const std::uint8_t* getBitArray() const {
    return bit_array_.get();
  }

  /**
   * @brief Inserts a given value into the bloom filter in a thread-safe manner.
   *
   * @param key_begin A pointer to the value being inserted.
   * @param length Size of the value being inserted in bytes.
   */
  inline void insert(const std::uint8_t *key_begin, const std::size_t length) {
    // Locks are needed during insertion, when multiple workers may be modifying the
    // bloom filter concurrently. However, locks are not required during membership test.
    std::size_t bit_index = 0;
    std::size_t bit = 0;
    std::vector<std::pair<std::size_t, std::size_t>> modified_bit_positions;
    std::vector<bool> is_bit_position_correct;

    // Determine all the bit positions that are required to be set.
    for (std::size_t i = 0; i < hash_fn_count_; ++i) {
      compute_indices(hash_multiplicative(key_begin, length, hash_fn_[i]), &bit_index, &bit);
      modified_bit_positions.push_back(std::make_pair(bit_index, bit));
    }

    // Acquire a reader lock and check which of the bit positions are already set.
    {
      SpinSharedMutexSharedLock<false> shared_reader_lock(bloom_filter_insert_mutex_);
      for (std::size_t i = 0; i < hash_fn_count_; ++i) {
        bit_index = modified_bit_positions[i].first;
        bit = modified_bit_positions[i].second;
        if (((bit_array_.get())[bit_index / kNumBitsPerByte] & (1 << bit)) != (1 << bit)) {
          is_bit_position_correct.push_back(false);
        } else {
          is_bit_position_correct.push_back(true);
        }
      }
    }

    // Acquire a writer lock and set the bit positions are which are not set.
    {
      SpinSharedMutexExclusiveLock<false> exclusive_writer_lock(bloom_filter_insert_mutex_);
      for (std::size_t i = 0; i < hash_fn_count_; ++i) {
        if (!is_bit_position_correct[i]) {
          bit_index = modified_bit_positions[i].first;
          bit = modified_bit_positions[i].second;
          (bit_array_.get())[bit_index / kNumBitsPerByte] |= (1 << bit);
        }
      }
    }
    ++inserted_element_count_;
  }

  /**
   * @brief Inserts a given value into the bloom filter.
   * @Warning This is a faster thread-unsafe version of the insert() function.
   *          The caller needs to ensure the thread safety.
   *
   * @param key_begin A pointer to the value being inserted.
   * @param length Size of the value being inserted in bytes.
   */
  inline void insertUnSafe(const std::uint8_t *key_begin, const std::size_t length) {
    std::size_t bit_index = 0;
    std::size_t bit = 0;

    for (std::size_t i = 0; i < hash_fn_count_; ++i) {
      compute_indices(hash_multiplicative(key_begin, length, hash_fn_[i]), &bit_index, &bit);
      (bit_array_.get())[bit_index / kNumBitsPerByte] |= (1 << bit);
    }

    ++inserted_element_count_;
  }

  /**
   * @brief Test membership of a given value in the bloom filter.
   *        If true is returned, then a value may or may not be present in the bloom filter.
   *        If false is returned, a value is certainly not present in the bloom filter.
   *
   * @note The membersip test does not require any locks, because the assumption is that
   *       the bloom filter will only be used after it has been built.
   *
   * @param key_begin A pointer to the value being tested for membership.
   * @param length Size of the value being inserted in bytes.
   */
  inline bool contains(const std::uint8_t *key_begin, const std::size_t length) const {
    std::size_t bit_index = 0;
    std::size_t bit = 0;
    for (std::size_t i = 0; i < hash_fn_count_; ++i) {
      compute_indices(hash_multiplicative(key_begin, length, hash_fn_[i]), &bit_index, &bit);
      if (((bit_array_.get())[bit_index / kNumBitsPerByte] & (1 << bit)) != (1 << bit)) {
        return false;
      }
    }
    return true;
  }

  /**
   * @brief Perform a bitwise-OR of the given Bloom filter with this bloom filter.
   *        Essentially, it does a union of this bloom filter with the passed bloom filter.
   *
   * @param bloom_filter A const pointer to the bloom filter object to do bitwise-OR with.
   */
  inline void bitwiseOr(const BloomFilterOriginal *bloom_filter) {
    SpinSharedMutexExclusiveLock<false> exclusive_writer_lock(bloom_filter_insert_mutex_);
    for (std::size_t byte_index = 0; byte_index < bloom_filter->getBitArraySize(); ++byte_index) {
      (bit_array_.get())[byte_index] |= bloom_filter->getBitArray()[byte_index];
    }
  }

  /**
   * @brief Return the number of elements currently inserted into bloom filter.
   *
   * @return The number of elements inserted into bloom filter.
   **/
  inline std::size_t element_count() const {
    return inserted_element_count_;
  }

 protected:
  inline void compute_indices(const std::uint32_t &hash, std::size_t *bit_index, std::size_t *bit) const {
    *bit_index = hash % array_size_;
    *bit = *bit_index % kNumBitsPerByte;
  }

  inline std::uint32_t hash_multiplicative(
      const std::uint8_t *begin,
      std::size_t remaining_length,
      const std::uint64_t multiplier) const {
    std::uint32_t hash = 0;
    std::size_t bytes_hashed = 0;
    if (remaining_length >= 4) {
      while (bytes_hashed < remaining_length) {
        auto val = *reinterpret_cast<const std::uint32_t *>(begin + bytes_hashed);
        hash += (multiplier * val) >> 32;
        bytes_hashed += 4;
      }
    }
    while (bytes_hashed < remaining_length) {
      std::uint8_t val = *(begin + bytes_hashed);
      hash += (multiplier * val) >> 32;
      bytes_hashed++;
    }
    return hash;
  }

 private:
  const std::uint32_t hash_fn_count_;
  std::uint64_t array_size_in_bytes_;
  std::uint64_t array_size_;
  std::unique_ptr<std::uint8_t> bit_array_;
  std::uint32_t inserted_element_count_;
  const bool is_bit_array_owner_;

  static constexpr std::uint64_t kKnuthGoldenRatioNumber = 2654435761;
  static constexpr std::size_t kMaxNumHashFns = 8;
  const std::uint64_t hash_fn_[kMaxNumHashFns] = { // hash_fn_[i] is 2**(i+1) - 1
    0x00000001 * kKnuthGoldenRatioNumber, // 0x00000003, 0x00000007, 0x0000000f,
    0x0000001f * kKnuthGoldenRatioNumber, // 0x0000003f, 0x0000007f, 0x000000ff,
    0x000001ff * kKnuthGoldenRatioNumber, // 0x000003ff, 0x000007ff, 0x00000fff,
    0x00001fff * kKnuthGoldenRatioNumber, // 0x00003fff, 0x00007fff, 0x0000ffff,
    0x0001ffff * kKnuthGoldenRatioNumber, // 0x0003ffff, 0x0007ffff, 0x000fffff,
    0x001fffff * kKnuthGoldenRatioNumber, // 0x003fffff, 0x007fffff, 0x00ffffff,
    0x01ffffff * kKnuthGoldenRatioNumber, // 0x03ffffff, 0x07ffffff, 0x0fffffff,
    0x1fffffff * kKnuthGoldenRatioNumber  // 0x3fffffff, 0x7fffffff, 0xffffffff
    };
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> bloom_filter_insert_mutex_;

  DISALLOW_COPY_AND_ASSIGN(BloomFilterOriginal);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BLOOM_FILTER_HPP
