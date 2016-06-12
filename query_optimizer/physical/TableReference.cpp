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

#include "query_optimizer/physical/TableReference.hpp"

#include <string>
#include <set>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = ::quickstep::optimizer::expressions;

bool TableReference::impliesUniqueAttributes(
    const std::vector<expressions::AttributeReferencePtr> &attributes) const {
  std::set<E::ExprId> attr_ids;
  for (const auto &attr : attributes) {
    attr_ids.emplace(attr->id());
  }

  std::set<attribute_id> rel_attr_ids;
  for (std::size_t i = 0; i < attribute_list_.size(); ++i) {
    if (attr_ids.find(attribute_list_[i]->id()) != attr_ids.end()) {
      rel_attr_ids.emplace(i);
    }
  }

  return relation_->getConstraints().impliesUniqueAttributes(rel_attr_ids);
}

void TableReference::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("relation");
  inline_field_values->push_back(relation_->getName());

  if (alias_ != relation_->getName()) {
    inline_field_names->push_back("alias");
    inline_field_values->push_back(alias_);
  }

  container_child_field_names->push_back("");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(attribute_list_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
