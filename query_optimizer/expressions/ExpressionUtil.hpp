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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_UTIL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_UTIL_HPP_

#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

/**
 * @brief ExprID comparator for two named expressions.
 */
struct NamedExpressionCompare {
  inline bool operator()(const NamedExpressionPtr &lhs,
                         const NamedExpressionPtr &rhs) const {
    return lhs->id() < rhs->id();
  }
};

/**
 * @brief Implements the equal function for named expressions.
 */
struct NamedExpressionEqual {
  inline bool operator()(const NamedExpressionPtr &lhs,
                         const NamedExpressionPtr &rhs) const {
    return lhs->id() == rhs->id();
  }
};

/**
 * @brief Implements the function function for named expressions.
 */
struct NamedExpressionHash {
  inline std::size_t operator()(const NamedExpressionPtr &named_expr) const {
    return std::hash<expressions::ExprId>()(named_expr->id());
  }
};

typedef std::unordered_set<NamedExpressionPtr, NamedExpressionHash, NamedExpressionEqual> UnorderedNamedExpressionSet;
typedef std::unordered_set<AttributeReferencePtr, NamedExpressionHash, NamedExpressionEqual> UnorderedAttributeSet;

/**
 * @brief Checks whether \p expression_to_check is contained by \p expressions.
 *
 * @param expressions A list of expressions to be checked with.
 * @param expression_to_check The expression that is to be checked if it is in
 *                            \p expressions.
 * @return True if \p expression_to_check is contained by \p expressions.
 */
template <class NamedExpressionType1, class NamedExpressionType2>
bool ContainsExpression(
    const std::vector<std::shared_ptr<const NamedExpressionType1>> &expressions,
    const std::shared_ptr<const NamedExpressionType2> &expression_to_check) {
  for (const std::shared_ptr<const NamedExpressionType1> &expression :
       expressions) {
    if (expression->equals(expression_to_check)) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Checks whether \p left is a subset of \p right.
 *
 * @param left The left operand of the subset operator (i.e. the one that may be
 *             a subset).
 * @param right The right operand of the subset operator (i.e. the one that may
 *              contain the other operand).
 * @return True if \p left is a subset of \p right.
 */
template <class NamedExpressionType>
bool SubsetOfExpressions(
    const std::vector<std::shared_ptr<const NamedExpressionType>> &left,
    const std::vector<std::shared_ptr<const NamedExpressionType>> &right) {
  UnorderedNamedExpressionSet supset(right.begin(), right.end());
  for (const std::shared_ptr<const NamedExpressionType> &expr : left) {
    if (supset.find(expr) == supset.end()) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Make a copy of the input attribute vector with each attribute's type
 *        converted as nullable.
 *
 * @param attributes The input attribute vector.
 * @return The nullable copy of the input attribute vector.
 */
std::vector<AttributeReferencePtr> GetNullableAttributeVector(
    const std::vector<AttributeReferencePtr> &attributes);

/**
 * @brief Returns a reference to this named expression.
 *
 * @return An AttributeReference of this named expression.
 */
AttributeReferencePtr ToRef(const NamedExpressionPtr &expression);

/**
 * @brief Given a vector of expressions and a vector of attributes, return a
 *        bool vector that indicates whether each expression is refering to
 *        ANY attribute in the attribute vector.
 *
 * @param expressions The vector of expressions to be checked.
 * @param attributes The vector of attributes to be checked.
 * @return A vector of bools indicating whether each expression is refering to
 *         any attribute in the attribute vector. Note that the length of this
 *         bool vector equals the length of \p expressions.
 */
template <class NamedExpressionType>
std::vector<bool> MarkExpressionsReferingAnyAttribute(
    const std::vector<std::shared_ptr<const NamedExpressionType>> &expressions,
    const std::vector<AttributeReferencePtr> &attributes) {
  std::vector<bool> matches;
  UnorderedAttributeSet attr_set(attributes.begin(), attributes.end());
  for (std::size_t i = 0; i < expressions.size(); ++i) {
    for (const auto referenced_attr : expressions[i]->getReferencedAttributes()) {
      if (attr_set.find(referenced_attr) != attr_set.end()) {
        matches.emplace_back(true);
      } else {
        matches.emplace_back(false);
      }
    }
  }
  return matches;
}

/**
 * @brief Filter a vector of AttributeReferencePtr to get those which are in
 *        the specified scope.
 *
 * @param attributes The vector of AttributeReferencePtr to be filtered.
 * @param scope The specified reference scope.
 *
 * @return A vector of AttributeReferencePtr within the specified scope.
 */
std::vector<AttributeReferencePtr> GetAttributeReferencesWithinScope(
    const std::vector<AttributeReferencePtr> &attributes,
    const AttributeReferenceScope scope);

/**
 * @brief Creates a list of AttributeReferences from a list of NamedExpressions
 *        with each AttributeReference referencing a distinct NamedExpression.
 *        The input and the output have 1:1 matching.
 *
 * @param expressions A list of NamedExpressions to be converted to
 *                    AttributeReferences.
 * @return AttributeReferences for the NamedExpressions.
 */
template <class NamedExpressionType>
std::vector<AttributeReferencePtr> ToRefVector(
    const std::vector<std::shared_ptr<const NamedExpressionType>> &expressions) {
  std::vector<AttributeReferencePtr> cast_expressions;
  for (const std::shared_ptr<const NamedExpressionType> &expression :
       expressions) {
    cast_expressions.push_back(ToRef(expression));
  }
  return cast_expressions;
}

/**
 * @brief Casts a list of Expressions to a list of NamedExpressions.
 *
 * @param expressions A list of expressions to be cast to NamedExpression. The
 *                    expressions must be of type NamedExpression.
 * @return A list of casted NamedExpressions.
 */
template <class ExpressionType>
std::vector<NamedExpressionPtr> ToNamedExpressions(
    const std::vector<std::shared_ptr<const ExpressionType>> &expressions) {
  std::vector<NamedExpressionPtr> cast_expressions;
  for (const std::shared_ptr<const ExpressionType> &expression : expressions) {
    NamedExpressionPtr cast_expression;
    CHECK(SomeNamedExpression::MatchesWithConditionalCast(expression, &cast_expression))
        << expression->toString();
    cast_expressions.push_back(cast_expression);
  }
  return cast_expressions;
}

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_UTIL_HPP_ */
