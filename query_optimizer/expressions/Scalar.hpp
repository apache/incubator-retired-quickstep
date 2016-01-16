/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_HPP_

#include <memory>
#include <unordered_map>

#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Scalar;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class Scalar;
typedef std::shared_ptr<const Scalar> ScalarPtr;

/**
 * @brief Scalar expression that returns a single value based on input values
 *        that are not across multiple rows in an input relation.
 */
class Scalar : public Expression {
 public:
  ~Scalar() override {}

  /**
   * @brief Converts this expression to an expression for evaluation (in
   *        quickstep execution expressions system under /expressions)
   *        with each attribute reference being replaced by a CatalogAttribute
   *        reference provided in \p substitution_map.
   *
   * @param substitution_map Map from ExprId to CatalogAttribute for use in
   *                         substituting CatalogAttribute for
   *                         AttributeReference.
   * @return Concretized expression for evaluation.
   */
  virtual ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*>& substitution_map)
      const = 0;

 protected:
  Scalar() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Scalar);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_HPP_ */
