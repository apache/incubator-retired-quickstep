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

#ifndef QUICKSTEP_TYPES_YEAR_MONTH_INTERVAL_TYPE_HPP_
#define QUICKSTEP_TYPES_YEAR_MONTH_INTERVAL_TYPE_HPP_

#include <cstddef>
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
 * @brief A type representing the year-month interval.
 **/
class YearMonthIntervalType : public Type {
 public:
  typedef YearMonthIntervalLit cpptype;

  static const TypeID kStaticTypeID = kYearMonthInterval;

  /**
   * @brief Get a reference to the non-nullable singleton instance of this
   *        Type.
   *
   * @return A reference to the non-nullable singleton instance of this Type.
   **/
  static const YearMonthIntervalType& InstanceNonNullable() {
    static YearMonthIntervalType instance(false);
    return instance;
  }

  /**
   * @brief Get a reference to the nullable singleton instance of this Type.
   *
   * @return A reference to the nullable singleton instance of this Type.
   **/
  static const YearMonthIntervalType& InstanceNullable() {
    static YearMonthIntervalType instance(true);
    return instance;
  }

  /**
   * @brief Get a reference to a singleton instance of this Type.
   *
   * @param nullable Whether to get the nullable version of this Type.
   * @return A reference to the desired singleton instance of this Type.
   **/
  static const YearMonthIntervalType& Instance(const bool nullable) {
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
    return sizeof(YearMonthIntervalLit);
  }

  bool isCoercibleFrom(const Type &original_type) const override;

  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  int getPrintWidth() const override {
    return YearMonthIntervalLit::kPrintingChars;
  }

  std::string printValueToString(const TypedValue &value) const override;

  void printValueToFile(const TypedValue &value,
                        FILE *file,
                        const int padding = 0) const override;

  TypedValue makeZeroValue() const override {
    return TypedValue(YearMonthIntervalLit{0});
  }

  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override;

 private:
  explicit YearMonthIntervalType(const bool nullable)
      : Type(Type::kOther, kYearMonthInterval, nullable, sizeof(YearMonthIntervalLit), sizeof(YearMonthIntervalLit)) {
  }

  DISALLOW_COPY_AND_ASSIGN(YearMonthIntervalType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_YEAR_MONTH_INTERVAL_TYPE_HPP_
