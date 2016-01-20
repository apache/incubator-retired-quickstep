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
