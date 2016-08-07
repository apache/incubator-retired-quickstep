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

#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace cost {

/** \addtogroup CostModel
 *  @{
 */

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
