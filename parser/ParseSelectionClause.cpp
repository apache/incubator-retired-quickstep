/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
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
