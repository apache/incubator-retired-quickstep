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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_GENERATOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_GENERATOR_HPP_

#include "query_optimizer/logical/Logical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseStatement;

namespace optimizer {

class OptimizerContext;

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Generates a logical plan from a parse tree.
 */
class LogicalGenerator {
 public:
  /**
   * @brief Constructor.
   *
   * @param optimizer_context The optimizer context.
   */
  explicit LogicalGenerator(OptimizerContext *optimizer_context);

  /**
   * @brief Destructor.
   */
  ~LogicalGenerator();

  /**
   * @return The current logical plan.
   */
  const logical::LogicalPtr& logical_plan() const { return logical_plan_; }

  /**
   * @brief Converts the parse tree to a logical plan and optimizes the logical
   *        plan by using rules.
   *
   * @param parse_statement The parse tree to be converted.
   * @return An optimized logical plan.
   */
  logical::LogicalPtr generatePlan(const ParseStatement &parse_statement);

 private:
  /**
   * @brief Applies rules to the logical plan.
   */
  void optimizePlan();

  OptimizerContext *optimizer_context_;
  logical::LogicalPtr logical_plan_;

  DISALLOW_COPY_AND_ASSIGN(LogicalGenerator);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_GENERATOR_HPP_ */
