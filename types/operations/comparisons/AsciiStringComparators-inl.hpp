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
// AsciiStringComparators.hpp. It should ONLY be included where an
// AsciiStringUncheckedComparator is actually created (e.g. the various
// comparison source files like LessComparison.cpp, and ComparisonUtil.hpp).

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_ASCII_STRING_COMPARATORS_INL_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_ASCII_STRING_COMPARATORS_INL_HPP_

#include <cstddef>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/TupleIdSequence.hpp"

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorUtil.hpp"
#include "types/operations/comparisons/AsciiStringComparators.hpp"

#include "glog/logging.h"

namespace quickstep {

template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
TupleIdSequence* AsciiStringUncheckedComparator<ComparisonFunctor,
                                                left_nullable, left_null_terminated, left_longer,
                                                right_nullable, right_null_terminated, right_longer>
    ::compareColumnVectors(
        const ColumnVector &left,
        const ColumnVector &right,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  // CHAR will always use NativeColumnVector, but VARCHAR requires
  // IndirectColumnVector.
  return InvokeOnColumnVector(
      left,
      [&](const auto &left_column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
    return InvokeOnColumnVector(
        right,
        [&](const auto &right_column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
      static constexpr bool short_circuit = true;
#else
      static constexpr bool short_circuit = false;
#endif
      DCHECK_EQ(left_column_vector.size(), right_column_vector.size());
      DCHECK((existence_bitmap == nullptr)
             || (existence_bitmap->numTuples() == left_column_vector.size()));
      DCHECK((filter == nullptr)
             || ((existence_bitmap == nullptr) ? (filter->length() == left_column_vector.size())
                                               : (filter->length() == existence_bitmap->length())));
      TupleIdSequence *result = new TupleIdSequence(
          (existence_bitmap == nullptr) ? left_column_vector.size()
                                        : existence_bitmap->length());
      if (short_circuit && (filter != nullptr)) {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < left_column_vector.size();
               ++cv_pos) {
            DCHECK(existence_it != existence_bitmap->end());
            result->set(*existence_it,
                        filter->get(*existence_it)
                            && this->compareDataPtrsInl(
                                left_column_vector.template getUntypedValue<left_nullable>(cv_pos),
                                right_column_vector.template getUntypedValue<right_nullable>(cv_pos)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            result->set(*filter_it,
                        this->compareDataPtrsInl(
                            left_column_vector.template getUntypedValue<left_nullable>(*filter_it),
                            right_column_vector.template getUntypedValue<right_nullable>(*filter_it)));
          }
        }
      } else {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < left_column_vector.size();
               ++cv_pos) {
            DCHECK(existence_it != existence_bitmap->end());
            result->set(*existence_it,
                        this->compareDataPtrsInl(
                            left_column_vector.template getUntypedValue<left_nullable>(cv_pos),
                            right_column_vector.template getUntypedValue<right_nullable>(cv_pos)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          for (std::size_t pos = 0;
               pos < left_column_vector.size();
               ++pos) {
            result->set(pos,
                        this->compareDataPtrsInl(
                            left_column_vector.template getUntypedValue<left_nullable>(pos),
                            right_column_vector.template getUntypedValue<right_nullable>(pos)));
          }
        }
        if (!short_circuit && (filter != nullptr)) {
          result->intersectWith(*filter);
        }
      }

      return result;
    });
  });
}

template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
template <bool column_vector_on_left>
TupleIdSequence* AsciiStringUncheckedComparator<ComparisonFunctor,
                                                left_nullable, left_null_terminated, left_longer,
                                                right_nullable, right_null_terminated, right_longer>
    ::compareColumnVectorAndStaticValueHelper(
        const ColumnVector &column_vector,
        const TypedValue &static_value,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  return InvokeOnColumnVector(
      column_vector,
      [&](const auto &column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    constexpr bool cv_nullable = column_vector_on_left ? left_nullable : right_nullable;
    constexpr bool static_value_nullable = column_vector_on_left ? right_nullable : left_nullable;
    DCHECK((existence_bitmap == nullptr)
            || (existence_bitmap->numTuples() == column_vector.size()));
    DCHECK((filter == nullptr)
            || ((existence_bitmap == nullptr) ? (filter->length() == column_vector.size())
                                              : (filter->length() == existence_bitmap->length())));
    TupleIdSequence *result = new TupleIdSequence(
        (existence_bitmap == nullptr) ? column_vector.size()
                                      : existence_bitmap->length());
    if (static_value_nullable && static_value.isNull()) {
      return result;
    }
    const void *static_string = static_value.getDataPtr();

    if (short_circuit && (filter != nullptr)) {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          if (filter->get(*existence_it)) {
            const void *cv_value
                = column_vector.template getUntypedValue<cv_nullable>(cv_pos);
            result->set(*existence_it,
                        !(cv_nullable && (cv_value == nullptr))
                            && this->compareDataPtrsHelper<column_vector_on_left>(cv_value, static_string));
          }
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *cv_value
              = column_vector.template getUntypedValue<cv_nullable>(*filter_it);
          result->set(*filter_it,
                      !(cv_nullable && (cv_value == nullptr))
                          && this->compareDataPtrsHelper<column_vector_on_left>(cv_value, static_string));
        }
      }
    } else {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          const void *cv_value
              = column_vector.template getUntypedValue<cv_nullable>(cv_pos);
          result->set(*existence_it,
                      !(cv_nullable && (cv_value == nullptr))
                          && this->compareDataPtrsHelper<column_vector_on_left>(cv_value, static_string));
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (std::size_t pos = 0;
             pos < column_vector.size();
             ++pos) {
          const void *cv_value
              = column_vector.template getUntypedValue<cv_nullable>(pos);
          result->set(pos,
                      !(cv_nullable && (cv_value == nullptr))
                          && this->compareDataPtrsHelper<column_vector_on_left>(cv_value, static_string));
        }
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
TupleIdSequence* AsciiStringUncheckedComparator<ComparisonFunctor,
                                                left_nullable, left_null_terminated, left_longer,
                                                right_nullable, right_null_terminated, right_longer>
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
                      (!(left_nullable && (left_value == nullptr))
                          || (right_nullable && (right_value == nullptr)))
                          && this->compareDataPtrsHelper<true>(left_value, right_value));
        }
      } else {
        while (accessor->next()) {
          const void *left_value = accessor->template getUntypedValue<left_nullable>(left_id);
          const void *right_value = accessor->template getUntypedValue<right_nullable>(right_id);
          result->set(accessor->getCurrentPosition(),
                      (!(left_nullable && (left_value == nullptr))
                          || (right_nullable && (right_value == nullptr)))
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

template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
template <bool value_accessor_on_left>
TupleIdSequence* AsciiStringUncheckedComparator<ComparisonFunctor,
                                                left_nullable, left_null_terminated, left_longer,
                                                right_nullable, right_null_terminated, right_longer>
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
    constexpr bool va_nullable = value_accessor_on_left ? left_nullable : right_nullable;
    constexpr bool static_value_nullable = value_accessor_on_left ? right_nullable : left_nullable;

    TupleIdSequence *result = new TupleIdSequence(accessor->getEndPosition());
    if (static_value_nullable && static_value.isNull()) {
      return result;
    }
    const void *static_string = static_value.getDataPtr();

    if (short_circuit && (filter != nullptr)) {
      DCHECK_EQ(filter->length(), result->length());
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        const void *va_value
            = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(value_accessor_attr_id,
                                                                                *filter_it);
        result->set(*filter_it,
                    !(va_nullable && (va_value == nullptr))
                        && this->compareDataPtrsHelper<value_accessor_on_left>(va_value, static_string));
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
                          && this->compareDataPtrsHelper<value_accessor_on_left>(va_value,
                                                                                 static_string));
        }
      } else {
        while (accessor->next()) {
          const void *va_value
              = accessor->template getUntypedValue<va_nullable>(value_accessor_attr_id);
          result->set(accessor->getCurrentPosition(),
                      !(va_nullable && (va_value == nullptr))
                          && this->compareDataPtrsHelper<value_accessor_on_left>(va_value,
                                                                                 static_string));
        }
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
template <bool column_vector_on_left>
TupleIdSequence* AsciiStringUncheckedComparator<ComparisonFunctor,
                                                left_nullable, left_null_terminated, left_longer,
                                                right_nullable, right_null_terminated, right_longer>
    ::compareColumnVectorAndValueAccessorHelper(
        const ColumnVector &column_vector,
        ValueAccessor *accessor,
        const attribute_id value_accessor_attr_id,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  return InvokeOnColumnVector(
      column_vector,
      [&](const auto &column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
    DCHECK((filter == nullptr)
           || ((existence_bitmap == nullptr) ? (filter->length() == column_vector.size())
                                             : (filter->length() == existence_bitmap->length())));

    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor,
        [&](auto *accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
      static constexpr bool short_circuit = true;
#else
      static constexpr bool short_circuit = false;
#endif
      constexpr bool cv_nullable = column_vector_on_left ? left_nullable : right_nullable;
      constexpr bool va_nullable = column_vector_on_left ? right_nullable : left_nullable;

      DCHECK_EQ(column_vector.size(),
                static_cast<std::size_t>(accessor->getNumTuples()));
      DCHECK((existence_bitmap == nullptr)
             || (existence_bitmap->numTuples() == column_vector.size()));
      TupleIdSequence *result = new TupleIdSequence(
          (existence_bitmap == nullptr) ? column_vector.size()
                                        : existence_bitmap->length());
      if (short_circuit && (filter != nullptr)) {
        DCHECK_EQ(filter->length(), result->length());
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < column_vector.size();
               ++cv_pos) {
            DCHECK(existence_it != existence_bitmap->end());
            if (filter->get(*existence_it)) {
              const void *cv_value
                  = column_vector.template getUntypedValue<cv_nullable>(cv_pos);
              const void *va_value
                  = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(value_accessor_attr_id,
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
                = column_vector.template getUntypedValue<cv_nullable>(*filter_it);
            const void *va_value
                = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(value_accessor_attr_id,
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
               cv_pos < column_vector.size();
               ++cv_pos) {
            DCHECK(existence_it != existence_bitmap->end());

            const void *cv_value
                = column_vector.template getUntypedValue<cv_nullable>(cv_pos);
            const void *va_value
                = accessor->template getUntypedValueAtAbsolutePosition<va_nullable>(value_accessor_attr_id,
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
                  = column_vector.template getUntypedValue<cv_nullable>(cv_pos);
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
                  = column_vector.template getUntypedValue<cv_nullable>(cv_pos);
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
      return result;
    });
  });
}

template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
TypedValue AsciiStringUncheckedComparator<ComparisonFunctor,
                                          left_nullable, left_null_terminated, left_longer,
                                          right_nullable, right_null_terminated, right_longer>
    ::accumulateValueAccessor(
        const TypedValue &current,
        ValueAccessor *accessor,
        const attribute_id value_accessor_id) const {
  TypedValue new_value = current;

  InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    accessor->beginIteration();
    while (accessor->next()) {
      TypedValue va_value = accessor->getTypedValue(value_accessor_id);
      if (left_nullable && va_value.isNull()) {
        continue;
      }
      if (new_value.isNull() || this->compareTypedValuesInl(va_value, new_value)) {
        new_value = va_value;
      }
    }
  });

  return new_value;
}

#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

template <template <typename T> class ComparisonFunctor,
          bool left_nullable, bool left_null_terminated, bool left_longer,
          bool right_nullable, bool right_null_terminated, bool right_longer>
TypedValue AsciiStringUncheckedComparator<ComparisonFunctor,
                                          left_nullable, left_null_terminated, left_longer,
                                          right_nullable, right_null_terminated, right_longer>
    ::accumulateColumnVector(
        const TypedValue &current,
        const ColumnVector &column_vector) const {
  TypedValue new_value = current;

  InvokeOnColumnVector(
      column_vector,
      [&](const auto &column_vector) -> void {  // NOLINT(build/c++11)
    for (std::size_t pos = 0;
         pos < column_vector.size();
         ++pos) {
      TypedValue cv_value = column_vector.getTypedValue(pos);
      if (left_nullable && cv_value.isNull()) {
        continue;
      }
      if (new_value.isNull() || this->compareTypedValuesInl(cv_value, new_value)) {
        new_value = cv_value;
      }
    }
  });

  return new_value;
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_ASCII_STRING_COMPARATORS_INL_HPP_
