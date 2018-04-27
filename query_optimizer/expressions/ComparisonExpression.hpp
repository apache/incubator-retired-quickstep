/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_COMPARISON_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_COMPARISON_EXPRESSION_HPP_

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
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Comparison;
class Predicate;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class ComparisonExpression;
typedef std::shared_ptr<const ComparisonExpression> ComparisonExpressionPtr;

/**
 * @brief Applies a comparison operator to two scalar expressions and returns a
 *        bool value.
 */
class ComparisonExpression : public Predicate {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kComparisonExpression;
  }

  bool isConstant() const override {
    return left_->isConstant() && right_->isConstant();
  }

  std::string getName() const override;

  /**
   * @return The comparison operator.
   */
  const Comparison& comparison() const { return comparison_; }

  /**
   * @return The left operand.
   */
  const ScalarPtr& left() const { return left_; }

  /**
   * @return The right operand.
   */
  const ScalarPtr& right() const { return right_; }

  bool isEqualityComparisonPredicate() const;

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override;

  ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Creates an immutable ComparisonExpression.
   *
   * @param comparison The comparison operator.
   * @param left The left operand.
   * @param right The right operand.
   * @return An immutable ComparisonExpression.
   */
  static ComparisonExpressionPtr Create(const Comparison &comparison,
                                        const ScalarPtr &left,
                                        const ScalarPtr &right) {
    return ComparisonExpressionPtr(
        new ComparisonExpression(comparison, left, right));
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
  ComparisonExpression(const Comparison &comparison,
                       const ScalarPtr &left,
                       const ScalarPtr &right);

  const Comparison &comparison_;
  const ScalarPtr left_;
  const ScalarPtr right_;

  DISALLOW_COPY_AND_ASSIGN(ComparisonExpression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_COMPARISON_EXPRESSION_HPP_ */
