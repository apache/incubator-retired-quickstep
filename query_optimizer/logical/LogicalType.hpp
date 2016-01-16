/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_TYPE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_TYPE_HPP_

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerExpressions
 *  @{
 */

/**
 * @brief Optimizer logical node types.
 **/
enum class LogicalType {
  kAggregate,
  kCopyFrom,
  kCreateTable,
  kDeleteTuples,
  kDropTable,
  kFilter,
  kHashJoin,
  kInsertTuple,
  kMultiwayCartesianJoin,
  kNestedLoopsJoin,
  kProject,
  kSharedSubplanReference,
  kSort,
  kTableReference,
  kTopLevelPlan,
  kUpdateTable
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_TYPE_HPP_ */
