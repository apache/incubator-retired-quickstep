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
#include "types/TypeSynthesizer.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class TypedValue;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing the date.
 **/
class DateType : public TypeSynthesizer<kDate> {
 public:
  int getPrintWidth() const override {
    return DateLit::kIsoChars;
  }

  std::string printValueToString(const UntypedLiteral *value) const override;

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
      : TypeSynthesizer<kDate>(nullable) {}

  QUICKSTEP_SYNTHESIZE_TYPE(DateType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DATE_TYPE_HPP_
