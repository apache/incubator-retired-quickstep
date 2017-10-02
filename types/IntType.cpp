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

#include "types/IntType.hpp"

#include <cstdio>
#include <string>

#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"

#include "glog/logging.h"

namespace quickstep {

std::string IntType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  return std::to_string(castValueToLiteral(value));
}

void IntType::printValueToFile(const UntypedLiteral *value,
                               FILE *file,
                               const int padding) const {
  DCHECK(value != nullptr);

  std::fprintf(file,
               "%*d",
               static_cast<int>(padding),
               castValueToLiteral(value));
}

bool IntType::parseTypedValueFromString(const std::string &value_string,
                                        TypedValue *value) const {
  int parsed_int;
  int read_chars;
  int matched = std::sscanf(value_string.c_str(),
                            "%d%n",
                            &parsed_int,
                            &read_chars);
  if ((matched != 1)
      || (static_cast<std::string::size_type>(read_chars) != value_string.length())) {
    return false;
  }

  *value = TypedValue(parsed_int);
  return true;
}

}  // namespace quickstep
