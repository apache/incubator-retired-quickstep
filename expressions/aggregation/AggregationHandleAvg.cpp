/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "expressions/aggregation/AggregationHandleAvg.hpp"

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/HashTable.hpp"
#include "storage/HashTableFactory.hpp"
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

class StorageManager;

AggregationHandleAvg::AggregationHandleAvg(const Type &type)
    : argument_type_(type) {
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
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
          .makeUncheckedBinaryOperatorForTypes(sum_type, argument_type_));
  // Add operator for merging states.
  merge_add_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
          .makeUncheckedBinaryOperatorForTypes(sum_type, sum_type));
  // Divide operator for dividing sum by count to get final average.
  divide_operator_.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide)
          .makeUncheckedBinaryOperatorForTypes(sum_type, TypeFactory::GetType(kDouble)));

  // Result is nullable, because AVG() over 0 values (or all NULL values) is
  // NULL.
  result_type_
      = &(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide)
              .resultTypeForArgumentTypes(sum_type, TypeFactory::GetType(kDouble))
                  ->getNullableVersion());
}

AggregationStateHashTableBase* AggregationHandleAvg::createGroupByHashTable(
    const HashTableImplType hash_table_impl,
    const std::vector<const Type*> &group_by_types,
    const std::size_t estimated_num_groups,
    StorageManager *storage_manager) const {
  return AggregationStateHashTableFactory<AggregationStateAvg>::CreateResizable(
      hash_table_impl,
      group_by_types,
      estimated_num_groups,
      storage_manager);
}

AggregationState* AggregationHandleAvg::accumulateColumnVectors(
    const std::vector<std::unique_ptr<ColumnVector>> &column_vectors) const {
  DCHECK_EQ(1u, column_vectors.size())
      << "Got wrong number of ColumnVectors for AVG: " << column_vectors.size();

  AggregationStateAvg *state = new AggregationStateAvg(blank_state_);
  std::size_t count = 0;
  state->sum_ = fast_add_operator_->accumulateColumnVector(state->sum_,
                                                           *column_vectors.front(),
                                                           &count);
  state->count_ = count;
  return state;
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
AggregationState* AggregationHandleAvg::accumulateValueAccessor(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &accessor_ids) const {
  DCHECK_EQ(1u, accessor_ids.size())
      << "Got wrong number of attributes for AVG: " << accessor_ids.size();

  AggregationStateAvg *state = new AggregationStateAvg(blank_state_);
  std::size_t count = 0;
  state->sum_ = fast_add_operator_->accumulateValueAccessor(state->sum_,
                                                            accessor,
                                                            accessor_ids.front(),
                                                            &count);
  state->count_ = count;
  return state;
}
#endif

void AggregationHandleAvg::aggregateValueAccessorIntoHashTable(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &argument_ids,
    const std::vector<attribute_id> &group_by_key_ids,
    AggregationStateHashTableBase *hash_table) const {
  DCHECK_EQ(1u, argument_ids.size())
      << "Got wrong number of arguments for AVG: " << argument_ids.size();

  aggregateValueAccessorIntoHashTableUnaryHelper<
      AggregationHandleAvg,
      AggregationStateAvg,
      AggregationStateHashTable<AggregationStateAvg>>(
          accessor,
          argument_ids.front(),
          group_by_key_ids,
          blank_state_,
          hash_table);
}

void AggregationHandleAvg::mergeStates(
    const AggregationState &source,
    AggregationState *destination) const {
  const AggregationStateAvg &avg_source = static_cast<const AggregationStateAvg&>(source);
  AggregationStateAvg *avg_destination = static_cast<AggregationStateAvg*>(destination);

  SpinMutexLock lock(avg_destination->mutex_);
  avg_destination->count_ += avg_source.count_;
  avg_destination->sum_ = merge_add_operator_->applyToTypedValues(avg_destination->sum_,
                                                                  avg_source.sum_);
}

TypedValue AggregationHandleAvg::finalize(const AggregationState &state) const {
  const AggregationStateAvg &agg_state = static_cast<const AggregationStateAvg&>(state);
  if (agg_state.count_ == 0) {
    // AVG() over no values is NULL.
    return result_type_->makeNullValue();
  } else {
    // Divide sum by count to get final average.
    return divide_operator_->applyToTypedValues(agg_state.sum_,
                                                TypedValue(static_cast<double>(agg_state.count_)));
  }
}

ColumnVector* AggregationHandleAvg::finalizeHashTable(
    const AggregationStateHashTableBase &hash_table,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  return finalizeHashTableHelper<AggregationHandleAvg,
                                 AggregationStateHashTable<AggregationStateAvg>>(
      *result_type_,
      hash_table,
      group_by_keys);
}

AggregationState* AggregationHandleAvg::aggregateOnDistinctifyHashTableForSingle(
    const AggregationStateHashTableBase &distinctify_hash_table) const {
  return aggregateOnDistinctifyHashTableForSingleUnaryHelper<
      AggregationHandleAvg,
      AggregationStateAvg>(
          distinctify_hash_table);
}

void AggregationHandleAvg::aggregateOnDistinctifyHashTableForGroupBy(
    const AggregationStateHashTableBase &distinctify_hash_table,
    AggregationStateHashTableBase *aggregation_hash_table) const {
  aggregateOnDistinctifyHashTableForGroupByUnaryHelper<
      AggregationHandleAvg,
      AggregationStateAvg,
      AggregationStateHashTable<AggregationStateAvg>>(
          distinctify_hash_table,
          blank_state_,
          aggregation_hash_table);
}

void AggregationHandleAvg::mergeGroupByHashTables(
    const AggregationStateHashTableBase &source_hash_table,
    AggregationStateHashTableBase *destination_hash_table) const {
  mergeGroupByHashTablesHelper<AggregationHandleAvg,
                               AggregationStateAvg,
                               AggregationStateHashTable<AggregationStateAvg>>(
      source_hash_table, destination_hash_table);
}

}  // namespace quickstep
