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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SUBQUERY_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SUBQUERY_EXPRESSION_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogAttribute;
class Scalar;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class SubqueryExpression;
typedef std::shared_ptr<const SubqueryExpression> SubqueryExpressionPtr;

/**
 * @brief A subquery used in an expression.
 */
class SubqueryExpression : public Scalar {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kSubqueryExpression;
  }

  std::string getName() const override { return "SubqueryExpression"; }

  const Type& getValueType() const override {
    return output_attribute_->getValueType();
  }

  bool isConstant() const override {
    return output_attribute_->isConstant();
  }

  /**
   * @return The referenced logical subquery node.
   */
  const logical::LogicalPtr& subquery() const {
    return subquery_;
  }

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override;

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override {
    DCHECK(new_children.empty());
    return Create(subquery_);
  }

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
      const std::unordered_set<ExprId> &left_expr_ids = std::unordered_set<ExprId>(),
      const std::unordered_set<ExprId> &right_expr_ids = std::unordered_set<ExprId>()) const override;

  /**
   * @brief Creates a subquery expression.
   * @note This expression can only be used in a logical plan.
   *
   * @param subquery The logical subquery node.
   * @return An immutable SubqueryExpression.
   */
  static SubqueryExpressionPtr Create(const logical::LogicalPtr &subquery) {
    return SubqueryExpressionPtr(new SubqueryExpression(subquery));
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
  explicit SubqueryExpression(const logical::LogicalPtr &subquery)
      : subquery_(subquery),
        output_attribute_(subquery->getOutputAttributes()[0]) {
    DCHECK(!subquery->getOutputAttributes().empty());
  }

  const logical::LogicalPtr subquery_;
  // Set to the first output attribute if the subquery is a multi-column table query.
  const AttributeReferencePtr output_attribute_;

  DISALLOW_COPY_AND_ASSIGN(SubqueryExpression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SUBQUERY_EXPRESSION_HPP_ */
