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

#ifndef QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATE_FUNCTION_HPP_
#define QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATE_FUNCTION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/window_aggregation/WindowAggregateFunction.pb.h"
#include "expressions/window_aggregation/WindowAggregationID.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;
class Scalar;
class Type;
class WindowAggregationHandle;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief A class representing a particular window aggregate function in the
 *        abstract sense. Each named aggregate function is represented by a
 *        singleton subclass of WindowAggregateFunction.
 *
 * WindowAggregateFunction provides informational methods about the
 * applicability of a particular window aggregate function to particular
 * argument Type(s). The actual implementation of the window aggregate
 * functions' logic is in the WindowAggregationHandle class hierarchy, and can
 * be different depending on the particular argument Type(s) given to the window
 * aggregate. To perform a window aggregation, a caller should first call
 * WindowAggregateFunction::createHandle() to instantiate an
 * WindowAggregationHandle object, then use the methods of
 * WindowAggregationHandle to do the actual window aggregation. Finally, delete
 * the WindowAggregationHandle after finished.
 * 
 * See WindowAggregationHandle for more detailed information about how
 * window aggregates are actually computed.
 **/
class WindowAggregateFunction {
 public:
  /**
   * @brief Get the ID of this window aggregate (i.e. its unique ID amongst all
   *        the WindowAggregateFunctions).
   *
   * @return The WindowAggregationID of this WindowAggregateFunction.
   **/
  inline WindowAggregationID getWindowAggregationID() const {
    return win_agg_id_;
  }

  /**
   * @brief Get the human-readable name of this WindowAggregateFunction.
   *
   * @return The human-readable name of this WindowAggregateFunction.
   **/
  virtual std::string getName() const = 0;

  /**
   * @brief Get the serialized protocol buffer representation of this
   *        WindowAggregateFunction.
   *
   * @return A serialized protocol buffer representation of this
   *         WindowAggregateFunction.
   **/
  virtual serialization::WindowAggregateFunction getProto() const;

  /**
   * @brief Determine if this WindowAggregateFunction can be applied to
   *        arguments of particular Type(s).
   *
   * @param argument_types A list of zero or more Types (in order) for
   *        arguments to this WindowAggregateFunction.
   * @return Whether this WindowAggregateFunction is applicable to the given
   *         argument_types.
   **/
  virtual bool canApplyToTypes(
      const std::vector<const Type*> &argument_types) const = 0;

  /**
   * @brief Determine the result Type for this WindowAggregateFunction given
   *        arguments of particular Type(s).
   *
   * @param argument_types A list of zero or more Types (in order) for
   *        arguments to this WindowAggregateFunction.
   * @return The result Type for this WindowAggregateFunction applied to the
   *         specified argument_types, or nullptr if this
   *         WindowAggregateFunction is not applicable to the specified Type(s).
   **/
  virtual const Type* resultTypeForArgumentTypes(
      const std::vector<const Type*> &argument_types) const = 0;

  /**
   * @brief Create a WindowAggregationHandle to compute aggregates.
   *
   * @warning It is an error to call this method for argument_types which this
   *          WindowAggregateFunction can not apply to. For safety, check
   *          canApplyToTypes() first.
   *
   * @param argument_types A list of zero or more Types (in order) for
   *        arguments to this WindowAggregateFunction.
   * @param partition_by_attributes A list of attributes used as partition key.
   * @param order_by_attributes A list of attributes used as order key.
   * @param is_row True if the frame mode is ROWS, false if RANGE.
   * @param num_preceding The number of rows/range that precedes the current row.
   * @param num_following The number of rows/range that follows the current row.
   * 
   * @return A new WindowAggregationHandle that can be used to compute this
   *         WindowAggregateFunction over the specified window definition.
   *         Caller is responsible for deleting the returned object.
   **/
  virtual WindowAggregationHandle* createHandle(
      const std::vector<const Type*> &argument_types,
      const std::vector<std::unique_ptr<const Scalar>> &partition_by_attributes,
      const std::vector<std::unique_ptr<const Scalar>> &order_by_attributes,
      const bool is_row,
      const std::int64_t num_preceding,
      const std::int64_t num_following) const = 0;

 protected:
  explicit WindowAggregateFunction(const WindowAggregationID win_agg_id)
      : win_agg_id_(win_agg_id) {
  }

 private:
  const WindowAggregationID win_agg_id_;

  DISALLOW_COPY_AND_ASSIGN(WindowAggregateFunction);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_WINDOW_AGGREGATION_WINDOW_AGGREGATE_FUNCTION_HPP_
