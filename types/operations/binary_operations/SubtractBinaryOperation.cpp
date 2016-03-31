/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "types/operations/binary_operations/SubtractBinaryOperation.hpp"

#include <functional>
#include <string>
#include <utility>

#include "types/DateOperatorOverloads.hpp"
#include "types/DatetimeLit.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DatetimeType.hpp"
#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "types/operations/binary_operations/ArithmeticBinaryOperators.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

namespace quickstep {

bool SubtractBinaryOperation::canApplyToTypes(const Type &left, const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      return (right.getSuperTypeID() == Type::kNumeric);
    }
    case kDatetime: {
      return (right.getTypeID() == kDatetime         ||
              right.getTypeID() == kDatetimeInterval ||
              right.getTypeID() == kYearMonthInterval);
    }
    case kDatetimeInterval: {
      return (right.getTypeID() == kDatetimeInterval);
    }
    case kYearMonthInterval: {
      return (right.getTypeID() == kYearMonthInterval);
    }
    default:
      return false;
  }
}

const Type* SubtractBinaryOperation::resultTypeForArgumentTypes(const Type &left, const Type &right) const {
  if (left.getSuperTypeID() == Type::kNumeric && right.getSuperTypeID() == Type::kNumeric) {
    return TypeFactory::GetUnifyingType(left, right);
  } else if ((left.getTypeID() == kDatetime && right.getTypeID() == kDatetime) ||
             (left.getTypeID() == kDatetimeInterval && right.getTypeID() == kDatetimeInterval)) {
    // NOTE(zuyu): we set the result type of the Subtract
    // between two Datetimes as DatetimeInterval, instead of YearMonthInterval.
    return &(DatetimeIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else if (left.getTypeID() == kDatetime && right.getTypeID() == kDatetimeInterval) {
    return &(DatetimeType::Instance(left.isNullable() || right.isNullable()));
  } else if (left.getTypeID() == kDatetime && right.getTypeID() == kYearMonthInterval) {
    return &(DatetimeType::Instance(left.isNullable() || right.isNullable()));
  } else if (left.getTypeID() == kYearMonthInterval && right.getTypeID() == kYearMonthInterval) {
    return &(YearMonthIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else {
    return nullptr;
  }
}

const Type* SubtractBinaryOperation::resultTypeForPartialArgumentTypes(
    const Type *left,
    const Type *right) const {
  if (left == nullptr) {
    if (right == nullptr) {
      return nullptr;
    } else {
      switch (right->getTypeID()) {
        case kDouble:
          // Double has highest precedence of numeric types.
          return &TypeFactory::GetType(kDouble, true);
        case kDatetime:
          // If the subtrahend is Datetime, then the only allowed minuend is
          // another Datetime, and the result is an interval.
          return &TypeFactory::GetType(kDatetimeInterval, true);
        default:
          // Ambiguous or inapplicable.
          return nullptr;
      }
    }
  } else {
    if (right == nullptr) {
      switch (left->getTypeID()) {
        case kDouble:
          // Double has highest precedence of numeric types.
          return &TypeFactory::GetType(kDouble, true);
        case kDatetimeInterval:
          // If minuend is a DatetimeInterval, the subtrahend and result must
          // also be DatetimeInterval.
          return &TypeFactory::GetType(kDatetimeInterval, true);
        case kYearMonthInterval:
          // Similarly, if minuend is a YearMonthInterval, the subtrahend and
          // result must also be YearMonthInterval.
          return &TypeFactory::GetType(kYearMonthInterval, true);
        default:
          // Ambiguous or inapplicable.
          return nullptr;
      }
    } else {
      return resultTypeForArgumentTypes(*left, *right);
    }
  }
}

bool SubtractBinaryOperation::partialTypeSignatureIsPlausible(
    const Type *result_type,
    const Type *left_argument_type,
    const Type *right_argument_type) const {
  // Early check: if either argument type is nullable or unknown, result type
  // must also be nullable.
  if ((left_argument_type == nullptr)
      || left_argument_type->isNullable()
      || (right_argument_type == nullptr)
      || right_argument_type->isNullable()) {
    if ((result_type != nullptr) && (!result_type->isNullable())) {
      return false;
    }
  }

  if (left_argument_type == nullptr) {
    if (right_argument_type == nullptr) {
      if (result_type == nullptr) {
        // All types unknown.
        return true;
      } else {
        // Only result type is known, just check that it is one of the types
        // that can possibly be returned.
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval);
      }
    }

    if (result_type == nullptr) {
      // Right (minuend) argument type is known, left (subtrahend) argument and
      // result types are unknown. Just check that right (minuend) type can be
      // subtracted.
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          right_argument_type->getTypeID(),
          kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval);
    }

    // Return type and right (minuend) argument type are known, left
    // (subtrahend) argument type is unknown. Check that result and subtrahend
    // are compatible.
    switch (right_argument_type->getTypeID()) {
      case kInt:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kInt, kLong, kFloat, kDouble);
      case kLong:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kLong, kDouble);
      case kFloat:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kFloat, kDouble);
      case kDouble:
        return (result_type->getTypeID() == kDouble);
      case kDatetime:
        return (result_type->getTypeID() == kDatetimeInterval);
      case kDatetimeInterval:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kDatetime, kDatetimeInterval);
      case kYearMonthInterval:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kDatetime, kYearMonthInterval);
      default:
        return false;
    }
  } else {  // left_argument_type != nullptr
    if (right_argument_type == nullptr) {
      if (result_type == nullptr) {
        // Left (subtrahend) argument type is known, right (minuend) argument
        // type and result type are unknown. Just check that the left
        // (subtrahend) type can be subtracted from.
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            left_argument_type->getTypeID(),
            kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval);
      }

      // Result type and left (subtrahend) argument type are known, but right
      // (minuend) argument type is unknown. Check that result and minuend are
      // compatible.
      switch (left_argument_type->getTypeID()) {
        case kInt:
          return QUICKSTEP_EQUALS_ANY_CONSTANT(
              result_type->getTypeID(),
              kInt, kLong, kFloat, kDouble);
        case kLong:
          return QUICKSTEP_EQUALS_ANY_CONSTANT(
              result_type->getTypeID(),
              kLong, kDouble);
        case kFloat:
          return QUICKSTEP_EQUALS_ANY_CONSTANT(
              result_type->getTypeID(),
              kFloat, kDouble);
        case kDouble:
          return (result_type->getTypeID() == kDouble);
        case kDatetime:
          return QUICKSTEP_EQUALS_ANY_CONSTANT(
              result_type->getTypeID(),
              kDatetime, kDatetimeInterval);
        case kDatetimeInterval:
          return (result_type->getTypeID() == kDatetimeInterval);
        case kYearMonthInterval:
          return (result_type->getTypeID() == kYearMonthInterval);
        default:
          return false;
      }
    }

    // Left and right (subtrahend and minuend) argument types are both known.
    const Type *actual_result_type = resultTypeForArgumentTypes(*left_argument_type,
                                                                *right_argument_type);
    if (actual_result_type == nullptr) {
      // Both argument Types are known, but this operation is NOT applicable to
      // them. No matter what the result_type is, the signature is not
      // plausible.
      return false;
    } else if (result_type == nullptr) {
      return true;
    } else {
      // Check if result type matches.
      return result_type->equals(*actual_result_type);
    }
  }
}

std::pair<const Type*, const Type*> SubtractBinaryOperation::pushDownTypeHint(
    const Type *result_type_hint) const {
  if (result_type_hint == nullptr) {
    return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }

  switch (result_type_hint->getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble:
    case kYearMonthInterval:
      return std::pair<const Type*, const Type*>(result_type_hint, result_type_hint);
    case kDatetime:
      // Left should be a Datetime, right may be either interval type.
      return std::pair<const Type*, const Type*>(result_type_hint, nullptr);
    case kDatetimeInterval:
      // Ambiguous: could be subtracting two Datetimes or two DatetimeIntervals.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
    default:
      // Inapplicable.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }
}

TypedValue SubtractBinaryOperation::applyToChecked(const TypedValue &left,
                                                   const Type &left_type,
                                                   const TypedValue &right,
                                                   const Type &right_type) const {
  switch (left_type.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      if (right_type.getSuperTypeID() == Type::kNumeric) {
        return applyToCheckedNumericHelper<SubtractFunctor>(left, left_type,
                                                            right, right_type);
      }
      break;
    }
    case kDatetime: {
      if (right_type.getTypeID() == kDatetime) {
        // NOTE(zuyu): The result type of the Subtract between two Datetimes is DatetimeInterval,
        // instead of YearMonthInterval.
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetimeInterval);
        }

        return TypedValue(left.getLiteral<DatetimeLit>() - right.getLiteral<DatetimeLit>());
      } else if (right_type.getTypeID() == kDatetimeInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetime);
        }

        return TypedValue(left.getLiteral<DatetimeLit>() - right.getLiteral<DatetimeIntervalLit>());
      } else if (right_type.getTypeID() == kYearMonthInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetime);
        }

        return TypedValue(left.getLiteral<DatetimeLit>() - right.getLiteral<YearMonthIntervalLit>());
      }
      break;
    }
    case kDatetimeInterval: {
      if (right_type.getTypeID() == kDatetimeInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetimeInterval);
        }

        return TypedValue(left.getLiteral<DatetimeIntervalLit>() - right.getLiteral<DatetimeIntervalLit>());
      }
      break;
    }
    case kYearMonthInterval: {
      if (right_type.getTypeID() == kYearMonthInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kYearMonthInterval);
        }

        return TypedValue(left.getLiteral<YearMonthIntervalLit>() - right.getLiteral<YearMonthIntervalLit>());
      }
      break;
    }
    default:
      break;
  }

  LOG(FATAL) << "Can not apply " << getName() << " to arguments of types "
             << left_type.getName() << " and " << right_type.getName();
}

UncheckedBinaryOperator* SubtractBinaryOperation::makeUncheckedBinaryOperatorForTypes(const Type &left,
                                                                                      const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<SubtractArithmeticUncheckedBinaryOperator>(left, right);
      }
      break;
    }
    case kDatetime: {
      if (right.getTypeID() == kDatetime) {
        // NOTE(zuyu): The result type of the Subtract between two Datetimes is DatetimeInterval,
        // instead of YearMonthInterval.
        return makeDateBinaryOperatorOuterHelper<SubtractArithmeticUncheckedBinaryOperator,
                                                 DatetimeIntervalType,
                                                 DatetimeLit, DatetimeLit>(left, right);
      } else if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<SubtractArithmeticUncheckedBinaryOperator,
                                                 DatetimeType,
                                                 DatetimeLit, DatetimeIntervalLit>(left, right);
      } else if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<SubtractArithmeticUncheckedBinaryOperator,
                                                 DatetimeType,
                                                 DatetimeLit, YearMonthIntervalLit>(left, right);
      }
      break;
    }
    case kDatetimeInterval: {
      if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<SubtractArithmeticUncheckedBinaryOperator,
                                                 DatetimeIntervalType,
                                                 DatetimeIntervalLit, DatetimeIntervalLit>(left, right);
      }
      break;
    }
    case kYearMonthInterval: {
      if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<SubtractArithmeticUncheckedBinaryOperator,
                                                 YearMonthIntervalType,
                                                 YearMonthIntervalLit, YearMonthIntervalLit>(left, right);
      }
      break;
    }
    default:
      break;
  }

  throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
}

}  // namespace quickstep
