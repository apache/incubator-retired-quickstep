/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_PRIORITY_HPP_
#define QUICKSTEP_PARSER_PARSE_PRIORITY_HPP_

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
 * @brief A parsed representation of PRIORITY.
 **/
class ParsePriority : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "PRIORITY" in the SQL statement.
   * @param column_number The column number of "PRIORITY" in the SQL statement.
   * @param priority_expression The PRIORITY value expression.
   **/
  ParsePriority(const int line_number,
                const int column_number,
                NumericParseLiteralValue *priority_expression)
      : ParseTreeNode(line_number, column_number),
        priority_expression_(priority_expression) {}

  /**
   * @brief Destructor.
   */
  ~ParsePriority() override {}

  /**
   * @brief Gets the PRIORITY expression.
   *
   * @return PRIORITY expression
   */
  const NumericParseLiteralValue* priority_expression() const {
    return priority_expression_.get();
  }

  std::string getName() const override {
    return "PRIORITY";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode *> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode *>> *container_child_fields)
      const override {
    non_container_child_field_names->push_back("");
    non_container_child_fields->push_back(priority_expression_.get());
  }

 private:
  std::unique_ptr<NumericParseLiteralValue> priority_expression_;

  DISALLOW_COPY_AND_ASSIGN(ParsePriority);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_PRIORITY_HPP_
