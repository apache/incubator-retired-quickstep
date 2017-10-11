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

#include "types/TextType.hpp"

#include <cstddef>
#include <string>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "utility/EqualsAnyConstant.hpp"

namespace quickstep {

bool TextType::isSafelyCoercibleFrom(const Type &original_type) const {
  return QUICKSTEP_EQUALS_ANY_CONSTANT(original_type.getTypeID(),
                                       kChar, kVarChar, kText);
}

bool TextType::checkValuesEqual(const UntypedLiteral *lhs,
                                const UntypedLiteral *rhs,
                                const Type &rhs_type) const {
  if (!equals(rhs_type)) {
    return false;
  }
  return castValueToLiteral(lhs) == castValueToLiteral(rhs);
}

TypedValue TextType::marshallValue(const UntypedLiteral *value) const {
  const std::string &str = castValueToLiteral(value);
  return TypedValue(kText, str.c_str(), str.length()).ensureNotReference();
}

UntypedLiteral* TextType::unmarshallValue(const void *data,
                                          const std::size_t length) const {
  return new std::string(static_cast<const char*>(data), length);
}

std::string TextType::printValueToString(const UntypedLiteral *value) const {
  return castValueToLiteral(value);
}

}  // namespace quickstep
