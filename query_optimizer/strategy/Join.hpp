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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_JOIN_HPP_

#include <string>
#include <vector>

#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/NestedLoopsJoin.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/strategy/Strategy.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class LogicalToPhysicalMapper;

namespace strategy {

/** \addtogroup OptimizerStrategy
 *  @{
 */

/**
 * @brief Strategy to create a physical join plan.
 */
class Join : public Strategy {
 public:
  /**
   * @brief Constructor.
   *
   * @param physical_mapper The physical generator used to get child physical
   *        plans.
   */
  explicit Join(LogicalToPhysicalMapper *physical_mapper)
      : Strategy(physical_mapper) {}

  std::string getName() const override { return "Join"; }

  bool generatePlan(const logical::LogicalPtr &logical_input,
                    physical::PhysicalPtr *physical_output) override;

 private:
  /**
   * @brief Returns a physical HashJoin in physical_output.
   *
   * @param logical_project The project to be folded into the hash join.
   * @param logical_filter The filter to be folded into the hash join.
   * @param logical_hash_join The hash join to be transformed.
   * @param physical_output The output physical plan.
   */
  void addHashJoin(const logical::ProjectPtr &logical_project,
                   const logical::FilterPtr &logical_filter,
                   const logical::HashJoinPtr &logical_hash_join,
                   physical::PhysicalPtr *physical_output);

  /**
   * @brief Returns a physical NestedLoopsJoin in physical_output.
   *
   * @param logical_nested_loops_join The logical nested loops join.
   * @param join_predicate Join predicate which may include the predicate
   *        from a filter upon the nested loops join.
   * @param project_expressions The project expressions for the join.
   * @param physical_output The output physical plan.
   */
  void addNestedLoopsJoin(
      const logical::NestedLoopsJoinPtr &logical_nested_loops_join,
      expressions::PredicatePtr join_predicate,
      std::vector<expressions::NamedExpressionPtr> project_expressions,
      physical::PhysicalPtr *physical_output);

  DISALLOW_COPY_AND_ASSIGN(Join);
};

/** @} */

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_JOIN_HPP_ */
