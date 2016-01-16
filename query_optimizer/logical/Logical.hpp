/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Logical;
typedef std::shared_ptr<const Logical> LogicalPtr;

/**
 * @brief Base class for all logical operator/node.
 */
class Logical : public OptimizerTree<Logical> {
 public:
  /**
   * @brief Destructor.
   */
  ~Logical() {}

  /**
   * @return The type of this logical operator.
   */
  virtual LogicalType getLogicalType() const = 0;

  /**
   * @return The references to the output attributes produced by this logical
   * operator.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const = 0;

  /**
   * @return The references to the input attributes used in this logical
   * operator.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const = 0;

 protected:
  /**
   * @brief Constructor.
   */
  Logical() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Logical);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_LOGICAL_HPP_ */
