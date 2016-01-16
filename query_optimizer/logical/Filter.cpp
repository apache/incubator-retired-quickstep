/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/
#include "query_optimizer/logical/Filter.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/logical/PatternMatcher.hpp"

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
