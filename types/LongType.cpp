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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "types/LongType.hpp"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <string>

#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"

#include "glog/logging.h"

namespace quickstep {

std::string LongType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  return std::to_string(castValueToLiteral(value));
}

void LongType::printValueToFile(const UntypedLiteral *value,
                                FILE *file,
                                const int padding) const {
  DCHECK(value != nullptr);

  std::fprintf(file,
               "%*" PRId64,
               static_cast<int>(padding),
               castValueToLiteral(value));
}

bool LongType::parseValueFromString(const std::string &value_string,
                                    TypedValue *value) const {
  std::int64_t parsed_long;
  int read_chars;
  int matched = std::sscanf(value_string.c_str(),
                            "%" SCNd64 "%n",
                            &parsed_long,
                            &read_chars);
  if ((matched != 1)
      || (static_cast<std::string::size_type>(read_chars) != value_string.length())) {
    return false;
  }

  *value = TypedValue(parsed_long);
  return true;
}

}  // namespace quickstep
