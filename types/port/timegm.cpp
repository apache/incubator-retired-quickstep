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

#include "types/port/timegm.hpp"

#include <ctime>

#include "types/port/TypesPortConfig.h"
#include "types/port/gmtime_r.hpp"

#if defined(QUICKSTEP_HAVE_SETENV) && defined(QUICKSTEP_HAVE_TZSET)

#include <stdlib.h>
#include <time.h>

namespace quickstep {
namespace {

static bool SetupGMT() {
  setenv("TZ", "", 1);
  tzset();
  return true;
}

}  // namespace

const bool kGMTSetup = SetupGMT();

}  // namespace quickstep

#endif  // defined(QUICKSTEP_HAVE_SETENV) && defined(QUICKSTEP_HAVE_TZSET)

namespace quickstep {
namespace {

static std::time_t SetupLocalTimeOffset() {
  const std::time_t basetime = 0;
  struct tm time_gmt;
  quickstep::gmtime_r(&basetime, &time_gmt);
  return -mktime(&time_gmt);
}

}  // namespace

const std::time_t kLocalTimeOffset = SetupLocalTimeOffset();

namespace {

constexpr std::time_t kSecondsInMinute = 60;
constexpr std::time_t kMinutesInHour = 60;
constexpr std::time_t kHoursInDay = 24;
constexpr std::time_t kDaysInYear = 365;
constexpr std::time_t kSecondsInHour = kSecondsInMinute * kMinutesInHour;
constexpr std::time_t kSecondsInDay = kSecondsInHour * kHoursInDay;
constexpr std::time_t kSecondsInYear = kDaysInYear * kSecondsInDay;
constexpr std::time_t kEpochYearBase = 1970;
constexpr std::time_t kTMYearBase = 1900;
constexpr std::time_t kDaysUptoMonth[] = {
    0,
    31,                                                    // Jan
    31 + 28,                                               // Feb
    31 + 28 + 31,                                          // Mar
    31 + 28 + 31 + 30,                                     // Apr
    31 + 28 + 31 + 30 + 31,                                // May
    31 + 28 + 31 + 30 + 31 + 30,                           // Jun
    31 + 28 + 31 + 30 + 31 + 30 + 31,                      // Jul
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,                 // Aug
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,            // Sep
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,       // Oct
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,  // Nov
};
constexpr std::time_t kDaysUptoMonthLeapYear[] = {
    0,
    31,                                                    // Jan
    31 + 29,                                               // Feb
    31 + 29 + 31,                                          // Mar
    31 + 29 + 31 + 30,                                     // Apr
    31 + 29 + 31 + 30 + 31,                                // May
    31 + 29 + 31 + 30 + 31 + 30,                           // Jun
    31 + 29 + 31 + 30 + 31 + 30 + 31,                      // Jul
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,                 // Aug
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,            // Sep
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,       // Oct
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,  // Nov
};

inline bool IsLeapYear(std::time_t year) {
  // year is base 1900.
  const bool common_year =
      ((year & 3) != 0) ||
      (((year + kTMYearBase) % 100 == 0) && ((year + kTMYearBase) % 400 != 0));
  return !common_year;
}

constexpr std::time_t kPosBase4 = (kEpochYearBase + 3) % 4;
constexpr std::time_t kPosBase100 = (kEpochYearBase + 99) % 100;
constexpr std::time_t kPosBase400 = (kEpochYearBase + 399) % 400;
static_assert(static_cast<std::time_t>(-1) / 2 == 0,
              "Rounding towards 0 not guaranteed.");
// Negative bases will be similar as positive, if division rounds to negative
// infinity.
constexpr std::time_t kNegBase4 = 4 - (kEpochYearBase % 4);
constexpr std::time_t kNegBase100 = 100 - (kEpochYearBase % 100);
constexpr std::time_t kNegBase400 = 400 - (kEpochYearBase % 400);

// Number of leap days since 1970.
inline std::time_t NumLeapDays(std::time_t year) {
  // year is base 1970.
  if (year >= 0) {
    return ((year + kPosBase4) / 4) - ((year + kPosBase100) / 100) +
           ((year + kPosBase400) / 400);
  } else {
    return ((year - kNegBase4) / 4) - ((year - kNegBase100) / 100) +
           ((year - kNegBase400) / 400);
  }
}

}  // namespace

std::time_t timegmCustom(struct tm *tm) {
  const std::time_t years_since_epoch =
      tm->tm_year + kTMYearBase - kEpochYearBase;
  std::time_t time =
      years_since_epoch * kSecondsInYear +
      NumLeapDays(years_since_epoch) * kSecondsInDay;  // Account for leap days.

  // Account for months in current year.
  time += IsLeapYear(tm->tm_year)
              ? kDaysUptoMonthLeapYear[tm->tm_mon] * kSecondsInDay
              : kDaysUptoMonth[tm->tm_mon] * kSecondsInDay;
  // Account for days in current month, hours-mins-seconds in current day.
  time += (tm->tm_mday - 1) * kSecondsInDay + (tm->tm_hour * kSecondsInHour) +
          (tm->tm_min * kSecondsInMinute) + tm->tm_sec;
  return time;
}

}  // namespace quickstep
