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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_LITERAL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_LITERAL_HPP_

#include <cstddef>
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
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class ScalarLiteral;
typedef std::shared_ptr<const ScalarLiteral> ScalarLiteralPtr;

/**
 * @brief A non-boolean scalar literal.
 */
class ScalarLiteral : public Scalar {
 public:
  std::string getName() const override {
    return "Literal";
  }

  ExpressionType getExpressionType() const override {
    return ExpressionType::kScalarLiteral;
  }

  const Type& getValueType() const override;

  bool isConstant() const override { return true; }

  /**
   * @return The literal value.
   */
  const TypedValue& value() const { return value_; }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
      const std::unordered_set<ExprId> &left_expr_ids = std::unordered_set<ExprId>(),
      const std::unordered_set<ExprId> &right_expr_ids = std::unordered_set<ExprId>()) const override;

  bool equals(const ScalarPtr &other) const override;

  /**
   * @brief Creates an immutable ScalarLiteral.
   * @param literal_value The literal value.
   * @return An immutable ScalarLiteral with the given literal value.
   */
  static const ScalarLiteralPtr Create(const TypedValue &literal_value,
                                       const Type &literal_value_type) {
    return ScalarLiteralPtr(new ScalarLiteral(literal_value, literal_value_type));
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
  ScalarLiteral(const TypedValue &literal_value,
                const Type &literal_value_type)
      : value_(literal_value),
        value_type_(literal_value_type) {}

  const TypedValue value_;
  const Type &value_type_;

  DISALLOW_COPY_AND_ASSIGN(ScalarLiteral);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_LITERAL_HPP_ */
