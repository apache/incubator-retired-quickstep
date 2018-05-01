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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_CAST_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_CAST_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogAttribute;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class Cast;
typedef std::shared_ptr<const Cast> CastPtr;

/**
 * @brief Converts a value of a type to another type.
 */
class Cast : public Scalar {
 public:
  ExpressionType getExpressionType() const override { return ExpressionType::kCast; }

  std::string getName() const override { return "Cast"; }

  const Type& getValueType() const override { return target_type_; }

  bool isConstant() const override { return operand_->isConstant(); }

  /**
   * @return The expression to be coerced.
   */
  const ScalarPtr& operand() const { return operand_; }

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return operand_->getReferencedAttributes();
  }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
      const std::unordered_set<ExprId> &left_expr_ids = std::unordered_set<ExprId>(),
      const std::unordered_set<ExprId> &right_expr_ids = std::unordered_set<ExprId>()) const override;

  bool equals(const ScalarPtr &other) const override;

  /**
   * @brief Creates a Cast expression that converts \p operand to \p target_type.
   *
   * @param operand The input expression to be coerced.
   * @param target_type The target type that the expression is converted to.
   * @return A Cast expression.
   */
  static CastPtr Create(const ScalarPtr &operand, const Type &target_type) {
    return CastPtr(new Cast(operand, target_type));
  }

 protected:
  std::size_t computeHash() const override;

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  Cast(const ScalarPtr &operand, const Type &target_type)
      : operand_(operand),
        target_type_(target_type) {
    addChild(operand);
    DCHECK(target_type.isCoercibleFrom(operand->getValueType()));
  }

  const ScalarPtr operand_;
  const Type &target_type_;

  DISALLOW_COPY_AND_ASSIGN(Cast);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_CAST_HPP_ */
