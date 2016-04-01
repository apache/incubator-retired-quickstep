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

#include "types/operations/binary_operations/MultiplyBinaryOperation.hpp"

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

bool MultiplyBinaryOperation::canApplyToTypes(const Type &left, const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      return (right.getSuperTypeID() == Type::kNumeric ||
              right.getTypeID() == kDatetimeInterval   ||
              right.getTypeID() == kYearMonthInterval);
    }
    case kDatetimeInterval:
    case kYearMonthInterval: {
      return (right.getSuperTypeID() == Type::kNumeric);
    }
    default:
      return false;
  }
}

const Type* MultiplyBinaryOperation::resultTypeForArgumentTypes(const Type &left, const Type &right) const {
  if (left.getSuperTypeID() == Type::kNumeric && right.getSuperTypeID() == Type::kNumeric) {
    return TypeFactory::GetUnifyingType(left, right);
  } else if ((left.getSuperTypeID() == Type::kNumeric && right.getTypeID() == kDatetimeInterval) ||
             (left.getTypeID() == kDatetimeInterval && right.getSuperTypeID() == Type::kNumeric)) {
    return &(DatetimeIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else if ((left.getSuperTypeID() == Type::kNumeric && right.getTypeID() == kYearMonthInterval) ||
             (left.getTypeID() == kYearMonthInterval && right.getSuperTypeID() == Type::kNumeric)) {
    return &(YearMonthIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else {
    return nullptr;
  }
}

const Type* MultiplyBinaryOperation::resultTypeForPartialArgumentTypes(
    const Type *left,
    const Type *right) const {
  if ((left == nullptr) && (right == nullptr)) {
    return nullptr;
  }

  if ((left != nullptr) && (right != nullptr)) {
    return resultTypeForArgumentTypes(*left, *right);
  }

  // Multiplication is commutative, so we just determine based on the known
  // type, left or right.
  const Type *known_type = (left != nullptr) ? left : right;
  switch (known_type->getTypeID()) {
    case kDatetimeInterval:
      // DatetimeInterval can be multiplied against any numeric type, yielding
      // DatetimeInterval.
      return &TypeFactory::GetType(kDatetimeInterval, true);
    case kYearMonthInterval:
      // Same deal for YearMonthInterval.
      return &TypeFactory::GetType(kYearMonthInterval, true);
    default:
      // Ambiguous or inapplicable. Note that we can't apply numeric precedence
      // order for a Double argument, because the other argument could be a
      // numeric type OR an interval type.
      return nullptr;
  }
}

bool MultiplyBinaryOperation::partialTypeSignatureIsPlausible(
    const Type *result_type,
    const Type *left_argument_type,
    const Type *right_argument_type) const {
  if ((left_argument_type == nullptr) && (right_argument_type == nullptr)) {
    if (result_type == nullptr) {
      return true;
    } else if (!result_type->isNullable()) {
      // Unknown arguments are assumed to be nullable, since they arise from
      // untyped NULL literals in the parser. Therefore, a non-nullable result
      // Type is not plausible with unknown arguments.
      return false;
    } else {
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          result_type->getTypeID(),
          kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
    }
  }

  if ((left_argument_type != nullptr) && (right_argument_type != nullptr)) {
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
      return result_type->equals(*actual_result_type);
    }
  }

  // Multiplication is commutative, so we just determine based on the known
  // type, left or right.
  const Type *known_argument_type = (left_argument_type != nullptr)
                                    ? left_argument_type
                                    : right_argument_type;
  if (result_type == nullptr) {
    return QUICKSTEP_EQUALS_ANY_CONSTANT(
        known_argument_type->getTypeID(),
        kInt, kLong, kFloat, kDouble, kDatetimeInterval, kYearMonthInterval);
  }

  if (!result_type->isNullable()) {
    // One of the arguments is unknown, but it is nevertheless assumed
    // nullable, since unknown argument Types arise from untyped NULL literals
    // in the parser. Therefore, a non-nullable result Type is not plausible
    // with an unknown argument.
    return false;
  }

  switch (result_type->getTypeID()) {
    case kInt:
      return (known_argument_type->getTypeID() == kInt);
    case kLong:
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          known_argument_type->getTypeID(),
          kInt, kLong);
    case kFloat:
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          known_argument_type->getTypeID(),
          kInt, kFloat);
    case kDouble:
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          known_argument_type->getTypeID(),
          kInt, kLong, kFloat, kDouble);
    case kDatetimeInterval:
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          known_argument_type->getTypeID(),
          kInt, kLong, kFloat, kDouble, kDatetimeInterval);
    case kYearMonthInterval:
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          known_argument_type->getTypeID(),
          kInt, kLong, kFloat, kDouble, kYearMonthInterval);
    default:
      return false;
  }
}

std::pair<const Type*, const Type*> MultiplyBinaryOperation::pushDownTypeHint(
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
      // Ambiguous hint. One of the arguments should be the same as the
      // '*type_hint', the other can be any numeric type, but either order is
      // OK.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
    default:
      // Inapplicable.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }
}

TypedValue MultiplyBinaryOperation::applyToChecked(const TypedValue &left,
                                                   const Type &left_type,
                                                   const TypedValue &right,
                                                   const Type &right_type) const {
  switch (left_type.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      if (right_type.getSuperTypeID() == Type::kNumeric) {
        return applyToCheckedNumericHelper<MultiplyFunctor>(left, left_type,
                                                            right, right_type);
      } else if (right_type.getTypeID() == kDatetimeInterval) {
        return applyToCheckedIntervalMultiplyNumericHelper<DatetimeIntervalType>(right, right_type,
                                                                                 left, left_type);
      } else if (right_type.getTypeID() == kYearMonthInterval) {
        return applyToCheckedIntervalMultiplyNumericHelper<YearMonthIntervalType>(right, right_type,
                                                                                  left, left_type);
      }
      break;
    }
    case kDatetimeInterval: {
      if (right_type.getSuperTypeID() == Type::kNumeric) {
        return applyToCheckedIntervalMultiplyNumericHelper<DatetimeIntervalType>(left, left_type,
                                                                                 right, right_type);
      }
      break;
    }
    case kYearMonthInterval: {
      if (right_type.getSuperTypeID() == Type::kNumeric) {
        return applyToCheckedIntervalMultiplyNumericHelper<YearMonthIntervalType>(left, left_type,
                                                                                  right, right_type);
      }
      break;
    }
    default:
      break;
  }

  LOG(FATAL) << "Can not apply " << getName() << " to arguments of types "
             << left_type.getName() << " and " << right_type.getName();
}

UncheckedBinaryOperator* MultiplyBinaryOperation::makeUncheckedBinaryOperatorForTypes(const Type &left,
                                                                                      const Type &right) const {
  switch (left.getTypeID()) {
    case kInt: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator>(left, right);
      } else if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 DatetimeIntervalType,
                                                 IntType::cpptype, DatetimeIntervalLit>(left, right);
      } else if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 YearMonthIntervalType,
                                                 IntType::cpptype, YearMonthIntervalLit>(left, right);
      }
      break;
    }
    case kLong: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator>(left, right);
      } else if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 DatetimeIntervalType,
                                                 LongType::cpptype, DatetimeIntervalLit>(left, right);
      } else if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 YearMonthIntervalType,
                                                 LongType::cpptype, YearMonthIntervalLit>(left, right);
      }
      break;
    }
    case kFloat: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator>(left, right);
      } else if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 DatetimeIntervalType,
                                                 FloatType::cpptype, DatetimeIntervalLit>(left, right);
      } else if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 YearMonthIntervalType,
                                                 FloatType::cpptype, YearMonthIntervalLit>(left, right);
      }
      break;
    }
    case kDouble: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator>(left, right);
      } else if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 DatetimeIntervalType,
                                                 DoubleType::cpptype, DatetimeIntervalLit>(left, right);
      } else if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                 YearMonthIntervalType,
                                                 DoubleType::cpptype, YearMonthIntervalLit>(left, right);
      }
      break;
    }
    case kDatetimeInterval: {
      switch (right.getTypeID()) {
        case kInt: {
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                   DatetimeIntervalType,
                                                   DatetimeIntervalLit, IntType::cpptype>(left, right);
        }
        case kLong: {
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                   DatetimeIntervalType,
                                                   DatetimeIntervalLit, LongType::cpptype>(left, right);
        }
        case kFloat: {
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                   DatetimeIntervalType,
                                                   DatetimeIntervalLit, FloatType::cpptype>(left, right);
        }
        case kDouble: {
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
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
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                   YearMonthIntervalType,
                                                   YearMonthIntervalLit, IntType::cpptype>(left, right);
        }
        case kLong: {
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                   YearMonthIntervalType,
                                                   YearMonthIntervalLit, LongType::cpptype>(left, right);
        }
        case kFloat: {
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
                                                   YearMonthIntervalType,
                                                   YearMonthIntervalLit, FloatType::cpptype>(left, right);
        }
        case kDouble: {
          return makeDateBinaryOperatorOuterHelper<MultiplyArithmeticUncheckedBinaryOperator,
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

template <typename IntervalType>
TypedValue MultiplyBinaryOperation::applyToCheckedIntervalMultiplyNumericHelper(
    const TypedValue &left,
    const Type &left_type,
    const TypedValue &right,
    const Type &right_type) const {
  DCHECK(IntervalType::kStaticTypeID == kDatetimeInterval ||
         IntervalType::kStaticTypeID == kYearMonthInterval);
  DCHECK(IntervalType::kStaticTypeID == left_type.getTypeID());
  DCHECK_EQ(Type::kNumeric, right_type.getSuperTypeID());

  if (left.isNull() || right.isNull()) {
    return TypedValue(IntervalType::kStaticTypeID);
  }

  switch (right_type.getTypeID()) {
    case kInt: {
      return TypedValue(left.getLiteral<typename IntervalType::cpptype>() * right.getLiteral<IntType::cpptype>());
    }
    case kLong: {
      return TypedValue(left.getLiteral<typename IntervalType::cpptype>() * right.getLiteral<LongType::cpptype>());
    }
    case kFloat: {
      return TypedValue(left.getLiteral<typename IntervalType::cpptype>() * right.getLiteral<FloatType::cpptype>());
    }
    case kDouble: {
      return TypedValue(left.getLiteral<typename IntervalType::cpptype>() * right.getLiteral<DoubleType::cpptype>());
    }
    default: {
      LOG(FATAL) << "Can not apply " << getName() << " to arguments of types "
                 << left_type.getName() << " and " << right_type.getName();
    }
  }
}

}  // namespace quickstep
