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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_RULE_HELPER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_RULE_HELPER_HPP_

#include <vector>

#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Returns the operand predicates if \p predicate is a conjunction,
 *        otherwise returns \p predicate itself.
 *
 * @param predicate The input predicate.
 * @return The operand predicates if \p predicate is a conjunction;
 *         otherwise returns \p predicate itself.
 */
std::vector<expressions::PredicatePtr> GetConjunctivePredicates(
    const expressions::PredicatePtr &predicate);

/**
 * @brief Updates the expressions in \p non_project_expression_lists
 *        and \p project_expression_list by replacing the attribute
 *        references to the Aliases in \p project_expressions_for_pullup
 *        with the child expressions (i.e. the referent) of the Aliases.
 *        For project expressions, the expression IDs remain unchanged.
 *        For example, assume a project expression 'attr1+attr2' in
 *        project_expression_lists and assume that 'attr1' is defined in
 *        project_expressions_for_pullup as '1+1'.
 *        The new project expression after the 'pulling up'
 *        \p project_expressions_for_pullup is '1+1+attr2'.
 *        This is used when we want to merge two projects
 *        or pull up selections.
 *
 * @param project_expressions_for_pullup The project expressions to to be pulled up.
 * @param non_project_expression_lists Non-project expressions to be updated. Any reference
 *                                     to an Alias in project_expressions_for_pullup is to
 *                                     be replaced with the child expression of the Alias.
 * @param project_expression_lists Project expressions to be updated. The expression IDs are not changed.
 */
void PullUpProjectExpressions(
    const std::vector<expressions::NamedExpressionPtr> &project_expressions_for_pullup,
    const std::vector<std::vector<expressions::ExpressionPtr>*> &non_project_expression_lists,
    const std::vector<std::vector<expressions::NamedExpressionPtr>*> &project_expression_lists);

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_RULE_HELPER_HPP_ */
