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

#include "parser/ParseJoinedTableReference.hpp"

#include <string>
#include <type_traits>
#include <vector>

#include "glog/logging.h"

namespace quickstep {

void ParseJoinedTableReference::getFieldStringItems(
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

  inline_field_names->push_back("join_type");
  switch (join_type_) {
    case JoinType::kInnerJoin:
      inline_field_values->push_back("InnerJoin");
      break;
    case JoinType::kLeftOuterJoin:
      inline_field_values->push_back("LeftOuterJoin");
      break;
    case JoinType::kRightOuterJoin:
      inline_field_values->push_back("RightOuterJoin");
      break;
    case JoinType::kFullOuterJoin:
      inline_field_values->push_back("FullOuterJoin");
      break;
    default:
      LOG(FATAL) << "Invalid JoinType: "
                 << static_cast<typename std::underlying_type<JoinType>::type>(join_type_);
  }

  non_container_child_field_names->push_back("left_table");
  non_container_child_fields->push_back(left_table_.get());

  non_container_child_field_names->push_back("right_table");
  non_container_child_fields->push_back(right_table_.get());

  non_container_child_field_names->push_back("join_predicate");
  non_container_child_fields->push_back(join_predicate_.get());
}

}  // namespace quickstep
