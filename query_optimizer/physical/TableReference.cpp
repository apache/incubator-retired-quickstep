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

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/PartitionScheme.hpp"
#include "catalog/PartitionSchemeHeader.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/physical/PartitionSchemeHeader.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = ::quickstep::optimizer::expressions;

TableReferencePtr TableReference::Create(
    const CatalogRelation *relation,
    const std::string &alias,
    const std::vector<expressions::AttributeReferencePtr> &attribute_list) {
  std::unique_ptr<PartitionSchemeHeader> output_partition_scheme_header;
  const quickstep::PartitionScheme *partition_scheme = relation->getPartitionScheme();
  if (partition_scheme) {
    const quickstep::PartitionSchemeHeader &partition_scheme_header = partition_scheme->getPartitionSchemeHeader();

    PartitionSchemeHeader::PartitionType physical_partition_type;
    switch (partition_scheme_header.getPartitionType()) {
      case quickstep::PartitionSchemeHeader::PartitionType::kHash:
        physical_partition_type = PartitionSchemeHeader::PartitionType::kHash;
        break;
      case quickstep::PartitionSchemeHeader::PartitionType::kRange:
        physical_partition_type = PartitionSchemeHeader::PartitionType::kRange;
        break;
      default:
        return nullptr;
    }

    PartitionSchemeHeader::PartitionExprIds partition_expr_ids;
    for (const attribute_id part_attr : partition_scheme_header.getPartitionAttributeIds()) {
      partition_expr_ids.push_back({ attribute_list[part_attr]->id() });
    }

    output_partition_scheme_header =
        std::make_unique<PartitionSchemeHeader>(physical_partition_type,
                                                partition_scheme_header.getNumPartitions(),
                                                std::move(partition_expr_ids));
  }

  return TableReferencePtr(new TableReference(relation, alias, attribute_list,
                                              output_partition_scheme_header.release()));
}

PhysicalPtr TableReference::copyWithNewChildren(
    const std::vector<PhysicalPtr> &new_children) const {
  DCHECK_EQ(new_children.size(), children().size());
  std::unique_ptr<PartitionSchemeHeader> output_partition_scheme_header;
  return TableReferencePtr(new TableReference(relation_, alias_, attribute_list_,
                                              cloneOutputPartitionSchemeHeader()));
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

  if (partition_scheme_header_) {
    inline_field_names->push_back("output_partition_scheme_header");
    inline_field_values->push_back(partition_scheme_header_->toString());
  }

  container_child_field_names->push_back("");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(attribute_list_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
