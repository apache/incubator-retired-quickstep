/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include "expressions/aggregation/AggregationHandleMax.hpp"

#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/HashTable.hpp"
#include "storage/HashTableFactory.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "glog/logging.h"

namespace quickstep {

class StorageManager;

AggregationHandleMax::AggregationHandleMax(const Type &type)
    : type_(type) {
  fast_comparator_.reset(ComparisonFactory::GetComparison(ComparisonID::kGreater)
                         .makeUncheckedComparatorForTypes(type,
                                                          type.getNonNullableVersion()));
}

AggregationStateHashTableBase* AggregationHandleMax::createGroupByHashTable(
    const HashTableImplType hash_table_impl,
    const std::vector<const Type*> &group_by_types,
    const std::size_t estimated_num_groups,
    StorageManager *storage_manager) const {
  return AggregationStateHashTableFactory<AggregationStateMax>::CreateResizable(
      hash_table_impl,
      group_by_types,
      estimated_num_groups,
      storage_manager);
}

AggregationState* AggregationHandleMax::accumulateColumnVectors(
    const std::vector<std::unique_ptr<ColumnVector>> &column_vectors) const {
  DCHECK_EQ(1u, column_vectors.size())
      << "Got wrong number of ColumnVectors for MAX: " << column_vectors.size();

  return new AggregationStateMax(
      fast_comparator_->accumulateColumnVector(type_.getNullableVersion().makeNullValue(),
                                               *column_vectors.front()));
}

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
AggregationState* AggregationHandleMax::accumulateValueAccessor(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &accessor_ids) const {
  DCHECK_EQ(1u, accessor_ids.size())
      << "Got wrong number of attributes for MAX: " << accessor_ids.size();

  return new AggregationStateMax(
      fast_comparator_->accumulateValueAccessor(type_.getNullableVersion().makeNullValue(),
                                                accessor,
                                                accessor_ids.front()));
}
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

void AggregationHandleMax::aggregateValueAccessorIntoHashTable(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &argument_ids,
    const std::vector<attribute_id> &group_by_key_ids,
    AggregationStateHashTableBase *hash_table) const {
  DCHECK_EQ(1u, argument_ids.size())
      << "Got wrong number of arguments for MAX: " << argument_ids.size();

  aggregateValueAccessorIntoHashTableUnaryHelper<
      AggregationHandleMax,
      AggregationStateMax,
      AggregationStateHashTable<AggregationStateMax>>(
          accessor,
          argument_ids.front(),
          group_by_key_ids,
          AggregationStateMax(type_),
          hash_table);
}

void AggregationHandleMax::mergeStates(
    const AggregationState &source,
    AggregationState *destination) const {
  const AggregationStateMax &max_source = static_cast<const AggregationStateMax&>(source);
  AggregationStateMax *max_destination = static_cast<AggregationStateMax*>(destination);

  if (!max_source.max_.isNull()) {
    compareAndUpdate(max_destination, max_source.max_);
  }
}

ColumnVector* AggregationHandleMax::finalizeHashTable(
    const AggregationStateHashTableBase &hash_table,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  return finalizeHashTableHelper<AggregationHandleMax,
                                 AggregationStateHashTable<AggregationStateMax>>(
      type_.getNullableVersion(),
      hash_table,
      group_by_keys);
}

AggregationState* AggregationHandleMax::aggregateOnDistinctifyHashTableForSingle(
    const AggregationStateHashTableBase &distinctify_hash_table) const {
  return aggregateOnDistinctifyHashTableForSingleUnaryHelper<
      AggregationHandleMax,
      AggregationStateMax>(
          distinctify_hash_table);
}

void AggregationHandleMax::aggregateOnDistinctifyHashTableForGroupBy(
    const AggregationStateHashTableBase &distinctify_hash_table,
    AggregationStateHashTableBase *aggregation_hash_table) const {
  aggregateOnDistinctifyHashTableForGroupByUnaryHelper<
      AggregationHandleMax,
      AggregationStateMax,
      AggregationStateHashTable<AggregationStateMax>>(
          distinctify_hash_table,
          AggregationStateMax(type_),
          aggregation_hash_table);
}

void AggregationHandleMax::mergeGroupByHashTables(
    const AggregationStateHashTableBase &source_hash_table,
    AggregationStateHashTableBase *destination_hash_table) const {
  mergeGroupByHashTablesHelper<AggregationHandleMax,
                               AggregationStateMax,
                               AggregationStateHashTable<AggregationStateMax>>(
      source_hash_table, destination_hash_table);
}

}  // namespace quickstep
