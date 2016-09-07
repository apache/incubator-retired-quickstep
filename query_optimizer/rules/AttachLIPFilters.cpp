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

#include "query_optimizer/rules/AttachLIPFilters.hpp"

#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/LIPFilterConfiguration.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "utility/lip_filter/LIPFilter.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr AttachLIPFilters::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  const P::TopLevelPlanPtr top_level_plan =
     std::static_pointer_cast<const P::TopLevelPlan>(input);
  cost_model_.reset(
      new cost::StarSchemaSimpleCostModel(
          top_level_plan->shared_subplans()));
  lip_filter_configuration_.reset(new P::LIPFilterConfiguration());

  std::set<E::ExprId> already_filtered_attributes;
  attachLIPFilters(NodeList(input), &already_filtered_attributes);

  P::PhysicalPtr output;
  if (!lip_filter_configuration_->getBuildInfoMap().empty() ||
      !lip_filter_configuration_->getProbeInfoMap().empty()) {
    output = top_level_plan->copyWithLIPFilterConfiguration(
        P::LIPFilterConfigurationPtr(lip_filter_configuration_.release()));
  } else {
    output = input;
  }
  return output;
}

void AttachLIPFilters::attachLIPFilters(
    const NodeList &path,
    std::set<expressions::ExprId> *already_filtered_attributes) {
  const P::PhysicalPtr &node = path.node;

  // First process child nodes
  for (const auto &child : node->children()) {
    std::set<E::ExprId> child_filtered_attributes;
    attachLIPFilters(path.cons(child), &child_filtered_attributes);
    already_filtered_attributes->insert(child_filtered_attributes.begin(),
                                        child_filtered_attributes.end());
  }

  // Attach LIP filters to HashJoin/Selection/Aggregate nodes
  P::PhysicalPtr probe_child = nullptr;
  switch (node->getPhysicalType()) {
    case P::PhysicalType::kHashJoin:
      probe_child = std::static_pointer_cast<const P::HashJoin>(node)->left();
      break;
    case P::PhysicalType::kSelection:
      probe_child = std::static_pointer_cast<const P::Selection>(node)->input();
      break;
    case P::PhysicalType::kAggregate:
      probe_child = std::static_pointer_cast<const P::Aggregate>(node)->input();
      break;
    default:
      break;
  }

  if (probe_child != nullptr &&
      cost_model_->estimateCardinality(probe_child) > 10000000) {
    const auto &candidate_lip_filters = getProbeSideInfo(path.cons(probe_child));
    if (!candidate_lip_filters.empty()) {
      std::map<E::AttributeReferencePtr, LIPFilterInfoPtr> selected_filters;
      for (const auto &info : candidate_lip_filters) {
        auto it = selected_filters.find(info->attribute);
        if (it == selected_filters.end()) {
          selected_filters.emplace(info->attribute, info);
        } else if (LIPFilterInfo::isBetterThan(*info, *it->second)) {
          it->second = info;
        }
      }

      for (const auto &pair : selected_filters) {
        const E::ExprId source_attr_id = pair.second->source_attribute->id();
        if (already_filtered_attributes->find(source_attr_id)
                == already_filtered_attributes->end()) {
          lip_filter_configuration_->addBuildInfo(
              pair.second->source_attribute,
              pair.second->source,
              pair.second->estimated_cardinality * 8,
              LIPFilterType::kSingleIdentityHashFilter);
          lip_filter_configuration_->addProbeInfo(
              pair.first,
              node,
              pair.second->source_attribute,
              pair.second->source);
          already_filtered_attributes->emplace(source_attr_id);
        }
      }
    }
  }
}

const std::vector<AttachLIPFilters::LIPFilterInfoPtr>& AttachLIPFilters
    ::getBuildSideInfo(const NodeList &path) {
  const P::PhysicalPtr &node = path.node;
  if (build_side_info_.find(node) == build_side_info_.end()) {
    std::vector<LIPFilterInfoPtr> lip_filters;

    // 1. Gather candidate LIP filters propagated from descendant nodes.
    std::unordered_set<E::ExprId> output_attribute_ids;
    for (const auto &attr : node->getOutputAttributes()) {
      output_attribute_ids.emplace(attr->id());
    }
    switch (node->getPhysicalType()) {
      case P::PhysicalType::kAggregate:
      case P::PhysicalType::kSelection:
      case P::PhysicalType::kHashJoin: {
        for (const P::PhysicalPtr &child : node->children()) {
          for (const LIPFilterInfoPtr &info : getBuildSideInfo(path.cons(child))) {
            lip_filters.emplace_back(info);
          }
        }
        break;
      }
      default:
        break;
    }

    // 2. Consider the parent physical node. If it is a HashJoin,
    // then each build-side join attribute is a candidate LIP filter
    // which can be built by the BuildHashOperator that corresponds
    // to the parent HashJoin node.
    P::HashJoinPtr hash_join;
    if (path.cdr() != nullptr &&
        P::SomeHashJoin::MatchesWithConditionalCast(path.cdr()->node, &hash_join)) {
      const P::PhysicalPtr &build_node = hash_join->right();
      // TODO(jianqiao): consider probe-side info to allow cascading propagation.
      double selectivity = cost_model_->estimateSelectivity(build_node);
      // Only consider attributes that are selective.
      if (selectivity < 1.0) {
        std::size_t cardinality = cost_model_->estimateCardinality(build_node);
        for (const auto &attr : hash_join->right_join_attributes()) {
          lip_filters.emplace_back(
              std::make_shared<LIPFilterInfo>(attr,
                                              path.cdr()->node,
                                              path.depth,
                                              selectivity,
                                              cardinality));
        }
      }
    }
    build_side_info_.emplace(node, std::move(lip_filters));
  }
  return build_side_info_.at(node);
}

const std::vector<AttachLIPFilters::LIPFilterInfoPtr>& AttachLIPFilters
    ::getProbeSideInfo(const NodeList &path) {
  const P::PhysicalPtr &node = path.node;
  if (probe_side_info_.find(node) == probe_side_info_.end()) {
    std::vector<LIPFilterInfoPtr> lip_filters;
    if (path.cdr() != nullptr) {
      // 1. Gather candidate LIP filters propagated from ancestor nodes.
      const auto &parent_lip_filters = getProbeSideInfo(*path.cdr());
      if (!parent_lip_filters.empty()) {
        std::unordered_set<E::ExprId> output_attribute_ids;
        for (const auto &attr : node->getOutputAttributes()) {
          output_attribute_ids.emplace(attr->id());
        }
        for (const auto &info : parent_lip_filters) {
          if (output_attribute_ids.find(info->attribute->id()) != output_attribute_ids.end()) {
            lip_filters.emplace_back(info);
          }
        }
      }

      // 2. Consider the parent physical node. If it is an InnerHashJoin or
      // LeftSemiHashJoin, then we can propagate the build-side LIP filters
      // to the probe-side.
      P::HashJoinPtr hash_join;
      if (P::SomeHashJoin::MatchesWithConditionalCast(path.cdr()->node, &hash_join) &&
          (hash_join->join_type() == P::HashJoin::JoinType::kInnerJoin ||
           hash_join->join_type() == P::HashJoin::JoinType::kLeftSemiJoin)) {
        const P::PhysicalPtr &build_side_child = hash_join->right();
        std::unordered_map<E::ExprId, E::AttributeReferencePtr> join_attribute_pairs;
        for (std::size_t i = 0; i < hash_join->left_join_attributes().size(); ++i) {
          const E::AttributeReferencePtr probe_side_join_attribute =
              hash_join->left_join_attributes()[i];
          const E::AttributeReferencePtr build_side_join_attribute =
              hash_join->right_join_attributes()[i];
          join_attribute_pairs.emplace(build_side_join_attribute->id(),
                                       probe_side_join_attribute);
        }
        for (const auto &info : getBuildSideInfo(path.cdr()->cons(build_side_child))) {
          const auto pair_it = join_attribute_pairs.find(info->attribute->id());
          if (pair_it != join_attribute_pairs.end()) {
            lip_filters.emplace_back(
                std::make_shared<LIPFilterInfo>(pair_it->second,
                                                info->source,
                                                info->depth,
                                                info->estimated_selectivity,
                                                info->estimated_cardinality,
                                                info->attribute));
          }
        }
      }
    }
    probe_side_info_.emplace(node, std::move(lip_filters));
  }
  return probe_side_info_.at(node);
}

}  // namespace optimizer
}  // namespace quickstep
