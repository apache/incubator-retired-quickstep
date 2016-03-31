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

#include "types/operations/binary_operations/AddBinaryOperation.hpp"

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

bool AddBinaryOperation::canApplyToTypes(const Type &left, const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:  // Fall through.
    case kLong:
    case kFloat:
    case kDouble: {
      return (right.getSuperTypeID() == Type::kNumeric);
    }
    case kDatetime: {
      return (right.getTypeID() == kDatetimeInterval ||
              right.getTypeID() == kYearMonthInterval);
    }
    case kDatetimeInterval: {
      return (right.getTypeID() == kDatetime ||
              right.getTypeID() == kDatetimeInterval);
    }
    case kYearMonthInterval: {
      return (right.getTypeID() == kDatetime ||
              right.getTypeID() == kYearMonthInterval);
    }
    default:
      return false;
  }
}

const Type* AddBinaryOperation::resultTypeForArgumentTypes(const Type &left, const Type &right) const {
  if (left.getSuperTypeID() == Type::kNumeric && right.getSuperTypeID() == Type::kNumeric) {
    return TypeFactory::GetUnifyingType(left, right);
  } else if ((left.getTypeID() == kDatetime && right.getTypeID() == kDatetimeInterval)  ||
             (left.getTypeID() == kDatetimeInterval && right.getTypeID() == kDatetime)  ||
             (left.getTypeID() == kDatetime && right.getTypeID() == kYearMonthInterval) ||
             (left.getTypeID() == kYearMonthInterval && right.getTypeID() == kDatetime)) {
    return &(DatetimeType::Instance(left.isNullable() || right.isNullable()));
  } else if (left.getTypeID() == kDatetimeInterval && right.getTypeID() == kDatetimeInterval) {
    return &(DatetimeIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else if (left.getTypeID() == kYearMonthInterval && right.getTypeID() == kYearMonthInterval) {
    return &(YearMonthIntervalType::Instance(left.isNullable() || right.isNullable()));
  } else {
    return nullptr;
  }
}

const Type* AddBinaryOperation::resultTypeForPartialArgumentTypes(const Type *left,
                                                                  const Type *right) const {
  if ((left == nullptr) && (right == nullptr)) {
    return nullptr;
  }

  if ((left != nullptr) && (right != nullptr)) {
    return resultTypeForArgumentTypes(*left, *right);
  }

  // Addition is commutative, so we just determine based on the known type,
  // left or right.
  const Type *known_type = (left != nullptr) ? left : right;
  switch (known_type->getTypeID()) {
    case kDouble:
      // Double has highest precedence of the numeric types.
      return &TypeFactory::GetType(kDouble, true);
    case kDatetime:
      // Datetime can be added with either interval type, and always yields
      // Datetime.
      return &TypeFactory::GetType(kDatetime, true);
    default:
      // Ambiguous or inapplicable.
      return nullptr;
  }
}

bool AddBinaryOperation::partialTypeSignatureIsPlausible(
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
          kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval);
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

  // Addition is commutative, so we just determine based on the known type,
  // left or right.
  const Type *known_argument_type = (left_argument_type != nullptr)
                                    ? left_argument_type
                                    : right_argument_type;
  if (result_type == nullptr) {
    return QUICKSTEP_EQUALS_ANY_CONSTANT(
        known_argument_type->getTypeID(),
        kInt, kLong, kFloat, kDouble, kDatetime, kDatetimeInterval, kYearMonthInterval);
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
    case kDatetime:
      return QUICKSTEP_EQUALS_ANY_CONSTANT(
          known_argument_type->getTypeID(),
          kDatetime, kDatetimeInterval, kYearMonthInterval);
    case kDatetimeInterval:
      return (known_argument_type->getTypeID() == kDatetimeInterval);
    case kYearMonthInterval:
      return (known_argument_type->getTypeID() == kYearMonthInterval);
    default:
      return false;
  }
}

std::pair<const Type*, const Type*> AddBinaryOperation::pushDownTypeHint(
    const Type *result_type_hint) const {
  if (result_type_hint == nullptr) {
    return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }

  switch (result_type_hint->getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble:
    case kDatetimeInterval:
    case kYearMonthInterval:
      // Hint the same as the result type. Note that, for numeric types, one of
      // the argument Types can be a less precise Type and still yield the
      // specified result Type (e.g. DoubleType + IntType = DoubleType). We
      // choose the highest-precision suitable Type (i.e. the same as the
      // result type) in such cases.
      return std::pair<const Type*, const Type*>(result_type_hint, result_type_hint);
    case kDatetime:
      // Hint is ambiguous: one argument should be a Datetime, the other should
      // be one of the interval types, but either order is acceptable.
      // Fortunately, the 3 types in question have syntactically distinct
      // representations in the SQL parser, so their literals don't need
      // disambiguation anyway.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
    default:
      // Inapplicable.
      return std::pair<const Type*, const Type*>(nullptr, nullptr);
  }
}

TypedValue AddBinaryOperation::applyToChecked(const TypedValue &left,
                                              const Type &left_type,
                                              const TypedValue &right,
                                              const Type &right_type) const {
  switch (left_type.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      switch (right_type.getTypeID()) {
        case kInt:
        case kLong:
        case kFloat:
        case kDouble:
          return applyToCheckedNumericHelper<AddFunctor>(left, left_type,
                                                         right, right_type);
        default:
          break;
      }
      break;
    }
    case kDatetime: {
      if (right_type.getTypeID() == kDatetimeInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetime);
        }

        return TypedValue(left.getLiteral<DatetimeLit>() + right.getLiteral<DatetimeIntervalLit>());
      } else if (right_type.getTypeID() == kYearMonthInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetime);
        }

        return TypedValue(left.getLiteral<DatetimeLit>() + right.getLiteral<YearMonthIntervalLit>());
      }
      break;
    }
    case kDatetimeInterval: {
      if (right_type.getTypeID() == kDatetime) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetime);
        }

        return TypedValue(left.getLiteral<DatetimeIntervalLit>() + right.getLiteral<DatetimeLit>());
      } else if (right_type.getTypeID() == kDatetimeInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetimeInterval);
        }

        return TypedValue(left.getLiteral<DatetimeIntervalLit>() + right.getLiteral<DatetimeIntervalLit>());
      }
      break;
    }
    case kYearMonthInterval: {
      if (right_type.getTypeID() == kDatetime) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kDatetime);
        }

        return TypedValue(left.getLiteral<YearMonthIntervalLit>() + right.getLiteral<DatetimeLit>());
      } else if (right_type.getTypeID() == kYearMonthInterval) {
        if (left.isNull() || right.isNull()) {
          return TypedValue(kYearMonthInterval);
        }

        return TypedValue(left.getLiteral<YearMonthIntervalLit>() + right.getLiteral<YearMonthIntervalLit>());
      }
      break;
    }
    default:
      break;
  }

  LOG(FATAL) << "Can not apply " << getName() << " to arguments of types "
             << left_type.getName() << " and " << right_type.getName();
}

UncheckedBinaryOperator* AddBinaryOperation::makeUncheckedBinaryOperatorForTypes(const Type &left,
                                                                                 const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
    case kLong:
    case kFloat:
    case kDouble: {
      if (right.getSuperTypeID() == Type::kNumeric) {
        return makeNumericBinaryOperatorOuterHelper<AddArithmeticUncheckedBinaryOperator>(left, right);
      }
      break;
    }
    case kDatetime: {
      if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<AddArithmeticUncheckedBinaryOperator,
                                                 DatetimeType,
                                                 DatetimeLit, DatetimeIntervalLit>(left, right);
      } else if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<AddArithmeticUncheckedBinaryOperator,
                                                 DatetimeType,
                                                 DatetimeLit, YearMonthIntervalLit>(left, right);
      }
      break;
    }
    case kDatetimeInterval: {
      if (right.getTypeID() == kDatetime) {
        return makeDateBinaryOperatorOuterHelper<AddArithmeticUncheckedBinaryOperator,
                                                 DatetimeType,
                                                 DatetimeIntervalLit, DatetimeLit>(left, right);
      } else if (right.getTypeID() == kDatetimeInterval) {
        return makeDateBinaryOperatorOuterHelper<AddArithmeticUncheckedBinaryOperator,
                                                 DatetimeIntervalType,
                                                 DatetimeIntervalLit, DatetimeIntervalLit>(left, right);
      }
      break;
    }
    case kYearMonthInterval: {
      if (right.getTypeID() == kDatetime) {
        return makeDateBinaryOperatorOuterHelper<AddArithmeticUncheckedBinaryOperator,
                                                 DatetimeType,
                                                 YearMonthIntervalLit, DatetimeLit>(left, right);
      } else if (right.getTypeID() == kYearMonthInterval) {
        return makeDateBinaryOperatorOuterHelper<AddArithmeticUncheckedBinaryOperator,
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
