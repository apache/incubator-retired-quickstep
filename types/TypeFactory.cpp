/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/TypeFactory.hpp"

#include <cstddef>

#include "types/CharType.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DatetimeType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

const Type& TypeFactory::GetType(const TypeID id,
                                 const bool nullable) {
  switch (id) {
    case kInt:
      return IntType::Instance(nullable);
    case kLong:
      return LongType::Instance(nullable);
    case kFloat:
      return FloatType::Instance(nullable);
    case kDouble:
      return DoubleType::Instance(nullable);
    case kDatetime:
      return DatetimeType::Instance(nullable);
    case kDatetimeInterval:
      return DatetimeIntervalType::Instance(nullable);
    case kYearMonthInterval:
      return YearMonthIntervalType::Instance(nullable);
    case kNullType:
      DCHECK(nullable);
      return NullType::InstanceNullable();
    default:
      FATAL_ERROR("Called TypeFactory::GetType() for a type which requires "
                  " a length parameter without specifying one.");
  }
}

const Type& TypeFactory::GetType(const TypeID id,
                                 const std::size_t length,
                                 const bool nullable) {
  switch (id) {
    case kChar:
      return CharType::Instance(length, nullable);
    case kVarChar:
      return VarCharType::Instance(length, nullable);
    default:
      FATAL_ERROR("Provided a length parameter to TypeFactory::GetType() for "
                  "a type which does not take one.");
  }
}

bool TypeFactory::ProtoIsValid(const serialization::Type &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that the type_id is valid, and extensions if any.
  switch (proto.type_id()) {
    case serialization::Type::INT:
    case serialization::Type::LONG:
    case serialization::Type::FLOAT:
    case serialization::Type::DOUBLE:
    case serialization::Type::DATETIME:
    case serialization::Type::DATETIME_INTERVAL:
    case serialization::Type::YEAR_MONTH_INTERVAL:
      return true;
    case serialization::Type::CHAR:
      return proto.HasExtension(serialization::CharType::length);
    case serialization::Type::VAR_CHAR:
      return proto.HasExtension(serialization::VarCharType::length);
    case serialization::Type::NULL_TYPE:
      return proto.nullable();
    default:
      return false;
  }
}

const Type& TypeFactory::ReconstructFromProto(const serialization::Type &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create Type from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.type_id()) {
    case serialization::Type::INT:
      return IntType::Instance(proto.nullable());
    case serialization::Type::LONG:
      return LongType::Instance(proto.nullable());
    case serialization::Type::FLOAT:
      return FloatType::Instance(proto.nullable());
    case serialization::Type::DOUBLE:
      return DoubleType::Instance(proto.nullable());
    case serialization::Type::DATETIME:
      return DatetimeType::Instance(proto.nullable());
    case serialization::Type::DATETIME_INTERVAL:
      return DatetimeIntervalType::Instance(proto.nullable());
    case serialization::Type::YEAR_MONTH_INTERVAL:
      return YearMonthIntervalType::Instance(proto.nullable());
    case serialization::Type::CHAR:
      return CharType::InstanceFromProto(proto);
    case serialization::Type::VAR_CHAR:
      return VarCharType::InstanceFromProto(proto);
    case serialization::Type::NULL_TYPE:
      DCHECK(proto.nullable());
      return NullType::InstanceNullable();
    default:
      FATAL_ERROR("Unrecognized TypeID in TypeFactory::ReconstructFromProto");
  }
}

const Type* TypeFactory::GetMostSpecificType(const Type &first, const Type &second) {
  if (first.isSafelyCoercibleFrom(second)) {
    return &first;
  } else if (second.isSafelyCoercibleFrom(first)) {
    return &second;
  } else {
    return nullptr;
  }
}

const Type* TypeFactory::GetUnifyingType(const Type &first, const Type &second) {
  const Type *unifier = nullptr;
  if (first.isNullable() || second.isNullable()) {
    unifier = GetMostSpecificType(first.getNullableVersion(), second.getNullableVersion());
    if (unifier == nullptr) {
      if (((first.getTypeID() == kLong) && (second.getTypeID() == kFloat))
            || ((first.getTypeID() == kFloat) && (second.getTypeID() == kLong))) {
        unifier = &(DoubleType::Instance(true));
      }
    }
  } else {
    unifier = GetMostSpecificType(first, second);
    if (unifier == nullptr) {
      if (((first.getTypeID() == kLong) && (second.getTypeID() == kFloat))
            || ((first.getTypeID() == kFloat) && (second.getTypeID() == kLong))) {
        unifier = &(DoubleType::Instance(false));
      }
    }
  }

  return unifier;
}

}  // namespace quickstep
