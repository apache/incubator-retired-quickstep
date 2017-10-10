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

#include "expressions/aggregation/AggregationHandleAvg.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "threading/SpinMutex.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/OperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;

AggregationHandleAvg::AggregationHandleAvg(const Type &type)
    : AggregationConcreteHandle(AggregationID::kAvg),
      argument_type_(type) {
  // We sum Int as Long and Float as Double so that we have more headroom when
  // adding many values.
  TypeID type_precision_id;
  switch (type.getTypeID()) {
    case kInt:
    case kLong:
      type_precision_id = kLong;
      break;
    case kFloat:
    case kDouble:
      type_precision_id = kDouble;
      break;
    default:
      type_precision_id = type.getTypeID();
      break;
  }

  const Type &sum_type = TypeFactory::GetType(type_precision_id);
  blank_state_.sum_ = sum_type.makeZeroValue();
  blank_state_.count_ = 0;

  // Make operators to do arithmetic:
  // Add operator for summing argument values.
  fast_add_operator_.reset(
      OperationFactory::GetAddOperation(type_precision_id, argument_type_.getTypeID())
          ->makeUncheckedBinaryOperator(sum_type, argument_type_));
  // Add operator for merging states.
  merge_add_operator_.reset(
      OperationFactory::GetAddOperation(type_precision_id, type_precision_id)
          ->makeUncheckedBinaryOperator(sum_type, sum_type));
  // Divide operator for dividing sum by count to get final average.
  divide_operator_.reset(
      OperationFactory::GetDivideOperation(type_precision_id, kDouble)
          ->makeUncheckedBinaryOperator(sum_type, TypeFactory::GetType(kDouble)));

  // Result is nullable, because AVG() over 0 values (or all NULL values) is
  // NULL.
  result_type_ =
      &OperationFactory::GetDivideOperation(type_precision_id, kDouble)
          ->getResultType(sum_type, TypeFactory::GetType(kDouble))
              ->getNullableVersion();
}

AggregationState* AggregationHandleAvg::accumulateValueAccessor(
    const std::vector<MultiSourceAttributeId> &argument_ids,
    const ValueAccessorMultiplexer &accessor_mux) const {
  DCHECK_EQ(1u, argument_ids.size())
      << "Got wrong number of attributes for AVG: " << argument_ids.size();

  const ValueAccessorSource argument_source = argument_ids.front().source;
  const attribute_id argument_id = argument_ids.front().attr_id;

  DCHECK(argument_source != ValueAccessorSource::kInvalid);
  DCHECK_NE(argument_id, kInvalidAttributeID);

  AggregationStateAvg *state = new AggregationStateAvg(blank_state_);
  std::size_t count = 0;
  state->sum_ =
      fast_add_operator_->accumulateValueAccessor(
          state->sum_,
          accessor_mux.getValueAccessorBySource(argument_source),
          argument_id,
          &count);
  state->count_ = count;
  return state;
}

void AggregationHandleAvg::mergeStates(const AggregationState &source,
                                       AggregationState *destination) const {
  const AggregationStateAvg &avg_source =
      static_cast<const AggregationStateAvg &>(source);
  AggregationStateAvg *avg_destination =
      static_cast<AggregationStateAvg *>(destination);

  SpinMutexLock lock(avg_destination->mutex_);
  avg_destination->count_ += avg_source.count_;
  avg_destination->sum_ = merge_add_operator_->applyToTypedValues(
      avg_destination->sum_, avg_source.sum_);
}

void AggregationHandleAvg::mergeStates(const std::uint8_t *source,
                                       std::uint8_t *destination) const {
  const TypedValue *src_sum_ptr =
      reinterpret_cast<const TypedValue *>(source + blank_state_.sum_offset_);
  const std::int64_t *src_count_ptr = reinterpret_cast<const std::int64_t *>(
      source + blank_state_.count_offset_);
  TypedValue *dst_sum_ptr =
      reinterpret_cast<TypedValue *>(destination + blank_state_.sum_offset_);
  std::int64_t *dst_count_ptr = reinterpret_cast<std::int64_t *>(
      destination + blank_state_.count_offset_);
  (*dst_count_ptr) += (*src_count_ptr);
  *dst_sum_ptr =
      merge_add_operator_->applyToTypedValues(*dst_sum_ptr, *src_sum_ptr);
}

TypedValue AggregationHandleAvg::finalize(const AggregationState &state) const {
  const AggregationStateAvg &agg_state =
      static_cast<const AggregationStateAvg &>(state);
  if (agg_state.count_ == 0) {
    // AVG() over no values is NULL.
    return result_type_->makeNullValue();
  } else {
    // Divide sum by count to get final average.
    return divide_operator_->applyToTypedValues(
        agg_state.sum_, TypedValue(static_cast<double>(agg_state.count_)));
  }
}

ColumnVector* AggregationHandleAvg::finalizeHashTable(
    const AggregationStateHashTableBase &hash_table,
    const std::size_t index,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  return finalizeHashTableHelper<AggregationHandleAvg>(
      *result_type_, hash_table, index, group_by_keys);
}

AggregationState* AggregationHandleAvg::aggregateOnDistinctifyHashTableForSingle(
    const AggregationStateHashTableBase &distinctify_hash_table) const {
  return aggregateOnDistinctifyHashTableForSingleUnaryHelper<
      AggregationHandleAvg, AggregationStateAvg>(
          distinctify_hash_table);
}

void AggregationHandleAvg::aggregateOnDistinctifyHashTableForGroupBy(
    const AggregationStateHashTableBase &distinctify_hash_table,
    const std::size_t index,
    AggregationStateHashTableBase *aggregation_hash_table) const {
  aggregateOnDistinctifyHashTableForGroupByUnaryHelper<AggregationHandleAvg>(
      distinctify_hash_table, index, aggregation_hash_table);
}

}  // namespace quickstep
