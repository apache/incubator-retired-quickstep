/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/LogicalNot.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "expressions/predicate/NegationPredicate.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

ExpressionPtr LogicalNot::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  PredicatePtr operand;
  CHECK(SomePredicate::MatchesWithConditionalCast(new_children[0], &operand));
  return LogicalNot::Create(operand);
}

::quickstep::Predicate* LogicalNot::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  return new ::quickstep::NegationPredicate(
      operand_->concretize(substitution_map));
}

void LogicalNot::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(operand_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
