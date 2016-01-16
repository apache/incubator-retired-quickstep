/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_DOUBLE_TYPE_HPP_
#define QUICKSTEP_TYPES_DOUBLE_TYPE_HPP_

#include <cstdio>
#include <limits>
#include <string>

#include "types/NumericSuperType.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing a double-precision floating-point number.
 **/
class DoubleType : public NumericSuperType<double> {
 public:
  static const TypeID kStaticTypeID;

  /**
   * @brief Get a reference to the non-nullable singleton instance of this
   *        Type.
   *
   * @return A reference to the non-nullable singleton instance of this Type.
   **/
  static const DoubleType& InstanceNonNullable() {
    static DoubleType instance(false);
    return instance;
  }

  /**
   * @brief Get a reference to the nullable singleton instance of this Type.
   *
   * @return A reference to the nullable singleton instance of this Type.
   **/
  static const DoubleType& InstanceNullable() {
    static DoubleType instance(true);
    return instance;
  }

  /**
   * @brief Get a reference to a singleton instance of this Type.
   *
   * @param nullable Whether to get the nullable version of this Type.
   * @return A reference to the desired singleton instance of this Type.
   **/
  static const DoubleType& Instance(const bool nullable) {
    if (nullable) {
      return InstanceNullable();
    } else {
      return InstanceNonNullable();
    }
  }

  const Type& getNullableVersion() const override {
    return InstanceNullable();
  }

  const Type& getNonNullableVersion() const override {
    return InstanceNonNullable();
  }

  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  int getPrintWidth() const override {
    return kPrintWidth;
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
  static_assert((std::numeric_limits<double>::max_exponent10 < 1000)
                    && (std::numeric_limits<double>::min_exponent10 > -1000),
                "Exponent range for double can not be represented with 3 "
                "decimal digits.");

  static constexpr int kPrintWidth
      = std::numeric_limits<double>::max_digits10  // Maximum mantissa digits
        + 1   // Decimal point '.'
        + 1   // Minus sign '-'
        + 2   // Exponent signifier 'e' and sign '+' or '-'
        + 3;  // Maximum exponent digits. Range of normalized base-10 exponents
              // for 64-bit double is -308 to +308. Smallest denormalized
              // exponent is -324. The static_assert above ensures that the
              // exponent never takes more than 3 base-10 digits to represent.

  explicit DoubleType(const bool nullable)
      : NumericSuperType<double>(kDouble, nullable) {
  }

  DISALLOW_COPY_AND_ASSIGN(DoubleType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DOUBLE_TYPE_HPP_
