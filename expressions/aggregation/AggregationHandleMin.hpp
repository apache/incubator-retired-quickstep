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
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "expressions/aggregation/AggregationConcreteHandle.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "threading/SpinMutex.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class AggregationStateHashTableBase;
class ColumnVector;

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

  std::vector<const Type *> getArgumentTypes() const override {
    return {&type_};
  }

  const Type* getResultType() const override {
    return &type_;
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateMin(type_);
  }

  inline void iterateUnaryInl(AggregationStateMin *state,
                              const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    compareAndUpdate(state, value);
  }

  inline void iterateUnaryInl(const TypedValue &value,
                              std::uint8_t *byte_ptr) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    TypedValue *min_ptr = reinterpret_cast<TypedValue *>(byte_ptr);
    compareAndUpdate(min_ptr, value);
  }

  AggregationState* accumulateValueAccessor(
      const std::vector<MultiSourceAttributeId> &argument_ids,
      const ValueAccessorMultiplexer &accessor_mux) const override;

  void mergeStates(const AggregationState &source,
                   AggregationState *destination) const override;

  TypedValue finalize(const AggregationState &state) const override {
    return static_cast<const AggregationStateMin &>(state).min_;
  }

  std::size_t getPayloadSize() const override {
    return sizeof(TypedValue);
  }

  void initPayload(std::uint8_t *byte_ptr) const override {
    TypedValue *min_ptr = reinterpret_cast<TypedValue *>(byte_ptr);
    TypedValue t1 = (type_.getNullableVersion().makeNullValue());
    *min_ptr = t1;
  }

  void destroyPayload(std::uint8_t *byte_ptr) const override {
    TypedValue *min_ptr = reinterpret_cast<TypedValue *>(byte_ptr);
    if (min_ptr != nullptr) {
      min_ptr->~TypedValue();
    }
  }

  inline void updateStateUnary(const TypedValue &argument,
                               std::uint8_t *byte_ptr) const override {
    if (!block_update_) {
      iterateUnaryInl(argument, byte_ptr);
    }
  }

  void mergeStates(const std::uint8_t *source,
                   std::uint8_t *destination) const override;

  inline TypedValue finalizeHashTableEntry(
      const std::uint8_t *byte_ptr) const {
    return *reinterpret_cast<const TypedValue *>(byte_ptr);
  }

  ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      const std::size_t index,
      std::vector<std::vector<TypedValue>> *group_by_keys) const override;

  AggregationState* aggregateOnDistinctifyHashTableForSingle(
      const AggregationStateHashTableBase &distinctify_hash_table) const override;

  void aggregateOnDistinctifyHashTableForGroupBy(
      const AggregationStateHashTableBase &distinctify_hash_table,
      const std::size_t index,
      AggregationStateHashTableBase *aggregation_hash_table) const override;

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

  inline void compareAndUpdate(TypedValue *min_ptr,
                               const TypedValue &value) const {
    if (value.isNull()) return;
    if (min_ptr->isNull() ||
        fast_comparator_->compareTypedValues(value, *min_ptr)) {
      *min_ptr = value;
    }
  }

  const Type &type_;
  std::unique_ptr<UncheckedComparator> fast_comparator_;

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleMin);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MIN_HPP_
