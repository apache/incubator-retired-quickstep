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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PATTERN_MATCHER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PATTERN_MATCHER_HPP_

#include <memory>
#include <type_traits>

#include "query_optimizer/expressions/ExpressionType.hpp"

namespace quickstep {
namespace optimizer {
namespace expressions {

class AggregateFunction;
class Alias;
class Avg;
class AttributeReference;
class BinaryExpression;
class Cast;
class CommonSubexpression;
class ComparisonExpression;
class Count;
class Exists;
class InTableQuery;
class InValueList;
class LogicalAnd;
class LogicalNot;
class LogicalOr;
class Max;
class Min;
class NamedExpression;
class Predicate;
class PredicateLiteral;
class Scalar;
class ScalarLiteral;
class SimpleCase;
class Sum;
class UnaryExpression;
class WindowAggregateFunction;

/** \addtogroup OptimizerExpressions
 *  @{
 */

/**
 * @brief Templated matcher for each Expression node class.
 *
 * @param ExpressionClass The Expression class for the expression node to be matched with.
 * @param expression_types All the expression types of the Expression class.
 */
template <class ExpressionClass, ExpressionType... expression_types>
class SomeExpressionNode {
 public:
  /**
   * @brief Checks whether the object managed in \p expression is an instance
   *        of the template argument ExpressionClass by checking whether
   *        it is one of types in the given template arguments expression_types.
   *
   * @param expression The expression node to be checked.
   * @return True for a match; otherwise false.
   */
  template <class OtherExpressionClass>
  static bool Matches(const std::shared_ptr<const OtherExpressionClass> &expression) {
    for (const ExpressionType expression_type : kExpressionTypes) {
      if (expression->getExpressionType() == expression_type) {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief Checks whether the object managed in \p expression is an instance
   *        of the template argument ExpressionClass by checking whether
   *        it is one of types in the given template arguments expression_types,
   *        If true, it additionally casts \p expression to a std::shared_ptr
   *        \p cast_expression of the template argument ExpressionClass.
   *
   * @param expression The expression node to be checked.
   * @param cast_expression The cast expression node.
   * @return True if the object managed in \p expression is an instance of ExpressionClass.
   */
  template <class OtherExpressionClass>
  static bool MatchesWithConditionalCast(
      const std::shared_ptr<const OtherExpressionClass> &expression,
      std::shared_ptr<const ExpressionClass> *cast_expression) {
    bool is_match = Matches(expression);
    if (is_match) {
      *cast_expression = std::static_pointer_cast<const ExpressionClass>(expression);
    }
    return is_match;
  }

 private:
  constexpr static ExpressionType kExpressionTypes[] = {expression_types...};
};

template <class ExpressionClass, ExpressionType... expression_types>
constexpr ExpressionType SomeExpressionNode<ExpressionClass, expression_types...>::kExpressionTypes[];

// Specializations for all Expression classes.

using SomeAlias = SomeExpressionNode<Alias, ExpressionType::kAlias>;
using SomeAttributeReference = SomeExpressionNode<AttributeReference, ExpressionType::kAttributeReference>;
using SomeBinaryExpression = SomeExpressionNode<BinaryExpression, ExpressionType::kBinaryExpression>;
using SomeCast = SomeExpressionNode<Cast, ExpressionType::kCast>;
using SomeComparisonExpression = SomeExpressionNode<ComparisonExpression, ExpressionType::kComparisonExpression>;
using SomeExists = SomeExpressionNode<Exists, ExpressionType::kExists>;
using SomeInTableQuery = SomeExpressionNode<InTableQuery, ExpressionType::kInTableQuery>;
using SomeInValueList = SomeExpressionNode<InValueList, ExpressionType::kInValueList>;
using SomeLogicalAnd = SomeExpressionNode<LogicalAnd, ExpressionType::kLogicalAnd>;
using SomeLogicalNot = SomeExpressionNode<LogicalNot, ExpressionType::kLogicalNot>;
using SomeLogicalOr = SomeExpressionNode<LogicalOr, ExpressionType::kLogicalOr>;
using SomeNamedExpression = SomeExpressionNode<NamedExpression,
                                               ExpressionType::kAttributeReference,
                                               ExpressionType::kAlias>;
using SomePredicate = SomeExpressionNode<Predicate,
                                         ExpressionType::kComparisonExpression,
                                         ExpressionType::kExists,
                                         ExpressionType::kInTableQuery,
                                         ExpressionType::kInValueList,
                                         ExpressionType::kLogicalAnd,
                                         ExpressionType::kLogicalNot,
                                         ExpressionType::kLogicalOr,
                                         ExpressionType::kPredicateLiteral>;
using SomePredicateLiteral = SomeExpressionNode<PredicateLiteral, ExpressionType::kPredicateLiteral>;
using SomeScalar = SomeExpressionNode<Scalar,
                                      ExpressionType::kAlias,
                                      ExpressionType::kAttributeReference,
                                      ExpressionType::kBinaryExpression,
                                      ExpressionType::kCast,
                                      ExpressionType::kCommonSubexpression,
                                      ExpressionType::kScalarLiteral,
                                      ExpressionType::kSearchedCase,
                                      ExpressionType::kSimpleCase,
                                      ExpressionType::kUnaryExpression>;
using SomeScalarLiteral = SomeExpressionNode<ScalarLiteral, ExpressionType::kScalarLiteral>;
using SomeSimpleCase = SomeExpressionNode<SimpleCase, ExpressionType::kSimpleCase>;
using SomeUnaryExpression = SomeExpressionNode<UnaryExpression, ExpressionType::kUnaryExpression>;
using SomeWindowAggregateFunction = SomeExpressionNode<WindowAggregateFunction,
                                                       ExpressionType::kWindowAggregateFunction>;

using SomeAggregateFunction = SomeExpressionNode<AggregateFunction,
                                                 ExpressionType::kAggregateFunction>;

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PATTERN_MATCHER_HPP_ */
