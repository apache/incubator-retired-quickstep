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

#ifndef QUICKSTEP_PARSER_PARSE_WINDOW_HPP_
#define QUICKSTEP_PARSER_PARSE_WINDOW_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseOrderBy.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

/**
 * @brief The information of the how the framing in the window is defined
 **/
struct ParseFrameInfo : ParseTreeNode {
  /**
   * @brief Constructor.
   * @param row True if the frame mode is ROW, false if it is RANGE.
   * @param num_pre The number of rows/value of range that is preceding
   *                the current row in the frame.
   * @param num_follow The number of rows/value of range that is following
   *                   the current row in the frame.
   **/
  ParseFrameInfo(const int line_number,
                 const int column_number,
                 const bool is_row_in,
                 const std::int64_t num_preceding_in,
                 const std::int64_t num_following_in)
      : ParseTreeNode(line_number, column_number),
        is_row(is_row_in),
        num_preceding(num_preceding_in),
        num_following(num_following_in) {
  }

  std::string getName() const override { return "FrameInfo"; }

  const bool is_row;
  const std::int64_t num_preceding;
  const std::int64_t num_following;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode *> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode *>> *container_child_fields)
      const override {
    inline_field_names->push_back("frame_mode");
    inline_field_values->push_back(is_row ? "row" : "range");

    inline_field_names->push_back("num_preceding");
    inline_field_values->push_back(std::to_string(num_preceding));

    inline_field_names->push_back("num_following");
    inline_field_values->push_back(std::to_string(num_following));
  }
};

/**
 * @brief The parsed representation of a WINDOW definition.
 **/
class ParseWindow : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   * @param line_number The line number of the first token of this WINDOW clause
   *                    in the SQL statement.
   * @param column_number The column number of the first token of this WINDOW
   *                      clause in the SQL statement.
   * @param partition_by_expressions Optional grouping expressions that might be
   *                                 specified in the SQL statement. Similar to
   *                                 GROUP BY with regular aggregates.
   * @param order_by_expressions Optional ordering expressions that might be
   *                             specified in the SQL statement.
   * @param frame_info The information about framing.
   **/
  ParseWindow(const int line_number,
              const int column_number,
              PtrList<ParseExpression> *partition_by_expressions,
              PtrList<ParseOrderByItem> *order_by_expressions,
              ParseFrameInfo *frame_info)
      : ParseTreeNode(line_number, column_number),
        partition_by_expressions_(partition_by_expressions),
        order_by_expressions_(order_by_expressions),
        frame_info_(frame_info) {
  }

  /**
   * @brief Destructor.
   **/
  ~ParseWindow() override {}

  std::string getName() const override {
    return "window";
  }

  /**
   * @brief Grouping expressions.
   **/
  const PtrList<ParseExpression>* partition_by_expressions() const {
    return partition_by_expressions_.get();
  }

  /**
   * @brief Ordering expressions.
   **/
  const PtrList<ParseOrderByItem>* order_by_expressions() const {
    return order_by_expressions_.get();
  }

  /**
   * @brief Frame information.
   **/
  const ParseFrameInfo* frame_info() const {
    return frame_info_.get();
  }

  /**
   * @return The window name.
   */
  const ParseString* name() const {
    return name_.get();
  }

  /**
   * @brief Set the name of the window.
   * @param name The name of the window.
   **/
  void setName(ParseString *name) {
    name_.reset(name);
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode *> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode *>> *container_child_fields) const override {
    if (name_ != nullptr) {
      inline_field_names->push_back("window_name");
      inline_field_values->push_back(name_->value());
    }

    container_child_field_names->push_back("partition_by");
    container_child_fields->emplace_back();
    if (partition_by_expressions_ != nullptr) {
      for (const auto &e : *partition_by_expressions_) {
        container_child_fields->back().emplace_back(&e);
      }
    }

    container_child_field_names->push_back("order_by");
    container_child_fields->emplace_back();
    if (order_by_expressions_ != nullptr) {
      for (const auto &e : *order_by_expressions_) {
        container_child_fields->back().emplace_back(&e);
      }
    }

    if (frame_info_ != nullptr) {
      non_container_child_field_names->push_back("frame_info");
      non_container_child_fields->push_back(frame_info_.get());
    }
  }

 private:
  std::unique_ptr<PtrList<ParseExpression>> partition_by_expressions_;
  std::unique_ptr<PtrList<ParseOrderByItem>> order_by_expressions_;
  std::unique_ptr<ParseFrameInfo> frame_info_;
  std::unique_ptr<ParseString> name_;
};

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_WINDOW_HPP_
