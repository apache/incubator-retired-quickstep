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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXISTS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXISTS_HPP_

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

class Exists;
typedef std::shared_ptr<const Exists> ExistsPtr;

/**
 * @brief EXISTS predicate expression.
 */
class Exists : public Predicate {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kExists;
  }

  std::string getName() const override {
    return "Exists";
  }

  bool isConstant() const override {
    return false;
  }

  /**
   * @return The subquery expression for this EXISTS predicate.
   */
  const SubqueryExpressionPtr& exists_subquery() const {
    return exists_subquery_;
  }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override {
    DCHECK_EQ(1u, new_children.size());
    return Create(std::static_pointer_cast<const SubqueryExpression>(new_children[0]));
  }

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Create an Exists predicate expression.
   *
   * @param exists_subquery The subquery expression for this EXISTS predicate.
   * @return An immutable Exists expression node.
   */
  static ExistsPtr Create(const SubqueryExpressionPtr &exists_subquery) {
    return ExistsPtr(new Exists(exists_subquery));
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
  explicit Exists(const SubqueryExpressionPtr &exists_subquery)
      : exists_subquery_(exists_subquery) {
    addChild(exists_subquery_);
  }

  const SubqueryExpressionPtr exists_subquery_;

  DISALLOW_COPY_AND_ASSIGN(Exists);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep


#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXISTS_HPP_ */
