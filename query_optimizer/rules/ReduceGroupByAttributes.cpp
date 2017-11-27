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

#include "query_optimizer/rules/ReduceGroupByAttributes.hpp"

#include <algorithm>
#include <map>
#include <vector>
#include <unordered_set>
#include <utility>

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/rules/PruneColumns.hpp"

#include "gflags/gflags.h"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

DEFINE_uint64(reduce_group_by_attributes_threshold, 3u,
              "The threshold for a stored relation's number of attributes in a "
              "group-by clause for the ReduceGroupByAttributes optimization "
              "rule to pull the stored relation up the aggregation");

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr ReduceGroupByAttributes::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);
  cost_model_.reset(new cost::StarSchemaSimpleCostModel(
      std::static_pointer_cast<const P::TopLevelPlan>(input)->shared_subplans()));

  P::PhysicalPtr output = applyInternal(input);
  if (output != input) {
    output = PruneColumns().apply(output);
  }
  return output;
}

P::PhysicalPtr ReduceGroupByAttributes::applyInternal(const P::PhysicalPtr &input) {
  std::vector<P::PhysicalPtr> new_children;
  for (const P::PhysicalPtr &child : input->children()) {
    new_children.push_back(applyInternal(child));
  }

  if (new_children != input->children()) {
    return applyToNode(input->copyWithNewChildren(new_children));
  } else {
    return applyToNode(input);
  }
}

P::PhysicalPtr ReduceGroupByAttributes::applyToNode(const P::PhysicalPtr &input) {
  P::TableReferencePtr table_reference;
  if (P::SomeTableReference::MatchesWithConditionalCast(input, &table_reference)) {
    // Collect the attributes-to-TableReference mapping info.
    for (const auto &attr : table_reference->attribute_list()) {
      source_.emplace(attr->id(), std::make_pair(table_reference, attr));
    }
    return input;
  }

  P::AggregatePtr aggregate;
  if (!P::SomeAggregate::MatchesWithConditionalCast(input, &aggregate) ||
      aggregate->grouping_expressions().size() <= 1u) {
    return input;
  }

  // Divide the group-by attributes into groups based on their source table.
  std::map<P::TableReferencePtr, std::vector<E::AttributeReferencePtr>> table_attributes;
  for (const auto &expr : aggregate->grouping_expressions()) {
    const auto source_it = source_.find(expr->id());
    if (source_it != source_.end()) {
      table_attributes[source_it->second.first].emplace_back(source_it->second.second);
    }
  }

  std::unordered_set<E::ExprId> erased_grouping_attr_ids;
  std::vector<std::pair<P::TableReferencePtr, E::AttributeReferencePtr>> hoisted_tables;

  // For each group (i.e. each source table), if it is profitable then we pull
  // the table up the aggregation.
  for (const auto &pair : table_attributes) {
    const P::TableReferencePtr table = pair.first;
    const std::vector<E::AttributeReferencePtr> &attributes = pair.second;
    // TODO(jianqiao): find a cost-based metic instead of hard-coding the threshold
    // number of group-by attributes.
    if (attributes.size() <= FLAGS_reduce_group_by_attributes_threshold) {
      continue;
    }

    std::vector<AttributeInfo> attr_infos;
    for (const auto &attr : attributes) {
      attr_infos.emplace_back(attr,
                              cost_model_->impliesUniqueAttributes(table, {attr}),
                              !attr->getValueType().isVariableLength(),
                              attr->getValueType().maximumByteLength());
    }

    std::vector<const AttributeInfo *> attr_info_refs;
    for (const auto &info : attr_infos) {
      attr_info_refs.emplace_back(&info);
    }
    std::sort(attr_info_refs.begin(),
              attr_info_refs.end(),
              AttributeInfo::IsBetterThan);

    const AttributeInfo &best_candidate = *attr_info_refs.front();
    if (!best_candidate.is_unique) {
      // Cannot find a key attribute. Give up pulling this table up.
      continue;
    }

    const E::AttributeReferencePtr key_attribute = best_candidate.attribute;
    hoisted_tables.emplace_back(table, key_attribute);

    for (const auto &attr : attributes) {
      if (attr->id() != key_attribute->id()) {
        erased_grouping_attr_ids.emplace(attr->id());
      }
    }
  }

  if (erased_grouping_attr_ids.empty()) {
    return input;
  }

  // Reconstuct the Aggregate node with reduced group-by attributes and then
  // construct HashJoin nodes on top of the Aggregate.
  std::vector<E::NamedExpressionPtr> reduced_grouping_expressions;
  for (const auto &expr : aggregate->grouping_expressions()) {
    if (erased_grouping_attr_ids.find(expr->id()) == erased_grouping_attr_ids.end()) {
      reduced_grouping_expressions.emplace_back(expr);
    }
  }

  const P::AggregatePtr new_aggregate =
      P::Aggregate::Create(aggregate->input(),
                           reduced_grouping_expressions,
                           aggregate->aggregate_expressions(),
                           aggregate->filter_predicate());

  P::PhysicalPtr output = new_aggregate;
  std::vector<E::NamedExpressionPtr> project_expressions =
      E::ToNamedExpressions(output->getOutputAttributes());
  for (const auto &pair : hoisted_tables) {
    const P::TableReferencePtr &source_table = pair.first;
    const E::AttributeReferencePtr &probe_attribute = pair.second;

    E::AttributeReferencePtr build_attribute;
    std::vector<E::AttributeReferencePtr> new_attribute_list;
    for (const auto &attr : source_table->attribute_list()) {
      if (attr->id() == probe_attribute->id()) {
        build_attribute =
          E::AttributeReference::Create(optimizer_context_->nextExprId(),
                                        attr->attribute_name(),
                                        attr->attribute_alias(),
                                        attr->relation_name(),
                                        attr->getValueType(),
                                        E::AttributeReferenceScope::kLocal);
        new_attribute_list.emplace_back(build_attribute);
      } else {
        new_attribute_list.emplace_back(attr);
        project_expressions.emplace_back(attr);
      }
    }

    DCHECK(build_attribute != nullptr);
    const P::TableReferencePtr build_side_table =
        P::TableReference::Create(source_table->relation(),
                                  source_table->relation()->getName(),
                                  new_attribute_list);
    output = P::HashJoin::Create(output,
                                 build_side_table,
                                 {probe_attribute},
                                 {build_attribute},
                                 nullptr,
                                 nullptr,
                                 project_expressions,
                                 P::HashJoin::JoinType::kInnerJoin);
  }

  return output;
}

}  // namespace optimizer
}  // namespace quickstep
