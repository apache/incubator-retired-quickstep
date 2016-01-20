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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "types/YearMonthIntervalType.hpp"

#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <string>

#include "types/IntervalLit.hpp"
#include "types/IntervalParser.hpp"
#include "types/NullCoercibilityCheckMacro.hpp"
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

bool YearMonthIntervalType::isCoercibleFrom(const Type &original_type) const {
  QUICKSTEP_NULL_COERCIBILITY_CHECK();
  return (original_type.getTypeID() == kYearMonthInterval);
}

bool YearMonthIntervalType::isSafelyCoercibleFrom(const Type &original_type) const {
  QUICKSTEP_NULL_COERCIBILITY_CHECK();
  return (original_type.getTypeID() == kYearMonthInterval);
}

std::string YearMonthIntervalType::printValueToString(const TypedValue &value) const {
  DCHECK(!value.isNull());

  std::int64_t months = value.getLiteral<YearMonthIntervalLit>().months;
  const bool negative_interval = months < 0;
  if (negative_interval) {
    months = -months;
  }
  std::int64_t years = months / 12;
  months -= years * 12;

  char interval_buf[YearMonthIntervalLit::kPrintingChars + 1];
  std::size_t chars_written = 0;
  int snprintf_result = 0;

  if (negative_interval) {
    interval_buf[0] = '-';
    interval_buf[1] = '\0';
    ++chars_written;
  }

  if (years != 0) {
    snprintf_result = snprintf(interval_buf + chars_written,
                               sizeof(interval_buf) - chars_written,
                               "%" PRId64,
                               years);
    CheckSnprintf(snprintf_result, sizeof(interval_buf), &chars_written);

    if (years == 1) {
      snprintf_result = snprintf(interval_buf + chars_written,
                                 sizeof(interval_buf) - chars_written,
                                 " year");
    } else {
      snprintf_result = snprintf(interval_buf + chars_written,
                                 sizeof(interval_buf) - chars_written,
                                 " years");
    }
    DCHECK_GE(snprintf_result, 0);
    chars_written += snprintf_result;
    DCHECK_LT(chars_written, sizeof(interval_buf));
  }

  if ((months != 0) || (years == 0)) {
    if (years != 0) {
      DCHECK_LT(chars_written + 2, sizeof(interval_buf));
      interval_buf[chars_written] = ' ';
      ++chars_written;
      interval_buf[chars_written] = '\0';
    }

    snprintf_result = snprintf(interval_buf + chars_written,
                               sizeof(interval_buf) - chars_written,
                               "%" PRId64,
                               months);
    CheckSnprintf(snprintf_result, sizeof(interval_buf), &chars_written);

    if (months == 1) {
      snprintf_result = snprintf(interval_buf + chars_written,
                                 sizeof(interval_buf) - chars_written,
                                 " mon");
    } else {
      snprintf_result = snprintf(interval_buf + chars_written,
                                 sizeof(interval_buf) - chars_written,
                                 " mons");
    }
    CheckSnprintf(snprintf_result, sizeof(interval_buf), &chars_written);
  }

  return std::string(interval_buf);
}

void YearMonthIntervalType::printValueToFile(const TypedValue &value,
                                             FILE *file,
                                             const int padding) const {
  // We simply re-use the logic from printValueToString(), as trying to do
  // padding on-the fly with so many different fields is too much of a hassle.
  std::fprintf(file, "%*s", static_cast<int>(padding), printValueToString(value).c_str());
}

bool YearMonthIntervalType::parseValueFromString(const std::string &value_string,
                                                 TypedValue *value) const {
  // Try simple-format parse first.
  std::int64_t count;
  std::string units;
  YearMonthIntervalLit literal;
  if (IntervalParser::ParseSimpleFormatFieldsFromCombinedStringNoExtraWhitespace(
          value_string, &count, &units)
      && IntervalParser::ParseYearMonthIntervalSimpleFormat(count, units, &literal)) {
    *value = TypedValue(literal);
    return true;
  }

  // Try complex format.
  if (IntervalParser::ParseYearMonthIntervalComplexFormat(value_string, &literal)) {
    *value = TypedValue(literal);
    return true;
  }

  return false;
}

}  // namespace quickstep
