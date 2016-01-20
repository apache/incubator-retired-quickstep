/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include "types/CharType.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>

#include "types/NullCoercibilityCheckMacro.hpp"
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

template <bool nullable_internal>
const CharType& CharType::InstanceInternal(const std::size_t length) {
  static PtrMap<size_t, CharType> instance_map;
  PtrMap<size_t, CharType>::iterator imit = instance_map.find(length);
  if (imit == instance_map.end()) {
    imit = instance_map.insert(length, new CharType(length, nullable_internal)).first;
  }
  return *(imit->second);
}

const CharType& CharType::InstanceNonNullable(const std::size_t length) {
  return InstanceInternal<false>(length);
}

const CharType& CharType::InstanceNullable(const std::size_t length) {
  return InstanceInternal<true>(length);
}

const CharType& CharType::InstanceFromProto(const serialization::Type &proto) {
  return Instance(proto.GetExtension(serialization::CharType::length), proto.nullable());
}

serialization::Type CharType::getProto() const {
  serialization::Type proto;
  proto.set_type_id(serialization::Type::CHAR);

  proto.set_nullable(nullable_);

  proto.SetExtension(serialization::CharType::length, length_);
  return proto;
}

bool CharType::isSafelyCoercibleFrom(const Type &original_type) const {
  QUICKSTEP_NULL_COERCIBILITY_CHECK();

  switch (original_type.getTypeID()) {
    case kChar:
      return original_type.maximumByteLength() <= length_;
    case kVarChar:
      return original_type.maximumByteLength() - 1 <= length_;
    default:
      return false;
  }
}

string CharType::getName() const {
  string name("Char(");
  name.append(std::to_string(length_));
  name.push_back(')');
  if (nullable_) {
    name.append(" NULL");
  }
  return name;
}

std::string CharType::printValueToString(const TypedValue &value) const {
  DCHECK(!value.isNull());

  const char *cstr = static_cast<const char*>(value.getOutOfLineData());
  return std::string(cstr, strnlen(cstr, length_));
}

void CharType::printValueToFile(const TypedValue &value,
                                FILE *file,
                                const int padding) const {
  DCHECK(!value.isNull());
  DCHECK_EQ(length_, static_cast<decltype(length_)>(static_cast<int>(length_)))
      << "Can not convert CHAR Type's maximum length " << length_
      << " to int for fprintf()";

  std::fprintf(file,
               "%*.*s",
               padding,
               static_cast<int>(length_),
               static_cast<const char*>(value.getOutOfLineData()));
}

bool CharType::parseValueFromString(const std::string &value_string,
                                    TypedValue *value) const {
  if (value_string.length() > length_) {
    return false;
  }

  *value = TypedValue(kChar,
                      value_string.c_str(),
                      value_string.length() == length_
                          ? value_string.length()
                          : value_string.length() + 1);
  value->ensureNotReference();
  return true;
}

TypedValue CharType::coerceValue(const TypedValue &original_value,
                                 const Type &original_type) const {
  DCHECK(isCoercibleFrom(original_type))
      << "Can't coerce value of Type " << original_type.getName()
      << " to Type " << getName();

  if (original_value.isNull()) {
    return makeNullValue();
  }

  const void *original_data = original_value.getOutOfLineData();
  const std::size_t original_data_size = original_value.getDataSize();

  // VARCHAR always has a null-terminator. CHAR(X) has a null-terminator when
  // string's length is less than X.
  const bool null_terminated
      = (original_type.getTypeID() == kVarChar)
        || (original_data_size < original_type.maximumByteLength())
        || (std::memchr(original_data, '\0', original_data_size) != nullptr);

  if (original_data_size <= length_) {
    if (null_terminated || (original_data_size == length_)) {
      TypedValue value_copy(original_value);
      value_copy.markType(kChar);
      return value_copy;
    } else {
      // Need to make a new NULL-terminated copy of the string.
      char *null_terminated_str = static_cast<char*>(std::malloc(original_data_size + 1));
      std::memcpy(null_terminated_str, original_data, original_data_size);
      null_terminated_str[original_data_size] = '\0';
      return TypedValue::CreateWithOwnedData(kChar,
                                             null_terminated_str,
                                             original_data_size + 1);
    }
  } else {
    // Need to truncate.
    if (original_value.ownsOutOfLineData()) {
      char *truncated_str = static_cast<char*>(std::malloc(length_));
      std::memcpy(truncated_str, original_data, length_);
      return TypedValue::CreateWithOwnedData(kChar, truncated_str, length_);
    } else {
      // Original is a reference, so we will just make a shorter reference.
      return TypedValue(kChar, original_data, length_);
    }
  }
}

}  // namespace quickstep
