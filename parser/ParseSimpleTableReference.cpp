/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParseSimpleTableReference.hpp"

#include <string>
#include <vector>

#include "parser/ParseTableReference.hpp"

namespace quickstep {

class ParseTreeNode;

void ParseSimpleTableReference::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  ParseTableReference::getFieldStringItems(inline_field_names,
                                           inline_field_values,
                                           non_container_child_field_names,
                                           non_container_child_fields,
                                           container_child_field_names,
                                           container_child_fields);
  inline_field_names->push_back("table");
  inline_field_values->push_back(table_name_->value());
}

}  // namespace quickstep
