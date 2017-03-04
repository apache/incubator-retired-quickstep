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

#include "expressions/window_aggregation/WindowAggregationHandle.hpp"

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/operations/OperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/comparisons/Comparison.hpp"

#include "glog/logging.h"

namespace quickstep {

WindowAggregationHandle::WindowAggregationHandle(
    const std::vector<std::unique_ptr<const Scalar>> &partition_by_attributes,
    const std::vector<std::unique_ptr<const Scalar>> &order_by_attributes,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following)
    : is_row_(is_row),
      num_preceding_(num_preceding),
      num_following_(num_following) {
  // IDs and types of partition keys.
  std::vector<const Type*> partition_key_types;
  for (const std::unique_ptr<const Scalar> &partition_by_attribute : partition_by_attributes) {
    partition_key_ids_.push_back(
        partition_by_attribute->getAttributeIdForValueAccessor());
    partition_key_types.push_back(&partition_by_attribute->getType());
  }

  // Comparison operators for checking if two tuples belong to the same partition.
  for (const Type *partition_key_type : partition_key_types) {
    partition_equal_comparators_.emplace_back(
        ComparisonFactory::GetComparison(ComparisonID::kEqual)
            .makeUncheckedComparatorForTypes(*partition_key_type, *partition_key_type));
  }

  // IDs and types of order keys.
  const Type *first_order_key_type = nullptr;
  for (const std::unique_ptr<const Scalar> &order_by_attribute : order_by_attributes) {
    order_key_ids_.push_back(
        order_by_attribute->getAttributeIdForValueAccessor());
    if (first_order_key_type == nullptr) {
      first_order_key_type = &order_by_attribute->getType();
    }
  }

  // ID and type of the first order key if in RANGE mode.
  if (!is_row) {
    DCHECK(first_order_key_type != nullptr);

    // Comparators and operators to check window frame in RANGE mode.
    const Type &long_type = TypeFactory::GetType(kLong, false);
    range_compare_type_ =
        TypeFactory::GetUnifyingType(*first_order_key_type, long_type);

    range_add_operator_.reset(
        OperationFactory::Instance().getBinaryOperation(
            "+", {first_order_key_type->getTypeID(), kLong}, 0)
                ->makeUncheckedBinaryOperator(*first_order_key_type, long_type));
    range_comparator_.reset(
        ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual)
            .makeUncheckedComparatorForTypes(*range_compare_type_, *range_compare_type_));
  }
}

bool WindowAggregationHandle::samePartition(
    const ColumnVectorsValueAccessor *tuple_accessor,
    const tuple_id test_tuple_id) const {
  // If test tuple does not exist.
  if (test_tuple_id < 0 ||
      test_tuple_id >= tuple_accessor->getNumTuples()) {
    return false;
  }

  // Check all partition by attributes.
  for (std::size_t partition_by_index = 0;
       partition_by_index < partition_key_ids_.size();
       ++partition_by_index) {
    if (!partition_equal_comparators_[partition_by_index]->compareTypedValues(
            tuple_accessor->getTypedValue(partition_key_ids_[partition_by_index]),
            tuple_accessor->getTypedValueAtAbsolutePosition(
                partition_key_ids_[partition_by_index], test_tuple_id))) {
      return false;
    }
  }

  return true;
}

bool WindowAggregationHandle::inWindow(
    const ColumnVectorsValueAccessor *tuple_accessor,
    const tuple_id test_tuple_id) const {
  // If test tuple does not exist.
  if (!samePartition(tuple_accessor, test_tuple_id)) {
    return false;
  }

  tuple_id current_tuple_id = tuple_accessor->getCurrentPosition();

  // If test tuple is the current tuple, then it is in the window.
  if (test_tuple_id == current_tuple_id) {
    return true;
  }

  // In ROWS mode, check the difference of tuple_id.
  if (is_row_) {
    if (num_preceding_ != -1 &&
        test_tuple_id < current_tuple_id - num_preceding_) {
      return false;
    }

    if (num_following_ != -1 &&
        test_tuple_id > current_tuple_id + num_following_) {
      return false;
    }
  } else {
    // In RANGE mode, check the difference of first order key value.
    // Get the test value.
    const Type &long_type = TypeFactory::GetType(kLong, false);
    TypedValue test_value =
        range_add_operator_->applyToTypedValues(
            tuple_accessor->getTypedValueAtAbsolutePosition(order_key_ids_[0], test_tuple_id),
            long_type.makeZeroValue());

    // NULL will be considered not in range.
    if (test_value.isNull() ||
        tuple_accessor->getTypedValue(order_key_ids_[0]).isNull()) {
      return false;
    }

    // Get the boundary value if it is not UNBOUNDED.
    if (num_preceding_ > -1) {
      // num_preceding needs to be negated for calculation.
      std::int64_t neg_num_preceding = -num_preceding_;
      TypedValue start_boundary_value =
          range_add_operator_->applyToTypedValues(
              tuple_accessor->getTypedValue(order_key_ids_[0]),
              long_type.makeValue(&neg_num_preceding));
      if (!range_comparator_->compareTypedValues(start_boundary_value, test_value)) {
        return false;
      }
    }

    if (num_following_ > -1) {
      TypedValue end_boundary_value =
          range_add_operator_->applyToTypedValues(
              tuple_accessor->getTypedValue(order_key_ids_[0]),
              long_type.makeValue(&num_following_));
      if (!range_comparator_->compareTypedValues(test_value, end_boundary_value)) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace quickstep
