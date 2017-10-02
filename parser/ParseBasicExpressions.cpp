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

    if (window_name_ != nullptr) {
      inline_field_names->push_back("window_name");
      inline_field_values->push_back(window_name_->value());
    }

    if (window_ != nullptr) {
      non_container_child_field_names->push_back("window");
      non_container_child_fields->push_back(window_.get());
    }
  }
}

std::string ParseArray::generateName() const {
  string name("{");
  if (!elements_.empty()) {
    name.append(elements_.front()->generateName());
    for (std::size_t i = 1; i < elements_.size(); ++i) {
      name.append(",");
      name.append(elements_.at(i)->generateName());
    }
  }
  name.append("}");
  return name;
}

void ParseArray::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  container_child_field_names->emplace_back("elements");
  container_child_fields->emplace_back();
  for (const auto &element : elements_) {
    container_child_fields->back().emplace_back(element.get());
  }
}

}  // namespace quickstep
