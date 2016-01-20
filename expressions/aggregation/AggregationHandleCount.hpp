/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_COUNT_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_COUNT_HPP_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/HashTableBase.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ColumnVector;
class StorageManager;
class Type;
class ValueAccessor;

template <bool, bool> class AggregationHandleCount;

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

 private:
  friend class AggregationHandleCount<false, false>;
  friend class AggregationHandleCount<false, true>;
  friend class AggregationHandleCount<true, false>;
  friend class AggregationHandleCount<true, true>;

  AggregationStateCount()
      : count_(0) {
  }

  explicit AggregationStateCount(const std::int64_t initial_count)
      : count_(initial_count) {
  }

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
class AggregationHandleCount : public AggregationHandle {
 public:
  ~AggregationHandleCount() override {
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateCount();
  }

  AggregationStateHashTableBase* createGroupByHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type*> &group_by_types,
      const std::size_t estimated_num_groups,
      StorageManager *storage_manager) const override;

  inline void iterateNullaryInl(AggregationStateCount *state) const {
    state->count_.fetch_add(1, std::memory_order_relaxed);
  }

  /**
   * @brief Iterate with count aggregation state.
   */
  inline void iterateUnaryInl(AggregationStateCount *state, const TypedValue &value) const {
    if ((!nullable_type) || (!value.isNull())) {
      state->count_.fetch_add(1, std::memory_order_relaxed);
    }
  }

  AggregationState* accumulateNullary(const std::size_t num_tuples) const override {
    return new AggregationStateCount(num_tuples);
  }

  AggregationState* accumulateColumnVectors(
      const std::vector<std::unique_ptr<ColumnVector>> &column_vectors) const override;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  AggregationState* accumulateValueAccessor(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &accessor_id) const override;
#endif

  void aggregateValueAccessorIntoHashTable(
      ValueAccessor *accessor,
      const std::vector<attribute_id> &argument_ids,
      const std::vector<attribute_id> &group_by_key_ids,
      AggregationStateHashTableBase *hash_table) const override;

  void mergeStates(const AggregationState &source,
                   AggregationState *destination) const override;

  TypedValue finalize(const AggregationState &state) const override {
    return TypedValue(static_cast<const AggregationStateCount&>(state).count_.load(std::memory_order_relaxed));
  }

  inline TypedValue finalizeHashTableEntry(const AggregationState &state) const {
    return TypedValue(static_cast<const AggregationStateCount&>(state).count_.load(std::memory_order_relaxed));
  }

  ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      std::vector<std::vector<TypedValue>> *group_by_keys) const override;

 private:
  friend class AggregateFunctionCount;

  /**
   * @brief Constructor.
   **/
  AggregationHandleCount() {
  }

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleCount);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_COUNT__
