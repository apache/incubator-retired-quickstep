/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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

}  // namespace quickstep
