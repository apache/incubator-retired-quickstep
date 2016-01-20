/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "types/operations/comparisons/Comparison.hpp"

#include <cstddef>
#include <cstring>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/TupleIdSequence.hpp"

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorUtil.hpp"
#include "types/operations/Operation.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

using std::strcmp;

namespace quickstep {

TupleIdSequence* UncheckedComparator::compareColumnVectors(
    const ColumnVector &left,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareColumnVectors()");
  DCHECK((filter == nullptr)
         || (existence_bitmap == nullptr)
         || (existence_bitmap->length() == filter->length()));

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
                            && this->compareDataPtrs(
                                left_column_vector.getUntypedValue(cv_pos),
                                right_column_vector.getUntypedValue(cv_pos)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            result->set(*filter_it,
                        this->compareDataPtrs(
                            left_column_vector.getUntypedValue(*filter_it),
                            right_column_vector.getUntypedValue(*filter_it)));
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
                        this->compareDataPtrs(
                            left_column_vector.getUntypedValue(cv_pos),
                            right_column_vector.getUntypedValue(cv_pos)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          for (std::size_t pos = 0;
               pos < left_column_vector.size();
               ++pos) {
            result->set(pos,
                        this->compareDataPtrs(
                            left_column_vector.getUntypedValue(pos),
                            right_column_vector.getUntypedValue(pos)));
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

TupleIdSequence* UncheckedComparator::compareColumnVectorAndStaticValue(
    const ColumnVector &left,
    const TypedValue &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareColumnVectorAndStaticValue()");
  DCHECK((filter == nullptr)
         || (existence_bitmap == nullptr)
         || (existence_bitmap->length() == filter->length()));

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
    TupleIdSequence *result = new TupleIdSequence(
        (existence_bitmap == nullptr) ? column_vector.size()
                                      : existence_bitmap->length());
    if (short_circuit && (filter != nullptr)) {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          result->set(*existence_it,
                      filter->get(*existence_it)
                          && this->compareDataPtrWithTypedValue(
                              column_vector.getUntypedValue(cv_pos),
                              right));
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          result->set(*filter_it,
                      this->compareDataPtrWithTypedValue(
                          column_vector.getUntypedValue(*filter_it),
                          right));
        }
      }
    } else {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          result->set(*existence_it,
                      this->compareDataPtrWithTypedValue(
                          column_vector.getUntypedValue(cv_pos),
                          right));
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (std::size_t pos = 0;
             pos < column_vector.size();
             ++pos) {
          result->set(pos,
                      this->compareDataPtrWithTypedValue(
                          column_vector.getUntypedValue(pos),
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

TupleIdSequence* UncheckedComparator::compareStaticValueAndColumnVector(
    const TypedValue &left,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareStaticValueAndColumnVector()");
  DCHECK((filter == nullptr)
         || (existence_bitmap == nullptr)
         || (existence_bitmap->length() == filter->length()));

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
    TupleIdSequence *result = new TupleIdSequence(
        (existence_bitmap == nullptr) ? column_vector.size()
                                      : existence_bitmap->length());
    if (short_circuit && (filter != nullptr)) {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          result->set(*existence_it,
                      filter->get(*existence_it)
                          && this->compareTypedValueWithDataPtr(
                              left,
                              column_vector.getUntypedValue(cv_pos)));
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (TupleIdSequence::const_iterator filter_it = filter->begin();
             filter_it != filter->end();
             ++filter_it) {
          result->set(*filter_it,
                      this->compareTypedValueWithDataPtr(
                          left,
                          column_vector.getUntypedValue(*filter_it)));
        }
      }
    } else {
      if (existence_bitmap != nullptr) {
        TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
        for (std::size_t cv_pos = 0;
             cv_pos < column_vector.size();
             ++cv_pos) {
          DCHECK(existence_it != existence_bitmap->end());
          result->set(*existence_it,
                      this->compareTypedValueWithDataPtr(
                          left,
                          column_vector.getUntypedValue(cv_pos)));
          ++existence_it;
        }
        DCHECK(existence_it == existence_bitmap->end());
      } else {
        for (std::size_t pos = 0;
             pos < column_vector.size();
             ++pos) {
          result->set(pos,
                      this->compareTypedValueWithDataPtr(
                          left,
                          column_vector.getUntypedValue(pos)));
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
TupleIdSequence* UncheckedComparator::compareSingleValueAccessor(
    ValueAccessor *accessor,
    const attribute_id left_id,
    const attribute_id right_id,
    const TupleIdSequence *filter) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareSingleValueAccessor()");

  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    TupleIdSequence *result = new TupleIdSequence(accessor->getEndPosition());
    DCHECK((filter == nullptr) || (filter->length() == result->length()));
    if (short_circuit && (filter != nullptr)) {
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        result->set(*filter_it,
                    this->compareDataPtrs(
                        accessor->getUntypedValueAtAbsolutePosition(left_id, *filter_it),
                        accessor->getUntypedValueAtAbsolutePosition(right_id, *filter_it)));
      }
    } else {
      accessor->beginIteration();
      while (accessor->next()) {
        result->set(accessor->getCurrentPosition(),
                    this->compareDataPtrs(
                        accessor->getUntypedValue(left_id),
                        accessor->getUntypedValue(right_id)));
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

TupleIdSequence* UncheckedComparator::compareValueAccessorAndStaticValue(
    ValueAccessor *left_accessor,
    const attribute_id left_id,
    const TypedValue &right,
    const TupleIdSequence *filter) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareValueAccessorAndStaticValue()");

  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      left_accessor,
      [&](auto *left_accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    TupleIdSequence *result = new TupleIdSequence(left_accessor->getEndPosition());
    DCHECK((filter == nullptr) || (filter->length() == result->length()));
    if (short_circuit && (filter != nullptr)) {
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        result->set(*filter_it,
                    this->compareDataPtrWithTypedValue(
                        left_accessor->getUntypedValueAtAbsolutePosition(left_id, *filter_it),
                        right));
      }
    } else {
      left_accessor->beginIteration();
      while (left_accessor->next()) {
        result->set(left_accessor->getCurrentPosition(),
                    this->compareDataPtrWithTypedValue(
                        left_accessor->getUntypedValue(left_id),
                        right));
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

TupleIdSequence* UncheckedComparator::compareStaticValueAndValueAccessor(
    const TypedValue &left,
    ValueAccessor *right_accessor,
    const attribute_id right_id,
    const TupleIdSequence *filter) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareStaticValueAndValueAccessor()");

  return InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      right_accessor,
      [&](auto *right_accessor) -> TupleIdSequence* {  // NOLINT(build/c++11)
#ifdef QUICKSTEP_ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT
    static constexpr bool short_circuit = true;
#else
    static constexpr bool short_circuit = false;
#endif
    TupleIdSequence *result = new TupleIdSequence(right_accessor->getEndPosition());
    DCHECK((filter == nullptr) || (filter->length() == result->length()));
    if (short_circuit && (filter != nullptr)) {
      for (TupleIdSequence::const_iterator filter_it = filter->begin();
           filter_it != filter->end();
           ++filter_it) {
        result->set(*filter_it,
                    this->compareTypedValueWithDataPtr(
                        left,
                        right_accessor->getUntypedValueAtAbsolutePosition(right_id, *filter_it)));
      }
    } else {
      right_accessor->beginIteration();
      while (right_accessor->next()) {
        result->set(right_accessor->getCurrentPosition(),
                    this->compareTypedValueWithDataPtr(
                        left,
                        right_accessor->getUntypedValue(right_id)));
      }
      if (!short_circuit && (filter != nullptr)) {
        result->intersectWith(*filter);
      }
    }

    return result;
  });
}

TupleIdSequence* UncheckedComparator::compareColumnVectorAndValueAccessor(
    const ColumnVector &left,
    ValueAccessor *right_accessor,
    const attribute_id right_id,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareColumnVectorAndValueAccessor()");
  DCHECK((filter == nullptr)
         || (existence_bitmap == nullptr)
         || (existence_bitmap->length() == filter->length()));

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
            result->set(*existence_it,
                        filter->get(*existence_it)
                            && this->compareDataPtrs(
                                column_vector.getUntypedValue(cv_pos),
                                right_accessor->getUntypedValueAtAbsolutePosition(right_id, *existence_it)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            result->set(*filter_it,
                        this->compareDataPtrs(
                            column_vector.getUntypedValue(*filter_it),
                            right_accessor->getUntypedValueAtAbsolutePosition(right_id, *filter_it)));
          }
        }
      } else {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < column_vector.size();
               ++cv_pos) {
            DCHECK(existence_it != existence_bitmap->end());
            result->set(*existence_it,
                        this->compareDataPtrs(
                            column_vector.getUntypedValue(cv_pos),
                            right_accessor->getUntypedValueAtAbsolutePosition(right_id, *existence_it)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          right_accessor->beginIteration();
          std::size_t cv_pos = 0;
          while (right_accessor->next()) {
            result->set(right_accessor->getCurrentPosition(),
                        this->compareDataPtrs(
                            column_vector.getUntypedValue(cv_pos),
                            right_accessor->getUntypedValue(right_id)));
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

TupleIdSequence* UncheckedComparator::compareValueAccessorAndColumnVector(
    ValueAccessor *left_accessor,
    const attribute_id left_id,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareValueAccessorAndColumnVector()");
  DCHECK((filter == nullptr)
         || (existence_bitmap == nullptr)
         || (existence_bitmap->length() == filter->length()));

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
            result->set(*existence_it,
                        filter->get(*existence_it)
                            && this->compareDataPtrs(
                                left_accessor->getUntypedValueAtAbsolutePosition(left_id, *existence_it),
                                column_vector.getUntypedValue(cv_pos)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          for (TupleIdSequence::const_iterator filter_it = filter->begin();
               filter_it != filter->end();
               ++filter_it) {
            result->set(*filter_it,
                        this->compareDataPtrs(
                            left_accessor->getUntypedValueAtAbsolutePosition(left_id, *filter_it),
                            column_vector.getUntypedValue(*filter_it)));
          }
        }
      } else {
        if (existence_bitmap != nullptr) {
          TupleIdSequence::const_iterator existence_it = existence_bitmap->begin();
          for (std::size_t cv_pos = 0;
               cv_pos < column_vector.size();
               ++cv_pos) {
            DCHECK(existence_it != existence_bitmap->end());
            result->set(*existence_it,
                        this->compareDataPtrs(
                            left_accessor->getUntypedValueAtAbsolutePosition(left_id, *existence_it),
                            column_vector.getUntypedValue(cv_pos)));
            ++existence_it;
          }
          DCHECK(existence_it == existence_bitmap->end());
        } else {
          left_accessor->beginIteration();
          std::size_t cv_pos = 0;
          while (left_accessor->next()) {
            result->set(left_accessor->getCurrentPosition(),
                        this->compareDataPtrs(
                            left_accessor->getUntypedValue(left_id),
                            column_vector.getUntypedValue(cv_pos)));
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

TypedValue UncheckedComparator::accumulateValueAccessor(
    const TypedValue &current,
    ValueAccessor *accessor,
    const attribute_id value_accessor_id) const {
  TypedValue accumulated = current;
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::accumulateValueAccessor()");

  InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    accessor->beginIteration();
    while (accessor->next()) {
      TypedValue value = accessor->getTypedValue(value_accessor_id);
      if (value.isNull()) {
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

TypedValue UncheckedComparator::accumulateColumnVector(
    const TypedValue &current,
    const ColumnVector &column_vector) const {
  TypedValue accumulated = current;
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::accumulateColumnVector()");

  InvokeOnColumnVector(
      column_vector,
      [&](const auto &column_vector) -> void {  // NOLINT(build/c++11)
    for (std::size_t pos = 0;
         pos < column_vector.size();
         ++pos) {
      TypedValue value = column_vector.getTypedValue(pos);
      if (value.isNull()) {
        continue;
      }
      if (accumulated.isNull() || this->compareTypedValues(value, accumulated)) {
        accumulated = value;
      }
    }
  });

  return accumulated;
}

serialization::Comparison Comparison::getProto() const {
  serialization::Comparison proto;
  switch (comparison_id_) {
    case ComparisonID::kEqual:
      proto.set_comparison_id(serialization::Comparison::EQUAL);
      break;
    case ComparisonID::kNotEqual:
      proto.set_comparison_id(serialization::Comparison::NOT_EQUAL);
      break;
    case ComparisonID::kLess:
      proto.set_comparison_id(serialization::Comparison::LESS);
      break;
    case ComparisonID::kLessOrEqual:
      proto.set_comparison_id(serialization::Comparison::LESS_OR_EQUAL);
      break;
    case ComparisonID::kGreater:
      proto.set_comparison_id(serialization::Comparison::GREATER);
      break;
    case ComparisonID::kGreaterOrEqual:
      proto.set_comparison_id(serialization::Comparison::GREATER_OR_EQUAL);
      break;
    default:
      FATAL_ERROR("Unrecognized ComparisonID in Comparison::getProto");
  }

  return proto;
}

}  // namespace quickstep
