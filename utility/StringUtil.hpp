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

#ifndef QUICKSTEP_UTILITY_STRING_UTIL_HPP_
#define QUICKSTEP_UTILITY_STRING_UTIL_HPP_

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Convert a string \p str to lower case.
 *
 * @param str The string to be converted.
 * @return The converted string with all lower case characters bing converted to upper case characters.
 */
extern std::string ToLower(const std::string &str);

/**
 * @brief Converts special characters to escape characters.
 *
 * @param text The string to be unescaped.
 * @return Unescaped string.
 */
extern std::string EscapeSpecialChars(const std::string &text);

/**
 * @brief Escape quotes (i.e. ' or ") in 'str'.
 * @return Escaped string.
 */
extern std::string EscapeQuotes(const std::string &str, const char quote);

/**
 * @brief Join all objects in a iterable container into a single string. The object
 *        must have implemented the operator<< overloading with std::stringstream.
 *
 * @param container The iterable container of objects.
 * @param separator A string to separate each object.
 */
template <typename ContainerType>
std::string JoinToString(const ContainerType &container,
                         const std::string &separator) {
  std::ostringstream oss;
  bool is_first = true;
  for (const auto &item : container) {
    if (is_first) {
      is_first = false;
    } else {
      oss << separator;
    }
    oss << item;
  }
  return oss.str();
}

/**
 * @brief Parse a string of base-10 integers separated by delimiter characters
 *        into a vector of ints.
 *
 * @param int_string The input string, expected to be some number of base-10
 *        integers separated by delimiter.
 * @param delimiter The delimiter character that separates the integers in
 *        int_string.
 * @param parsed_output A vector of ints that where the integers parsed from
 *        int_string will be stored.
 * @return true on success, false if parsing failed because int_string was not
 *         in the expected format.
 **/
extern bool ParseIntString(const std::string &int_string,
                           const char delimiter,
                           std::vector<int> *parsed_output);

/**
 * @brief Convert a integral value to a zero-padded string.
 *
 * @param val The integral value to be stringified.
 * @param pad_width The length of the padded string.
 * @return The zero-padded string from the given integral value.
 */
extern std::string ToZeroPaddedString(std::uint64_t val, int pad_width);


/**
 * @brief Parse double with significant digits.
 *
 * @param val The value that will be parsed.
 * @param significant_digit The number of non-zero digits that will be parsed
 *        in the fraction part of double.
 * @return Corresponding string representation of parsed double.
 *         Ex:
 *         DoubleToStringWithSignificantDigits(3.124355123, 3)
 *          => 3.124
 *         DoubleToStringWithSignificantDigits(0.00000000323411, 3)
 *         => 0.00000000323
 **/
extern std::string
DoubleToStringWithSignificantDigits(double val,
                                    std::uint64_t significant_digits);

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_STRING_UTIL_HPP_ */
