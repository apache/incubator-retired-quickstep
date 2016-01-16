/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_JOIN_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class Join;
typedef std::shared_ptr<const Join> JoinPtr;

/**
 * @brief Base class for physical join nodes.
 */
class Join : public Physical {
 public:
  /**
   * @brief Destructor.
   */
  ~Join() override {}

  /**
   * @brief Gets the project expressions.
   *
   * @return The list of project expressions.
   */
  const std::vector<expressions::NamedExpressionPtr>& project_expressions() const {
    return project_expressions_;
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return expressions::ToRefVector(project_expressions_);
  }

 protected:
  /**
   * @brief Constructor.
   *
   * @param project_expressions The project expressions.
   */
  explicit Join(
      const std::vector<expressions::NamedExpressionPtr>& project_expressions)
      : project_expressions_(project_expressions) {}

 private:
  std::vector<expressions::NamedExpressionPtr> project_expressions_;

  DISALLOW_COPY_AND_ASSIGN(Join);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_JOIN_HPP_ */
