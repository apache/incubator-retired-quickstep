/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"

#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class Physical;
typedef std::shared_ptr<const Physical> PhysicalPtr;

/**
 * @brief Base class for physical plan nodes.
 */
class Physical : public OptimizerTree<Physical> {
 public:
  /**
   * @brief Destructor.
   */
  ~Physical() override {}

  /**
   * @return The physical node type.
   */
  virtual PhysicalType getPhysicalType() const = 0;

  /**
   * @brief Gets the output attributes of this node.
   *
   * @return The output AttributeReferences by this node.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const = 0;

  /**
   * @brief Returns AttributeReferences referenced by the input expressions of
   *        this physical node.
   *
   * @return A vector of AttributeReference referenced by the input expressions
   *         of this physical node.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const = 0;

  /**
   * @brief Creates a new copy in \p output if there are some project
   *        expressions that are not in \p referenced_expressions.
   *        Otherwise, keeps \p output unchanged.
   *
   * @param referenced_attributes The attributes to be kept in the project
   *                              expression list.
   * @param output A new copy if some project expressions should be removed.
   * @return True if a new copy is generated (i.e. \p output is changed).
   */
  virtual bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const = 0;

 protected:
  /**
   * @brief Constructor.
   */
  Physical() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Physical);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_HPP_ */
