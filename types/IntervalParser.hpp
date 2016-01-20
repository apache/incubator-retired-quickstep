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

#ifndef QUICKSTEP_TYPES_INTERVAL_PARSER_HPP_
#define QUICKSTEP_TYPES_INTERVAL_PARSER_HPP_

#include <cstdint>
#include <string>

#include "utility/Macros.hpp"

namespace quickstep {

struct DatetimeIntervalLit;
struct YearMonthIntervalLit;

/**
 * @brief All-static utility class with methods for parsing INTERVAL values
 *        from string representations.
 *
 * INTERVAL literals can have two formats. The "simple" format is an integer
 * followed by a string indicating units (e.g. "1 second", "2 days",
 * "3 years"). The "complex" format is the same as that printed by
 * DatetimeIntervalType::printValueToString() or
 * YearMonthIntervalType::printValueToString() (e.g. "1 day 00:39:35.244090"
 * for DatetimeInterval, or "2 years 1 mon" for YearMonthInterval).
 **/
class IntervalParser {
 public:
  /**
   * @brief Finish parsing a DatetimeIntervalLit from a simple format
   *        representation.
   *
   * @param count The numeric portion of the interval specification, already
   *        parsed as int64_t.
   * @param units_lowercase A lowercase string representing the units that
   *        count should be interpreted as. Valid options are "microsecond",
   *        "millisecond", "second", "minute", "hour", "day", and "week", as
   *        well as plural forms and SI abbreviations.
   * @param interval A pointer to a DatetimeIntervalLit which will be
   *        overwritten with the parsed DatetimeInterval value on success.
   * @return true on success, false if units_lowercase was not valid.
   **/
  static bool ParseDatetimeIntervalSimpleFormat(const std::int64_t count,
                                                const std::string &units_lowercase,
                                                DatetimeIntervalLit *interval);

  /**
   * @brief Parse a DatetimeIntervalLit from a complex format representation.
   *
   * @param interval_string A description of a DatetimeInterval in complex
   *        format.
   * @param interval A pointer to a DatetimeIntervalLit which will be
   *        overwritten with the parsed DatetimeInterval value on success.
   * @return true on success, false if interval_string was not in valid format.
   **/
  static bool ParseDatetimeIntervalComplexFormat(const std::string &interval_string,
                                                 DatetimeIntervalLit *interval);

  /**
   * @brief Finish parsing a YearMonthIntervalLit from a simple format
   *        representation.
   *
   * @param count The numeric portion of the interval specification, already
   *        parsed as int64_t.
   * @param units_lowercase A lowercase string representing the units that
   *        count should be interpreted as. Valid options are "month", "year",
   *        "decade", "century", and "millenium", as well as plural forms and
   *        SI abbreviations.
   * @param interval A pointer to a YearMonthIntervalLit which will be
   *        overwritten with the parsed YearMonthInterval value on success.
   * @return true on success, false if units_lowercase was not valid.
   **/
  static bool ParseYearMonthIntervalSimpleFormat(const std::int64_t count,
                                                 const std::string &units_lowercase,
                                                 YearMonthIntervalLit *interval);

  /**
   * @brief Parse a YearMonthIntervalLit from a complex format representation.
   *
   * @param interval_string A description of a YearMonthInterval in complex
   *        format.
   * @param interval A pointer to a YearMonthIntervalLit which will be
   *        overwritten with the parsed YearMonthInterval value on success.
   * @return true on success, false if interval_string was not in valid format.
   **/
  static bool ParseYearMonthIntervalComplexFormat(const std::string &interval_string,
                                                  YearMonthIntervalLit *interval);

  /**
   * @brief Extract the count and units fields from a combined string
   *        describing an interval.
   *
   * @param interval_string A single combined string describing an interval.
   *        Valid formats are all of the form "count units", and single quotes
   *        may appear around the entire string, just the count portion, or be
   *        omitted entirely. Note that quotes around the units only are NOT a
   *        valid format.
   * @param count A pointer to an int64_t that will be overwritten with the
   *        parsed count portion of interval_string on success.
   * @param units_lowercase A pointer to a string that will be overwritten with
   *        the units portion of interval_string, converted to lowercase, on
   *        success. Note that this method does not check that units_lowercase
   *        specifies a known unit.
   * @return true on successful parse, false if interval_string was not in the
   *         expected format.
   **/
  static bool ParseSimpleFormatFieldsFromCombinedString(
      const std::string &interval_string,
      std::int64_t *count,
      std::string *units_lowercase);

  /**
   * @brief A stricter version of ParseSimpleFormatFieldsFromCombinedString()
   *        that does not allow extra trailing whitespace, nor extra whitespace
   *        inside quotes.
   *
   * @param interval_string A single combined string describing an interval.
   *        Valid formats are all of the form "count units", and single quotes
   *        may appear around the entire string, just the count portion, or be
   *        omitted entirely. Note that quotes around the units only are NOT a
   *        valid format.
   * @param count A pointer to an int64_t that will be overwritten with the
   *        parsed count portion of interval_string on success.
   * @param units_lowercase A pointer to a string that will be overwritten with
   *        the units portion of interval_string, converted to lowercase, on
   *        success. Note that this method does not check that units_lowercase
   *        specifies a known unit.
   * @return true on successful parse, false if interval_string was not in the
   *         expected format.
   **/
  static bool ParseSimpleFormatFieldsFromCombinedStringNoExtraWhitespace(
      const std::string &interval_string,
      std::int64_t *count,
      std::string *units_lowercase);

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  IntervalParser();

  DISALLOW_COPY_AND_ASSIGN(IntervalParser);
};

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_INTERVAL_PARSER_HPP_
