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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_IN_TABLE_QUERY_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_IN_TABLE_QUERY_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/expressions/SubqueryExpression.hpp"
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

class InTableQuery;
typedef std::shared_ptr<const InTableQuery> InTableQueryPtr;

/**
 * @brief IN predicate with a subquery.
 */
class InTableQuery : public Predicate {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kInTableQuery;
  }

  std::string getName() const override {
    return "InTableQuery";
  }

  bool isConstant() const override {
    return false;
  }

  /**
   * @return The expression to test with the result of the table subquery.
   */
  const ScalarPtr& test_expression() const {
    return test_expression_;
  }

  /**
   * @return The table subquery that returns a single column to search for
   *         the value of the test expression.
   */
  const SubqueryExpressionPtr& table_query() const {
    return table_query_;
  }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override {
    DCHECK_EQ(2u, new_children.size());
    return Create(std::static_pointer_cast<const Scalar>(new_children[0]),
                  std::static_pointer_cast<const SubqueryExpression>(new_children[1]));
  }

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
      const std::unordered_set<ExprId> &left_expr_ids = std::unordered_set<ExprId>(),
      const std::unordered_set<ExprId> &right_expr_ids = std::unordered_set<ExprId>()) const override;

  /**
   * @brief Create an IN predicate with a subquery.
   *
   * @param test_expression The expression to test with the result of a table subquery.
   * @param table_query The table subquery that returns a single column to search for
   *        the value of the test expression.
   *
   * @return An immutable IN predicate node.
   */
  static InTableQueryPtr Create(const ScalarPtr &test_expression,
                                const SubqueryExpressionPtr &table_query) {
    return InTableQueryPtr(new InTableQuery(test_expression, table_query));
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
  InTableQuery(const ScalarPtr &test_expression,
               const SubqueryExpressionPtr &table_query)
      : test_expression_(test_expression),
        table_query_(table_query) {
    addChild(test_expression_);
    addChild(table_query_);
  }

  const ScalarPtr test_expression_;
  const SubqueryExpressionPtr table_query_;

  DISALLOW_COPY_AND_ASSIGN(InTableQuery);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_IN_TABLE_QUERY_HPP_
