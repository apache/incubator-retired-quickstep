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

#ifndef QUICKSTEP_TYPES_DATE_TYPE_HPP_
#define QUICKSTEP_TYPES_DATE_TYPE_HPP_

#include <cstddef>
#include <cstdio>
#include <string>

#include "types/DatetimeLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class TypedValue;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing the date.
 **/
class DateType : public Type {
 public:
  typedef DateLit cpptype;

  static const TypeID kStaticTypeID = kDate;

  /**
   * @brief Get a reference to the non-nullable singleton instance of this
   *        Type.
   *
   * @return A reference to the non-nullable singleton instance of this Type.
   **/
  static const DateType& InstanceNonNullable() {
    static DateType instance(false);
    return instance;
  }

  /**
   * @brief Get a reference to the nullable singleton instance of this Type.
   *
   * @return A reference to the nullable singleton instance of this Type.
   **/
  static const DateType& InstanceNullable() {
    static DateType instance(true);
    return instance;
  }

  /**
   * @brief Get a reference to a singleton instance of this Type.
   *
   * @param nullable Whether to get the nullable version of this Type.
   * @return A reference to the desired singleton instance of this Type.
   **/
  static const DateType& Instance(const bool nullable) {
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
    return sizeof(DateLit);
  }

  bool isCoercibleFrom(const Type &original_type) const override;

  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  int getPrintWidth() const override {
    return DateLit::kIsoChars;
  }

  std::string printValueToString(const TypedValue &value) const override;

  void printValueToFile(const TypedValue &value,
                        FILE *file,
                        const int padding = 0) const override;

  /**
   * @note value_string is expected to be in (possibly extended) ISO-8601
   *       format. Extended ISO-8601 date format is "YYYY-MM-DD". YYYY is an
   *       integer year (in extended format, this may be more than 4 digits and
   *       include an optional sign prefix +/-). MM is a 2-digit month in the
   *       range 01-12. DD is a 2-digit day of month in the appropriate range
   *       for the month (i.e. 28 days for February in non-leap years, 29 days
   *       for February in leap years, 30 days for April, June, September, and
   *       November, and 31 days for all other months). Note that parsing will
   *       fail if there are any "extra" characters at the end of the string
   *       after a parsable ISO-8601 date.
   **/
  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override;

 private:
  explicit DateType(const bool nullable)
      : Type(Type::kOther, kDate, nullable, sizeof(DateLit), sizeof(DateLit)) {
  }

  DISALLOW_COPY_AND_ASSIGN(DateType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DATE_TYPE_HPP_
