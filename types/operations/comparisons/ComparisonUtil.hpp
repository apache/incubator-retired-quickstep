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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_UTIL_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_UTIL_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>

#include "catalog/CatalogTypedefs.hpp"
#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/AsciiStringComparators.hpp"
#include "types/operations/comparisons/AsciiStringComparators-inl.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/LiteralComparators.hpp"
#include "types/operations/comparisons/LiteralComparators-inl.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace comparison_util_detail {

// This wraps an instance of the abstract base UncheckedComparator class and
// forwards calls from the inline -Inl methods to virtual methods of the base
// class. It is necessary for comparing types that are unknown to the macros
// and functions in this file (NOTE: it is vastly preferable to modify this
// file to be aware of a new type rather than relying on this slower fallback).
class GenericComparatorWrapper : public UncheckedComparator {
 public:
  explicit GenericComparatorWrapper(const UncheckedComparator &internal_comparator)
      : internal_comparator_(internal_comparator) {
  }

  ~GenericComparatorWrapper() {
  }

  inline bool compareTypedValues(const TypedValue &left, const TypedValue &right) const {
    return internal_comparator_.compareTypedValues(left, right);
  }

  inline bool compareTypedValuesInl(const TypedValue &left, const TypedValue &right) const {
    return internal_comparator_.compareTypedValues(left, right);
  }

  inline bool compareDataPtrs(const void *left, const void *right) const {
    return internal_comparator_.compareDataPtrs(left, right);
  }

  inline bool compareDataPtrsInl(const void *left, const void *right) const {
    return internal_comparator_.compareDataPtrs(left, right);
  }

  inline bool compareTypedValueWithDataPtr(const TypedValue &left,
                                           const void *right) const {
    return internal_comparator_.compareTypedValueWithDataPtr(left, right);
  }

  inline bool compareTypedValueWithDataPtrInl(const TypedValue &left,
                                              const void *right) const {
    return internal_comparator_.compareTypedValueWithDataPtr(left, right);
  }

  inline bool compareDataPtrWithTypedValue(const void *left,
                                           const TypedValue &right) const {
    return internal_comparator_.compareDataPtrWithTypedValue(left, right);
  }

  inline bool compareDataPtrWithTypedValueInl(const void *left,
                                              const TypedValue &right) const {
    return internal_comparator_.compareDataPtrWithTypedValue(left, right);
  }

 private:
  const UncheckedComparator &internal_comparator_;
};

}  // namespace comparison_util_detail

// TODO(chasseur): Maintaining the various Invoke* functions for new types and
// combinations of types is a bit of a pain. This is an instance of the
// double-dispatch problem that we are trying to resolve statically at compile
// time so that we can use fast inlined methods of comparators in generic
// algorithms. Try to think of a more clever and easily maintainable way of
// achieving the same ends.

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Invoke a generic functor (or lambda) on a less-than comparator for
 *        a given Type (ignoring nullability).
 *
 * @param type The Type to compare.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with a less-than UncheckedComparator for type as its argument. The
 *        provided UncheckedComparator will be of a concrete type and its
 *        inline methods will be usable.
 * @return The return value of functor's call operator applied to the less-than
 *         comparator.
 **/
template <typename FunctorT>
auto InvokeOnLessComparatorForTypeIgnoreNullability(const Type &type,
                                                    const FunctorT &functor) {
  switch (type.getTypeID()) {
    case kInt: {
      LessLiteralUncheckedComparator<int, false, int, false> comp;
      return functor(comp);
    }
    case kLong: {
      LessLiteralUncheckedComparator<std::int64_t, false,
                                     std::int64_t, false> comp;
      return functor(comp);
    }
    case kFloat: {
      LessLiteralUncheckedComparator<float, false,
                                     float, false> comp;
      return functor(comp);
    }
    case kDouble: {
      LessLiteralUncheckedComparator<double, false,
                                     double, false> comp;
      return functor(comp);
    }
    case kChar: {
      const std::size_t string_length
          = static_cast<const AsciiStringSuperType&>(type).getStringLength();
      LessAsciiStringUncheckedComparator<false, false, false,
                                         false, false, false>
          comp(string_length, string_length);
      return functor(comp);
    }
    case kVarChar: {
      LessAsciiStringUncheckedComparator<false, true, false,
                                         false, true, false>
          comp(0, 0);
      return functor(comp);
    }
    case kDate: {
      LessLiteralUncheckedComparator<DateLit, false,
                                     DateLit, false> comp;
      return functor(comp);
    }
    case kDatetime: {
      LessLiteralUncheckedComparator<DatetimeLit, false,
                                     DatetimeLit, false> comp;
      return functor(comp);
    }
    case kDatetimeInterval: {
      LessLiteralUncheckedComparator<DatetimeIntervalLit, false,
                                     DatetimeIntervalLit, false>
          comp;
      return functor(comp);
    }
    case kYearMonthInterval: {
      LessLiteralUncheckedComparator<YearMonthIntervalLit, false,
                                     YearMonthIntervalLit, false>
          comp;
      return functor(comp);
    }
    default: {
      LOG(WARNING)
          << "Developer warning: using generic fallback for unrecognized Type: "
          << type.getName();
      std::unique_ptr<UncheckedComparator> generic_less_comparator(
          ComparisonFactory::GetComparison(ComparisonID::kLess).makeUncheckedComparatorForTypes(
              type, type));
      comparison_util_detail::GenericComparatorWrapper comp(*generic_less_comparator);
      return functor(comp);
    }
  }
}

namespace comparison_util_detail {

// Fallback generic (slow) helper for
// InvokeOnLessComparatorForDifferentTypesIgnoreNullability() when that
// function doesn't know how to make a comparator for 'left_type' and
// 'right_type'.
template <typename FunctorT>
auto InvokeOnLessComparatorForDifferentTypesFallback(
    const Type &left_type,
    const Type &right_type,
    const FunctorT &functor) {
  LOG(WARNING)
      << "Developer warning: using generic fallback for unrecognized pair of "
      << "Types: " << left_type.getName() << ", " << right_type.getName();
  std::unique_ptr<UncheckedComparator> generic_less_comparator(
      ComparisonFactory::GetComparison(ComparisonID::kLess).makeUncheckedComparatorForTypes(
          left_type, right_type));
  GenericComparatorWrapper comp(*generic_less_comparator);
  return functor(comp);
}

}  // namespace comparison_util_detail

/**
 * @brief Invoke a generic functor (or lambda) on a less-than comparator for
 *        a given pair of Types, which may be different (ignoring nullability).
 *
 * @param left_type The Type on the left side of the less-than comparison.
 * @param right_type The Type on the right side of the less-than comparison.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking a single argument. The call operator will be invoked
 *        with a less-than UncheckedComparator for left_type and right_type as
 *        its argument. The provided UncheckedComparator will be of a concrete
 *        type and its inline methods will be usable.
 * @return The return value of functor's call operator applied to the less-than
 *         comparator.
 **/
template <typename FunctorT>
auto InvokeOnLessComparatorForDifferentTypesIgnoreNullability(
    const Type &left_type,
    const Type &right_type,
    const FunctorT &functor) {
  switch (left_type.getTypeID()) {
    case kInt: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<int, false, int, false> comp;
          return functor(comp);
        }
        case kLong: {
          LessLiteralUncheckedComparator<int, false, std::int64_t, false> comp;
          return functor(comp);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<int, false, float, false> comp;
          return functor(comp);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<int, false, double, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kLong: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<std::int64_t, false, int, false> comp;
          return functor(comp);
        }
        case kLong: {
          LessLiteralUncheckedComparator<std::int64_t, false,
                                         std::int64_t, false> comp;
          return functor(comp);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<std::int64_t, false,
                                         float, false> comp;
          return functor(comp);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<std::int64_t, false,
                                         double, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kFloat: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<float, false, int, false> comp;
          return functor(comp);
        }
        case kLong: {
          LessLiteralUncheckedComparator<float, false, std::int64_t, false> comp;
          return functor(comp);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<float, false, float, false> comp;
          return functor(comp);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<float, false, double, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDouble: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<double, false, int, false> comp;
          return functor(comp);
        }
        case kLong: {
          LessLiteralUncheckedComparator<double, false, std::int64_t, false> comp;
          return functor(comp);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<double, false, float, false> comp;
          return functor(comp);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<double, false, double, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kChar: {
      const std::size_t left_string_length
          = static_cast<const AsciiStringSuperType&>(left_type).getStringLength();
      switch (right_type.getTypeID()) {
        case kChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, false, true>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, false, true,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          }
        }
        case kVarChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, true, true>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, false, true,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          }
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kVarChar: {
      const std::size_t left_string_length
          = static_cast<const AsciiStringSuperType&>(left_type).getStringLength();
      switch (right_type.getTypeID()) {
        case kChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, false, true>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, true, true,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          }
        }
        case kVarChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, true, true>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, true, true,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          } else {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            return functor(comp);
          }
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDate: {
      switch (right_type.getTypeID()) {
        case kDate: {
          LessLiteralUncheckedComparator<DateLit, false,
                                         DateLit, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDatetime: {
      switch (right_type.getTypeID()) {
        case kDatetime: {
          LessLiteralUncheckedComparator<DatetimeLit, false,
                                         DatetimeLit, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDatetimeInterval: {
      switch (right_type.getTypeID()) {
        case kDatetimeInterval: {
          LessLiteralUncheckedComparator<DatetimeIntervalLit, false,
                                         DatetimeIntervalLit, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kYearMonthInterval: {
      switch (right_type.getTypeID()) {
        case kYearMonthInterval: {
          LessLiteralUncheckedComparator<YearMonthIntervalLit, false,
                                         YearMonthIntervalLit, false> comp;
          return functor(comp);
        }
        default:
          return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    default:
      return comparison_util_detail::InvokeOnLessComparatorForDifferentTypesFallback(
          left_type, right_type, functor);
  }
}

namespace comparison_util_detail {

// Fallback generic (slow) helper for
// InvokeOnBothLessComparatorForDifferentTypesIgnoreNullability() when that
// function doesn't know how to make comparators for 'left_type' and
// 'right_type'.
template <typename FunctorT>
auto InvokeOnBothLessComparatorsForDifferentTypesFallback(
    const Type &left_type,
    const Type &right_type,
    const FunctorT &functor) {
  LOG(WARNING)
      << "Developer warning: using generic fallback for unrecognized pair of "
      << "Types: " << left_type.getName() << ", " << right_type.getName();

  std::unique_ptr<UncheckedComparator> generic_less_comparator(
      ComparisonFactory::GetComparison(ComparisonID::kLess).makeUncheckedComparatorForTypes(
          left_type, right_type));
  GenericComparatorWrapper comp(*generic_less_comparator);

  std::unique_ptr<UncheckedComparator> generic_less_comparator_reversed(
      ComparisonFactory::GetComparison(ComparisonID::kLess).makeUncheckedComparatorForTypes(
          right_type, left_type));
  GenericComparatorWrapper comp_reversed(*generic_less_comparator_reversed);

  return functor(comp, comp_reversed);
}

}  // namespace comparison_util_detail

/**
 * @brief Invoke a generic functor (or lambda) on both directions of the
 *        less-than comparator for a given pair of Types, which may be
 *        different (ignoring nullability).
 *
 * @param left_type The Type on the left side of the less-than comparison.
 * @param right_type The Type on the right side of the less-than comparison.
 * @param functor A generic functor or lambda that has a templated call
 *        operator taking two arguments. The call operator will be invoked
 *        with a less-than UncheckedComparator for left_type < right_type as
 *        its first argument, and a comparator for right_type < left_type as
 *        its seconds argument. The provided UncheckedComparators will be of
 *        concrete types and their inline methods will be usable.
 * @return The return value of functor's call operator applied to the less-than
 *         comparator.
 **/
template <typename FunctorT>
auto InvokeOnBothLessComparatorsForDifferentTypesIgnoreNullability(
    const Type &left_type,
    const Type &right_type,
    const FunctorT &functor) {
  switch (left_type.getTypeID()) {
    case kInt: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<int, false, int, false> comp;
          return functor(comp, comp);
        }
        case kLong: {
          LessLiteralUncheckedComparator<int, false, std::int64_t, false> comp;
          LessLiteralUncheckedComparator<std::int64_t, false, int, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<int, false, float, false> comp;
          LessLiteralUncheckedComparator<float, false, int, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<int, false, double, false> comp;
          LessLiteralUncheckedComparator<double, false, int, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kLong: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<std::int64_t, false, int, false> comp;
          LessLiteralUncheckedComparator<int, false, std::int64_t, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kLong: {
          LessLiteralUncheckedComparator<std::int64_t, false,
                                         std::int64_t, false> comp;
          return functor(comp, comp);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<std::int64_t, false,
                                         float, false> comp;
          LessLiteralUncheckedComparator<float, false,
                                         std::int64_t, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<std::int64_t, false,
                                         double, false> comp;
          LessLiteralUncheckedComparator<double, false,
                                         std::int64_t, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kFloat: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<float, false, int, false> comp;
          LessLiteralUncheckedComparator<int, false, float, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kLong: {
          LessLiteralUncheckedComparator<float, false, std::int64_t, false> comp;
          LessLiteralUncheckedComparator<std::int64_t, false, float, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<float, false, float, false> comp;
          return functor(comp, comp);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<float, false, double, false> comp;
          LessLiteralUncheckedComparator<double, false, float, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDouble: {
      switch (right_type.getTypeID()) {
        case kInt: {
          LessLiteralUncheckedComparator<double, false, int, false> comp;
          LessLiteralUncheckedComparator<int, false, double, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kLong: {
          LessLiteralUncheckedComparator<double, false, std::int64_t, false> comp;
          LessLiteralUncheckedComparator<std::int64_t, false, double, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kFloat: {
          LessLiteralUncheckedComparator<double, false, float, false> comp;
          LessLiteralUncheckedComparator<float, false, double, false> comp_reversed;
          return functor(comp, comp_reversed);
        }
        case kDouble: {
          LessLiteralUncheckedComparator<double, false, double, false> comp;
          return functor(comp, comp);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kChar: {
      const std::size_t left_string_length
          = static_cast<const AsciiStringSuperType&>(left_type).getStringLength();
      switch (right_type.getTypeID()) {
        case kChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, false, true>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, false, true,
                                               false, false, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, false, true,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, false, true>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, false, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          }
        }
        case kVarChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, true, true>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, true, true,
                                               false, false, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, false, true,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, false, true>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else {
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, false, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          }
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kVarChar: {
      const std::size_t left_string_length
          = static_cast<const AsciiStringSuperType&>(left_type).getStringLength();
      switch (right_type.getTypeID()) {
        case kChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, false, true>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, false, true,
                                               false, true, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, true, true,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, true, true>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, false, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, false, false,
                                               false, true, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          }
        }
        case kVarChar: {
          const std::size_t right_string_length
              = static_cast<const AsciiStringSuperType&>(right_type).getStringLength();
          if (left_string_length < right_string_length) {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, true, true>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, true, true,
                                               false, true, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else if (right_string_length < left_string_length) {
            LessAsciiStringUncheckedComparator<false, true, true,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, true, true>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          } else {
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, true, false>
                comp(left_string_length, right_string_length);
            LessAsciiStringUncheckedComparator<false, true, false,
                                               false, true, false>
                comp_reversed(right_string_length, left_string_length);
            return functor(comp, comp_reversed);
          }
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDate: {
      switch (right_type.getTypeID()) {
        case kDate: {
          LessLiteralUncheckedComparator<DateLit, false,
                                         DateLit, false> comp;
          return functor(comp, comp);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDatetime: {
      switch (right_type.getTypeID()) {
        case kDatetime: {
          LessLiteralUncheckedComparator<DatetimeLit, false,
                                         DatetimeLit, false> comp;
          return functor(comp, comp);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kDatetimeInterval: {
      switch (right_type.getTypeID()) {
        case kDatetimeInterval: {
          LessLiteralUncheckedComparator<DatetimeIntervalLit, false,
                                         DatetimeIntervalLit, false> comp;
          return functor(comp, comp);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    case kYearMonthInterval: {
      switch (right_type.getTypeID()) {
        case kYearMonthInterval: {
          LessLiteralUncheckedComparator<YearMonthIntervalLit, false,
                                         YearMonthIntervalLit, false> comp;
          return functor(comp, comp);
        }
        default:
          return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
              left_type, right_type, functor);
      }
    }
    default:
      return comparison_util_detail::InvokeOnBothLessComparatorsForDifferentTypesFallback(
          left_type, right_type, functor);
  }
}

/**
 * @brief A functor object which wraps std::lower_bound.
 **/
template <class IteratorT, class T, class Compare>
struct LowerBoundFunctor {
  static constexpr bool literal_on_left = false;

  /**
   * @brief Calls std::lower_bound to do a binary search for a value in the
   *        specified range of iterators.
   *
   * @param first The beginning of the range to search (range must be sorted).
   * @param last One past the end of the range to search.
   * @param val The value to find the lower-bound position for.
   * @param comp A comparison functor which compares val with the values at
   *        iterators in the range from first to last.
   * @return The position in the range from first to last which is the lower
   *         bound for val.
   **/
  inline IteratorT operator()(IteratorT first,
                              IteratorT last,
                              const T &val,
                              const Compare &comp) {
    return std::lower_bound(first, last, val, comp);
  }
};

/**
 * @brief A functor object which wraps std::upper_bound.
 **/
template <class IteratorT, class T, class Compare>
struct UpperBoundFunctor {
  static constexpr bool literal_on_left = true;

  /**
   * @brief Calls std::upper_bound to do a binary search for a value in the
   *        specified range of iterators.
   *
   * @param first The beginning of the range to search (range must be sorted).
   * @param last One past the end of the range to search.
   * @param val The value to find the upper-bound position for.
   * @param comp A comparison functor which compares val with the values at
   *        iterators in the range from first to last.
   * @return The position in the range from first to last which is the upper
   *         bound for val.
   **/
  inline IteratorT operator()(IteratorT first,
                              IteratorT last,
                              const T &val,
                              const Compare &comp) {
    return std::upper_bound(first, last, val, comp);
  }
};

/**
 * @brief Find a boundary according to BoundFunctorT for untyped (i.e. void*)
 *        values of a specified type.
 *
 * @param type The Type of values to compare.
 * @param begin_it The beginning of the range of values to search.
 * @param end_it One past the end of the range of values to search.
 * @param value The literal value to find a bound for.
 * @return The position in the range from begin_it to end_it which is the bound
 *         for value according to BoundFunctorT.
 **/
template <typename IteratorT,
          template <class BoundIteratorT, class T, class Compare> class BoundFunctorT>
inline IteratorT GetBoundForUntypedValue(const Type &type,
                                         IteratorT begin_it,
                                         IteratorT end_it,
                                         const void *value) {
  return InvokeOnLessComparatorForTypeIgnoreNullability(
      type,
      [&](const auto &comp) -> IteratorT {  // NOLINT(build/c++11)
    STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>
        stl_comp(comp);
    return BoundFunctorT<IteratorT,
                         const void*,
                         decltype(stl_comp)>()(begin_it, end_it, value, stl_comp);
  });
}

/**
 * @brief Similar to GetBoundForUntypedValue, but allows finding bounds for a
 *        value which is of a different type than the values in the range
 *        being searched.
 *
 * @param type The Type of values in the range from begin_it to end_it.
 * @param begin_it The beginning of the range of values to search.
 * @param end_it One past the end of the range of values to search.
 * @param value The literal value to find a bound for, which need not be an
 *        instance of type so long as it is comparable with type.
 * @param value_type The Type that value belongs to.
 * @return The position in the range from begin_it to end_it which is the bound
 *         for value according to BoundFunctorT.
 **/
template <typename IteratorT,
          template <class BoundIteratorT, class T, class Compare> class BoundFunctorT>
IteratorT GetBoundForDifferentTypedValue(
    const Type &type,
    IteratorT begin_it,
    IteratorT end_it,
    const TypedValue &value,
    const Type &value_type) {
  constexpr bool literal_on_left
      = BoundFunctorT<IteratorT,
                      const void*,
                      STLUncheckedComparatorWrapper<UncheckedComparator>>::literal_on_left;
  if (literal_on_left) {
    return InvokeOnLessComparatorForDifferentTypesIgnoreNullability(
        value_type, type,
        [&](const auto &comp) -> IteratorT {  // NOLINT(build/c++11)
      STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>
          stl_comp(comp);
      return BoundFunctorT<IteratorT,
                           const void*,
                           decltype(stl_comp)>()(begin_it, end_it, value.getDataPtr(), stl_comp);
    });
  } else {
    return InvokeOnLessComparatorForDifferentTypesIgnoreNullability(
        type,
        value_type,
        [&](const auto &comp) -> IteratorT {  // NOLINT(build/c++11)
      STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>
          stl_comp(comp);
      return BoundFunctorT<IteratorT,
                           const void*,
                           decltype(stl_comp)>()(begin_it, end_it, value.getDataPtr(), stl_comp);
    });
  }
}

/**
 * @brief Determine if two untyped values are equal.
 *
 * @param type The Type which the untyped values should be interpreted as.
 * @param left The first value to check for equality.
 * @param right The second value to check for equality.
 * @return Whether left is equal to right (treating both values as instances of
 *         type).
 **/
inline bool CheckUntypedValuesEqual(const Type &type, const void *left, const void *right) {
  switch (type.getTypeID()) {
    case kInt:
      return STLLiteralEqual<int>()(left, right);
    case kLong:
      return STLLiteralEqual<std::int64_t>()(left, right);
    case kFloat:
      return STLLiteralEqual<float>()(left, right);
    case kDouble:
      return STLLiteralEqual<double>()(left, right);
    case kChar:
      return STLCharEqual(static_cast<const AsciiStringSuperType&>(type).getStringLength())(left, right);
    case kVarChar:
      return STLVarCharEqual()(left, right);
    case kDate:
      return STLLiteralEqual<DateLit>()(left, right);
    case kDatetime:
      return STLLiteralEqual<DatetimeLit>()(left, right);
    case kDatetimeInterval:
      return STLLiteralEqual<DatetimeIntervalLit>()(left, right);
    case kYearMonthInterval:
      return STLLiteralEqual<YearMonthIntervalLit>()(left, right);
    default: {
      DEV_WARNING("Unrecognized type encountered in CheckUntypedValuesEqual() "
                  "from types/operations/comparisons/ComparisonUtil.hpp. This file should be updated "
                  "to be aware of the new type.");
      std::unique_ptr<UncheckedComparator> generic_comp(
          ComparisonFactory::GetComparison(ComparisonID::kEqual)
              .makeUncheckedComparatorForTypes(type, type));
      return generic_comp->compareDataPtrs(left, right);
    }
  }
}

/**
 * @brief Sort a range of values into ascending order.
 *
 * @param type The Type which values in the range from begin_it to end_it
 *        should be treated as.
 * @param begin_it The beginning of the range of values to sort.
 * @param end_it One past the end of the range of values to sort.
 **/
template <typename IteratorT>
void SortValues(const Type &type,
                IteratorT begin_it,
                IteratorT end_it) {
  InvokeOnLessComparatorForTypeIgnoreNullability(
      type,
      [&](const auto &comp) {  // NOLINT(build/c++11)
    STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>
        stl_comp(comp);
    std::sort(begin_it, end_it, stl_comp);
  });
}

/**
 * @brief An adapter which wraps a ComparatorT comparison functor for objects
 *        of type ValueT, which supports a method getDataPtr() to return an
 *        untyped pointer to an underlying value.
 **/
template <typename ValueT, typename ComparatorT>
class ComparatorAdapter {
 public:
  /**
   * @brief Constructor.
   *
   * @param comparator The comparison functor to adapt.
   **/
  explicit ComparatorAdapter(const ComparatorT &comparator = ComparatorT())
      : comparator_(comparator) {
  }

  /**
   * @brief Apply the comparator provided to the constructor to the underlying
   *        values represented by left and right.
   *
   * @param left The left value to compare.
   * @param right The right value to compare.
   * @return The result of applying the adapted comparator to left and right.
   **/
  inline bool operator()(const ValueT &left, const ValueT &right) {
    return comparator_(left.getDataPtr(), right.getDataPtr());
  }

 private:
  ComparatorT comparator_;
};

/**
 * @brief Sort a range of wrapped values (which provide a getDataPtr() method
 *        to access underlying data) into ascending order.
 *
 * @note This always sorts the range from begin_it to end_it into ascending
 *       order, but the underlying sequence can be sorted in descending order
 *       by using reverse iterators.
 *
 * @param type The Type which values should be interpreted as.
 * @param begin_it The beginning of the range of wrapped values to sort.
 * @param end_it One past the end of the range of wrapped values to sort.
 **/
template <typename ValueT, typename IteratorT>
void SortWrappedValues(const Type &type,
                       IteratorT begin_it,
                       IteratorT end_it) {
  InvokeOnLessComparatorForTypeIgnoreNullability(
      type,
      [&](const auto &comp) {  // NOLINT(build/c++11)
    std::sort(
        begin_it,
        end_it,
        ComparatorAdapter<
            ValueT,
            STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>>(
                STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>(
                    comp)));
  });
}

/**
 * @brief Sort a range of wrapped values (which provide a getDataPtr() method
 *        to access underlying data).
 *
 * @note This always sorts the range from begin_it to end_it into ascending
 *       order, but the underlying sequence can be sorted in descending order
 *       by using reverse iterators.
 *
 * @param type The Type which values should be interpreted as.
 * @param begin_it The beginning of the range of wrapped values to sort.
 * @param end_it One past the end of the range of wrapped values to sort.
 **/
template <typename ValueT, typename IteratorT>
void StableSortWrappedValues(const Type &type,
                             IteratorT begin_it,
                             IteratorT end_it) {
  InvokeOnLessComparatorForTypeIgnoreNullability(
      type,
      [&](const auto &comp) {  // NOLINT(build/c++11)
    std::stable_sort(
        begin_it,
        end_it,
        ComparatorAdapter<
            ValueT,
            STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>>(
                STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>(
                    comp)));
  });
}

/**
 * @brief Find a boundary according to BoundFunctorT for wrapped values (which
 *        provide a getDataPtr() method to access underlying data, and a
 *        a single-argument constructor which takes a const void* pointing to a
 *        literal value).
 *
 * @param type The Type of values to compare.
 * @param begin_it The beginning of the range of values to search.
 * @param end_it One past the end of the range of values to search.
 * @param value The literal value to find a bound for.
 * @return The position in the range from begin_it to end_it which is the bound
 *         for value according to BoundFunctorT.
 **/
template <typename ValueT,
          typename IteratorT,
          template <class BoundIteratorT, class T, class Compare> class BoundFunctorT>
inline IteratorT GetBoundForWrappedValues(const Type &type,
                                          IteratorT begin_it,
                                          IteratorT end_it,
                                          const void *value) {
  return InvokeOnLessComparatorForTypeIgnoreNullability(
      type,
      [&](const auto &comp) -> IteratorT {  // NOLINT(build/c++11)
    STLUncheckedComparatorWrapper<typename std::remove_reference<decltype(comp)>::type>
        stl_comp(comp);
    ComparatorAdapter<ValueT, decltype(stl_comp)> stl_comp_adapter(stl_comp);
    return BoundFunctorT<IteratorT,
                         ValueT,
                         decltype(stl_comp_adapter)>()(begin_it,
                                                       end_it,
                                                       ValueT(value),
                                                       stl_comp_adapter);
  });
}

/**
 * @brief An adapter which compares tuples according to the value of a
 *        particular attribute.
 **/
template <typename AttributeComparatorT>
class STLTupleComparator {
 public:
  /**
   * @brief Constructor.
   *
   * @param attr_id The ID of the attribute which Tuples should be compared by.
   * @param attr_comp A comparison functor which can compare values of the
   *        attribute specified by attr_id.
   **/
  STLTupleComparator(const attribute_id attr_id,
                     const AttributeComparatorT &attr_comp)
      : attr_id_(attr_id),
        attr_comp_(attr_comp) {
  }

  inline bool operator() (const Tuple &left, const Tuple &right) const {
    return attr_comp_(left.getAttributeValue(attr_id_),
                      right.getAttributeValue(attr_id_));
  }

  inline bool operator() (const Tuple *left, const Tuple *right) const {
    return attr_comp_(left->getAttributeValue(attr_id_),
                      right->getAttributeValue(attr_id_));
  }

 private:
  const attribute_id attr_id_;
  AttributeComparatorT attr_comp_;
};

/**
 * @brief Sort a range of Tuples into ascending order by comparing values of a
 *        particular attribute.
 *
 * @param sort_attr_id The ID of the attribute to order tuples by.
 * @param sort_attr_type The Type of the attribute indicated by sort_attr_id.
 * @param begin_it The beginning of the range of tuples to sort.
 * @param end_it One past the end of the range of tuples to sort.
 **/
template <typename IteratorT>
void SortTuples(const attribute_id sort_attr_id,
                const Type &sort_attr_type,
                IteratorT begin_it,
                IteratorT end_it) {
  DCHECK(!sort_attr_type.isNullable());
  InvokeOnLessComparatorForTypeIgnoreNullability(
      sort_attr_type,
      [&](const auto &comp) {  // NOLINT(build/c++11)
    std::sort(
        begin_it,
        end_it,
        STLTupleComparator<
            STLUncheckedComparatorWrapper<
                typename std::remove_reference<decltype(comp)>::type>>(
            sort_attr_id,
            STLUncheckedComparatorWrapper<
                typename std::remove_reference<decltype(comp)>::type>(comp)));
  });
}

/**
 * @brief Get a special value for a Type that compares after all other values
 *        for the type.
 * @warning In general, it is still possible for user-supplied data to compare
 *          as equal to a value generated by this method.
 *
 * @param type The Type to generate a "last" value for.
 * @return A value that comes after all other values for type.
 **/
inline TypedValue GetLastValueForType(const Type &type) {
  static const char kLastString[] = "\x7F";  // ASCII "DEL" control code.

  switch (type.getTypeID()) {
    case kInt:
      return TypedValue(std::numeric_limits<int>::max());
    case kLong:
      return TypedValue(std::numeric_limits<std::int64_t>::max());
    case kFloat:
      return TypedValue(std::numeric_limits<float>::max());
    case kDouble:
      return TypedValue(std::numeric_limits<double>::max());
    case kChar:
      return TypedValue(kChar, kLastString, 2);
    case kVarChar:
      return TypedValue(kVarChar, kLastString, 2);
    case kDate: {
      return TypedValue(DateLit::Create(99999, 12, 31));
    }
    case kDatetime: {
      DatetimeLit lit;
      lit.ticks = std::numeric_limits<std::int64_t>::max();
      return TypedValue(lit);
    }
    case kDatetimeInterval: {
      DatetimeIntervalLit lit;
      lit.interval_ticks = std::numeric_limits<std::int64_t>::max();
      return TypedValue(lit);
    }
    case kYearMonthInterval: {
      YearMonthIntervalLit lit;
      lit.months = std::numeric_limits<std::int64_t>::max();
      return TypedValue(lit);
    }
    default:
      FATAL_ERROR("Unknown Type encountered in GetLastValueForType() in "
                  "types/operations/comparisons/ComparisonUtil.hpp");
  }
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARISON_UTIL_HPP_
