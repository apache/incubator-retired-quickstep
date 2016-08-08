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

#include "query_optimizer/ExecutionHeuristics.hpp"

#include <cstddef>
#include <utility>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryPlan.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

static const std::size_t kNumBitsPerByte = 8;
DEFINE_double(bloom_num_bits_per_tuple, kNumBitsPerByte,
    "Number of bits per tuple used to size the Bloom filter.");

DEFINE_int32(bloom_num_hash_fns, 1,
    "Number of hash functions used in the Bloom filter.");

void ExecutionHeuristics::optimizeExecutionPlan(QueryPlan *query_plan,
                                                serialization::QueryContext *query_context_proto) {
  std::map<std::pair<E::ExprId, P::PhysicalPtr>,
           std::pair<QueryContext::bloom_filter_id, QueryPlan::DAGNodeIndex>> bloom_filter_map;
  for (const auto &info : hash_joins_) {
    auto *hash_table_proto =
        query_context_proto->mutable_join_hash_tables(info.join_hash_table_id_);
    const auto &bloom_filter_config = info.bloom_filter_config_;

    for (std::size_t i = 0; i < info.build_side_bloom_filter_ids_.size(); ++i) {
      const QueryContext::bloom_filter_id bloom_filter_id = query_context_proto->bloom_filters_size();
      serialization::BloomFilter *bloom_filter_proto = query_context_proto->add_bloom_filters();
      setBloomFilterProperties(bloom_filter_proto, info.estimated_build_relation_cardinality_);

      const auto &build_side_bf =
          bloom_filter_config.build_side_bloom_filters[i];
      bloom_filter_map.emplace(
          std::make_pair(build_side_bf.attribute->id(),
                         bloom_filter_config.builder),
          std::make_pair(bloom_filter_id, info.build_operator_index_));

      auto *build_side_bloom_filter = hash_table_proto->add_build_side_bloom_filters();
      build_side_bloom_filter->set_bloom_filter_id(bloom_filter_id);
      build_side_bloom_filter->set_attr_id(info.build_side_bloom_filter_ids_[i]);
//      std::cerr << "Build " << build_side_bf.attribute->toString()
//                << " @" << bloom_filter_config.builder << "\n";
    }
  }

  for (const auto &info : hash_joins_) {
    auto *hash_table_proto =
        query_context_proto->mutable_join_hash_tables(info.join_hash_table_id_);
    const auto &bloom_filter_config = info.bloom_filter_config_;

    for (std::size_t i = 0; i < info.probe_side_bloom_filter_ids_.size(); ++i) {
      auto *probe_side_bloom_filter = hash_table_proto->add_probe_side_bloom_filters();
      const auto &probe_side_bf =
          bloom_filter_config.probe_side_bloom_filters[i];
      std::cerr << "HashJoin probe " << probe_side_bf.attribute->toString()
                << " @" << probe_side_bf.builder << "\n";

      const auto &build_side_info =
           bloom_filter_map.at(
               std::make_pair(probe_side_bf.source_attribute->id(),
                              probe_side_bf.builder));
      probe_side_bloom_filter->set_bloom_filter_id(build_side_info.first);
      probe_side_bloom_filter->set_attr_id(info.probe_side_bloom_filter_ids_[i]);
//      std::cerr << "HashJoin probe attr_id = " << info.probe_side_bloom_filter_ids_[i] << "\n";

      query_plan->addDirectDependency(info.join_operator_index_,
                                      build_side_info.second,
                                      true /* is_pipeline_breaker */);
    }
  }

  for (const auto &info : aggregates_) {
    auto *aggregate_proto =
        query_context_proto->mutable_aggregation_states(info.aggregate_state_id_);
    const auto &bloom_filter_config = info.bloom_filter_config_;

    for (std::size_t i = 0; i < info.bloom_filter_ids_.size(); ++i) {
      auto *bloom_filter = aggregate_proto->add_bloom_filters();
      const auto &bf =
          bloom_filter_config.probe_side_bloom_filters[i];
      std::cerr << "Aggregate probe " << bf.attribute->toString()
                << " @" << bf.builder << "\n";

      const auto &build_side_info =
           bloom_filter_map.at(
               std::make_pair(bf.source_attribute->id(),
                              bf.builder));
      bloom_filter->set_bloom_filter_id(build_side_info.first);
      bloom_filter->set_attr_id(info.bloom_filter_ids_[i]);
//      std::cerr << "Aggregate probe attr_id = "
//                << info.bloom_filter_ids_[i] << "\n";

      query_plan->addDirectDependency(info.aggregate_operator_index_,
                                      build_side_info.second,
                                      true /* is_pipeline_breaker */);
    }
  }

  for (const auto &info : selects_) {
    const auto &bloom_filter_config = info.bloom_filter_config_;

    for (std::size_t i = 0; i < info.bloom_filter_ids_.size(); ++i) {
      const auto &bf =
          bloom_filter_config.probe_side_bloom_filters[i];
      std::cerr << "Select probe " << bf.attribute->toString()
                << " @" << bf.builder << "\n";

      const auto &build_side_info =
           bloom_filter_map.at(
               std::make_pair(bf.source_attribute->id(),
                              bf.builder));
      info.select_operator_->addBloomFilter(
          build_side_info.first, info.bloom_filter_ids_[i]);
//      std::cerr << "Select probe attr_id = "
//                << info.bloom_filter_ids_[i] << "\n";

      query_plan->addDirectDependency(info.select_operator_index_,
                                      build_side_info.second,
                                      true /* is_pipeline_breaker */);
    }
  }
}

void ExecutionHeuristics::setBloomFilterProperties(serialization::BloomFilter *bloom_filter_proto,
                                                   const std::size_t cardinality) {
  bloom_filter_proto->set_bloom_filter_size(
      BloomFilter::getNearestAllowedSize(
          (FLAGS_bloom_num_bits_per_tuple * cardinality) / kNumBitsPerByte));
//  std::cerr << "bf size = " << bloom_filter_proto->bloom_filter_size() << "\n";
  bloom_filter_proto->set_number_of_hashes(FLAGS_bloom_num_hash_fns);
}

}  // namespace optimizer
}  // namespace quickstep
