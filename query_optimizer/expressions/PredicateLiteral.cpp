/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/PredicateLiteral.hpp"

#include <string>
#include <unordered_map>
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
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
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
