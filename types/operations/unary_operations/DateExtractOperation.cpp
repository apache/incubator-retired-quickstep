/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c), 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/unary_operations/DateExtractOperation.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
#include <utility>
#include <vector>

#include "storage/StorageBlockInfo.hpp"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

#include "catalog/CatalogTypedefs.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/DatetimeLit.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/Operation.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

using std::int64_t;

namespace quickstep {

template <DateExtractUnit unit, bool argument_nullable>
TypedValue DateExtractUncheckedOperator<unit, argument_nullable>::applyToTypedValue(
    const TypedValue &argument) const {
  if (argument_nullable && argument.isNull()) {
    return TypedValue(kLong);
  }

  return TypedValue(dateExtract(argument.getLiteral<DatetimeLit>()));
}

template <DateExtractUnit unit, bool argument_nullable>
TypedValue DateExtractUncheckedOperator<unit, argument_nullable>::applyToDataPtr(const void *argument) const {
  if (argument_nullable && argument == nullptr) {
    return TypedValue(kLong);
  }

  return TypedValue(dateExtract(*static_cast<const DatetimeLit*>(argument)));
}

template <DateExtractUnit unit, bool argument_nullable>
ColumnVector* DateExtractUncheckedOperator<unit, argument_nullable>::applyToColumnVector(
    const ColumnVector &argument) const {
  // Datetime are usable with NativeColumnVector, so 'argument' should always
  // be native.
  DCHECK(argument.isNative());

  const NativeColumnVector &native_argument = static_cast<const NativeColumnVector&>(argument);
  std::unique_ptr<NativeColumnVector> result(
      new NativeColumnVector(LongType::Instance(argument_nullable), native_argument.size()));

  for (std::size_t pos = 0;
       pos < native_argument.size();
       ++pos) {
    const DatetimeLit *datetime_arg =
        static_cast<const DatetimeLit*>(native_argument.getUntypedValue<argument_nullable>(pos));
    if (argument_nullable && (datetime_arg == nullptr)) {
      result->appendNullValue();
    } else {
      *static_cast<int64_t*>(result->getPtrForDirectWrite())
          = dateExtract(*datetime_arg);
    }
  }

  return result.release();
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
template <DateExtractUnit unit, bool argument_nullable>
ColumnVector* DateExtractUncheckedOperator<unit, argument_nullable>::applyToValueAccessor(
    ValueAccessor *accessor,
    const attribute_id argument_attr_id) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
    std::unique_ptr<NativeColumnVector> result(
        new NativeColumnVector(LongType::Instance(argument_nullable), accessor->getNumTuples()));
    accessor->beginIteration();
    while (accessor->next()) {
      const DatetimeLit *datetime_arg =
          static_cast<const DatetimeLit*>(
              accessor->template getUntypedValue<argument_nullable>(argument_attr_id));
      if (argument_nullable && (datetime_arg == nullptr)) {
        result->appendNullValue();
      } else {
        *static_cast<int64_t*>(result->getPtrForDirectWrite())
            = this->dateExtract(*datetime_arg);
      }
    }
    return result.release();
  });
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN
template <DateExtractUnit unit, bool argument_nullable>
ColumnVector* DateExtractUncheckedOperator<unit, argument_nullable>::applyToValueAccessorForJoin(
    ValueAccessor *accessor,
    const bool use_left_relation,
    const attribute_id argument_attr_id,
    const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const {
  std::unique_ptr<NativeColumnVector> result(
      new NativeColumnVector(LongType::Instance(argument_nullable), joined_tuple_ids.size()));
  return InvokeOnValueAccessorNotAdapter(
      accessor,
      [&](auto *accessor) -> ColumnVector* {  // NOLINT(build/c++11)
    for (const std::pair<tuple_id, tuple_id> &joined_pair : joined_tuple_ids) {
      const DatetimeLit *datetime_arg =
          static_cast<const DatetimeLit*>(
              accessor->template getUntypedValueAtAbsolutePosition<argument_nullable>(
                  argument_attr_id,
                  use_left_relation ? joined_pair.first : joined_pair.second));
      if (argument_nullable && (datetime_arg == nullptr)) {
        result->appendNullValue();
      } else {
        *static_cast<int64_t*>(result->getPtrForDirectWrite())
            = this->dateExtract(*datetime_arg);
      }
    }
    return result.release();
  });
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_JOIN

template <DateExtractUnit unit, bool argument_nullable>
inline int64_t DateExtractUncheckedOperator<unit, argument_nullable>::dateExtract(const DatetimeLit &argument) const {
  switch (unit) {
    case DateExtractUnit::kYear:
      return argument.yearField();
    case DateExtractUnit::kMonth:
      return argument.monthField();
    case DateExtractUnit::kDay:
      return argument.dayField();
    case DateExtractUnit::kHour:
      return argument.hourField();
    case DateExtractUnit::kMinute:
      return argument.minuteField();
    case DateExtractUnit::kSecond:
      return argument.secondField();
    default:
      FATAL_ERROR("Unsupported DateExtractUnit in DateExtractUncheckedOperator::dateExtract.");
  }
}

const DateExtractOperation& DateExtractOperation::Instance(const DateExtractUnit unit) {
  switch (unit) {
    case DateExtractUnit::kYear: {
      static DateExtractOperation instance(DateExtractUnit::kYear);
      return instance;
    }
    case DateExtractUnit::kMonth: {
      static DateExtractOperation instance(DateExtractUnit::kMonth);
      return instance;
    }
    case DateExtractUnit::kDay: {
      static DateExtractOperation instance(DateExtractUnit::kDay);
      return instance;
    }
    case DateExtractUnit::kHour: {
      static DateExtractOperation instance(DateExtractUnit::kHour);
      return instance;
    }
    case DateExtractUnit::kMinute: {
      static DateExtractOperation instance(DateExtractUnit::kMinute);
      return instance;
    }
    case DateExtractUnit::kSecond: {
      static DateExtractOperation instance(DateExtractUnit::kSecond);
      return instance;
    }
    default: {
      FATAL_ERROR("Unsupported DateExtractUnit in DateExtractOperation::Instance.");
    }
  }
}

serialization::UnaryOperation DateExtractOperation::getProto() const {
  serialization::UnaryOperation proto;
  proto.set_operation_id(serialization::UnaryOperation::DATE_EXTRACT);

  switch (unit_) {
    case DateExtractUnit::kYear:
      proto.SetExtension(serialization::DateExtractOperation::unit, serialization::DateExtractOperation::YEAR);
      break;
    case DateExtractUnit::kMonth:
      proto.SetExtension(serialization::DateExtractOperation::unit, serialization::DateExtractOperation::MONTH);
      break;
    case DateExtractUnit::kDay:
      proto.SetExtension(serialization::DateExtractOperation::unit, serialization::DateExtractOperation::DAY);
      break;
    case DateExtractUnit::kHour:
      proto.SetExtension(serialization::DateExtractOperation::unit, serialization::DateExtractOperation::HOUR);
      break;
    case DateExtractUnit::kMinute:
      proto.SetExtension(serialization::DateExtractOperation::unit, serialization::DateExtractOperation::MINUTE);
      break;
    case DateExtractUnit::kSecond:
      proto.SetExtension(serialization::DateExtractOperation::unit, serialization::DateExtractOperation::SECOND);
      break;
    default:
      FATAL_ERROR("Unsupported DateExtractUnit in DateExtractOperation::getProto.");
  }

  return proto;
}

std::string DateExtractOperation::getName() const {
  std::string name(kUnaryOperationNames[static_cast<std::size_t>(operation_id_)]);
  name.push_back('(');
  switch (unit_) {
    case DateExtractUnit::kYear:
      name.append("YEAR)");
      break;
    case DateExtractUnit::kMonth:
      name.append("MONTH)");
      break;
    case DateExtractUnit::kDay:
      name.append("DAY)");
      break;
    case DateExtractUnit::kHour:
      name.append("HOUR)");
      break;
    case DateExtractUnit::kMinute:
      name.append("MINUTE)");
      break;
    case DateExtractUnit::kSecond:
      name.append("SECOND)");
      break;
    default:
      name.append("UNKNOWN)");
      break;
  }
  return name;
}

const Type* DateExtractOperation::pushDownTypeHint(const Type *type_hint) const {
  if (type_hint == nullptr) {
    return nullptr;
  }

  if (type_hint->getTypeID() == kLong) {
    return &TypeFactory::GetType(kDatetime, type_hint->isNullable());
  } else {
    return nullptr;
  }
}

TypedValue DateExtractOperation::applyToChecked(const TypedValue &argument,
                                                const Type &argument_type) const {
  if ((argument.getTypeID() != TypeID::kDatetime)
      || (argument_type.getTypeID() != TypeID::kDatetime)) {
    LOG(FATAL) << "UnaryOperation " << getName() << " is only applicable to Type "
               << kTypeNames[TypeID::kDatetime] << ", but applyToChecked() was "
               << "called with 'argument' of Type " << kTypeNames[argument.getTypeID()]
               << " and explicit 'argument_type' parameter of "
               << argument_type.getName();
  }

  if (argument.isNull()) {
    return TypedValue(kLong);
  }

  switch (unit_) {
    case DateExtractUnit::kYear:
      return TypedValue(argument.getLiteral<DatetimeLit>().yearField());
    case DateExtractUnit::kMonth:
      return TypedValue(argument.getLiteral<DatetimeLit>().monthField());
    case DateExtractUnit::kDay:
      return TypedValue(argument.getLiteral<DatetimeLit>().dayField());
    case DateExtractUnit::kHour:
      return TypedValue(argument.getLiteral<DatetimeLit>().hourField());
    case DateExtractUnit::kMinute:
      return TypedValue(argument.getLiteral<DatetimeLit>().minuteField());
    case DateExtractUnit::kSecond:
      return TypedValue(argument.getLiteral<DatetimeLit>().secondField());
    default: {
      LOG(FATAL) << "Unsupported DateExtractUnit in DateExtractOperation::applyToChecked().";
    }
  }
}

UncheckedUnaryOperator* DateExtractOperation::makeUncheckedUnaryOperatorForTypeHelper(const Type &type) const {
  switch (unit_) {
    case DateExtractUnit::kYear:
      if (type.isNullable()) {
        return new DateExtractUncheckedOperator<DateExtractUnit::kYear, true>();
      } else {
        return new DateExtractUncheckedOperator<DateExtractUnit::kYear, false>();
      }
    case DateExtractUnit::kMonth:
      if (type.isNullable()) {
        return new DateExtractUncheckedOperator<DateExtractUnit::kMonth, true>();
      } else {
        return new DateExtractUncheckedOperator<DateExtractUnit::kMonth, false>();
      }
    case DateExtractUnit::kDay:
      if (type.isNullable()) {
        return new DateExtractUncheckedOperator<DateExtractUnit::kDay, true>();
      } else {
        return new DateExtractUncheckedOperator<DateExtractUnit::kDay, false>();
      }
    case DateExtractUnit::kHour:
      if (type.isNullable()) {
        return new DateExtractUncheckedOperator<DateExtractUnit::kHour, true>();
      } else {
        return new DateExtractUncheckedOperator<DateExtractUnit::kHour, false>();
      }
    case DateExtractUnit::kMinute:
      if (type.isNullable()) {
        return new DateExtractUncheckedOperator<DateExtractUnit::kMinute, true>();
      } else {
        return new DateExtractUncheckedOperator<DateExtractUnit::kMinute, false>();
      }
    case DateExtractUnit::kSecond:
      if (type.isNullable()) {
        return new DateExtractUncheckedOperator<DateExtractUnit::kSecond, true>();
      } else {
        return new DateExtractUncheckedOperator<DateExtractUnit::kSecond, false>();
      }
    default:
      FATAL_ERROR("Unsupported DateExtractUnit in DateExtractOperation::makeUncheckedUnaryOperatorForTypeHelper.");
  }
}

}  // namespace quickstep
