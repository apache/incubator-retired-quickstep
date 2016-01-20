/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_TYPES_FLOAT_TYPE_HPP_
#define QUICKSTEP_TYPES_FLOAT_TYPE_HPP_

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
 * @brief A type representing a single-precision floating-point number.
 **/
class FloatType : public NumericSuperType<float> {
 public:
  static const TypeID kStaticTypeID;

  /**
   * @brief Get a reference to the non-nullable singleton instance of this
   *        Type.
   *
   * @return A reference to the non-nullable singleton instance of this Type
   **/
  static const FloatType& InstanceNonNullable() {
    static FloatType instance(false);
    return instance;
  }

  /**
   * @brief Get a reference to the nullable singleton instance of this Type
   *
   * @return A reference to the nullable singleton instance of this Type
   **/
  static const FloatType& InstanceNullable() {
    static FloatType instance(true);
    return instance;
  }

  /**
   * @brief Get a reference to a singleton instance of this Type
   *
   * @param nullable Whether to get the nullable version of this Type
   * @return A reference to the desired singleton instance of this Type
   **/
  static const FloatType& Instance(const bool nullable) {
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
  static_assert((std::numeric_limits<float>::max_exponent10 < 100)
                    && (std::numeric_limits<float>::min_exponent10 > -100),
                "Exponent range for float can not be represented with 2 "
                "decimal digits.");

  static constexpr int kPrintWidth
      = std::numeric_limits<float>::max_digits10  // Maximum mantissa digits
        + 1   // Decimal point '.'
        + 1   // Minus sign '-'
        + 2   // Exponent signifier 'e' and sign '+' or '-'
        + 2;  // Maximum exponent digits. Range of normalized base-10 exponents
              // for 32-bit float is -38 to +38. Smallest denormalized exponent
              // is -45. The static_assert above ensures that the exponent
              // never takes more than 2 base-10 digits to represent.

  explicit FloatType(const bool nullable)
      : NumericSuperType<float>(kFloat, nullable) {
  }

  DISALLOW_COPY_AND_ASSIGN(FloatType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_FLOAT_TYPE_HPP_
