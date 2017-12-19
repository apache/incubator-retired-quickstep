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

#include "query_optimizer/expressions/InValueList.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/predicate/DisjunctionPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace expressions {

::quickstep::Predicate* InValueList::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  std::unique_ptr<quickstep::DisjunctionPredicate>
      disjunction_predicate(new quickstep::DisjunctionPredicate());
  for (const ScalarPtr &match_expression : match_expressions_) {
    const PredicatePtr match_predicate =
        ComparisonExpression::Create(
            quickstep::ComparisonFactory::GetComparison(quickstep::ComparisonID::kEqual),
            test_expression_,
            match_expression);

    disjunction_predicate->addPredicate(
        match_predicate->concretize(substitution_map));
  }
  return disjunction_predicate.release();
}

void InValueList::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("test_expression");
  non_container_child_fields->push_back(test_expression_);

  container_child_field_names->push_back("match_expressions");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(match_expressions_));
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
