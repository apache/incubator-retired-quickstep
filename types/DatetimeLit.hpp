/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
 * @brief A literal representing the datetime.
 **/
struct DatetimeLit {
  std::int64_t ticks;  // Ticks in GMT.

  static constexpr std::int64_t kTicksPerSecond = 1000000LL;

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
