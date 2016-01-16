/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/IntType.hpp"

#include <cstdint>
#include <cstdio>
#include <string>

#include "types/NullCoercibilityCheckMacro.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"

#include "glog/logging.h"

namespace quickstep {

const TypeID IntType::kStaticTypeID = kInt;

bool IntType::isSafelyCoercibleFrom(const Type &original_type) const {
  QUICKSTEP_NULL_COERCIBILITY_CHECK();
  return original_type.getTypeID() == kInt;
}

TypedValue IntType::coerceValue(const TypedValue &original_value,
                                const Type &original_type) const {
  DCHECK(isCoercibleFrom(original_type))
      << "Can't coerce value of Type " << original_type.getName()
      << " to Type " << getName();

  if (original_value.isNull()) {
    return makeNullValue();
  }

  switch (original_type.getTypeID()) {
    case kInt:
      return original_value;
    case kLong:
      return TypedValue(static_cast<int>(original_value.getLiteral<std::int64_t>()));
    case kFloat:
      return TypedValue(static_cast<int>(original_value.getLiteral<float>()));
    case kDouble:
      return TypedValue(static_cast<int>(original_value.getLiteral<double>()));
    default:
      LOG(FATAL) << "Attempted to coerce Type " << original_type.getName()
                 << " (not recognized as a numeric Type) to " << getName();
  }
}

std::string IntType::printValueToString(const TypedValue &value) const {
  DCHECK(!value.isNull());

  return std::to_string(value.getLiteral<int>());
}

void IntType::printValueToFile(const TypedValue &value,
                               FILE *file,
                               const int padding) const {
  DCHECK(!value.isNull());

  std::fprintf(file,
               "%*d",
               static_cast<int>(padding),
               value.getLiteral<int>());
}

bool IntType::parseValueFromString(const std::string &value_string,
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
