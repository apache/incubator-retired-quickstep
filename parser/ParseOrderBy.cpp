/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParseOrderBy.hpp"

#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"

namespace quickstep {

void ParseOrderByItem::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  inline_field_names->push_back("is_asc");
  inline_field_values->push_back(is_ascending_ ? "true" : "false");
  inline_field_names->push_back("nulls_first");
  inline_field_values->push_back(nulls_first_ ? "true" : "false");

  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(ordering_expression_.get());
}

void ParseOrderBy::getFieldStringItems(std::vector<std::string> *inline_field_names,
                                       std::vector<std::string> *inline_field_values,
                                       std::vector<std::string> *non_container_child_field_names,
                                       std::vector<const ParseTreeNode*> *non_container_child_fields,
                                       std::vector<std::string> *container_child_field_names,
                                       std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  for (const ParseOrderByItem &order_by_item : *order_by_items_) {
    non_container_child_field_names->push_back("");
    non_container_child_fields->push_back(&order_by_item);
  }
}

}  // namespace quickstep
