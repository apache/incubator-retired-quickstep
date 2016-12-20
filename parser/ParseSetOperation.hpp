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

#ifndef QUICKSTEP_PARSER_PARSE_SET_OPERATION_HPP_
#define QUICKSTEP_PARSER_PARSE_SET_OPERATION_HPP_

#include <string>
#include <vector>

#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief A parsed representation of set operations.
 */
class ParseSetOperation : public ParseTreeNode {
 public:
  /**
   * @brief The possible types of set operations.
   */
  enum SetOperationType {
    kIntersect = 0,
    kSelect,
    kUnion,
    kUnionAll
  };

  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the set operation token in the SQL statement.
   * @param column_number Column number of the set operation toke in the SQL statement.
   * @param set_operation The set operation type.
   */
  ParseSetOperation(const int line_number,
                    const int column_number,
                    const SetOperationType set_operation_type)
      : ParseTreeNode(line_number, column_number),
        set_operation_type_(set_operation_type) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseSetOperation() override {}

  std::string getName() const override {
    return "SetOperation";
  }

  /**
   * @return The set operation type.
   */
  SetOperationType getOperationType() const {
    return set_operation_type_;
  }

  /**
   * @return The operands of the set operation.
   */
  const PtrList<ParseTreeNode>& operands() const {
    return operands_;
  }

  /**
   * @brief Add an operand for the set operation.
   *
   * @param operand The operand.
   */
  void addOperand(ParseTreeNode *operand) {
    operands_.push_back(operand);
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("set_operation_type");
    switch (set_operation_type_) {
      case kIntersect:
        inline_field_values->push_back("Intersect");
        break;
      case kSelect:
        inline_field_values->push_back("Select");
        break;
      case kUnion:
        inline_field_values->push_back("Union");
        break;
      case kUnionAll:
        inline_field_values->push_back("UnionAll");
        break;
      default:
        LOG(FATAL) << "Unknown set operation type.";
    }

    container_child_field_names->push_back("children");
    container_child_fields->emplace_back();
    for (const ParseTreeNode &child : operands_) {
      container_child_fields->back().push_back(&child);
    }
  }

 private:
  PtrList<ParseTreeNode> operands_;
  const SetOperationType set_operation_type_;

  DISALLOW_COPY_AND_ASSIGN(ParseSetOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_SET_OPERATION_HPP_
