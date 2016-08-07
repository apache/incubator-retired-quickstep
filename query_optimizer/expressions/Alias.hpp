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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_ALIAS_HPP
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_ALIAS_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Scalar;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class Alias;
typedef std::shared_ptr<const Alias> AliasPtr;

/**
 * @brief Assigns an ID and a name to an expression so that the expression can be
 *        referenced by others. It corresponds to a new CatalogAttribute.
 */
class Alias : public NamedExpression {
 public:
  ExpressionType getExpressionType() const override { return ExpressionType::kAlias; }

  std::string getName() const override { return "Alias"; }

  const Type& getValueType() const override {
    return expression_->getValueType();
  }

  bool isConstant() const override { return expression_->isConstant(); }

  /**
   * @brief Returns the named expression.
   *
   * @return The expression named by this Alias.
   */
  inline const ExpressionPtr& expression() const { return expression_; }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return expression_->getReferencedAttributes();
  }

  ::quickstep::Scalar *concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Creates an immutable Alias. If \p expression is also an Alias,
   *        its child expression rather than the Alias is the child expression
   *        of the new Alias.
   *
   * @param id Expression ID.
   * @param expression Expression that the alias is applied on.
   * @param attribute_name The internal name (corresponding to the attribute
   *                       name if being concretized).
   * @param attribute_alias The display name (corresponding to the display name
   *                        if being concretized).
   * @param relation_name The name of the source relation.
   * @return An immutable Alias.
   */
  static AliasPtr Create(ExprId id,
                         const ExpressionPtr &expression,
                         const std::string &attribute_name,
                         const std::string &attribute_alias,
                         const std::string &relation_name = "") {
    return AliasPtr(new Alias(id,
                              expression,
                              attribute_name,
                              attribute_alias,
                              relation_name));
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
  Alias(ExprId id,
        const ExpressionPtr &expression,
        const std::string &attribute_name,
        const std::string &attribute_alais,
        const std::string &relation_name);

  ExpressionPtr expression_;

  DISALLOW_COPY_AND_ASSIGN(Alias);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_ALIAS_HPP */
