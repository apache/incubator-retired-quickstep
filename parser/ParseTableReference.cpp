/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParseTableReference.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace quickstep {

void ParseTableReferenceSignature::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("table_alias");
  inline_field_values->push_back(table_alias_->value());

  if (column_aliases_ != nullptr && !column_aliases_->empty()) {
    std::ostringstream oss;
    PtrList<ParseString>::const_iterator argument_alias_it = column_aliases_->begin();
    oss << "(" << argument_alias_it->value();
    for (std::size_t i = 1; i < column_aliases_->size(); ++i) {
      ++argument_alias_it;
      oss << ", " << argument_alias_it->value();
    }
    oss << ")";
    inline_field_names->push_back("columns");
    inline_field_values->push_back(oss.str());
  }
}

void ParseTableReference::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  if (table_reference_signature_ != nullptr) {
    non_container_child_field_names->push_back("table_signature");
    non_container_child_fields->push_back(table_reference_signature_.get());
  }
}

}  // namespace quickstep
