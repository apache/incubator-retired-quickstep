/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/aggregation/AggregationHandleSum.hpp"

#include <cstddef>
#include <memory>
#include <utility>
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

AggregationHandleSum::AggregationHandleSum(const Type &type)
    : argument_type_(type) {
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
  fast_operator_.reset(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
                       .makeUncheckedBinaryOperatorForTypes(sum_type, argument_type_));
  // Add operator for merging states.
  merge_operator_.reset(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
                        .makeUncheckedBinaryOperatorForTypes(sum_type, sum_type));

  // Result is nullable, because SUM() over 0 values (or all NULL values) is
  // NULL.
  result_type_ = &sum_type.getNullableVersion();
}

AggregationStateHashTableBase* AggregationHandleSum::createGroupByHashTable(
    const HashTableImplType hash_table_impl,
    const std::vector<const Type*> &group_by_types,
    const std::size_t estimated_num_groups,
    StorageManager *storage_manager) const {
  return AggregationStateHashTableFactory<AggregationStateSum>::CreateResizable(
      hash_table_impl,
      group_by_types,
      estimated_num_groups,
      storage_manager);
}

AggregationState* AggregationHandleSum::accumulateColumnVectors(
    const std::vector<std::unique_ptr<ColumnVector>> &column_vectors) const {
  DCHECK_EQ(1u, column_vectors.size())
      << "Got wrong number of ColumnVectors for SUM: " << column_vectors.size();

  std::size_t num_tuples = 0;
  TypedValue cv_sum = fast_operator_->accumulateColumnVector(
      blank_state_.sum_,
      *column_vectors.front(),
      &num_tuples);
  return new AggregationStateSum(std::move(cv_sum), num_tuples == 0);
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
AggregationState* AggregationHandleSum::accumulateValueAccessor(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &accessor_ids) const {
  DCHECK_EQ(1u, accessor_ids.size())
      << "Got wrong number of attributes for SUM: " << accessor_ids.size();

  std::size_t num_tuples = 0;
  TypedValue va_sum = fast_operator_->accumulateValueAccessor(
      blank_state_.sum_,
      accessor,
      accessor_ids.front(),
      &num_tuples);
  return new AggregationStateSum(std::move(va_sum), num_tuples == 0);
}
#endif

void AggregationHandleSum::aggregateValueAccessorIntoHashTable(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &argument_ids,
    const std::vector<attribute_id> &group_by_key_ids,
    AggregationStateHashTableBase *hash_table) const {
  DCHECK_EQ(1u, argument_ids.size())
      << "Got wrong number of arguments for SUM: " << argument_ids.size();

  aggregateValueAccessorIntoHashTableUnaryHelper<
      AggregationHandleSum,
      AggregationStateSum,
      AggregationStateHashTable<AggregationStateSum>>(
          accessor,
          argument_ids.front(),
          group_by_key_ids,
          blank_state_,
          hash_table);
}

void AggregationHandleSum::mergeStates(
    const AggregationState &source,
    AggregationState *destination) const {
  const AggregationStateSum &sum_source = static_cast<const AggregationStateSum&>(source);
  AggregationStateSum *sum_destination = static_cast<AggregationStateSum*>(destination);

  SpinMutexLock(sum_destination->mutex_);
  sum_destination->sum_ = merge_operator_->applyToTypedValues(sum_destination->sum_,
                                                              sum_source.sum_);
  sum_destination->null_ = sum_destination->null_ && sum_source.null_;
}

TypedValue AggregationHandleSum::finalize(const AggregationState &state) const {
  const AggregationStateSum &agg_state = static_cast<const AggregationStateSum&>(state);
  if (agg_state.null_) {
    // SUM() over no values is NULL.
    return result_type_->makeNullValue();
  } else {
    return agg_state.sum_;
  }
}

ColumnVector* AggregationHandleSum::finalizeHashTable(
    const AggregationStateHashTableBase &hash_table,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  return finalizeHashTableHelper<AggregationHandleSum,
                                 AggregationStateHashTable<AggregationStateSum>>(
      *result_type_,
      hash_table,
      group_by_keys);
}

}  // namespace quickstep
