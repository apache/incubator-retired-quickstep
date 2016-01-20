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

#include "types/operations/unary_operations/ArithmeticUnaryOperations.hpp"

#include <string>

#include "types/DatetimeIntervalType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "types/operations/unary_operations/ArithmeticUnaryOperators.hpp"
#include "utility/EqualsAnyConstant.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

bool ArithmeticUnaryOperation::canApplyToType(const Type &type) const {
  return QUICKSTEP_EQUALS_ANY_CONSTANT(
      type.getTypeID(),
      kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
}

const Type* ArithmeticUnaryOperation::resultTypeForArgumentType(const Type &type) const {
  if (canApplyToType(type)) {
    return &type;
  } else {
    return nullptr;
  }
}

const Type* ArithmeticUnaryOperation::pushDownTypeHint(const Type *type_hint) const {
  if (type_hint == nullptr) {
    return nullptr;
  }

  if (canApplyToType(*type_hint)) {
    return type_hint;
  } else {
    return nullptr;
  }
}

bool NegateUnaryOperation::resultTypeIsPlausible(const Type &result_type) const {
  return QUICKSTEP_EQUALS_ANY_CONSTANT(
      result_type.getTypeID(),
      kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
}

TypedValue NegateUnaryOperation::applyToChecked(const TypedValue &argument,
                                                const Type &argument_type) const {
  DCHECK_EQ(argument.getTypeID(), argument_type.getTypeID());

  if (argument.isNull()) {
    return argument;
  }

  switch (argument.getTypeID()) {
    case kInt:
      return TypedValue(-argument.getLiteral<typename IntType::cpptype>());
    case kLong:
      return TypedValue(-argument.getLiteral<typename LongType::cpptype>());
    case kFloat:
      return TypedValue(-argument.getLiteral<typename FloatType::cpptype>());
    case kDouble:
      return TypedValue(-argument.getLiteral<typename DoubleType::cpptype>());
    case kDatetimeInterval:
      return TypedValue(-argument.getLiteral<typename DatetimeIntervalType::cpptype>());
    case kYearMonthInterval:
      return TypedValue(-argument.getLiteral<typename YearMonthIntervalType::cpptype>());
    default: {
      LOG(FATAL) << "Can not apply UnaryOperation " << getName()
                 << " to argument of type " << argument_type.getName();
    }
  }
}

UncheckedUnaryOperator* NegateUnaryOperation::makeUncheckedUnaryOperatorForType(const Type &type) const {
  switch (type.getTypeID()) {
    case kInt:
      if (type.isNullable()) {
        return new NegateUncheckedUnaryOperator<IntType, true>();
      } else {
        return new NegateUncheckedUnaryOperator<IntType, false>();
      }
    case kLong:
      if (type.isNullable()) {
        return new NegateUncheckedUnaryOperator<LongType, true>();
      } else {
        return new NegateUncheckedUnaryOperator<LongType, false>();
      }
    case kFloat:
      if (type.isNullable()) {
        return new NegateUncheckedUnaryOperator<FloatType, true>();
      } else {
        return new NegateUncheckedUnaryOperator<FloatType, false>();
      }
    case kDouble:
      if (type.isNullable()) {
        return new NegateUncheckedUnaryOperator<DoubleType, true>();
      } else {
        return new NegateUncheckedUnaryOperator<DoubleType, false>();
      }
    case kDatetimeInterval:
      if (type.isNullable()) {
        return new NegateUncheckedUnaryOperator<DatetimeIntervalType, true>();
      } else {
        return new NegateUncheckedUnaryOperator<DatetimeIntervalType, false>();
      }
    case kYearMonthInterval:
      if (type.isNullable()) {
        return new NegateUncheckedUnaryOperator<YearMonthIntervalType, true>();
      } else {
        return new NegateUncheckedUnaryOperator<YearMonthIntervalType, false>();
      }
    default:
      throw OperationInapplicableToType(getName(), 1, type.getName().c_str());
  }
}

}  // namespace quickstep
