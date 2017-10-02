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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_AVG_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_AVG_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
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
 * @brief Aggregation state for average.
 */
class AggregationStateAvg : public AggregationState {
 public:
  /**
   * @brief Copy constructor (ignores mutex).
   */
  AggregationStateAvg(const AggregationStateAvg &orig)
      : sum_(orig.sum_),
        count_(orig.count_),
        sum_offset_(orig.sum_offset_),
        count_offset_(orig.count_offset_),
        mutex_offset_(orig.mutex_offset_) {}

  /**
   * @brief Destructor.
   */
  ~AggregationStateAvg() override {}

  std::size_t getPayloadSize() const {
    std::size_t p1 = reinterpret_cast<std::size_t>(&sum_);
    std::size_t p2 = reinterpret_cast<std::size_t>(&mutex_);
    return (p2 - p1);
  }

  const std::uint8_t *getPayloadAddress() const {
    return reinterpret_cast<const uint8_t *>(&sum_);
  }

 private:
  friend class AggregationHandleAvg;

  AggregationStateAvg()
      : sum_(0),
        count_(0),
        sum_offset_(0),
        count_offset_(reinterpret_cast<std::uint8_t *>(&count_) -
                      reinterpret_cast<std::uint8_t *>(&sum_)),
        mutex_offset_(reinterpret_cast<std::uint8_t *>(&mutex_) -
                      reinterpret_cast<std::uint8_t *>(&sum_)) {}

  // TODO(shoban): We might want to specialize sum_ and count_ to use atomics
  // for int types similar to in AggregationStateCount.
  TypedValue sum_;
  std::int64_t count_;
  SpinMutex mutex_;

  int sum_offset_, count_offset_, mutex_offset_;
};

/**
 * @brief An aggregationhandle for avg.
 **/
class AggregationHandleAvg : public AggregationConcreteHandle {
 public:
  ~AggregationHandleAvg() override {}

  std::vector<const Type *> getArgumentTypes() const override {
    return {&argument_type_};
  }

  const Type* getResultType() const override {
    return result_type_;
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateAvg(blank_state_);
  }

  inline void iterateUnaryInl(AggregationStateAvg *state,
                              const TypedValue &value) const {
//    DCHECK(value.isPlausibleInstanceOf(argument_type_.getSignature()));
    // TODO(refactor-type): fix signature.
    if (value.isNull()) return;

    SpinMutexLock lock(state->mutex_);
    state->sum_ = fast_add_operator_->applyToTypedValues(state->sum_, value);
    ++state->count_;
  }

  inline void iterateUnaryInl(const TypedValue &value,
                              std::uint8_t *byte_ptr) const {
//    DCHECK(value.isPlausibleInstanceOf(argument_type_.getSignature()));
   // TODO(refactor-type): fix signature.
   if (value.isNull()) return;
    TypedValue *sum_ptr =
        reinterpret_cast<TypedValue *>(byte_ptr + blank_state_.sum_offset_);
    std::int64_t *count_ptr =
        reinterpret_cast<std::int64_t *>(byte_ptr + blank_state_.count_offset_);
    *sum_ptr = fast_add_operator_->applyToTypedValues(*sum_ptr, value);
    ++(*count_ptr);
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
    std::int64_t *count_ptr =
        reinterpret_cast<std::int64_t *>(byte_ptr + blank_state_.count_offset_);
    *sum_ptr = blank_state_.sum_;
    *count_ptr = blank_state_.count_;
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

  inline TypedValue finalizeHashTableEntry(
      const std::uint8_t *byte_ptr) const {
    std::uint8_t *value_ptr = const_cast<std::uint8_t *>(byte_ptr);
    TypedValue *sum_ptr =
        reinterpret_cast<TypedValue *>(value_ptr + blank_state_.sum_offset_);
    std::int64_t *count_ptr = reinterpret_cast<std::int64_t *>(
        value_ptr + blank_state_.count_offset_);
    return divide_operator_->applyToTypedValues(
        *sum_ptr, TypedValue(static_cast<double>(*count_ptr)));
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
  friend class AggregateFunctionAvg;

  /**
   * @brief Constructor.
   *
   * @param type Type of the avg value.
   **/
  explicit AggregationHandleAvg(const Type &type);

  const Type &argument_type_;
  const Type *result_type_;
  AggregationStateAvg blank_state_;
  std::unique_ptr<UncheckedBinaryOperator> fast_add_operator_;
  std::unique_ptr<UncheckedBinaryOperator> merge_add_operator_;
  std::unique_ptr<UncheckedBinaryOperator> divide_operator_;

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleAvg);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_AVG_HPP_
