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

#ifndef QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATION_HANDLE_AVG_HPP_
#define QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATION_HANDLE_AVG_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <queue>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/window_aggregation/WindowAggregationHandle.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class ColumnVectorsValueAccessor;
class ValueAccessor;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief A WindowAggregationHandle for average.
 **/
class WindowAggregationHandleAvg : public WindowAggregationHandle {
 public:
  ~WindowAggregationHandleAvg() override {}

  ColumnVector* calculate(ColumnVectorsValueAccessor* block_accessors,
                          const std::vector<ColumnVector*> &arguments) const override;

 private:
  friend class WindowAggregateFunctionAvg;

  /**
   * @brief Constructor.
   *
   * @param partition_by_attributes A list of attributes used as partition key.
   * @param order_by_attributes A list of attributes used as order key.
   * @param is_row True if the frame mode is ROWS, false if RANGE.
   * @param num_preceding The number of rows/range that precedes the current row.
   * @param num_following The number of rows/range that follows the current row.
   * @param argument_type Type of the argument.
   **/
  WindowAggregationHandleAvg(
      const std::vector<std::unique_ptr<const Scalar>> &partition_by_attributes,
      const std::vector<std::unique_ptr<const Scalar>> &order_by_attributes,
      const bool is_row,
      const std::int64_t num_preceding,
      const std::int64_t num_following,
      const Type *argument_type);

  const Type *sum_type_;
  const Type *result_type_;
  std::unique_ptr<UncheckedBinaryOperator> fast_add_operator_;
  std::unique_ptr<UncheckedBinaryOperator> fast_subtract_operator_;
  std::unique_ptr<UncheckedBinaryOperator> divide_operator_;

  DISALLOW_COPY_AND_ASSIGN(WindowAggregationHandleAvg);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATION_HANDLE_AVG_HPP_
