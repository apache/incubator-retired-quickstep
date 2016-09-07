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
  // ----------------------------------------------------
  // |  23 bits                       | 4 bits | 5 bits |
  // ----------------------------------------------------
  // |       year                     |  month |   day  |
  // | (unsigned)  (18 bits used)     |  (unsigned)     |
  // ----------------------------------------------------
  std::uint32_t year_month_day;

  // The maximum number of characters needed to represent any date in ISO 8601
  // notation.
  static constexpr int kIsoChars
      = 1     // + or - prefix
        + 5   // Year digits
        + 1   // -
        + 2   // Month
        + 1   // -
        + 2;  // Day

  // Years should be between [-kMaxYear, +kMaxYear] inclusive both end.
  static constexpr std::int32_t kMaxYear = 99999;
  static constexpr std::uint8_t kBitsNeededForDay = 5u;
  static constexpr std::uint8_t kBitsNeededForMonth = 4u;

  static DateLit Create(const std::int32_t _year,
                        const std::uint8_t _month,
                        const std::uint8_t _day) {
    DateLit date;
    // Normalize year by adding kMaxYear value, because we try to
    // encode signed year value into an unsigned integer.
    std::uint32_t representation
        = (static_cast<std::uint32_t>(_year + kMaxYear) << (kBitsNeededForDay + kBitsNeededForMonth))
          | (_month << kBitsNeededForDay)
          | _day;
    date.year_month_day = representation;
    return date;
  }

  static DateLit Create(const std::uint32_t serialized) {
    DateLit date;
    date.year_month_day = serialized;
    return date;
  }

  inline bool operator< (const DateLit& rhs) const {
    return year_month_day < rhs.year_month_day;
  }

  inline bool operator> (const DateLit& rhs) const {
    return year_month_day < rhs.year_month_day;
  }

  inline bool operator<=(const DateLit& rhs) const {
    return year_month_day <= rhs.year_month_day;
  }

  inline bool operator>=(const DateLit& rhs) const {
    return year_month_day >= rhs.year_month_day;
  }

  inline bool operator==(const DateLit& rhs) const {
    return year_month_day == rhs.year_month_day;
  }

  inline bool operator!=(const DateLit& rhs) const {
    return year_month_day != rhs.year_month_day;
  }

  inline std::int32_t yearField() const {
    const std::int32_t year =
        static_cast<std::int32_t>(year_month_day >>
                                  (kBitsNeededForDay + kBitsNeededForMonth)) - kMaxYear;
    return year;
  }

  inline std::uint32_t monthField() const {
    const std::uint32_t mask = 0x1E0u;  // 0b111100000
    return (year_month_day & mask) >> kBitsNeededForDay;
  }

  inline std::uint32_t dayField() const {
    const std::uint32_t mask = 0x1Fu;  // 0b11111
    return year_month_day & mask;
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
