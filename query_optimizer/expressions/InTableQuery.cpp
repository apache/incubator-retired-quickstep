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

#include "query_optimizer/expressions/InTableQuery.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ExprId.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

::quickstep::Predicate* InTableQuery::concretize(
    const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const {
  LOG(FATAL) << "InTableQuery predicate should not be concretized";
}

void InTableQuery::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("test_expression");
  non_container_child_fields->push_back(test_expression_);

  non_container_child_field_names->push_back("table_query");
  non_container_child_fields->push_back(table_query_);
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
