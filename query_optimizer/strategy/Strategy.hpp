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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_STRATEGY_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_STRATEGY_HPP_

#include <string>
#include <vector>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class OptimizerContext;
class LogicalToPhysicalMapper;

namespace strategy {

/** \addtogroup OptimizerStrategy
 *  @{
 */

/**
 * @brief Base class for strategies that implement how to convert a logical
 *        plan to a physical plan for a type of operators.
 */
class Strategy {
 public:
  /**
   * @brief Destructor.
   */
  virtual ~Strategy() {}

  /**
   * @brief Converts \p logical_input to \p physical_output if the strategy
   *        can be applied to the input; otherwise, set physical_output to NULL.
   *
   * @param logical_input The logical plan to be converted
   * @param physical_output The physical plan output. NULL if the strategy
   *                        cannot be applied to the input.
   * @return True if the strategy has been applied to the input logical.
   */
  virtual bool generatePlan(const logical::LogicalPtr &logical_input,
                            physical::PhysicalPtr *physical_output) = 0;

  /**
   * @return The name of the strategy.
   */
  virtual std::string getName() const = 0;

 protected:
  /**
   * @brief Constructor.
   *
   * @param physical_mapper The physical plan generator. A strategy can
   *                        interact with other strategies via the plan
   *                        generator to create a complete plan.
   */
  explicit Strategy(LogicalToPhysicalMapper *physical_mapper)
      : physical_mapper_(physical_mapper) {}

  LogicalToPhysicalMapper *physical_mapper_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Strategy);
};

/** @} */

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_STRATEGY_STRATEGY_HPP_ */
