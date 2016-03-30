/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#include "types/operations/binary_operations/ModuloBinaryOperation.hpp"

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

bool ModuloBinaryOperation::canApplyToTypes(const Type &left, const Type &right) const {
  switch (left.getTypeID()) {
    case kInt: // Fall through
    case kLong:
    case kFloat:
    case kDouble: {
      return (right.getSuperTypeID() == Type::kNumeric);
    }
    // TODO(jianqiao): Extend modulo operator to be applicable to DatetimeInterval
    // and YearMonthInterval.
    default:
      return false;
  }
}

const Type* ModuloBinaryOperation::resultTypeForArgumentTypes(const Type &left, const Type &right) const {
  if (left.getSuperTypeID() == Type::kNumeric && right.getSuperTypeID() == Type::kNumeric) {
    return TypeFactory::GetUnifyingType(left, right);
  } else {
    return nullptr;
  }
}

const Type* ModuloBinaryOperation::resultTypeForPartialArgumentTypes(
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
      default:
        // Ambiguous or inapplicable.
        return nullptr;
    }
  } else {
    return resultTypeForArgumentTypes(*left, *right);
  }
}

bool ModuloBinaryOperation::partialTypeSignatureIsPlausible(
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
            kInt, kLong, kFloat, kDouble);
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
            kInt, kLong, kFloat, kDouble);
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

std::pair<const Type*, const Type*> ModuloBinaryOperation::pushDownTypeHint(
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
    default:
      // Inapplicable.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }
}

TypedValue ModuloBinaryOperation::applyToChecked(const TypedValue &left,
                                                 const Type &left_type,
                                                 const TypedValue &right,
                                                 const Type &right_type) const {
  switch (left_type.getTypeID()) {
    case kInt:
    case kLong: {
      if (right_type.getTypeID() == TypeID::kInt
          || right_type.getTypeID() == TypeID::kLong) {
        return applyToCheckedIntegerHelper<IntegerModuloFunctor>(left, left_type,
                                                                 right, right_type);
      }
    }  // Fall through
    case kFloat:
    case kDouble: {
      if (right_type.getSuperTypeID() == Type::kNumeric) {
        return applyToCheckedNumericHelper<FloatModuloFunctor>(left, left_type,
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

UncheckedBinaryOperator* ModuloBinaryOperation::makeUncheckedBinaryOperatorForTypes(const Type &left,
                                                                                    const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong: {
      if (right.getTypeID() == TypeID::kInt
          || right.getTypeID() == TypeID::kLong) {
        return makeIntegerBinaryOperatorOuterHelper<IntegerModuloArithmeticUncheckedBinaryOperator>(left, right);
      }
    }  // Fall through
    case kFloat:
    case kDouble: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<FloatModuloArithmeticUncheckedBinaryOperator>(left, right);
      }
      break;
    }
    default:
      break;
  }

  throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
}

}  // namespace quickstep
