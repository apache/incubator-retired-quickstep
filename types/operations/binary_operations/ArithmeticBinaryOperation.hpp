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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_OPERATION_HPP_

#include <string>

#include "types/DecimalType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/NumericTypeUnifier.hpp"
#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A BinaryOperation which applies to and yields values
 *        including numeric, datetime, and intervals.
 **/
class ArithmeticBinaryOperation : public BinaryOperation {
 protected:
  explicit ArithmeticBinaryOperation(const BinaryOperationID operation_id)
      : BinaryOperation(operation_id) {
  }

  template <template <typename LeftCppType, typename RightCppType> class OperationFunctor>
  TypedValue applyToCheckedIntegerHelper(const TypedValue &left,
                                         const Type &left_type,
                                         const TypedValue &right,
                                         const Type &right_type) const;

  template <template <typename LeftCppType, typename RightCppType> class OperationFunctor>
  TypedValue applyToCheckedNumericHelper(const TypedValue &left,
                                         const Type &left_type,
                                         const TypedValue &right,
                                         const Type &right_type) const;

  template <template <class ResultType,
                      typename LeftCppType, bool left_nullable,
                      typename RightCppType, bool right_nullable> class OperatorType>
  UncheckedBinaryOperator* makeIntegerBinaryOperatorOuterHelper(const Type &left,
                                                                const Type &right) const;

  template <template <class ResultType,
                      typename LeftCppType, bool left_nullable,
                      typename RightCppType, bool right_nullable> class OperatorType,
            typename LeftType, bool left_nullable>
  UncheckedBinaryOperator* makeIntegerBinaryOperatorInnerHelper(const Type &left,
                                                                const Type &right) const;

  template <template <class ResultType,
                      typename LeftCppType, bool left_nullable,
                      typename RightCppType, bool right_nullable> class OperatorType>
  UncheckedBinaryOperator* makeNumericBinaryOperatorOuterHelper(const Type &left,
                                                                const Type &right) const;

  template <template <class ResultType,
                      typename LeftCppType, bool left_nullable,
                      typename RightCppType, bool right_nullable> class OperatorType,
            typename LeftType, bool left_nullable>
  UncheckedBinaryOperator* makeNumericBinaryOperatorInnerHelper(const Type &left,
                                                                const Type &right) const;

  template <template <typename ResultType,
                      typename LeftCppType, bool left_nullable,
                      typename RightCppType, bool right_nullable> class OperatorType,
            typename ResultType,
            typename LeftCppType,
            typename RightCppType>
  UncheckedBinaryOperator* makeDateBinaryOperatorOuterHelper(const Type &left,
                                                             const Type &right) const;

  template <template <typename ResultType,
                      typename LeftCppType, bool left_nullable,
                      typename RightCppType, bool right_nullable> class OperatorType,
            typename ResultType,
            typename LeftCppType,
            bool left_nullable,
            typename RightCppType>
  UncheckedBinaryOperator* makeDateBinaryOperatorInnerHelper(const Type &right) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(ArithmeticBinaryOperation);
};

/** @} */

// ----------------------------------------------------------------------------
// Templated method implementations follow:

template <template <typename LeftCppType, typename RightCppType> class OperationFunctor>
TypedValue ArithmeticBinaryOperation::applyToCheckedIntegerHelper(
    const TypedValue &left,
    const Type &left_type,
    const TypedValue &right,
    const Type &right_type) const {
  DCHECK(left_type.getTypeID() == TypeID::kInt
         || left_type.getTypeID() == TypeID::kLong);
  DCHECK(right_type.getTypeID() == TypeID::kInt
         || right_type.getTypeID() == TypeID::kLong);

  const Type *unifier = TypeFactory::GetUnifyingType(left_type, right_type);
  DCHECK(unifier != nullptr);

  if (left.isNull() || right.isNull()) {
    return unifier->makeNullValue();
  }

  const TypedValue left_coerced = unifier->coerceValue(left, left_type);
  const TypedValue right_coerced = unifier->coerceValue(right, right_type);

  switch (unifier->getTypeID()) {
    case kInt: {
      OperationFunctor<IntType::cpptype, IntType::cpptype> operation_functor;
      return TypedValue(operation_functor(left_coerced.getLiteral<IntType::cpptype>(),
                                          right_coerced.getLiteral<IntType::cpptype>()));
    }
    case kLong: {
      OperationFunctor<LongType::cpptype, LongType::cpptype> operation_functor;
      return TypedValue(operation_functor(left_coerced.getLiteral<LongType::cpptype>(),
                                          right_coerced.getLiteral<LongType::cpptype>()));
    }
    default: {
      LOG(FATAL) << "Can not apply " << getName() << " to arguments of types "
                 << left_type.getName() << " and " << right_type.getName();
    }
  }

  QUICKSTEP_UNREACHABLE();
}

template <template <typename LeftCppType, typename RightCppType> class OperationFunctor>
TypedValue ArithmeticBinaryOperation::applyToCheckedNumericHelper(
    const TypedValue &left,
    const Type &left_type,
    const TypedValue &right,
    const Type &right_type) const {
  DCHECK_EQ(Type::kNumeric, left_type.getSuperTypeID());
  DCHECK_EQ(Type::kNumeric, right_type.getSuperTypeID());

  const Type *unifier = TypeFactory::GetUnifyingType(left_type, right_type);
  DCHECK(unifier != nullptr);

  if (left.isNull() || right.isNull()) {
    return unifier->makeNullValue();
  }

  const TypedValue left_coerced = unifier->coerceValue(left, left_type);
  const TypedValue right_coerced = unifier->coerceValue(right, right_type);

  switch (unifier->getTypeID()) {
    case kInt: {
      OperationFunctor<IntType::cpptype, IntType::cpptype> operation_functor;
      return TypedValue(operation_functor(left_coerced.getLiteral<IntType::cpptype>(),
                                          right_coerced.getLiteral<IntType::cpptype>()));
    }
    case kLong: {
      OperationFunctor<LongType::cpptype, LongType::cpptype> operation_functor;
      return TypedValue(operation_functor(left_coerced.getLiteral<LongType::cpptype>(),
                                          right_coerced.getLiteral<LongType::cpptype>()));
    }
    case kFloat: {
      OperationFunctor<FloatType::cpptype, FloatType::cpptype> operation_functor;
      return TypedValue(operation_functor(left_coerced.getLiteral<FloatType::cpptype>(),
                                          right_coerced.getLiteral<FloatType::cpptype>()));
    }
    case kDouble: {
      OperationFunctor<DoubleType::cpptype, DoubleType::cpptype> operation_functor;
      return TypedValue(operation_functor(left_coerced.getLiteral<DoubleType::cpptype>(),
                                          right_coerced.getLiteral<DoubleType::cpptype>()));
    }
    default: {
      LOG(FATAL) << "Can not apply " << getName() << " to arguments of types "
                 << left_type.getName() << " and " << right_type.getName();
    }
  }

  QUICKSTEP_UNREACHABLE();
}

template <template <class ResultType,
                    typename LeftCppType, bool left_nullable,
                    typename RightCppType, bool right_nullable> class OperatorType>
UncheckedBinaryOperator* ArithmeticBinaryOperation::makeIntegerBinaryOperatorOuterHelper(
    const Type &left,
    const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
      if (left.isNullable()) {
        return makeIntegerBinaryOperatorInnerHelper<OperatorType, IntType, true>(
            left, right);
      } else {
        return makeIntegerBinaryOperatorInnerHelper<OperatorType, IntType, false>(
            left, right);
      }
    case kLong:
      if (left.isNullable()) {
        return makeIntegerBinaryOperatorInnerHelper<OperatorType, LongType, true>(
            left, right);
      } else {
        return makeIntegerBinaryOperatorInnerHelper<OperatorType, LongType, false>(
            left, right);
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <class ResultType,
                    typename LeftCppType, bool left_nullable,
                    typename RightCppType, bool right_nullable> class OperatorType,
          typename LeftType, bool left_nullable>
UncheckedBinaryOperator* ArithmeticBinaryOperation::makeIntegerBinaryOperatorInnerHelper(
    const Type &left,
    const Type &right) const {
  switch (right.getTypeID()) {
    case kInt:
      if (right.isNullable()) {
        return new OperatorType<typename NumericTypeUnifier<LeftType, IntType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename IntType::cpptype, true>();
      } else {
        return new OperatorType<typename NumericTypeUnifier<LeftType, IntType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename IntType::cpptype, false>();
      }
    case kLong:
      if (right.isNullable()) {
        return new OperatorType<typename NumericTypeUnifier<LeftType, LongType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename LongType::cpptype, true>();
      } else {
        return new OperatorType<typename NumericTypeUnifier<LeftType, LongType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename LongType::cpptype, false>();
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <class ResultType,
                    typename LeftCppType, bool left_nullable,
                    typename RightCppType, bool right_nullable> class OperatorType>
UncheckedBinaryOperator* ArithmeticBinaryOperation::makeNumericBinaryOperatorOuterHelper(
    const Type &left,
    const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
      if (left.isNullable()) {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, IntType, true>(
            left, right);
      } else {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, IntType, false>(
            left, right);
      }
    case kLong:
      if (left.isNullable()) {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, LongType, true>(
            left, right);
      } else {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, LongType, false>(
            left, right);
      }
    case kFloat:
      if (left.isNullable()) {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, FloatType, true>(
            left, right);
      } else {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, FloatType, false>(
            left, right);
      }
    case kDouble:
      if (left.isNullable()) {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, DoubleType, true>(
            left, right);
      } else {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, DoubleType, false>(
            left, right);
      }
    case kDecimal:
      if (left.isNullable()) {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, DecimalType,
                                                    true>(left, right);
      } else {
        return makeNumericBinaryOperatorInnerHelper<OperatorType, DecimalType,
                                                    false>(left, right);
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <class ResultType,
                    typename LeftCppType, bool left_nullable,
                    typename RightCppType, bool right_nullable> class OperatorType,
          typename LeftType, bool left_nullable>
UncheckedBinaryOperator* ArithmeticBinaryOperation::makeNumericBinaryOperatorInnerHelper(
    const Type &left,
    const Type &right) const {
  switch (right.getTypeID()) {
    case kInt:
      if (right.isNullable()) {
        return new OperatorType<typename NumericTypeUnifier<LeftType, IntType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename IntType::cpptype, true>();
      } else {
        return new OperatorType<typename NumericTypeUnifier<LeftType, IntType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename IntType::cpptype, false>();
      }
    case kLong:
      if (right.isNullable()) {
        return new OperatorType<typename NumericTypeUnifier<LeftType, LongType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename LongType::cpptype, true>();
      } else {
        return new OperatorType<typename NumericTypeUnifier<LeftType, LongType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename LongType::cpptype, false>();
      }
    case kFloat:
      if (right.isNullable()) {
        return new OperatorType<typename NumericTypeUnifier<LeftType, FloatType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename FloatType::cpptype, true>();
      } else {
        return new OperatorType<typename NumericTypeUnifier<LeftType, FloatType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename FloatType::cpptype, false>();
      }
    case kDouble:
      if (right.isNullable()) {
        return new OperatorType<typename NumericTypeUnifier<LeftType, DoubleType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename DoubleType::cpptype, true>();
      } else {
        return new OperatorType<typename NumericTypeUnifier<LeftType, DoubleType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename DoubleType::cpptype, false>();
      }
    case kDecimal:
      if (right.isNullable()) {
        return new OperatorType<typename NumericTypeUnifier<LeftType, DecimalType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename DecimalType::cpptype, true>();
      } else {
        return new OperatorType<typename NumericTypeUnifier<LeftType, DecimalType>::type,
                                typename LeftType::cpptype, left_nullable,
                                typename DecimalType::cpptype, false>();
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <typename ResultType,
                    typename LeftCppType, bool left_nullable,
                    typename RightCppType, bool right_nullable> class OperatorType,
          typename ResultType,
          typename LeftCppType,
          typename RightCppType>
UncheckedBinaryOperator* ArithmeticBinaryOperation::makeDateBinaryOperatorOuterHelper(
    const Type &left,
    const Type &right) const {
  if (left.isNullable()) {
    return makeDateBinaryOperatorInnerHelper<OperatorType, ResultType, LeftCppType, true, RightCppType>(right);
  } else {
    return makeDateBinaryOperatorInnerHelper<OperatorType, ResultType, LeftCppType, false, RightCppType>(right);
  }
}

template <template <typename ResultType,
                    typename LeftCppType, bool left_nullable,
                    typename RightCppType, bool right_nullable> class OperatorType,
          typename ResultType,
          typename LeftCppType,
          bool left_nullable,
          typename RightCppType>
UncheckedBinaryOperator* ArithmeticBinaryOperation::makeDateBinaryOperatorInnerHelper(
    const Type &right) const {
  if (right.isNullable()) {
    return new OperatorType<ResultType, LeftCppType, left_nullable, RightCppType, true>();
  } else {
    return new OperatorType<ResultType, LeftCppType, left_nullable, RightCppType, false>();
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_ARITHMETIC_BINARY_OPERATION_HPP_
