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

#ifndef QUICKSTEP_PARSER_PARSE_STRING_HPP_
#define QUICKSTEP_PARSER_PARSE_STRING_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief String (an identifier or a string literal value) in the SQL statement.
 */
class ParseString : public ParseTreeNode {
 public:
  /**
   * @brief Construction.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param value The string value.
   */
  ParseString(const int line_number,
              const int column_number,
              const std::string &value = "")
      : ParseTreeNode(line_number, column_number),
        value_(value) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseString() override {}

  std::string getName() const override {
    return "String";
  }

  /**
   * @return The string value.
   */
  const std::string& value() const { return value_; }

  /**
   * @brief Append \p c into the string.
   *
   * @param c Char to append.
   */
  void push_back(char c) {
    value_.push_back(c);
  }

  /**
   * @brief Appends a copy of the first n characters in the array of characters pointed by s.
   *
   * @param s The string to be appended.
   * @param n The number of characters to be copied from \p s.
   * @return A reference to this ParseString.
   */
  ParseString& append(const char *s, const std::size_t n) {
    value_.append(s, n);
    return *this;
  }

  /**
   * @brief Makes a copy of this ParseString.
   *
   * @return A copy of this ParseString. The caller is responsible for taking the ownership.
   */
  ParseString* clone() const {
    return new ParseString(line_number(), column_number(), value_);
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
  std::string value_;

  DISALLOW_COPY_AND_ASSIGN(ParseString);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSE_STRING_HPP_ */
