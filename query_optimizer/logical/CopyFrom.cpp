/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
