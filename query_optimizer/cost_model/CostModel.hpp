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

#ifndef QUERY_OPTIMIZER_COST_MODEL_COST_MODEL_HPP_
#define QUERY_OPTIMIZER_COST_MODEL_COST_MODEL_HPP_

#include <cstddef>
#include <exception>
#include <string>

#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace cost {

/** \addtogroup CostModel
 *  @{
 */

/**
 * @brief Exception thrown for unsupported physical plan.
 **/
class UnsupportedPhysicalPlan : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param physical_plan The physical plan that is not supported by the cost
   *        model.
   **/
  explicit UnsupportedPhysicalPlan(const physical::PhysicalPtr &physical_plan)
      : message_("UnsupportedPhysicalPlan: \n" + physical_plan->toString()) {
  }

  ~UnsupportedPhysicalPlan() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Interface to a cost model of physical plans.
 */
class CostModel {
 public:
  /**
   * @brief Destructor.
   */
  virtual ~CostModel() {}

  /**
   * @brief Estimate the cardinality of the output relation of \p physical_plan.
   *
   * @param physical_plan The physical plan for which the cardinality of the
   *                      output relation is estimated.
   * @return The estimated cardinality.
   */
  virtual std::size_t estimateCardinality(
      const physical::PhysicalPtr &physical_plan) = 0;

  /**
   * @brief Estimate the number of groups of an aggregation.
   *
   * @param aggregate The physical plan of the aggregation.
   * @return The estimated number of groups.
   */
  virtual std::size_t estimateNumGroupsForAggregate(const physical::AggregatePtr &aggregate) {
    return estimateCardinality(aggregate);
  }

 protected:
  /**
   * @brief Constructor.
   */
  CostModel() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(CostModel);
};

/** @} */

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUERY_OPTIMIZER_COST_MODEL_COST_MODEL_HPP_ */
