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

#ifndef QUICKSTEP_PARSER_PARSE_GROUP_BY_HPP_
#define QUICKSTEP_PARSER_PARSE_GROUP_BY_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representaiton of a GROUP BY clause.
 */
class ParseGroupBy : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "GROUP" in the SQL statement.
   * @param column_number The column number of "GROUP" in the SQL statement.
   * @param group_by_expressions The group by expressions.
   */
  ParseGroupBy(const int line_number, const int column_number, PtrList<ParseExpression> *grouping_expressions)
      : ParseTreeNode(line_number, column_number),
        grouping_expressions_(grouping_expressions) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseGroupBy() override {}

  /**
   * @brief Gets the GROUP BY expressions.
   *
   * @return GROUP BY expressions.
   */
  const PtrList<ParseExpression>* grouping_expressions() const {
    return grouping_expressions_.get();
  }

  std::string getName() const override {
    return "GroupBy";
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<PtrList<ParseExpression>> grouping_expressions_;

  DISALLOW_COPY_AND_ASSIGN(ParseGroupBy);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSEGROUPBY_HPP_ */
