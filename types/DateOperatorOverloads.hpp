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

#ifndef QUICKSTEP_TYPES_DATE_BINARY_OPERATORS_HPP_
#define QUICKSTEP_TYPES_DATE_BINARY_OPERATORS_HPP_

#include <cstdint>
#include <ctime>

#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/port/gmtime_r.hpp"
#include "types/port/timegm.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

// Month arithmetic clamps to the actual last day of a given month.
inline int ClampDayOfMonth(const int year, const int month, const int day) {
  DCHECK_LT(day, 32);
  switch (month) {
    case 2: {
      const int days_in_february =
          (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
              ? 29
              : 28;
      return (day > days_in_february) ? days_in_february : day;
    }
    case 4:
    case 6:
    case 9:
    case 11: {
      return day == 31 ? 30 : day;
    }
    default:
      return day;
  }
}

// We have a separate function here because timeinfo's months begin from 0.
inline void ClampDayOfMonthStructTM(struct tm *timeinfo) {
  DCHECK_LT(timeinfo->tm_mday, 32);
  switch (timeinfo->tm_mon) {
    case 1: {
      const int days_in_february
          = ((timeinfo->tm_year & 3) == 0 && ((timeinfo->tm_year % 25) != 0
                                              || ((timeinfo->tm_year + 1900) & 15) == 0))
            ? 29 : 28;
      timeinfo->tm_mday = timeinfo->tm_mday > days_in_february
                          ? days_in_february : timeinfo->tm_mday;
      return;
    }
    case 3:
    case 5:
    case 8:
    case 10: {
      timeinfo->tm_mday = timeinfo->tm_mday == 31 ? 30 : timeinfo->tm_mday;
      return;
    }
    default:
      return;
  }
}

// Overloaded operators for all date operations.
inline DatetimeLit operator+(const DatetimeLit &lhs, const DatetimeIntervalLit &rhs) {
  DatetimeLit datetime;
  datetime.ticks = lhs.ticks + rhs.interval_ticks;
  return datetime;
}

inline DatetimeLit operator+(const DatetimeIntervalLit &lhs, const DatetimeLit &rhs) {
  DatetimeLit datetime;
  datetime.ticks = lhs.interval_ticks + rhs.ticks;
  return datetime;
}

inline DatetimeLit operator+(const DatetimeLit &lhs, const YearMonthIntervalLit &rhs) {
  std::time_t timestamp = lhs.epochTime();

  struct tm timeinfo;
  quickstep::gmtime_r(&timestamp, &timeinfo);

  timeinfo.tm_year += rhs.months / 12;
  timeinfo.tm_mon += rhs.months % 12;

  if (timeinfo.tm_mon > 11) {
    timeinfo.tm_mon -= 12;
    ++timeinfo.tm_year;
  }

  ClampDayOfMonthStructTM(&timeinfo);

  DatetimeLit datetime(DatetimeLit::FromEpochTime(quickstep::timegm(&timeinfo)));
  datetime.ticks += lhs.subseconds();
  return datetime;  // Datetime in GMT.
}

inline DatetimeLit operator+(const YearMonthIntervalLit &lhs, const DatetimeLit &rhs) {
  return rhs + lhs;
}

inline DateLit operator+(const DateLit &lhs, const YearMonthIntervalLit &rhs) {
  std::int32_t result_year = lhs.yearField() + (rhs.months / 12);
  std::uint8_t result_month = static_cast<std::uint8_t>(lhs.monthField()) + (rhs.months % 12);

  if (result_month > 11) {
    result_month -= 12;
    ++result_year;
  }

  const std::uint8_t result_day = static_cast<std::uint8_t>(
      ClampDayOfMonth(result_year, result_month, lhs.dayField()));

  return DateLit::Create(result_year, result_month, result_day);
}

inline DateLit operator+(const YearMonthIntervalLit &lhs, const DateLit &rhs) {
  return rhs + lhs;
}

inline DatetimeIntervalLit operator+(const DatetimeIntervalLit &lhs, const DatetimeIntervalLit &rhs) {
  DatetimeIntervalLit interval(lhs);
  interval += rhs;
  return interval;
}

inline YearMonthIntervalLit operator+(const YearMonthIntervalLit &lhs, const YearMonthIntervalLit &rhs) {
  YearMonthIntervalLit interval(lhs);
  interval += rhs;
  return interval;
}

// NOTE(zuyu): The result type of the Subtract between two Datetimes
//             is DatetimeInterval, instead of YearMonthInterval.
inline DatetimeIntervalLit operator-(const DatetimeLit &lhs, const DatetimeLit &rhs) {
  DatetimeIntervalLit interval;
  interval.interval_ticks = lhs.ticks - rhs.ticks;
  return interval;
}

inline DatetimeLit operator-(const DatetimeLit &lhs, const DatetimeIntervalLit &rhs) {
  DatetimeLit datetime;
  datetime.ticks = lhs.ticks - rhs.interval_ticks;
  return datetime;
}

inline DatetimeLit operator-(const DatetimeLit &lhs, const YearMonthIntervalLit &rhs) {
  std::time_t timestamp = lhs.epochTime();
  struct tm timeinfo;
  quickstep::gmtime_r(&timestamp, &timeinfo);

  timeinfo.tm_year -= rhs.months / 12;
  timeinfo.tm_mon -= rhs.months % 12;

  if (timeinfo.tm_mon < 0) {
    --timeinfo.tm_year;
    timeinfo.tm_mon += 12;
  }

  ClampDayOfMonthStructTM(&timeinfo);

  DatetimeLit datetime(DatetimeLit::FromEpochTime(quickstep::timegm(&timeinfo)));
  datetime.ticks += lhs.subseconds();
  return datetime;  // Datetime in GMT.
}

inline DateLit operator-(const DateLit &lhs, const YearMonthIntervalLit &rhs) {
  std::int32_t result_year = lhs.yearField() - (rhs.months / 12);
  std::int8_t result_month = static_cast<std::int8_t>(lhs.monthField()) - (rhs.months % 12);

  if (result_month < 0) {
    --result_year;
    result_month += 12;
  }

  const std::uint8_t result_day = static_cast<std::uint8_t>(
      ClampDayOfMonth(result_year, result_month, lhs.dayField()));

  return DateLit::Create(result_year, result_month, result_day);
}

inline DatetimeIntervalLit operator-(const DatetimeIntervalLit &lhs, const DatetimeIntervalLit &rhs) {
  DatetimeIntervalLit interval(lhs);
  interval -= rhs;
  return interval;
}

inline YearMonthIntervalLit operator-(const YearMonthIntervalLit &lhs, const YearMonthIntervalLit &rhs) {
  YearMonthIntervalLit interval(lhs);
  interval -= rhs;
  return interval;
}

template <typename NumericCppType>
inline DatetimeIntervalLit operator*(const DatetimeIntervalLit &lhs, const NumericCppType &rhs) {
  DatetimeIntervalLit interval(lhs);
  interval *= rhs;
  return interval;
}

template <typename NumericCppType>
inline DatetimeIntervalLit operator*(const NumericCppType &lhs, const DatetimeIntervalLit &rhs) {
  DatetimeIntervalLit interval(rhs);
  interval *= lhs;
  return interval;
}

template <typename NumericCppType>
inline YearMonthIntervalLit operator*(const YearMonthIntervalLit &lhs, const NumericCppType &rhs) {
  YearMonthIntervalLit interval(lhs);
  interval *= rhs;
  return interval;
}

template <typename NumericCppType>
inline YearMonthIntervalLit operator*(const NumericCppType &lhs, const YearMonthIntervalLit &rhs) {
  YearMonthIntervalLit interval(rhs);
  interval *= lhs;
  return interval;
}

template <typename NumericCppType>
inline DatetimeIntervalLit operator/(const DatetimeIntervalLit &lhs, const NumericCppType &rhs) {
  DatetimeIntervalLit interval(lhs);
  interval /= rhs;
  return interval;
}

template <typename NumericCppType>
inline YearMonthIntervalLit operator/(const YearMonthIntervalLit &lhs, const NumericCppType &rhs) {
  YearMonthIntervalLit interval(lhs);
  interval /= rhs;
  return interval;
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DATE_BINARY_OPERATORS_HPP_
