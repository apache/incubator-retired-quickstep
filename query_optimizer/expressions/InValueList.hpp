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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_IN_VALUE_LIST_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_IN_VALUE_LIST_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogAttribute;
class Predicate;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class InValueList;
typedef std::shared_ptr<const InValueList> InValueListPtr;

/**
 * @brief IN predicate with a value list.
 */
class InValueList : public Predicate {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kInValueList;
  }

  std::string getName() const override {
    return "InValueList";
  }

  bool isConstant() const override {
    return false;
  }

  /**
   * @return The expression to test with a value list.
   */
  const ScalarPtr& test_expression() const {
    return test_expression_;
  }

  /**
   * @return Expressions to search for the value of the test_expression.
   */
  const std::vector<ScalarPtr>& match_expressions() const {
    return match_expressions_;
  }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override {
    DCHECK_EQ(children().size(), new_children.size());
    std::vector<ScalarPtr> new_match_expressions;
    for (std::size_t idx = 1; idx < new_children.size(); ++idx) {
      new_match_expressions.emplace_back(
          std::static_pointer_cast<const Scalar>(new_children[idx]));
    }
    return Create(std::static_pointer_cast<const Scalar>(new_children[0]),
                  new_match_expressions);
  }

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    std::vector<AttributeReferencePtr> referenced_attrs =
        test_expression_->getReferencedAttributes();
    for (const ScalarPtr &match_expression : match_expressions_) {
      const std::vector<AttributeReferencePtr> referenced_attrs_in_match_expr =
          match_expression->getReferencedAttributes();
      referenced_attrs.insert(referenced_attrs.end(),
                              referenced_attrs_in_match_expr.begin(),
                              referenced_attrs_in_match_expr.end());
    }
    return referenced_attrs;
  }

  ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Create an IN predicate with a value list.
   *
   * @param test_expression The expression to test with a value list.
   * @param match_expressions Expressions to search for the value of the test_expression.
   *
   * @return An immutable IN predicate node with a value list.
   */
  static InValueListPtr Create(const ScalarPtr &test_expression,
                               const std::vector<ScalarPtr> &match_expressions) {
    return InValueListPtr(new InValueList(test_expression, match_expressions));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  InValueList(const ScalarPtr &test_expression,
              const std::vector<ScalarPtr> &match_expressions)
      : test_expression_(test_expression),
        match_expressions_(match_expressions) {
    addChild(test_expression_);
    for (const ScalarPtr &match_expression : match_expressions_) {
      addChild(match_expression);
    }
  }

  const ScalarPtr test_expression_;
  const std::vector<ScalarPtr> match_expressions_;

  DISALLOW_COPY_AND_ASSIGN(InValueList);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_IN_VALUE_LIST_HPP_
