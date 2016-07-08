/**
 *   Licensed to the Apache Software Foundation (ASF) under one
 *   or more contributor license agreements.  See the NOTICE file
 *   distributed with this work for additional information
 *   regarding copyright ownership.  The ASF licenses this file
 *   to you under the Apache License, Version 2.0 (the
 *   "License"); you may not use this file except in compliance
 *   with the License.  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *   Unless required by applicable law or agreed to in writing,
 *   software distributed under the License is distributed on an
 *   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *   KIND, either express or implied.  See the License for the
 *   specific language governing permissions and limitations
 *   under the License.
 **/

#include "expressions/window_aggregation/WindowAggregationHandleAvg.hpp"

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/Comparison.hpp"

#include "glog/logging.h"

namespace quickstep {

WindowAggregationHandleAvg::WindowAggregationHandleAvg(
    const std::vector<const Type*> &partition_key_types,
    const Type &type)
    : WindowAggregationHandle(partition_key_types),
      argument_type_(type) {
  // We sum Int as Long and Float as Double so that we have more headroom when
  // adding many values.
  TypeID type_id;
  switch (type.getTypeID()) {
    case kInt:
    case kLong:
      type_id = kLong;
      break;
    case kFloat:
    case kDouble:
      type_id = kDouble;
      break;
    default:
      type_id = type.getTypeID();
      break;
  }

  sum_type_ = &(TypeFactory::GetType(type_id));

  // Result is nullable, because AVG() over 0 values (or all NULL values) is
  // NULL.
  result_type_
      = &(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide)
              .resultTypeForArgumentTypes(*sum_type_, TypeFactory::GetType(kDouble))
                  ->getNullableVersion());

  // Make operators to do arithmetic:
  // Add operator for summing argument values.
  fast_add_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
          .makeUncheckedBinaryOperatorForTypes(*sum_type_, argument_type_));
  // Divide operator for dividing sum by count to get final average.
  divide_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide)
          .makeUncheckedBinaryOperatorForTypes(*sum_type_, TypeFactory::GetType(kDouble)));
}

ColumnVector* WindowAggregationHandleAvg::calculate(
    ColumnVectorsValueAccessor *tuple_accessor,
    std::vector<ColumnVector*> &&arguments,
    const std::vector<attribute_id> &partition_by_ids,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following) const {
  DCHECK_EQ(1u, arguments.size());
  DCHECK(arguments[0]->isNative());
  DCHECK_EQ(static_cast<std::size_t>(tuple_accessor->getNumTuples()),
            static_cast<const NativeColumnVector*>(arguments[0])->size());

  // Initialize the output column and argument accessor.
  NativeColumnVector *window_aggregates =
      new NativeColumnVector(*result_type_, tuple_accessor->getNumTuples());
  ColumnVectorsValueAccessor* argument_accessor = new ColumnVectorsValueAccessor();
  argument_accessor->addColumn(arguments[0]);

  // Create a window for each tuple and calculate the window aggregate.
  tuple_accessor->beginIteration();
  argument_accessor->beginIteration();

  while (tuple_accessor->next() && argument_accessor->next()) {
    const TypedValue window_aggregate = this->calculateOneWindow(tuple_accessor,
                                                                 argument_accessor,
                                                                 partition_by_ids,
                                                                 is_row,
                                                                 num_preceding,
                                                                 num_following);
    window_aggregates->appendTypedValue(window_aggregate);
  }

  return window_aggregates;
}

TypedValue WindowAggregationHandleAvg::calculateOneWindow(
    ColumnVectorsValueAccessor *tuple_accessor,
    ColumnVectorsValueAccessor *argument_accessor,
    const std::vector<attribute_id> &partition_by_ids,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following) const {
  // Initialize.
  TypedValue sum = sum_type_->makeZeroValue();
  TypedValue current_value = argument_accessor->getTypedValue(0);
  std::uint64_t count = 0;

  // Ignore the value if null.
  if (!current_value.isNull()) {
    sum = fast_add_operator_->applyToTypedValues(sum, current_value);
    count++;
  }

  // Get the partition key for the current row.
  std::vector<TypedValue> current_row_partition_key;
  for (attribute_id partition_by_id : partition_by_ids) {
    current_row_partition_key.push_back(
        tuple_accessor->getTypedValue(partition_by_id));
  }

  // Get current position.
  tuple_id current_tuple_id = tuple_accessor->getCurrentPositionVirtual();

  // Find preceding tuples.
  int count_preceding = 0;
  tuple_id preceding_tuple_id = current_tuple_id;
  while (num_preceding == -1 || count_preceding < num_preceding) {
    preceding_tuple_id--;

    // No more preceding tuples.
    if (preceding_tuple_id < 0) {
      break;
    }

    // Get the partition keys and compare. If not the same partition as the
    // current row, stop searching preceding tuples.
    if (!samePartition(tuple_accessor,
                       current_row_partition_key,
                       preceding_tuple_id,
                       partition_by_ids)) {
      break;
    }

    // Actually count the element and do the calculation.
    count_preceding++;
    TypedValue preceding_value =
        argument_accessor->getTypedValueAtAbsolutePosition(0, preceding_tuple_id);

    // Ignore the value if null.
    if (!preceding_value.isNull()) {
      sum = fast_add_operator_->applyToTypedValues(sum, preceding_value);
      count++;
    }
  }

  // Find following tuples.
  int count_following = 0;
  tuple_id following_tuple_id = current_tuple_id;
  while (num_following == -1 || count_following < num_following) {
    following_tuple_id++;

    // No more following tuples.
    if (following_tuple_id == tuple_accessor->getNumTuples()) {
      break;
    }

    // Get the partition keys and compare. If not the same partition as the
    // current row, stop searching preceding tuples.
    if (!samePartition(tuple_accessor,
                       current_row_partition_key,
                       following_tuple_id,
                       partition_by_ids)) {
      break;
    }

    // Actually count the element and do the calculation.
    count_following++;
    TypedValue following_value =
        argument_accessor->getTypedValueAtAbsolutePosition(0, following_tuple_id);

    // Ignore the value if null.
    if (!following_value.isNull()) {
      sum = fast_add_operator_->applyToTypedValues(sum, following_value);
      count++;
    }
  }

  // If all values are NULLs, return NULL; Otherwise, return the quotient.
  if (count == 0) {
    return result_type_->makeNullValue();
  } else {
    return divide_operator_->applyToTypedValues(sum,
                                                TypedValue(static_cast<double>(count)));
  }
}

bool WindowAggregationHandleAvg::samePartition(
    const ColumnVectorsValueAccessor *tuple_accessor,
    const std::vector<TypedValue> &current_row_partition_key,
    const tuple_id boundary_tuple_id,
    const std::vector<attribute_id> &partition_by_ids) const {
  for (std::size_t partition_by_index = 0;
       partition_by_index < partition_by_ids.size();
       ++partition_by_index) {
    if (!equal_comparators_[partition_by_index]->compareTypedValues(
            current_row_partition_key[partition_by_index],
            tuple_accessor->getTypedValueAtAbsolutePosition(
                partition_by_ids[partition_by_index], boundary_tuple_id))) {
      return false;
    }
  }

  return true;
}

}  // namespace quickstep
