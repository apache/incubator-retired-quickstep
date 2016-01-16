/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
class ComparisonExpression;
class Count;
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
class Sum;
class UnaryExpression;

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
using SomeLogicalAnd = SomeExpressionNode<LogicalAnd, ExpressionType::kLogicalAnd>;
using SomeLogicalNot = SomeExpressionNode<LogicalNot, ExpressionType::kLogicalNot>;
using SomeLogicalOr = SomeExpressionNode<LogicalOr, ExpressionType::kLogicalOr>;
using SomeNamedExpression = SomeExpressionNode<NamedExpression,
                                               ExpressionType::kAttributeReference,
                                               ExpressionType::kAlias>;
using SomePredicate = SomeExpressionNode<Predicate,
                                         ExpressionType::kComparisonExpression,
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
                                      ExpressionType::kComparisonExpression,
                                      ExpressionType::kLogicalAnd,
                                      ExpressionType::kLogicalNot,
                                      ExpressionType::kLogicalOr,
                                      ExpressionType::kPredicateLiteral,
                                      ExpressionType::kScalarLiteral,
                                      ExpressionType::kUnaryExpression>;
using SomeScalarLiteral = SomeExpressionNode<ScalarLiteral, ExpressionType::kScalarLiteral>;
using SomeUnaryExpression = SomeExpressionNode<UnaryExpression, ExpressionType::kUnaryExpression>;

using SomeAggregateFunction = SomeExpressionNode<AggregateFunction,
                                                 ExpressionType::kAggregateFunction>;

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PATTERN_MATCHER_HPP_ */
