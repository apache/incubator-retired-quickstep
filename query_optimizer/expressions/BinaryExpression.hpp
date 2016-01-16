/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_BINARY_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_BINARY_EXPRESSION_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class BinaryOperation;
class CatalogAttribute;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class BinaryExpression;
typedef std::shared_ptr<const BinaryExpression> BinaryExpressionPtr;

/**
 * @brief Applies a binary operator to two scalar expressions and returns a
 *        scalar value.
 */
class BinaryExpression : public Scalar {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kBinaryExpression;
  }

  std::string getName() const override;

  const Type& getValueType() const override;

  bool isConstant() const override {
    return left_->isConstant() && right_->isConstant();
  }

  /**
   * @return The binary operation.
   */
  const BinaryOperation& operation() const { return operation_; }

  /**
   * @return The left operand.
   */
  const ScalarPtr& left() const { return left_; }

  /**
   * @return The right operand.
   */
  const ScalarPtr& right() const { return right_; }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override;

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  static BinaryExpressionPtr Create(const BinaryOperation &operation,
                                    const ScalarPtr &left,
                                    const ScalarPtr &right) {
    return BinaryExpressionPtr(new BinaryExpression(operation, left, right));
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
  BinaryExpression(const BinaryOperation &operation,
                   const ScalarPtr &left,
                   const ScalarPtr &right);

  const BinaryOperation &operation_;

  ScalarPtr left_;
  ScalarPtr right_;

  DISALLOW_COPY_AND_ASSIGN(BinaryExpression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_BINARY_EXPRESSION_HPP_ */
