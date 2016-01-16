/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <vector>
#include <unordered_map>

#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/rules/UpdateExpression.hpp"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;

std::vector<expressions::PredicatePtr> GetConjunctivePredicates(
    const expressions::PredicatePtr &predicate) {
  expressions::LogicalAndPtr logical_and;
  if (expressions::SomeLogicalAnd::MatchesWithConditionalCast(predicate, &logical_and)) {
    return logical_and->operands();
  } else {
    return {predicate};
  }
}

void PullUpProjectExpressions(
    const std::vector<E::NamedExpressionPtr> &project_expressions_for_pullup,
    const std::vector<std::vector<E::ExpressionPtr>*> &non_project_expression_lists,
    const std::vector<std::vector<E::NamedExpressionPtr>*> &project_expression_lists) {
  std::unordered_map<E::ExprId, E::ExpressionPtr> expression_replacement_map;
  for (const E::NamedExpressionPtr &named_expression :
       project_expressions_for_pullup) {
    E::AliasPtr alias;
    if (E::SomeAlias::MatchesWithConditionalCast(named_expression, &alias)) {
      const E::ExpressionPtr replacement_expression = alias->expression();
      expression_replacement_map.emplace(alias->id(), replacement_expression);
    }
  }

  if (!expression_replacement_map.empty()) {
    UpdateExpression rule(expression_replacement_map);

    // For non-project expressions, simply apply the UpdateExpression rule.
    for (std::vector<E::ExpressionPtr> *non_project_expressions :
         non_project_expression_lists) {
      for (std::vector<E::ExpressionPtr>::size_type i = 0; i < non_project_expressions->size(); ++i) {
        const E::ExpressionPtr &expression_to_update = (*non_project_expressions)[i];
        (*non_project_expressions)[i] = rule.apply(expression_to_update);
      }
    }

    for (std::vector<E::NamedExpressionPtr> *project_expressions :
         project_expression_lists) {
      for (std::vector<E::NamedExpressionPtr>::size_type i = 0; i < project_expressions->size(); ++i) {
        const E::NamedExpressionPtr &expression_to_update = (*project_expressions)[i];
        E::ExpressionPtr updated_expression = rule.apply(expression_to_update);

        E::NamedExpressionPtr updated_named_expression;
        // If the updated expression is not a NamedExpression or we have changed
        // the ID, restore the ID by adding an Alias.
        if (!E::SomeNamedExpression::MatchesWithConditionalCast(updated_expression,
                                                                &updated_named_expression) ||
            expression_to_update->id() != updated_named_expression->id()) {
          (*project_expressions)[i] =
              E::Alias::Create(expression_to_update->id(),
                               updated_expression,
                               expression_to_update->attribute_name(),
                               expression_to_update->attribute_alias(),
                               expression_to_update->relation_name());
        } else {
          (*project_expressions)[i] = updated_named_expression;
        }
      }
    }
  }
}

}  // namespace optimizer
}  // namespace quickstep
