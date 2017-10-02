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

#include "types/FloatType.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>

#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"

#include "glog/logging.h"

// NetBSD's libc has snprintf, but it doesn't show up in the std namespace for
// C++.
#ifndef __NetBSD__
using std::snprintf;
#endif

namespace quickstep {

std::string FloatType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  char printbuffer[kPrintWidth + 1];
  int written = snprintf(printbuffer,
                         sizeof(printbuffer),
                         "%.*g",
                         std::numeric_limits<float>::max_digits10,
                         castValueToLiteral(value));
  DCHECK_GE(written, 0) << "snprintf() encountered an encoding error";
  DCHECK_LT(static_cast<std::size_t>(written), sizeof(printbuffer))
      << "snprintf() would have written a string of length " << written
      << " (plus null-terminator), but buffer size is only " << sizeof(printbuffer);
  return std::string(printbuffer);
}

void FloatType::printValueToFile(const UntypedLiteral *value,
                                 FILE *file,
                                 const int padding) const {
  DCHECK(value != nullptr);

  std::fprintf(file,
               "%*.*g",
               static_cast<int>(padding),
               std::numeric_limits<float>::max_digits10,
               castValueToLiteral(value));
}

bool FloatType::parseValueFromString(const std::string &value_string,
                                     TypedValue *value) const {
  float parsed_float;
  int read_chars;
  int matched = std::sscanf(value_string.c_str(),
                            "%f%n",
                            &parsed_float,
                            &read_chars);
  if ((matched != 1)
      || (static_cast<std::string::size_type>(read_chars) != value_string.length())) {
    return false;
  }

  *value = TypedValue(parsed_float);
  return true;
}

}  // namespace quickstep
