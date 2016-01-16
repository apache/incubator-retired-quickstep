/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MIN_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MIN_HPP_

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
 * @brief Aggregation state for min.
 */
class AggregationStateMin : public AggregationState {
 public:
  /**
   * @brief Copy constructor (ignores mutex).
   */
  AggregationStateMin(const AggregationStateMin &orig)
      : min_(orig.min_) {
  }

  /**
   * @brief Destructor.
   */
  ~AggregationStateMin() override {}

 private:
  friend class AggregationHandleMin;

  explicit AggregationStateMin(const Type &type)
      : min_(type.getNullableVersion().makeNullValue()) {}

  explicit AggregationStateMin(TypedValue &&value)
      : min_(std::move(value)) {
  }

  TypedValue min_;
  SpinMutex mutex_;
};

/**
 * @brief An aggregationhandle for min.
 **/
class AggregationHandleMin : public AggregationHandle {
 public:
  ~AggregationHandleMin() override {
  }

  AggregationState* createInitialState() const override {
    return new AggregationStateMin(type_);
  }

  AggregationStateHashTableBase* createGroupByHashTable(
      const HashTableImplType hash_table_impl,
      const std::vector<const Type*> &group_by_types,
      const std::size_t estimated_num_groups,
      StorageManager *storage_manager) const override;

  /**
   * @brief Iterate with min aggregation state.
   */
  inline void iterateUnaryInl(AggregationStateMin *state, const TypedValue &value) const {
    DCHECK(value.isPlausibleInstanceOf(type_.getSignature()));
    compareAndUpdate(state, value);
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
    return static_cast<const AggregationStateMin&>(state).min_;
  }

  inline TypedValue finalizeHashTableEntry(const AggregationState &state) const {
    return static_cast<const AggregationStateMin&>(state).min_;
  }

  ColumnVector* finalizeHashTable(
      const AggregationStateHashTableBase &hash_table,
      std::vector<std::vector<TypedValue>> *group_by_keys) const override;

 private:
  friend class AggregateFunctionMin;

  /**
   * @brief Constructor.
   *
   * @param type Type of the data set.
   **/
  explicit AggregationHandleMin(const Type &type);

  /**
   * @brief compare the value with min_ and update it if the value is smaller than
   *        current minimum. NULLs are ignored.
   *
   * @param value A TypedValue to compare.
   **/
  inline void compareAndUpdate(AggregationStateMin *state, const TypedValue &value) const {
    if (value.isNull()) return;

    SpinMutexLock lock(state->mutex_);
    if (state->min_.isNull() || fast_comparator_->compareTypedValues(value, state->min_)) {
      state->min_ = value;
    }
  }

  const Type &type_;
  std::unique_ptr<UncheckedComparator> fast_comparator_;

  DISALLOW_COPY_AND_ASSIGN(AggregationHandleMin);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_HANDLE_MIN_HPP_
