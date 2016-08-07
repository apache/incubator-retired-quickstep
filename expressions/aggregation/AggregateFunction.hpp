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

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_HPP_

#include <string>
#include <vector>

#include "expressions/aggregation/AggregateFunction.pb.h"
#include "expressions/aggregation/AggregationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregationHandle;
class Type;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief A class representing a particular aggregate function in the abstract
 *        sense. Each distinct, named aggregate function (e.g. AVG, COUNT, MAX,
 *        MIN, SUM) is represented by a singleton subclass of
 *        AggregateFunction.
 *
 * AggregateFunction provides informational methods about the applicability of
 * a particular aggregate function to particular argument Type(s). The actual
 * implementation of the aggregate functions' logic is in the AggregationHandle
 * class hierarchy, and can be different depending on the particular argument
 * Type(s) given to an aggregate. To perform an aggregation, a caller should
 * first call AggregateFunction::createHandle() to instantiate an
 * AggregationHandle object, then use the methods of AggregationHandle to do
 * the actual aggregation, then finally delete the AggregationHandle when
 * finished.
 *
 * See AggregationHandle for more detailed information about how aggregates are
 * actually computed.
 **/
class AggregateFunction {
 public:
  /**
   * @brief Get the ID of this aggregate (i.e. its unique ID amongst all the
   *        AggregateFunctions).
   *
   * @return The AggregationID of this AggregateFunction.
   **/
  inline AggregationID getAggregationID() const {
    return agg_id_;
  }

  /**
   * @brief Get the human-readable name of this AggregateFunction.
   *
   * @return The human-readable name of this AggregateFunction.
   **/
  virtual std::string getName() const = 0;

  /**
   * @brief Get the serialized protocol buffer representation of this
   *        AggregateFunction.
   *
   * @return A serialized protocol buffer representation of this
   *         AggregateFunction.
   **/
  virtual serialization::AggregateFunction getProto() const;

  /**
   * @brief Determine if this AggregateFunction can be applied to arguments of
   *        particular Type(s).
   *
   * @note In general, aggregates can have any arity (number of arguments).
   *       COUNT(*) has 0 arguments. Ordinary COUNT takes 1 arguments, as do
   *       the other SQL89 aggregates AVG, MAX, MIN, and SUM. Some statistical
   *       aggregates (not currently implemented in Quickstep) can take 2 or
   *       more arguments.
   *
   * @param argument_types A list of zero or more Types (in order) for
   *        arguments to this AggregateFunction.
   * @return Whether this AggregateFunction is applicable to the given
   *         argument_types.
   **/
  virtual bool canApplyToTypes(
      const std::vector<const Type*> &argument_types) const = 0;

  /**
   * @brief Determine the result Type for this AggregateFunction given
   *        arguments of particular Type(s).
   *
   * @param argument_types A list of zero or more Types (in order) for
   *        arguments to this AggregateFunction.
   * @return The result Type for this AggregateFunction applied to the
   *         specified argument_types, or NULL if this AggregateFunction is not
   *         applicable to the specified Type(s).
   **/
  virtual const Type* resultTypeForArgumentTypes(
      const std::vector<const Type*> &argument_types) const = 0;

  /**
   * @brief Create an AggregationHandle to compute aggregates.
   *
   * @warning It is an error to call this method for argument_types which this
   *          AggregateFunction can not apply to. If in doubt, check
   *          canApplyToTypes() first.
   *
   * @param argument_types A list of zero or more Types (in order) for
   *        arguments to this AggregateFunction.
   * @return A new AggregationHandle that can be used to compute this
   *         AggregateFunction over the specified argument_types. Caller is
   *         responsible for deleting the returned object.
   **/
  virtual AggregationHandle* createHandle(
      const std::vector<const Type*> &argument_types) const = 0;

 protected:
  explicit AggregateFunction(const AggregationID agg_id)
      : agg_id_(agg_id) {
  }

 private:
  const AggregationID agg_id_;

  DISALLOW_COPY_AND_ASSIGN(AggregateFunction);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_HPP_
