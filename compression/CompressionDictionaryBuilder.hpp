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

#ifndef QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_BUILDER_HPP_
#define QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_BUILDER_HPP_

#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Compression
 *  @{
 */

/**
 * @brief An object which accumulates typed values and builds a physical
 *        dictionary for a CompressionDictionary object. Usable with both
 *        fixed-length and variable-length types.
 **/
class CompressionDictionaryBuilder {
 public:
  /**
   * @brief Constructor.
   *
   * @param type The Type to build a CompressionDictionary for. Must be
   *        orderable by LessComparison.
   **/
  explicit CompressionDictionaryBuilder(const Type &type);

  /**
   * @brief Destructor.
   **/
  ~CompressionDictionaryBuilder() {
  }

  /**
   * @brief Get the number of entries (unique values/codes) in the dictionary
   *        being built.
   *
   * @return The number of entries in the dictionary.
   **/
  inline std::uint32_t numberOfEntries() const {
    return num_values_;
  }

  /**
   * @brief Get the number of bits needed to represent a code in the dictionary
   *        being built.
   *
   * @return The length, in bits, of codes for the dictionary.
   **/
  inline std::uint8_t codeLengthBits() const {
    return code_length_bits_;
  }

  /**
   * @brief Get the number of bytes used to represent a code in the dictionary
   *        being build when all codes are padded up to the next power-of-two
   *        number of bytes.
   *
   * @return The length, in bytes, of codes padded up to a power-of-two bytes.
   **/
  inline std::uint8_t codeLengthPaddedBytes() const {
    if (code_length_bits_ < 9) {
      return 1;
    } else if (code_length_bits_ < 17) {
      return 2;
    } else {
      return 4;
    }
  }

  /**
   * @brief Get the number of bytes needed to store the physical dictionary
   *        being built.
   *
   * @return The size, in bytes, of the dictionary.
   **/
  inline std::size_t dictionarySizeBytes() const {
    if (fixed_type_length_) {
      return 2 * sizeof(std::uint32_t) + total_value_size_;
    } else {
      return (num_values_ + 2 - null_value_present_) * sizeof(std::uint32_t)
             + total_value_size_;
    }
  }

  /**
   * @brief Determine if the dictionary being built contains a particular
   *        value.
   **/
  bool containsValue(const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    if (value.isNull()) {
      return null_value_present_;
    }
    return value_set_.find(value) != value_set_.end();
  }

  /**
   * @brief Lookup the code for a particular value in a dictionary that has
   *        been built by this CompressionDictionaryBuilder.
   * @note This method uses a hash-table lookup for O(1) constant time
   *       performance in the general case, in contrast to the O(log(n))
   *       lookup performance of CompressionDictionary::getCodeForTypedValue().
   * @warning Only call this AFTER buildDictionary().
   *
   * @param value A previously-inserted value to look up.
   * @return The code that maps to value.
   **/
  inline std::uint32_t getCodeForValue(const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    DCHECK(containsValue(value));
    DCHECK(built_);
    if (value.isNull()) {
      return num_values_ - 1;
    }
    return value_set_.find(value)->second;
  }

  /**
   * @brief Construct a physical dictionary in the specified memory location.
   *
   * @param location The memory location where the physical dictionary should
   *        be built. Must have dictionarySizeBytes() available to write at
   *        location.
   **/
  void buildDictionary(void *location);

  /**
   * @brief Add a value to the dictionary being built.
   * @note This method makes a copy of the value passed in. If the caller can
   *       guarantee that value remains in existence for the life of this
   *       CompressionDictionaryBuilder, it is more memory-efficient to use
   *       insertEntryByReference() instead.
   *
   * @param value A typed value to add to the dictionary.
   * @return True if value has been added, false if it was already present and
   *         the dictionary was not modified.
   **/
  bool insertEntry(const TypedValue &value) {
    DCHECK(!built_);
    // This is used in buildDictionary() to avoid an intermediate copy of
    // out-of-line data.
    may_contain_out_of_line_literal_ = true;
    return insertEntryInternal(value, false);
  }

  /**
   * @brief Add a value to the dictionary being built without copying it.
   * @warning The caller must ensure that value is not deleted until after
   *          done using this CompressionDictionaryBuilder.
   *
   * @param value A typed value to add to the dictionary.
   * @return True if value has been added, false if it was already present and
   *         the dictionary was not modified.
   **/
  bool insertEntryByReference(const TypedValue &value) {
    DCHECK(!built_);
    return insertEntryInternal(value, true);
  }

  /**
   * @brief Remove the last entry successfully added to the dictionary via
   *        insertEntry(), reducing the dictionary size and potentially
   *        reducing the code length in bits.
   **/
  void undoLastInsert();

 private:
  struct TypedValueHasher {
    inline std::size_t operator()(const TypedValue &value) const {
      return value.getHash();
    }
  };

  struct TypedValueEqualChecker {
    inline bool operator()(const TypedValue &left, const TypedValue &right) const {
      return left.fastEqualCheck(right);
    }
  };

  bool insertEntryInternal(const TypedValue &value, bool by_reference);

  const Type &type_;
  const bool type_is_nullable_;
  std::size_t fixed_type_length_;  // 0 for variable-length types.

  // Initially, the uint32_t value associated with each key is simply the max
  // for uint32_t. buildDictionary() updates values to be the actual compressed
  // codes for each value so that getCodeForValue() can do fast lookups.
  std::unordered_map<TypedValue,
                     std::uint32_t,
                     TypedValueHasher,
                     TypedValueEqualChecker>
      value_set_;
  std::unordered_map<TypedValue,
                     std::uint32_t,
                     TypedValueHasher,
                     TypedValueEqualChecker>::iterator
      last_inserted_iterator_;

  std::uint32_t num_values_;
  std::uint8_t code_length_bits_;
  std::size_t total_value_size_;
  bool null_value_present_;
  bool last_insert_was_null_;
  bool may_contain_out_of_line_literal_;

  // Keeps track of whether buildDictionary() has been called for
  // error-checking purposes.
  bool built_;

  DISALLOW_COPY_AND_ASSIGN(CompressionDictionaryBuilder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_BUILDER_HPP_
