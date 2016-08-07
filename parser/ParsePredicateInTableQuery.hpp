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

#ifndef QUICKSTEP_PARSER_PARSE_PREDICATE_IN_TABLE_QUERY_HPP_
#define QUICKSTEP_PARSER_PARSE_PREDICATE_IN_TABLE_QUERY_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParseSubqueryExpression.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseTreeNode;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Parsed representation of IN with a table subquery expression.
 *
 * @note Putting the class here instead of in ParsePredicate.hpp is to avoid
 *       circular dependencies with ParseSelect.
 */
class ParsePredicateInTableQuery : public ParsePredicate {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of the token "IN" in the statement.
   * @param column_number The column number of the token "IN" in the statement.
   * @param test_expression The expression to test with a subquery expression.
   * @param table_query The table subquery expression to search for test_expression.
   */
  ParsePredicateInTableQuery(const int line_number,
                             const int column_number,
                             ParseExpression *test_expression,
                             ParseSubqueryExpression *table_query)
      : ParsePredicate(line_number, column_number),
        test_expression_(test_expression),
        table_query_(table_query) {}

  ParsePredicateType getParsePredicateType() const override {
    return kInTableQuery;
  }

  std::string getName() const override {
    return "InTableQuery";
  }

  /**
   * @return The expression to test with a subquery expression.
   */
  const ParseExpression* test_expression() const {
    return test_expression_.get();
  }

  /**
   * @return The table subquery expression to search for test_expression.
   */
  const ParseSubqueryExpression* table_query() const {
    return table_query_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    non_container_child_field_names->push_back("test_expression");
    non_container_child_fields->push_back(test_expression_.get());

    non_container_child_field_names->push_back("table_query");
    non_container_child_fields->push_back(table_query_.get());
  }

 private:
  std::unique_ptr<ParseExpression> test_expression_;
  std::unique_ptr<ParseSubqueryExpression> table_query_;

  DISALLOW_COPY_AND_ASSIGN(ParsePredicateInTableQuery);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_PREDICATE_IN_TABLE_QUERY_HPP_
