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
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/physical/LIPFilterConfiguration.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "types/Type.hpp"
#include "utility/lip_filter/LIPFilter.hpp"
#include "utility/lip_filter/LIPFilter.pb.h"

#include "glog/logging.h"

using std::unordered_set;

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
      E::ExprId attr_id = info->build_attribute()->id();
      map_entry.emplace(attr_id, attribute_substitution_map.at(attr_id));
    }
  }
  // Check if a prober is attached to node.
  const auto &probe_info_map = lip_filter_configuration_->getProbeInfoMap();
  const auto probe_it = probe_info_map.find(node);
  if (probe_it != probe_info_map.end()) {
    auto &map_entry = attribute_map_[node];
    for (const auto &info : probe_it->second) {
      E::ExprId attr_id = info->probe_attribute()->id();
      map_entry.emplace(attr_id, attribute_substitution_map.at(attr_id));
    }
  }
}

void LIPFilterGenerator::deployLIPFilters(QueryPlan *execution_plan,
                                          serialization::QueryContext *query_context_proto) {
  lip_filter_builder_map_.clear();
  lip_filter_deployment_protos_.clear();

  // Deploy builders
  const auto &build_info_map = lip_filter_configuration_->getBuildInfoMap();
  for (const auto &info : builder_infos_) {
    const auto build_it = build_info_map.find(info.builder_node);
    if (build_it != build_info_map.end()) {
      deployBuilderInternal(execution_plan,
                            query_context_proto,
                            info.builder_node,
                            info.builder_operator_index,
                            build_it->second);
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
                          probe_it->second);
    }
  }

  // Attach LIPFilterDeployment information to the RelationalOperators.
  for (const auto &entry : lip_filter_deployment_protos_) {
    RelationalOperator *relop =
        execution_plan->getQueryPlanDAGMutable()->getNodePayloadMutable(entry.first);

    const auto &lip_filter_deployment_proto_pair = entry.second;
    unordered_set<QueryContext::lip_filter_id> lip_filter_indexes;
    const serialization::LIPFilterDeployment &lip_filter_deployment_proto =
        *(lip_filter_deployment_proto_pair.second);
    for (int i = 0; i < lip_filter_deployment_proto.build_entries_size(); ++i) {
      lip_filter_indexes.insert(lip_filter_deployment_proto.build_entries(i).lip_filter_id());
    }
    for (int i = 0; i < lip_filter_deployment_proto.probe_entries_size(); ++i) {
      lip_filter_indexes.insert(lip_filter_deployment_proto.probe_entries(i).lip_filter_id());
    }

    relop->deployLIPFilters(lip_filter_deployment_proto_pair.first, lip_filter_indexes);
  }
}

void LIPFilterGenerator::deployBuilderInternal(
    QueryPlan *execution_plan,
    serialization::QueryContext *query_context_proto,
    const physical::PhysicalPtr &builder_node,
    const QueryPlan::DAGNodeIndex builder_operator_index,
    const std::vector<physical::LIPFilterBuildInfoPtr> &build_info_vec) {
  auto *lip_filter_deployment_info_proto =
      getLIPFilterDeploymentProto(builder_operator_index, query_context_proto);

  const auto &builder_attribute_map = attribute_map_.at(builder_node);
  for (const auto &info : build_info_vec) {
    // Add the LIPFilter information into query context.
    const QueryContext::lip_filter_id lip_filter_id = query_context_proto->lip_filters_size();
    serialization::LIPFilter *lip_filter_proto = query_context_proto->add_lip_filters();
    const CatalogAttribute *target_attr =
        builder_attribute_map.at(info->build_attribute()->id());
    const Type &attr_type = target_attr->getType();

    switch (info->filter_type()) {
      case LIPFilterType::kSingleIdentityHashFilter: {
        DCHECK(!attr_type.isVariableLength());
        const P::SingleIdentityHashFilterBuildInfo &sihf_info =
            *std::static_pointer_cast<const P::SingleIdentityHashFilterBuildInfo>(info);
        lip_filter_proto->set_lip_filter_type(
            serialization::LIPFilterType::SINGLE_IDENTITY_HASH_FILTER);
        lip_filter_proto->SetExtension(serialization::SingleIdentityHashFilter::filter_cardinality,
                                       sihf_info.filter_cardinality());
        lip_filter_proto->SetExtension(serialization::SingleIdentityHashFilter::attribute_size,
                                       attr_type.minimumByteLength());
        break;
      }
      case LIPFilterType::kBitVectorExactFilter: {
        DCHECK(!attr_type.isVariableLength());
        const P::BitVectorExactFilterBuildInfo &bvef_info =
            *std::static_pointer_cast<const P::BitVectorExactFilterBuildInfo>(info);
        lip_filter_proto->set_lip_filter_type(
            serialization::LIPFilterType::BIT_VECTOR_EXACT_FILTER);
        lip_filter_proto->SetExtension(serialization::BitVectorExactFilter::min_value,
                                       bvef_info.min_value());
        lip_filter_proto->SetExtension(serialization::BitVectorExactFilter::max_value,
                                       bvef_info.max_value());
        lip_filter_proto->SetExtension(serialization::BitVectorExactFilter::attribute_size,
                                       attr_type.minimumByteLength());
        lip_filter_proto->SetExtension(serialization::BitVectorExactFilter::is_anti_filter,
                                       bvef_info.is_anti_filter());
        break;
      }
      default:
        LOG(FATAL) << "Unsupported LIPFilter type";
        break;
    }

    // Register the builder information which is needed later by the probers.
    lip_filter_builder_map_.emplace(
        std::make_pair(info->build_attribute()->id(), builder_node),
        std::make_pair(lip_filter_id, builder_operator_index));

    // Add the builder deployment information into query context.
    auto *lip_filter_entry_proto = lip_filter_deployment_info_proto->add_build_entries();
    lip_filter_entry_proto->set_lip_filter_id(lip_filter_id);
    lip_filter_entry_proto->set_attribute_id(target_attr->getID());
    lip_filter_entry_proto->mutable_attribute_type()->CopyFrom(attr_type.getProto());
  }
}

void LIPFilterGenerator::deployProberInteral(
    QueryPlan *execution_plan,
    serialization::QueryContext *query_context_proto,
    const physical::PhysicalPtr &prober_node,
    const QueryPlan::DAGNodeIndex prober_operator_index,
    const std::vector<physical::LIPFilterProbeInfoPtr> &probe_info_vec) {
  auto *lip_filter_deployment_info_proto =
      getLIPFilterDeploymentProto(prober_operator_index, query_context_proto);

  const auto &prober_attribute_map = attribute_map_.at(prober_node);
  for (const auto &info : probe_info_vec) {
    // Find the corresponding builder for the to-be-probed LIPFilter.
    const auto &builder_info =
        lip_filter_builder_map_.at(
            std::make_pair(info->build_attribute()->id(), info->builder()));
    const CatalogAttribute *target_attr =
        prober_attribute_map.at(info->probe_attribute()->id());

    // Add the prober deployment information into query context.
    auto *lip_filter_entry_proto = lip_filter_deployment_info_proto->add_probe_entries();
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
}

serialization::LIPFilterDeployment* LIPFilterGenerator::getLIPFilterDeploymentProto(
    const QueryPlan::DAGNodeIndex op_index,
    serialization::QueryContext *query_context_proto) {
  const auto proto_it = lip_filter_deployment_protos_.find(op_index);
  if (proto_it == lip_filter_deployment_protos_.end()) {
    const int lip_deployment_index =
        query_context_proto->lip_filter_deployments_size();
    auto *lip_filter_deployment_proto =
        query_context_proto->add_lip_filter_deployments();
    lip_filter_deployment_protos_.emplace(
        op_index, std::make_pair(lip_deployment_index, lip_filter_deployment_proto));
    return lip_filter_deployment_proto;
  } else {
    return proto_it->second.second;
  }
}

}  // namespace optimizer
}  // namespace quickstep
