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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "types/LongType.hpp"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <string>

#include "types/NullCoercibilityCheckMacro.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

namespace quickstep {

const TypeID LongType::kStaticTypeID = kLong;

bool LongType::isSafelyCoercibleFrom(const Type &original_type) const {
  QUICKSTEP_NULL_COERCIBILITY_CHECK();
  return QUICKSTEP_EQUALS_ANY_CONSTANT(original_type.getTypeID(),
                                       kInt, kLong);
}

TypedValue LongType::coerceValue(const TypedValue &original_value,
                                 const Type &original_type) const {
  DCHECK(isCoercibleFrom(original_type))
      << "Can't coerce value of Type " << original_type.getName()
      << " to Type " << getName();

  if (original_value.isNull()) {
    return makeNullValue();
  }

  switch (original_type.getTypeID()) {
    case kInt:
      return TypedValue(static_cast<std::int64_t>(original_value.getLiteral<int>()));
    case kLong:
      return original_value;
    case kFloat:
      return TypedValue(static_cast<std::int64_t>(original_value.getLiteral<float>()));
    case kDouble:
      return TypedValue(static_cast<std::int64_t>(original_value.getLiteral<double>()));
    default:
      LOG(FATAL) << "Attempted to coerce Type " << original_type.getName()
                 << " (not recognized as a numeric Type) to " << getName();
  }
}

std::string LongType::printValueToString(const TypedValue &value) const {
  DCHECK(!value.isNull());

  return std::to_string(value.getLiteral<std::int64_t>());
}

void LongType::printValueToFile(const TypedValue &value,
                                FILE *file,
                                const int padding) const {
  DCHECK(!value.isNull());

  std::fprintf(file,
               "%*" PRId64,
               static_cast<int>(padding),
               value.getLiteral<std::int64_t>());
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
