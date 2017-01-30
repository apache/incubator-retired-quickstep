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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_SUM_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_SUM_HPP_

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
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class AggregationStateHashTableBase;
class ColumnVector;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Aggregation state for sum.
 */
class AggregationStateSum : public AggregationState {
 public:
  /**
   * @brief Copy constructor (ignores mutex).
   */
  AggregationStateSum(const AggregationStateSum &orig)
      : sum_(orig.sum_),
        null_(orig.null_),
        sum_offset_(orig.sum_offset_),
        null_offset_(orig.null_offset_) {}

  std::size_t getPayloadSize() const {
    std::size_t p1 = reinterpret_cast<std::size_t>(&sum_);
    std::size_t p2 = reinterpret_cast<std::size_t>(&mutex_);
    return (p2 - p1);
  }

  const std::uint8_t* getPayloadAddress() const {
    return reinterpret_cast<const uint8_t *>(&sum_);
  }

 private:
  friend class AggregationHandleSum;

  AggregationStateSum()
      : sum_(0),
        null_(true),
        sum_offset_(0),
        null_offset_(reinterpret_cast<std::uint8_t *>(&null_) -
                     reinterpret_cast<std::uint8_t *>(&sum_)) {}

  AggregationStateSum(TypedValue &&sum, const bool is_null)
      : sum_(std::move(sum)), null_(is_null) {}

  // TODO(shoban): We might want to specialize sum_ to use atomics for int types
  // similar to in AggregationStateCount.
  TypedValue sum_;
  bool null_;
  SpinMutex mutex_;

  int sum_offset_, null_offset_;
};

/**
 * @brief An aggregationhandle for sum.
 **/
class AggregationHandleSum : public AggregationConcreteHandle {
 public:
  ~AggregationHandleSum() override {}

  std::vector<const Type *> getArgumentTypes() const override {
    return {&argument_type_};
  }

  const Type* getResultType() const override {
    return result_type_;
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateSum(blank_state_);
  }

  inline void iterateUnaryInl(AggregationStateSum *state,
                              const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(argument_type_.getSignature()));
    if (value.isNull()) return;

    SpinMutexLock lock(state->mutex_);
    state->sum_ = fast_operator_->applyToTypedValues(state->sum_, value);
    state->null_ = false;
  }

  inline void iterateUnaryInl(const TypedValue &value,
                              std::uint8_t *byte_ptr) const {
    DCHECK(value.isPlausibleInstanceOf(argument_type_.getSignature()));
    if (value.isNull()) return;
    TypedValue *sum_ptr =
        reinterpret_cast<TypedValue *>(byte_ptr + blank_state_.sum_offset_);
    bool *null_ptr =
        reinterpret_cast<bool *>(byte_ptr + blank_state_.null_offset_);
    *sum_ptr = fast_operator_->applyToTypedValues(*sum_ptr, value);
    *null_ptr = false;
  }

  AggregationState* accumulateValueAccessor(
      const std::vector<MultiSourceAttributeId> &argument_ids,
      const ValueAccessorMultiplexer &accessor_mux) const override;

  void mergeStates(const AggregationState &source,
                   AggregationState *destination) const override;

  TypedValue finalize(const AggregationState &state) const override;

  std::size_t getPayloadSize() const override {
    return blank_state_.getPayloadSize();
  }

  void initPayload(std::uint8_t *byte_ptr) const override {
    TypedValue *sum_ptr =
        reinterpret_cast<TypedValue *>(byte_ptr + blank_state_.sum_offset_);
    bool *null_ptr =
        reinterpret_cast<bool *>(byte_ptr + blank_state_.null_offset_);
    *sum_ptr = blank_state_.sum_;
    *null_ptr = true;
  }

  void destroyPayload(std::uint8_t *byte_ptr) const override {
    TypedValue *sum_ptr =
        reinterpret_cast<TypedValue *>(byte_ptr + blank_state_.sum_offset_);
    if (sum_ptr != nullptr) {
      sum_ptr->~TypedValue();
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

  inline TypedValue finalizeHashTableEntry(const std::uint8_t *byte_ptr) const {
    return *reinterpret_cast<const TypedValue *>(byte_ptr + blank_state_.sum_offset_);
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
  friend class AggregateFunctionSum;

  /**
   * @brief Initialize handle for type.
   *
   * @param type Type of the sum value.
   **/
  explicit AggregationHandleSum(const Type &type);

  const Type &argument_type_;
  const Type *result_type_;
  AggregationStateSum blank_state_;
  std::unique_ptr<UncheckedBinaryOperator> fast_operator_;
  std::unique_ptr<UncheckedBinaryOperator> merge_operator_;

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleSum);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_SUM_HPP_
