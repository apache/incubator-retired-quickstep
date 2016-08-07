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

#ifndef QUICKSTEP_PARSER_PARSE_CASE_EXPRESSIONS_HPP_
#define QUICKSTEP_PARSER_PARSE_CASE_EXPRESSIONS_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representation of a WHEN clause in a simple CASE
 *        expression (WHEN <condition operand> THEN <result expression>).
 */
class ParseSimpleWhenClause : public ParseTreeNode {
 public:
  /**
   * @brief Constructor. Takes ownership of all pointers.
   *
   * @param line_number The line number of "WHEN" in the SQL statement.
   * @param column_number The column number of "WHEN" in the SQL statement.
   * @param check_operand The condition operand to be compared with the
   *                      CASE operand (not in this class) of the CASE
   *                      expression.
   * @param result_expression The result expression for this condition.
   */
  ParseSimpleWhenClause(int line_number,
                        int column_number,
                        ParseExpression *condition_operand,
                        ParseExpression *result_expression)
      : ParseTreeNode(line_number, column_number),
        condition_operand_(condition_operand),
        result_expression_(result_expression) {
  }

  std::string getName() const override {
    return "SimpleWhenClause";
  }

  /**
   * @return The condition operand.
   */
  const ParseExpression* condition_operand() const {
    return condition_operand_.get();
  }

  /**
   * @return The result expression for this condition.
   */
  const ParseExpression* result_expression() const {
    return result_expression_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseExpression> condition_operand_;
  std::unique_ptr<ParseExpression> result_expression_;

  DISALLOW_COPY_AND_ASSIGN(ParseSimpleWhenClause);
};

/**
 * @brief The parsed representation of a WHEN clause in a searched CASE
 *        expression (WHEN <condition predicate> THEN <result expression>).
 *
 */
class ParseSearchedWhenClause : public ParseTreeNode {
 public:
  /**
   * @brief Constructor. Takes ownership of all pointers.
   *
   * @param line_number The line number of "WHEN" in the SQL statement.
   * @param column_number The column number of "WHEN" in the SQL statement.
   * @param condition_predicate The condition predicate.
   * @param result_expression The result expression for this condition.
   */
  ParseSearchedWhenClause(int line_number,
                          int column_number,
                          ParsePredicate *condition_predicate,
                          ParseExpression *result_expression)
      : ParseTreeNode(line_number, column_number),
        condition_predicate_(condition_predicate),
        result_expression_(result_expression) {
  }

  std::string getName() const override {
    return "SearchedWhenClause";
  }

  /**
   * @return The condition predicate.
   */
  const ParsePredicate* condition_predicate() const {
    return condition_predicate_.get();
  }

  /**
   * @return The result expression.
   */
  const ParseExpression* result_expression() const {
    return result_expression_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParsePredicate> condition_predicate_;
  std::unique_ptr<ParseExpression> result_expression_;

  DISALLOW_COPY_AND_ASSIGN(ParseSearchedWhenClause);
};

/**
 * @brief The parsed representation of a simple CASE expression:
 *           CASE <case operand>
 *               WHEN <condition_operand> THEN <result_expression>
 *               [...n]
 *               [ELSE <else_result_expression>]
 *           END
 *        It returns the <result_expression> of the first <case operand> = <when_operand>
 *        that evaluates to true; if none is found and <else_result_expression> exists,
 *        returns <else_result_expression>; otherwise, returns NULL.
 **/
class ParseSimpleCaseExpression : public ParseExpression {
 public:
  /**
   * @brief Constructor. Takes ownership of all pointers.
   *
   * @param line_number The line number of "CASE" in the SQL statement.
   * @param column_number The column number of "CASE" in the SQL statement.
   * @param case_operand The CASE operand.
   * @param when_clauses A vector of WHEN clauses, each having a check operand to
   *                     be compared with the CASE operand and a result expression
   *                     to be evaluated if the condition is satisfied.
   * @param else_result_expression Optional ELSE result expression.
   */
  ParseSimpleCaseExpression(int line_number,
                            int column_number,
                            ParseExpression *case_operand,
                            PtrVector<ParseSimpleWhenClause> *when_clauses,
                            ParseExpression *else_result_expression)
      : ParseExpression(line_number, column_number),
        case_operand_(case_operand),
        when_clauses_(when_clauses),
        else_result_expression_(else_result_expression) {
  }

  std::string getName() const override {
    return "SimpleCaseExpression";
  }

  ExpressionType getExpressionType() const override {
    return kSimpleCaseExpression;
  }

  /**
   * @return The CASE operand.
   */
  const ParseExpression* case_operand() const {
    return case_operand_.get();
  }

  /**
   * @return The vector of WHEN clauses.
   */
  const PtrVector<ParseSimpleWhenClause>* when_clauses() const {
    return when_clauses_.get();
  }

  /**
   * @return The ELSE result expression. Can be NULL.
   */
  const ParseExpression* else_result_expression() const {
    return else_result_expression_.get();
  }

  std::string generateName() const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseExpression> case_operand_;
  std::unique_ptr<PtrVector<ParseSimpleWhenClause>> when_clauses_;
  std::unique_ptr<ParseExpression> else_result_expression_;

  DISALLOW_COPY_AND_ASSIGN(ParseSimpleCaseExpression);
};

/**
 * @brief The parsed representation of a searched CASE expression:
 *           CASE
 *               WHEN <condition_predicate> THEN <result_expression>
 *               [...n]
 *               [ELSE <else_result_expression>]
 *           END
 *        It returns the <result_expression> of the first <condition_predicate>
 *        that evaluates to true; if none is found and <else_result_expression> exists,
 *        returns <else_result_expression>; otherwise, returns NULL.
 */
class ParseSearchedCaseExpression : public ParseExpression {
 public:
  /**
   * @brief Constructor. Takes ownership of all pointers.
   *
   * @param line_number The line number of "CASE" in the SQL statement.
   * @param column_number The column number of "CASE" in the SQL statement.
   * @param when_clauses A vector of WHEN clauses, each having a predicate
   *                     and a result expression to be evaluate if
   *                     the predicate evaluates to true.
   * @param else_result_expression Optional ELSE result expression.
   */
  ParseSearchedCaseExpression(int line_number,
                              int column_number,
                              PtrVector<ParseSearchedWhenClause> *when_clauses,
                              ParseExpression *else_result_expression)
      : ParseExpression(line_number, column_number),
        when_clauses_(when_clauses),
        else_result_expression_(else_result_expression) {
  }

  std::string getName() const override {
    return "SearchedCaseExpression";
  }

  ExpressionType getExpressionType() const override {
    return kSearchedCaseExpression;
  }

  /**
   * @return The vector of WHEN clauses.
   */
  const PtrVector<ParseSearchedWhenClause>* when_clauses() const {
    return when_clauses_.get();
  }

  /**
   * @return The ELSE result expression. Can be NULL.
   */
  const ParseExpression* else_result_expression() const {
    return else_result_expression_.get();
  }

  std::string generateName() const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<PtrVector<ParseSearchedWhenClause>> when_clauses_;
  std::unique_ptr<ParseExpression> else_result_expression_;

  DISALLOW_COPY_AND_ASSIGN(ParseSearchedCaseExpression);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSE_CASE_EXPRESSIONS_HPP_ */
