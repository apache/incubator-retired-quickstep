/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_HPP_

#include <memory>
#include <map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "types/Type.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class AttributeReference;
class Expression;

typedef std::shared_ptr<const Expression> ExpressionPtr;

/**
 * @brief Base class for all expressions in the query optimizer.
 */
class Expression : public OptimizerTree<Expression> {
 public:
  /**
   * @brief Destructor.
   */
  ~Expression() override {}

  /**
   * @brief Returns the expression type.
   *
   * @return ExpressionType of this expression.
   */
  virtual ExpressionType getExpressionType() const = 0;

  /**
   * @brief Returns the type of the output.
   *
   * @return The type of the output.
   */
  virtual const Type& getValueType() const = 0;

  /**
   * @brief Returns AttributeReferences referenced by this expression.
   *
   * @return A vector of AttributeReference referenced by this expression.
   */
  virtual std::vector<std::shared_ptr<const AttributeReference>> getReferencedAttributes() const = 0;

  /**
   * @return True if this expression is constant.
   */
  virtual bool isConstant() const = 0;

 protected:
  Expression() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Expression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_HPP_ */
