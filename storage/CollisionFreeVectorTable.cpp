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
#include <type_traits>
#include <vector>

#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "threading/SpinMutex.hpp"
#include "types/TypeID.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/BoolVector.hpp"

#include "glog/logging.h"

namespace quickstep {

DEFINE_uint64(vt_threadprivate_threshold, 1000000L, "");
DEFINE_bool(use_latch, false, "");

namespace {

template <typename T>
using remove_const_reference_t = std::remove_const_t<std::remove_reference_t<T>>;

template <typename FunctorT>
inline auto InvokeOnKeyType(const Type &type,
                            const FunctorT &functor) {
  switch (type.getTypeID()) {
    case TypeID::kInt:
      return functor(static_cast<const IntType&>(type));
    case TypeID::kLong:
      return functor(static_cast<const LongType&>(type));
    default:
      LOG(FATAL) << "Not supported";
  }
}

template <typename FunctorT>
inline auto InvokeOnType(const Type &type,
                         const FunctorT &functor) {
  switch (type.getTypeID()) {
    case TypeID::kInt:
      return functor(static_cast<const IntType&>(type));
    case TypeID::kLong:
      return functor(static_cast<const LongType&>(type));
    case TypeID::kFloat:
      return functor(static_cast<const FloatType&>(type));
    case TypeID::kDouble:
      return functor(static_cast<const DoubleType&>(type));
    default:
      LOG(FATAL) << "Not supported";
  }
}

template <typename FunctorT>
inline auto InvokeOnBool(const bool &val,
                         const FunctorT &functor) {
  if (val) {
    return functor(std::true_type());
  } else {
    return functor(std::false_type());
  }
}

template <typename FunctorT>
inline auto InvokeOnBools(const bool &val1,
                          const bool &val2,
                          const FunctorT &functor) {
  if (val1) {
    if (val2) {
      return functor(std::true_type(), std::true_type());
    } else {
      return functor(std::true_type(), std::false_type());
    }
  } else {
    if (val2) {
      return functor(std::false_type(), std::true_type());
    } else {
      return functor(std::false_type(), std::false_type());
    }
  }
}

template <typename FunctorT>
inline auto InvokeOnAggFunc(const AggregationID &agg_id,
                            const FunctorT &functor) {
  switch (agg_id) {
    case AggregationID::kSum: {
      return functor(Sum());
    }
    default:
      LOG(FATAL) << "Not supported";
  }
}

template <typename FunctorT>
inline auto InvokeIf(const std::true_type &val,
                     const FunctorT &functor) {
  return functor();
}

template <typename FunctorT>
inline void InvokeIf(const std::false_type &val,
                     const FunctorT &functor) {
}

//template <typename FunctorT>
//inline void InvokeOnAggFuncIfApplicableToArgType(
//    const AggregationID &agg_id,
//    const Type &arg_type,
//    const FunctorT &functor) {
//  InvokeOnAggFunc(
//      agg_id,
//      [&](const auto &agg_func) -> void {
//    InvokeOnType(
//        arg_type,
//        [&](const auto &arg_type) -> void {
//      using AggFuncT = std::remove_reference_t<decltype(agg_func)>;
//      using ArgT = remove_const_reference_t<decltype(arg_type)>;
//
//      InvokeIf(
//          typename AggFuncT::template HasAtomicImpl<ArgT>(),
//          [&]() -> void {
//        functor(agg_func, arg_type);
//      });
//    });
//  });
//}

template <typename FunctorT>
inline void InvokeOnAggFuncWithArgType(
    const AggregationID &agg_id,
    const Type &arg_type,
    const FunctorT &functor) {
  InvokeOnAggFunc(
      agg_id,
      [&](const auto &agg_func) -> void {
    InvokeOnType(
        arg_type,
        [&](const auto &arg_type) -> void {
      functor(agg_func, arg_type);
    });
  });
}

template <typename FunctorT>
inline auto InvokeOnTwoAccessors(
    const ValueAccessorMultiplexer &accessor_mux,
    const ValueAccessorSource &first_source,
    const ValueAccessorSource &second_source,
    const FunctorT &functor) {
  ValueAccessor *base_accessor = accessor_mux.getBaseAccessor();
  ColumnVectorsValueAccessor *derived_accessor =
      static_cast<ColumnVectorsValueAccessor *>(accessor_mux.getDerivedAccessor());

  InvokeOnAnyValueAccessor(
      base_accessor,
      [&](auto *accessor) {
    if (first_source == ValueAccessorSource::kBase) {
      if (second_source == ValueAccessorSource::kBase) {
        return functor(std::false_type(), accessor, accessor);
      } else {
        return functor(std::true_type(), accessor, derived_accessor);
      }
    } else {
      if (second_source == ValueAccessorSource::kBase) {
        return functor(std::true_type(), derived_accessor, accessor);
      } else {
        return functor(std::false_type(), derived_accessor, derived_accessor);
      }
    }
  });
}

}  // namespace

CollisionFreeVectorTable::CollisionFreeVectorTable(
    const Type *key_type,
    const std::size_t num_entries,
    const std::vector<AggregationHandle *> &handles,
    StorageManager *storage_manager)
    : key_type_(key_type),
      num_entries_(num_entries),
      num_handles_(handles.size()),
      handles_(handles),
      use_thread_private_existence_map_(num_entries_ < FLAGS_vt_threadprivate_threshold),
      num_finalize_partitions_(CalculateNumFinalizationPartitions(num_entries_)),
      storage_manager_(storage_manager) {
  DCHECK_GT(num_entries, 0u);

  std::size_t required_memory = 0;
  const std::size_t existence_map_offset = 0;
  std::size_t mutex_vec_offset = 0;
  std::vector<std::size_t> state_offsets;

  if (!use_thread_private_existence_map_) {
    required_memory += CacheLineAlignedBytes(
        BarrieredReadWriteConcurrentBoolVector::BytesNeeded(num_entries));
  }

  if (FLAGS_use_latch) {
    mutex_vec_offset = required_memory;
    required_memory += CacheLineAlignedBytes(num_entries * sizeof(SpinMutex));
  }

  for (std::size_t i = 0; i < num_handles_; ++i) {
    const AggregationHandle *handle = handles_[i];
    const std::vector<const Type *> argument_types = handle->getArgumentTypes();
    DCHECK_EQ(1u, argument_types.size());

    std::size_t state_size = 0;
    InvokeOnAggFuncWithArgType(
        handle->getAggregationID(),
        *argument_types.front(),
        [&](const auto &agg_func, const auto &arg_type) {
      using AggFuncT = std::remove_reference_t<decltype(agg_func)>;
      using ArgT = remove_const_reference_t<decltype(arg_type)>;

      if (FLAGS_use_latch) {
        state_size = sizeof(typename AggFuncT::template AggState<ArgT>::T);
      } else {
        state_size = sizeof(typename AggFuncT::template AggState<ArgT>::AtomicT);
      }
    });

    state_offsets.emplace_back(required_memory);
    required_memory += CacheLineAlignedBytes(state_size * num_entries);
  }

  const std::size_t num_storage_slots =
      storage_manager_->SlotsNeededForBytes(required_memory);

  const block_id blob_id = storage_manager_->createBlob(num_storage_slots);
  blob_ = storage_manager_->getBlobMutable(blob_id);

  void *memory_start = blob_->getMemoryMutable();
  if (use_thread_private_existence_map_) {
    thread_private_existence_map_pool_.reset(new BoolVectorPool(num_entries));
  } else {
    concurrent_existence_map_.reset(new BarrieredReadWriteConcurrentBoolVector(
        reinterpret_cast<char *>(memory_start) + existence_map_offset,
        num_entries,
        false /* initialize */));
  }

  if (FLAGS_use_latch) {
    mutex_vec_ = reinterpret_cast<SpinMutex *>(
        reinterpret_cast<char *>(memory_start) + mutex_vec_offset);
  } else {
    mutex_vec_ = nullptr;
  }

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
    LOG(FATAL) << "Not implemented";
  }

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
//      argument_source = ValueAccessorSource::kInvalid;
//      argument_id = kInvalidAttributeID;
//
//      DCHECK(argument_types.empty());
//      argument_type = nullptr;
//      is_argument_nullable = false;
      LOG(FATAL) << "Not supported";
    } else {
      DCHECK_EQ(1u, argument_ids_i.size());
      argument_source = argument_ids_i.front().source;
      argument_id = argument_ids_i.front().attr_id;

      DCHECK_EQ(1u, argument_types.size());
      argument_type = argument_types.front();
      is_argument_nullable = argument_type->isNullable();
    }

    InvokeOnAggFuncWithArgType(
        handle->getAggregationID(),
        *argument_types.front(),
        [&](const auto &agg_func, const auto &arg_type) {
      using AggFuncT = std::remove_reference_t<decltype(agg_func)>;
      using ArgT = remove_const_reference_t<decltype(arg_type)>;

      InvokeOnKeyType(
          *key_type_,
          [&](const auto &key_type) -> void {
        using KeyT = remove_const_reference_t<decltype(key_type)>;

        InvokeOnBools(
            is_key_nullable,
            is_argument_nullable,
            [&](const auto &is_key_nullable,
                const auto &is_argument_nullable) -> void {
          using KeyNullableT =
              remove_const_reference_t<decltype(is_key_nullable)>;
          using ArgNullableT =
              remove_const_reference_t<decltype(is_argument_nullable)>;

          InvokeOnTwoAccessors(
              accessor_mux,
              key_source,
              argument_source,
              [&](const auto &use_two_accessors,
                  auto *key_accessor,
                  auto *argument_accessor) {
            using UseTwoAccessorsT =
                remove_const_reference_t<decltype(use_two_accessors)>;

            invokeOnExistenceMap(
                [&](auto *existence_map) -> void {
              if (FLAGS_use_latch) {
                upsertValueAccessorInternalUnaryLatch<
                    AggFuncT, KeyT, ArgT,
                    KeyNullableT::value, ArgNullableT::value, UseTwoAccessorsT::value>(                                                                 key_id,
                        argument_id,
                        vec_tables_[i],
                        existence_map,
                        key_accessor,
                        argument_accessor);
              } else {
                upsertValueAccessorInternalUnaryAtomic<
                    AggFuncT, KeyT, ArgT,
                    KeyNullableT::value, ArgNullableT::value, UseTwoAccessorsT::value>(                                                                 key_id,
                        argument_id,
                        vec_tables_[i],
                        existence_map,
                        key_accessor,
                        argument_accessor);
              }
            });
          });
        });
      });
    });
  }

  return true;
}

void CollisionFreeVectorTable::finalizeKey(const std::size_t partition_id,
                                           NativeColumnVector *output_cv) const {
  const std::size_t start_position =
      calculatePartitionStartPosition(partition_id);
  const std::size_t end_position =
      calculatePartitionEndPosition(partition_id);

  InvokeOnKeyType(
      *key_type_,
      [&](const auto &key_type) {
    using KeyT = remove_const_reference_t<decltype(key_type)>;

    invokeOnExistenceMapFinal(
        [&](const auto *existence_map) -> void {
      finalizeKeyInternal<typename KeyT::cpptype>(
          start_position, end_position, existence_map, output_cv);
    });
  });
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

  DCHECK(argument_type != nullptr);

  InvokeOnAggFuncWithArgType(
      handle->getAggregationID(),
      *argument_type,
      [&](const auto &agg_func, const auto &arg_type) {
    using AggFuncT = std::remove_reference_t<decltype(agg_func)>;
    using ArgT = remove_const_reference_t<decltype(arg_type)>;

    invokeOnExistenceMapFinal(
        [&](const auto *existence_map) -> void {
      if (FLAGS_use_latch) {
        finalizeStateInternalLatch<AggFuncT, ArgT>(start_position,
                                                   end_position,
                                                   vec_tables_[handle_id],
                                                   existence_map,
                                                   output_cv);
      } else {
        finalizeStateInternalAtomic<AggFuncT, ArgT>(start_position,
                                                    end_position,
                                                    vec_tables_[handle_id],
                                                    existence_map,
                                                    output_cv);
      }
    });
  });
}

}  // namespace quickstep
