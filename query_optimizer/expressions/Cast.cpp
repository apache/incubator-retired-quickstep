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

#include "query_optimizer/expressions/Cast.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarUnaryExpression.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/MetaType.hpp"
#include "types/Type.hpp"
#include "types/operations/OperationSignature.hpp"
#include "types/operations/OperationFactory.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "utility/HashPair.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

ExpressionPtr Cast::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  ScalarPtr scalar;
  CHECK(SomeScalar::MatchesWithConditionalCast(new_children[0], &scalar))
      << new_children[0]->toString();
  return Create(scalar, target_type_);
}

::quickstep::Scalar *Cast::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  const OperationSignaturePtr op_signature =
      OperationSignature::Create(
          "cast", {operand_->getValueType().getTypeID(), kMetaType}, 1);
  const UnaryOperationPtr cast_operation =
      OperationFactory::GetUnaryOperation(op_signature);

  std::vector<TypedValue> meta_type_value =
      { GenericValue::CreateWithLiteral(
            MetaType::InstanceNonNullable(), &target_type_).toTypedValue() };
  DCHECK(cast_operation->canApplyTo(operand_->getValueType(), meta_type_value));

  return new ::quickstep::ScalarUnaryExpression(
      op_signature, cast_operation, operand_->concretize(substitution_map),
      std::make_shared<const std::vector<TypedValue>>(std::move(meta_type_value)));
}

std::size_t Cast::computeHash() const {
  return CombineHashes(
      CombineHashes(static_cast<std::size_t>(ExpressionType::kCast),
                    operand_->hash()),
      static_cast<std::size_t>(target_type_.getTypeID()));
}

bool Cast::equals(const ScalarPtr &other) const {
  CastPtr expr;
  if (SomeCast::MatchesWithConditionalCast(other, &expr)) {
    return operand_->equals(expr->operand_) && target_type_.equals(expr->target_type_);
  }
  return false;
}

void Cast::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("target_type");
  inline_field_values->push_back(target_type_.getName());

  non_container_child_field_names->push_back("operand");
  non_container_child_fields->push_back(operand_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
