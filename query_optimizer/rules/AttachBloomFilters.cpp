/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_optimizer/rules/AttachBloomFilters.hpp"

#include <memory>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>
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

P::PhysicalPtr AttachBloomFilters::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);
  cost_model_.reset(
      new cost::StarSchemaSimpleCostModel(
          std::static_pointer_cast<const P::TopLevelPlan>(input)->shared_subplans()));

  visitProducer(input, 0);
  visitConsumer(input);

//  for (const auto &info_vec_pair : consumers_) {
//    std::cerr << "--------\n"
//              << "Node " << info_vec_pair.first->getName()
//              << " " << info_vec_pair.first << "\n";
//
//    for (const auto &info : info_vec_pair.second) {
//      std::cerr << info.attribute->attribute_alias();
//      if (info.attribute->id() != info.source_attribute->id()) {
//        std::cerr << "{FROM " << info.source_attribute->attribute_alias() << "}";
//      }
//      if (info.from_sibling) {
//        std::cerr << " sibling";
//      }
//      std::cerr << " @" << info.source << "[" << info.depth << "]"
//                << ": " << info.selectivity << "\n";
//    }
//    std::cerr << "********\n";
//  }

  return visitAndAttach(input);
}

void AttachBloomFilters::visitProducer(const P::PhysicalPtr &node, const int depth) {
  for (const P::PhysicalPtr &child : node->children()) {
    visitProducer(child, depth+1);
  }

  std::vector<BloomFilterInfo> bloom_filters;

  if (node->getPhysicalType() == P::PhysicalType::kHashJoin) {
    const P::HashJoinPtr &hash_join =
        std::static_pointer_cast<const P::HashJoin>(node);
    const P::PhysicalPtr &build_node = hash_join->right();
    double selectivity = cost_model_->estimateSelectivity(build_node);
    if (selectivity < 1.0) {
      auto &build_node_info = producers_[build_node];
      for (const auto &attr : hash_join->right_join_attributes()) {
        build_node_info.emplace_back(node, attr, depth, selectivity, false);
      }
    }
  }

  const std::vector<E::AttributeReferencePtr> output_attributes(
      node->getOutputAttributes());
  std::unordered_set<E::ExprId> output_attribute_ids;
  for (const auto &attr : output_attributes) {
    output_attribute_ids.emplace(attr->id());
  }

  // First check inherited bloom filters
  std::vector<const BloomFilterInfo*> candidates;
  switch (node->getPhysicalType()) {
    case P::PhysicalType::kAggregate:
    case P::PhysicalType::kSelection:
    case P::PhysicalType::kHashJoin: {
      for (const P::PhysicalPtr &child : node->children()) {
        for (const BloomFilterInfo &info : producers_[child]) {
          candidates.emplace_back(&info);
        }
      }
    }
    default:
      break;
  }

  for (const BloomFilterInfo *info : candidates) {
    if (output_attribute_ids.find(info->attribute->id()) != output_attribute_ids.end()) {
      bloom_filters.emplace_back(
          info->source, info->attribute, info->depth, info->selectivity, false);
    }
  }

  // Self-produced bloom filters
//  double selectivity = cost_model_->estimateSelectivity(node);
//  if (selectivity < 1.0) {
//    for (const auto &attr : output_attributes) {
//      bloom_filters.emplace_back(node, attr, depth, selectivity, false);
//    }
//  }

  producers_.emplace(node, std::move(bloom_filters));
}

void AttachBloomFilters::visitConsumer(const P::PhysicalPtr &node) {
  std::vector<BloomFilterInfo> bloom_filters;

  // Bloom filters from parent
  const auto &parent_bloom_filters = consumers_[node];
  if (!parent_bloom_filters.empty()) {
    for (const auto &child : node->children()) {
      std::unordered_set<E::ExprId> child_output_attribute_ids;
      for (const auto &attr : child->getOutputAttributes()) {
        child_output_attribute_ids.emplace(attr->id());
      }

      std::vector<BloomFilterInfo> bloom_filters;
      for (const auto &info : parent_bloom_filters) {
        if (child_output_attribute_ids.find(info.attribute->id())
                != child_output_attribute_ids.end()) {
          bloom_filters.emplace_back(info.source,
                                     info.attribute,
                                     info.depth,
                                     info.selectivity,
                                     false,
                                     info.source_attribute);
        }
      }
      consumers_.emplace(child, std::move(bloom_filters));
    }
  }

  // Bloom filters from build side to probe side via HashJoin
  if (node->getPhysicalType() == P::PhysicalType::kHashJoin) {
    const P::HashJoinPtr hash_join =
        std::static_pointer_cast<const P::HashJoin>(node);
    if (hash_join->join_type() == P::HashJoin::JoinType::kInnerJoin ||
        hash_join->join_type() == P::HashJoin::JoinType::kLeftSemiJoin) {
      const P::PhysicalPtr &producer_child = hash_join->right();
      const P::PhysicalPtr &consumer_child = hash_join->left();
      std::unordered_map<E::ExprId, E::AttributeReferencePtr> join_attribute_pairs;
      for (std::size_t i = 0; i < hash_join->left_join_attributes().size(); ++i) {
        const E::AttributeReferencePtr probe_join_attribute =
            hash_join->left_join_attributes()[i];
        const E::AttributeReferencePtr build_join_attribute =
            hash_join->right_join_attributes()[i];
        join_attribute_pairs.emplace(build_join_attribute->id(),
                                     probe_join_attribute);
      }

      auto &consumer_bloom_filters = consumers_[consumer_child];
      for (const auto &info : producers_[producer_child]) {
        const auto pair_it = join_attribute_pairs.find(info.attribute->id());
        if (pair_it != join_attribute_pairs.end()) {
          consumer_bloom_filters.emplace_back(info.source,
                                              pair_it->second,
                                              info.depth,
                                              info.selectivity,
                                              true,
                                              info.attribute);
        }
      }
    }
  }

  P::PhysicalPtr consumer_child = nullptr;
  if (node->getPhysicalType() == P::PhysicalType::kHashJoin) {
    consumer_child = std::static_pointer_cast<const P::HashJoin>(node)->left();
  }
  if (node->getPhysicalType() == P::PhysicalType::kAggregate) {
    consumer_child = std::static_pointer_cast<const P::Aggregate>(node)->input();
  }

  if (consumer_child != nullptr) {
    // Decide attaches
    auto &consumer_bloom_filters = consumers_[consumer_child];
    if (cost_model_->estimateCardinality(consumer_child) > 10000000 &&
        !consumer_bloom_filters.empty()) {
      std::map<E::AttributeReferencePtr, const BloomFilterInfo*> filters;
      for (const auto &info : consumer_bloom_filters) {
        auto it = filters.find(info.attribute);
        if (it == filters.end()) {
          filters.emplace(info.attribute, &info);
        } else {
          if (BloomFilterInfo::isBetterThan(&info, it->second)) {
            it->second = &info;
          }
        }
      }

      auto &probe_attaches = getBloomFilterConfig(node);
      for (const auto &pair : filters) {
        auto &build_attaches = getBloomFilterConfig(pair.second->source);
        build_attaches.addBuildSideBloomFilter(
            pair.second->source_attribute);
        probe_attaches.addProbeSideBloomFilter(
            pair.first,
            pair.second->source_attribute,
            pair.second->source);
      }
    }
  }

  for (const auto &child : node->children()) {
    visitConsumer(child);
  }
}

P::PhysicalPtr AttachBloomFilters::visitAndAttach(const physical::PhysicalPtr &node) {
  std::vector<P::PhysicalPtr> new_children;
  bool has_changed = false;
  for (const auto &child : node->children()) {
    P::PhysicalPtr new_child = visitAndAttach(child);
    if (new_child != child) {
      has_changed = true;
    }
    new_children.emplace_back(new_child);
  }

  if (node->getPhysicalType() == P::PhysicalType::kHashJoin) {
    const auto attach_it = attaches_.find(node);
    if (attach_it != attaches_.end()) {
//      for (const auto& item : attach_it->second.probe_side_bloom_filters) {
//        std::cout << "Attach probe from " << item.builder
//                  << " to " << node << "\n";
//      }

      const P::HashJoinPtr hash_join =
          std::static_pointer_cast<const P::HashJoin>(node);
      return P::HashJoin::Create(
          new_children[0],
          new_children[1],
          hash_join->left_join_attributes(),
          hash_join->right_join_attributes(),
          hash_join->residual_predicate(),
          hash_join->project_expressions(),
          hash_join->join_type(),
          attach_it->second);
    }
  }

  if (node->getPhysicalType() == P::PhysicalType::kAggregate) {
    const auto attach_it = attaches_.find(node);
    if (attach_it != attaches_.end()) {
//      for (const auto& item : attach_it->second.probe_side_bloom_filters) {
//        std::cout << "Attach probe from " << item.builder
//                  << " to " << node << "\n";
//      }

      const P::AggregatePtr aggregate =
          std::static_pointer_cast<const P::Aggregate>(node);
      return P::Aggregate::Create(
          aggregate->input(),
          aggregate->grouping_expressions(),
          aggregate->aggregate_expressions(),
          aggregate->filter_predicate(),
          attach_it->second);
    }
  }

  if (has_changed) {
    return node->copyWithNewChildren(new_children);
  }

  return node;
}

P::BloomFilterConfig& AttachBloomFilters::getBloomFilterConfig(const physical::PhysicalPtr &node) {
  if (attaches_.find(node) == attaches_.end()) {
    attaches_.emplace(node, node);
  }
  return attaches_[node];
}

}  // namespace optimizer
}  // namespace quickstep
