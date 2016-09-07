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

#include "query_optimizer/LIPFilterGenerator.hpp"

#include <map>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "query_execution/QueryContext.pb.h"
#include "relational_operators/RelationalOperator.hpp"
#include "types/Type.hpp"
#include "utility/lip_filter/LIPFilter.hpp"
#include "utility/lip_filter/LIPFilter.pb.h"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

void LIPFilterGenerator::registerAttributeMap(
    const P::PhysicalPtr &node,
    const std::unordered_map<E::ExprId, const CatalogAttribute *> &attribute_substitution_map) {
  // Check if a builder is attached to node.
  const auto &build_info_map = lip_filter_configuration_->getBuildInfoMap();
  const auto build_it = build_info_map.find(node);
  if (build_it != build_info_map.end()) {
    auto &map_entry = attribute_map_[node];
    for (const auto &info : build_it->second) {
      E::ExprId attr_id = info.build_attribute->id();
      map_entry.emplace(attr_id, attribute_substitution_map.at(attr_id));
    }
  }
  // Check if a prober is attached to node.
  const auto &probe_info_map = lip_filter_configuration_->getProbeInfoMap();
  const auto probe_it = probe_info_map.find(node);
  if (probe_it != probe_info_map.end()) {
    auto &map_entry = attribute_map_[node];
    for (const auto &info : probe_it->second) {
      E::ExprId attr_id = info.probe_attribute->id();
      map_entry.emplace(attr_id, attribute_substitution_map.at(attr_id));
    }
  }
}

void LIPFilterGenerator::deployLIPFilters(QueryPlan *execution_plan,
                                          serialization::QueryContext *query_context_proto) const {
  LIPFilterBuilderMap lip_filter_builder_map;

  // Deploy builders
  const auto &build_info_map = lip_filter_configuration_->getBuildInfoMap();
  for (const auto &info : builder_infos_) {
    const auto build_it = build_info_map.find(info.builder_node);
    if (build_it != build_info_map.end()) {
      deployBuilderInternal(execution_plan,
                            query_context_proto,
                            info.builder_node,
                            info.builder_operator_index,
                            build_it->second,
                            &lip_filter_builder_map);
    }
  }

  // Deploy probers
  const auto &probe_info_map = lip_filter_configuration_->getProbeInfoMap();
  for (const auto &info : prober_infos_) {
    const auto probe_it = probe_info_map.find(info.prober_node);
    if (probe_it != probe_info_map.end()) {
      deployProberInteral(execution_plan,
                          query_context_proto,
                          info.prober_node,
                          info.prober_operator_index,
                          probe_it->second,
                          lip_filter_builder_map);
    }
  }
}

void LIPFilterGenerator::deployBuilderInternal(
    QueryPlan *execution_plan,
    serialization::QueryContext *query_context_proto,
    const physical::PhysicalPtr &builder_node,
    const QueryPlan::DAGNodeIndex builder_operator_index,
    const std::vector<physical::LIPFilterBuildInfo> &build_info_vec,
    LIPFilterBuilderMap *lip_filter_builder_map) const {
  const auto lip_deployment_index = query_context_proto->lip_filter_deployments_size();
  auto *lip_filter_deployment_info_proto =
      query_context_proto->add_lip_filter_deployments();
  lip_filter_deployment_info_proto->set_action_type(serialization::LIPFilterActionType::BUILD);

  const auto &builder_attribute_map = attribute_map_.at(builder_node);
  for (const auto &info : build_info_vec) {
    // Add the LIPFilter information into query context.
    const QueryContext::lip_filter_id lip_filter_id = query_context_proto->lip_filters_size();
    serialization::LIPFilter *lip_filter_proto = query_context_proto->add_lip_filters();
    const CatalogAttribute *target_attr = builder_attribute_map.at(info.build_attribute->id());
    const Type &attr_type = target_attr->getType();

    switch (info.filter_type) {
      case LIPFilterType::kSingleIdentityHashFilter: {
        DCHECK(!attr_type.isVariableLength());
        lip_filter_proto->set_lip_filter_type(
            serialization::LIPFilterType::SINGLE_IDENTITY_HASH_FILTER);
        lip_filter_proto->SetExtension(
            serialization::SingleIdentityHashFilter::filter_cardinality, info.filter_cardinality);
        lip_filter_proto->SetExtension(
            serialization::SingleIdentityHashFilter::attribute_size, attr_type.minimumByteLength());
        break;
      }
      default:
        LOG(FATAL) << "Unsupported LIPFilter type";
        break;
    }

    // Register the builder information which is needed later by the probers.
    lip_filter_builder_map->emplace(
        std::make_pair(info.build_attribute->id(), builder_node),
        std::make_pair(lip_filter_id, builder_operator_index));

    // Add the builder deployment information into query context.
    auto *lip_filter_entry_proto = lip_filter_deployment_info_proto->add_entries();
    lip_filter_entry_proto->set_lip_filter_id(lip_filter_id);
    lip_filter_entry_proto->set_attribute_id(target_attr->getID());
    lip_filter_entry_proto->mutable_attribute_type()->CopyFrom(attr_type.getProto());
  }

  // Attach the LIPFilterDeployment information to the RelationalOperator.
  RelationalOperator *relop =
      execution_plan->getQueryPlanDAGMutable()->getNodePayloadMutable(builder_operator_index);
  relop->deployLIPFilters(lip_deployment_index);
}

void LIPFilterGenerator::deployProberInteral(
    QueryPlan *execution_plan,
    serialization::QueryContext *query_context_proto,
    const physical::PhysicalPtr &prober_node,
    const QueryPlan::DAGNodeIndex prober_operator_index,
    const std::vector<physical::LIPFilterProbeInfo> &probe_info_vec,
    const LIPFilterBuilderMap &lip_filter_builder_map) const {
  const auto lip_deployment_index = query_context_proto->lip_filter_deployments_size();
  auto *lip_filter_deployment_info_proto =
      query_context_proto->add_lip_filter_deployments();
  lip_filter_deployment_info_proto->set_action_type(serialization::LIPFilterActionType::PROBE);

  const auto &prober_attribute_map = attribute_map_.at(prober_node);
  for (const auto &info : probe_info_vec) {
    // Find the corresponding builder for the to-be-probed LIPFilter.
    const auto &builder_info =
        lip_filter_builder_map.at(
            std::make_pair(info.build_attribute->id(), info.builder));
    const CatalogAttribute *target_attr = prober_attribute_map.at(info.probe_attribute->id());

    // Add the prober deployment information into query context.
    auto *lip_filter_entry_proto = lip_filter_deployment_info_proto->add_entries();
    lip_filter_entry_proto->set_lip_filter_id(builder_info.first);
    lip_filter_entry_proto->set_attribute_id(target_attr->getID());
    lip_filter_entry_proto->mutable_attribute_type()->CopyFrom(
        target_attr->getType().getProto());

    // A prober must wait until the corresponding builder has completed building
    // the LIPFilter.
    execution_plan->addOrUpgradeDirectDependency(prober_operator_index,
                                                 builder_info.second,
                                                 true /* is_pipeline_breaker */);
  }

  // Attach the LIPFilterDeployment information to the RelationalOperator.
  RelationalOperator *relop =
      execution_plan->getQueryPlanDAGMutable()->getNodePayloadMutable(prober_operator_index);
  relop->deployLIPFilters(lip_deployment_index);
}

}  // namespace optimizer
}  // namespace quickstep
