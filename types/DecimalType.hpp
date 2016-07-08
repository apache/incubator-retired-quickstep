/**
 *   Licensed to the Apache Software Foundation (ASF) under one
 *   or more contributor license agreements.  See the NOTICE file
 *   distributed with this work for additional information
 *   regarding copyright ownership.  The ASF licenses this file
 *   to you under the Apache License, Version 2.0 (the
 *   "License"); you may not use this file except in compliance
 *   with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing,
 *   software distributed under the License is distributed on an
 *   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *   KIND, either express or implied.  See the License for the
 *   specific language governing permissions and limitations
 *   under the License.
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_TYPES_DECIMAL_TYPE_HPP_
#define QUICKSTEP_TYPES_DECIMAL_TYPE_HPP_

#include <limits>
#include <string>

#include "types/DecimalLit.hpp"
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
 * @brief A type representing a fixed-precision number.
 **/
class DecimalType : public NumericSuperType<DecimalLit> {
 public:
  static const TypeID kStaticTypeID;

  /**
   * @brief Get a reference to the non-nullable singleton instance of this
   *        Type.
   *
   * @return A reference to the non-nullable singleton instance of this Type.
   **/
  static const DecimalType& InstanceNonNullable() {
    static DecimalType instance(false);
    return instance;
  }

  /**
   * @brief Get a reference to the nullable singleton instance of this Type.
   *
   * @return A reference to the nullable singleton instance of this Type.
   **/
  static const DecimalType& InstanceNullable() {
    static DecimalType instance(true);
    return instance;
  }

  static const DecimalType& Instance(const bool nullable) {
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

  TypedValue makeZeroValue() const override {
    return TypedValue(DecimalLit(0));
  }

 private:
  static constexpr int kPrintWidth =
      std::numeric_limits<DecimalLit::data_type>::digits10
      + 1   // Decimal point '.'
      + 1;  // Minus sign '-'

  explicit DecimalType(const bool nullable)
       : NumericSuperType<DecimalLit>(kDecimal, nullable) {
  }

  DISALLOW_COPY_AND_ASSIGN(DecimalType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DECIMAL_TYPE_HPP_
