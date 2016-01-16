/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_NOT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_NOT_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class LogicalNot;
typedef std::shared_ptr<const LogicalNot> LogicalNotPtr;

/**
 * @brief Applies a logical negation (NOT) operator to a predicate expression.
 */
class LogicalNot : public Predicate {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kLogicalNot;
  }

  std::string getName() const override { return "NOT"; }

  bool isConstant() const override { return operand_->isConstant(); }

  /**
   * @return The operand of this negation.
   */
  const PredicatePtr& operand() const { return operand_; }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return operand_->getReferencedAttributes();
  }

  ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Creates an immutable LogicalNot.
   *
   * @param operand Operand of the logical not operator.
   * @return  An immutable LogicalNot.
   */
  static LogicalNotPtr Create(const PredicatePtr &operand) {
    return LogicalNotPtr(new LogicalNot(operand));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  explicit LogicalNot(const PredicatePtr &operand)
      : operand_(operand) {
    addChild(operand_);
  }

  PredicatePtr operand_;

  DISALLOW_COPY_AND_ASSIGN(LogicalNot);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_NOT_HPP_ */
