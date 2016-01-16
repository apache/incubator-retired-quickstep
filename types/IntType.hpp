/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_INT_TYPE_HPP_
#define QUICKSTEP_TYPES_INT_TYPE_HPP_

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
 * @brief A type representing a 32-bit integer.
 **/
class IntType : public NumericSuperType<int> {
 public:
  static const TypeID kStaticTypeID;

  /**
   * @brief Get a reference to the non-nullable singleton instance of this
   *        Type.
   *
   * @return A reference to the non-nullable singleton instance of this Type.
   **/
  static const IntType& InstanceNonNullable() {
    static IntType instance(false);
    return instance;
  }

  /**
   * @brief Get a reference to the nullable singleton instance of this Type.
   *
   * @return A reference to the nullable singleton instance of this Type.
   **/
  static const IntType& InstanceNullable() {
    static IntType instance(true);
    return instance;
  }

  /**
   * @brief Get a reference to a singleton instance of this Type.
   *
   * @param nullable Whether to get the nullable version of this Type.
   * @return A reference to the desired singleton instance of this Type.
   **/
  static const IntType& Instance(const bool nullable) {
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
    // Fully represented digits, single leading digit, and possible '-'
    // character.
    return std::numeric_limits<int>::digits10 + 2;
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
  explicit IntType(const bool nullable)
      : NumericSuperType<int>(kInt, nullable) {
  }

  DISALLOW_COPY_AND_ASSIGN(IntType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_INT_TYPE_HPP_
