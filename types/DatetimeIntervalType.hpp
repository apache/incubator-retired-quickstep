/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_DATETIME_INTERVAL_TYPE_HPP_
#define QUICKSTEP_TYPES_DATETIME_INTERVAL_TYPE_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing the datetime interval.
 **/
class DatetimeIntervalType : public Type {
 public:
  typedef DatetimeIntervalLit cpptype;

  static const TypeID kStaticTypeID = kDatetimeInterval;

  /**
   * @brief Get a reference to the non-nullable singleton instance of this
   *        Type.
   *
   * @return A reference to the non-nullable singleton instance of this Type.
   **/
  static const DatetimeIntervalType& InstanceNonNullable() {
    static DatetimeIntervalType instance(false);
    return instance;
  }

  /**
   * @brief Get a reference to the nullable singleton instance of this Type.
   *
   * @return A reference to the nullable singleton instance of this Type.
   **/
  static const DatetimeIntervalType& InstanceNullable() {
    static DatetimeIntervalType instance(true);
    return instance;
  }

  /**
   * @brief Get a reference to a singleton instance of this Type.
   *
   * @param nullable Whether to get the nullable version of this Type.
   * @return A reference to the desired singleton instance of this Type.
   **/
  static const DatetimeIntervalType& Instance(const bool nullable) {
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

  std::size_t estimateAverageByteLength() const override {
    return sizeof(DatetimeIntervalLit);
  }

  bool isCoercibleFrom(const Type &original_type) const override;

  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  int getPrintWidth() const override {
    return DatetimeIntervalLit::kPrintingChars;
  }

  std::string printValueToString(const TypedValue &value) const override;

  void printValueToFile(const TypedValue &value,
                        FILE *file,
                        const int padding = 0) const override;

  TypedValue makeZeroValue() const override {
    return TypedValue(DatetimeIntervalLit{0});
  }

  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override;

 private:
  explicit DatetimeIntervalType(const bool nullable)
      : Type(Type::kOther, kDatetimeInterval, nullable, sizeof(DatetimeIntervalLit), sizeof(DatetimeIntervalLit)) {
  }

  DISALLOW_COPY_AND_ASSIGN(DatetimeIntervalType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DATETIME_INTERVAL_TYPE_HPP_
