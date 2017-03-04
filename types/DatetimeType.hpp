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

#ifndef QUICKSTEP_TYPES_DATETIME_TYPE_HPP_
#define QUICKSTEP_TYPES_DATETIME_TYPE_HPP_

#include <cstddef>
#include <cstdio>
#include <string>

#include "types/DatetimeLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeSynthesizer.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class TypedValue;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing the datetime.
 **/
class DatetimeType
    : public TypeSynthesizer<kDatetime> {
 public:
  int getPrintWidth() const override {
    return DatetimeLit::kIsoChars;
  }

  std::string printValueToString(const TypedValue &value) const override;

  /**
   * @note value_string is expected to be in (possibly extended) ISO-8601
   *       format. Extended ISO-8601 format is one of the following:
   *       "YYYY-MM-DD", "YYYY-MM-DDThh:mm:ss", or
   *       "YYYY-MM-DDThh:mm:ss.uuuuuu". YYYY is an integer year (in extended
   *       format, this may be more than 4 digits and include an optional
   *       sign prefix +/-). MM is a 2-digit month in the range 01-12. DD is
   *       a 2-digit day of month in the appropriate range for the month (i.e.
   *       28 days for February in non-leap years, 29 days for February in
   *       leap years, 30 days for April, June, September, and November, and
   *       31 days for all other months). T is the separator character between
   *       the date and time portions of the string (strict ISO-8601 requires
   *       that the character be 'T', but we also allow lower-case 't' and a
   *       single blank space ' '). hh is a 2-digit hour in the range 00-23.
   *       mm is a 2-digit minute in the range 00-59. ss is a 2-digit second
   *       in the range 00-59. uuuuuu is a fractional subsecond part of up to
   *       six digits (i.e. microsecond resolution). Note that parsing will
   *       fail if there are any "extra" characters at the end of the string
   *       after a parsable ISO-8601 date/time.
   **/
  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override;

 private:
  explicit DatetimeType(const bool nullable)
      : TypeSynthesizer<kDatetime>(nullable) {}

  template <typename, bool> friend class TypeInstance;

  DISALLOW_COPY_AND_ASSIGN(DatetimeType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DATETIME_TYPE_HPP_
