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
    kArray,
    kAttribute,
    kBinaryExpression,
    kFunctionCall,
    kScalarLiteral,
    kSearchedCaseExpression,
    kSimpleCaseExpression,
    kSubqueryExpression,
    kTypeCast,
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
