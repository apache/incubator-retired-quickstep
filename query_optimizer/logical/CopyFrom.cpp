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

#include "query_optimizer/logical/CopyFrom.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "catalog/CatalogRelation.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

void CopyFrom::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("relation");
  inline_field_values->push_back(catalog_relation_->getName());

  inline_field_names->push_back("file_name");
  inline_field_values->push_back(file_name_);

  inline_field_names->push_back("column_delimiter");
  inline_field_values->push_back("\"" + EscapeSpecialChars(std::string(1, column_delimiter_)) +
                                 "\"");

  inline_field_names->push_back("escape_strings");
  inline_field_values->push_back(escape_strings_ ? "true" : "false");
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
