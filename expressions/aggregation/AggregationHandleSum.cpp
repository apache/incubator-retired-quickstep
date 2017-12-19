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

#include "expressions/aggregation/AggregationHandleSum.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "threading/SpinMutex.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;

AggregationHandleSum::AggregationHandleSum(const Type &type)
    : AggregationConcreteHandle(AggregationID::kSum),
      argument_type_(type) {
  // We sum Int as Long and Float as Double so that we have more headroom when
  // adding many values.
  TypeID type_precision_id;
  switch (argument_type_.getTypeID()) {
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

  // Make operators to do arithmetic:
  // Add operator for summing argument values.
  fast_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
          .makeUncheckedBinaryOperatorForTypes(sum_type, argument_type_));
  // Add operator for merging states.
  merge_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
          .makeUncheckedBinaryOperatorForTypes(sum_type, sum_type));

  // Result is nullable, because SUM() over 0 values (or all NULL values) is
  // NULL.
  result_type_ = &sum_type.getNullableVersion();
}

AggregationState* AggregationHandleSum::accumulateValueAccessor(
    const std::vector<MultiSourceAttributeId> &argument_ids,
    const ValueAccessorMultiplexer &accessor_mux) const {
  DCHECK_EQ(1u, argument_ids.size())
      << "Got wrong number of attributes for SUM: " << argument_ids.size();

  const ValueAccessorSource argument_source = argument_ids.front().source;
  const attribute_id argument_id = argument_ids.front().attr_id;

  DCHECK(argument_source != ValueAccessorSource::kInvalid);
  DCHECK_NE(argument_id, kInvalidAttributeID);

  std::size_t num_tuples = 0;
  TypedValue va_sum =
      fast_operator_->accumulateValueAccessor(
          blank_state_.sum_,
          accessor_mux.getValueAccessorBySource(argument_source),
          argument_id,
          &num_tuples);
  return new AggregationStateSum(std::move(va_sum), num_tuples == 0);
}

void AggregationHandleSum::mergeStates(const AggregationState &source,
                                       AggregationState *destination) const {
  const AggregationStateSum &sum_source =
      static_cast<const AggregationStateSum &>(source);
  AggregationStateSum *sum_destination =
      static_cast<AggregationStateSum *>(destination);

  SpinMutexLock lock(sum_destination->mutex_);
  sum_destination->sum_ = merge_operator_->applyToTypedValues(
      sum_destination->sum_, sum_source.sum_);
  sum_destination->null_ = sum_destination->null_ && sum_source.null_;
}

void AggregationHandleSum::mergeStates(const std::uint8_t *source,
                                       std::uint8_t *destination) const {
  const TypedValue *src_sum_ptr =
      reinterpret_cast<const TypedValue *>(source + blank_state_.sum_offset_);
  const bool *src_null_ptr =
      reinterpret_cast<const bool *>(source + blank_state_.null_offset_);
  TypedValue *dst_sum_ptr =
      reinterpret_cast<TypedValue *>(destination + blank_state_.sum_offset_);
  bool *dst_null_ptr =
      reinterpret_cast<bool *>(destination + blank_state_.null_offset_);
  *dst_sum_ptr =
      merge_operator_->applyToTypedValues(*dst_sum_ptr, *src_sum_ptr);
  *dst_null_ptr = (*dst_null_ptr) && (*src_null_ptr);
}

TypedValue AggregationHandleSum::finalize(const AggregationState &state) const {
  const AggregationStateSum &agg_state =
      static_cast<const AggregationStateSum &>(state);
  if (agg_state.null_) {
    // SUM() over no values is NULL.
    return result_type_->makeNullValue();
  } else {
    return agg_state.sum_;
  }
}

ColumnVector* AggregationHandleSum::finalizeHashTable(
    const AggregationStateHashTableBase &hash_table,
    const std::size_t index,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  return finalizeHashTableHelper<AggregationHandleSum>(
      *result_type_, hash_table, index, group_by_keys);
}

AggregationState* AggregationHandleSum::aggregateOnDistinctifyHashTableForSingle(
    const AggregationStateHashTableBase &distinctify_hash_table) const {
  return aggregateOnDistinctifyHashTableForSingleUnaryHelper<
      AggregationHandleSum, AggregationStateSum>(
          distinctify_hash_table);
}

void AggregationHandleSum::aggregateOnDistinctifyHashTableForGroupBy(
    const AggregationStateHashTableBase &distinctify_hash_table,
    const std::size_t index,
    AggregationStateHashTableBase *aggregation_hash_table) const {
  aggregateOnDistinctifyHashTableForGroupByUnaryHelper<AggregationHandleSum>(
      distinctify_hash_table, index, aggregation_hash_table);
}

}  // namespace quickstep
