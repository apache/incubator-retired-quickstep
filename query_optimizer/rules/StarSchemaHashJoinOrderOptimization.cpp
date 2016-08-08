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
#include "utility/DisjointTreeForest.hpp"

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
      for (const auto &attr : input->getReferencedAttributes()) {
        new_join_group->referenced_attributes.emplace(attr->id());
      }
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
        cost_model_->estimateSelectivity(tables[i]),
        CountSharedAttributes(join_group.referenced_attributes,
                              tables[i]->getOutputAttributes()),
        tables[i]->getPhysicalType() == physical::PhysicalType::kAggregate);
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

  std::set<TableInfo*> remaining_tables;
  for (auto &table_info : table_info_storage) {
    remaining_tables.emplace(&table_info);
  }

  DisjointTreeForest<E::ExprId> join_attribute_forest;
  for (const auto &attr_id_pair : join_group.join_attribute_pairs) {
    join_attribute_forest.makeSet(attr_id_pair.first);
    join_attribute_forest.makeSet(attr_id_pair.second);
    join_attribute_forest.merge(attr_id_pair.first, attr_id_pair.second);
  }

  std::map<std::size_t, std::map<std::size_t, E::ExprId>> join_attribute_groups;
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

    DCHECK_EQ(join_attribute_forest.find(attr_id_pair.first),
              join_attribute_forest.find(attr_id_pair.second));
    const std::size_t attr_group_id = join_attribute_forest.find(attr_id_pair.first);
    auto &attr_group = join_attribute_groups[attr_group_id];
    attr_group.emplace(first_table_idx, attr_id_pair.first);
    attr_group.emplace(second_table_idx, attr_id_pair.second);
  }

  while (true) {
    // TODO(jianqiao): design better data structure to improve efficiency here.
    std::unique_ptr<JoinPair> best_join = nullptr;
    for (TableInfo *probe_table_info : remaining_tables) {
      for (TableInfo *build_table_info : remaining_tables) {
        if (probe_table_info != build_table_info) {
          std::vector<E::AttributeReferencePtr> build_attrs;
          const std::size_t probe_table_id = probe_table_info->table_info_id;
          const std::size_t build_table_id = build_table_info->table_info_id;
          for (const auto &attr_group_pair : join_attribute_groups) {
            const auto &attr_group = attr_group_pair.second;
            auto probe_it = attr_group.find(probe_table_id);
            auto build_it = attr_group.find(build_table_id);
            if (probe_it != attr_group.end() && build_it != attr_group.end()) {
              build_attrs.emplace_back(
                  attribute_id_to_reference_map.at(build_it->second));
            }
          }

          const bool build_side_unique =
              !build_attrs.empty() && build_table_info->table->impliesUniqueAttributes(build_attrs);
          std::unique_ptr<JoinPair> new_join(
              new JoinPair(probe_table_info,
                           build_table_info,
                           build_side_unique));
          if (best_join == nullptr || new_join->isBetterThan(*best_join)) {
//              if (best_join != nullptr) {
//                std::cerr << "(" << best_join->probe->estimated_selectivity
//                          << ", " << best_join->probe->estimated_cardinality << ")"
//                          << " -- "
//                          << "(" << best_join->build->estimated_selectivity
//                          << ", " << best_join->build->estimated_cardinality << ")"
//                          << "\n";
//                std::cerr << "REPLACED WITH\n";
//              }
//              std::cerr << "(" << new_join->probe->estimated_selectivity
//                        << ", " << new_join->probe->estimated_cardinality << ")"
//                        << " -- "
//                        << "(" << new_join->build->estimated_selectivity
//                        << ", " << new_join->build->estimated_cardinality << ")"
//                        << "\n****\n";
            best_join.reset(new_join.release());
          }
        }
      }
    }

    CHECK(best_join != nullptr);

    TableInfo *selected_probe_table_info = best_join->probe;
    TableInfo *selected_build_table_info = best_join->build;
//    std::cerr << "card: " << selected_probe_table_info->estimated_cardinality << "\n";
//    std::cerr << "card: " << selected_build_table_info->estimated_cardinality << "\n";
//    std::cerr << "--------\n";
    if (!best_join->build_side_unique &&
        selected_probe_table_info->estimated_cardinality < selected_build_table_info->estimated_cardinality) {
      std::swap(selected_probe_table_info, selected_build_table_info);
    }

//    std::cerr << selected_probe_table_info->estimated_selectivity
//              << " -- "
//              << selected_build_table_info->estimated_selectivity
//              << "\n";

//    std::cerr << selected_probe_table_info->estimated_num_output_attributes
//              << " -- "
//              << selected_build_table_info->estimated_num_output_attributes
//              << "\n";

    remaining_tables.erase(selected_probe_table_info);
    remaining_tables.erase(selected_build_table_info);

    const P::PhysicalPtr &probe_child = selected_probe_table_info->table;
    const P::PhysicalPtr &build_child = selected_build_table_info->table;
    std::vector<E::NamedExpressionPtr> output_attributes;
    for (const E::AttributeReferencePtr &probe_attr : probe_child->getOutputAttributes()) {
      output_attributes.emplace_back(probe_attr);
    }
    for (const E::AttributeReferencePtr &build_attr : build_child->getOutputAttributes()) {
      output_attributes.emplace_back(build_attr);
    }

    std::vector<E::AttributeReferencePtr> probe_attributes;
    std::vector<E::AttributeReferencePtr> build_attributes;
    const std::size_t probe_table_id = selected_probe_table_info->table_info_id;
    const std::size_t build_table_id = selected_build_table_info->table_info_id;
    for (const auto &attr_group_pair : join_attribute_groups) {
      const auto &attr_group = attr_group_pair.second;
      auto probe_it = attr_group.find(probe_table_id);
      auto build_it = attr_group.find(build_table_id);
      if (probe_it != attr_group.end() && build_it != attr_group.end()) {
        probe_attributes.emplace_back(
            attribute_id_to_reference_map.at(probe_it->second));
        build_attributes.emplace_back(
            attribute_id_to_reference_map.at(build_it->second));
      }
    }

    if (remaining_tables.size() > 0) {
      P::PhysicalPtr output =
          P::HashJoin::Create(probe_child,
                              build_child,
                              probe_attributes,
                              build_attributes,
                              nullptr,
                              output_attributes,
                              P::HashJoin::JoinType::kInnerJoin);

//      P::PhysicalPtr output;
//      if (selected_build_table_info->estimated_num_output_attributes >= 4 &&
//          selected_probe_table_info->estimated_num_output_attributes < 4) {
//        output = P::HashJoin::Create(build_child,
//                                     probe_child,
//                                     build_attributes,
//                                     probe_attributes,
//                                     nullptr,
//                                     output_attributes,
//                                     P::HashJoin::JoinType::kInnerJoin);
//      } else {
//        output = P::HashJoin::Create(probe_child,
//                                     build_child,
//                                     probe_attributes,
//                                     build_attributes,
//                                     nullptr,
//                                     output_attributes,
//                                     P::HashJoin::JoinType::kInnerJoin);
//      }

      selected_probe_table_info->table = output;

      // TODO(jianqiao): Cache the estimated cardinality for each plan in cost
      // model to avoid duplicated estimation.
      selected_probe_table_info->estimated_cardinality = cost_model_->estimateCardinality(output);
      selected_probe_table_info->estimated_selectivity = cost_model_->estimateSelectivity(output);

      selected_probe_table_info->estimated_num_output_attributes =
          CountSharedAttributes(join_group.referenced_attributes,
                                output->getOutputAttributes());
      selected_probe_table_info->is_aggregation = false;

      remaining_tables.emplace(selected_probe_table_info);

      // Update join attribute groups.
      for (auto &attr_group_pair : join_attribute_groups) {
        auto &attr_group = attr_group_pair.second;
        auto build_it = attr_group.find(build_table_id);
        if (build_it != attr_group.end()) {
          const E::ExprId attr_id = build_it->second;
          attr_group.erase(build_it);
          attr_group.emplace(probe_table_id, attr_id);
        }
      }
    } else {
      return P::HashJoin::Create(probe_child,
                                 build_child,
                                 probe_attributes,
                                 build_attributes,
                                 residual_predicate,
                                 project_expressions,
                                 P::HashJoin::JoinType::kInnerJoin);
    }
  }
}

std::size_t StarSchemaHashJoinOrderOptimization::CountSharedAttributes(
    const std::unordered_set<expressions::ExprId> &attr_set1,
    const std::vector<expressions::AttributeReferencePtr> &attr_set2) {
  std::size_t cnt = 0;
  for (const auto &attr : attr_set2) {
    if (attr_set1.find(attr->id()) != attr_set1.end()) {
      ++cnt;
    }
  }
  return cnt;
}


}  // namespace optimizer
}  // namespace quickstep
