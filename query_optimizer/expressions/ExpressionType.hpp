/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_TYPE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_TYPE_HPP_

namespace quickstep {
namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

/**
 * @brief Optimizer expression types.
 **/
enum class ExpressionType {
  kAggregateFunction,
  kAlias,
  kAttributeReference,
  kBinaryExpression,
  kCast,
  kComparisonExpression,
  kLogicalAnd,
  kLogicalOr,
  kLogicalNot,
  kPredicateLiteral,
  kScalarLiteral,
  kUnaryExpression
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_EXPRESSION_TYPE_HPP_ */
