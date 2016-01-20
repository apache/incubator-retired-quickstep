/**
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TO_PHYSICAL_MAPPER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TO_PHYSICAL_MAPPER_HPP_

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Abstract base class for PhysicalGenerator that provides a single
 *        method needed by Strategy objects. This class exists to resolve a
 *        cyclic dependency between PhysicalGenerator and the various
 *        subclasses of Strategy.
 **/
class LogicalToPhysicalMapper {
 public:
  virtual ~LogicalToPhysicalMapper() {
  }

  /**
   * @brief Returns the physical plan for \p logical_plan.
   *        Creates the physical plan if it is not yet created.
   *
   * @param logical_plan The input logical plan.
   * @return The physical plan for the logical plan.
   */
  virtual physical::PhysicalPtr createOrGetPhysicalFromLogical(
      const logical::LogicalPtr &logical_plan) = 0;

 protected:
  LogicalToPhysicalMapper() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(LogicalToPhysicalMapper);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TO_PHYSICAL_MAPPER_HPP_
