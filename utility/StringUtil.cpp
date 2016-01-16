/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#define __STDC_FORMAT_MACROS

#include "utility/StringUtil.hpp"

#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
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

}  // namespace quickstep
