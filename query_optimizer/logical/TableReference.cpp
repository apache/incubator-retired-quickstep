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

#include "query_optimizer/logical/TableReference.hpp"

#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

TableReference::TableReference(const CatalogRelation *catalog_relation,
                               const std::string &relation_alias,
                               OptimizerContext *optimizer_context)
    : catalog_relation_(catalog_relation),
      relation_alias_(relation_alias) {
  for (CatalogRelation::const_iterator attribute_it = catalog_relation_->begin();
       attribute_it < catalog_relation_->end();
       ++attribute_it) {
    attribute_list_.emplace_back(
        E::AttributeReference::Create(
            optimizer_context->nextExprId(),
            attribute_it->getName(),
            attribute_it->getDisplayName(),
            relation_alias,
            attribute_it->getType()));
  }
}

LogicalPtr TableReference::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  return TableReferencePtr(
      new TableReference(catalog_relation_, relation_alias_, attribute_list_));
}

void TableReference::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("relation_name");
  inline_field_values->push_back(catalog_relation_->getName());

  if (relation_alias_ != catalog_relation_->getName()) {
    inline_field_names->push_back("relation_alias");
    inline_field_values->push_back(relation_alias_);
  }

  container_child_field_names->push_back("");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(attribute_list_));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
