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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_UNARY_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_UNARY_EXPRESSION_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/GenericValue.hpp"
#include "types/operations/OperationSignature.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
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

class UnaryExpression;
typedef std::shared_ptr<const UnaryExpression> UnaryExpressionPtr;

class UnaryExpression : public Scalar {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kUnaryExpression;
  }

  std::string getName() const override;

  bool isConstant() const override { return operand_->isConstant(); }

  /**
   * @return The unary operator.
   */
  const UnaryOperationPtr& operation() const { return operation_; }

  /**
   * @return The operand of the unary operator.
   */
  const ScalarPtr& operand() const { return operand_; }

  const Type& getValueType() const override {
    return result_type_;
  }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return operand_->getReferencedAttributes();
  }

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  bool equals(const ScalarPtr &other) const override;

  /**
   * @brief Creates an immutable UnaryExpression.
   *
   * @param operation The unary operation.
   * @param operand The operand.
   * @return An immutable UnaryExpression that applies the operation to the
   *         operand.
   */
  static UnaryExpressionPtr Create(
      const OperationSignaturePtr &op_signature,
      const UnaryOperationPtr &operation,
      const ScalarPtr &operand,
      const std::shared_ptr<const std::vector<GenericValue>> &static_arguments) {
    return UnaryExpressionPtr(
        new UnaryExpression(op_signature,
                            operation,
                            operand,
                            static_arguments));
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
  UnaryExpression(const OperationSignaturePtr &op_signature,
                  const UnaryOperationPtr &operation,
                  const ScalarPtr &operand,
                  const std::shared_ptr<const std::vector<GenericValue>> &static_arguments)
      : op_signature_(op_signature),
        operation_(operation),
        operand_(operand),
        static_arguments_(static_arguments),
        static_arguments_cache_(ToTypedValue(*static_arguments_)),
        result_type_(*(operation_->getResultType(operand_->getValueType(),
                                                 *static_arguments_cache_))) {
    addChild(operand);
  }

  const OperationSignaturePtr op_signature_;
  const UnaryOperationPtr operation_;
  const ScalarPtr operand_;
  const std::shared_ptr<const std::vector<GenericValue>> static_arguments_;
  // TODO(refactor-type): Remove this.
  const std::shared_ptr<const std::vector<TypedValue>> static_arguments_cache_;
  const Type &result_type_;

  DISALLOW_COPY_AND_ASSIGN(UnaryExpression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_UNARY_EXPRESSION_HPP_ */
