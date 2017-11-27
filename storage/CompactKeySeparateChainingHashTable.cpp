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

#include "storage/CompactKeySeparateChainingHashTable.hpp"

#include <cstddef>
#include <memory>
#include <vector>
#include <type_traits>

#include "cli/Flags.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "types/CharType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/PrimeNumber.hpp"
#include "utility/Range.hpp"
#include "utility/ScopedArray.hpp"

#include "glog/logging.h"

namespace quickstep {

class AggregationHandle;

namespace {

template <typename Functor>
inline auto InvokeOnKeySize(const std::size_t key_size,
                            const Functor &functor) {
  switch (key_size) {
    case 0u:
      break;
    case 1u:
      return functor(std::integral_constant<std::size_t, 1u>());
    case 2u:
      return functor(std::integral_constant<std::size_t, 2u>());
    case 3u:
      return functor(std::integral_constant<std::size_t, 3u>());
    case 4u:
      return functor(std::integral_constant<std::size_t, 4u>());
    case 5u:
      return functor(std::integral_constant<std::size_t, 5u>());
    case 6u:
      return functor(std::integral_constant<std::size_t, 6u>());
    case 7u:
      return functor(std::integral_constant<std::size_t, 7u>());
    case 8u:
      return functor(std::integral_constant<std::size_t, 8u>());
    default:
      break;
  }
  LOG(FATAL) << "Not supported";
}

}  // namespace

CompactKeySeparateChainingHashTable::CompactKeySeparateChainingHashTable(
    const std::vector<const Type*> &key_types,
    const std::size_t num_entries,
    const std::vector<AggregationHandle *> &handles,
    StorageManager *storage_manager)
    : key_types_(key_types),
      buckets_allocated_(0) {
  DCHECK_GT(num_entries, 0u);
  if (!handles.empty()) {
    LOG(FATAL) << "CompactKeySeparateChainingHashTable do not "
               << "support aggregate functions yet";
  }

  num_slots_ = get_next_prime_number(num_entries * kHashTableLoadFactor);
  num_key_buckets_ = num_slots_ / kHashTableLoadFactor;

  // TODO: use storage manager for allocating memory.
  slots_.reset(num_slots_);
  key_buckets_.reset(num_key_buckets_);

  std::size_t total_key_size = 0;
  for (const Type *key_type : key_types) {
    CHECK(!key_type->isNullable()) << "Not supported";
    CHECK(!key_type->isVariableLength()) << "Not supported";

    const std::size_t key_size = key_type->maximumByteLength();
    key_sizes_.emplace_back(key_size);
    total_key_size += key_size;
  }

  if (total_key_size > sizeof(KeyCode)) {
    LOG(FATAL) << "Total key size exceeded KeyCode size";
  }

  slots_init_splitter_ = std::make_unique<RangeSplitter>(
      RangeSplitter::CreateWithPartitionLength(
          0, num_slots_, kInitMinPartitionLength, FLAGS_num_workers));

  key_buckets_init_splitter_ = std::make_unique<RangeSplitter>(
      RangeSplitter::CreateWithNumPartitions(
          0, num_key_buckets_, slots_init_splitter_->getNumPartitions()));
}

bool CompactKeySeparateChainingHashTable::upsertValueAccessorCompositeKey(
    const std::vector<std::vector<MultiSourceAttributeId>> &argument_ids,
    const std::vector<MultiSourceAttributeId> &key_attr_ids,
    const ValueAccessorMultiplexer &accessor_mux) {
  DCHECK(argument_ids.empty());
  DCHECK_EQ(key_sizes_.size(), key_attr_ids.size());

  const std::size_t num_tuples =
      accessor_mux.getValueAccessorBySource(
          key_attr_ids.front().source)->getNumTuplesVirtual();

  ScopedArray<KeyCode> key_codes(num_tuples, true);

  std::size_t offset = 0;
  for (std::size_t i = 0; i < key_sizes_.size(); ++i) {
    const MultiSourceAttributeId &key_attr = key_attr_ids[i];

    InvokeOnAnyValueAccessor(
        accessor_mux.getValueAccessorBySource(key_attr.source),
        [&](auto *accessor) -> void {

      InvokeOnKeySize(
          key_sizes_[i],
          [&](auto key_size) -> void {
        this->constructCompactKeyCodeComponent(num_tuples,
                                               offset,
                                               decltype(key_size)::value,
                                               accessor,
                                               key_attr.attr_id,
                                               key_codes.get());
      });
    });

    offset += key_sizes_[i];
  }

  for (std::size_t i = 0; i < num_tuples; ++i) {
    locateBucketInternal(key_codes[i]);
  }

  return true;
}

void CompactKeySeparateChainingHashTable::finalizeKeys(
    const std::size_t partition_id,
    ColumnVectorsValueAccessor *output) const {
  DCHECK(final_splitter_ != nullptr);
  const Range range = final_splitter_->getPartition(partition_id);

  std::size_t offset = 0;
  for (std::size_t i = 0; i < key_sizes_.size(); ++i) {
    std::unique_ptr<NativeColumnVector> cv =
        std::make_unique<NativeColumnVector>(*key_types_[i], range.size());

    InvokeOnKeySize(
        key_sizes_[i],
        [&](auto key_size) -> void {
      const char *key_ptr =
          reinterpret_cast<const char*>(&key_buckets_[range.begin()].key_code) + offset;
      for (std::size_t j = 0; j < range.size(); ++j) {
        std::memcpy(cv->getPtrForDirectWrite(),
                    key_ptr,
                    decltype(key_size)::value);
        key_ptr += kKeyBucketDataSize;
      }
    });

    output->addColumn(ColumnVectorPtr(cv.release()));
    offset += key_sizes_[i];
  }
}

}  // namespace quickstep
