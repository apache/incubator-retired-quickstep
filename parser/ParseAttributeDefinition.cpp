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

#include "parser/ParseAttributeDefinition.hpp"

#include <string>
#include <vector>

#include "parser/ParseString.hpp"
#include "types/Type.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

ParseAttributeDefinition::ParseAttributeDefinition(const int line_number,
                                                   const int column_number,
                                                   ParseString *name,
                                                   ParseDataType *data_type,
                                                   PtrList<ParseColumnConstraint> *constraint_list)
    : ParseTreeNode(line_number, column_number), name_(name), data_type_(data_type) {
  if (constraint_list != nullptr) {
    for (PtrList<ParseColumnConstraint>::const_iterator it = constraint_list->begin();
         it != constraint_list->end();
         ++it) {
      it->applyTo(this);
    }

    delete constraint_list;
  }
}

void ParseAttributeDefinition::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("name");
  inline_field_values->push_back(name_->value());
  inline_field_names->push_back("type");
  inline_field_values->push_back(data_type_->getType().getName());
}

void ParseColumnConstraintNull::applyTo(ParseAttributeDefinition *target) const {
  target->data_type_->type_ = &(target->data_type_->type_->getNullableVersion());
}

void ParseColumnConstraintNull::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("nullable");
  inline_field_values->push_back("true");
}

void ParseColumnConstraintNotNull::applyTo(ParseAttributeDefinition *target) const {
  target->data_type_->type_ = &(target->data_type_->type_->getNonNullableVersion());
}

void ParseColumnConstraintNotNull::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("nullable");
  inline_field_values->push_back("false");
}

}  // namespace quickstep
