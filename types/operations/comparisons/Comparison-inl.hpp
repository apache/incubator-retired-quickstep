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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARATORS_INL_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARATORS_INL_HPP_

#include "types/operations/comparisons/Comparison.hpp"

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

#include "glog/logging.h"

namespace quickstep {

template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareColumnVectorsDefaultImpl(
    const ColumnVector &left,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
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
            result->set(*existence_it,
                        filter->get(*existence_it)
                            && this->compareDataPtrs(
                                left_column_vector.template getUntypedValue<left_nullable>(cv_pos),
                                right_column_vector.template getUntypedValue<right_nullable>(cv_pos)));
            ++existence_it;
          }
        } else {
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            result->set(*filter_it,
                        this->compareDataPtrs(
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
            result->set(*existence_it,
                        this->compareDataPtrs(
                            left_column_vector.template getUntypedValue<left_nullable>(cv_pos),
                            right_column_vector.template getUntypedValue<right_nullable>(cv_pos)));
            ++existence_it;
          }
        } else {
          for (std::size_t pos = 0;
               pos < left_column_vector.size();
               ++pos) {
            result->set(pos,
                        this->compareDataPtrs(
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

template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareColumnVectorAndStaticValueDefaultImpl(
    const ColumnVector &left,
    const TypedValue &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  return InvokeOnColumnVector(
      left,
      [&](const auto &column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    DCHECK((existence_bitmap == nullptr)
           || (existence_bitmap->numTuples() == column_vector.size()));
    DCHECK((filter == nullptr)
           || ((existence_bitmap == nullptr) ? (filter->length() == column_vector.size())
                                             : (filter->length() == existence_bitmap->length())));
    TupleIdSequence *result = new TupleIdSequence(
        (existence_bitmap == nullptr) ? column_vector.size()
                                      : existence_bitmap->length());
    if (right_nullable && right.isNull()) {
      return result;
    }

    if (short_circuit && (filter != nullptr)) {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          result->set(*existence_it,
                      filter->get(*existence_it)
                          && this->compareDataPtrWithTypedValue(
                              column_vector.template getUntypedValue<left_nullable>(cv_pos),
                              right));
          ++existence_it;
        }
      } else {
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          result->set(*filter_it,
                      this->compareDataPtrWithTypedValue(
                          column_vector.template getUntypedValue<left_nullable>(*filter_it),
                          right));
        }
      }
    } else {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          result->set(*existence_it,
                      this->compareDataPtrWithTypedValue(
                          column_vector.template getUntypedValue<left_nullable>(cv_pos),
                          right));
          ++existence_it;
        }
      } else {
        for (std::size_t pos = 0;
             pos < column_vector.size();
             ++pos) {
          result->set(pos,
                      this->compareDataPtrWithTypedValue(
                          column_vector.template getUntypedValue<left_nullable>(pos),
                          right));
        }
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareStaticValueAndColumnVectorDefaultImpl(
    const TypedValue &left,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  return InvokeOnColumnVector(
      right,
      [&](const auto &column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    DCHECK((existence_bitmap == nullptr)
           || (existence_bitmap->numTuples() == column_vector.size()));
    DCHECK((filter == nullptr)
           || ((existence_bitmap == nullptr) ? (filter->length() == column_vector.size())
                                             : (filter->length() == existence_bitmap->length())));
    TupleIdSequence *result = new TupleIdSequence(
        (existence_bitmap == nullptr) ? column_vector.size()
                                      : existence_bitmap->length());
    if (left_nullable && left.isNull()) {
      return result;
    }

    if (short_circuit && (filter != nullptr)) {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          result->set(*existence_it,
                      filter->get(*existence_it)
                          && this->compareTypedValueWithDataPtr(
                              left,
                              column_vector.template getUntypedValue<right_nullable>(cv_pos)));
          ++existence_it;
        }
      } else {
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          result->set(*filter_it,
                      this->compareTypedValueWithDataPtr(
                          left,
                          column_vector.template getUntypedValue<right_nullable>(*filter_it)));
        }
      }
    } else {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          result->set(*existence_it,
                      this->compareTypedValueWithDataPtr(
                          left,
                          column_vector.template getUntypedValue<right_nullable>(cv_pos)));
          ++existence_it;
        }
      } else {
        for (std::size_t pos = 0;
             pos < column_vector.size();
             ++pos) {
          result->set(pos,
                      this->compareTypedValueWithDataPtr(
                          left,
                          column_vector.template getUntypedValue<right_nullable>(pos)));
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
template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareSingleValueAccessorDefaultImpl(
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
        result->set(*filter_it,
                    this->compareDataPtrs(
                        accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(left_id, *filter_it),
                        accessor->template getUntypedValueAtAbsolutePosition<right_nullable>(right_id, *filter_it)));
      }
    } else {
      accessor->beginIteration();
      while (accessor->next()) {
        result->set(accessor->getCurrentPosition(),
                    this->compareDataPtrs(
                        accessor->template getUntypedValue<left_nullable>(left_id),
                        accessor->template getUntypedValue<right_nullable>(right_id)));
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareValueAccessorAndStaticValueDefaultImpl(
    ValueAccessor *left_accessor,
    const attribute_id left_id,
    const TypedValue &right,
    const TupleIdSequence *filter) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      left_accessor,
      [&](auto *left_accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    TupleIdSequence *result = new TupleIdSequence(left_accessor->getEndPosition());
    if (right_nullable && right.isNull()) {
      return result;
    }
    if (short_circuit && (filter != nullptr)) {
      DCHECK_EQ(filter->length(), result->length());
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        result->set(*filter_it,
                    this->compareDataPtrWithTypedValue(
                        left_accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(
                            left_id, *filter_it),
                        right));
      }
    } else {
      left_accessor->beginIteration();
      while (left_accessor->next()) {
        result->set(left_accessor->getCurrentPosition(),
                    this->compareDataPtrWithTypedValue(
                        left_accessor->template getUntypedValue<left_nullable>(left_id),
                        right));
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareStaticValueAndValueAccessorDefaultImpl(
    const TypedValue &left,
    ValueAccessor *right_accessor,
    const attribute_id right_id,
    const TupleIdSequence *filter) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      right_accessor,
      [&](auto *right_accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    TupleIdSequence *result = new TupleIdSequence(right_accessor->getEndPosition());
    if (left_nullable && left.isNull()) {
      return result;
    }
    if (short_circuit && (filter != nullptr)) {
      DCHECK_EQ(filter->length(), result->length());
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        result->set(*filter_it,
                    this->compareTypedValueWithDataPtr(
                        left,
                        right_accessor->template getUntypedValueAtAbsolutePosition<right_nullable>(
                            right_id, *filter_it)));
      }
    } else {
      right_accessor->beginIteration();
      while (right_accessor->next()) {
        result->set(right_accessor->getCurrentPosition(),
                    this->compareTypedValueWithDataPtr(
                        left,
                        right_accessor->template getUntypedValue<right_nullable>(right_id)));
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareColumnVectorAndValueAccessorDefaultImpl(
    const ColumnVector &left,
    ValueAccessor *right_accessor,
    const attribute_id right_id,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  return InvokeOnColumnVector(
      left,
      [&](const auto &column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
    return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        right_accessor,
        [&](auto *right_accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
      static constexpr bool short_circuit = true;
#else
      static constexpr bool short_circuit = false;
#endif
      DCHECK_EQ(column_vector.size(),
                static_cast<std::size_t>(right_accessor->getNumTuples()));
      DCHECK((existence_bitmap == nullptr)
             || (existence_bitmap->numTuples() == column_vector.size()));
      DCHECK((filter == nullptr)
             || ((existence_bitmap == nullptr) ? (filter->length() == column_vector.size())
                                               : (filter->length() == existence_bitmap->length())));
      TupleIdSequence *result = new TupleIdSequence(
          (existence_bitmap == nullptr) ? column_vector.size()
                                        : existence_bitmap->length());
      if (short_circuit && (filter != nullptr)) {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < column_vector.size();
               ++cv_pos) {
            result->set(*existence_it,
                        filter->get(*existence_it)
                            && this->compareDataPtrs(
                                column_vector.template getUntypedValue<left_nullable>(cv_pos),
                                right_accessor->template getUntypedValueAtAbsolutePosition<right_nullable>(
                                    right_id, *existence_it)));
            ++existence_it;
          }
        } else {
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            result->set(*filter_it,
                        this->compareDataPtrs(
                            column_vector.template getUntypedValue<left_nullable>(*filter_it),
                            right_accessor->template getUntypedValueAtAbsolutePosition<right_nullable>(
                                right_id, *filter_it)));
          }
        }
      } else {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < column_vector.size();
               ++cv_pos) {
            result->set(*existence_it,
                        this->compareDataPtrs(
                            column_vector.template getUntypedValue<left_nullable>(cv_pos),
                            right_accessor->template getUntypedValueAtAbsolutePosition<right_nullable>(
                                right_id, *existence_it)));
            ++existence_it;
          }
        } else {
          right_accessor->beginIteration();
          std::size_t cv_pos = 0;
          while (right_accessor->next()) {
            result->set(right_accessor->getCurrentPosition(),
                        this->compareDataPtrs(
                            column_vector.template getUntypedValue<left_nullable>(cv_pos),
                            right_accessor->template getUntypedValue<right_nullable>(right_id)));
            ++cv_pos;
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

template <bool left_nullable, bool right_nullable>
TupleIdSequence* UncheckedComparator::compareValueAccessorAndColumnVectorDefaultImpl(
    ValueAccessor *left_accessor,
    const attribute_id left_id,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      left_accessor,
      [&](auto *left_accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
    return InvokeOnColumnVector(
        right,
        [&](const auto &column_vector) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
      static constexpr bool short_circuit = true;
#else
      static constexpr bool short_circuit = false;
#endif
      DCHECK_EQ(column_vector.size(),
                static_cast<std::size_t>(left_accessor->getNumTuples()));
      DCHECK((existence_bitmap == nullptr)
             || (existence_bitmap->numTuples() == column_vector.size()));
      DCHECK((filter == nullptr)
             || ((existence_bitmap == nullptr) ? (filter->length() == column_vector.size())
                                               : (filter->length() == existence_bitmap->length())));
      TupleIdSequence *result = new TupleIdSequence(
          (existence_bitmap == nullptr) ? column_vector.size()
                                        : existence_bitmap->length());
      if (short_circuit && (filter != nullptr)) {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < column_vector.size();
               ++cv_pos) {
            result->set(*existence_it,
                        filter->get(*existence_it)
                            && this->compareDataPtrs(
                                left_accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(
                                    left_id, *existence_it),
                                column_vector.template getUntypedValue<right_nullable>(cv_pos)));
            ++existence_it;
          }
        } else {
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            result->set(*filter_it,
                        this->compareDataPtrs(
                            left_accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(
                                left_id, *filter_it),
                            column_vector.template getUntypedValue<right_nullable>(*filter_it)));
          }
        }
      } else {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < column_vector.size();
               ++cv_pos) {
            result->set(*existence_it,
                        this->compareDataPtrs(
                            left_accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(
                                left_id, *existence_it),
                            column_vector.template getUntypedValue<right_nullable>(cv_pos)));
            ++existence_it;
          }
        } else {
          left_accessor->beginIteration();
          std::size_t cv_pos = 0;
          while (left_accessor->next()) {
            result->set(left_accessor->getCurrentPosition(),
                        this->compareDataPtrs(
                            left_accessor->template getUntypedValue<left_nullable>(left_id),
                            column_vector.template getUntypedValue<right_nullable>(cv_pos)));
            ++cv_pos;
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

template <bool value_nullable>
TypedValue UncheckedComparator::accumulateValueAccessorDefaultImpl(
    const TypedValue &current,
    ValueAccessor *accessor,
    const attribute_id value_accessor_id) const {
  TypedValue accumulated = current;

  InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    accessor->beginIteration();
    while (accessor->next()) {
      TypedValue value = accessor->getTypedValue(value_accessor_id);
      if (value_nullable && value.isNull()) {
        continue;
      }
      if (accumulated.isNull() || this->compareTypedValues(value, accumulated)) {
        accumulated = value;
      }
    }
  });

  return accumulated;
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

template <bool value_nullable>
TypedValue UncheckedComparator::accumulateColumnVectorDefaultImpl(
    const TypedValue &current,
    const ColumnVector &column_vector) const {
  TypedValue accumulated = current;

  InvokeOnColumnVector(
      column_vector,
      [&](const auto &column_vector) -> void {  // NOLINT(build/c++11)
    for (std::size_t pos = 0;
         pos < column_vector.size();
         ++pos) {
      TypedValue value = column_vector.getTypedValue(pos);
      if (value_nullable && value.isNull()) {
        continue;
      }
      if (accumulated.isNull() || this->compareTypedValues(value, accumulated)) {
        accumulated = value;
      }
    }
  });

  return accumulated;
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_COMPARATORS_INL_HPP_
