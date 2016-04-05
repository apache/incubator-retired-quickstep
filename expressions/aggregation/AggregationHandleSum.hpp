/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_SUM_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_SUM_HPP_

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationConcreteHandle.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "storage/HashTableBase.hpp"
#include "threading/SpinMutex.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
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
 * @brief Aggregation state for sum.
 */
class AggregationStateSum : public AggregationState {
 public:
  /**
   * @brief Copy constructor (ignores mutex).
   */
  AggregationStateSum(const AggregationStateSum &orig)
      : sum_(orig.sum_),
        null_(orig.null_) {
  }

 private:
  friend class AggregationHandleSum;

  AggregationStateSum()
      : sum_(0), null_(true) {
  }

  AggregationStateSum(TypedValue &&sum, const bool is_null)
      : sum_(std::move(sum)), null_(is_null) {
  }

  // TODO(shoban): We might want to specialize sum_ to use atomics for int types
  // similar to in AggregationStateCount.
  TypedValue sum_;
  bool null_;
  SpinMutex mutex_;
};

/**
 * @brief An aggregationhandle for sum.
 **/
class AggregationHandleSum : public AggregationConcreteHandle {
 public:
  ~AggregationHandleSum() override {
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateSum(blank_state_);
  }

  AggregationStateHashTableBase* createGroupByHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type*> &group_by_types,
      const std::size_t estimated_num_groups,
      StorageManager *storage_manager) const override;

  inline void iterateUnaryInl(AggregationStateSum *state, const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(argument_type_.getSignature()));
    if (value.isNull()) return;

    SpinMutexLock lock(state->mutex_);
    state->sum_ = fast_operator_->applyToTypedValues(state->sum_, value);
    state->null_ = false;
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

  TypedValue finalize(const AggregationState &state) const override;

  inline TypedValue finalizeHashTableEntry(const AggregationState &state) const {
    return static_cast<const AggregationStateSum&>(state).sum_;
  }

  ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      std::vector<std::vector<TypedValue>> *group_by_keys) const override;

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
