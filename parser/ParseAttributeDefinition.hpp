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

#ifndef QUICKSTEP_PARSER_PARSE_ATTRIBUTE_DEFINITION_HPP_
#define QUICKSTEP_PARSER_PARSE_ATTRIBUTE_DEFINITION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseDataType.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseColumnConstraint;

template <class T> class PtrList;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Parsed representation of an attribute definition
 **/
class ParseAttributeDefinition : public ParseTreeNode {
 public:
  /**
   * @brief Constructor
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param name The attribute name.
   * @param data_type The parsed data type (becomes owned by this
   *        ParseAttributeDefinition).
   * @param constraint_list An optional list of column constraints (may be NULL
   *        if no constraints).
   **/
  ParseAttributeDefinition(const int line_number,
                           const int column_number,
                           ParseString *name,
                           ParseDataType *data_type,
                           PtrList<ParseColumnConstraint> *constraint_list);

  /**
   * @brief Destructor
   **/
  ~ParseAttributeDefinition() override {}

  std::string getName() const override {
    return "AttributeDefinition";
  }

  /**
   * @brief Get the attribute name
   *
   * @return The attribute name
   **/
  const ParseString* name() const {
    return name_.get();
  }

  /**
   * @brief Get the parsed data type
   *
   * @return The data type
   **/
  const ParseDataType& data_type() const {
    return *data_type_;
  }

  const bool nullable() const {
    return nullable_;
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
  std::unique_ptr<ParseString> name_;
  std::unique_ptr<ParseDataType> data_type_;
  bool nullable_;

  DISALLOW_COPY_AND_ASSIGN(ParseAttributeDefinition);
};

/**
 * @brief A column constraint that can be applied to an attribute definition.
 **/
class ParseColumnConstraint : public ParseTreeNode {
 public:
  /**
   * @brief Virtual destructor.
   **/
  ~ParseColumnConstraint() override {
  }

  /**
   * @brief Apply this constraint to an attribute definition.
   *
   * @param target The attribute definition to modify with this constraint.
   **/
  virtual void applyTo(ParseAttributeDefinition *target) const = 0;

 protected:
  ParseColumnConstraint(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseColumnConstraint);
};

/**
 * @brief A column constraint allowing NULL values.
 **/
class ParseColumnConstraintNull: public ParseColumnConstraint {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  ParseColumnConstraintNull(const int line_number, const int column_number)
      : ParseColumnConstraint(line_number, column_number) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseColumnConstraintNull() override {
  }

  std::string getName() const override {
    return "NullablityColumnConstraint";
  }

  void applyTo(ParseAttributeDefinition *target) const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseColumnConstraintNull);
};

/**
 * @brief A column constraint disallowing NULL values.
 **/
class ParseColumnConstraintNotNull: public ParseColumnConstraint {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  ParseColumnConstraintNotNull(const int line_number, const int column_number)
     : ParseColumnConstraint(line_number, column_number) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseColumnConstraintNotNull() override {
  }

  std::string getName() const override {
    return "NullablityColumnConstraint";
  }

  void applyTo(ParseAttributeDefinition *target) const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseColumnConstraintNotNull);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_ATTRIBUTE_DEFINITION_HPP_
