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

#ifndef QUICKSTEP_PARSER_PARSE_LIMIT_HPP_
#define QUICKSTEP_PARSER_PARSE_LIMIT_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief A parsed representation of LIMIT.
 */
class ParseLimit : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "LIMIT" in the SQL statement.
   * @param column_number The column number of "LIMIT" in the SQL statement.
   * @param limit_expression The LIMIT value expression.
   */
  ParseLimit(const int line_number, const int column_number, NumericParseLiteralValue *limit_expression)
      : ParseTreeNode(line_number, column_number),
        limit_expression_(limit_expression) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseLimit() override {}

  /**
   * @brief Gets the LIMIT expression.
   *
   * @return LIMIT expression
   */
  const NumericParseLiteralValue* limit_expression() const {
    return limit_expression_.get();
  }

  std::string getName() const override {
    return "LIMIT";
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<NumericParseLiteralValue> limit_expression_;

  DISALLOW_COPY_AND_ASSIGN(ParseLimit);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSELIMIT_HPP_ */
