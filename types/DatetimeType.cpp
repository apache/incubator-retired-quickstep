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

#include "types/DatetimeType.hpp"

#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#include "types/DatetimeLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/port/gmtime_r.hpp"
#include "types/port/timegm.hpp"
#include "utility/CheckSnprintf.hpp"

#include "glog/logging.h"

// NetBSD's libc has snprintf, but it doesn't show up in the std namespace for
// C++.
#ifndef __NetBSD__
using std::snprintf;
#endif

namespace quickstep {

std::string DatetimeType::printValueToString(const TypedValue &value) const {
  DCHECK(!value.isNull());

  const DatetimeLit literal = value.getLiteral<DatetimeLit>();
  const std::time_t timestamp = literal.epochTime();
  struct tm timeinfo;
  quickstep::gmtime_r(&timestamp, &timeinfo);
  const std::int64_t subseconds = literal.subseconds();

  // TODO(chasseur): std::put_time from C++11 makes it easy to write out
  // ISO-8601 formatted dates, but unfortunately it is not yet implemented in
  // GCC 4.8.3.

  // Note that although there is no year 0 in the Gregorian calendar, ISO 8601
  // has year 0 equivalent to 1 BCE, year -1 equivalent to 2 BCE, and so on.
  std::int64_t actual_year = INT64_C(1900) + timeinfo.tm_year;

  char datebuf[DatetimeLit::kIsoChars + 1];
  std::size_t chars_written = 0;
  int snprintf_result = 0;

  // ISO 8601 requires that "expanded" year ranges (> 4 digits or before year
  // 0) are prefixed with a plus or minus.
  if ((actual_year > 9999) || (actual_year < 0)) {
    snprintf_result = snprintf(datebuf, sizeof(datebuf), "%+05" PRId64 "-", actual_year);
  } else {
    snprintf_result = snprintf(datebuf, sizeof(datebuf), "%04" PRId64 "-", actual_year);
  }
  CheckSnprintf(snprintf_result, sizeof(datebuf), &chars_written);

  // All the rest of the ISO 8601 date/time parts.
  snprintf_result = snprintf(datebuf + chars_written,
                             sizeof(datebuf) - chars_written,
                             "%02d-%02dT%02d:%02d:%02d",
                             timeinfo.tm_mon + 1,
                             timeinfo.tm_mday,
                             timeinfo.tm_hour,
                             timeinfo.tm_min,
                             timeinfo.tm_sec);
  CheckSnprintf(snprintf_result, sizeof(datebuf), &chars_written);

  // Fractional subseconds, if any.
  if (subseconds != 0) {
    snprintf_result = snprintf(datebuf + chars_written,
                               sizeof(datebuf) - chars_written,
                               ".%06" PRId64,
                               subseconds);
    CheckSnprintf(snprintf_result, sizeof(datebuf), &chars_written);
  }

  return std::string(datebuf);
}

bool DatetimeType::parseValueFromString(const std::string &value_string,
                                        TypedValue *value) const {
  int year;
  int month;
  int day;
  char separator;
  int hour;
  int minute;
  int second;
  char subsecond_buffer[7];

  int date_chars = 0;
  int date_time_chars = 0;
  int date_time_subseconds_chars = 0;

  int matched = std::sscanf(value_string.c_str(),
                            "%d-%2d-%2d%n%c%2d:%2d:%2d%n.%6[0-9]%n",
                            &year, &month, &day, &date_chars,
                            &separator, &hour, &minute, &second, &date_time_chars,
                            subsecond_buffer, &date_time_subseconds_chars);

  // Check that the string specified date, date + time, or date + time with
  // optional subseconds, then check that there is no unmatched garbage at the
  // end of the string.
  if (matched == 3) {
    if (static_cast<std::string::size_type>(date_chars)
        != value_string.length()) {
      return false;
    }
  } else if (matched == 7) {
    if (static_cast<std::string::size_type>(date_time_chars)
        != value_string.length()) {
      return false;
    }
  } else if (matched == 8) {
    if (static_cast<std::string::size_type>(date_time_subseconds_chars)
        != value_string.length()) {
      return false;
    }
  } else {
    return false;
  }

  // Validate month.
  if ((month < 1) || (month > 12)) {
    return false;
  }

  // Validate day-of-month.
  if (day < 1) {
    return false;
  }
  switch (month) {
    case 2: {
      const int days_in_february
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

  // Broken-down time counts from midnight, January 1st, 1900.
  struct tm time_parts = {};
  time_parts.tm_year = year - 1900;
  time_parts.tm_mon = month - 1;
  time_parts.tm_mday = day;

  if (matched >= 7) {
    if (!((separator == 'T') || (separator == 't') || (separator == ' '))) {
      return false;
    }

    if ((hour < 0) || (hour > 23)) {
      return false;
    }

    if ((minute < 0) || (minute > 59)) {
      return false;
    }

    if ((second < 0) || (second > 59)) {
      // TODO(chasseur): Fix to account for leap-seconds.
      return false;
    }

    time_parts.tm_hour = hour;
    time_parts.tm_min = minute;
    time_parts.tm_sec = second;
  }

  std::time_t epoch_time = quickstep::timegm(&time_parts);
  if (epoch_time == static_cast<std::time_t>(-1)) {
    // Special case: the last second of 1969 actually was one second before the
    // epoch.
    if (!((year == 1969)
          && (month == 12)
          && (day == 31)
          && (time_parts.tm_hour = 23)
          && (time_parts.tm_min = 59)
          && (time_parts.tm_sec == 59))) {
      return false;
    }
  }

  std::int64_t subseconds = 0;
  if (matched == 8) {
    const std::size_t subsecond_digits = std::strlen(subsecond_buffer);
    if (subsecond_digits == 0) {
      return false;
    }

    subseconds = std::atoi(subsecond_buffer);
    if (subseconds < 0) {
      return false;
    }

    for (std::size_t adjustment_pos = subsecond_digits;
         adjustment_pos < 6;
         ++adjustment_pos) {
      subseconds *= 10;
    }
  }

  *value = TypedValue(DatetimeLit::FromEpochTimePlusSubseconds(epoch_time, subseconds));
  return true;
}

}  // namespace quickstep
