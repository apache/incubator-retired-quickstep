/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "types/IntervalParser.hpp"

#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include "types/IntervalLit.hpp"

// TODO(chasseur): This file has some moderately complex invocations of
// sscanf() and some branchy conditional parsing. Parsing performance might be
// improved by using some pre-compiled regexes instead of just sscanf().

namespace quickstep {

bool IntervalParser::ParseDatetimeIntervalSimpleFormat(
    const std::int64_t count,
    const std::string &units_lowercase,
    DatetimeIntervalLit *interval) {
  if ((units_lowercase == "microsecond")
      || (units_lowercase == "microseconds")
      || (units_lowercase == "us")) {
    interval->interval_ticks = count * (DatetimeIntervalLit::kTicksPerSecond / 1000000);
    return true;
  } else if ((units_lowercase == "millisecond")
      || (units_lowercase == "milliseconds")
      || (units_lowercase == "ms")) {
    interval->interval_ticks = count * (DatetimeIntervalLit::kTicksPerSecond / 1000);
    return true;
  } else if ((units_lowercase == "second")
      || (units_lowercase == "seconds")
      || (units_lowercase == "s")) {
    interval->interval_ticks = count * DatetimeIntervalLit::kTicksPerSecond;
    return true;
  } else if ((units_lowercase == "minute")
      || (units_lowercase == "minutes")) {
    interval->interval_ticks = count * DatetimeIntervalLit::kTicksPerSecond * 60;
    return true;
  } else if ((units_lowercase == "hour")
      || (units_lowercase == "hours")
      || (units_lowercase == "h")) {
    interval->interval_ticks = count * DatetimeIntervalLit::kTicksPerSecond * 60 * 60;
    return true;
  } else if ((units_lowercase == "day")
      || (units_lowercase == "days")
      || (units_lowercase == "d")) {
    interval->interval_ticks = count * DatetimeIntervalLit::kTicksPerSecond * 60 * 60 * 24;
    return true;
  } else if ((units_lowercase == "week")
      || (units_lowercase == "weeks")
      || (units_lowercase == "w")) {
    interval->interval_ticks = count * DatetimeIntervalLit::kTicksPerSecond * 60 * 60 * 24 * 7;
    return true;
  } else {
    return false;
  }
}

bool IntervalParser::ParseDatetimeIntervalComplexFormat(
    const std::string &interval_string,
    DatetimeIntervalLit *interval) {
  // First, look for a leading minus (we don't just parse the first field as
  // signed, because the first field could look like negative 0, e.g. in
  // "-00:30:00", and we would mistakenly parse such an interval as being
  // futureward rather than pastward).
  int start_offset = 0;
  int matched = std::sscanf(interval_string.c_str(),
                            "%*1[-]%n",
                            &start_offset);

  // Now, 'start_offset' is 0 if there is no leading minus, or nonzero and
  // pointing to the first character after the minus if there is one.

  std::int64_t days, hours;
  int minutes, seconds, subseconds;
  char daysbuf[5];  // Buffer for unit string containing "days" or something similar.
  int seconds_end = 0;
  int parse_end = 0;
  bool have_subseconds = false;

  // First, try matching with "X day(s)" specified at the front of the string.
  matched = std::sscanf(interval_string.c_str() + start_offset,
                        "%" SCNd64 " %4s %2" SCNd64 ":%2d:%2d%n.%6d%n",
                        &days,
                        daysbuf,
                        &hours,
                        &minutes,
                        &seconds,
                        &seconds_end,
                        &subseconds,
                        &parse_end);

  if ((matched == 5) || (matched == 6)) {
    // Make sure there is no garbage at the end of the string.
    switch (matched) {
      case 5:
        if (static_cast<std::string::size_type>(start_offset + seconds_end)
            != interval_string.length()) {
          return false;
        }
        break;
      case 6:
        if (static_cast<std::string::size_type>(start_offset + parse_end)
            != interval_string.length()) {
          return false;
        }
        have_subseconds = true;
        break;
      default:
        return false;
    }

    // Check that "day" or "days" (case insensitive) was specified where we
    // expect.
    const std::size_t daysbuf_length = std::strlen(daysbuf);
    for (std::size_t i = 0; i < daysbuf_length; ++i) {
      daysbuf[i] = ::tolower(daysbuf[i]);
    }
    if (!((std::strcmp("day", daysbuf) == 0)
          || (std::strcmp("days", daysbuf) == 0))) {
      return false;
    }

    // Check that hours are in the range 0-23 when a day is specified.
    if (hours > 23) {
      return false;
    }
  } else {
    // Try again without leading "days" portion.
    days = 0;
    seconds_end = parse_end = 0;
    matched = std::sscanf(interval_string.c_str() + start_offset,
                          "%" SCNd64 ":%2d:%2d%n.%6d%n",
                          &hours,
                          &minutes,
                          &seconds,
                          &seconds_end,
                          &subseconds,
                          &parse_end);

    // Make sure there is no garbage at the end of the string, and that at
    // least hours, minutes, seconds have been specified.
    switch (matched) {
      case 3:
        if (static_cast<std::string::size_type>(start_offset + seconds_end)
            != interval_string.length()) {
          return false;
        }
        break;
      case 4:
        if (static_cast<std::string::size_type>(start_offset + parse_end)
            != interval_string.length()) {
          return false;
        }
        have_subseconds = true;
        break;
      default:
        return false;
    }
  }

  // We already consumed the leading minus (if any) above, so make sure that
  // none of the fields parsed as negative.
  if ((days < 0)
      || (hours < 0)
      || (minutes < 0)
      || (seconds < 0)
      || (have_subseconds && (subseconds < 0))) {
    return false;
  }

  // Check that minutes and seconds are in-range.
  if ((minutes > 59) || (seconds > 59)) {
    return false;
  }

  if (have_subseconds) {
    // If less than 6 digits of subseconds were specified, multiply the
    // the subseconds so that they are scaled properly.
    const int adjustment_digits = 6 - (parse_end - seconds_end - 1);
    for (int i = 0; i < adjustment_digits; ++i) {
      subseconds *= 10;
    }
  } else {
    subseconds = 0;
  }

  // Add up the total magnitude of the interval in ticks.
  const std::int64_t total_ticks
      = (((days * 24 + hours) * 60 + minutes) * 60 + seconds)
            * DatetimeIntervalLit::kTicksPerSecond
        + subseconds;

  // Negate if there was a preceding minus "-".
  interval->interval_ticks = (start_offset == 0) ? total_ticks
                                                 : -total_ticks;

  return true;
}

bool IntervalParser::ParseYearMonthIntervalSimpleFormat(
    const std::int64_t count,
    const std::string &units_lowercase,
    YearMonthIntervalLit *interval) {
  if ((units_lowercase == "month")
      || (units_lowercase == "months")
      || (units_lowercase == "mon")
      || (units_lowercase == "mons")) {
    interval->months = count;
    return true;
  } else if ((units_lowercase == "year")
      || (units_lowercase == "years")
      || (units_lowercase == "y")) {
    interval->months = count * 12;
    return true;
  } else if ((units_lowercase == "decade")
      || (units_lowercase == "decades")) {
    interval->months = count * 12 * 10;
    return true;
  } else if ((units_lowercase == "century")
      || (units_lowercase == "centuries")) {
    interval->months = count * 12 * 100;
    return true;
  } else if ((units_lowercase == "millennium")
      || (units_lowercase == "millennia")) {
    interval->months = count * 12 * 1000;
    return true;
  } else {
    return false;
  }
}

bool IntervalParser::ParseYearMonthIntervalComplexFormat(
    const std::string &interval_string,
    YearMonthIntervalLit *interval) {
  // First, look for a leading minus (we don't just parse the first field as
  // signed, because the first field could look like negative 0, e.g. in
  // "-0 years 2 mons", and we would mistakenly parse such an interval as being
  // futureward rather than pastward).
  int start_offset = 0;
  int matched = std::sscanf(interval_string.c_str(),
                            "%*1[-]%n",
                            &start_offset);

  // Now, 'start_offset' is 0 if there is no leading minus, or nonzero and
  // pointing to the first character after the minus if there is one.

  std::int64_t years;
  int months;
  char yearsbuf[6];
  char monthsbuf[7];
  int parse_end = 0;

  matched = std::sscanf(interval_string.c_str() + start_offset,
                        "%" SCNd64 " %5s %d %6s%n",
                        &years,
                        yearsbuf,
                        &months,
                        monthsbuf,
                        &parse_end);

  // Make sure we matched all the expected fields, that there is no trailing
  // garbage at the end of the string, and that years and months are in range.
  if ((matched != 4)
      || (static_cast<std::string::size_type>(start_offset + parse_end)
          != interval_string.length())
      || (years < 0) || (months < 0) || (months > 11)) {
    return false;
  }

  // Case-insensitive check for some string indicating years.
  const std::size_t yearsbuf_length = std::strlen(yearsbuf);
  for (std::size_t i = 0; i < yearsbuf_length; ++i) {
    yearsbuf[i] = ::tolower(yearsbuf[i]);
  }
  if (!((std::strcmp("year", yearsbuf) == 0)
        || (std::strcmp("years", yearsbuf) == 0)
        || (std::strcmp("y", yearsbuf) == 0))) {
    return false;
  }

  // Case-insensitive check for some string indicating months.
  const std::size_t monthsbuf_length = std::strlen(monthsbuf);
  for (std::size_t i = 0; i < monthsbuf_length; ++i) {
    monthsbuf[i] = ::tolower(monthsbuf[i]);
  }
  if (!((std::strcmp("month", monthsbuf) == 0)
        || (std::strcmp("months", monthsbuf) == 0)
        || (std::strcmp("mon", monthsbuf) == 0)
        || (std::strcmp("mons", monthsbuf) == 0))) {
    return false;
  }

  // Add up total magnitude in months, negate if there was a preceding minus
  // "-".
  interval->months = (start_offset == 0) ? (years * 12 + months)
                                         : - (years * 12 + months);

  return true;
}

bool IntervalParser::ParseSimpleFormatFieldsFromCombinedString(
    const std::string &interval_string,
    std::int64_t *count,
    std::string *units_lowercase) {
  int units_start, units_end, scan_end;

  // Both value and units single-quoted.
  units_start = units_end = scan_end = 0;
  int matched = std::sscanf(interval_string.c_str(),
                            " ' %" SCNd64 " %n%*[^' \t\n\v\f\r]%n ' %n",
                            count, &units_start, &units_end, &scan_end);
  if ((matched == 1)
      && (static_cast<std::string::size_type>(scan_end) == interval_string.length())) {
    units_lowercase->assign(interval_string, units_start, units_end - units_start);
    std::transform(units_lowercase->begin(),
                   units_lowercase->end(),
                   units_lowercase->begin(),
                   ::tolower);
    return true;
  }

  // Value single-quoted, units not.
  units_start = units_end = scan_end = 0;
  matched = std::sscanf(interval_string.c_str(),
                        " ' %" SCNd64 " ' %n%*[^' \t\n\v\f\r]%n %n",
                        count, &units_start, &units_end, &scan_end);
  if ((matched == 1)
      && (static_cast<std::string::size_type>(scan_end) == interval_string.length())) {
    units_lowercase->assign(interval_string, units_start, units_end - units_start);
    std::transform(units_lowercase->begin(),
                   units_lowercase->end(),
                   units_lowercase->begin(),
                   ::tolower);
    return true;
  }

  // No quotes.
  units_start = units_end = scan_end = 0;
  matched = std::sscanf(interval_string.c_str(),
                        " %" SCNd64 " %n%*[^' \t\n\v\f\r]%n %n",
                        count, &units_start, &units_end, &scan_end);
  if ((matched == 1)
      && (static_cast<std::string::size_type>(scan_end) == interval_string.length())) {
    units_lowercase->assign(interval_string, units_start, units_end - units_start);
    std::transform(units_lowercase->begin(),
                   units_lowercase->end(),
                   units_lowercase->begin(),
                   ::tolower);
    return true;
  }

  // Unrecognized format.
  return false;
}

bool IntervalParser::ParseSimpleFormatFieldsFromCombinedStringNoExtraWhitespace(
    const std::string &interval_string,
    std::int64_t *count,
    std::string *units_lowercase) {
  int units_start, units_end, scan_end;

  // Both value and units single-quoted.
  units_start = units_end = scan_end = 0;
  int matched = std::sscanf(interval_string.c_str(),
                            "'%" SCNd64 " %n%*[^' \t\n\v\f\r]%n'%n",
                            count, &units_start, &units_end, &scan_end);
  if ((matched == 1)
      && (static_cast<std::string::size_type>(scan_end) == interval_string.length())) {
    units_lowercase->assign(interval_string, units_start, units_end - units_start);
    std::transform(units_lowercase->begin(),
                   units_lowercase->end(),
                   units_lowercase->begin(),
                   ::tolower);
    return true;
  }

  // Value single-quoted, units not.
  units_start = units_end = scan_end = 0;
  matched = std::sscanf(interval_string.c_str(),
                        "'%" SCNd64 "' %n%*[^' \t\n\v\f\r]%n",
                        count, &units_start, &units_end);
  if ((matched == 1)
      && (static_cast<std::string::size_type>(units_end) == interval_string.length())) {
    units_lowercase->assign(interval_string, units_start, units_end - units_start);
    std::transform(units_lowercase->begin(),
                   units_lowercase->end(),
                   units_lowercase->begin(),
                   ::tolower);
    return true;
  }

  // No quotes.
  units_start = units_end = scan_end = 0;
  matched = std::sscanf(interval_string.c_str(),
                        "%" SCNd64 " %n%*[^' \t\n\v\f\r]%n",
                        count, &units_start, &units_end);
  if ((matched == 1)
      && (static_cast<std::string::size_type>(units_end) == interval_string.length())) {
    units_lowercase->assign(interval_string, units_start, units_end - units_start);
    std::transform(units_lowercase->begin(),
                   units_lowercase->end(),
                   units_lowercase->begin(),
                   ::tolower);
    return true;
  }

  // Unrecognized format.
  return false;
}

}  // namespace quickstep
