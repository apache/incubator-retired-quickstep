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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_OR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_OR_HPP_

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
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogAttribute;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class LogicalOr;
typedef std::shared_ptr<const LogicalOr> LogicalOrPtr;

/**
 * @brief A logical disconjunction (OR) of a list of predicate
 * operands.
 */
class LogicalOr : public Predicate {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kLogicalOr;
  }

  std::string getName() const override {
    return "Or";
  }

  // FIXME(qzeng): isConstant() is not strictly well-defined.
  // We do not determine the constant value for each expression.
  // A logical AND or OR is considered as not constant
  // only if any predicate is not constant, even it may have
  // a static value.
  bool isConstant() const override {
    for (const PredicatePtr &operand : operands_) {
      if (!operand->isConstant()) {
        return false;
      }
    }
    return true;
  }

  /**
   * @return Operands that the logical OR is applied on.
   */
  const std::vector<PredicatePtr>& operands() const { return operands_; }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override;

  ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Creates an immutable LogicalOr. If any operand is also a LogicalOr,
   *        the operand is pulled out and merged into the returned predicate
   *        (i.e. nested OR expressions are flattened).
   *
   * @param operands The operands to be applied to.
   * @return An immutable LogicalOr.
   */
  static LogicalOrPtr Create(const std::vector<PredicatePtr> &operands) {
    DCHECK_GT(operands.size(), 1u);
    return LogicalOrPtr(new LogicalOr(operands));
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
  explicit LogicalOr(const std::vector<PredicatePtr> &operands);

  std::vector<PredicatePtr> operands_;
  DISALLOW_COPY_AND_ASSIGN(LogicalOr);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_OR_HPP_ */
