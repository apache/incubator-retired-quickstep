/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_LITE_HPP_
#define QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_LITE_HPP_

#include <cstddef>
#include <cstdint>
#include <limits>

#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

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
 * @brief A dictionary which maps short integer codes to typed values.
 * @note Codes in a CompressionDictionaryLite compare in the same order as the
 *       underlying values.
 **/
class CompressionDictionaryLite {
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
  CompressionDictionaryLite(const Type &type,
                            const void *dictionary_memory,
                            const std::size_t dictionary_memory_size);

  virtual ~CompressionDictionaryLite() = 0;

  /**
   * @brief Get the number of code/value mappings in this dictionary.
   *
   * @return The number of codes/values in this dictionary.
   **/
  inline std::uint32_t numberOfCodes() const {
    return number_of_codes_including_null_;
  }

  /**
   * @brief Get the minimum number of bits needed to represent a code for this
   *        dictionary.
   *
   * @return The length of codes for this dictionary in bits.
   **/
  inline std::uint8_t codeLengthBits() const {
    return code_length_bits_;
  }

  /**
   * @brief Determine whether this dictionary contains a code for NULL values.
   *
   * @return Whether this dictionary contains NULL.
   **/
  inline bool containsNull() const {
    return getNullCode() != std::numeric_limits<std::uint32_t>::max();
  }

  /**
   * @brief Get the code representing NULL in this dictionary, or the maximum
   *        possible value of a std::uint32_t if NULL is not present in this
   *        dictionary.
   *
   * @return The code representing NULL in this dictionary, or UINT32_MAX if
   *         NULL is not present in this dictionary.
   **/
  inline std::uint32_t getNullCode() const {
    return *(static_cast<const std::uint32_t*>(dictionary_memory_) + 1);
  }

  /**
   * @brief Get an untyped pointer to the value represented by the specified
   *        code.
   * @note This version is for codes of 8 bits or less. Also see
   *       getUntypedValueForShortCode() and getUntypedValueForCode().
   * @warning It is an error to use this method with a code which does not
   *          exist in this dictionary, i.e. code must be less than
   *          numberOfCodes().
   *
   * @param code The compressed code to get the value for.
   * @return An untyped pointer to the value that corresponds to code.
   **/
  template <bool check_null = true>
  inline const void* getUntypedValueForByteCode(const std::uint8_t code) const {
    if (type_is_variable_length_) {
      return variableLengthGetUntypedValueHelper<std::uint8_t, check_null>(code);
    } else {
      return fixedLengthGetUntypedValueHelper<std::uint8_t, check_null>(code);
    }
  }

  /**
   * @brief Get an untyped pointer to the value represented by the specified
   *        code.
   * @note This version is for codes of 16 bits or less. Also see
   *       getUntypedValueForByteCode() and getUntypedValueForCode().
   * @warning It is an error to use this method with a code which does not
   *          exist in this dictionary, i.e. code must be less than
   *          numberOfCodes().
   *
   * @param code The compressed code to get the value for.
   * @return An untyped pointer to the value that corresponds to code.
   **/
  template <bool check_null = true>
  inline const void* getUntypedValueForShortCode(const std::uint16_t code) const {
    if (type_is_variable_length_) {
      return variableLengthGetUntypedValueHelper<std::uint16_t, check_null>(code);
    } else {
      return fixedLengthGetUntypedValueHelper<std::uint16_t, check_null>(code);
    }
  }

  /**
   * @brief Get an untyped pointer to the value represented by the specified
   *        code.
   * @note This version is for any code up to the maximum length of 32 bits.
   *       Also see getUntypedValueForByteCode() and
   *       getUntypedValueForShortCode().
   * @warning It is an error to use this method with a code which does not
   *          exist in this dictionary, i.e. code must be less than
   *          numberOfCodes().
   *
   * @param code The compressed code to get the value for.
   * @return An untyped pointer to the value that corresponds to code.
   **/
  template <bool check_null = true>
  inline const void* getUntypedValueForCode(const std::uint32_t code) const {
    if (type_is_variable_length_) {
      return variableLengthGetUntypedValueHelper<std::uint32_t, check_null>(code);
    } else {
      return fixedLengthGetUntypedValueHelper<std::uint32_t, check_null>(code);
    }
  }

  /**
   * @brief Get the value represented by the specified code as a TypedValue.
   * @note This version is for codes of 8 bits or less. Also see
   *       getTypedValueForShortCode() and getTypedValueForCode().
   * @warning It is an error to use this method with a code which does not
   *          exist in this dictionary, i.e. code must be less than
   *          numberOfCodes().
   *
   * @param code The compressed code to get the value for.
   * @return The typed value that corresponds to code.
   **/
  inline TypedValue getTypedValueForByteCode(const std::uint8_t code) const {
    if (type_is_variable_length_) {
      return variableLengthGetTypedValueHelper<std::uint8_t>(code);
    } else {
      return fixedLengthGetTypedValueHelper<std::uint8_t>(code);
    }
  }

  /**
   * @brief Get the value represented by the specified code as a TypedValue.
   * @note This version is for codes of 16 bits or less. Also see
   *       getTypedValueForByteCode() and getTypedValueForCode().
   * @warning It is an error to use this method with a code which does not
   *          exist in this dictionary, i.e. code must be less than
   *          numberOfCodes().
   *
   * @param code The compressed code to get the value for.
   * @return The typed value that corresponds to code.
   **/
  inline TypedValue getTypedValueForShortCode(const std::uint16_t code) const {
    if (type_is_variable_length_) {
      return variableLengthGetTypedValueHelper<std::uint16_t>(code);
    } else {
      return fixedLengthGetTypedValueHelper<std::uint16_t>(code);
    }
  }

  /**
   * @brief Get the value represented by the specified code as a TypedValue.
   * @note This version is for any code up to the maximum length of 32 bits.
   *       Also see getTypedValueForByteCode() and getTypedValueForShortCode().
   * @warning It is an error to use this method with a code which does not
   *          exist in this dictionary, i.e. code must be less than
   *          numberOfCodes().
   *
   * @param code The compressed code to get the value for.
   * @return The typed value that corresponds to code.
   **/
  inline TypedValue getTypedValueForCode(const std::uint32_t code) const {
    if (type_is_variable_length_) {
      return variableLengthGetTypedValueHelper<std::uint32_t>(code);
    } else {
      return fixedLengthGetTypedValueHelper<std::uint32_t>(code);
    }
  }

 protected:
  template <typename CodeType, bool check_null = true>
  inline const void* fixedLengthGetUntypedValueHelper(const CodeType code) const {
    if (check_null && (code == getNullCode())) {
      return nullptr;
    }
    DCHECK_LT(code, numberOfCodes());
    return static_cast<const char*>(dictionary_memory_)
           + 2 * sizeof(std::uint32_t)        // Header.
           + code * type_fixed_byte_length_;  // Index into value array.
  }

  template <typename CodeType, bool check_null = true>
  inline const void* variableLengthGetUntypedValueHelper(const CodeType code) const {
    if (check_null && (code == getNullCode())) {
      return nullptr;
    }
    DCHECK_LT(code, numberOfCodes());
    const void *retval = variable_length_data_region_
                         + static_cast<const std::uint32_t*>(dictionary_memory_)[code + 2];
    DCHECK_LT(retval, static_cast<const char*>(dictionary_memory_) + dictionary_memory_size_);
    return retval;
  }

  template <typename CodeType>
  inline TypedValue fixedLengthGetTypedValueHelper(const CodeType code) const {
    if (code == getNullCode()) {
      return TypedValue(type_.getTypeID());;
    }
    DCHECK_LT(code, numberOfCodes());
    return type_.makeValue(static_cast<const char*>(dictionary_memory_)
                               + 2 * sizeof(std::uint32_t)        // Header.
                               + code * type_fixed_byte_length_,  // Index into value array.
                           type_fixed_byte_length_);
  }

  template <typename CodeType>
  inline TypedValue variableLengthGetTypedValueHelper(const CodeType code) const {
    if (code == getNullCode()) {
      return TypedValue(type_.getTypeID());
    }
    DCHECK_LT(code, numberOfCodes());

    std::uint32_t value_offset = static_cast<const std::uint32_t*>(dictionary_memory_)[code + 2];
    const void *data_ptr = variable_length_data_region_ + value_offset;

    DCHECK_LT(data_ptr, static_cast<const char*>(dictionary_memory_) + dictionary_memory_size_);

    std::size_t data_size = (code == *static_cast<const std::uint32_t*>(dictionary_memory_) - 1) ?
        (static_cast<const char*>(dictionary_memory_)
            + dictionary_memory_size_
            - static_cast<const char*>(data_ptr))
        : (static_cast<const std::uint32_t*>(dictionary_memory_)[code + 3] - value_offset);
    return TypedValue(type_.getTypeID(), data_ptr, data_size);
  }

  const Type &type_;
  const bool type_is_variable_length_;
  const void *dictionary_memory_;
  const std::size_t dictionary_memory_size_;
  std::uint32_t number_of_codes_including_null_;
  std::uint8_t code_length_bits_;

  const std::size_t type_fixed_byte_length_;
  const char *variable_length_data_region_;

 private:
  bool paranoidOffsetsCheck() const;

  DISALLOW_COPY_AND_ASSIGN(CompressionDictionaryLite);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_COMPRESSION_COMPRESSION_DICTIONARY_LITE_HPP_
