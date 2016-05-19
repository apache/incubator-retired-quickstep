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

/**
 * @brief A simple Bloom Filter implementation with basic primitives
 *        based on Partow's Bloom Filter implementation.
 **/
class BloomFilter {
 public:
  static const uint32_t kNumBitsPerByte = 8;

  /**
   * @brief Constructor.
   * @note When no bit_array is being passed to the constructor,
   *       then the bit_array is owned and managed by this class.
   *
   * @param random_seed A random_seed that generates unique hash functions.
   * @param hash_fn_count The number of hash functions used by this bloom filter.
   * @param bit_array_size_in_bytes Size of the bit array.
   **/
  BloomFilter(const std::uint64_t random_seed,
              const std::size_t hash_fn_count,
              const std::uint64_t bit_array_size_in_bytes)
      : random_seed_(random_seed),
        hash_fn_count_(hash_fn_count),
        array_size_in_bytes_(bit_array_size_in_bytes),
        array_size_(array_size_in_bytes_ * kNumBitsPerByte),
        bit_array_(new std::uint8_t[array_size_in_bytes_]),
        is_bit_array_owner_(true) {
    reset();
    generate_unique_hash_fn();
  }

  /**
   * @brief Constructor.
   * @note When a bit_array is passed as an argument to the constructor,
   *       then the ownership of the bit array lies with the caller.
   *
   * @param random_seed A random_seed that generates unique hash functions.
   * @param hash_fn_count The number of hash functions used by this bloom filter.
   * @param bit_array_size_in_bytes Size of the bit array.
   * @param bit_array A pointer to the memory region that is used to store bit array.
   * @param is_initialized A boolean that indicates whether to zero-out the region
   *                       before use or not.
   **/
  BloomFilter(const std::uint64_t random_seed,
              const std::size_t hash_fn_count,
              const std::uint64_t bit_array_size_in_bytes,
              std::uint8_t *bit_array,
              const bool is_initialized)
      : random_seed_(random_seed),
        hash_fn_count_(hash_fn_count),
        array_size_in_bytes_(bit_array_size_in_bytes),
        array_size_(bit_array_size_in_bytes * kNumBitsPerByte),
        bit_array_(bit_array),  // Owned by the calling method.
        is_bit_array_owner_(false) {
    if (!is_initialized) {
      reset();
    }
    generate_unique_hash_fn();
  }

  /**
   * @brief Constructor.
   * @note When a bloom filter proto is passed as an initializer,
   *       then the bit_array is owned and managed by this class.
   *
   * @param bloom_filter_proto The protobuf representation of a
   *        bloom filter configuration.
   **/
  explicit BloomFilter(const serialization::BloomFilter &bloom_filter_proto)
      : random_seed_(bloom_filter_proto.bloom_filter_seed()),
        hash_fn_count_(bloom_filter_proto.number_of_hashes()),
        array_size_in_bytes_(bloom_filter_proto.bloom_filter_size()),
        array_size_(array_size_in_bytes_ * kNumBitsPerByte),
        bit_array_(new std::uint8_t[array_size_in_bytes_]),
        is_bit_array_owner_(true) {
    reset();
    generate_unique_hash_fn();
  }

  /**
   * @brief Destructor.
   **/
  ~BloomFilter() {
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
   * @brief Get the random seed that was used to initialize this bloom filter.
   *
   * @return Returns the random seed.
   **/
  inline std::uint64_t getRandomSeed() const {
    return random_seed_;
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
      compute_indices(hash_id(key_begin, length, hash_fn_[i]), &bit_index, &bit);
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
      compute_indices(hash_id(key_begin, length, hash_fn_[i]), &bit_index, &bit);
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
      compute_indices(hash_id(key_begin, length, hash_fn_[i]), &bit_index, &bit);
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
  inline void bitwiseOr(const BloomFilter *bloom_filter) {
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

  void generate_unique_hash_fn() {
    hash_fn_.reserve(hash_fn_count_);
    const std::uint32_t predef_hash_fn_count = 128;
    static const std::uint32_t predef_hash_fn[predef_hash_fn_count] = {
       0xAAAAAAAA, 0x55555555, 0x33333333, 0xCCCCCCCC,
       0x66666666, 0x99999999, 0xB5B5B5B5, 0x4B4B4B4B,
       0xAA55AA55, 0x55335533, 0x33CC33CC, 0xCC66CC66,
       0x66996699, 0x99B599B5, 0xB54BB54B, 0x4BAA4BAA,
       0xAA33AA33, 0x55CC55CC, 0x33663366, 0xCC99CC99,
       0x66B566B5, 0x994B994B, 0xB5AAB5AA, 0xAAAAAA33,
       0x555555CC, 0x33333366, 0xCCCCCC99, 0x666666B5,
       0x9999994B, 0xB5B5B5AA, 0xFFFFFFFF, 0xFFFF0000,
       0xB823D5EB, 0xC1191CDF, 0xF623AEB3, 0xDB58499F,
       0xC8D42E70, 0xB173F616, 0xA91A5967, 0xDA427D63,
       0xB1E8A2EA, 0xF6C0D155, 0x4909FEA3, 0xA68CC6A7,
       0xC395E782, 0xA26057EB, 0x0CD5DA28, 0x467C5492,
       0xF15E6982, 0x61C6FAD3, 0x9615E352, 0x6E9E355A,
       0x689B563E, 0x0C9831A8, 0x6753C18B, 0xA622689B,
       0x8CA63C47, 0x42CC2884, 0x8E89919B, 0x6EDBD7D3,
       0x15B6796C, 0x1D6FDFE4, 0x63FF9092, 0xE7401432,
       0xEFFE9412, 0xAEAEDF79, 0x9F245A31, 0x83C136FC,
       0xC3DA4A8C, 0xA5112C8C, 0x5271F491, 0x9A948DAB,
       0xCEE59A8D, 0xB5F525AB, 0x59D13217, 0x24E7C331,
       0x697C2103, 0x84B0A460, 0x86156DA9, 0xAEF2AC68,
       0x23243DA5, 0x3F649643, 0x5FA495A8, 0x67710DF8,
       0x9A6C499E, 0xDCFB0227, 0x46A43433, 0x1832B07A,
       0xC46AFF3C, 0xB9C8FFF0, 0xC9500467, 0x34431BDF,
       0xB652432B, 0xE367F12B, 0x427F4C1B, 0x224C006E,
       0x2E7E5A89, 0x96F99AA5, 0x0BEB452A, 0x2FD87C39,
       0x74B2E1FB, 0x222EFD24, 0xF357F60C, 0x440FCB1E,
       0x8BBE030F, 0x6704DC29, 0x1144D12F, 0x948B1355,
       0x6D8FD7E9, 0x1C11A014, 0xADD1592F, 0xFB3C712E,
       0xFC77642F, 0xF9C4CE8C, 0x31312FB9, 0x08B0DD79,
       0x318FA6E7, 0xC040D23D, 0xC0589AA7, 0x0CA5C075,
       0xF874B172, 0x0CF914D5, 0x784D3280, 0x4E8CFEBC,
       0xC569F575, 0xCDB2A091, 0x2CC016B4, 0x5C5F4421
    };
    if (hash_fn_count_ <= predef_hash_fn_count) {
      std::copy(predef_hash_fn, predef_hash_fn + hash_fn_count_, hash_fn_.begin());
      for (std::uint32_t i = 0; i < hash_fn_.size(); ++i) {
        hash_fn_[i] = hash_fn_[i] * hash_fn_[(i + 3) % hash_fn_count_] + static_cast<std::uint32_t>(random_seed_);
      }
    } else {
      LOG(FATAL) << "Requested number of hash functions is too large.";
    }
  }
  
  inline std::uint32_t hash_id(const std::uint8_t *begin, std::size_t remaining_length, std::uint32_t hash) const {
    std::uint32_t hash_val = 0;
    for (std::size_t i = 0; i < remaining_length && i < 4; ++i) {
      std::uint32_t mask = 0;
      mask |= begin[i];
      mask = mask << ((i * 8) - 1);
      hash_val |= mask;
    }
    return hash_val;
  }
  
  inline std::uint32_t hash_ap(const std::uint8_t *begin, std::size_t remaining_length, std::uint32_t hash) const {
    const std::uint8_t *itr = begin;
    std::uint32_t loop = 0;
    while (remaining_length >= 8) {
      const std::uint32_t &i1 = *(reinterpret_cast<const std::uint32_t*>(itr)); itr += sizeof(std::uint32_t);
      const std::uint32_t &i2 = *(reinterpret_cast<const std::uint32_t*>(itr)); itr += sizeof(std::uint32_t);
      hash ^= (hash <<  7) ^  i1 * (hash >> 3) ^ (~((hash << 11) + (i2 ^ (hash >> 5))));
      remaining_length -= 8;
    }
    if (remaining_length) {
      if (remaining_length >= 4) {
        const std::uint32_t &i = *(reinterpret_cast<const std::uint32_t*>(itr));
        if (loop & 0x01) {
          hash ^= (hash <<  7) ^  i * (hash >> 3);
        } else {
          hash ^= (~((hash << 11) + (i ^ (hash >> 5))));
        }
        ++loop;
        remaining_length -= 4;
        itr += sizeof(std::uint32_t);
      }
      if (remaining_length >= 2) {
        const std::uint16_t &i = *(reinterpret_cast<const std::uint16_t*>(itr));
        if (loop & 0x01) {
          hash ^= (hash <<  7) ^  i * (hash >> 3);
        } else {
          hash ^= (~((hash << 11) + (i ^ (hash >> 5))));
        }
        ++loop;
        remaining_length -= 2;
        itr += sizeof(std::uint16_t);
      }
      if (remaining_length) {
        hash += ((*itr) ^ (hash * 0xA5A5A5A5)) + loop;
      }
    }
    return hash;
  }

 private:
  const std::uint64_t random_seed_;
  std::vector<std::uint32_t> hash_fn_;
  const std::uint32_t hash_fn_count_;
  std::uint64_t array_size_in_bytes_;
  std::uint64_t array_size_;
  std::unique_ptr<std::uint8_t> bit_array_;
  std::uint32_t inserted_element_count_;
  const bool is_bit_array_owner_;

  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> bloom_filter_insert_mutex_;

  DISALLOW_COPY_AND_ASSIGN(BloomFilter);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BLOOM_FILTER_HPP
