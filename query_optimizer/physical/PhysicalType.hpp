/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_TYPE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_TYPE_HPP_

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

/**
 * @brief Optimizer physical node types.
 **/
enum class PhysicalType {
  kAggregate,
  kCopyFrom,
  kCreateTable,
  kDeleteTuples,
  kDropTable,
  kHashJoin,
  kInsertTuple,
  kNestedLoopsJoin,
  kSelection,
  kSharedSubplanReference,
  kSort,
  kTableReference,
  kTopLevelPlan,
  kUpdateTable
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_TYPE_HPP_ */
