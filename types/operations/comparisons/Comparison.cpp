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

#include "types/operations/comparisons/Comparison.hpp"

#include "types/TypedValue.hpp"
#include "types/operations/Operation.pb.h"

#include "types/operations/comparisons/Comparison-inl.hpp"

#include "utility/Macros.hpp"

namespace quickstep {

TupleIdSequence* UncheckedComparator::compareColumnVectors(
    const ColumnVector &left,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareColumnVectors()");
  return compareColumnVectorsDefaultImpl<true, true>(
      left, right, filter, existence_bitmap);
}

TupleIdSequence* UncheckedComparator::compareColumnVectorAndStaticValue(
    const ColumnVector &left,
    const TypedValue &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareColumnVectorAndStaticValue()");
  return compareColumnVectorAndStaticValueDefaultImpl<true, true>(
      left, right, filter, existence_bitmap);
}

TupleIdSequence* UncheckedComparator::compareStaticValueAndColumnVector(
    const TypedValue &left,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareStaticValueAndColumnVector()");
  return compareStaticValueAndColumnVectorDefaultImpl<true, true>(
      left, right, filter, existence_bitmap);
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
TupleIdSequence* UncheckedComparator::compareSingleValueAccessor(
    ValueAccessor *accessor,
    const attribute_id left_id,
    const attribute_id right_id,
    const TupleIdSequence *filter) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareSingleValueAccessor()");
  return compareSingleValueAccessorDefaultImpl<true, true>(
      accessor, left_id, right_id, filter);
}

TupleIdSequence* UncheckedComparator::compareValueAccessorAndStaticValue(
    ValueAccessor *left_accessor,
    const attribute_id left_id,
    const TypedValue &right,
    const TupleIdSequence *filter) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareValueAccessorAndStaticValue()");
  return compareValueAccessorAndStaticValueDefaultImpl<true, true>(
      left_accessor, left_id, right, filter);
}

TupleIdSequence* UncheckedComparator::compareStaticValueAndValueAccessor(
    const TypedValue &left,
    ValueAccessor *right_accessor,
    const attribute_id right_id,
    const TupleIdSequence *filter) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareStaticValueAndValueAccessor()");
  return compareStaticValueAndValueAccessorDefaultImpl<true, true>(
      left, right_accessor, right_id, filter);
}

TupleIdSequence* UncheckedComparator::compareColumnVectorAndValueAccessor(
    const ColumnVector &left,
    ValueAccessor *right_accessor,
    const attribute_id right_id,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareColumnVectorAndValueAccessor()");
  return compareColumnVectorAndValueAccessorDefaultImpl<true, true>(
      left, right_accessor, right_id, filter, existence_bitmap);
}

TupleIdSequence* UncheckedComparator::compareValueAccessorAndColumnVector(
    ValueAccessor *left_accessor,
    const attribute_id left_id,
    const ColumnVector &right,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_bitmap) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::compareValueAccessorAndColumnVector()");
  return compareValueAccessorAndColumnVectorDefaultImpl<true, true>(
      left_accessor, left_id, right, filter, existence_bitmap);
}

TypedValue UncheckedComparator::accumulateValueAccessor(
    const TypedValue &current,
    ValueAccessor *accessor,
    const attribute_id value_accessor_id) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::accumulateValueAccessor()");
  return accumulateValueAccessorDefaultImpl<true>(
      current, accessor, value_accessor_id);
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

TypedValue UncheckedComparator::accumulateColumnVector(
    const TypedValue &current,
    const ColumnVector &column_vector) const {
  DEV_WARNING("Using fallback non-specialized implementation of "
              "UncheckedComparator::accumulateColumnVector()");
  return accumulateColumnVectorDefaultImpl<true>(current, column_vector);
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
    case ComparisonID::kLike:
      proto.set_comparison_id(serialization::Comparison::LIKE);
      break;
    case ComparisonID::kNotLike:
      proto.set_comparison_id(serialization::Comparison::NOT_LIKE);
      break;
    case ComparisonID::kRegexMatch:
      proto.set_comparison_id(serialization::Comparison::REGEX_MATCH);
      break;
    case ComparisonID::kNotRegexMatch:
      proto.set_comparison_id(serialization::Comparison::NOT_REGEX_MATCH);
      break;
    default:
      FATAL_ERROR("Unrecognized ComparisonID in Comparison::getProto");
  }

  return proto;
}

}  // namespace quickstep
