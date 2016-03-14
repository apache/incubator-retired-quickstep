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
    case kInt: {
      return (right.getTypeID() == kInt);
    }
    case kLong: {
      return (right.getTypeID() == kInt ||
              right.getTypeID() == kLong);
    }
    default:
      return false;
  }
}

const Type* ModuloBinaryOperation::resultTypeForArgumentTypes(const Type &left, const Type &right) const {
  switch (left.getTypeID()) {
    case kInt: {
      return &TypeFactory::GetType(kInt, true);
    }
    case kLong: {
      return &TypeFactory::GetType(kLong, true);
    }
    default:
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
      case kInt:
        return &TypeFactory::GetType(kInt, true);
      case kLong:
        return &TypeFactory::GetType(kLong, true);
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
            kInt, kLong);
      }
    }

    if (result_type == nullptr) {
      // Right (divisor) argument type is known, left (dividend) argument and
      // result types are unknown. Just check that it is possible to divide by
      // the right (divisor) type.
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          right_argument_type->getTypeID(),
          kInt, kLong);
    }

    // Return type and right (divisor) argument type are known, left (dividend)
    // argument type is unknown. Check that result and divisor are compatible.
    switch (right_argument_type->getTypeID()) {
      case kInt:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kInt, kLong);
      case kLong:
        return QUICKSTEP_EQUALS_ANY_CONSTANT(
            result_type->getTypeID(),
            kLong);
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
            kInt, kLong);
      }

      // Result type and left (dividend) argument type are known, but right
      // (divisor) argument type is unknown. Check that result and dividend are
      // compatible.
      switch (left_argument_type->getTypeID()) {
        case kInt:
          return QUICKSTEP_EQUALS_ANY_CONSTANT(
              result_type->getTypeID(),
              kInt, kLong);
        case kLong:
          return (result_type->getTypeID() == kLong);
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
    case kInt: {
      if (right_type.getTypeID() == kInt) {
        // TODO(jmp) Hack
        /*
        return applyToCheckedNumericHelper<std::modulus>(left, left_type,
                                                         right, right_type);
        */
        /*
        return TypedValue(std::modulus<int>(left.getLiteral<IntType::cpptype>(),
                                            right.getLiteral<IntType::cpptype>()));
         */
        // TODO(jmp): Ugly hack
        return TypedValue(left.getLiteral<IntType::cpptype>() % right.getLiteral<IntType::cpptype>());

        // int

        /*
        OperationFunctor<IntType::cpptype> operation_functor;
        return TypedValue(operation_functor(left_coerced.getLiteral<IntType::cpptype>(),
                                            right_coerced.getLiteral<IntType::cpptype>()));
         */

      }
      break;
    }
    case kLong: {
      if (right_type.getTypeID() == kInt || right_type.getTypeID() == kLong) {
        // TODO(jmp) Hack
        /*
        return applyToCheckedNumericHelper<std::modulus>(left, left_type,
                                                         right, right_type);
         */
      /*
      return TypedValue(std::modulus<LongType::cpptype>(left.getLiteral<LongType::cpptype>(),
                                                       right.getLiteral<LongType::cpptype>()));

       */
      // TODO(jmp): Ugly hack
      return TypedValue(left.getLiteral<LongType::cpptype>() % right.getLiteral<LongType::cpptype>());
      // std::int64_t
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
    case kInt: {
      if (right.getTypeID() == kInt) {
        return makeNumericBinaryOperatorOuterHelper<ModuloArithmeticUncheckedBinaryOperator>(left, right);
      }
      break;
    }
    case kLong: {
      if (right.getTypeID() == kInt || right.getTypeID() == kLong) {
        return makeNumericBinaryOperatorOuterHelper<ModuloArithmeticUncheckedBinaryOperator>(left, right);
      }
      break;
    }
    default:
      break;
  }

  throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
}

}  // namespace quickstep
