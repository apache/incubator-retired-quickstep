/**
 *   Licensed to the Apache Software Foundation (ASF) under one
 *   or more contributor license agreements.  See the NOTICE file
 *   distributed with this work for additional information
 *   regarding copyright ownership.  The ASF licenses this file
 *   to you under the Apache License, Version 2.0 (the
 *   "License"); you may not use this file except in compliance
 *   with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing,
 *   software distributed under the License is distributed on an
 *   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *   KIND, either express or implied.  See the License for the
 *   specific language governing permissions and limitations
 *   under the License.
 *   limitations under the License.
 **/

#include "types/DecimalType.hpp"

#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <string>
#include <sstream>

#include "types/DecimalLit.hpp"
#include "types/NullCoercibilityCheckMacro.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

namespace quickstep {

class Type;

const TypeID DecimalType::kStaticTypeID = kDecimal;

bool DecimalType::isSafelyCoercibleFrom(const Type &original_type) const {
  QUICKSTEP_NULL_COERCIBILITY_CHECK();
  return QUICKSTEP_EQUALS_ANY_CONSTANT(original_type.getTypeID(),
                                       kInt, kDecimal);
}

TypedValue DecimalType::coerceValue(const TypedValue &original_value,
                                    const Type &original_type) const {
  DCHECK(isCoercibleFrom(original_type))
      << "Can't coerce value of Type " << original_type.getName()
      << " to Type " << getName();

  if (original_value.isNull()) {
    return makeNullValue();
  }

  switch (original_type.getTypeID()) {
    case kInt:
      return TypedValue(DecimalLit(original_value.getLiteral<int>()));
    case kLong:
      return TypedValue(DecimalLit(original_value.getLiteral<std::int64_t>()));
    case kFloat:
      return TypedValue(DecimalLit(original_value.getLiteral<float>()));
    case kDouble:
      return original_value;
    default:
      LOG(FATAL) << "Attempted to coerce Type " << original_type.getName()
                 << " (not recognized as a numeric Type) to " << getName();
  }
}

std::string DecimalType::printValueToString(const TypedValue &value) const {
  DCHECK(!value.isNull());

  DecimalLit decimal = value.getLiteral<DecimalLit>();
  std::stringstream ss;
  ss << decimal.getIntegerPart() << "."
     << std::setfill('0') << std::setw(DecimalLit::kPrecisionWidth)
     <<  decimal.getFractionalPart();
  return ss.str();
}

void DecimalType::printValueToFile(const TypedValue &value,
                                   FILE *file,
                                   const int padding) const {
  DCHECK(!value.isNull());

  DecimalLit decimal = value.getLiteral<DecimalLit>();

  std::fprintf(file, "%*ld.%0*lu",
               static_cast<int>(padding -
                                (DecimalLit::kPrecisionWidth
                                 + 1 /* Less one space for point. */)),
               decimal.getIntegerPart(),
               static_cast<int>(DecimalLit::kPrecisionWidth),
               decimal.getFractionalPart());
}

bool DecimalType::parseValueFromString(const std::string &value_string,
                                       TypedValue *value) const {
  double parsed_double;
  int read_chars;

  int matched = std::sscanf(value_string.c_str(),
                            "%lf%n",
                            &parsed_double,
                            &read_chars);

  if (matched != 1 || read_chars != static_cast<int>(value_string.length()))  {
    return false;
  }

  *value = TypedValue(DecimalLit(parsed_double));
  return true;
}

}  // namespace quickstep
