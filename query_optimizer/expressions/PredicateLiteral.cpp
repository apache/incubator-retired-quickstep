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

#include "query_optimizer/expressions/PredicateLiteral.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "expressions/predicate/TrivialPredicates.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

ExpressionPtr PredicateLiteral::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return PredicateLiteral::Create(is_true_);
}

::quickstep::Predicate* PredicateLiteral::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
    const std::unordered_set<ExprId> &left_expr_ids,
    const std::unordered_set<ExprId> &right_expr_ids) const {
  if (is_true()) {
    return new TruePredicate();
  } else {
    return new FalsePredicate();
  }
}

void PredicateLiteral::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("value");
  inline_field_values->push_back(is_true_ ? "true" : "false");
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
