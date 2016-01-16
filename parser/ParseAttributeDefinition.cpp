/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
