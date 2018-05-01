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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SEARCHED_CASE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SEARCHED_CASE_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "expressions/scalar/Scalar.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class SearchedCase;
typedef std::shared_ptr<const SearchedCase> SearchedCasePtr;

/**
 * @brief A searched CASE expression. It searches the first predicate in
 *        <condition_predicates_> that evaluates to true and returns the value
 *        of the expression in <conditional_result_expressions_> at the same
 *        position; if none of <condition_predicates_> evaluate to true, returns
 *        the value of <else_result_expression_> when <else_result_expression_>
 *        is not NULL, otherwise returns NULL.
 */
class SearchedCase : public Scalar {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kSearchedCase;
  }

  std::string getName() const override {
    return "SearchedCase";
  }

  const Type& getValueType() const override {
    return value_type_;
  }

  /**
   * @return The vector of condition predicates.
   */
  const std::vector<PredicatePtr>& condition_predicates() const {
    return condition_predicates_;
  }

  /**
   * @return The vector of conditional result expressions.
   */
  const std::vector<ScalarPtr>& conditional_result_expressions() const {
    return conditional_result_expressions_;
  }

  /**
   * @return The ELSE result expression.
   */
  const ScalarPtr& else_result_expression() const {
    return else_result_expression_;
  }

  bool isConstant() const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override;

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
      const std::unordered_set<ExprId> &left_expr_ids = std::unordered_set<ExprId>(),
      const std::unordered_set<ExprId> &right_expr_ids = std::unordered_set<ExprId>()) const override;

  /**
   * @brief Creates an immutable SearchedCase.
   *
   * @param condition_predicates A vector of condition predicates.
   * @param conditional_result_expressions A vector of result expressions, one
   *        for each condition predicate.
   * @param else_result_expression The optional ELSE expression.
   * @param value_type The data type of this expression which should be the
   *        unified type of all result expressions.
   * @return An immutable SimpleCase.
   */
  static SearchedCasePtr Create(const std::vector<PredicatePtr> &condition_predicates,
                                const std::vector<ScalarPtr> &conditional_result_expressions,
                                const ScalarPtr &else_result_expression,
                                const Type &value_type) {
    return SearchedCasePtr(new SearchedCase(condition_predicates,
                                            conditional_result_expressions,
                                            else_result_expression,
                                            value_type));
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  SearchedCase(const std::vector<PredicatePtr> &condition_predicates,
               const std::vector<ScalarPtr> &conditional_result_expressions,
               const ScalarPtr &else_result_expression,
               const Type &value_type)
      : condition_predicates_(condition_predicates),
        conditional_result_expressions_(conditional_result_expressions),
        else_result_expression_(else_result_expression),
        value_type_(value_type) {
    for (const PredicatePtr &condition_predicate : condition_predicates_) {
      addChild(condition_predicate);
    }
    for (const ScalarPtr &conditional_result_expression : conditional_result_expressions_) {
      addChild(conditional_result_expression);
    }
    if (else_result_expression_ != nullptr) {
      addChild(else_result_expression_);
    }
  }

  const std::vector<PredicatePtr> condition_predicates_;
  const std::vector<ScalarPtr> conditional_result_expressions_;

  // May be NULL.
  const ScalarPtr else_result_expression_;

  const Type &value_type_;

  DISALLOW_COPY_AND_ASSIGN(SearchedCase);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SEARCHED_CASE_HPP_ */
