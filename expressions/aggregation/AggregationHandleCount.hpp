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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_COUNT_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_COUNT_HPP_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "expressions/aggregation/AggregationConcreteHandle.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"
#include "types/LongType.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregationStateHashTableBase;
class ColumnVector;
class Type;

template <bool, bool>
class AggregationHandleCount;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Aggregation state of count.
 */
class AggregationStateCount : public AggregationState {
 public:
  /**
   * @brief Copy constructor.
   */
  AggregationStateCount(const AggregationStateCount &state)
      : count_(state.count_.load(std::memory_order_relaxed)) {}

  /**
   * @brief Destructor.
   */
  ~AggregationStateCount() override {}

  std::size_t getPayloadSize() const { return sizeof(count_); }

  const std::uint8_t* getPayloadAddress() const {
    return reinterpret_cast<const uint8_t *>(&count_);
  }

 private:
  friend class AggregationHandleCount<false, false>;
  friend class AggregationHandleCount<false, true>;
  friend class AggregationHandleCount<true, false>;
  friend class AggregationHandleCount<true, true>;

  AggregationStateCount() : count_(0) {}

  explicit AggregationStateCount(const std::int64_t initial_count)
      : count_(initial_count) {}

  std::atomic<std::int64_t> count_;
};

/**
 * @brief An aggregationhandle for count.
 *
 * @param count_star If true, this AggregationHandleCount is for nullary
 *        COUNT(*). If false, it is a COUNT over some scalar expression.
 * @param nullable_type If true, the argument to COUNT() is nullable, and NULL
 *        values should not be included in the count. If false, the argument is
 *        not nullable and NULL-checks can safely be skipped.
 **/
template <bool count_star, bool nullable_type>
class AggregationHandleCount : public AggregationConcreteHandle {
 public:
  ~AggregationHandleCount() override {}

  std::vector<const Type *> getArgumentTypes() const override {
    if (argument_type_ == nullptr) {
      return {};
    } else {
      return {argument_type_};
    }
  }

  const Type* getResultType() const override {
    return &LongType::InstanceNonNullable();
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateCount();
  }

  inline void iterateNullaryInl(AggregationStateCount *state) const {
    state->count_.fetch_add(1, std::memory_order_relaxed);
  }

  inline void iterateNullaryInl(std::uint8_t *byte_ptr) const {
    std::int64_t *count_ptr = reinterpret_cast<std::int64_t *>(byte_ptr);
    ++(*count_ptr);
  }

  inline void iterateUnaryInl(AggregationStateCount *state,
                              const TypedValue &value) const {
    if ((!nullable_type) || (!value.isNull())) {
      state->count_.fetch_add(1, std::memory_order_relaxed);
    }
  }

  inline void iterateUnaryInl(const TypedValue &value,
                              std::uint8_t *byte_ptr) const {
    if ((!nullable_type) || (!value.isNull())) {
      ++(*reinterpret_cast<std::int64_t *>(byte_ptr));
    }
  }

  AggregationState* accumulateNullary(
      const std::size_t num_tuples) const override {
    return new AggregationStateCount(num_tuples);
  }

  AggregationState* accumulateValueAccessor(
      const std::vector<MultiSourceAttributeId> &argument_ids,
      const ValueAccessorMultiplexer &accessor_mux) const override;

  void mergeStates(const AggregationState &source,
                   AggregationState *destination) const override;

  TypedValue finalize(const AggregationState &state) const override {
    return TypedValue(
        static_cast<const AggregationStateCount &>(state).count_.load(
            std::memory_order_relaxed));
  }

  std::size_t getPayloadSize() const override {
    return sizeof(std::int64_t);
  }

  void initPayload(std::uint8_t *byte_ptr) const override {
    std::int64_t *count_ptr = reinterpret_cast<std::int64_t *>(byte_ptr);
    *count_ptr = 0;
  }

  void destroyPayload(std::uint8_t *byte_ptr) const override {}

  inline void updateStateNullary(std::uint8_t *byte_ptr) const override {
    if (!block_update_) {
      iterateNullaryInl(byte_ptr);
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
    return TypedValue(*reinterpret_cast<const std::int64_t *>(byte_ptr));
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
  friend class AggregateFunctionCount;

  /**
   * @brief Initialize handle for count.
   *
   * @param argument_type Type of the value to be counted. The parameter should
   *        be nullptr for nullary aggregation (i.e. COUNT(*)).
   **/
  explicit AggregationHandleCount(const Type *argument_type)
      : AggregationConcreteHandle(AggregationID::kCount),
        argument_type_(argument_type) {}

  const Type *argument_type_;

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleCount);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_COUNT__
