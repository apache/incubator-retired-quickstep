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

#include "expressions/window_aggregation/WindowAggregationHandleAvg.hpp"

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/window_aggregation/WindowAggregationHandle.hpp"
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
    const std::vector<std::unique_ptr<const Scalar>> &partition_by_attributes,
    const std::vector<std::unique_ptr<const Scalar>> &order_by_attributes,
    const bool is_row,
    const std::int64_t num_preceding,
    const std::int64_t num_following,
    const Type *argument_type)
    : WindowAggregationHandle(partition_by_attributes,
                              order_by_attributes,
                              is_row,
                              num_preceding,
                              num_following) {
  // We sum Int as Long and Float as Double so that we have more headroom when
  // adding many values.
  TypeID type_id;
  switch (argument_type->getTypeID()) {
    case kInt:
    case kLong:
      type_id = kLong;
      break;
    case kFloat:
    case kDouble:
      type_id = kDouble;
      break;
    default:
      type_id = argument_type->getTypeID();
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
          .makeUncheckedBinaryOperatorForTypes(*sum_type_, *argument_type));

  // Subtract operator for dropping argument values off the window.
  fast_subtract_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract)
          .makeUncheckedBinaryOperatorForTypes(*sum_type_, *argument_type));

  // Divide operator for dividing sum by count to get final average.
  divide_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide)
          .makeUncheckedBinaryOperatorForTypes(*sum_type_, TypeFactory::GetType(kDouble)));
}

ColumnVector* WindowAggregationHandleAvg::calculate(
    ColumnVectorsValueAccessor *tuple_accessor,
    const std::vector<ColumnVector*> &arguments) const {
  DCHECK_EQ(1u, arguments.size());
  DCHECK(arguments[0]->isNative());
  DCHECK_EQ(static_cast<std::size_t>(tuple_accessor->getNumTuples()),
            static_cast<const NativeColumnVector*>(arguments[0])->size());

  // Initialize the output column and argument accessor.
  NativeColumnVector *window_aggregates =
      new NativeColumnVector(*result_type_, tuple_accessor->getNumTuples());
  ColumnVectorsValueAccessor *argument_accessor = new ColumnVectorsValueAccessor();
  argument_accessor->addColumn(arguments[0]);

  // Initialize the information about the window.
  TypedValue sum = sum_type_->makeZeroValue();
  std::uint64_t count = 0;
  tuple_id start_tuple_id = 0;  // The id of the first tuple in the window.
  tuple_id end_tuple_id = 0;  // The id of the tuple that just passed the last
                              // tuple in the window.

  // Create a window for each tuple and calculate the window aggregate.
  tuple_accessor->beginIteration();
  argument_accessor->beginIteration();

  while (tuple_accessor->next() && argument_accessor->next()) {
    tuple_id current_tuple_id = tuple_accessor->getCurrentPosition();

    // If current tuple is not in the same partition as the previous tuple,
    // reset the window.
    if (!samePartition(tuple_accessor, current_tuple_id - 1)) {
      start_tuple_id = current_tuple_id;
      end_tuple_id = current_tuple_id;
      count = 0;
      sum = sum_type_->makeZeroValue();
    }

    // Drop tuples that will be out of the window from the beginning.
    while (!inWindow(tuple_accessor, start_tuple_id)) {
      TypedValue start_value =
          argument_accessor->getTypedValueAtAbsolutePosition(0, start_tuple_id);
      // Ignore the value if NULL.
      if (!start_value.isNull()) {
        sum = fast_subtract_operator_->applyToTypedValues(sum, start_value);
        count--;
      }

      start_tuple_id++;
    }

    // Add tuples that will be included by the window at the end.
    while (inWindow(tuple_accessor, end_tuple_id)) {
      TypedValue end_value =
          argument_accessor->getTypedValueAtAbsolutePosition(0, end_tuple_id);

      // Ignore the value if NULL.
      if (!end_value.isNull()) {
        sum = fast_add_operator_->applyToTypedValues(sum, end_value);
        count++;
      }

      end_tuple_id++;
    }

    // If all values are NULLs, return NULL; Otherwise, return the quotient.
    if (count == 0) {
      window_aggregates->appendTypedValue(result_type_->makeNullValue());
    } else {
      window_aggregates->appendTypedValue(
          divide_operator_->applyToTypedValues(sum, TypedValue(static_cast<double>(count))));
    }
  }

  return window_aggregates;
}

}  // namespace quickstep
