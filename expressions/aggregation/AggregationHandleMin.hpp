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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MIN_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MIN_HPP_

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationConcreteHandle.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/FastHashTable.hpp"
#include "storage/HashTableBase.hpp"
#include "threading/SpinMutex.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class StorageManager;
class ValueAccessor;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Aggregation state for min.
 */
class AggregationStateMin : public AggregationState {
 public:
  /**
   * @brief Copy constructor (ignores mutex).
   */
  AggregationStateMin(const AggregationStateMin &orig) : min_(orig.min_) {}

  /**
   * @brief Destructor.
   */
  ~AggregationStateMin() override {}

  std::size_t getPayloadSize() const { return sizeof(TypedValue); }

  const std::uint8_t *getPayloadAddress() const {
    return reinterpret_cast<const uint8_t *>(&min_);
  }

 private:
  friend class AggregationHandleMin;

  explicit AggregationStateMin(const Type &type)
      : min_(type.getNullableVersion().makeNullValue()) {}

  explicit AggregationStateMin(TypedValue &&value) : min_(std::move(value)) {}

  TypedValue min_;
  SpinMutex mutex_;
};

/**
 * @brief An aggregationhandle for min.
 **/
class AggregationHandleMin : public AggregationConcreteHandle {
 public:
  ~AggregationHandleMin() override {}

  AggregationState* createInitialState() const override {
    return new AggregationStateMin(type_);
  }

  AggregationStateHashTableBase* createGroupByHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type *> &group_by_types,
      const std::size_t estimated_num_groups,
      StorageManager *storage_manager) const override;

  /**
   * @brief Iterate with min aggregation state.
   */
  inline void iterateUnaryInl(AggregationStateMin *state,
                              const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    compareAndUpdate(state, value);
  }

  inline void iterateUnaryInlFast(const TypedValue &value,
                                  std::uint8_t *byte_ptr) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    TypedValue *min_ptr = reinterpret_cast<TypedValue *>(byte_ptr);
    compareAndUpdateFast(min_ptr, value);
  }

  inline void updateStateUnary(const TypedValue &argument,
                               std::uint8_t *byte_ptr) const override {
    if (!block_update_) {
      iterateUnaryInlFast(argument, byte_ptr);
    }
  }

  void blockUpdate() override { block_update_ = true; }

  void allowUpdate() override { block_update_ = false; }

  void initPayload(std::uint8_t *byte_ptr) const override {
    TypedValue *min_ptr = reinterpret_cast<TypedValue *>(byte_ptr);
    TypedValue t1 = (type_.getNullableVersion().makeNullValue());
    *min_ptr = t1;
  }

  AggregationState* accumulateColumnVectors(
      const std::vector<std::unique_ptr<ColumnVector>> &column_vectors)
      const override;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  AggregationState* accumulateValueAccessor(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &accessor_ids) const override;
#endif

  void aggregateValueAccessorIntoHashTable(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &argument_ids,
      const std::vector<attribute_id> &group_by_key_ids,
      AggregationStateHashTableBase *hash_table) const override;

  void mergeStates(const AggregationState &source,
                   AggregationState *destination) const override;

  void mergeStatesFast(const std::uint8_t *source,
                       std::uint8_t *destination) const override;

  TypedValue finalize(const AggregationState &state) const override {
    return static_cast<const AggregationStateMin &>(state).min_;
  }

  inline TypedValue finalizeHashTableEntry(
      const AggregationState &state) const {
    return static_cast<const AggregationStateMin &>(state).min_;
  }

  inline TypedValue finalizeHashTableEntryFast(
      const std::uint8_t *byte_ptr) const {
    const TypedValue *min_ptr = reinterpret_cast<const TypedValue *>(byte_ptr);
    return TypedValue(*min_ptr);
  }

  ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      std::vector<std::vector<TypedValue>> *group_by_keys,
      int index) const override;

  /**
   * @brief Implementation of
   * AggregationHandle::aggregateOnDistinctifyHashTableForSingle()
   *        for MIN aggregation.
   */
  AggregationState* aggregateOnDistinctifyHashTableForSingle(
      const AggregationStateHashTableBase &distinctify_hash_table)
      const override;

  /**
   * @brief Implementation of
   *        AggregationHandle::aggregateOnDistinctifyHashTableForGroupBy()
   *        for MIN aggregation.
   */
  void aggregateOnDistinctifyHashTableForGroupBy(
      const AggregationStateHashTableBase &distinctify_hash_table,
      AggregationStateHashTableBase *aggregation_hash_table,
      std::size_t index) const override;

  std::size_t getPayloadSize() const override { return sizeof(TypedValue); }

 private:
  friend class AggregateFunctionMin;

  /**
   * @brief Constructor.
   *
   * @param type Type of the data set.
   **/
  explicit AggregationHandleMin(const Type &type);

  /**
   * @brief compare the value with min_ and update it if the value is smaller
   *        than current minimum. NULLs are ignored.
   *
   * @param value A TypedValue to compare.
   **/
  inline void compareAndUpdate(AggregationStateMin *state,
                               const TypedValue &value) const {
    if (value.isNull()) return;

    SpinMutexLock lock(state->mutex_);
    if (state->min_.isNull() ||
        fast_comparator_->compareTypedValues(value, state->min_)) {
      state->min_ = value;
    }
  }

  inline void compareAndUpdateFast(TypedValue *min_ptr,
                                   const TypedValue &value) const {
    if (value.isNull()) return;
    if (min_ptr->isNull() ||
        fast_comparator_->compareTypedValues(value, *min_ptr)) {
      *min_ptr = value;
    }
  }

  const Type &type_;
  std::unique_ptr<UncheckedComparator> fast_comparator_;

  bool block_update_;

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleMin);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MIN_HPP_
