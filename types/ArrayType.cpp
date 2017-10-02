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

#include "types/ArrayType.hpp"

#include <string>

#include "types/TypeID.hpp"

#include "glog/logging.h"

namespace quickstep {

std::string ArrayType::printValueToString(const UntypedLiteral *value) const {
  DCHECK(value != nullptr);

  const std::vector<UntypedLiteral*> &literals = castValueToLiteral(value);
  std::string ret = "{";
  if (!literals.empty()) {
    ret.append(element_type_.printValueToString(literals.front()));
    for (std::size_t i = 1; i < literals.size(); ++i) {
      ret.append(", ");
      ret.append(element_type_.printValueToString(literals.at(i)));
    }
  }
  ret.append("}");
  return ret;
}

}  // namespace quickstep
