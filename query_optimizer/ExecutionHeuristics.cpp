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
  while (origin_node < hash_joins_.size() - 1) {
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

    // Only chains of length greater than one are suitable candidates for semi-join optimization.
    if (chained_nodes.size() > 1) {
      std::unordered_map<QueryContext::bloom_filter_id, std::vector<attribute_id>> probe_bloom_filter_info;
      for (const std::size_t node : chained_nodes) {
        // Provision for a new bloom filter to be used by the build operator.
        const QueryContext::bloom_filter_id bloom_filter_id =  query_context_proto->bloom_filters_size();
        serialization::BloomFilter *bloom_filter_proto = query_context_proto->add_bloom_filters();

        // Modify the bloom filter properties based on the statistics of the relation.
        setBloomFilterProperties(bloom_filter_proto, hash_joins_[node].referenced_stored_build_relation_);

        // Add build-side bloom filter information to the corresponding hash table proto.
        const std::size_t num_partitions = hash_joins_[origin_node]
                                               .referenced_stored_build_relation_->getPartitionScheme()
                                               .getPartitionSchemeHeader()
                                               .getNumPartitions();
        for (std::size_t part_id = 0; part_id < num_partitions; ++part_id) {
          query_context_proto->mutable_join_hash_table_groups(hash_joins_[node].join_hash_table_group_id_)
              ->mutable_join_hash_tables(part_id)
              ->add_build_side_bloom_filter_id(bloom_filter_id);
        }

        probe_bloom_filter_info.insert(std::make_pair(bloom_filter_id, hash_joins_[node].probe_attributes_));
      }

      // Add probe-side bloom filter information to the corresponding hash table proto for each build-side bloom filter.
      std::vector<serialization::HashTable_ProbeSideBloomFilter *> probe_side_bloom_filter;
      const std::size_t num_partitions = hash_joins_[origin_node]
                                             .referenced_stored_probe_relation_->getPartitionScheme()
                                             .getPartitionSchemeHeader()
                                             .getNumPartitions();
      probe_side_bloom_filter.resize(num_partitions);
      for (std::size_t part_id = 0; part_id < num_partitions; ++part_id) {
        for (const std::pair<QueryContext::bloom_filter_id, std::vector<attribute_id>> &bloom_filter_info :
             probe_bloom_filter_info) {
          probe_side_bloom_filter[part_id] =
              query_context_proto->mutable_join_hash_table_groups(hash_joins_[origin_node].join_hash_table_group_id_)
                  ->mutable_join_hash_tables(part_id)
                  ->add_probe_side_bloom_filters();
          probe_side_bloom_filter[part_id]->set_probe_side_bloom_filter_id(bloom_filter_info.first);
          for (const attribute_id &probe_attribute_id : bloom_filter_info.second) {
            probe_side_bloom_filter[part_id]->add_probe_side_attr_ids(probe_attribute_id);
          }
        }
      }

      // Add node dependencies from chained build nodes to origin node probe.
      for (std::size_t i = 1; i < chained_nodes.size(); ++i) {  // Note: It starts from index 1.
        query_plan->addDirectDependency(hash_joins_[origin_node].join_operator_index_,
                                        hash_joins_[origin_node + i].build_operator_index_,
                                        true /* is_pipeline_breaker */);
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
