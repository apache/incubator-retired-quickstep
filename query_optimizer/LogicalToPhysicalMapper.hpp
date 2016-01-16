/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
