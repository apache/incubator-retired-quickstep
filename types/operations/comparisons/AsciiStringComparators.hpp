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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_ASCII_STRING_COMPARATORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_ASCII_STRING_COMPARATORS_HPP_

#include <cstddef>
#include <cstring>
#include <functional>

#include "catalog/CatalogTypedefs.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/port/strnlen.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class TupleIdSequence;
class ValueAccessor;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Base class for UncheckedComparators which compare strings.
 **/
template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
class AsciiStringUncheckedComparator : public UncheckedComparator {
 public:
  /**
   * @brief Constructor.
   *
   * @param left_length The maximum length of the left string to compare.
   * @param right_length The maximum length of the right string to compare.
   **/
  AsciiStringUncheckedComparator(const std::size_t left_length,
                                 const std::size_t right_length)
      : left_length_(left_length),
        right_length_(right_length) {
  }

  AsciiStringUncheckedComparator(const AsciiStringUncheckedComparator &orig) = default;
  ~AsciiStringUncheckedComparator() override = default;

  inline bool compareTypedValues(const TypedValue &left,
                                 const TypedValue &right) const override {
    return compareTypedValuesInl(left, right);
  }

  inline bool compareTypedValuesInl(const TypedValue &left, const TypedValue &right) const {
    if ((left_nullable && left.isNull()) || (right_nullable && right.isNull())) {
      return false;
    }
    return comparison_functor_(strcmpHelper(static_cast<const char*>(left.getOutOfLineData()),
                                            static_cast<const char*>(right.getOutOfLineData())),
                               0);
  }

  inline bool compareDataPtrs(const void *left,
                              const void *right) const override {
    return compareDataPtrsInl(left, right);
  }

  inline bool compareDataPtrsInl(const void *left, const void *right) const {
    if ((left_nullable && (left == nullptr)) || (right_nullable && (right == nullptr))) {
      return false;
    }
    return comparison_functor_(strcmpHelper(static_cast<const char*>(left),
                                            static_cast<const char*>(right)),
                               0);
  }

  inline bool compareTypedValueWithDataPtr(const TypedValue &left,
                                           const void *right) const override {
    return compareTypedValueWithDataPtrInl(left, right);
  }

  inline bool compareTypedValueWithDataPtrInl(const TypedValue &left,
                                              const void *right) const {
    if ((left_nullable && left.isNull()) || (right_nullable && (right == nullptr))) {
      return false;
    }
    return comparison_functor_(strcmpHelper(static_cast<const char*>(left.getOutOfLineData()),
                                            static_cast<const char*>(right)),
                               0);
  }

  inline bool compareDataPtrWithTypedValue(const void *left,
                                           const TypedValue &right) const override {
    return compareDataPtrWithTypedValueInl(left, right);
  }

  inline bool compareDataPtrWithTypedValueInl(const void *left, const TypedValue &right) const {
    if ((left_nullable && (left == nullptr)) || (right_nullable && right.isNull())) {
      return false;
    }
    return comparison_functor_(strcmpHelper(static_cast<const char*>(left),
                                            static_cast<const char*>(right.getOutOfLineData())),
                               0);
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
  /**
   * @brief Compare left and right strings like strcmp, but safely handle cases
   *        where one of the strings might not be NULL-terminated.
   *
   * @param left The left string to compare.
   * @param right The right string to compare.
   **/
  // TODO(chasseur): May be able to make this faster for VARCHAR TypedValues by
  // using a priori size information.
  inline int strcmpHelper(const char *left, const char *right) const {
    DCHECK(left != nullptr);
    DCHECK(right != nullptr);

    if (left_null_terminated && right_null_terminated) {
      return std::strcmp(left, right);
    } else {
      if (right_longer) {
        int res = std::strncmp(left, right, left_length_);
        if (res) {
          return res;
        } else {
          if (strnlen(right, right_length_) > left_length_) {
            return -1;
          } else {
            return res;
          }
        }
      } else if (left_longer) {
        int res = std::strncmp(left, right, right_length_);
        if (res) {
          return res;
        } else {
          if (strnlen(left, left_length_) > right_length_) {
            return 1;
          } else {
            return res;
          }
        }
      } else {
        return std::strncmp(left, right, left_length_);
      }
    }
  }

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
    return comparison_functor_(strcmpHelper(static_cast<const char*>(arguments_in_order ? left : right),
                                            static_cast<const char*>(arguments_in_order ? right : left)),
                               0);
  }

  std::size_t left_length_, right_length_;
  ComparisonFunctor<int> comparison_functor_;
};

/**
 * @brief The equals UncheckedComparator for strings.
 **/
template <bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
using EqualAsciiStringUncheckedComparator
    = AsciiStringUncheckedComparator<std::equal_to,
                                     left_nullable, left_null_terminated, left_longer,
                                     right_nullable, right_null_terminated, right_longer>;

/**
 * @brief The not-equal UncheckedComparator for strings.
 **/
template <bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
using NotEqualAsciiStringUncheckedComparator
    = AsciiStringUncheckedComparator<std::not_equal_to,
                                     left_nullable, left_null_terminated, left_longer,
                                     right_nullable, right_null_terminated, right_longer>;

/**
 * @brief The less-than UncheckedComparator for strings.
 **/
template <bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
using LessAsciiStringUncheckedComparator
    = AsciiStringUncheckedComparator<std::less,
                                     left_nullable, left_null_terminated, left_longer,
                                     right_nullable, right_null_terminated, right_longer>;

/**
 * @brief The less-than-or-equal UncheckedComparator for strings.
 **/
template <bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
using LessOrEqualAsciiStringUncheckedComparator
    = AsciiStringUncheckedComparator<std::less_equal,
                                     left_nullable, left_null_terminated, left_longer,
                                     right_nullable, right_null_terminated, right_longer>;

/**
 * @brief The greater-than UncheckedComparator for strings.
 **/
template <bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
using GreaterAsciiStringUncheckedComparator
    = AsciiStringUncheckedComparator<std::greater,
                                     left_nullable, left_null_terminated, left_longer,
                                     right_nullable, right_null_terminated, right_longer>;

/**
 * @brief The greater-than-or-equal UncheckedComparator for strings.
 **/
template <bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
using GreaterOrEqualAsciiStringUncheckedComparator
    = AsciiStringUncheckedComparator<std::greater_equal,
                                     left_nullable, left_null_terminated, left_longer,
                                     right_nullable, right_null_terminated, right_longer>;

// Shorthand for STL-compatible less-comparator for non-nullable char type.
class STLCharLess : public STLUncheckedComparatorWrapper<
    LessAsciiStringUncheckedComparator<false, false, false, false, false, false>> {
 public:
  explicit STLCharLess(const std::size_t max_length)
      : STLUncheckedComparatorWrapper<
          LessAsciiStringUncheckedComparator<false, false, false, false, false, false>>(
              LessAsciiStringUncheckedComparator<false, false, false, false, false, false>(max_length, max_length)) {
  }
};

// Shorthand for STL-compatible equal-comparator for non-nullable char type.
class STLCharEqual : public STLUncheckedComparatorWrapper<
    EqualAsciiStringUncheckedComparator<false, false, false, false, false, false>> {
 public:
  explicit STLCharEqual(const std::size_t max_length)
      : STLUncheckedComparatorWrapper<
          EqualAsciiStringUncheckedComparator<false, false, false, false, false, false>>(
              EqualAsciiStringUncheckedComparator<false, false, false, false, false, false>(max_length, max_length)) {
  }
};

// Shorthand for STL-compatible less-comparator for non-nullable varchar type.
class STLVarCharLess : public STLUncheckedComparatorWrapper<
    LessAsciiStringUncheckedComparator<false, true, false, false, true, false>> {
 public:
  STLVarCharLess()
      : STLUncheckedComparatorWrapper<
          LessAsciiStringUncheckedComparator<false, true, false, false, true, false>>(
              // NOTE: Safe to use zero for length params, since the lengths
              // are not used when both sides are null-terminated.
              LessAsciiStringUncheckedComparator<false, true, false, false, true, false>(0, 0)) {
  }
};

// Shorthand for STL-compatible equal-comparator for non-nullable varchar type.
class STLVarCharEqual : public STLUncheckedComparatorWrapper<
    EqualAsciiStringUncheckedComparator<false, true, false, false, true, false>> {
 public:
  STLVarCharEqual()
      : STLUncheckedComparatorWrapper<
          EqualAsciiStringUncheckedComparator<false, true, false, false, true, false>>(
              // NOTE: Safe to use zero for length params, since the lengths
              // are not used when both sides are null-terminated.
              EqualAsciiStringUncheckedComparator<false, true, false, false, true, false>(0, 0)) {
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_ASCII_STRING_COMPARATORS_HPP_
