/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARATORS_INL_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARATORS_INL_HPP_

#include "types/operations/comparisons/PatternMatchingComparators.hpp"

#include <cstddef>
#include <string>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/TupleIdSequence.hpp"

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorUtil.hpp"
#include "types/operations/comparisons/Comparison-inl.hpp"

#include "glog/logging.h"

namespace quickstep {

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareColumnVectors(
        const ColumnVector &left,
        const ColumnVector &right,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  // Use default implementation.
  return compareColumnVectorsDefaultImpl<left_nullable, right_nullable>(
      left, right, filter, existence_bitmap);
}


template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareColumnVectorAndStaticValue(
        const ColumnVector &column_vector,
        const TypedValue &static_value,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  // Specialized implementation for matching a ColumnVector of strings to a
  // single pattern. In this situation, the pattern will be compiled only once
  // in advance and then matched by each of the string in the ColumnVector.
  return InvokeOnColumnVector(
      column_vector,
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
    if (right_nullable && static_value.isNull()) {
      return result;
    }

    // Transform and compile pattern in advance before the loop.
    const char *pattern = static_cast<const char *>(static_value.getDataPtr());
    std::string regex_pattern;
    re2::StringPiece pattern_piece;
    int orig_pattern_len = strnlen(pattern, right_length_);
    if (is_like_pattern) {
      regex_pattern = transformLikeToRegex(pattern, orig_pattern_len);
      pattern_piece.set(regex_pattern.c_str(), regex_pattern.size());
    } else {
      pattern_piece.set(pattern, orig_pattern_len);
    }
    const re2::RE2 re2_pattern(pattern_piece);

    if (short_circuit && (filter != nullptr)) {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          if (filter->get(*existence_it)) {
            const void *cv_value
                = column_vector.template getUntypedValue<left_nullable>(cv_pos);
            result->set(*existence_it,
                        !(left_nullable && (cv_value == nullptr))
                            && this->matchDataPtrWithPattern(cv_value, re2_pattern));
          }
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          const void *cv_value
              = column_vector.template getUntypedValue<left_nullable>(*filter_it);
          result->set(*filter_it,
                      !(left_nullable && (cv_value == nullptr))
                          && this->matchDataPtrWithPattern(cv_value, re2_pattern));
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
              = column_vector.template getUntypedValue<left_nullable>(cv_pos);
          result->set(*existence_it,
                      !(left_nullable && (cv_value == nullptr))
                          && this->matchDataPtrWithPattern(cv_value, re2_pattern));
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (std::size_t pos = 0;
             pos < column_vector.size();
             ++pos) {
          const void *cv_value
              = column_vector.template getUntypedValue<left_nullable>(pos);
          result->set(pos,
                      !(left_nullable && (cv_value == nullptr))
                          && this->matchDataPtrWithPattern(cv_value, re2_pattern));
        }
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareStaticValueAndColumnVector(
        const TypedValue &left,
        const ColumnVector &right,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  // Use default implementation.
  return compareStaticValueAndColumnVectorDefaultImpl<left_nullable, right_nullable>(
      left, right, filter, existence_bitmap);
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareSingleValueAccessor(
        ValueAccessor *accessor,
        const attribute_id left_id,
        const attribute_id right_id,
        const TupleIdSequence *filter) const {
  // Use default implementation.
  return compareSingleValueAccessorDefaultImpl<left_nullable, right_nullable>(
      accessor, left_id, right_id, filter);
}

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareValueAccessorAndStaticValue(
        ValueAccessor *accessor,
        const attribute_id value_accessor_attr_id,
        const TypedValue &static_value,
        const TupleIdSequence *filter) const {
  // Specialized implementation for matching a ValueAccessor of strings to a
  // single pattern. In this situation, the pattern will be compiled only once
  // in advance and then matched by each of the string in the ValueAccessor.
  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    TupleIdSequence *result = new TupleIdSequence(accessor->getEndPosition());
    if (right_nullable && static_value.isNull()) {
      return result;
    }

    // Transform and compile pattern in advance before the loop.
    const char *pattern = static_cast<const char *>(static_value.getDataPtr());
    std::string regex_pattern;
    re2::StringPiece pattern_piece;
    int orig_pattern_len = strnlen(pattern, right_length_);
    if (is_like_pattern) {
      regex_pattern = transformLikeToRegex(pattern, orig_pattern_len);
      pattern_piece.set(regex_pattern.c_str(), regex_pattern.size());
    } else {
      pattern_piece.set(pattern, orig_pattern_len);
    }
    const re2::RE2 re2_pattern(pattern_piece);

    if (short_circuit && (filter != nullptr)) {
      DCHECK_EQ(filter->length(), result->length());
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        const void *va_value
            = accessor->template getUntypedValueAtAbsolutePosition<left_nullable>(value_accessor_attr_id,
                                                                                  *filter_it);
        result->set(*filter_it,
                    this->matchDataPtrWithPattern(va_value, re2_pattern));
      }
    } else {
      accessor->beginIteration();
      while (accessor->next()) {
        const void *va_value
            = accessor->template getUntypedValue<left_nullable>(value_accessor_attr_id);
        result->set(accessor->getCurrentPosition(),
                    this->matchDataPtrWithPattern(va_value, re2_pattern));
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareStaticValueAndValueAccessor(
        const TypedValue &left,
        ValueAccessor *right_accessor,
        const attribute_id right_id,
        const TupleIdSequence *filter) const {
  // Use default implementation.
  return compareStaticValueAndValueAccessorDefaultImpl<left_nullable, right_nullable>(
      left, right_accessor, right_id, filter);
}

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareColumnVectorAndValueAccessor(
        const ColumnVector &left,
        ValueAccessor *right_accessor,
        const attribute_id right_id,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  // Use default implementation.
  return compareColumnVectorAndValueAccessorDefaultImpl<left_nullable, right_nullable>(
      left, right_accessor, right_id, filter, existence_bitmap);
}

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TupleIdSequence* PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::compareValueAccessorAndColumnVector(
        ValueAccessor *left_accessor,
        const attribute_id left_id,
        const ColumnVector &right,
        const TupleIdSequence *filter,
        const TupleIdSequence *existence_bitmap) const {
  // Use default implementation.
  return compareValueAccessorAndColumnVectorDefaultImpl<left_nullable, right_nullable>(
      left_accessor, left_id, right, filter, existence_bitmap);
}

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TypedValue PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::accumulateValueAccessor(
        const TypedValue &current,
        ValueAccessor *accessor,
        const attribute_id value_accessor_id) const {
  // Use default implementation.
  return accumulateValueAccessorDefaultImpl<left_nullable>(
      current, accessor, value_accessor_id);
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
TypedValue PatternMatchingUncheckedComparator<is_like_pattern, is_negation,
                                                    left_nullable, right_nullable>
    ::accumulateColumnVector(
        const TypedValue &current,
        const ColumnVector &column_vector) const {
  // Use default implementation.
  return accumulateColumnVectorDefaultImpl<left_nullable>(
      current, column_vector);
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARATORS_INL_HPP_
