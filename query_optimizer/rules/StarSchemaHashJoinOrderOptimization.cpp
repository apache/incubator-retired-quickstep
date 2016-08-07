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

#include "query_optimizer/rules/StarSchemaHashJoinOrderOptimization.hpp"

#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr StarSchemaHashJoinOrderOptimization::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);
  cost_model_.reset(
      new cost::StarSchemaSimpleCostModel(
          std::static_pointer_cast<const P::TopLevelPlan>(input)->shared_subplans()));

  return applyInternal(input, nullptr);
}

P::PhysicalPtr StarSchemaHashJoinOrderOptimization::applyInternal(const P::PhysicalPtr &input,
                                                                  JoinGroupInfo *parent_join_group) {
  P::HashJoinPtr hash_join;
  const bool is_hash_inner_join =
      P::SomeHashJoin::MatchesWithConditionalCast(input, &hash_join)
          && hash_join->join_type() == P::HashJoin::JoinType::kInnerJoin;

  if (is_hash_inner_join) {
    bool is_valid_cascading_hash_join = false;
    if (hash_join->residual_predicate() == nullptr) {
      is_valid_cascading_hash_join = true;
      for (const E::NamedExpressionPtr expr : hash_join->project_expressions()) {
        if (!E::SomeAttributeReference::Matches(expr)) {
          is_valid_cascading_hash_join = false;
          break;
        }
      }
    }

    std::unique_ptr<JoinGroupInfo> new_join_group;
    JoinGroupInfo *join_group = nullptr;
    if (parent_join_group == nullptr || !is_valid_cascading_hash_join) {
      new_join_group.reset(new JoinGroupInfo());
      join_group = new_join_group.get();
    } else {
      join_group = parent_join_group;
    }

    // Gather tables into the join group.
    for (const P::PhysicalPtr &child : input->children()) {
      applyInternal(child, join_group);
    }

    // Gather join attribute pairs.
    for (std::size_t i = 0; i < hash_join->left_join_attributes().size(); ++i) {
      const std::size_t left_attr_id = hash_join->left_join_attributes()[i]->id();
      const std::size_t right_attr_id = hash_join->right_join_attributes()[i]->id();

      join_group->join_attribute_pairs.emplace_back(left_attr_id, right_attr_id);
    }

    if (join_group != parent_join_group) {
      // This node is the root node for a group of hash inner joins. Now plan the
      // ordering of the joins.
      P::PhysicalPtr output = generatePlan(*join_group,
                                           hash_join->residual_predicate(),
                                           hash_join->project_expressions());
      if (parent_join_group == nullptr) {
        return output;
      } else {
        parent_join_group->tables.emplace_back(output);
        return nullptr;
      }
    } else {
      return nullptr;
    }
  } else {
    std::vector<P::PhysicalPtr> new_children;
    bool has_changed_children = false;
    for (const P::PhysicalPtr &child : input->children()) {
      P::PhysicalPtr new_child = applyInternal(child, nullptr);
      DCHECK(new_child != nullptr);
      if (child != new_child && !has_changed_children) {
        has_changed_children = true;
      }
      new_children.push_back(new_child);
    }

    P::PhysicalPtr output =
        (has_changed_children ? input->copyWithNewChildren(new_children)
                              : input);

    if (parent_join_group == nullptr) {
      return output;
    } else {
      parent_join_group->tables.emplace_back(output);
      return nullptr;
    }
  }
}

physical::PhysicalPtr StarSchemaHashJoinOrderOptimization::generatePlan(
    const JoinGroupInfo &join_group,
    const E::PredicatePtr &residual_predicate,
    const std::vector<E::NamedExpressionPtr> &project_expressions) {
  const std::size_t num_tables = join_group.tables.size();
  DCHECK_GE(num_tables, 2u);

  std::vector<TableInfo> table_info_storage;
  const std::vector<P::PhysicalPtr> &tables = join_group.tables;
  for (std::size_t i = 0; i < join_group.tables.size(); ++i) {
    table_info_storage.emplace_back(
        i,
        tables[i],
        cost_model_->estimateCardinality(tables[i]),
        cost_model_->estimateSelectivity(tables[i]));
  }

  // Auxiliary mapping info.
  std::unordered_map<E::ExprId, std::size_t> attribute_id_to_table_info_index_map;
  std::unordered_map<E::ExprId, E::AttributeReferencePtr> attribute_id_to_reference_map;
  for (std::size_t table_idx = 0; table_idx < num_tables; ++table_idx) {
    for (const E::AttributeReferencePtr &attr :
             table_info_storage[table_idx].table->getOutputAttributes()) {
      DCHECK(attribute_id_to_table_info_index_map.find(attr->id())
                 == attribute_id_to_table_info_index_map.end());

      attribute_id_to_table_info_index_map.emplace(attr->id(), table_idx);
      attribute_id_to_reference_map.emplace(attr->id(), attr);
    }
  }

  // Create a join graph where tables are vertices, and add an edge between vertices
  // t1 and t2 for each join predicate t1.x = t2.y
  std::vector<std::unordered_set<std::size_t>> join_graph(table_info_storage.size());
  for (const auto &attr_id_pair : join_group.join_attribute_pairs) {
    DCHECK(attribute_id_to_table_info_index_map.find(attr_id_pair.first)
               != attribute_id_to_table_info_index_map.end());
    DCHECK(attribute_id_to_table_info_index_map.find(attr_id_pair.second)
               != attribute_id_to_table_info_index_map.end());

    std::size_t first_table_idx =
        attribute_id_to_table_info_index_map[attr_id_pair.first];
    std::size_t second_table_idx =
        attribute_id_to_table_info_index_map[attr_id_pair.second];
    DCHECK_NE(first_table_idx, second_table_idx);

    table_info_storage[first_table_idx].join_attribute_pairs.emplace(
        attr_id_pair.first, attr_id_pair.second);
    table_info_storage[second_table_idx].join_attribute_pairs.emplace(
        attr_id_pair.second, attr_id_pair.first);

    join_graph[first_table_idx].emplace(second_table_idx);
    join_graph[second_table_idx].emplace(first_table_idx);
  }

  std::set<TableInfo*, TableInfoPtrLessComparator> table_info_ordered_by_priority;
  for (std::size_t i = 0; i < table_info_storage.size(); ++i) {
    table_info_ordered_by_priority.emplace(&table_info_storage[i]);
  }

  // Contruct hash join tree.
  while (true) {
    TableInfo *first_table_info = *table_info_ordered_by_priority.begin();
    table_info_ordered_by_priority.erase(
        table_info_ordered_by_priority.begin());
    const std::size_t first_table_info_id = first_table_info->table_info_id;

    TableInfo *second_table_info = nullptr;
    std::set<TableInfo*, TableInfoPtrLessComparator>::iterator second_table_info_it;
    for (auto candidate_table_info_it = table_info_ordered_by_priority.begin();
         candidate_table_info_it != table_info_ordered_by_priority.end();
         ++candidate_table_info_it) {
      TableInfo *candidate_table_info = *candidate_table_info_it;
      const std::size_t candidate_table_info_id = candidate_table_info->table_info_id;

      if (join_graph[first_table_info_id].find(candidate_table_info_id)
              == join_graph[first_table_info_id].end() &&
          join_graph[candidate_table_info_id].find(first_table_info_id)
              == join_graph[candidate_table_info_id].end()) {
        continue;
      } else if (second_table_info == nullptr) {
        second_table_info = candidate_table_info;
        second_table_info_it = candidate_table_info_it;
      }

      bool is_likely_many_to_many_join = false;
      for (const auto join_attr_pair : first_table_info->join_attribute_pairs) {
        if (candidate_table_info->joined_attribute_set.find(join_attr_pair.second)
                != candidate_table_info->joined_attribute_set.end()) {
          is_likely_many_to_many_join = true;
          break;
        }
      }
      for (const auto join_attr_pair : candidate_table_info->join_attribute_pairs) {
        if (first_table_info->joined_attribute_set.find(join_attr_pair.second)
                != first_table_info->joined_attribute_set.end()) {
          is_likely_many_to_many_join = true;
          break;
        }
      }
      if (!is_likely_many_to_many_join) {
        second_table_info = candidate_table_info;
        second_table_info_it = candidate_table_info_it;
        break;
      }
    }
    DCHECK(second_table_info != nullptr);
    table_info_ordered_by_priority.erase(second_table_info_it);

    const P::PhysicalPtr &left_child = first_table_info->table;
    const P::PhysicalPtr &right_child = second_table_info->table;
    std::vector<E::NamedExpressionPtr> output_attributes;
    for (const E::AttributeReferencePtr &left_attr : left_child->getOutputAttributes()) {
      output_attributes.emplace_back(left_attr);
    }
    for (const E::AttributeReferencePtr &right_attr : right_child->getOutputAttributes()) {
      output_attributes.emplace_back(right_attr);
    }

    std::vector<E::AttributeReferencePtr> left_join_attributes;
    std::vector<E::AttributeReferencePtr> right_join_attributes;
    std::unordered_set<expressions::ExprId> new_joined_attribute_set;
    for (const auto &join_attr_pair : first_table_info->join_attribute_pairs) {
      if (second_table_info->join_attribute_pairs.find(join_attr_pair.second)
              != second_table_info->join_attribute_pairs.end()) {
        left_join_attributes.emplace_back(
            attribute_id_to_reference_map[join_attr_pair.first]);
        right_join_attributes.emplace_back(
            attribute_id_to_reference_map[join_attr_pair.second]);

        new_joined_attribute_set.emplace(join_attr_pair.first);
        new_joined_attribute_set.emplace(join_attr_pair.second);
      }
    }
    DCHECK_GE(left_join_attributes.size(), static_cast<std::size_t>(1));

    if (table_info_ordered_by_priority.size() > 0) {
      P::PhysicalPtr output =
          P::HashJoin::Create(left_child,
                              right_child,
                              left_join_attributes,
                              right_join_attributes,
                              nullptr,
                              output_attributes,
                              P::HashJoin::JoinType::kInnerJoin);

      second_table_info->table = output;

      // TODO(jianqiao): Cache the estimated cardinality for each plan in cost
      // model to avoid duplicated estimation.
      second_table_info->estimated_cardinality = cost_model_->estimateCardinality(output);

      second_table_info->join_attribute_pairs.insert(first_table_info->join_attribute_pairs.begin(),
                                                     first_table_info->join_attribute_pairs.end());
      second_table_info->joined_attribute_set.insert(first_table_info->joined_attribute_set.begin(),
                                                     first_table_info->joined_attribute_set.end());
      second_table_info->joined_attribute_set.insert(new_joined_attribute_set.begin(),
                                                     new_joined_attribute_set.end());
      table_info_ordered_by_priority.emplace(second_table_info);

      join_graph[second_table_info->table_info_id].insert(join_graph[first_table_info_id].begin(),
                                                          join_graph[first_table_info_id].end());

    } else {
      return P::HashJoin::Create(left_child,
                                 right_child,
                                 left_join_attributes,
                                 right_join_attributes,
                                 residual_predicate,
                                 project_expressions,
                                 P::HashJoin::JoinType::kInnerJoin);
    }
  }
}

}  // namespace optimizer
}  // namespace quickstep
