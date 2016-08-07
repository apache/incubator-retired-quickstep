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

#include "query_optimizer/expressions/ExpressionUtil.hpp"

#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"

namespace quickstep {
namespace optimizer {
namespace expressions {

AttributeReferencePtr ToRef(const NamedExpressionPtr &expression) {
  return AttributeReference::Create(expression->id(),
                                    expression->attribute_name(),
                                    expression->attribute_alias(),
                                    expression->relation_name(),
                                    expression->getValueType(),
                                    AttributeReferenceScope::kLocal);
}

std::vector<AttributeReferencePtr> GetNullableAttributeVector(
    const std::vector<AttributeReferencePtr> &attributes) {
  std::vector<AttributeReferencePtr> nullable_attributes;
  for (const auto &attr : attributes) {
    if (!attr->getValueType().isNullable()) {
      nullable_attributes.emplace_back(
          AttributeReference::Create(attr->id(),
                                     attr->attribute_name(),
                                     attr->attribute_alias(),
                                     attr->relation_name(),
                                     attr->getValueType().getNullableVersion(),
                                     attr->scope()));
    } else {
      nullable_attributes.emplace_back(attr);
    }
  }
  return nullable_attributes;
}

std::vector<AttributeReferencePtr> GetAttributeReferencesWithinScope(
    const std::vector<AttributeReferencePtr> &attributes,
    const AttributeReferenceScope scope) {
  std::vector<AttributeReferencePtr> filtered_attributes;
  for (const auto &attr_it : attributes) {
    if (attr_it->scope() == scope) {
      filtered_attributes.emplace_back(attr_it);
    }
  }
  return filtered_attributes;
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
