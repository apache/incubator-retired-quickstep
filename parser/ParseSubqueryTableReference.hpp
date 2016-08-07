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

#ifndef QUICKSTEP_PARSER_PARSE_SUBQUERY_TABLE_REFERENCE_HPP_
#define QUICKSTEP_PARSER_PARSE_SUBQUERY_TABLE_REFERENCE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseSubqueryExpression.hpp"
#include "parser/ParseTableReference.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseTreeNode;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief A subquery table (table derived by a subquery expression) in the FROM clause.
 */
class ParseSubqueryTableReference : public ParseTableReference {
 public:
  /**
   * @brief Constructor. Takes ownership of \p subquery_expr.
   *
   * @param line_number The line number of the first token of the table subquery.
   * @param column_number The column number of the first token of the table subquery.
   * @param subquery_expr The subquery expression.
   */
  ParseSubqueryTableReference(const int line_number,
                              const int column_number,
                              ParseSubqueryExpression *subquery_expr)
      : ParseTableReference(line_number, column_number),
        subquery_expr_(subquery_expr) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseSubqueryTableReference() override {}

  TableReferenceType getTableReferenceType() const override {
    return kSubqueryTableReference;
  }

  std::string getName() const override { return "SubqueryTable"; }

  /**
   * @return The subquery expression.
   */
  const ParseSubqueryExpression* subquery_expr() const { return subquery_expr_.get(); }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseSubqueryExpression> subquery_expr_;

  DISALLOW_COPY_AND_ASSIGN(ParseSubqueryTableReference);
};

/** @} */

}  // namespace quickstep



#endif /* QUICKSTEP_PARSER_PARSE_SUBQUERY_TABLE_REFERENCE_HPP_ */
