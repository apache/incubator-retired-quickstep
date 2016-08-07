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

#define __STDC_FORMAT_MACROS

#include "utility/StringUtil.hpp"

#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "glog/logging.h"

// NetBSD's libc has snprintf, but it doesn't show up in the std namespace
// for c++.
#ifndef __NetBSD__
using std::snprintf;
#endif

using std::string;

namespace quickstep {

std::string ToLower(const std::string& str) {
  std::string lower_str(str.size(), ' ');
  std::transform(str.begin(), str.end(), lower_str.begin(), tolower);
  return lower_str;
}

std::string EscapeSpecialChars(const std::string& text) {
  std::string new_text;
  for (const char& c : text) {
    switch (c) {
      case '\'':
        new_text.append("\\\'");
        break;
      case '\"':
        new_text.append("\\\"");
        break;
      case '\?':
        new_text.append("\\\?");
        break;
      case '\t':
        new_text.append("\\t");
        break;
      case '\a':
        new_text.append("\\a");
        break;
      case '\b':
        new_text.append("\\b");
        break;
      case '\f':
        new_text.append("\\f");
        break;
      case '\n':
        new_text.append("\\n");
        break;
      case '\r':
        new_text.append("\\r");
        break;
      case '\v':
        new_text.append("\\v");
        break;
      default:
        new_text += c;
    }
  }
  return new_text;
}

bool ParseIntString(const std::string &int_string,
                    const char delimiter,
                    std::vector<int> *parsed_output) {
  std::vector<int>::size_type original_size = parsed_output->size();

  std::size_t pos = 0;
  while (pos < int_string.size()) {
    char *endptr = nullptr;
    int element = std::strtol(int_string.c_str() + pos, &endptr, 10);
    if ((endptr > int_string.c_str() + pos)
        && ((*endptr == delimiter) || (*endptr == '\0'))) {
      parsed_output->push_back(element);
      pos = endptr - int_string.c_str() + 1;
    } else {
      parsed_output->resize(original_size);
      return false;
    }
  }

  return true;
}

string ToZeroPaddedString(std::uint64_t val, int pad_width) {
  DCHECK_GT(pad_width, 0);
  const std::size_t str_len = pad_width + 1;
  char *str = new char[str_len]();

  int bytes = snprintf(str,
                       str_len,
                       "%0*" PRIu64,
                       pad_width,
                       val);
  DCHECK_EQ(pad_width, bytes);

  string result(str, pad_width);
  delete[] str;

  return result;
}

std::string
DoubleToStringWithSignificantDigits(double val,
                                    std::uint64_t significant_digits) {
  std::uint64_t precision_needed =
      (val >= 1.0) ? significant_digits
                   : significant_digits + -(std::trunc(std::log10(val)));
  std::stringstream string_buffer;
  string_buffer << std::fixed << std::setprecision(precision_needed) << val;
  return string_buffer.str();
}

}  // namespace quickstep
