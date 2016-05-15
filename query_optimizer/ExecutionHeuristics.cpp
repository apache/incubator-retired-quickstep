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

#include "query_optimizer/ExecutionHeuristics.hpp"

#include <cstddef>
#include <utility>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

void ExecutionHeuristics::optimizeExecutionPlan(QueryPlan *query_plan,
                                                serialization::QueryContext *query_context_proto) {
  // Currently this only optimizes left deep joins using bloom filters.
  // It uses a simple algorithm to discover the left deep joins.
  // It starts with the first hash join in the plan and keeps on iterating
  // over the next hash joins, till a probe on a different relation id is found.
  // The set of hash joins found in this way forms a chain and can be recognized
  // as a left deep join. It becomes a candidate for optimization.

  // The optimization is done by modifying each of the build operators in the chain
  // to generate a bloom filter on the build key during their hash table creation.
  // The leaf-level probe operator is then modified to query all the bloom
  // filters generated from all the build operators in the chain. These
  // bloom filters are queried to test the membership of the probe key
  // just prior to probing the hash table.

  QueryPlan::DAGNodeIndex origin_node = 0;
  while (origin_node < hash_joins_.size()) {
    std::vector<std::size_t> chained_nodes;
    chained_nodes.push_back(origin_node);
    for (std::size_t i = origin_node + 1; i < hash_joins_.size(); ++i) {
      const relation_id checked_relation_id = hash_joins_[origin_node].referenced_stored_probe_relation_->getID();
      const relation_id expected_relation_id = hash_joins_[i].referenced_stored_probe_relation_->getID();
      if (checked_relation_id == expected_relation_id) {
        chained_nodes.push_back(i);
      } else {
        break;
      }
    }
    
    if (chained_nodes.size() >= 1) {
      // There is a strategy on where the bloom filters from the build operators can be used.
      // Strategy: Bloom Filters can be pushed further down to the dependencies of the bottom-most probe operator.

      QueryPlan::DAGNodeIndex operator_consuming_bloom_filters = hash_joins_[origin_node].join_operator_index_;

      bool can_push_bloom_filters_to_dependencies = false;
      auto probe_dependencies = query_plan->getQueryPlanDAG().getDependencies(hash_joins_[origin_node].join_operator_index_);
      auto build_dependencies = query_plan->getQueryPlanDAG().getDependencies(hash_joins_[origin_node].build_operator_index_);
      for (auto const dependency : probe_dependencies) {
        if (build_dependencies.count(dependency) > 0) {
          // Check for the case where the probe operator is made dependent on build-side selection.
          continue;
        }
        RelationalOperator *relational_operator = query_plan->getQueryPlanDAGMutable()->getNodePayloadMutable(dependency);
        if (relational_operator->canApplyBloomFilter()) {
          can_push_bloom_filters_to_dependencies = true;
          operator_consuming_bloom_filters = dependency;
          break;
        }
      }

      if (can_push_bloom_filters_to_dependencies) {
        std::unordered_map<QueryContext::bloom_filter_id, std::vector<attribute_id>> probe_bloom_filter_info;
        std::vector<QueryPlan::DAGNodeIndex> operators_building_bloom_filter;
        
        for (const std::size_t node : chained_nodes) {
          // Check to make sure that there is no edge between the node building the bloom filter and the
          // node applying the bloom filter. This ensures that we do not introduce cycles in the DAG.
          if (query_plan->getQueryPlanDAG().getDependencies(hash_joins_[node].build_operator_index_)
              .count(operator_consuming_bloom_filters) > 0) {
            continue;
          }
          
          operators_building_bloom_filter.push_back(hash_joins_[node].build_operator_index_);
          
          // Provision for a new bloom filter to be used by the build operator.
          const QueryContext::bloom_filter_id bloom_filter_id =  query_context_proto->bloom_filters_size();
          serialization::BloomFilter *bloom_filter_proto = query_context_proto->add_bloom_filters();
          
          // Modify the bloom filter properties based on the statistics of the relation.
          setBloomFilterProperties(bloom_filter_proto, hash_joins_[node].referenced_stored_build_relation_);
          
          // Add build-side bloom filter information to the corresponding hash table proto.
          query_context_proto->mutable_join_hash_tables(hash_joins_[node].join_hash_table_id_)
              ->add_build_side_bloom_filter_id(bloom_filter_id);
          
          probe_bloom_filter_info.insert(std::make_pair(bloom_filter_id, hash_joins_[node].probe_attributes_));
        }
        
        RelationalOperator *relational_operator
            = query_plan->getQueryPlanDAGMutable()->getNodePayloadMutable(operator_consuming_bloom_filters);
        relational_operator->ingestBloomFilters((probe_bloom_filter_info));
        
        // Add edge dependencies from operators building bloom filters to operator consuming the bloom filters.
        for (QueryPlan::DAGNodeIndex dependency : operators_building_bloom_filter) {
          // Ensure that operator_consuming_bloom_filters is not already dependent on operator_building_bloom_filter.
          if (query_plan->getQueryPlanDAG().getDependencies(operator_consuming_bloom_filters).count(dependency) > 0) {
            continue;
          }
          query_plan->addDirectDependency(operator_consuming_bloom_filters,
                                          dependency,
                                          true /* is_pipeline_breaker */);
        }
      }
    }
    
    // Update the origin node.
    origin_node = chained_nodes.back() + 1;
  }
}

void ExecutionHeuristics::setBloomFilterProperties(serialization::BloomFilter *bloom_filter_proto,
                                                   const CatalogRelation *relation) {
  const std::size_t cardinality = relation->estimateTupleCardinality();
  if (cardinality < kOneThousand) {
    bloom_filter_proto->set_bloom_filter_size(kOneThousand / kCompressionFactor);
    bloom_filter_proto->set_number_of_hashes(kVeryLowSparsityHash);
  } else if (cardinality < kTenThousand) {
    bloom_filter_proto->set_bloom_filter_size(kTenThousand / kCompressionFactor);
    bloom_filter_proto->set_number_of_hashes(kLowSparsityHash);
  } else if (cardinality < kHundredThousand) {
    bloom_filter_proto->set_bloom_filter_size(kHundredThousand / kCompressionFactor);
    bloom_filter_proto->set_number_of_hashes(kMediumSparsityHash);
  } else {
    bloom_filter_proto->set_bloom_filter_size(kMillion / kCompressionFactor);
    bloom_filter_proto->set_number_of_hashes(kHighSparsityHash);
  }
}

}  // namespace optimizer
}  // namespace quickstep
