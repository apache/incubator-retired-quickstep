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

#include "storage/CollisionFreeVectorTable.hpp"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>

#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/BarrieredReadWriteConcurrentBitVector.hpp"

#include "glog/logging.h"

namespace quickstep {

CollisionFreeVectorTable::CollisionFreeVectorTable(
    const Type *key_type,
    const std::size_t num_entries,
    const std::size_t num_finalize_partitions,
    const std::vector<AggregationHandle *> &handles,
    StorageManager *storage_manager)
    : key_type_(key_type),
      num_entries_(num_entries),
      num_handles_(handles.size()),
      handles_(handles),
      num_finalize_partitions_(num_finalize_partitions),
      storage_manager_(storage_manager) {
  DCHECK_GT(num_entries, 0u);
  DCHECK_GT(num_finalize_partitions_, 0u);

  std::size_t required_memory = 0;
  const std::size_t existence_map_offset = 0;
  std::vector<std::size_t> state_offsets;

  required_memory += CacheLineAlignedBytes(
      BarrieredReadWriteConcurrentBitVector::BytesNeeded(num_entries));

  for (std::size_t i = 0; i < num_handles_; ++i) {
    const AggregationHandle *handle = handles_[i];
    const std::vector<const Type *> argument_types = handle->getArgumentTypes();

    std::size_t state_size = 0;
    switch (handle->getAggregationID()) {
      case AggregationID::kCount: {
        state_size = sizeof(std::atomic<std::size_t>);
        break;
      }
      case AggregationID::kSum: {
        DCHECK_EQ(1u, argument_types.size());
        switch (argument_types.front()->getTypeID()) {
          case TypeID::kInt:  // Fall through
          case TypeID::kLong:
            state_size = sizeof(std::atomic<std::int64_t>);
            break;
          case TypeID::kFloat:  // Fall through
          case TypeID::kDouble:
            state_size = sizeof(std::atomic<double>);
            break;
          default:
            LOG(FATAL) << "Not implemented";
        }
        break;
      }
      default:
        LOG(FATAL) << "Not implemented";
    }

    state_offsets.emplace_back(required_memory);
    required_memory += CacheLineAlignedBytes(state_size * num_entries);
  }

  const std::size_t num_storage_slots =
      storage_manager_->SlotsNeededForBytes(required_memory);

  const block_id blob_id = storage_manager_->createBlob(num_storage_slots);
  blob_ = storage_manager_->getBlobMutable(blob_id);

  void *memory_start = blob_->getMemoryMutable();
  existence_map_.reset(new BarrieredReadWriteConcurrentBitVector(
      reinterpret_cast<char *>(memory_start) + existence_map_offset,
      num_entries,
      false /* initialize */));

  for (std::size_t i = 0; i < num_handles_; ++i) {
    // Columnwise layout.
    vec_tables_.emplace_back(
        reinterpret_cast<char *>(memory_start) + state_offsets.at(i));
  }

  memory_size_ = required_memory;
  num_init_partitions_ = CalculateNumInitializationPartitions(memory_size_);
}

CollisionFreeVectorTable::~CollisionFreeVectorTable() {
  const block_id blob_id = blob_->getID();
  blob_.release();
  storage_manager_->deleteBlockOrBlobFile(blob_id);
}

void CollisionFreeVectorTable::destroyPayload() {
}

bool CollisionFreeVectorTable::upsertValueAccessorCompositeKey(
    const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
    const std::vector<MultiSourceAttributeId> &key_ids,
    const ValueAccessorMultiplexer &accessor_mux) {
  DCHECK_EQ(1u, key_ids.size());

  if (handles_.empty()) {
    InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
        accessor_mux.getValueAccessorBySource(key_ids.front().source),
        [&key_ids, this](auto *accessor) -> void {  // NOLINT(build/c++11)
      this->upsertValueAccessorKeyOnlyHelper(key_type_->isNullable(),
                                             key_type_,
                                             key_ids.front().attr_id,
                                             accessor);
    });
    return true;
  }

  DCHECK(accessor_mux.getDerivedAccessor() == nullptr ||
         accessor_mux.getDerivedAccessor()->getImplementationType()
             == ValueAccessor::Implementation::kColumnVectors);

  ValueAccessor *base_accessor = accessor_mux.getBaseAccessor();
  ColumnVectorsValueAccessor *derived_accesor =
      static_cast<ColumnVectorsValueAccessor *>(accessor_mux.getDerivedAccessor());

  // Dispatch to specialized implementations to achieve maximum performance.
  InvokeOnValueAccessorMaybeTupleIdSequenceAdapter(
      base_accessor,
      [&argument_ids, &key_ids, &derived_accesor, this](auto *accessor) -> void {  // NOLINT(build/c++11)
    const ValueAccessorSource key_source = key_ids.front().source;
    const attribute_id key_id = key_ids.front().attr_id;
    const bool is_key_nullable = key_type_->isNullable();

    for (std::size_t i = 0; i < num_handles_; ++i) {
      DCHECK_LE(argument_ids[i].size(), 1u);

      const AggregationHandle *handle = handles_[i];
      const auto &argument_types = handle->getArgumentTypes();
      const auto &argument_ids_i = argument_ids[i];

      ValueAccessorSource argument_source;
      attribute_id argument_id;
      const Type *argument_type;
      bool is_argument_nullable;

      if (argument_ids_i.empty()) {
        argument_source = ValueAccessorSource::kInvalid;
        argument_id = kInvalidAttributeID;

        DCHECK(argument_types.empty());
        argument_type = nullptr;
        is_argument_nullable = false;
      } else {
        DCHECK_EQ(1u, argument_ids_i.size());
        argument_source = argument_ids_i.front().source;
        argument_id = argument_ids_i.front().attr_id;

        DCHECK_EQ(1u, argument_types.size());
        argument_type = argument_types.front();
        is_argument_nullable = argument_type->isNullable();
      }

      if (key_source == ValueAccessorSource::kBase) {
        if (argument_source == ValueAccessorSource::kBase) {
          this->upsertValueAccessorDispatchHelper<false>(is_key_nullable,
                                                         is_argument_nullable,
                                                         key_type_,
                                                         argument_type,
                                                         handle->getAggregationID(),
                                                         key_id,
                                                         argument_id,
                                                         vec_tables_[i],
                                                         accessor,
                                                         accessor);
        } else {
          this->upsertValueAccessorDispatchHelper<true>(is_key_nullable,
                                                        is_argument_nullable,
                                                        key_type_,
                                                        argument_type,
                                                        handle->getAggregationID(),
                                                        key_id,
                                                        argument_id,
                                                        vec_tables_[i],
                                                        accessor,
                                                        derived_accesor);
        }
      } else {
        if (argument_source == ValueAccessorSource::kBase) {
          this->upsertValueAccessorDispatchHelper<true>(is_key_nullable,
                                                        is_argument_nullable,
                                                        key_type_,
                                                        argument_type,
                                                        handle->getAggregationID(),
                                                        key_id,
                                                        argument_id,
                                                        vec_tables_[i],
                                                        derived_accesor,
                                                        accessor);
        } else {
          this->upsertValueAccessorDispatchHelper<false>(is_key_nullable,
                                                         is_argument_nullable,
                                                         key_type_,
                                                         argument_type,
                                                         handle->getAggregationID(),
                                                         key_id,
                                                         argument_id,
                                                         vec_tables_[i],
                                                         derived_accesor,
                                                         derived_accesor);
        }
      }
    }
  });
  return true;
}

void CollisionFreeVectorTable::finalizeKey(const std::size_t partition_id,
                                           NativeColumnVector *output_cv) const {
  const std::size_t start_position =
      calculatePartitionStartPosition(partition_id);
  const std::size_t end_position =
      calculatePartitionEndPosition(partition_id);

  switch (key_type_->getTypeID()) {
    case TypeID::kInt:
      finalizeKeyInternal<int>(start_position, end_position, output_cv);
      return;
    case TypeID::kLong:
      finalizeKeyInternal<std::int64_t>(start_position, end_position, output_cv);
      return;
    default:
      LOG(FATAL) << "Not supported";
  }
}

void CollisionFreeVectorTable::finalizeState(const std::size_t partition_id,
                                             const std::size_t handle_id,
                                             NativeColumnVector *output_cv) const {
  const std::size_t start_position =
      calculatePartitionStartPosition(partition_id);
  const std::size_t end_position =
      calculatePartitionEndPosition(partition_id);

  const AggregationHandle *handle = handles_[handle_id];
  const auto &argument_types = handle->getArgumentTypes();
  const Type *argument_type =
      argument_types.empty() ? nullptr : argument_types.front();

  finalizeStateDispatchHelper(handle->getAggregationID(),
                              argument_type,
                              vec_tables_[handle_id],
                              start_position,
                              end_position,
                              output_cv);
}

}  // namespace quickstep
