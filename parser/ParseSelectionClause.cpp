/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParseSelectionClause.hpp"

#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseString.hpp"

namespace quickstep {

void ParseSelectionItem::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  if (alias_ != nullptr) {
    inline_field_names->push_back("alias");
    inline_field_values->push_back(alias_->value());
  }

  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(expression_.get());
}

void ParseSelectionList::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  container_child_field_names->push_back("");
  container_child_fields->emplace_back();
  for (const ParseSelectionItem& select_item : select_item_list_) {
    container_child_fields->back().push_back(&select_item);
  }
}

}  // namespace quickstep
