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

#ifndef QUICKSTEP_PARSER_PARSE_WINDOW_HPP_
#define QUICKSTEP_PARSER_PARSE_WINDOW_HPP_

#include "parser/ParseBasicExpressions.hpp"
#include "parser/ParseExpression.hpp"
#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/PtrList.hpp"

#include "glog/logging.h"

namespace quickstep {

/**
 * @brief The parsed representation of a WINDOW clause consisting of window
 *        duration, window attribute, etc..
 **/
class ParseWindow : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   * @param line_number The line number of the first token of this WINDOW clause
   *                    in the SQL statement.
   * @param column_number The column number of the first token of this WINDOW
   *                      clause in the SQL statement.
   * @param window_attribute Attribute on which to compute the windows to
   *                         aggregate on.
   * @param grouping_expressions Optional grouping expressions that might be
   *                             specified in the SQL statement. Similar to
   *                             GROUP BY with regular aggregates.
   * @param window_duration Size of the tumbling windows.
   * @param emit_duraion Time period between scanning for new tuples to
   *                     aggregate.
   * @param age_duration Windows beyond this duration are aged out.
   **/
  ParseWindow(const int line_number,
              const int column_number,
              ParseAttribute *window_attribute,
              PtrList<ParseExpression> *grouping_expressions,
              ParseLiteralValue *window_duration,
              ParseLiteralValue *emit_duration,
              ParseLiteralValue *age_duration)
      : ParseTreeNode(line_number, column_number),
        window_attribute_(window_attribute),
        grouping_expressions_(grouping_expressions),
        window_duration_(window_duration),
        emit_duration_(emit_duration),
        age_duration_(age_duration) {}

  /**
   * @brief Destructor.
   **/
  ~ParseWindow() override {}

  std::string getName() const override {
    return "WINDOW";
  }

  /**
   * @brief Helper to check if the clause is valid.
   **/
  bool isValid() const {
    return window_attribute_ && window_duration_ && emit_duration_ && age_duration_;
  }

  /**
   * @brief Windowing attribute.
   **/
  const ParseAttribute& window_attribute() const {
    return *window_attribute_;
  }

  /**
   * @brief Grouping expressions.
   **/
  const PtrList<ParseExpression> *grouping_expressions() const {
    return grouping_expressions_.get();
  }

  /**
   * @brief Window size/duration.
   **/
  const ParseLiteralValue& window_duration() const { return *window_duration_; }

  /**
   * @brief  Emit duration.
   **/
  const ParseLiteralValue& emit_duration() const { return *emit_duration_; }

  /**
   * @brief Aging duration.
   **/
  const ParseLiteralValue& age_duration() const { return *age_duration_; }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode *> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode *>> *container_child_fields)
      const override {
    non_container_child_field_names->push_back("window_attribute");
    non_container_child_fields->push_back(window_attribute_.get());

    non_container_child_field_names->push_back("window_duration");
    non_container_child_fields->push_back(window_duration_.get());

    non_container_child_field_names->push_back("emit_duration");
    non_container_child_fields->push_back(emit_duration_.get());

    non_container_child_field_names->push_back("age_duration");
    non_container_child_fields->push_back(age_duration_.get());

    container_child_field_names->push_back("partition_by");
    std::vector<const ParseTreeNode *> partition_by;
    if (grouping_expressions_) {
      for (const auto &e : *grouping_expressions_) {
        partition_by.push_back(static_cast<const ParseTreeNode *>(&e));
      }
    }
    container_child_fields->push_back(partition_by);
  }

 private:
  std::unique_ptr<ParseAttribute> window_attribute_;
  std::unique_ptr<PtrList<ParseExpression>> grouping_expressions_;
  std::unique_ptr<ParseLiteralValue> window_duration_;
  std::unique_ptr<ParseLiteralValue> emit_duration_;
  std::unique_ptr<ParseLiteralValue> age_duration_;
};

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_WINDOW_HPP_
