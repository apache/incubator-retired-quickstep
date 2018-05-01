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

#include "query_optimizer/expressions/ScalarLiteral.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "expressions/scalar/ScalarLiteral.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "types/Type.hpp"
#include "utility/HashPair.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

const Type& ScalarLiteral::getValueType() const {
  return value_type_;
}

ExpressionPtr ScalarLiteral::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return ScalarLiteral::Create(value_, value_type_);
}

::quickstep::Scalar *ScalarLiteral::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
    const std::unordered_set<ExprId> &left_expr_ids,
    const std::unordered_set<ExprId> &right_expr_ids) const {
  return new ::quickstep::ScalarLiteral(value_, value_type_);
}

std::size_t ScalarLiteral::computeHash() const {
  std::size_t hash_code = static_cast<std::size_t>(ExpressionType::kScalarLiteral);
  if (!value_.isNull()) {
    hash_code = CombineHashes(hash_code, value_.getHash());
  }
  return hash_code;
}

bool ScalarLiteral::equals(const ScalarPtr &other) const {
  ScalarLiteralPtr lit;
  if (SomeScalarLiteral::MatchesWithConditionalCast(other, &lit) &&
      value_type_.equals(lit->value_type_)) {
    if (value_.isNull() || lit->value_.isNull()) {
      return value_.isNull() && lit->value_.isNull();
    }
    return value_.fastEqualCheck(lit->value_);
  }
  return false;
}

void ScalarLiteral::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("value");
  if (value_.isNull()) {
    inline_field_values->push_back("NULL");
  } else {
    inline_field_values->push_back(value_type_.printValueToString(value_));
  }

  inline_field_names->push_back("type");
  inline_field_values->push_back(getValueType().getName());
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
