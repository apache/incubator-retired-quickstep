/**
 *   Licensed to the Apache Software Foundation (ASF) under one
 *   or more contributor license agreements.  See the NOTICE file
 *   distributed with this work for additional information
 *   regarding copyright ownership.  The ASF licenses this file
 *   to you under the Apache License, Version 2.0 (the
 *   "License"); you may not use this file except in compliance
 *   with the License.  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *   Unless required by applicable law or agreed to in writing,
 *   software distributed under the License is distributed on an
 *   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *   KIND, either express or implied.  See the License for the
 *   specific language governing permissions and limitations
 *   under the License.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATION_HANDLE_HPP_
#define QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATION_HANDLE_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class InsertDestinationInterface;
class Scalar;
class StorageManager;
class Type;
class ValueAccessor;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief WindowAggregationHandle encapsulates logic for actually computing
 *        window aggregates with particular argument(s).
 * @note See also WindowAggregateFunction, which represents a SQL aggregate
 *       function in the abstract sense.
 *
 * A WindowAggregationHandle is created by calling
 * WindowAggregateFunction::createHandle(). The WindowAggregationHandle object
 * provides methods that are used to actually compute the window aggregate,
 * storing intermediate results in WindowAggregationState objects.
 *
 * The work flow for computing a window aggregate is:
 *     1. Create an initial state by createInitialState().
 *     2. One thread will handle all the computation, iterating from the first
 *        tuple to the last tuple. Note there will be two modes that could be
 *        used upon different situations:
 *        a. If the window aggregate is defined as accumulative, which are:
 *           i.  Functions applied to whole partition, such as rank(), ntile()
 *               and dense_rank().
 *           ii. The window frame is defined as "BETWEEN UNBOUNDED PRECEDING
 *               AND CURRENT ROW" or "BETWEEN CURRENT ROW AND UNBOUNDED
 *               FOLLOWING".
 *           Then, for functions except median, we could store some global
 *           values in the state without keeping all the tuple values around.
 *        b. If the window frame is sliding, such as "BETWEEN 3 PRECEDING AND
 *           3 FOLLOWING", we have to store all the tuples in the state so that
 *           we could know which values should be dropped as the window slides.
 *        For each computed value, generate a tuple store in the column vector.
 *     3. Insert the new column into the original relation and return.
 *
 * TODO(Shixuan): Currently we don't support parallelization. The basic idea for
 * parallelization is to calculate the partial result inside each block. Each
 * block could visit the following blocks as long as the block's last partition
 * is not finished. WindowAggregationOperationState will be used for handling
 * the global state of the calculation.
 **/

class WindowAggregationHandle {
 public:
  /**
   * @brief Destructor.
   **/
  virtual ~WindowAggregationHandle() {}

  /**
   * @brief Calculate the window aggregate result.
   *
   * @param block_accessors A pointer to the value accessor of block attributes.
   * @param arguments The ColumnVectors of arguments
   * @param partition_by_ids The ids of partition keys.
   * @param is_row True if the frame mode is ROWS, false if it is RANGE.
   * @param num_preceding The number of rows/range that precedes the current row.
   * @param num_following The number of rows/range that follows the current row.
   *
   * @return A ColumnVector of the calculated window aggregates.
   **/
  virtual ColumnVector* calculate(ColumnVectorsValueAccessor* block_accessors,
                                  std::vector<ColumnVector*> &&arguments,
                                  const std::vector<attribute_id> &partition_by_ids,
                                  const bool is_row,
                                  const std::int64_t num_preceding,
                                  const std::int64_t num_following) const = 0;

 protected:
  /**
   * @brief Constructor.
   *
   * @param partition_key_types The Types of the partition key.
   **/
  explicit WindowAggregationHandle(
      const std::vector<const Type*> &partition_key_types) {
    // Comparison operators for checking if two tuples belong to the same partition.
    for (const Type *partition_key_type : partition_key_types) {
      equal_comparators_.emplace_back(
          ComparisonFactory::GetComparison(ComparisonID::kEqual)
              .makeUncheckedComparatorForTypes(*partition_key_type, *partition_key_type));
    }
  }

  std::vector<std::unique_ptr<UncheckedComparator>> equal_comparators_;

 private:
  DISALLOW_COPY_AND_ASSIGN(WindowAggregationHandle);
};

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATION_HANDLE_HPP_
