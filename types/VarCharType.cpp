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

#include "types/VarCharType.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>

#include "types/NullCoercibilityCheckMacro.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/port/strnlen.hpp"
#include "utility/PtrMap.hpp"

#include "glog/logging.h"

using std::pair;
using std::size_t;
using std::strcmp;
using std::string;

namespace quickstep {

size_t VarCharType::estimateAverageByteLength() const {
  if (length_ > 160) {
    return 80;
  } else {
    return (length_ >> 1) + 1;
  }
}

bool VarCharType::isSafelyCoercibleFrom(const Type &original_type) const {
  QUICKSTEP_NULL_COERCIBILITY_CHECK();
  switch (original_type.getTypeID()) {
    case kChar:
      return original_type.maximumByteLength() <= length_;
    case kVarChar:
      return original_type.maximumByteLength() <= length_ + 1;
    default:
      return false;
  }
}

bool VarCharType::isSubsumedBy(const Type &original_type) const {
  if (Type::isSubsumedBy(original_type)) {
    return true;
  }

  if (original_type.getTypeID() == kVarChar) {
    if (maximum_byte_length_ <= original_type.maximumByteLength()) {
      return (!nullable_ || original_type.isNullable());
    }
  }

  return false;
}

string VarCharType::getName() const {
  string name("VarChar(");
  name.append(std::to_string(length_));
  name.push_back(')');
  if (nullable_) {
    name.append(" NULL");
  }
  return name;
}

std::string VarCharType::printValueToString(const TypedValue &value) const {
  DCHECK(!value.isNull());

  return std::string(static_cast<const char*>(value.getOutOfLineData()));
}

void VarCharType::printValueToFile(const TypedValue &value,
                                   FILE *file,
                                   const int padding) const {
  DCHECK(!value.isNull());

  std::fprintf(file,
               "%*s",
               static_cast<int>(padding),
               static_cast<const char*>(value.getOutOfLineData()));
}

bool VarCharType::parseValueFromString(const std::string &value_string,
                                       TypedValue *value) const {
  if (value_string.length() > length_) {
    return false;
  }

  *value = TypedValue(kVarChar, value_string.c_str(), value_string.length() + 1);
  value->ensureNotReference();
  return true;
}

TypedValue VarCharType::coerceValue(const TypedValue &original_value,
                                    const Type &original_type) const {
  DCHECK(isCoercibleFrom(original_type))
      << "Can't coerce value of Type " << original_type.getName()
      << " to Type " << getName();

  if (original_value.isNull()) {
    return makeNullValue();
  }

  const void *original_data = original_value.getOutOfLineData();
  const std::size_t original_data_size = original_value.getDataSize();
  const std::size_t original_strlen
      = (original_type.getTypeID() == kVarChar)
        ? original_data_size - 1
        : strnlen(static_cast<const char*>(original_data), original_data_size);
  const bool null_terminated = original_strlen < original_data_size;

  if ((original_strlen <= length_) && null_terminated) {
    TypedValue value_copy(original_value);
    value_copy.markType(kVarChar);
    return value_copy;
  }

  const std::size_t copy_strlen = original_strlen < length_
                                  ? original_strlen
                                  : length_;
  char *copied_str = static_cast<char*>(std::malloc(copy_strlen + 1));
  std::memcpy(copied_str, original_data, copy_strlen);
  copied_str[copy_strlen] = '\0';
  return TypedValue::CreateWithOwnedData(kVarChar,
                                         copied_str,
                                         copy_strlen + 1);
}

}  // namespace quickstep
