/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_EXPRESSION_HPP_
#define QUICKSTEP_PARSER_PARSE_EXPRESSION_HPP_

#include <string>

#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representation of an expression.
 **/
class ParseExpression : public ParseTreeNode {
 public:
  enum ExpressionType {
    kAttribute,
    kBinaryExpression,
    kFunctionCall,
    kScalarLiteral,
    kSubqueryExpression,
    kUnaryExpression,
  };

  /**
   * @brief Virtual destructor.
   **/
  ~ParseExpression() override {
  }

  /**
   * @return The expression type.
   */
  virtual ExpressionType getExpressionType() const = 0;

  /**
   * @brief Get a human-readable representation of this expression.
   *
   * @return The human-readable form of this expression.
   **/
  virtual std::string generateName() const = 0;

 protected:
  ParseExpression(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseExpression);
};


}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSE_EXPRESSION_HPP_ */
