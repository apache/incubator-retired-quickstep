/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_PARSER_PARSE_ASSIGNMENT_HPP_
#define QUICKSTEP_PARSER_PARSE_ASSIGNMENT_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Parsed representation of an assignment of a value to an attribute.
 **/
class ParseAssignment : public ParseTreeNode {
 public:
  /**
   * @brief Constructor
   *
   * @param line_number The line number of the first token of this parser node.
   * @param column_number The column number of the first token of this parser node.
   * @param attr_name The name of the attribute in the assignment.
   * @param value The scalar value of the assignment (becomes owned by this
   *        ParseAssignment).
   **/
  ParseAssignment(const int line_number, const int column_number, ParseString *attr_name, ParseExpression *value)
      : ParseTreeNode(line_number, column_number), attr_name_(attr_name), value_(value) {
  }

  /**
   * @brief Destructor.
   **/
  ~ParseAssignment() override {
  }

  std::string getName() const override {
    return "AttributeAssignment";
  }

  /**
   * @brief Get the name of the attribute in the assignment.
   *
   * @return The attribute name.
   **/
  const ParseString* attr_name() const {
    return attr_name_.get();
  }

  /**
   * @brief Get the scalar in the assignment.
   *
   * @return The scalar value.
   **/
  const ParseExpression& value() const {
    return *value_;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("attribute_name");
    inline_field_values->push_back(attr_name_->value());

    non_container_child_field_names->push_back("value");
    non_container_child_fields->push_back(value_.get());
  }

 private:
  std::unique_ptr<ParseString> attr_name_;
  std::unique_ptr<ParseExpression> value_;

  DISALLOW_COPY_AND_ASSIGN(ParseAssignment);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_ASSIGNMENT_HPP_
