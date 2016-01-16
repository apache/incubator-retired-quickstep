/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParseSubqueryTableReference.hpp"

#include <string>
#include <vector>

namespace quickstep {

class ParseTreeNode;

void ParseSubqueryTableReference::getFieldStringItems(
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
  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(subquery_expr_.get());
}

}  // namespace quickstep
