/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "query_optimizer/logical/CopyTo.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

void CopyTo::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("file_name");
  inline_field_values->push_back(file_name_);

  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  inline_field_names->push_back("format");
  inline_field_values->push_back(options_->getFormatName());

  inline_field_names->push_back("column_delimiter");
  inline_field_values->push_back(
      "\"" + EscapeSpecialChars(std::string(1, options_->getDelimiter())) + "\"");

  if (options_->escapeStrings()) {
    inline_field_names->push_back("escape_strings");
    inline_field_values->push_back("true");
  }

  if (options_->hasHeader()) {
    inline_field_names->push_back("header");
    inline_field_values->push_back("true");
  }

  if (options_->getQuoteCharacter() != 0) {
    inline_field_names->push_back("quote");
    inline_field_values->push_back(std::string(1, options_->getQuoteCharacter()));
  }

  if (options_->getNullString() != "") {
    inline_field_names->push_back("null_string");
    inline_field_values->push_back(options_->getNullString());
  }
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
