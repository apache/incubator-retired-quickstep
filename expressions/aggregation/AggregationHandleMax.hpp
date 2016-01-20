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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MAX_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MAX_HPP_

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
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
 * @brief Aggregation state for max.
 */
class AggregationStateMax : public AggregationState {
 public:
  /**
   * @brief Copy constructor (ignores mutex).
   */
  AggregationStateMax(const AggregationStateMax &orig)
      : max_(orig.max_) {
  }

  /**
   * @brief Destructor.
   */
  ~AggregationStateMax() override {};

 private:
  friend class AggregationHandleMax;

  explicit AggregationStateMax(const Type &type)
      : max_(type.getNullableVersion().makeNullValue()) {
  }

  explicit AggregationStateMax(TypedValue &&value)
      : max_(std::move(value)) {
  }

  TypedValue max_;
  SpinMutex mutex_;
};

/**
 * @brief An aggregationhandle for max.
 **/
class AggregationHandleMax : public AggregationHandle {
 public:
  ~AggregationHandleMax() override {
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateMax(type_);
  }

  AggregationStateHashTableBase* createGroupByHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type*> &group_by_types,
      const std::size_t estimated_num_groups,
      StorageManager *storage_manager) const override;

  /**
   * @brief Iterate with max aggregation state.
   */
  inline void iterateUnaryInl(AggregationStateMax *state, const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    compareAndUpdate(static_cast<AggregationStateMax*>(state), value);
  }

  AggregationState* accumulateColumnVectors(
      const std::vector<std::unique_ptr<ColumnVector>> &column_vectors) const override;

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

  TypedValue finalize(const AggregationState &state) const override {
    return TypedValue(static_cast<const AggregationStateMax&>(state).max_);
  }

  inline TypedValue finalizeHashTableEntry(const AggregationState &state) const {
    return TypedValue(static_cast<const AggregationStateMax&>(state).max_);
  }

  ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      std::vector<std::vector<TypedValue>> *group_by_keys) const override;

 private:
  friend class AggregateFunctionMax;

  /**
   * @brief Constructor.
   *
   * @param type Type of the data set.
   **/
  explicit AggregationHandleMax(const Type &type);

  /**
   * @brief compare the value with max_ and update it if the value is larger than
   *        current maximum. NULLs are ignored.
   *
   * @param value A TypedValue to compare
   **/
  inline void compareAndUpdate(AggregationStateMax *state, const TypedValue &value) const {
    // TODO(chasseur): Avoid null-checks when aggregating a non-nullable Type.
    if (value.isNull()) return;

    SpinMutexLock lock(state->mutex_);
    if (state->max_.isNull() || fast_comparator_->compareTypedValues(value, state->max_)) {
      state->max_ = value;
    }
  }

  const Type &type_;
  std::unique_ptr<UncheckedComparator> fast_comparator_;

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleMax);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MAX_HPP_
