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

#ifndef QUICKSTEP_UTILITY_BIT_VECTOR_HPP_
#define QUICKSTEP_UTILITY_BIT_VECTOR_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>

#include "utility/BitManipulation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief An interface for using a region of memory as vector of bits (i.e.
 *        bools).
 * @param enable_short_version If true, short bit vectors (<= 32 bits) will use
 *        less memory at the cost of slightly reduced performance due to
 *        runtime branches. If false, all bit vectors will use a minimum of 8
 *        bytes of storage, but performance will be improved.
 **/
template <bool enable_short_version>
class BitVector {
 public:
  static const bool kShortVersionEnabled = enable_short_version;

  /**
   * @brief Constructor for a BitVector with external storage.
   * @note This constructor can be used to create a new BitVector, or to
   *       reconstitute and access a BitVector which was created earlier. If
   *       creating a new BitVector, clear() should be called after the
   *       constructor.
   *
   * @param memory_location The location of memory to use for the BitVector.
   *        Use BytesNeeded() to determine how much memory is needed.
   * @param num_bits The length of the BitVector in bits.
   **/
  BitVector(void *memory_location, const std::size_t num_bits)
      : owned_(false),
        short_version_(enable_short_version && (num_bits < 33)),
        data_array_(static_cast<std::size_t*>(memory_location)),
        num_bits_(num_bits),
        data_array_size_((num_bits >> kHigherOrderShift) + (num_bits & kLowerOrderMask ? 1 : 0)) {
    DCHECK(data_array_ != nullptr);
  }

  /**
   * @brief Constructor for a BitVector which owns its own storage.
   *
   * @param num_bits The length of the BitVector in bits.
   **/
  explicit BitVector(const std::size_t num_bits, const bool initialize = true)
      : owned_(true),
        short_version_(enable_short_version && (num_bits < 33)),
        // NOTE(chasseur): If 'num_bits' is 0, we put 'this' in 'data_array_'
        // instead of NULL. This is because functions like memset, memcpy, and
        // memmove require a non-null parameter even when the length parameter
        // is 0 (which causes them to be no-ops).
        data_array_(reinterpret_cast<std::size_t*>(num_bits ? std::malloc(BytesNeeded(num_bits))
                                                            : this)),
        num_bits_(num_bits),
        data_array_size_((num_bits >> kHigherOrderShift) + (num_bits & kLowerOrderMask ? 1 : 0)) {
    if (initialize) {
      clear();
    }
  }

  /**
   * @brief Destructor. Frees any owned memory.
   **/
  ~BitVector() {
    if (owned_ && (num_bits_ != 0)) {
      std::free(data_array_);
    }
  }

  /**
   * @brief Calculate the number of bytes needed to store a bit vector of the
   *        given number of bits.
   *
   * @param num_bits The desired length of a BitVector in bits.
   * @return The number of bytes needed for the BitVector.
   **/
  inline static std::size_t BytesNeeded(const std::size_t num_bits) {
    if (enable_short_version) {
      if (num_bits == 0) {
        return 0;
      } else if (num_bits < 9) {
        return sizeof(std::uint8_t);
      } else if (num_bits < 17) {
        return sizeof(std::uint16_t);
      } else if (num_bits < 33) {
        return sizeof(std::uint32_t);
      }
    }

    if (num_bits & kLowerOrderMask) {
      return ((num_bits >> kHigherOrderShift) + 1) * sizeof(std::size_t);
    } else {
      return (num_bits >> kHigherOrderShift) * sizeof(std::size_t);
    }
  }

  /**
   * @brief Determine the maximum length (in bits) of a BitVector that can be
   *        stored in the given number of bytes.
   *
   * @param num_bytes The amount of storage, in bytes, that a BitVector may
   *        use.
   * @return The maximum length of a BitVector (in bits) that may be stored in
   *         the specified number of bytes.
   **/
  inline static std::size_t MaxCapacityForBytes(const std::size_t num_bytes) {
    if (num_bytes == 0) {
      return 0;
    }

    if (enable_short_version) {
      if ((num_bytes >= sizeof(std::uint8_t)) && (num_bytes < sizeof(std::uint16_t))) {
        return 8;
      } else if ((num_bytes >= sizeof(std::uint16_t)) && (num_bytes < sizeof(std::uint32_t))) {
        return 16;
      } else if ((num_bytes >= sizeof(std::uint32_t)) && (num_bytes < sizeof(std::size_t))) {
        return 32;
      }
    }

    return (num_bytes / sizeof(std::size_t)) << kHigherOrderShift;
  }

  /**
   * @brief Get the length of this BitVector in bits.
   *
   * @return The length of this BitVector in bits.
   **/
  inline std::size_t size() const {
    return num_bits_;
  }

  /**
   * @brief Clear this BitVector, setting all bits to zero.
   **/
  inline void clear() {
    std::memset(data_array_, 0, BytesNeeded(num_bits_));
  }

  /**
   * @brief Assign this BitVector's contents so that they are exactly the same
   *        as another BitVector's.
   * @warning other must be exactly the same size as this BitVector.
   *
   * @param other Another BitVector of exactly the same size to assign from.
   **/
  inline void assignFrom(const BitVector &other) {
    DCHECK_EQ(size(), other.size());
    std::memcpy(data_array_,
                other.data_array_,
                BytesNeeded(num_bits_));
  }

  /**
   * @brief Assign this BitVector's contents to the pointed-to memory.
   *
   * @warning caller is responsible for ensuring the Bitvector has the correct
   *          ownership and size.
   *
   * @param ptr Pointer to data representing a BitVector with the same parameters
   *            as this BitVector.
   **/
  inline void setMemory(void *ptr) {
    DCHECK(!owned_);
    this->data_array_ = static_cast<std::size_t*>(ptr);
  }

  /**
   * @brief Similar to assignFrom(), but the other BitVector to assign from is
   *        allowed to be longer than this one.
   * @warning Only available when enable_short_version is false.
   *
   * @param other Another BitVector to assign from. If the other BitVector is
   *        longer than this one, only bits up to the size of this BitVector
   *        will be assigned (the size will be unchanged).
   **/
  inline void assignFromLonger(const BitVector &other) {
    if (enable_short_version) {
      FATAL_ERROR("BitVector::assignFromLonger() is not available when "
                   "enable_short_version is true.");
    }
    DCHECK_LE(size(), other.size());
    std::memcpy(data_array_,
                other.data_array_,
                BytesNeeded(num_bits_));
    if (num_bits_ & kLowerOrderMask) {
      data_array_[data_array_size_ - 1]
          &= MaskBitRange<std::size_t>(0, num_bits_ & kLowerOrderMask);
    }
  }

  /**
   * @brief Get the value of a single bit.
   *
   * @param bit_num The desired bit in this BitVector.
   * @return The value of the bit at bit_num.
   **/
  inline bool getBit(const std::size_t bit_num) const {
    DCHECK_LT(bit_num, num_bits_);
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        return getBitShortVersion<std::uint8_t>(bit_num);
      } else if (num_bits_ < 17) {
        return getBitShortVersion<std::uint16_t>(bit_num);
      } else {
        return getBitShortVersion<std::uint32_t>(bit_num);
      }
    } else {
      return getBitRegularVersion(bit_num);
    }
  }

  /**
   * @brief Get the value of a word of bits.
   *
   * @param word_id The position of the word.
   * @return The value of the word at word_id.
   **/
  inline std::size_t getBitWord(const std::size_t word_id) const {
    DCHECK_LT(word_id, data_array_size_);
    if (enable_short_version && short_version_) {
      FATAL_ERROR("GetBitWord operator has not been implemented "
                  "for short version of BitVector.");
    } else {
      return data_array_[word_id];
    }
  }

  /**
   * @brief Set the value of a single bit.
   *
   * @param bit_num The desired bit in this BitVector.
   * @param value The new value to set for the bit at bit_num.
   **/
  inline void setBit(const std::size_t bit_num, bool value) {
    DCHECK_LT(bit_num, num_bits_);
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        setBitShortVersion<std::uint8_t>(bit_num, value);
      } else if (num_bits_ < 17) {
        setBitShortVersion<std::uint16_t>(bit_num, value);
      } else {
        setBitShortVersion<std::uint32_t>(bit_num, value);
      }
    } else {
      setBitRegularVersion(bit_num, value);
    }
  }

  /**
   * @brief Set the value of a single bit.
   *
   * @param bit_num The desired bit in this BitVector.
   **/
  inline void setBit(const std::size_t bit_num) {
    DCHECK(!short_version_) << "Not implemented.";
    DCHECK_LT(bit_num, num_bits_);

    setBitRegularVersion(bit_num);
  }

  /**
   * @brief Set the value of a range of bits simulaneously.
   *
   * @param start_bit_num The first bit whose value should be set.
   * @param range_num_bits The number of bits to set.
   * @param value The new value to set for the bits in the range from
   *        start_bit_num to limit_bit_num.
   **/
  inline void setBitRange(const std::size_t start_bit_num,
                          const std::size_t range_num_bits,
                          const bool value) {
    DCHECK(start_bit_num < num_bits_ || (start_bit_num == 0 && num_bits_ == 0));
    DCHECK_LE(start_bit_num + range_num_bits, num_bits_);
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        setBitRangeShortVersion<std::uint8_t>(start_bit_num, range_num_bits, value);
      } else if (num_bits_ < 17) {
        setBitRangeShortVersion<std::uint16_t>(start_bit_num, range_num_bits, value);
      } else {
        setBitRangeShortVersion<std::uint32_t>(start_bit_num, range_num_bits, value);
      }
    } else {
      setBitRangeRegularVersion(start_bit_num, range_num_bits, value);
    }
  }

  /**
   * @brief Set the value of a word of bits simultaneously.
   *
   * @param word_id The position of the word whose value should be set.
   * @param value The word value to set for the bits in the word.
   **/
  inline void setBitWord(const std::size_t word_id,
                         const std::size_t value) {
    DCHECK_LT(word_id, data_array_size_);
    if (enable_short_version && short_version_) {
      FATAL_ERROR("SetBitWord operator has not been implemented "
                  "for short version of BitVector.");
    } else {
      data_array_[word_id] = value;
      if (word_id == data_array_size_ - 1 && ((num_bits_ & kLowerOrderMask) != 0)) {
        data_array_[word_id] &= (std::numeric_limits<std::size_t>::max()
            << ((sizeof(std::size_t) << 3) - (num_bits_ & kLowerOrderMask)));
      }
    }
  }

  /**
   * @brief Flip all bits in this BitVector, setting 0 to 1 and vice-versa.
   **/
  inline void flipBits() {
    if (num_bits_ == 0) {
      return;
    }
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        FlipLimitedBits(reinterpret_cast<std::uint8_t*>(data_array_), num_bits_);
      } else if (num_bits_ < 17) {
        FlipLimitedBits(reinterpret_cast<std::uint16_t*>(data_array_), num_bits_);
      } else {
        FlipLimitedBits(reinterpret_cast<std::uint32_t*>(data_array_), num_bits_);
      }
    } else {
      for (std::size_t position = 0;
           position < data_array_size_ - 1;
           ++position) {
        data_array_[position] = ~data_array_[position];
      }

      FlipLimitedBits(data_array_ + data_array_size_ - 1, num_bits_ & kLowerOrderMask);
    }
  }

  /**
   * @brief Take the bitwise intersection of this BitVector with another one,
   *        modifying this BitVector in place.
   *
   * @param other Another BitVector to intersect with this one. Must be exactly
   *        the same length.
   **/
  inline void intersectWith(const BitVector &other) {
    DCHECK_EQ(size(), other.size());
    if (enable_short_version && short_version_) {
      if (num_bits_ == 0) {
        return;
      } else if (num_bits_ < 9) {
        *reinterpret_cast<std::uint8_t*>(data_array_)
            &= *reinterpret_cast<std::uint8_t*>(other.data_array_);
      } else if (num_bits_ < 17) {
        *reinterpret_cast<std::uint16_t*>(data_array_)
            &= *reinterpret_cast<std::uint16_t*>(other.data_array_);
      } else {
        *reinterpret_cast<std::uint32_t*>(data_array_)
            &= *reinterpret_cast<std::uint32_t*>(other.data_array_);
      }
    } else {
      for (std::size_t position = 0;
           position < data_array_size_;
           ++position) {
        data_array_[position] &= other.data_array_[position];
      }
    }
  }

  /**
   * @brief Unset any bits that are set to 1 in this vector that are also set
   *        to 1 in another BitVector. If the other BitVector is shorter than
   *        this one, any bits beyond the other BitVector's length are
   *        unchanged.
   * @warning This is only usable when enable_short_version is false.
   *
   * @param other Another BitVector whose bits shall be unset in this one.
   **/
  inline void unsetFrom(const BitVector &other) {
    if (enable_short_version) {
      FATAL_ERROR("BitVector::unsetFrom() is not available when "
                   "enable_short_version is true.");
    }
    const std::size_t limit = data_array_size_ < other.data_array_size_ ? data_array_size_
                                                                        : other.data_array_size_;
    for (std::size_t position = 0;
         position < limit;
         ++position) {
      data_array_[position] &= ~other.data_array_[position];
    }
  }

  /**
   * @brief Take the bitwise union of this BitVector with another one,
   *        modifying this BitVector in place.
   *
   * @param other Another BitVector to union with this one. Must be exactly
   *        the same length.
   **/
  inline void unionWith(const BitVector &other) {
    DCHECK_EQ(size(), other.size());
    if (enable_short_version && short_version_) {
      if (num_bits_ == 0) {
        return;
      } else if (num_bits_ < 9) {
        *reinterpret_cast<std::uint8_t*>(data_array_)
            |= *reinterpret_cast<std::uint8_t*>(other.data_array_);
      } else if (num_bits_ < 17) {
        *reinterpret_cast<std::uint16_t*>(data_array_)
            |= *reinterpret_cast<std::uint16_t*>(other.data_array_);
      } else {
        *reinterpret_cast<std::uint32_t*>(data_array_)
            |= *reinterpret_cast<std::uint32_t*>(other.data_array_);
      }
    } else {
      for (std::size_t position = 0;
           position < data_array_size_;
           ++position) {
        data_array_[position] |= other.data_array_[position];
      }
    }
  }

  /**
   * @brief Shift the tail bits in this BitVector forward, zero-filling the
   *        empty spaces.
   *
   * @param tail_start The first position to shift into (bits before this
   *                   position will be unaffected).
   * @param shift_distance The number of positions to shift bits forward.
   **/
  inline void shiftTailForward(const std::size_t tail_start,
                               const std::size_t shift_distance) {
    DCHECK_LT(tail_start, num_bits_);
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        shiftTailForwardShortVersion<std::uint8_t>(tail_start, shift_distance);
      } else if (num_bits_ < 17) {
        shiftTailForwardShortVersion<std::uint16_t>(tail_start, shift_distance);
      } else {
        shiftTailForwardShortVersion<std::uint32_t>(tail_start, shift_distance);
      }
    } else {
      shiftTailForwardRegularVersion(tail_start, shift_distance);
    }
  }

  /**
   * @brief Shift the tail bits in this BitVector backward, zero-filling the
   *        empty spaces.
   *
   * @param tail_start The first position to shift back from (bits before this
   *                   position will be unaffected).
   * @param shift_distance The number of positions to shift bits backward.
   **/
  inline void shiftTailBackward(const std::size_t tail_start,
                                const std::size_t shift_distance) {
    DCHECK_LT(tail_start, num_bits_);
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        shiftTailBackwardShortVersion<std::uint8_t>(tail_start, shift_distance);
      } else if (num_bits_ < 17) {
        shiftTailBackwardShortVersion<std::uint16_t>(tail_start, shift_distance);
      } else {
        shiftTailBackwardShortVersion<std::uint32_t>(tail_start, shift_distance);
      }
    } else {
      shiftTailBackwardRegularVersion(tail_start, shift_distance);
    }
  }

  /**
   * @brief Check if any bit is nonzero in this BitVector.
   *
   * @return True if any bit in this BitVector is set to 1, false if all bits
   *         are 0.
   **/
  inline bool any() const {
    if (enable_short_version && short_version_) {
      if (num_bits_ == 0) {
        return false;
      } else if (num_bits_ < 9) {
        return *reinterpret_cast<const std::uint8_t*>(data_array_);
      } else if (num_bits_ < 17) {
        return *reinterpret_cast<const std::uint16_t*>(data_array_);
      } else {
        return *reinterpret_cast<const std::uint32_t*>(data_array_);
      }
    } else {
      for (std::size_t array_idx = 0; array_idx < data_array_size_; ++array_idx) {
        if (data_array_[array_idx]) {
          return true;
        }
      }
      return false;
    }
  }

  /**
   * @brief Check if all bits are set to 1 in this BitVector.
   *
   * @return True if all bits in this BitVector are set to 1, false if any bit
   *         is 0.
   **/
  inline bool all() const {
    if (enable_short_version && short_version_) {
      if (num_bits_ == 0) {
        return true;
      } else if (num_bits_ < 9) {
        return *reinterpret_cast<const std::uint8_t*>(data_array_)
               == std::numeric_limits<std::uint8_t>::max() << (8 - num_bits_);
      } else if (num_bits_ < 17) {
        return *reinterpret_cast<const std::uint16_t*>(data_array_)
               == std::numeric_limits<std::uint16_t>::max() << (16 - num_bits_);
      } else {
        return *reinterpret_cast<const std::uint32_t*>(data_array_)
               == std::numeric_limits<std::uint32_t>::max() << (32 - num_bits_);
      }
    } else {
      const std::size_t tail_element_bits = num_bits_ & kLowerOrderMask;
      for (std::size_t array_idx = 0;
           array_idx < data_array_size_ - (tail_element_bits != 0);
           ++array_idx) {
        if (data_array_[array_idx] != std::numeric_limits<std::size_t>::max()) {
          return false;
        }
      }
      return (tail_element_bits == 0)
             || (data_array_[data_array_size_ - 1]
                 == std::numeric_limits<std::size_t>::max() << (kSizeTBits - tail_element_bits));
    }
  }

  /**
   * @brief Count the total number of 1-bits in this BitVector.
   *
   * @return The number of ones in this BitVector.
   **/
  inline std::size_t onesCount() const {
    if (enable_short_version && short_version_) {
      if (num_bits_ == 0) {
        return 0;
      } else if (num_bits_ < 9) {
        return population_count<std::uint8_t>(
            *reinterpret_cast<const std::uint8_t*>(data_array_));
      } else if (num_bits_ < 17) {
        return population_count<std::uint16_t>(
            *reinterpret_cast<const std::uint16_t*>(data_array_));
      } else {
        return population_count<std::uint32_t>(
            *reinterpret_cast<const std::uint32_t*>(data_array_));
      }
    } else {
      std::size_t count = 0;
      for (std::size_t position = 0;
           position < data_array_size_;
           ++position) {
        count += population_count<std::size_t>(data_array_[position]);
      }

      return count;
    }
  }

  /**
   * @brief Find the first 1-bit (at or after the specified position) in this
   *        BitVector.
   *
   * @param position The first bit to search for a one.
   * @return The position of the first one (at or after position) in this
   *         BitVector.
   **/
  inline std::size_t firstOne(std::size_t position = 0) const {
    DCHECK(position < num_bits_ || (position == 0 && num_bits_ == 0));
    if (num_bits_ == 0) {
      return 0;
    }
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        std::uint8_t value = *reinterpret_cast<const std::uint8_t*>(data_array_)
                             & (std::numeric_limits<std::uint8_t>::max() >> position);
        return value ? leading_zero_count<std::uint8_t>(value)
                     : num_bits_;
      } else if (num_bits_ < 17) {
        std::uint16_t value = *reinterpret_cast<const std::uint16_t*>(data_array_)
                              & (std::numeric_limits<std::uint16_t>::max() >> position);
        return value ? leading_zero_count<std::uint16_t>(value)
                     : num_bits_;
      } else {
        std::uint32_t value = *reinterpret_cast<const std::uint32_t*>(data_array_)
                              & (std::numeric_limits<std::uint32_t>::max() >> position);
        return value ? leading_zero_count<std::uint32_t>(value)
                     : num_bits_;
      }
    }

    if (position & kLowerOrderMask) {
      std::size_t value = data_array_[position >> kHigherOrderShift]
                          & (std::numeric_limits<std::size_t>::max() >> (position & kLowerOrderMask));
      if (value) {
        return (position & ~kLowerOrderMask) | leading_zero_count<std::size_t>(value);
      }
      position = (position & ~kLowerOrderMask) + kSizeTBits;
    }

    for (std::size_t array_idx = position >> kHigherOrderShift;
         array_idx < data_array_size_;
         ++array_idx) {
      if (data_array_[array_idx]) {
        return (array_idx << kHigherOrderShift)
               | leading_zero_count<std::size_t>(data_array_[array_idx]);
      }
    }

    return num_bits_;
  }

  /**
   * @brief Find the first 0-bit (at or after the specified position) in this
   *        BitVector.
   *
   * @param position The first bit to search for a zero.
   * @return The position of the first zero (at or after position) in this
   *         BitVector.
   **/
  inline std::size_t firstZero(std::size_t position = 0) const {
    DCHECK(position < num_bits_ || (position == 0 && num_bits_ == 0));
    if (num_bits_ == 0) {
      return 0;
    }
    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        std::uint8_t value = *reinterpret_cast<const std::uint8_t*>(data_array_);
        value |= position ? std::numeric_limits<std::uint8_t>::max() << (8 - position)
                          : 0;
        value = ~value;
        return value ? leading_zero_count<std::uint8_t>(value)
                     : num_bits_;
      } else if (num_bits_ < 17) {
        std::uint16_t value = *reinterpret_cast<const std::uint16_t*>(data_array_);
        value |= position ? std::numeric_limits<std::uint16_t>::max() << (16 - position)
                          : 0;
        value = ~value;
        return value ? leading_zero_count<std::uint16_t>(value)
                     : num_bits_;
      } else {
        std::uint32_t value = *reinterpret_cast<const std::uint32_t*>(data_array_);
        value |= position ? std::numeric_limits<std::uint32_t>::max() << (32 - position)
                          : 0;
        value = ~value;
        return value ? leading_zero_count<std::uint32_t>(value)
                     : num_bits_;
      }
    }

    if (position & kLowerOrderMask) {
      std::size_t value
          = ~(data_array_[position >> kHigherOrderShift]
              | (std::numeric_limits<std::size_t>::max() << (kSizeTBits - (position & kLowerOrderMask))));
      if (value) {
        return (position & ~kLowerOrderMask) | leading_zero_count<std::size_t>(value);
      }
      position = (position & ~kLowerOrderMask) + kSizeTBits;
    }

    for (std::size_t array_idx = position >> kHigherOrderShift;
         array_idx < data_array_size_;
         ++array_idx) {
      std::size_t inverse = ~data_array_[array_idx];
      if (inverse) {
        return (array_idx << kHigherOrderShift)
               | leading_zero_count<std::size_t>(inverse);
      }
    }

    return num_bits_;
  }

  /**
   * @brief Find the last 1-bit (strictly before the specified position) in
   *        this BitVector.
   *
   * @param position The bit position to search for a one before (using size()
   *        or SIZE_T_MAX indicates that the entire BitVector should be
   *        searched).
   * @return The position of the last one (before position) in this BitVector,
   *         or the value of size() if no one is found.
   **/
  inline std::size_t lastOne(
      std::size_t position = std::numeric_limits<std::size_t>::max()) const {
    if (position == std::numeric_limits<std::size_t>::max()) {
      position = num_bits_;
    }
    DCHECK_LE(position, num_bits_);
    if (position == 0) {
      return num_bits_;
    }

    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        std::uint8_t value = *reinterpret_cast<const std::uint8_t*>(data_array_)
                             & (std::numeric_limits<std::uint8_t>::max() << (8 - position));
        return value ? 7 - trailing_zero_count<std::uint8_t>(value)
                     : num_bits_;
      } else if (num_bits_ < 17) {
        std::uint16_t value = *reinterpret_cast<const std::uint16_t*>(data_array_)
                              & (std::numeric_limits<std::uint16_t>::max() << (16 - position));
        return value ? 15 - trailing_zero_count<std::uint16_t>(value)
                     : num_bits_;
      } else {
        std::uint32_t value = *reinterpret_cast<const std::uint32_t*>(data_array_)
                              & (std::numeric_limits<std::uint32_t>::max() << (32 - position));
        return value ? 31 - trailing_zero_count<std::uint32_t>(value)
                     : num_bits_;
      }
    }

    std::size_t array_idx = (position - 1) >> kHigherOrderShift;
    if (position & kLowerOrderMask) {
      std::size_t value = data_array_[array_idx]
                          & (std::numeric_limits<std::size_t>::max()
                             << (kSizeTBits - (position & kLowerOrderMask)));
      if (value) {
        return ((array_idx + 1) << kHigherOrderShift)
               - trailing_zero_count<std::size_t>(value)
               - 1;
      }

      if (array_idx) {
        --array_idx;
      } else {
        return num_bits_;
      }
    }

    for (; array_idx > 0; --array_idx) {
      if (data_array_[array_idx]) {
        return ((array_idx + 1) << kHigherOrderShift)
               - trailing_zero_count<std::size_t>(data_array_[array_idx])
               - 1;
      }
    }
    // Check the first element:
    if (data_array_[0]) {
      return (1 << kHigherOrderShift)
             - trailing_zero_count<std::size_t>(data_array_[0])
             - 1;
    }

    return num_bits_;
  }

  /**
   * @brief Find the last 0-bit (strictly before the specified position) in
   *        this BitVector.
   *
   * @param position The bit position to search for a zero before (using size()
   *        or SIZE_T_MAX indicates that the entire BitVector should be
   *        searched).
   * @return The position of the last zero (before position) in this BitVector,
   *         or the value of size() if no zero is found.
   **/
  inline std::size_t lastZero(
      std::size_t position = std::numeric_limits<std::size_t>::max()) const {
    if (position == std::numeric_limits<std::size_t>::max()) {
      position = num_bits_;
    }
    DCHECK_LE(position, num_bits_);
    if (position == 0) {
      return num_bits_;
    }

    if (enable_short_version && short_version_) {
      if (num_bits_ < 9) {
        std::uint8_t value = ~(*reinterpret_cast<const std::uint8_t*>(data_array_)
                               | ~(std::numeric_limits<std::uint8_t>::max() << (8 - position)));
        return value ? 7 - trailing_zero_count<std::uint8_t>(value)
                     : num_bits_;
      } else if (num_bits_ < 17) {
        std::uint16_t value = ~(*reinterpret_cast<const std::uint16_t*>(data_array_)
                                | ~(std::numeric_limits<std::uint16_t>::max() << (16 - position)));
        return value ? 15 - trailing_zero_count<std::uint16_t>(value)
                     : num_bits_;
      } else {
        std::uint32_t value = ~(*reinterpret_cast<const std::uint32_t*>(data_array_)
                                | ~(std::numeric_limits<std::uint32_t>::max() << (32 - position)));
        return value ? 31 - trailing_zero_count<std::uint32_t>(value)
                     : num_bits_;
      }
    }

    std::size_t array_idx = (position - 1) >> kHigherOrderShift;
    if (position & kLowerOrderMask) {
      std::size_t value
          = ~(data_array_[array_idx]
              | ~(std::numeric_limits<std::size_t>::max() << (kSizeTBits - (position & kLowerOrderMask))));

      if (value) {
        return ((array_idx + 1) << kHigherOrderShift)
               - trailing_zero_count<std::size_t>(value)
               - 1;
      }

      if (array_idx) {
        --array_idx;
      } else {
        return num_bits_;
      }
    }

    std::size_t data_element_inverse;
    for (; array_idx > 0; --array_idx) {
      data_element_inverse = ~data_array_[array_idx];
      if (data_element_inverse) {
        return ((array_idx + 1) << kHigherOrderShift)
               - trailing_zero_count<std::size_t>(data_element_inverse)
               - 1;
      }
    }
    // Check the first element:
    data_element_inverse = ~data_array_[0];
    if (data_element_inverse) {
      return (1 << kHigherOrderShift)
             - trailing_zero_count<std::size_t>(data_element_inverse)
             - 1;
    }

    return num_bits_;
  }

  inline void unionWith(const void *other) {
    DCHECK(!enable_short_version);
    const std::size_t *other_data_array = static_cast<const std::size_t*>(other);
    for (std::size_t array_idx = 0; array_idx < data_array_size_; ++array_idx) {
      data_array_[array_idx] |= other_data_array[array_idx];
    }
  }

  inline void subtractTo(const BitVector &other, std::vector<int> *output) {
    DCHECK(!enable_short_version);
    for (std::size_t array_idx = 0; array_idx < data_array_size_; ++array_idx) {
      const std::size_t base = array_idx << kHigherOrderShift;
      std::size_t value = data_array_[array_idx] & ~other.data_array_[array_idx];
      while (value != 0) {
        const std::size_t offset = leading_zero_count<std::size_t>(value);
        value ^= TopBit<std::size_t>() >> offset;
        output->emplace_back(base + offset);
      }
    }
  }

 private:
  // This works as long as the bit-width of size_t is power of 2:
  static const std::size_t kLowerOrderMask = (sizeof(std::size_t) << 3) - 1;
  // This works for 32-bit or 64-bit size_t:
  static const std::size_t kHigherOrderShift = sizeof(std::size_t) == 4 ? 5 : 6;
  static const std::size_t kSizeTBits = sizeof(std::size_t) << 3;

  template <typename WordType>
  static constexpr WordType TopBit() {
    return static_cast<WordType>(0x1) << ((sizeof(WordType) << 3) - 1);
  }

  // Flip 'num_bits' lower-order bits in '*item', setting higher-order bits to
  // zero.
  template <typename VectorType>
  inline static void FlipLimitedBits(VectorType *item, const std::size_t num_bits) {
    *item = (~*item);
    if (num_bits != 0) {
      *item &= (std::numeric_limits<VectorType>::max() << ((sizeof(VectorType) << 3) - num_bits));
    }
  }

  // Generate a mask with ones from 'start_bit_num' up to 'range_num_bits'.
  template <typename VectorType>
  inline static VectorType MaskBitRange(const std::size_t start_bit_num,
                                        std::size_t range_num_bits) {
    range_num_bits = (range_num_bits > sizeof(VectorType) << 3) ? sizeof(VectorType) << 3
                                                                : range_num_bits;
    return range_num_bits
               ? static_cast<VectorType>(
                     std::numeric_limits<VectorType>::max() << ((sizeof(VectorType) << 3) - range_num_bits))
                         >> start_bit_num
               : 0;
  }

  inline bool getBitRegularVersion(const std::size_t bit_num) const {
    return (data_array_[bit_num >> kHigherOrderShift] << (bit_num & kLowerOrderMask))
           & TopBit<std::size_t>();
  }

  template <typename VectorType>
  inline bool getBitShortVersion(const std::size_t bit_num) const {
    return (*reinterpret_cast<const VectorType*>(data_array_) << bit_num)
           & TopBit<VectorType>();
  }

  inline void setBitRegularVersion(const std::size_t bit_num, bool value) {
    // The mask has all the bits set to 1, except the bit at the required bit position
    // in the word that has be to set/unset based on the boolean value.
    std::size_t mask = ~(TopBit<std::size_t>() >> (bit_num & kLowerOrderMask));

    // The bit_value type casts the value argument (true or false) to either (1 or 0).
    std::size_t bit_value = value;

    // Subtracting 1 from the bit_value, will generate a value
    // that will either be all zeros (if the required bit_value is true),
    // or all ones (if the required bit_value was false). The trick here is that
    // subtracting one from an unsigned zero value will set all the bits to 1.
    // Finally, bitwise OR with the mask followed by negation will generate an op_value
    // that will have all bits as 0's if the required bit_value was false, or
    // will have only 1 at the required bit position if the bit_value was true.
    std::size_t op_value = ~((bit_value - 1) | mask);

    // Get the index position in the data_array_ that is being operated upon.
    std::size_t index_pos_in_data_array = bit_num >> kHigherOrderShift;

    // First, by doing a bitwise AND with the mask, we clear the required bit position.
    data_array_[index_pos_in_data_array] &= mask;

    // Then, we set the required bit position with the op_value the we generated above.
    // Note that bitwise OR with op_value will set the required bit position to 1 if the
    // value was indeed true, but it will be a no-op if the value was false.
    data_array_[index_pos_in_data_array] |= op_value;
  }

  inline void setBitRegularVersion(const std::size_t bit_num) {
    data_array_[bit_num >> kHigherOrderShift] |= (TopBit<std::size_t>() >> (bit_num & kLowerOrderMask));
  }

  template <typename VectorType>
  inline void setBitShortVersion(const std::size_t bit_num, bool value) {
    if (value) {
      *reinterpret_cast<VectorType*>(data_array_)
          |= (TopBit<VectorType>() >> bit_num);
    } else {
      *reinterpret_cast<VectorType*>(data_array_)
          &= ~(TopBit<VectorType>() >> bit_num);
    }
  }

  inline void setBitRangeRegularVersion(const std::size_t start_bit_num,
                                        std::size_t range_num_bits,
                                        const bool value) {
    std::size_t start_element = start_bit_num >> kHigherOrderShift;
    const std::size_t start_lower_bits = start_bit_num & kLowerOrderMask;
    if (value) {
      // Set bits part-way into an array element.
      if (start_lower_bits) {
        data_array_[start_element] |= MaskBitRange<std::size_t>(start_lower_bits,
                                                                range_num_bits);
        range_num_bits += start_lower_bits;
        if (range_num_bits <= kSizeTBits) {
          // If we are already done, return now.
          return;
        }
        ++start_element;
        range_num_bits -= kSizeTBits;
      }

      // Use memset to set all the whole elements covered in one go.
      const std::size_t range_elements = range_num_bits >> kHigherOrderShift;
      std::memset(data_array_ + start_element,
                  0xFF,
                  range_elements * sizeof(std::size_t));

      // Set any remaining bits in the first part of the last element.
      data_array_[start_element + range_elements]
          |= MaskBitRange<std::size_t>(0, range_num_bits & kLowerOrderMask);
    } else {
      // Same as above, but set bits to zero.
      if (start_lower_bits) {
        data_array_[start_element] &= ~MaskBitRange<std::size_t>(start_lower_bits,
                                                                 range_num_bits);
        range_num_bits += start_lower_bits;
        if (range_num_bits <= kSizeTBits) {
          return;
        }
        ++start_element;
        range_num_bits -= kSizeTBits;
      }

      const std::size_t range_elements = range_num_bits >> kHigherOrderShift;
      std::memset(data_array_ + start_element,
                  0x0,
                  range_elements * sizeof(std::size_t));

      data_array_[start_element + range_elements]
          &= ~MaskBitRange<std::size_t>(0, range_num_bits & kLowerOrderMask);
    }
  }

  template <typename VectorType>
  inline void setBitRangeShortVersion(const std::size_t start_bit_num,
                                      const std::size_t range_num_bits,
                                      const bool value) {
    if (value) {
      *reinterpret_cast<VectorType*>(data_array_)
          |= MaskBitRange<VectorType>(start_bit_num, range_num_bits);
    } else {
      *reinterpret_cast<VectorType*>(data_array_)
          &= ~MaskBitRange<VectorType>(start_bit_num, range_num_bits);
    }
  }

  inline void shiftTailForwardRegularVersion(const std::size_t tail_start,
                                             const std::size_t shift_distance) {
    if (shift_distance == 0) {
      return;
    }

    std::size_t dest_element = tail_start >> kHigherOrderShift;
    std::size_t dest_lower_bits = tail_start & kLowerOrderMask;
    std::size_t src_element = (tail_start + shift_distance) >> kHigherOrderShift;
    std::size_t src_lower_bits = (tail_start + shift_distance) & kLowerOrderMask;

    if (src_element >= data_array_size_) {
      // Shift is so large that we are really just zeroing out the entire tail.
      if (dest_lower_bits) {
        data_array_[dest_element] &= MaskBitRange<std::size_t>(0, dest_lower_bits);
        ++dest_element;
      }
      std::memset(data_array_ + dest_element,
                  0x0,
                  (data_array_size_ - dest_element) * sizeof(std::size_t));
      return;
    }

    if (dest_lower_bits) {
      // Fill in the rest of the first word with the appropriate bits.

      data_array_[dest_element]
            // Mask out lower bits.
          = (data_array_[dest_element] & MaskBitRange<std::size_t>(0, dest_lower_bits))
            // OR in the tail bits from the source element, shifted appropriately.
            | ((data_array_[src_element] << (src_lower_bits)) >> dest_lower_bits);

      if (dest_lower_bits < src_lower_bits) {
        // Need to get some bits from the next word.
        dest_lower_bits += kSizeTBits - src_lower_bits;
        ++src_element;

        if (src_element == data_array_size_) {
          ++dest_element;
          std::memset(data_array_ + dest_element,
                      0x0,
                      (data_array_size_ - dest_element) * sizeof(std::size_t));
          return;
        }

        data_array_[dest_element] |= data_array_[src_element] >> dest_lower_bits;
        src_lower_bits = kSizeTBits - dest_lower_bits;
      } else {
        src_lower_bits += kSizeTBits - dest_lower_bits;
        // If dest_lower_bits == src_lower_bits, then alignment is just right
        // and we roll over to the next word.
        src_element += src_lower_bits >> kHigherOrderShift;
        src_lower_bits &= kLowerOrderMask;
      }

      ++dest_element;
    }

    if (src_lower_bits) {
      while (src_element < data_array_size_ - 1) {
        data_array_[dest_element] = (data_array_[src_element] << src_lower_bits)
                                    | (data_array_[src_element + 1] >> (kSizeTBits - src_lower_bits));
        ++dest_element;
        ++src_element;
      }
      data_array_[dest_element] = data_array_[src_element] << src_lower_bits;
    } else {
      // If the shift is an exact multiple of sizeof(std::size_t), we can just
      // do a memmove.
      std::memmove(data_array_ + dest_element,
                   data_array_ + src_element,
                   (data_array_size_ - src_element) * sizeof(std::size_t));
      dest_element += data_array_size_ - src_element - 1;
    }

    ++dest_element;
    std::memset(data_array_ + dest_element,
                0x0,
                (data_array_size_ - dest_element) * sizeof(std::size_t));
  }

  inline void shiftTailBackwardRegularVersion(const std::size_t tail_start,
                                              const std::size_t shift_distance) {
    if (shift_distance == 0) {
      return;
    }

    const std::size_t tail_start_element = tail_start >> kHigherOrderShift;
    const std::size_t tail_start_lower_bits = tail_start & kLowerOrderMask;

    if (tail_start + shift_distance > num_bits_) {
      // Simply zero-out the tail.
      const std::size_t full_words = data_array_size_
                                     - tail_start_element
                                     - (tail_start_lower_bits ? 1 : 0);
      if (full_words) {
        std::memset(data_array_ + tail_start_element + (tail_start_lower_bits ? 1 : 0),
                    0x0,
                    full_words * sizeof(size_t));
      }
      if (tail_start_lower_bits) {
        data_array_[tail_start_element] &= ~MaskBitRange<std::size_t>(tail_start_lower_bits,
                                                                      kSizeTBits);
      }
      return;
    }

    const std::size_t shift_elements = shift_distance >> kHigherOrderShift;
    const std::size_t shift_lower_bits = shift_distance & kLowerOrderMask;
    const std::size_t tail_bits_from_tail_start_element
        = data_array_[tail_start_element] << tail_start_lower_bits >> tail_start_lower_bits;

    if (shift_lower_bits) {
      // Construct shifted elements back-to-front.
      for (std::size_t element = data_array_size_ - 1;
           element > tail_start_element + shift_elements + 1;
           --element) {
        data_array_[element] = (data_array_[element - shift_elements] >> shift_lower_bits)
                               | (data_array_[element - shift_elements - 1]
                                  << (kSizeTBits - shift_lower_bits));
      }

      if (tail_start_element + shift_elements + 1 < data_array_size_) {
        data_array_[tail_start_element + shift_elements + 1]
            = (data_array_[tail_start_element + 1] >> shift_lower_bits)
              | (tail_bits_from_tail_start_element << (kSizeTBits - shift_lower_bits));
      }

      if (shift_elements) {
        // Shifting by more than one word.
        data_array_[tail_start_element + shift_elements]
            = tail_bits_from_tail_start_element >> shift_lower_bits;

        if (tail_start_lower_bits) {
          data_array_[tail_start_element] &= ~MaskBitRange<std::size_t>(tail_start_lower_bits,
                                                                        kSizeTBits);
        }

        std::memset(data_array_ + tail_start_element + 1,
                    0x0,
                    (shift_elements - 1) * sizeof(std::size_t));
      } else {
        // Shifting by less than one word.
        if (tail_start_lower_bits) {
          data_array_[tail_start_element]
              = (data_array_[tail_start_element]
                     & MaskBitRange<std::size_t>(0, tail_start_lower_bits))
                | ((data_array_[tail_start_element]
                        & MaskBitRange<std::size_t>(tail_start_lower_bits, kSizeTBits))
                   >> shift_lower_bits);
        } else {
          data_array_[tail_start_element] >>= shift_lower_bits;
        }
      }
    } else {
      // Shift is an exact multiple of size_t, so we can just use memmove.
      std::memmove(data_array_ + tail_start_element + shift_elements,
                   data_array_ + tail_start_element,
                   (data_array_size_ - tail_start_element - shift_elements) * sizeof(std::size_t));
      if (tail_start_lower_bits) {
        data_array_[tail_start_element]
            &= MaskBitRange<std::size_t>(0, tail_start_lower_bits);
        data_array_[tail_start_element + shift_elements]
            &= MaskBitRange<std::size_t>(tail_start_lower_bits, kSizeTBits);

        std::memset(data_array_ + tail_start_element + 1,
                    0x0,
                    (shift_elements - 1) * sizeof(std::size_t));
      } else {
        std::memset(data_array_ + tail_start_element,
                    0x0,
                    shift_elements * sizeof(std::size_t));
      }
    }

    // Zero out extra bits in the last element.
    if (num_bits_ & kLowerOrderMask) {
      data_array_[data_array_size_ - 1]
          &= MaskBitRange<std::size_t>(0, num_bits_ & kLowerOrderMask);
    }
  }

  template <typename VectorType>
  inline void shiftTailForwardShortVersion(const std::size_t tail_start,
                                           const std::size_t shift_distance) {
    const VectorType head = *reinterpret_cast<VectorType*>(data_array_)
                            & MaskBitRange<VectorType>(0, tail_start);
    const VectorType tail = *reinterpret_cast<VectorType*>(data_array_)
                            & MaskBitRange<VectorType>(tail_start + shift_distance,
                                                       sizeof(VectorType) << 3);
    *reinterpret_cast<VectorType*>(data_array_) = head | (tail << shift_distance);
  }

  template <typename VectorType>
  inline void shiftTailBackwardShortVersion(const std::size_t tail_start,
                                            const std::size_t shift_distance) {
    const VectorType head = *reinterpret_cast<VectorType*>(data_array_)
                            & MaskBitRange<VectorType>(0, tail_start);
    const VectorType tail = *reinterpret_cast<VectorType*>(data_array_)
                            & MaskBitRange<VectorType>(tail_start, sizeof(VectorType) << 3);
    *reinterpret_cast<VectorType*>(data_array_) = head | (tail >> shift_distance);
  }

  const bool owned_;
  const bool short_version_;  // Vector is 32 bits or less, so we use not even a full size_t.
  std::size_t *data_array_;
  const std::size_t num_bits_;
  const std::size_t data_array_size_;

  DISALLOW_COPY_AND_ASSIGN(BitVector);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BIT_VECTOR_HPP_
