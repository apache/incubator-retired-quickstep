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

#ifndef QUICKSTEP_TYPES_DATETIME_LIT_HPP_
#define QUICKSTEP_TYPES_DATETIME_LIT_HPP_

#include <cstdint>
#include <ctime>
#include <limits>

#include "types/port/gmtime_r.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A literal representing the date.
 **/
struct DateLit {
  // Note that although there is no year 0 in the Gregorian calendar, ISO 8601
  // has year 0 equivalent to 1 BCE, year -1 equivalent to 2 BCE, and so on.
  std::int32_t year;
  std::uint8_t month, day;

  // The maximum number of characters needed to represent any date in ISO 8601
  // notation.
  static constexpr int kIsoChars
      = 1     // + or - prefix
        + 5   // Year digits
        + 1   // -
        + 2   // Month
        + 1   // -
        + 2;  // Day

  static DateLit Create(const std::int32_t _year,
                        const std::uint8_t _month,
                        const std::uint8_t _day) {
    DateLit date;
    date.year = _year;
    date.month = _month;
    date.day = _day;

    return date;
  }

  inline bool operator< (const DateLit& rhs) const {
    return (year != rhs.year)
        ? (year < rhs.year)
        : ((month != rhs.month) ? (month < rhs.month) : (day < rhs.day));
  }

  inline bool operator> (const DateLit& rhs) const {
    return (year != rhs.year)
        ? (year > rhs.year)
        : ((month != rhs.month) ? (month > rhs.month) : (day > rhs.day));
  }

  inline bool operator<=(const DateLit& rhs) const {
    return !(*this > rhs);
  }

  inline bool operator>=(const DateLit& rhs) const {
    return !(*this < rhs);
  }

  inline bool operator==(const DateLit& rhs) const {
    return (year == rhs.year) &&
           (month == rhs.month) &&
           (day == rhs.day);
  }

  inline bool operator!=(const DateLit& rhs) const {
    return !(*this == rhs);
  }

  inline std::int32_t yearField() const {
    return year;
  }

  inline std::int32_t monthField() const {
    return static_cast<std::int32_t>(month);
  }

  inline std::int32_t dayField() const {
    return static_cast<std::int32_t>(day);
  }
};

/**
 * @brief A literal representing the datetime.
 **/
struct DatetimeLit {
  std::int64_t ticks;  // Ticks in GMT.

  static constexpr std::int64_t kTicksPerSecond = INT64_C(1000000);

  // The maximum number of characters needed to represent any date in ISO 8601
  // notation.
  static constexpr int kIsoChars
      = 1                                              // + or - prefix
        + std::numeric_limits<std::int64_t>::digits10  // Year digits
        + 1                                            // -
        + 2                                            // Month
        + 1                                            // -
        + 2                                            // Day
        + 1                                            // T
        + 2                                            // Hour
        + 1                                            // :
        + 2                                            // Minute
        + 1                                            // :
        + 2                                            // Second
        + 1                                            // .
        + 6;                                           // Subseconds

  static DatetimeLit FromEpochTime(const std::time_t t) {
    DatetimeLit datetime;
    datetime.ticks = static_cast<std::int64_t>(t) * kTicksPerSecond;
    return datetime;
  }

  static DatetimeLit FromEpochTimePlusSubseconds(const std::time_t t,
                                                 const std::int64_t subseconds) {
    return DatetimeLit{static_cast<std::int64_t>(t) * kTicksPerSecond + subseconds};
  }

  inline std::int64_t yearField() const {
    const std::time_t timestamp = epochTime();
    struct tm timeinfo;
    quickstep::gmtime_r(&timestamp, &timeinfo);
    return (timeinfo.tm_year + 1900);
  }

  inline std::int64_t monthField() const {
    const std::time_t timestamp = epochTime();
    struct tm timeinfo;
    quickstep::gmtime_r(&timestamp, &timeinfo);
    return (timeinfo.tm_mon + 1);
  }

  inline std::int64_t dayField() const {
    const std::time_t timestamp = epochTime();
    struct tm timeinfo;
    quickstep::gmtime_r(&timestamp, &timeinfo);
    return timeinfo.tm_mday;
  }

  inline std::int64_t hourField() const {
    const std::time_t timestamp = epochTime();
    struct tm timeinfo;
    quickstep::gmtime_r(&timestamp, &timeinfo);
    return timeinfo.tm_hour;
  }

  inline std::int64_t minuteField() const {
    const std::time_t timestamp = epochTime();
    struct tm timeinfo;
    quickstep::gmtime_r(&timestamp, &timeinfo);
    return timeinfo.tm_min;
  }

  inline std::int64_t secondField() const {
    const std::time_t timestamp = epochTime();
    struct tm timeinfo;
    quickstep::gmtime_r(&timestamp, &timeinfo);
    return timeinfo.tm_sec;
  }

  inline std::time_t epochTime() const {
    return static_cast<std::time_t>(ticks / kTicksPerSecond);
  }

  inline std::int64_t subseconds() const {
    return ticks % kTicksPerSecond;
  }

  inline bool operator< (const DatetimeLit& rhs) const {
    return ticks < rhs.ticks;
  }

  inline bool operator> (const DatetimeLit& rhs) const {
    return ticks > rhs.ticks;
  }

  inline bool operator<=(const DatetimeLit& rhs) const {
    return ticks <= rhs.ticks;
  }

  inline bool operator>=(const DatetimeLit& rhs) const {
    return ticks >= rhs.ticks;
  }

  inline bool operator==(const DatetimeLit& rhs) const {
    return ticks == rhs.ticks;
  }

  inline bool operator!=(const DatetimeLit& rhs) const {
    return ticks != rhs.ticks;
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DATETIME_LIT_HPP_
