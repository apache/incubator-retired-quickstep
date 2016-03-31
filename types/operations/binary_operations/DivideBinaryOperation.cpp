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

#include "types/operations/binary_operations/DivideBinaryOperation.hpp"

#include <functional>
#include <string>
#include <utility>

#include "types/DateOperatorOverloads.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/IntervalLit.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "types/operations/binary_operations/ArithmeticBinaryOperators.hpp"
#include "utility/EqualsAnyConstant.hpp"

#include "glog/logging.h"

namespace quickstep {

bool DivideBinaryOperation::canApplyToTypes(const Type &left, const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble:
    case kDatetimeInterval:
    case kYearMonthInterval: {
      return (right.getSuperTypeID() == Type::kNumeric);
    }
    default:
      return false;
  }
}

const Type* DivideBinaryOperation::resultTypeForArgumentTypes(const Type &left, const Type &right) const {
  if (left.getSuperTypeID() == Type::kNumeric && right.getSuperTypeID() == Type::kNumeric) {
    return TypeFactory::GetUnifyingType(left, right);
  } else if (left.getTypeID() == kDatetimeInterval && right.getSuperTypeID() == Type::kNumeric) {
    return &(DatetimeIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else if (left.getTypeID() == kYearMonthInterval && right.getSuperTypeID() == Type::kNumeric) {
    return &(YearMonthIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else {
    return nullptr;
  }
}

const Type* DivideBinaryOperation::resultTypeForPartialArgumentTypes(
    const Type *left,
    const Type *right) const {
  if (left == nullptr) {
    // Can't determine result type just based on right (divisor) type.
    return nullptr;
  } else if (right == nullptr) {
    switch (left->getTypeID()) {
      case kDouble:
        // Double has highest precedence of numeric types.
        return &TypeFactory::GetType(kDouble, true);
      case kDatetimeInterval:
        return &TypeFactory::GetType(kDatetimeInterval, true);
      case kYearMonthInterval:
        return &TypeFactory::GetType(kYearMonthInterval, true);
      default:
        // Ambiguous or inapplicable.
        return nullptr;
    }
  } else {
    return resultTypeForArgumentTypes(*left, *right);
  }
}

bool DivideBinaryOperation::partialTypeSignatureIsPlausible(
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
            kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
      }
    }

    if (result_type == nullptr) {
      // Right (divisor) argument type is known, left (dividend) argument and
      // result types are unknown. Just check that it is possible to divide by
      // the right (divisor) type.
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          right_argument_type->getTypeID(),
          kInt, kLong, kFloat, kDouble);
    }

    // Return type and right (divisor) argument type are known, left (dividend)
    // argument type is unknown. Check that result and divisor are compatible.
    switch (right_argument_type->getTypeID()) {
      case kInt:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
      case kLong:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kLong, kDouble, kDatetimeInterval, kYearMonthInterval);
      case kFloat:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
      case kDouble:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kDouble, kDatetimeInterval, kYearMonthInterval);
      default:
        return false;
    }
  } else {  // left_argument_type != nullptr
    if (right_argument_type == nullptr) {
      if (result_type == nullptr) {
        // Left (dividend) argument type is known, right (divisor) argument
        // type and result type are unknown. Just check that the left
        // (dividend) type can be divided.
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            left_argument_type->getTypeID(),
            kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
      }

      // Result type and left (dividend) argument type are known, but right
      // (divisor) argument type is unknown. Check that result and dividend are
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
        case kDatetimeInterval:
          return (result_type->getTypeID() == kDatetimeInterval);
        case kYearMonthInterval:
          return (result_type->getTypeID() == kYearMonthInterval);
        default:
          return false;
      }
    }

    // Left and right (dividend and divisor) argument types are both known.
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

std::pair<const Type*, const Type*> DivideBinaryOperation::pushDownTypeHint(
    const Type *result_type_hint) const {
  if (result_type_hint == nullptr) {
    return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }

  switch (result_type_hint->getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble:
      return std::pair<const Type*, const Type*>(result_type_hint, result_type_hint);
    case kDatetimeInterval:
    case kYearMonthInterval:
      // Left (dividend) should be the same kind of interval as '*type_hint',
      // right (divisor) can be any numeric type.
      return std::pair<const Type*, const Type*>(result_type_hint, nullptr);
    default:
      // Inapplicable.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }
}

TypedValue DivideBinaryOperation::applyToChecked(const TypedValue &left,
                                                 const Type &left_type,
                                                 const TypedValue &right,
                                                 const Type &right_type) const {
  switch (left_type.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      if (right_type.getSuperTypeID() == Type::kNumeric) {
        return applyToCheckedNumericHelper<DivideFunctor>(left, left_type,
                                                          right, right_type);
      }
      break;
    }
    case kDatetimeInterval: {
      switch (right_type.getTypeID()) {
        case kInt:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kDatetimeInterval);
          }

          return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<IntType::cpptype>());
        case kLong:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kDatetimeInterval);
          }

          return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<LongType::cpptype>());
        case kFloat:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kDatetimeInterval);
          }

          return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<FloatType::cpptype>());
        case kDouble:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kDatetimeInterval);
          }

          return TypedValue(left.getLiteral<DatetimeIntervalLit>() / right.getLiteral<DoubleType::cpptype>());
        default:
          break;
      }
      break;
    }
    case kYearMonthInterval: {
      switch (right_type.getTypeID()) {
        case kInt:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kYearMonthInterval);
          }

          return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<IntType::cpptype>());
        case kLong:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kYearMonthInterval);
          }

          return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<LongType::cpptype>());
        case kFloat:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kYearMonthInterval);
          }

          return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<FloatType::cpptype>());
        case kDouble:
          if (left.isNull() || right.isNull()) {
            return TypedValue(kYearMonthInterval);
          }

          return TypedValue(left.getLiteral<YearMonthIntervalLit>() / right.getLiteral<DoubleType::cpptype>());
        default:
          break;
      }
      break;
    }
    default:
      break;
  }

  LOG(FATAL) << "Can not apply " << getName() << " to arguments of types "
             << left_type.getName() << " and " << right_type.getName();
}

UncheckedBinaryOperator* DivideBinaryOperation::makeUncheckedBinaryOperatorForTypes(const Type &left,
                                                                                    const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator>(left, right);
      }
      break;
    }
    case kDatetimeInterval: {
      switch (right.getTypeID()) {
        case kInt: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   DatetimeIntervalType,
                                                   DatetimeIntervalLit, IntType::cpptype>(left, right);
        }
        case kLong: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   DatetimeIntervalType,
                                                   DatetimeIntervalLit, LongType::cpptype>(left, right);
        }
        case kFloat: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   DatetimeIntervalType,
                                                   DatetimeIntervalLit, FloatType::cpptype>(left, right);
        }
        case kDouble: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   DatetimeIntervalType,
                                                   DatetimeIntervalLit, DoubleType::cpptype>(left, right);
        }
        default:
          break;
      }
      break;
    }
    case kYearMonthInterval: {
      switch (right.getTypeID()) {
        case kInt: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   YearMonthIntervalType,
                                                   YearMonthIntervalLit, IntType::cpptype>(left, right);
        }
        case kLong: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   YearMonthIntervalType,
                                                   YearMonthIntervalLit, LongType::cpptype>(left, right);
        }
        case kFloat: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   YearMonthIntervalType,
                                                   YearMonthIntervalLit, FloatType::cpptype>(left, right);
        }
        case kDouble: {
          return makeDateBinaryOperatorOuterHelper<DivideArithmeticUncheckedBinaryOperator,
                                                   YearMonthIntervalType,
                                                   YearMonthIntervalLit, DoubleType::cpptype>(left, right);
        }
        default:
          break;
      }
      break;
    }
    default:
      break;
  }

  throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
}

}  // namespace quickstep
