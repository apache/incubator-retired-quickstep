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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_COMMON_SUBEXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_COMMON_SUBEXPRESSION_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class Scalar;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class CommonSubexpression;
typedef std::shared_ptr<const CommonSubexpression> CommonSubexpressionPtr;

/**
 * @brief A wrapper expression that represents a common subexpression.
 */
class CommonSubexpression : public Scalar {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kCommonSubexpression;
  }

  std::string getName() const override {
    return "CommonSubexpression";
  }

  bool isConstant() const override {
    return operand_->isConstant();
  }

  /**
   * @return The unique ID for the equivalence class that this common subexpression
   *         belongs to.
   */
  inline ExprId common_subexpression_id() const {
    return common_subexpression_id_;
  }

  /**
   * @return The operand that represents the underlying subexpression.
   */
  const ScalarPtr& operand() const {
    return operand_;
  }

  const Type& getValueType() const override {
    return operand_->getValueType();
  }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return operand_->getReferencedAttributes();
  }

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Creates an immutable CommonSubexpression.
   *
   * @param common_subexpression_id A unique ID for the equivalence class that
   *        this common subexpressions belongs to.
   * @param operand The operand that represents the underlying subexpression.
   * @return An immutable CommonSubexpression.
   */
  static CommonSubexpressionPtr Create(const ExprId common_subexpression_id,
                                       const ScalarPtr &operand) {
    return CommonSubexpressionPtr(
        new CommonSubexpression(common_subexpression_id, operand));
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
  CommonSubexpression(const ExprId common_subexpression_id,
                      const ScalarPtr &operand)
      : common_subexpression_id_(common_subexpression_id),
        operand_(operand) {
    addChild(operand);
  }

  ExprId common_subexpression_id_;
  ScalarPtr operand_;

  DISALLOW_COPY_AND_ASSIGN(CommonSubexpression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_COMMON_SUBEXPRESSION_HPP_
