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

// NOTE(chasseur): This file helps resolve some otherwise intractible circular
// dependencies with the the vectorized methods of the template classes in
// LiteralComparators.hpp. It should ONLY be included where a
// LiteralUncheckedComparator is actually created (e.g. the various
// comparison source files like LessComparison.cpp, and ComparisonUtil.hpp).

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_LITERAL_COMPARATORS_INL_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_LITERAL_COMPARATORS_INL_HPP_

#include <cstddef>
#include <type_traits>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/TupleIdSequence.hpp"

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/comparisons/LiteralComparators.hpp"

#include "glog/logging.h"

namespace quickstep {

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
TupleIdSequence* LiteralUncheckedComparator<ComparisonFunctor,
                                            LeftCppType, left_nullable,
                                            RightCppType, right_nullable>
    ::compareColumnVectors(
        const ColumnVector &left,
        const ColumnVector &right,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif
  // All literal types are usable with NativeColumnVector.
  DCHECK(left.isNative());
  DCHECK(right.isNative());

  const NativeColumnVector &left_native = static_cast<const NativeColumnVector&>(left);
  const NativeColumnVector &right_native = static_cast<const NativeColumnVector&>(right);

  DCHECK_EQ(left_native.size(), right_native.size());
  DCHECK((existence_bitmap == nullptr)
         || (existence_bitmap->numTuples() == left_native.size()));
  DCHECK((filter == nullptr)
         || ((existence_bitmap == nullptr) ? (filter->length() == left_native.size())
                                                 : (filter->length() == existence_bitmap->length())));
  TupleIdSequence *result = new TupleIdSequence(
      (existence_bitmap == nullptr) ? left_native.size()
                                    : existence_bitmap->length());
  if (short_circuit && (filter != nullptr)) {
    if (existence_bitmap != nullptr) {
      TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
      for (std::size_t cv_pos = 0;
           cv_pos < left_native.size();
           ++cv_pos) {
        DCHECK(existence_it != existence_bitmap->end());
        result->set(*existence_it,
                    filter->get(*existence_it)
                        && compareDataPtrsInl(left_native.getUntypedValue<left_nullable>(cv_pos),
                                              right_native.getUntypedValue<right_nullable>(cv_pos)));
        ++existence_it;
      }
      DCHECK(existence_it == existence_bitmap->end());
    } else {
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        result->set(*filter_it,
                    compareDataPtrsInl(left_native.getUntypedValue<left_nullable>(*filter_it),
                                       right_native.getUntypedValue<right_nullable>(*filter_it)));
      }
    }
  } else {
    if (existence_bitmap != nullptr) {
      TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
      for (std::size_t cv_pos = 0;
           cv_pos < left_native.size();
           ++cv_pos) {
        DCHECK(existence_it != existence_bitmap->end());
        result->set(*existence_it,
                    compareDataPtrsInl(left_native.getUntypedValue<left_nullable>(cv_pos),
                                       right_native.getUntypedValue<right_nullable>(cv_pos)));
        ++existence_it;
      }
      DCHECK(existence_it == existence_bitmap->end());
    } else {
      for (std::size_t pos = 0;
           pos < left_native.size();
           ++pos) {
        result->set(pos,
                    compareDataPtrsInl(left_native.getUntypedValue<left_nullable>(pos),
                                       right_native.getUntypedValue<right_nullable>(pos)));
      }
    }
    if (!short_circuit && (filter != nullptr)) {
      result->intersectWith(*filter);
    }
  }

  return result;
}

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
template <bool column_vector_on_left>
TupleIdSequence* LiteralUncheckedComparator<ComparisonFunctor,
                                            LeftCppType, left_nullable,
                                            RightCppType, right_nullable>
    ::compareColumnVectorAndStaticValueHelper(
        const ColumnVector &column_vector,
        const TypedValue &static_value,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
  static constexpr bool short_circuit = true;
#else
  static constexpr bool short_circuit = false;
#endif
  typedef typename std::conditional<column_vector_on_left,
                                    RightCppType,
                                    LeftCppType>::type
      StaticValueCppType;

  constexpr bool cv_nullable = column_vector_on_left ? left_nullable : right_nullable;
  constexpr bool static_value_nullable = column_vector_on_left ? right_nullable : left_nullable;

  DCHECK(column_vector.isNative());
  const NativeColumnVector &native_column_vector
      = static_cast<const NativeColumnVector&>(column_vector);

  DCHECK((existence_bitmap == nullptr)
               || (existence_bitmap->numTuples() == native_column_vector.size()));
  DCHECK((filter == nullptr)
               || ((existence_bitmap == nullptr) ? (filter->length() == native_column_vector.size())
                                                 : (filter->length() == existence_bitmap->length())));
  TupleIdSequence *result = new TupleIdSequence(
      (existence_bitmap == nullptr) ? native_column_vector.size()
                                    : existence_bitmap->length());
  if (static_value_nullable && static_value.isNull()) {
    return result;
  }
  const StaticValueCppType literal = static_value.getLiteral<StaticValueCppType>();

  if (short_circuit && (filter != nullptr)) {
    if (existence_bitmap != nullptr) {
      TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
      for (std::size_t cv_pos = 0;
           cv_pos < native_column_vector.size();
           ++cv_pos) {
        DCHECK(existence_it != existence_bitmap->end());
        if (filter->get(*existence_it)) {
          const void *cv_value
              = native_column_vector.getUntypedValue<cv_nullable>(cv_pos);
          result->set(*existence_it,
                      !(cv_nullable && (cv_value == nullptr))
                          && compareDataPtrsHelper<column_vector_on_left>(cv_value, &literal));
        }
        ++existence_it;
      }
      DCHECK(existence_it == existence_bitmap->end());
    } else {
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        const void *cv_value
            = native_column_vector.getUntypedValue<cv_nullable>(*filter_it);
        result->set(*filter_it,
                    !(cv_nullable && (cv_value == nullptr))
                        && compareDataPtrsHelper<column_vector_on_left>(cv_value, &literal));
      }
    }
  } else {
    if (existence_bitmap != nullptr) {
      TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
      for (std::size_t cv_pos = 0;
           cv_pos < native_column_vector.size();
           ++cv_pos) {
        DCHECK(existence_it != existence_bitmap->end());
        const void *cv_value
            = native_column_vector.getUntypedValue<cv_nullable>(cv_pos);
        result->set(*existence_it,
                    !(cv_nullable && (cv_value == nullptr))
                        && compareDataPtrsHelper<column_vector_on_left>(cv_value, &literal));
        ++existence_it;
      }
      DCHECK(existence_it == existence_bitmap->end());
    } else {
      for (std::size_t pos = 0;
           pos < native_column_vector.size();
           ++pos) {
        const void *cv_value
            = native_column_vector.getUntypedValue<cv_nullable>(pos);
        result->set(pos,
                    !(cv_nullable && (cv_value == nullptr))
                        && compareDataPtrsHelper<column_vector_on_left>(cv_value, &literal));
      }
    }
    if (!short_circuit && (filter != nullptr)) {
      result->intersectWith(*filter);
    }
  }

  return result;
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
TupleIdSequence* LiteralUncheckedComparator<ComparisonFunctor,
                                            LeftCppType, left_nullable,
                                            RightCppType, right_nullable>
    ::compareSingleValueAccessor(
        ValueAccessor *accessor,
        const attribute_id left_id,
        const attribute_id right_id,
        const TupleIdSequence *filter) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif

    TupleIdSequence *result = new TupleIdSequence(accessor->getEndPosition());
    if (short_circuit && (filter != nullptr)) {
      DCHECK_EQ(filter->length(), result->length());
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        const void *left_value
            = accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(left_id, *filter_it);
        const void *right_value
            = accessor->template getUntypedValueAtAbsolutePosition<right_nullable>(right_id, *filter_it);
        result->set(*filter_it,
                    (!((left_nullable && (left_value == nullptr))
                        || (right_nullable && (right_value == nullptr))))
                            && this->compareDataPtrsHelper<true>(left_value, right_value));
      }
    } else {
      accessor->beginIteration();
      if (accessor->isColumnAccessorSupported()) {
        // If ColumnAccessor is supported on the underlying accessor, we have a fast strided
        // column accessor available for the iteration on the underlying block.
        std::unique_ptr<const ColumnAccessor<left_nullable>>
            left_column_accessor(accessor->template getColumnAccessor<left_nullable>(left_id));
        std::unique_ptr<const ColumnAccessor<right_nullable>>
            right_column_accessor(accessor->template getColumnAccessor<right_nullable>(right_id));
        DCHECK(left_column_accessor != nullptr);
        DCHECK(right_column_accessor != nullptr);
        while (accessor->next()) {
          const void *left_value = left_column_accessor->getUntypedValue();
          const void *right_value = right_column_accessor->getUntypedValue();
          result->set(accessor->getCurrentPosition(),
                      (!((left_nullable && (left_value == nullptr))
                          || (right_nullable && (right_value == nullptr))))
                          && this->compareDataPtrsHelper<true>(left_value, right_value));
        }
      } else {
        while (accessor->next()) {
          const void *left_value = accessor->template getUntypedValue<left_nullable>(left_id);
          const void *right_value = accessor->template getUntypedValue<right_nullable>(right_id);
          result->set(accessor->getCurrentPosition(),
                      (!((left_nullable && (left_value == nullptr))
                          || (right_nullable && (right_value == nullptr))))
                          && this->compareDataPtrsHelper<true>(left_value, right_value));
        }
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
template <bool value_accessor_on_left>
TupleIdSequence* LiteralUncheckedComparator<ComparisonFunctor,
                                            LeftCppType, left_nullable,
                                            RightCppType, right_nullable>
    ::compareValueAccessorAndStaticValueHelper(
        ValueAccessor *accessor,
        const attribute_id value_accessor_attr_id,
        const TypedValue &static_value,
        const TupleIdSequence *filter) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    typedef typename std::conditional<value_accessor_on_left,
                                      RightCppType,
                                      LeftCppType>::type
        StaticValueCppType;

    constexpr bool va_nullable = value_accessor_on_left ? left_nullable : right_nullable;
    constexpr bool static_value_nullable = value_accessor_on_left ? right_nullable : left_nullable;

    TupleIdSequence *result = new TupleIdSequence(accessor->getEndPosition());
    if (static_value_nullable && static_value.isNull()) {
      return result;
    }
    const StaticValueCppType literal = static_value.getLiteral<StaticValueCppType>();

    if (short_circuit && (filter != nullptr)) {
      DCHECK_EQ(filter->length(), result->length());
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        const void *va_value
            = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(
                value_accessor_attr_id,
                *filter_it);
        result->set(*filter_it,
                    !(va_nullable && (va_value == nullptr))
                        && this->compareDataPtrsHelper<value_accessor_on_left>(va_value, &literal));
      }
    } else {
      accessor->beginIteration();
      if (accessor->isColumnAccessorSupported()) {
        // If ColumnAccessor is supported on the underlying accessor, we have a fast strided
        // column accessor available for the iteration on the underlying block.
        std::unique_ptr<const ColumnAccessor<va_nullable>>
            column_accessor(accessor->template getColumnAccessor<va_nullable>(value_accessor_attr_id));
        DCHECK(column_accessor != nullptr);
        while (accessor->next()) {
          const void *va_value = column_accessor->getUntypedValue();
          result->set(accessor->getCurrentPosition(),
                      !(va_nullable && (va_value == nullptr))
                          && this->compareDataPtrsHelper<value_accessor_on_left>(va_value, &literal));
        }
      } else {
        while (accessor->next()) {
          const void *va_value
              = accessor->template getUntypedValue<va_nullable>(value_accessor_attr_id);
          result->set(accessor->getCurrentPosition(),
                      !(va_nullable && (va_value == nullptr))
                          && this->compareDataPtrsHelper<value_accessor_on_left>(va_value,
                                                                                 &literal));
        }
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
template <bool column_vector_on_left>
TupleIdSequence* LiteralUncheckedComparator<ComparisonFunctor,
                                            LeftCppType, left_nullable,
                                            RightCppType, right_nullable>
    ::compareColumnVectorAndValueAccessorHelper(
        const ColumnVector &column_vector,
        ValueAccessor *accessor,
        const attribute_id value_accessor_attr_id,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  DCHECK(column_vector.isNative());
  const NativeColumnVector &native_column_vector
      = static_cast<const NativeColumnVector&>(column_vector);

  DCHECK((existence_bitmap == nullptr)
               || (existence_bitmap->numTuples() == native_column_vector.size()));
  DCHECK((filter == nullptr)
               || ((existence_bitmap == nullptr) ? (filter->length() == native_column_vector.size())
                                                 : (filter->length() == existence_bitmap->length())));
  TupleIdSequence *result = new TupleIdSequence(
      (existence_bitmap == nullptr) ? native_column_vector.size()
                                    : existence_bitmap->length());

  InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    constexpr bool cv_nullable = column_vector_on_left ? left_nullable : right_nullable;
    constexpr bool va_nullable = column_vector_on_left ? right_nullable : left_nullable;

    DCHECK_EQ(native_column_vector.size(),
              static_cast<size_t>(accessor->getNumTuples()));
    if (short_circuit && (filter != nullptr)) {
      DCHECK_EQ(filter->length(), result->length());
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < native_column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          if (filter->get(*existence_it)) {
            const void *cv_value
                = native_column_vector.getUntypedValue<cv_nullable>(cv_pos);
            const void *va_value
                = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(
                    value_accessor_attr_id,
                    *existence_it);
            result->set(*existence_it,
                        (!((cv_nullable && (cv_value == nullptr))
                            || (va_nullable && (va_value == nullptr))))
                                && this->compareDataPtrsHelper<column_vector_on_left>(cv_value, va_value));
          }
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *cv_value
              = native_column_vector.getUntypedValue<cv_nullable>(*filter_it);
          const void *va_value
              = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(
                  value_accessor_attr_id,
                  *filter_it);
          result->set(*filter_it,
                      (!((cv_nullable && (cv_value == nullptr))
                          || (va_nullable && (va_value == nullptr))))
                              && this->compareDataPtrsHelper<column_vector_on_left>(cv_value, va_value));
        }
      }
    } else {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < native_column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());

          const void *cv_value
              = native_column_vector.getUntypedValue<cv_nullable>(cv_pos);
          const void *va_value
              = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(
                  value_accessor_attr_id,
                  *existence_it);
          result->set(*existence_it,
                      (!((cv_nullable && (cv_value == nullptr))
                          || (va_nullable && (va_value == nullptr))))
                              && this->compareDataPtrsHelper<column_vector_on_left>(cv_value, va_value));

          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        accessor->beginIteration();
        std::size_t cv_pos = 0;
        if (accessor->isColumnAccessorSupported()) {
          // If ColumnAccessor is supported on the underlying accessor, we have a fast strided
          // column accessor available for the iteration on the underlying block.
          std::unique_ptr<const ColumnAccessor<va_nullable>>
              column_accessor(accessor->template getColumnAccessor<va_nullable>(value_accessor_attr_id));
          DCHECK(column_accessor != nullptr);
          while (accessor->next()) {
            const void *cv_value
                = native_column_vector.getUntypedValue<cv_nullable>(cv_pos);
            const void *va_value = column_accessor->getUntypedValue();
            result->set(cv_pos,
                        (!((cv_nullable && (cv_value == nullptr))
                            || (va_nullable && (va_value == nullptr))))
                            && this->compareDataPtrsHelper<column_vector_on_left>(cv_value,
                                                                                  va_value));
            ++cv_pos;
          }
        } else {
          while (accessor->next()) {
            const void *cv_value
                = native_column_vector.getUntypedValue<cv_nullable>(cv_pos);
            const void *va_value
                = accessor->template getUntypedValue<va_nullable>(value_accessor_attr_id);
            result->set(cv_pos,
                        (!((cv_nullable && (cv_value == nullptr))
                            || (va_nullable && (va_value == nullptr))))
                            && this->compareDataPtrsHelper<column_vector_on_left>(cv_value,
                                                                                  va_value));
            ++cv_pos;
          }
        }
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }
  });

  return result;
}

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
TypedValue LiteralUncheckedComparator<ComparisonFunctor,
                                      LeftCppType, left_nullable,
                                      RightCppType, right_nullable>
    ::accumulateValueAccessor(
        const TypedValue &current,
        ValueAccessor *accessor,
        const attribute_id value_accessor_id) const {
  const void *current_literal = current.isNull() ? nullptr : current.getDataPtr();

  InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    accessor->beginIteration();
    if (accessor->isColumnAccessorSupported()) {
      // If ColumnAccessor is supported on the underlying accessor, we have a fast strided
      // column accessor available for the iteration on the underlying block.
      std::unique_ptr<const ColumnAccessor<left_nullable>>
          column_accessor(accessor->template getColumnAccessor<left_nullable>(value_accessor_id));
      DCHECK(column_accessor != nullptr);
      while (accessor->next()) {
        const void *va_value = column_accessor->getUntypedValue();
        if (left_nullable && !va_value) {
          continue;
        }
        if (!current_literal || this->compareDataPtrsHelper<true>(va_value, current_literal)) {
          current_literal = va_value;
        }
      }
    } else {
      while (accessor->next()) {
        const void *va_value = accessor->template getUntypedValue<left_nullable>(value_accessor_id);
        if (left_nullable && !va_value) {
          continue;
        }
        if (!current_literal || this->compareDataPtrsHelper<true>(va_value, current_literal)) {
          current_literal = va_value;
        }
      }
    }
  });

  if (current_literal) {
    return TypedValue(*static_cast<const LeftCppType*>(current_literal));
  } else {
    return TypedValue(current.getTypeID());
  }
}

#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

template <template <typename LeftArgument, typename RightArgument> class ComparisonFunctor,
          typename LeftCppType, bool left_nullable,
          typename RightCppType, bool right_nullable>
TypedValue LiteralUncheckedComparator<ComparisonFunctor,
                                      LeftCppType, left_nullable,
                                      RightCppType, right_nullable>
    ::accumulateColumnVector(
        const TypedValue &current,
        const ColumnVector &column_vector) const {
  const void *current_literal = current.isNull() ? nullptr : current.getDataPtr();

  // All literal types are usable with NativeColumnVector.
  DCHECK(column_vector.isNative());

  const NativeColumnVector &native_vector = static_cast<const NativeColumnVector&>(column_vector);
  for (std::size_t pos = 0; pos < native_vector.size(); ++pos) {
    const void *cv_value = native_vector.getUntypedValue<left_nullable>(pos);
    if (left_nullable && !cv_value) {
      continue;
    }
    if (!current_literal || compareDataPtrsHelper<true>(cv_value, current_literal)) {
      current_literal = cv_value;
    }
  }

  if (current_literal) {
    return TypedValue(*static_cast<const LeftCppType*>(current_literal));
  } else {
    return TypedValue(current.getTypeID());
  }
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_LITERAL_COMPARATORS_INL_HPP_
