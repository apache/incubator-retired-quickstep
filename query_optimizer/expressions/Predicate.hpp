/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PREDICATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PREDICATE_HPP_

#include <memory>
#include <unordered_map>

#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Predicate;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class Predicate;
typedef std::shared_ptr<const Predicate> PredicatePtr;

/**
 * @brief Expression that returns boolean values.
 */
class Predicate : public Expression {
 public:
  /**
   * @brief Destructor.
   */
  ~Predicate() override {}

  const Type& getValueType() const override {
    // The type system does not have a bool type, so we use non-nullable kInt
    // instead.
    return TypeFactory::GetType(kInt /* TypeID */,
                                false /* nullable */);
  }

  /**
   * @brief Returns a concretized predicate in the Quickstep execution
   *        expression system with AttributeReferences replaced by
   *        CatalogAttribute references provided in \p substitution_map.
   * @note In the Quickstep execution expression system, Predicate is not Scalar
           *type. We do not use concretize() for predicate concretization.
   *
   * @param substitution_map Map from ExprId to CatalogAttribute for use in
   *                         replacing AttributeReference.
   * @return A concretized predicate for evaluation.
   */
  virtual ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*>& substitution_map) const = 0;

 protected:
  Predicate() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Predicate);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PREDICATE_HPP_ */
