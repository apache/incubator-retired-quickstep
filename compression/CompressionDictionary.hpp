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

#ifndef QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_HPP_
#define QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_HPP_

#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>

#include "compression/CompressionDictionaryLite.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

namespace compression_dictionary_internal {
template <bool variable_length_type> class CompressionDictionaryIterator;
}

/** \addtogroup Compression
 *  @{
 */

// In-memory format of a CompressionDictionary is as follows:
//     std::uint32_t num_codes - number of codes/values stored in dictionary
//                               (excluding NULL)
//     std::uint32_t null_code - a code representing a NULL value (equal to
//                               num_codes if NULL is present, or the maximum
//                               possible value of std::uint32_t if NULL is not
//                               present)
// For fixed-length types:
//     array of num_codes values
// For variable-length types:
//     array of num_codes offsets of variable-length values
//     variable-length storage region, with values stored at offsets specified
//         by previous array.

/**
 * @brief Extends the basic interface provided by CompressionDictionaryLite
 *        with methods for inverse lookup.
 **/
class CompressionDictionary : public CompressionDictionaryLite {
 public:
  /**
   * @brief Constructor.
   *
   * @param type The type of values being compressed. LessComparison must be
   *        applicable to this Type.
   * @param dictionary_memory The memory location of the physical dictionary.
   * @param dictionary_memory_size The size (in bytes) of the physical
   *        dictionary at dictionary_memory.
   **/
  CompressionDictionary(const Type &type,
                        const void *dictionary_memory,
                        const std::size_t dictionary_memory_size)
      : CompressionDictionaryLite(type, dictionary_memory, dictionary_memory_size) {
  }

  virtual ~CompressionDictionary() {
  }

  /**
   * @brief Get the compressed code that represents the specified untyped
   *        value.
   * @note This uses a binary search to find the appropriate code. It runs in
   *       O(log(n)) time.
   *
   * @param value An untyped pointer to a value, which must be of the exact
   *        same Type as the Type used to construct this dictionary.
   * @return The code for value in this dictionary, or the value of
   *         numberOfCodes() (the maximum code plus one) if value is not
   *         contained in this dictionary.
   **/
  std::uint32_t getCodeForUntypedValue(const void *value) const;

  /**
   * @brief Get the compressed code that represents the specified typed value.
   * @note This uses a binary search to find the appropriate code. It runs in
   *       O(log(n)) time.
   *
   * @param value A typed value, which can be either the exact same Type as
   *        the values in this dictionary, or another Type which is comparable
   *        according to LessComparison.
   * @param value_type The Type that value belongs to.
   * @return The code for value in this dictionary, or the value of
   *         numberOfCodes() (the maximum code plus one) if value is not
   *         contained in this dictionary.
   **/
  std::uint32_t getCodeForTypedValue(const TypedValue &value,
                                     const Type &value_type) const {
    if (value.isNull()) {
      return getNullCode() == std::numeric_limits<std::uint32_t>::max() ? number_of_codes_including_null_
                                                                        : getNullCode();
    } else if (value_type.isSubsumedBy(type_)) {
      return getCodeForUntypedValue(value.getDataPtr());
    } else {
      return getCodeForDifferentTypedValue(value, value_type);
    }
  }

  /**
   * @brief Find the first code which is not less than the specified untyped
   *        value, similar to std::lower_bound().
   * @warning value must not be NULL.
   *
   * @param value An untyped pointer to a value, which must be of the exact
   *        same Type as the Type used to construct this dictionary.
   * @return The first code whose corresponding uncompressed value is not less
   *         than value. May return numberOfCodes() if every value in the
   *         dictionary is less than value.
   **/
  std::uint32_t getLowerBoundCodeForUntypedValue(const void *value,
                                                 const bool ignore_null_code = false) const;

  /**
   * @brief Find the first code which is not less than the specified typed
   *        value, similar to std::lower_bound().
   * @warning value must not be NULL.
   *
   * @param value A typed value, which can be either the exact same Type as
   *        the values in this dictionary, or another Type which is comparable
   *        according to LessComparison.
   * @param value_type The Type that value belongs to.
   * @return The first code whose corresponding uncompressed value is not less
   *         than value. May return numberOfCodes() if every value in the
   *         dictionary is less than value.
   **/
  std::uint32_t getLowerBoundCodeForTypedValue(const TypedValue &value,
                                               const Type &value_type,
                                               const bool ignore_null_code = false) const {
    DCHECK(!value.isNull());
    if (value_type.isSubsumedBy(type_)) {
      return getLowerBoundCodeForUntypedValue(value.getDataPtr(), ignore_null_code);
    } else {
      return getLowerBoundCodeForDifferentTypedValue(value, value_type, ignore_null_code);
    }
  }

  /**
   * @brief Find the first code which is greater than the specified untyped
   *        value, similar to std::upper_bound().
   * @warning value must not be NULL.
   *
   * @param value An untyped pointer to a value, which must be of the exact
   *        same Type as the Type used to construct this dictionary.
   * @return The first code whose corresponding uncompressed value is greater
   *         than value. May return numberOfCodes() if every value in the
   *         dictionary is less than or equal to value.
   **/
  std::uint32_t getUpperBoundCodeForUntypedValue(const void *value) const;

  /**
   * @brief Find the first code which is greater than the specified typed
   *        value, similar to std::upper_bound().
   * @warning value must not be NULL.
   *
   * @param value A typed value, which can be either the exact same Type as
   *        the values in this dictionary, or another Type which is comparable
   *        according to LessComparison.
   * @param value_type The Type that value belongs to.
   * @return The first code whose corresponding uncompressed value is greater
   *         than value. May return numberOfCodes() if every value in the
   *         dictionary is less than or equal to value.
   **/
  std::uint32_t getUpperBoundCodeForTypedValue(const TypedValue &value,
                                               const Type &value_type) const {
    DCHECK(!value.isNull());
    if (value_type.isSubsumedBy(type_)) {
      return getUpperBoundCodeForUntypedValue(value.getDataPtr());
    } else {
      return getUpperBoundCodeForDifferentTypedValue(value, value_type);
    }
  }

  /**
   * @brief Determine the range of codes that match a specified comparison with
   *        a specified untyped value.
   *
   * @param comp The comparison to evaluate.
   * @param value An untyped pointer to a value, which must be of the exact
   *        same Type as the Type used to construct this dictionary.
   * @return The limits of the range of codes which match the predicate
   *         "coded-value comp value". The range is [first, second) (i.e. it
   *         is inclusive of first but not second).
   **/
  std::pair<std::uint32_t, std::uint32_t> getLimitCodesForComparisonUntyped(
      const ComparisonID comp,
      const void *value) const;

  /**
   * @brief Determine the range of codes that match a specified comparison with
   *        a specified typed value.
   *
   * @param comp The comparison to evaluate.
   * @param value A typed value, which can be either the exact same Type as
   *        the values in this dictionary, or another Type which is comparable
   *        according to LessComparison.
   * @param value_type The Type that value belongs to.
   * @return The limits of the range of codes which match the predicate
   *         "coded-value comp value". The range is [first, second) (i.e. it
   *         is inclusive of first but not second).
   **/
  std::pair<std::uint32_t, std::uint32_t> getLimitCodesForComparisonTyped(
      const ComparisonID comp,
      const TypedValue &value,
      const Type &value_type) const {
    if (value_type.isSubsumedBy(type_)) {
      return getLimitCodesForComparisonUntyped(comp,
                                               value.isNull() ? nullptr : value.getDataPtr());
    } else {
      return getLimitCodesForComparisonDifferentTyped(comp, value, value_type);
    }
  }

 private:
  friend class compression_dictionary_internal::CompressionDictionaryIterator<false>;
  friend class compression_dictionary_internal::CompressionDictionaryIterator<true>;

  std::uint32_t getCodeForDifferentTypedValue(const TypedValue &value,
                                              const Type &value_type) const;
  std::uint32_t getLowerBoundCodeForDifferentTypedValue(const TypedValue &value,
                                                        const Type &value_type,
                                                        const bool ignore_null_code = false) const;
  std::uint32_t getUpperBoundCodeForDifferentTypedValue(const TypedValue &value,
                                                        const Type &value_type) const;
  std::pair<std::uint32_t, std::uint32_t> getLimitCodesForComparisonDifferentTyped(
      const ComparisonID comp,
      const TypedValue &value,
      const Type &value_type) const;

  DISALLOW_COPY_AND_ASSIGN(CompressionDictionary);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_HPP_
