/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParseGroupBy.hpp"

#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"

namespace quickstep {

void ParseGroupBy::getFieldStringItems(std::vector<std::string> *inline_field_names,
                                       std::vector<std::string> *inline_field_values,
                                       std::vector<std::string> *non_container_child_field_names,
                                       std::vector<const ParseTreeNode*> *non_container_child_fields,
                                       std::vector<std::string> *container_child_field_names,
                                       std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  for (const ParseExpression &group_by_expression : *grouping_expressions_) {
    non_container_child_field_names->push_back("");
    non_container_child_fields->push_back(&group_by_expression);
  }
}

}  // namespace quickstep
