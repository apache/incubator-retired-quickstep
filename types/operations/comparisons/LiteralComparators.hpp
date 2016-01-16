/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_LITERAL_COMPARATORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_LITERAL_COMPARATORS_HPP_

#include <functional>

#include "catalog/CatalogTypedefs.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"

namespace quickstep {

class ColumnVector;
class TupleIdSequence;
class ValueAccessor;

/** \addtogroup Types
 *  @{
 */

// We use these functors instead of the standard-library ones, because the
// standard-library functors in <functional> have to be instantiated for the
// most specific argument type, which would unnecessisarily multiply the number
// of distinct template instantiations of Comparators by 7.
template <typename LeftArgument, typename RightArgument> struct EqualFunctor
    : public std::binary_function<LeftArgument, RightArgument, bool> {
  inline bool operator() (const LeftArgument &left, const RightArgument &right) const {
    return left == right;
  }
};

template <typename LeftArgument, typename RightArgument> struct NotEqualFunctor
    : public std::binary_function<LeftArgument, RightArgument, bool> {
  inline bool operator() (const LeftArgument &left, const RightArgument &right) const {
    return left != right;
  }
};

template <typename LeftArgument, typename RightArgument> struct LessFunctor
    : public std::binary_function<LeftArgument, RightArgument, bool> {
  inline bool operator() (const LeftArgument &left, const RightArgument &right) const {
    return left < right;
  }
};

template <typename LeftArgument, typename RightArgument> struct LessOrEqualFunctor
    : public std::binary_function<LeftArgument, RightArgument, bool> {
  inline bool operator() (const LeftArgument &left, const RightArgument &right) const {
    return left <= right;
  }
};

template <typename LeftArgument, typename RightArgument> struct GreaterFunctor
    : public std::binary_function<LeftArgument, RightArgument, bool> {
  inline bool operator() (const LeftArgument &left, const RightArgument &right) const {
    return left > right;
  }
};

template <typename LeftArgument, typename RightArgument> struct GreaterOrEqualFunctor
    : public std::binary_function<LeftArgument, RightArgument, bool> {
  inline bool operator() (const LeftArgument &left, const RightArgument &right) const {
    return left >= right;
  }
};

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
class LiteralUncheckedComparator : public UncheckedComparator {
 public:
  LiteralUncheckedComparator() = default;
  LiteralUncheckedComparator(const LiteralUncheckedComparator &orig) = default;
  ~LiteralUncheckedComparator() override = default;

  inline bool compareTypedValues(const TypedValue &left,
                                 const TypedValue &right) const override {
    return compareTypedValuesInl(left, right);
  }

  inline bool compareTypedValuesInl(const TypedValue &left, const TypedValue &right) const {
    if ((left_nullable && left.isNull()) || (right_nullable && right.isNull())) {
      return false;
    }
    return comparison_functor_(left.getLiteral<LeftCppType>(),
                               right.getLiteral<RightCppType>());
  }

  inline bool compareDataPtrs(const void *left,
                              const void *right) const override {
    return compareDataPtrsInl(left, right);
  }

  inline bool compareDataPtrsInl(const void *left, const void *right) const {
    if ((left_nullable && (left == nullptr)) || (right_nullable && (right == nullptr))) {
      return false;
    }
    return comparison_functor_(*static_cast<const LeftCppType*>(left),
                               *static_cast<const RightCppType*>(right));
  }

  inline bool compareTypedValueWithDataPtr(const TypedValue &left,
                                           const void *right) const override {
    return compareTypedValueWithDataPtrInl(left, right);
  }

  inline bool compareTypedValueWithDataPtrInl(const TypedValue &left, const void *right) const {
    if ((left_nullable && left.isNull()) || (right_nullable && (right == nullptr))) {
      return false;
    }
    return comparison_functor_(left.getLiteral<LeftCppType>(),
                               *static_cast<const RightCppType*>(right));
  }

  inline bool compareDataPtrWithTypedValue(const void *left,
                                           const TypedValue &right) const override {
    return compareDataPtrWithTypedValueInl(left, right);
  }

  inline bool compareDataPtrWithTypedValueInl(const void *left, const TypedValue &right) const {
    if ((left_nullable && (left == nullptr)) || (right_nullable && right.isNull())) {
      return false;
    }
    return comparison_functor_(*static_cast<const LeftCppType*>(left),
                               right.getLiteral<RightCppType>());
  }

  TupleIdSequence* compareColumnVectors(
      const ColumnVector &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override;

  TupleIdSequence* compareColumnVectorAndStaticValue(
      const ColumnVector &left,
      const TypedValue &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override {
    return compareColumnVectorAndStaticValueHelper<true>(left, right, filter, existence_bitmap);
  }

  TupleIdSequence* compareStaticValueAndColumnVector(
      const TypedValue &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override {
    return compareColumnVectorAndStaticValueHelper<false>(right, left, filter, existence_bitmap);
  }

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  TupleIdSequence* compareSingleValueAccessor(
      ValueAccessor *accessor,
      const attribute_id left_id,
      const attribute_id right_id,
      const TupleIdSequence *filter) const override;

  TupleIdSequence* compareValueAccessorAndStaticValue(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const TypedValue &right,
      const TupleIdSequence *filter) const override {
    return compareValueAccessorAndStaticValueHelper<true>(left_accessor, left_id, right, filter);
  }

  TupleIdSequence* compareStaticValueAndValueAccessor(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter) const override {
    return compareValueAccessorAndStaticValueHelper<false>(right_accessor, right_id, left, filter);
  }

  TupleIdSequence* compareColumnVectorAndValueAccessor(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override {
    return compareColumnVectorAndValueAccessorHelper<true>(left,
                                                           right_accessor,
                                                           right_id,
                                                           filter,
                                                           existence_bitmap);
  }

  TupleIdSequence* compareValueAccessorAndColumnVector(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override {
    return compareColumnVectorAndValueAccessorHelper<false>(right,
                                                            left_accessor,
                                                            left_id,
                                                            filter,
                                                            existence_bitmap);
  }

  TypedValue accumulateValueAccessor(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  TypedValue accumulateColumnVector(
      const TypedValue &current,
      const ColumnVector &column_vector) const override;

 private:
  template <bool column_vector_on_left>
  TupleIdSequence* compareColumnVectorAndStaticValueHelper(
      const ColumnVector &column_vector,
      const TypedValue &static_value,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  template <bool value_accessor_on_left>
  TupleIdSequence* compareValueAccessorAndStaticValueHelper(
      ValueAccessor *accessor,
      const attribute_id value_accessor_attr_id,
      const TypedValue &static_value,
      const TupleIdSequence *filter) const;

  template <bool column_vector_on_left>
  TupleIdSequence* compareColumnVectorAndValueAccessorHelper(
      const ColumnVector &column_vector,
      ValueAccessor *accessor,
      const attribute_id value_accessor_attr_id,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  template <bool arguments_in_order>
  inline bool compareDataPtrsHelper(const void *left, const void *right) const {
    return comparison_functor_(*static_cast<const LeftCppType*>(arguments_in_order ? left : right),
                               *static_cast<const RightCppType*>(arguments_in_order ? right : left));
  }

  ComparisonFunctor<LeftCppType, RightCppType> comparison_functor_;
};

/**
 * @brief The equal UncheckedComparator for seven literals
 *        (int, std::int64_t, float, double, DatetimeLit, DatetimeIntervalLit, and YearMonthIntervalLit).
 **/
template <typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using EqualLiteralUncheckedComparator = LiteralUncheckedComparator<EqualFunctor,
                                                                   LeftCppType, left_nullable,
                                                                   RightCppType, right_nullable>;

/**
 * @brief The not-equal UncheckedComparator for seven literals
 *        (int, std::int64_t, float, double, DatetimeLit, DatetimeIntervalLit, and YearMonthIntervalLit).
 **/
template <typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using NotEqualLiteralUncheckedComparator = LiteralUncheckedComparator<NotEqualFunctor,
                                                                      LeftCppType, left_nullable,
                                                                      RightCppType, right_nullable>;

/**
 * @brief The less-than UncheckedComparator for seven literals
 *        (int, std::int64_t, float, double, DatetimeLit, DatetimeIntervalLit, and YearMonthIntervalLit).
 **/
template <typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using LessLiteralUncheckedComparator = LiteralUncheckedComparator<LessFunctor,
                                                                  LeftCppType, left_nullable,
                                                                  RightCppType, right_nullable>;

/**
 * @brief The less-than-or-equal UncheckedComparator for seven literals
 *        (int, std::int64_t, float, double, DatetimeLit, DatetimeIntervalLit, and YearMonthIntervalLit).
 **/
template <typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using LessOrEqualLiteralUncheckedComparator = LiteralUncheckedComparator<LessOrEqualFunctor,
                                                                         LeftCppType, left_nullable,
                                                                         RightCppType, right_nullable>;

/**
 * @brief The greater-than UncheckedComparator for seven literals
 *        (int, std::int64_t, float, double, DatetimeLit, DatetimeIntervalLit, and YearMonthIntervalLit).
 **/
template <typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using GreaterLiteralUncheckedComparator = LiteralUncheckedComparator<GreaterFunctor,
                                                                     LeftCppType, left_nullable,
                                                                     RightCppType, right_nullable>;

/**
 * @brief The greater-than-or-equal UncheckedComparator for seven literals
 *        (int, std::int64_t, float, double, DatetimeLit, DatetimeIntervalLit, and YearMonthIntervalLit).
 **/
template <typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
using GreaterOrEqualLiteralUncheckedComparator = LiteralUncheckedComparator<GreaterOrEqualFunctor,
                                                                            LeftCppType, left_nullable,
                                                                            RightCppType, right_nullable>;

// Shorthand for STL-compatible less-comparator for a given non-nullable literal type.
// Includes a handy default constructor.
template <typename LeftCppType, typename RightCppType = LeftCppType>
class STLLiteralLess : public STLUncheckedComparatorWrapper<
    LessLiteralUncheckedComparator<LeftCppType, false, RightCppType, false>> {
 public:
  STLLiteralLess()
      : STLUncheckedComparatorWrapper<LessLiteralUncheckedComparator<LeftCppType, false,
                                                                     RightCppType, false>>(
          LessLiteralUncheckedComparator<LeftCppType, false, RightCppType, false>()) {
  }
};

// Shorthand for STL-compatible equal-comparator for a given non-nullable literal type.
// Includes a handy default constructor.
template <typename LeftCppType, typename RightCppType = LeftCppType>
class STLLiteralEqual : public STLUncheckedComparatorWrapper<
    EqualLiteralUncheckedComparator<LeftCppType, false, RightCppType, false>> {
 public:
  STLLiteralEqual()
      : STLUncheckedComparatorWrapper<EqualLiteralUncheckedComparator<LeftCppType, false,
                                                                      RightCppType, false>>(
          EqualLiteralUncheckedComparator<LeftCppType, false, RightCppType, false>()) {
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_LITERAL_COMPARATORS_HPP_
