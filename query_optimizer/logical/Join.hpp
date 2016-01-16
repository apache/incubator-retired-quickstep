/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_JOIN_HPP_

#include <memory>
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Join;
typedef std::shared_ptr<const Join> JoinPtr;

/**
 * @brief Base class for logical join operator that combines tuples from two or
 *        more relations based on join predicates.
 */
class Join : public Logical {
 public:
  /**
   * @brief Destructor.
   */
  ~Join() override {}

 protected:
  /**
   * @brief Constructor.
   */
  Join() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Join);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_JOIN_HPP_ */
