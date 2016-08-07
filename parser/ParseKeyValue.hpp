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

#ifndef QUICKSTEP_PARSER_PARSE_KEY_VALUE_HPP_
#define QUICKSTEP_PARSER_PARSE_KEY_VALUE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

/**
 * @brief Base class for the Parse Key Values.
 */
class ParseKeyValue : public ParseTreeNode {
 public:
  enum class KeyValueType {
    kStringString,
    kStringStringList,
    kStringInteger
  };

  /**
   * @brief Single value constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param value A single literal value.
   **/
  ParseKeyValue(const int line_number,
                const int column_number,
                ParseString *key)
      : ParseTreeNode(line_number, column_number),
        key_(key) { }

  /**
   * @brief Destructor.
   **/
  ~ParseKeyValue() override { }

  /**
   * @brief Get the subclass type of the parse key value.
   */
  virtual KeyValueType getKeyValueType() const = 0;

  /**
   * @return Pointer to the key string.
   */
  const ParseString* key() const {
    return key_.get();
  }

  std::string getName() const override {
    return "KeyValue";
  }

 protected:
  std::unique_ptr<ParseString> key_;
};

/**
 * @brief The parsed representation of a key-value pair.
 **/
class ParseKeyStringValue : public ParseKeyValue {
 public:
  /**
   * @brief Single value constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param key A parse string representing the key.
   * @param value A parse string representing the key.
   **/
  ParseKeyStringValue(const int line_number,
                      const int column_number,
                      ParseString *key,
                      ParseString *value)
      : ParseKeyValue(line_number, column_number, key),
        value_(value) { }

  KeyValueType getKeyValueType() const override {
    return ParseKeyValue::KeyValueType::kStringString;
  }

  /**
   * @return A pointer to the value ParseString.
   */
  const ParseString* value() const {
    return value_.get();
  }

  std::string getName() const override {
    return "KeyStringValue";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("key");
    inline_field_values->push_back(key_->value());

    non_container_child_field_names->push_back("value");
    non_container_child_fields->push_back(value_.get());
  }

 private:
  std::unique_ptr<ParseString> value_;

  DISALLOW_COPY_AND_ASSIGN(ParseKeyStringValue);
};

/**
 * @brief The parsed representation of a key-value pair. Value is a list of values.
 **/
class ParseKeyStringList : public ParseKeyValue {
 public:
  /**
   * @brief Single value constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param key A parse string representing the key.
   * @param value A list of string values.
   **/
  ParseKeyStringList(const int line_number,
                     const int column_number,
                     ParseString *key,
                     PtrList<ParseString> *value)
      : ParseKeyValue(line_number, column_number, key),
        value_(value) { }

  KeyValueType getKeyValueType() const override {
    return ParseKeyValue::KeyValueType::kStringStringList;
  }

  /**
   * @return A list of ParseStrings.
   */
  const PtrList<ParseString>* value() const {
    return value_.get();
  }

  std::string getName() const override {
    return "KeyStringList";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("key");
    inline_field_values->push_back(key_->value());

    container_child_field_names->push_back("value_list");
    container_child_fields->emplace_back();
    for (const ParseString &value : *value_) {
      container_child_fields->back().push_back(&value);
    }
  }

 private:
  std::unique_ptr<PtrList<ParseString> > value_;

  DISALLOW_COPY_AND_ASSIGN(ParseKeyStringList);
};

/**
 * @brief The parsed representation of a key-value pair.
 **/
class ParseKeyIntegerValue : public ParseKeyValue {
 public:
  /**
   * @brief Single value constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param key A parse string representing the key.
   * @param value A single numeric literal value.
   **/
  ParseKeyIntegerValue(const int line_number,
                       const int column_number,
                       ParseString *key,
                       NumericParseLiteralValue *value)
      : ParseKeyValue(line_number, column_number, key),
        value_(value) { }

  KeyValueType getKeyValueType() const override {
    return ParseKeyValue::KeyValueType::kStringInteger;
  }

  /**
   * @return A pointer to the value ParseString.
   */
  const NumericParseLiteralValue* value() const {
    return value_.get();
  }

  std::string getName() const override {
    return "KeyIntegerValue";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("key");
    inline_field_values->push_back(key_->value());

    non_container_child_field_names->push_back("value");
    non_container_child_fields->push_back(value_.get());
  }

 private:
  std::unique_ptr<NumericParseLiteralValue> value_;

  DISALLOW_COPY_AND_ASSIGN(ParseKeyIntegerValue);
};

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_KEY_VALUE_HPP_
