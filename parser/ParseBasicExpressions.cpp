/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "parser/ParseBasicExpressions.hpp"

#include <string>
#include <vector>

#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseString.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"

using std::string;

namespace quickstep {

void ParseScalarLiteral::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(literal_value_.get());
}

string ParseAttribute::generateName() const {
  string name;
  if (rel_name_ != nullptr) {
    name.append(rel_name_->value());
    name.append(".");
  }
  name.append(attr_name_->value());
  return name;
}

void ParseAttribute::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("attribute_name");
  inline_field_values->push_back(attr_name_->value());

  if (rel_name_ != nullptr) {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(rel_name_->value());
  }
}

std::string ParseUnaryExpression::getName() const {
  return op_.getName();
}

string ParseUnaryExpression::generateName() const {
  string name(op_.getShortName());
  name.append(operand_->generateName());
  return name;
}

void ParseUnaryExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(operand_.get());
}

std::string ParseBinaryExpression::getName() const {
  return op_.getName();
}

string ParseBinaryExpression::generateName() const {
  string name("(");
  name.append(left_operand_->generateName());
  name.append(op_.getShortName());
  name.append(right_operand_->generateName());
  name.push_back(')');
  return name;
}

void ParseBinaryExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const  {
  non_container_child_field_names->push_back("left_operand");
  non_container_child_fields->push_back(left_operand_.get());

  non_container_child_field_names->push_back("right_operand");
  non_container_child_fields->push_back(right_operand_.get());
}

std::string ParseFunctionCall::generateName() const {
  string name(name_->value());
  name.push_back('(');
  if (star_ != nullptr) {
    name.push_back('*');
  } else {
    if (is_distinct_) {
      name.append("DISTINCT ");
    }
    bool first = true;
    for (const ParseExpression &argument : *arguments_) {
      if (!first) {
        name.append(", ");
      } else {
        first = false;
      }
      name.append(argument.generateName());
    }
  }
  name.push_back(')');
  return name;
}

void ParseFunctionCall::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("name");
  inline_field_values->push_back(name_->value());

  if (is_distinct_) {
    inline_field_names->push_back("is_distinct");
    inline_field_values->push_back("true");
  }

  if (star_ != nullptr) {
    inline_field_names->push_back("is_star");
    inline_field_values->push_back("true");
  } else {
    for (const ParseExpression &argument : *arguments_) {
      non_container_child_field_names->push_back("");
      non_container_child_fields->push_back(&argument);
    }
  }
}

std::string ParseExtractFunction::generateName() const {
  std::string name;
  name.append("EXTRACT(");
  name.append(extract_field_->value());
  name.append(" FROM ");
  name.append(date_expression_->generateName());
  name.push_back(')');
  return name;
}

void ParseExtractFunction::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("unit");
  inline_field_values->push_back(extract_field_->value());

  non_container_child_field_names->push_back("date_expression");
  non_container_child_fields->push_back(date_expression_.get());
}

}  // namespace quickstep
