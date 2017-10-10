/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_BASIC_COMPARISON_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_BASIC_COMPARISON_HPP_

#include <cstddef>
#include <cstdint>
#include <string>

#include "types/Type.hpp"
#include "types/TypeErrors.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

struct DateLit;
struct DatetimeIntervalLit;
struct DatetimeLit;
struct YearMonthIntervalLit;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Base class for the 6 basic comparisons.
 **/
class BasicComparison : public Comparison {
 public:
  bool canCompareTypes(const Type &left, const Type &right) const override;

  bool canComparePartialTypes(const Type *left_type,
                              const Type *right_type) const override;

 protected:
  explicit BasicComparison(const ComparisonID comparison_id)
      : Comparison(comparison_id) {
  }

  template <template <typename T> class ComparisonFunctor>
  bool compareTypedValuesCheckedHelper(const TypedValue &left,
                                       const Type &left_type,
                                       const TypedValue &right,
                                       const Type &right_type) const;

  template <template <typename LeftCppType, bool left_type_nullable,
                      typename RightCppType, bool right_type_nullable> class NumericComparator,
            template <bool left_nullable, bool left_null_terminated, bool left_longer,
                      bool right_nullable, bool right_null_terminated, bool right_longer> class StringComparator>
  UncheckedComparator* makeUncheckedComparatorForTypesHelper(const Type &left, const Type &right) const;

  template <template <typename LeftCppType, bool left_type_nullable,
                      typename RightCppType, bool right_type_nullable> class ComparatorType>
  UncheckedComparator* makeNumericComparatorOuterHelper(const Type &left,
                                                        const Type &right) const;

  template <template <typename LeftCppType, bool left_type_nullable,
                      typename RightCppType, bool right_type_nullable> class ComparatorType,
            typename LeftCppType,
            bool left_type_nullable>
  UncheckedComparator* makeNumericComparatorInnerHelper(const Type &left,
                                                        const Type &right) const;

  template <template <typename LeftCppType, bool left_type_nullable,
                      typename RightCppType, bool right_type_nullable> class ComparatorType>
  UncheckedComparator* makeDateComparatorOuterHelper(const Type &left,
                                                     const Type &right) const;

  template <template <typename LeftCppType, bool left_type_nullable,
                      typename RightCppType, bool right_type_nullable> class ComparatorType,
            typename CppType,
            bool left_type_nullable>
  UncheckedComparator* makeDateComparatorInnerHelper(const Type &left,
                                                     const Type &right) const;

  template <template <bool left_nullable,
                      bool left_null_terminated,
                      bool left_longer,
                      bool right_nullable,
                      bool right_null_terminated,
                      bool right_longer> class ComparatorType>
  UncheckedComparator* makeStringComparatorOuterHelper(const Type &left,
                                                       const Type &right) const;

  template <template <bool left_nullable,
                      bool left_null_terminated,
                      bool left_longer,
                      bool right_nullable,
                      bool right_null_terminated,
                      bool right_longer> class ComparatorType,
            bool left_nullable, bool left_null_terminated>
  UncheckedComparator* makeStringComparatorMiddleHelper(const Type &left,
                                                        const Type &right,
                                                        const std::size_t left_length) const;

  template <template <bool left_nullable,
                      bool left_null_terminated,
                      bool left_longer,
                      bool right_nullable,
                      bool right_null_terminated,
                      bool right_longer> class ComparatorType,
            bool left_nullable, bool left_null_terminated,
            bool right_nullable, bool right_null_terminated>
  UncheckedComparator* makeStringComparatorInnerHelper(const std::size_t left_length,
                                                       const std::size_t right_length) const;

 private:
  int strcmpHelper(const TypedValue &left, const TypedValue &right) const;

  DISALLOW_COPY_AND_ASSIGN(BasicComparison);
};

/** @} */

// ----------------------------------------------------------------------------
// Templated method implementations follow:

template <template <typename T> class ComparisonFunctor>
bool BasicComparison::compareTypedValuesCheckedHelper(const TypedValue &left,
                                                      const Type &left_type,
                                                      const TypedValue &right,
                                                      const Type &right_type) const {
  DCHECK_EQ(left.getTypeID(), left_type.getTypeID());
  DCHECK_EQ(right.getTypeID(), right_type.getTypeID());
  DCHECK(canCompareTypes(left_type, right_type));

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
          break;
        default: {
          LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                     << left_type.getName() << " and " << right_type.getName();
        }
      }
      break;
    }
    case kDate: {
      if (right.getTypeID() == kDate) {
        if (left.isNull() || right.isNull()) {
          return false;
        }

        ComparisonFunctor<DateLit> comparison_functor;
        return comparison_functor(left.getLiteral<DateLit>(), right.getLiteral<DateLit>());
      }
      LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                 << left_type.getName() << " and " << right_type.getName();
    }
    case kDatetime: {
      if (right.getTypeID() == kDatetime) {
        if (left.isNull() || right.isNull()) {
          return false;
        }

        ComparisonFunctor<DatetimeLit> comparison_functor;
        return comparison_functor(left.getLiteral<DatetimeLit>(), right.getLiteral<DatetimeLit>());
      }
      LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                 << left_type.getName() << " and " << right_type.getName();
    }
    case kDatetimeInterval: {
      if (right.getTypeID() == kDatetimeInterval) {
        if (left.isNull() || right.isNull()) {
          return false;
        }

        ComparisonFunctor<DatetimeIntervalLit> comparison_functor;
        return comparison_functor(left.getLiteral<DatetimeIntervalLit>(), right.getLiteral<DatetimeIntervalLit>());
      }
      LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                 << left_type.getName() << " and " << right_type.getName();
    }
    case kYearMonthInterval: {
      if (right.getTypeID() == kYearMonthInterval) {
        if (left.isNull() || right.isNull()) {
          return false;
        }

        ComparisonFunctor<YearMonthIntervalLit> comparison_functor;
        return comparison_functor(left.getLiteral<YearMonthIntervalLit>(), right.getLiteral<YearMonthIntervalLit>());
      }
      LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                 << left_type.getName() << " and " << right_type.getName();
    }
    case kChar:
    case kVarChar: {
      switch (right.getTypeID()) {
        case kChar:
        case kVarChar:
          if (left.isNull() || right.isNull()) {
            return false;
          }

          ComparisonFunctor<int> comparison_functor;
          return comparison_functor(strcmpHelper(left, right), 0);
        default: {
          LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                     << left_type.getName() << " and " << right_type.getName();
        }
      }
    }
    default: {
      LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                 << left_type.getName() << " and " << right_type.getName();
    }
  }

  // Comparison between numeric types.
  if (left.isNull() || right.isNull()) {
    return false;
  }

  const Type *unifier = TypeFactory::GetUnifyingType(left_type, right_type);
  DCHECK(unifier != nullptr);

  const TypedValue left_coerced = unifier->coerceTypedValue(left, left_type);
  const TypedValue right_coerced = unifier->coerceTypedValue(right, right_type);

  switch (unifier->getTypeID()) {
    case kInt: {
      ComparisonFunctor<int> comparison_functor;
      return comparison_functor(left_coerced.getLiteral<int>(),
                                right_coerced.getLiteral<int>());
    }
    case kLong: {
      ComparisonFunctor<std::int64_t> comparison_functor;
      return comparison_functor(left_coerced.getLiteral<std::int64_t>(),
                                right_coerced.getLiteral<std::int64_t>());
    }
    case kFloat: {
      ComparisonFunctor<float> comparison_functor;
      return comparison_functor(left_coerced.getLiteral<float>(),
                                right_coerced.getLiteral<float>());
    }
    case kDouble: {
      ComparisonFunctor<double> comparison_functor;
      return comparison_functor(left_coerced.getLiteral<double>(),
                                right_coerced.getLiteral<double>());
    }
    default: {
      LOG(FATAL) << "Comparison " << getName() << " can not be applied to types "
                 << left_type.getName() << " and " << right_type.getName();
    }
  }

  QUICKSTEP_UNREACHABLE();
}

template <template <typename LeftCppType, bool left_type_nullable,
                    typename RightCppType, bool right_type_nullable> class LiteralComparator,
          template <bool left_nullable, bool left_null_terminated, bool left_longer,
                    bool right_nullable, bool right_null_terminated, bool right_longer> class StringComparator>
UncheckedComparator* BasicComparison::makeUncheckedComparatorForTypesHelper(const Type &left,
                                                                            const Type &right) const {
  if (left.getSuperTypeID() == SuperTypeID::kNumeric && right.getSuperTypeID() == SuperTypeID::kNumeric) {
    return makeNumericComparatorOuterHelper<LiteralComparator>(left, right);
  } else if ((left.getTypeID() == kDate && right.getTypeID() == kDate)                         ||
             (left.getTypeID() == kDatetime && right.getTypeID() == kDatetime)                 ||
             (left.getTypeID() == kDatetimeInterval && right.getTypeID() == kDatetimeInterval) ||
             (left.getTypeID() == kYearMonthInterval && right.getTypeID() == kYearMonthInterval)) {
    return makeDateComparatorOuterHelper<LiteralComparator>(left, right);
  } else if (left.getSuperTypeID() == SuperTypeID::kAsciiString && right.getSuperTypeID() == SuperTypeID::kAsciiString) {
    return makeStringComparatorOuterHelper<StringComparator>(left, right);
  } else {
    throw OperationInapplicableToType(getName(), 2, kTypeNames[left.getTypeID()], kTypeNames[right.getTypeID()]);
  }
}

template <template <typename LeftCppType, bool left_type_nullable,
                    typename RightCppType, bool right_type_nullable> class ComparatorType>
UncheckedComparator* BasicComparison::makeNumericComparatorOuterHelper(
    const Type &left,
    const Type &right) const {
  switch (left.getTypeID()) {
    case kInt:
      if (left.isNullable()) {
        return makeNumericComparatorInnerHelper<ComparatorType, int, true>(left, right);
      } else {
        return makeNumericComparatorInnerHelper<ComparatorType, int, false>(left, right);
      }
    case kLong:
      if (left.isNullable()) {
        return makeNumericComparatorInnerHelper<ComparatorType, std::int64_t, true>(left, right);
      } else {
        return makeNumericComparatorInnerHelper<ComparatorType, std::int64_t, false>(left, right);
      }
    case kFloat:
      if (left.isNullable()) {
        return makeNumericComparatorInnerHelper<ComparatorType, float, true>(left, right);
      } else {
        return makeNumericComparatorInnerHelper<ComparatorType, float, false>(left, right);
      }
    case kDouble:
      if (left.isNullable()) {
        return makeNumericComparatorInnerHelper<ComparatorType, double, true>(left, right);
      } else {
        return makeNumericComparatorInnerHelper<ComparatorType, double, false>(left, right);
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <typename LeftCppType, bool left_type_nullable,
                    typename RightCppType, bool right_type_nullable> class ComparatorType,
          typename LeftCppType,
          bool left_type_nullable>
UncheckedComparator* BasicComparison::makeNumericComparatorInnerHelper(
    const Type &left,
    const Type &right) const {
  switch (right.getTypeID()) {
    case kInt:
      if (right.isNullable()) {
        return new ComparatorType<LeftCppType, left_type_nullable, int, true>();
      } else {
        return new ComparatorType<LeftCppType, left_type_nullable, int, false>();
      }
    case kLong:
      if (right.isNullable()) {
        return new ComparatorType<LeftCppType, left_type_nullable, std::int64_t, true>();
      } else {
        return new ComparatorType<LeftCppType, left_type_nullable, std::int64_t, false>();
      }
    case kFloat:
      if (right.isNullable()) {
        return new ComparatorType<LeftCppType, left_type_nullable, float, true>();
      } else {
        return new ComparatorType<LeftCppType, left_type_nullable, float, false>();
      }
    case kDouble:
      if (right.isNullable()) {
        return new ComparatorType<LeftCppType, left_type_nullable, double, true>();
      } else {
        return new ComparatorType<LeftCppType, left_type_nullable, double, false>();
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <typename LeftCppType, bool left_type_nullable,
                    typename RightCppType, bool right_type_nullable> class ComparatorType>
UncheckedComparator* BasicComparison::makeDateComparatorOuterHelper(
    const Type &left,
    const Type &right) const {
  switch (left.getTypeID()) {
    case kDate:
      if (left.isNullable()) {
        return makeDateComparatorInnerHelper<ComparatorType, DateLit, true>(left, right);
      } else {
        return makeDateComparatorInnerHelper<ComparatorType, DateLit, false>(left, right);
      }
    case kDatetime:
      if (left.isNullable()) {
        return makeDateComparatorInnerHelper<ComparatorType, DatetimeLit, true>(left, right);
      } else {
        return makeDateComparatorInnerHelper<ComparatorType, DatetimeLit, false>(left, right);
      }
    case kDatetimeInterval:
      if (left.isNullable()) {
        return makeDateComparatorInnerHelper<ComparatorType, DatetimeIntervalLit, true>(left, right);
      } else {
        return makeDateComparatorInnerHelper<ComparatorType, DatetimeIntervalLit, false>(left, right);
      }
    case kYearMonthInterval:
      if (left.isNullable()) {
        return makeDateComparatorInnerHelper<ComparatorType, YearMonthIntervalLit, true>(left, right);
      } else {
        return makeDateComparatorInnerHelper<ComparatorType, YearMonthIntervalLit, false>(left, right);
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <typename LeftCppType, bool left_type_nullable,
                    typename RightCppType, bool right_type_nullable> class ComparatorType,
          typename CppType,
          bool left_type_nullable>
UncheckedComparator* BasicComparison::makeDateComparatorInnerHelper(
    const Type &left,
    const Type &right) const {
  switch (right.getTypeID()) {
    case kDate:
    case kDatetime:
    case kDatetimeInterval:
    case kYearMonthInterval:
      if (right.isNullable()) {
        return new ComparatorType<CppType, left_type_nullable, CppType, true>();
      } else {
        return new ComparatorType<CppType, left_type_nullable, CppType, false>();
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <bool left_nullable,
                    bool left_null_terminated,
                    bool left_longer,
                    bool right_nullable,
                    bool right_null_terminated,
                    bool right_longer> class ComparatorType>
UncheckedComparator* BasicComparison::makeStringComparatorOuterHelper(
    const Type &left,
    const Type &right) const {
  switch (left.getTypeID()) {
    case kChar:
      if (left.isNullable()) {
        return makeStringComparatorMiddleHelper<ComparatorType, true, false>(left,
                                                                             right,
                                                                             left.maximumByteLength());
      } else {
        return makeStringComparatorMiddleHelper<ComparatorType, false, false>(left,
                                                                              right,
                                                                              left.maximumByteLength());
      }
    case kVarChar:
      if (left.isNullable()) {
        return makeStringComparatorMiddleHelper<ComparatorType, true, true>(left,
                                                                            right,
                                                                            left.maximumByteLength() - 1);
      } else {
        return makeStringComparatorMiddleHelper<ComparatorType, false, true>(left,
                                                                             right,
                                                                             left.maximumByteLength() - 1);
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <bool left_nullable, bool left_null_terminated, bool left_longer,
                    bool right_nullable, bool right_null_terminated, bool right_longer> class ComparatorType,
         bool left_nullable, bool left_null_terminated>
UncheckedComparator* BasicComparison::makeStringComparatorMiddleHelper(
    const Type &left,
    const Type &right,
    const std::size_t left_length) const {
  switch (right.getTypeID()) {
    case kChar:
      if (right.isNullable()) {
        return makeStringComparatorInnerHelper<ComparatorType,
                                               left_nullable, left_null_terminated,
                                               true, false>(left_length,
                                                            right.maximumByteLength());
      } else {
        return makeStringComparatorInnerHelper<ComparatorType,
                                               left_nullable, left_null_terminated,
                                               false, false>(left_length,
                                                             right.maximumByteLength());
      }
    case kVarChar:
      if (right.isNullable()) {
        return makeStringComparatorInnerHelper<ComparatorType,
                                               left_nullable, left_null_terminated,
                                               true, true>(left_length,
                                                           right.maximumByteLength() - 1);
      } else {
        return makeStringComparatorInnerHelper<ComparatorType,
                                               left_nullable, left_null_terminated,
                                               false, true>(left_length,
                                                            right.maximumByteLength() - 1);
      }
    default:
      throw OperationInapplicableToType(getName(), 2, left.getName().c_str(), right.getName().c_str());
  }
}

template <template <bool left_nullable, bool left_null_terminated, bool left_longer,
                    bool right_nullable, bool right_null_terminated, bool right_longer> class ComparatorType,
          bool left_nullable, bool left_null_terminated,
          bool right_nullable, bool right_null_terminated>
UncheckedComparator* BasicComparison::makeStringComparatorInnerHelper(
    const std::size_t left_length,
    const std::size_t right_length) const {
  if (left_length < right_length) {
    return new ComparatorType<left_nullable, left_null_terminated, false,
                              right_nullable, right_null_terminated, true>(left_length, right_length);
  } else if (left_length > right_length) {
    return new ComparatorType<left_nullable, left_null_terminated, true,
                              right_nullable, right_null_terminated, false>(left_length, right_length);
  } else {
    return new ComparatorType<left_nullable, left_null_terminated, false,
                              right_nullable, right_null_terminated, false>(left_length, right_length);
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_BASIC_COMPARISON_HPP_
