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
#include "query_optimizer/logical/Filter.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

Filter::Filter(const LogicalPtr &input,
               const E::PredicatePtr &filter_predicate) {
  FilterPtr child_filter;
  LogicalPtr flattened_input;

  // Flatten the Filer on top of another Filter
  if (SomeFilter::MatchesWithConditionalCast(input, &child_filter)) {
    flattened_input = child_filter->input();
    filter_predicate_ = E::LogicalAnd::Create(
        {filter_predicate, child_filter->filter_predicate()} /* operands */);
  } else {
    flattened_input = input;
    filter_predicate_ = filter_predicate;
  }
  addChild(flattened_input);
  addInputExpression(filter_predicate_);
}

LogicalPtr Filter::copyWithNewInputExpressions(
    const std::vector<expressions::ExpressionPtr> &input_expressions) const {
  DCHECK_EQ(1u, input_expressions.size());

  E::PredicatePtr new_filter_predicate;
  E::SomePredicate::MatchesWithConditionalCast(input_expressions[0], &new_filter_predicate);
  DCHECK(new_filter_predicate != nullptr);

  return Create(children()[0], new_filter_predicate);
}

void Filter::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_field_names->push_back("filter_predicate");

  non_container_child_fields->push_back(input());
  non_container_child_fields->push_back(filter_predicate_);
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
