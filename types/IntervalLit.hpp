/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_TYPES_INTERVAL_LIT_HPP_
#define QUICKSTEP_TYPES_INTERVAL_LIT_HPP_

#include <cstdint>
#include <limits>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A literal representing the datetime interval.
 **/
struct DatetimeIntervalLit {
  std::int64_t interval_ticks;

  static constexpr std::int64_t kTicksPerSecond = INT64_C(1000000);

  // The maximum number of characters needed to print out a DatetimeInterval in
  // the "standard" format.
  static constexpr int kPrintingChars
      = 1                                              // Possible '-' prefix
        + std::numeric_limits<std::int64_t>::digits10  // Day
        + 6                                            // " days "
        + 2                                            // Hour
        + 1                                            // :
        + 2                                            // Minute
        + 1                                            // :
        + 2                                            // Second
        + 1                                            // .
        + 6;                                           // Subseconds.

  inline std::int64_t seconds() const {
    return interval_ticks / kTicksPerSecond;
  }

  inline std::int64_t subseconds() const {
    return interval_ticks % kTicksPerSecond;
  }

  inline bool operator< (const DatetimeIntervalLit& rhs) const {
    return interval_ticks < rhs.interval_ticks;
  }

  inline bool operator> (const DatetimeIntervalLit& rhs) const {
    return interval_ticks > rhs.interval_ticks;
  }

  inline bool operator<=(const DatetimeIntervalLit& rhs) const {
    return interval_ticks <= rhs.interval_ticks;
  }

  inline bool operator>=(const DatetimeIntervalLit& rhs) const {
    return interval_ticks >= rhs.interval_ticks;
  }

  inline bool operator==(const DatetimeIntervalLit& rhs) const {
    return interval_ticks == rhs.interval_ticks;
  }

  inline bool operator!=(const DatetimeIntervalLit& rhs) const {
    return interval_ticks != rhs.interval_ticks;
  }

  inline DatetimeIntervalLit operator-() const {
    DatetimeIntervalLit result;
    result.interval_ticks = -interval_ticks;
    return result;
  }

  inline DatetimeIntervalLit& operator+=(const DatetimeIntervalLit& rhs) {
    interval_ticks += rhs.interval_ticks;
    return *this;
  }

  inline DatetimeIntervalLit& operator-=(const DatetimeIntervalLit& rhs) {
    interval_ticks -= rhs.interval_ticks;
    return *this;
  }

  template<typename NumericCppType>
  inline DatetimeIntervalLit& operator*=(const NumericCppType rhs) {
    interval_ticks *= rhs;
    return *this;
  }

  template<typename NumericCppType>
  inline DatetimeIntervalLit& operator/=(const NumericCppType rhs) {
    DCHECK_NE(rhs, 0);
    interval_ticks /= rhs;
    return *this;
  }
};

/**
 * @brief A literal representing the year-month interval.
 **/
struct YearMonthIntervalLit {
  std::int64_t months;

  // The maximum number of characters needed to print out a DatetimeInterval in
  // the "standard" format.
  static constexpr int kPrintingChars
      = 1                                              // Possible '-' prefix
        + std::numeric_limits<std::int64_t>::digits10  // Year
        + 7                                            // " years "
        + 2                                            // Month
        + 5;                                           // " mons"

  inline bool operator< (const YearMonthIntervalLit& rhs) const {
    return months < rhs.months;
  }

  inline bool operator> (const YearMonthIntervalLit& rhs) const {
    return months > rhs.months;
  }

  inline bool operator<=(const YearMonthIntervalLit& rhs) const {
    return months <= rhs.months;
  }

  inline bool operator>=(const YearMonthIntervalLit& rhs) const {
    return months >= rhs.months;
  }

  inline bool operator==(const YearMonthIntervalLit& rhs) const {
    return months == rhs.months;
  }

  inline bool operator!=(const YearMonthIntervalLit& rhs) const {
    return months != rhs.months;
  }

  inline YearMonthIntervalLit operator-() const {
    YearMonthIntervalLit result;
    result.months = -months;
    return result;
  }

  inline YearMonthIntervalLit& operator+=(const YearMonthIntervalLit& rhs) {
    months += rhs.months;
    return *this;
  }

  inline YearMonthIntervalLit& operator-=(const YearMonthIntervalLit& rhs) {
    months -= rhs.months;
    return *this;
  }

  template<typename type>
  inline YearMonthIntervalLit& operator*=(const type rhs) {
    months *= rhs;
    return *this;
  }

  template<typename type>
  inline YearMonthIntervalLit& operator/=(const type rhs) {
    DCHECK_NE(rhs, 0);
    months /= rhs;
    return *this;
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_INTERVAL_LIT_HPP_
