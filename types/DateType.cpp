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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "types/DateType.hpp"

#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "types/DatetimeLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/CheckSnprintf.hpp"

#include "glog/logging.h"

// NetBSD's libc has snprintf, but it doesn't show up in the std namespace for
// C++.
#ifndef __NetBSD__
using std::snprintf;
#endif

namespace quickstep {

std::string DateType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  const DateLit &literal = castValueToLiteral(value);
  const std::int32_t year = literal.year;

  char datebuf[DateLit::kIsoChars + 1];
  std::size_t chars_written = 0;
  int snprintf_result = 0;

  // ISO 8601 requires that "expanded" year ranges (> 4 digits or before year
  // 0) are prefixed with a plus or minus.
  if ((year > 9999) || (year < 0)) {
    snprintf_result = snprintf(datebuf, sizeof(datebuf), "%+05" PRId32 "-", year);
  } else {
    snprintf_result = snprintf(datebuf, sizeof(datebuf), "%04" PRId32 "-", year);
  }
  CheckSnprintf(snprintf_result, sizeof(datebuf), &chars_written);

  // All the rest of the ISO 8601 date/time parts.
  snprintf_result = snprintf(datebuf + chars_written,
                             sizeof(datebuf) - chars_written,
                             "%02d-%02d",
                             literal.month,
                             literal.day);
  CheckSnprintf(snprintf_result, sizeof(datebuf), &chars_written);

  return std::string(datebuf);
}

bool DateType::parseTypedValueFromString(const std::string &value_string,
                                         TypedValue *value) const {
  std::int32_t year;
  std::uint32_t month, day;

  int date_chars = 0;

  const int matched = std::sscanf(value_string.c_str(),
                                  "%d-%2u-%2u%n",
                                  &year, &month, &day, &date_chars);

  // Check that the string is date, then check that there is no unmatched
  // garbage at the end of the string.
  if (matched != 3 ||
      (static_cast<std::string::size_type>(date_chars) != value_string.length())) {
    return false;
  }

  // Validate month.
  if ((month == 0) || (month > 12)) {
    return false;
  }

  // Validate day-of-month.
  if (day == 0) {
    return false;
  }
  switch (month) {
    case 2: {
      const std::uint32_t days_in_february
          = ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0))
            ? 29 : 28;
      if (day > days_in_february) {
        return false;
      }
      break;
    }
    case 4:  // Fall-through for 30-day months.
    case 6:
    case 9:
    case 11:
      if (day > 30) {
        return false;
      }
      break;
    default:
      if (day > 31) {
        return false;
      }
      break;
  }

  *value = TypedValue(DateLit::Create(year, month, day));
  return true;
}

}  // namespace quickstep
