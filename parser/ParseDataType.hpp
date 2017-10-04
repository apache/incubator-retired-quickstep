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

#ifndef QUICKSTEP_PARSER_PARSE_DATA_TYPE_HPP_
#define QUICKSTEP_PARSER_PARSE_DATA_TYPE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class Type;

/** \addtogroup Parser
 *  @{
 */

class ParseDataTypeParameter : public ParseTreeNode {
 public:
  enum class ParameterType {
    kDataType,
    kLiteralValue
  };

  virtual ParameterType getParameterType() const = 0;

 protected:
  ParseDataTypeParameter(const int line_number,
                         const int column_number)
      : ParseTreeNode(line_number, column_number) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseDataTypeParameter);
};

/**
 * @brief Parsed representation of a data type.
 **/
class ParseDataType : public ParseTreeNode {
 public:
  ParseDataType(const int line_number,
                const int column_number,
                ParseString *name)
      : ParseTreeNode(line_number, column_number),
        name_(name),
        nullable_(false) {}

  std::string getName() const override {
    return "DataType";
  }

  const std::vector<std::unique_ptr<ParseDataTypeParameter>>& parameters() const {
    return parameters_;
  }

  bool nullable() const {
    return nullable_;
  }

  void addParameter(ParseDataTypeParameter *parameter) {
    parameters_.emplace_back(std::unique_ptr<ParseDataTypeParameter>(parameter));
  }

  void setNullable(const bool nullable) {
    nullable_ = nullable;
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
  const std::unique_ptr<ParseString> name_;
  std::vector<std::unique_ptr<ParseDataTypeParameter>> parameters_;
  bool nullable_;

  DISALLOW_COPY_AND_ASSIGN(ParseDataType);
};

class ParseDataTypeParameterLiteralValue : public ParseDataTypeParameter {
 public:
  ParseDataTypeParameterLiteralValue(const int line_number,
                                     const int column_number,
                                     ParseLiteralValue *literal_value)
      : ParseDataTypeParameter(line_number, column_number),
        literal_value_(literal_value) {}

  std::string getName() const override {
    return "DataTypeParameterLiteralValue";
  }

  ParameterType getParameterType() const override {
    return ParameterType::kLiteralValue;
  }

  const ParseLiteralValue& literal_value() const {
    return *literal_value_;
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
  std::unique_ptr<ParseLiteralValue> literal_value_;

  DISALLOW_COPY_AND_ASSIGN(ParseDataTypeParameterLiteralValue);
};

class ParseDataTypeParameterDataType : public ParseDataTypeParameter {
 public:
  ParseDataTypeParameterDataType(const int line_number,
                                 const int column_number,
                                 ParseDataType *data_type)
      : ParseDataTypeParameter(line_number, column_number),
        data_type_(data_type) {}

  std::string getName() const override {
    return "DataTypeParameterDataType";
  }

  ParameterType getParameterType() const override {
    return ParameterType::kDataType;
  }

  const ParseDataType& data_type() const {
    return *data_type_;
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
  std::unique_ptr<ParseDataType> data_type_;

  DISALLOW_COPY_AND_ASSIGN(ParseDataTypeParameterDataType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_DATA_TYPE_HPP_
