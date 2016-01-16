/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_CHAR_TYPE_HPP_
#define QUICKSTEP_TYPES_CHAR_TYPE_HPP_

#include <cstddef>
#include <cstdio>
#include <string>

#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing a fixed-size ASCII string.
 *
 * @note A string that is exactly the maximum length of a CharType is
 *       represented WITHOUT a null-terminator character. Any strings shorter
 *       than the maximum length will have a null-terminator.
 **/
class CharType : public AsciiStringSuperType {
 public:
  /**
   * @brief Get a reference to the non-nullable singleton instance of this Type
   *        for the specified length.
   *
   * @param length The length parameter of the CharType.
   * @return A reference to the non-nullable singleton instance of this Type
   *         for the specified length.
   **/
  static const CharType& InstanceNonNullable(const std::size_t length);

  /**
   * @brief Get a reference to the nullable singleton instance of this Type for
   *        the specified length.
   *
   * @param length The length parameter of the CharType.
   * @return A reference to the nullable singleton instance of this Type for
   *         the specified length.
   **/
  static const CharType& InstanceNullable(const std::size_t length);

  /**
   * @brief Get a reference to the singleton instance of this Type for the
   *        specified length and nullability.
   *
   * @param length The length parameter of the CharType.
   * @param nullable Whether to get the nullable version of this Type.
   * @return A reference to the singleton instance of this Type for the
   *         specified length.
   **/
  static const CharType& Instance(const std::size_t length, const bool nullable) {
    if (nullable) {
      return InstanceNullable(length);
    } else {
      return InstanceNonNullable(length);
    }
  }

  /**
   * @brief Get a reference to the singleton instance of this Type described
   *        by the given Protocol Buffer serialization.
   *
   * @param proto The serialized Protocol Buffer representation of the desired
   *        CharType.
   * @return A reference to the singleton instance of this Type for the given
   *         Protocol Buffer.
   **/
  static const CharType& InstanceFromProto(const serialization::Type &proto);

  /**
   * @brief Generate a serialized Protocol Buffer representation of this Type.
   *
   * @return The serialized Protocol Buffer representation of this Type.
   **/
  serialization::Type getProto() const override;

  const Type& getNullableVersion() const override {
    return InstanceNullable(length_);
  }

  const Type& getNonNullableVersion() const override {
    return InstanceNonNullable(length_);
  }

  std::size_t estimateAverageByteLength() const override {
    return length_;
  }

  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  std::string getName() const override;

  int getPrintWidth() const override {
    return length_;
  }

  std::string printValueToString(const TypedValue &value) const override;

  void printValueToFile(const TypedValue &value,
                        FILE *file,
                        const int padding = 0) const override;

  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override;

  TypedValue coerceValue(const TypedValue &original_value,
                         const Type &original_type) const override;

 private:
  CharType(const std::size_t length, const bool nullable)
      : AsciiStringSuperType(kChar, nullable, length, length, length) {
  }

  template <bool nullable_internal>
  static const CharType& InstanceInternal(const std::size_t length);

  DISALLOW_COPY_AND_ASSIGN(CharType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_CHAR_TYPE_HPP_
