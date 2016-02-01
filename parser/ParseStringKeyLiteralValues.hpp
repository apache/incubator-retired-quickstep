/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_PARSER_PARSE_STRING_KEY_LITERAL_VALUES_HPP_
#define QUICKSTEP_PARSER_PARSE_STRING_KEY_LITERAL_VALUES_HPP_

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
 * @brief The parsed representation of a key-value pair. Value is a list of values.
 **/
class ParseStringKeyLiteralValues : public ParseTreeNode {
 public:
  /**
   * @brief Single value constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param key A parse string representing the key.
   * @param value A single literal value.
   **/
  ParseStringKeyLiteralValues(const int line_number,
                              const int column_number,
                              ParseString *key,
                              ParseLiteralValue *value) 
      : ParseTreeNode(line_number, column_number),
        key_(key),
        values_(new PtrList<ParseLiteralValue>) {
    values_->push_back(value);
  }

  /**
   * @brief A multi-value constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param key A parse string representing the key.
   * @param values A list of literal values.
   **/
  ParseStringKeyLiteralValues(const int line_number,
                              const int column_number,
                              ParseString *key,
                              PtrList<ParseLiteralValue> *values) 
      : ParseTreeNode(line_number, column_number),
        key_(key),
        values_(values) {  }

  /**
   * @brief Virtual destructor.
   **/
  ~ParseStringKeyLiteralValues() override {  }

  /**
   * @return Pointer to the key string.
   */
  const ParseString* key() const {
    return key_.get();
  }

  /**
   * @return A list of ParseLiteralValues.
   */
  const PtrList<ParseLiteralValue>& values() const {
    return *(values_.get());
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

    container_child_field_names->push_back((values_->size() == 1) ? "value" : "values");
    container_child_fields->emplace_back();
    for (const ParseLiteralValue& literal_value : *values_) {
      container_child_fields->back().push_back(&literal_value);
    }
  }

 private:
  std::unique_ptr<ParseString> key_;
  std::unique_ptr<PtrList<ParseLiteralValue> > values_;

  DISALLOW_COPY_AND_ASSIGN(ParseStringKeyLiteralValues);
};

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_STRING_KEY_LITERAL_VALUES_HPP_