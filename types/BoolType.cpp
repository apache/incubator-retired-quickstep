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

#include "types/BoolType.hpp"

#include <cstdio>
#include <string>

#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

std::string BoolType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  return castValueToLiteral(value) ? "true" : "false";
}

void BoolType::printValueToFile(const UntypedLiteral *value,
                                FILE *file,
                                const int padding) const {
  DCHECK(value != nullptr);

  std::fprintf(file,
               "%*s",
               static_cast<int>(padding),
               castValueToLiteral(value) ? "true" : "false");
}

bool BoolType::parseTypedValueFromString(const std::string &value_string,
                                         TypedValue *value) const {
  const std::string lo_value = ToLower(value_string);
  if (lo_value == "true") {
    *value = TypedValue(true);
    return true;
  } else if (lo_value == "false") {
    *value = TypedValue(false);
    return true;
  }
  return false;
}

}  // namespace quickstep
