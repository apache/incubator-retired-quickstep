/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/Type.hpp"

#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

serialization::Type Type::getProto() const {
  serialization::Type proto;
  switch (type_id_) {
    case kInt:
      proto.set_type_id(serialization::Type::INT);
      break;
    case kLong:
      proto.set_type_id(serialization::Type::LONG);
      break;
    case kFloat:
      proto.set_type_id(serialization::Type::FLOAT);
      break;
    case kDouble:
      proto.set_type_id(serialization::Type::DOUBLE);
      break;
    case kDatetime:
      proto.set_type_id(serialization::Type::DATETIME);
      break;
    case kDatetimeInterval:
      proto.set_type_id(serialization::Type::DATETIME_INTERVAL);
      break;
    case kYearMonthInterval:
      proto.set_type_id(serialization::Type::YEAR_MONTH_INTERVAL);
      break;
    case kNullType:
      proto.set_type_id(serialization::Type::NULL_TYPE);
      break;
    default:
      FATAL_ERROR("Unrecognized TypeID in Type::getProto");
  }

  proto.set_nullable(nullable_);

  return proto;
}

TypedValue Type::coerceValue(const TypedValue &original_value,
                             const Type &original_type) const {
  DCHECK(isCoercibleFrom(original_type))
      << "Can't coerce value of Type " << original_type.getName()
      << " to Type " << getName();

  if (original_type.getTypeID() == kNullType) {
    return makeNullValue();
  }

  DCHECK(equals(original_type) || equals(original_type.getNullableVersion()))
      << "Base version of Type::coerceValue() called for a non-trivial "
      << "coercion from Type " << original_type.getName()
      << " to Type " << getName();

  return original_value;
}

bool AsciiStringSuperType::isCoercibleFrom(const Type &original_type) const {
  if (original_type.isNullable() && !nullable_) {
    return false;
  }
  return (original_type.getSuperTypeID() == kAsciiString)
         || (original_type.getTypeID() == kNullType);
}

}  // namespace quickstep
