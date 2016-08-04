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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_HEURISTICS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_HEURISTICS_HPP_

#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryPlan.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "relational_operators/SelectOperator.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief The ExecutionHeuristics compiles certain heuristics for an execution plan
 *        as it is being converted to a physical plan. These heuristics can then be
 *        used to optimize the execution plan after it has been generated.
 **/
class ExecutionHeuristics {
 public:
  static const std::size_t kOneHundred = 100;
  static const std::size_t kOneThousand = 1000;
  static const std::size_t kTenThousand = 10000;
  static const std::size_t kHundredThousand = 100000;
  static const std::size_t kMillion = 1000000;

  static const std::size_t kCompressionFactor = 1;

  static const std::size_t kVeryLowSparsityHash = 1;
  static const std::size_t kLowSparsityHash = 2;
  static const std::size_t kMediumSparsityHash = 5;
  static const std::size_t kHighSparsityHash = 10;

  /**
   * @brief A simple internal class that holds information about various
   *        hash joins within the execution plan for a query.
   **/
  struct HashJoinInfo {
    HashJoinInfo(const QueryPlan::DAGNodeIndex build_operator_index,
                 const QueryPlan::DAGNodeIndex join_operator_index,
                 const CatalogRelation *referenced_stored_build_relation,
                 const CatalogRelation *referenced_stored_probe_relation,
                 const physical::BloomFilterConfig &bloom_filter_config,
                 std::vector<attribute_id> &&build_side_bloom_filter_ids,
                 std::vector<attribute_id> &&probe_side_bloom_filter_ids,
                 const QueryContext::join_hash_table_id join_hash_table_id,
                 const std::size_t estimated_build_relation_cardinality)
        : build_operator_index_(build_operator_index),
          join_operator_index_(join_operator_index),
          referenced_stored_build_relation_(referenced_stored_build_relation),
          referenced_stored_probe_relation_(referenced_stored_probe_relation),
          bloom_filter_config_(bloom_filter_config),
          build_side_bloom_filter_ids_(std::move(build_side_bloom_filter_ids)),
          probe_side_bloom_filter_ids_(std::move(probe_side_bloom_filter_ids)),
          join_hash_table_id_(join_hash_table_id),
          estimated_build_relation_cardinality_(estimated_build_relation_cardinality) {
    }

    const QueryPlan::DAGNodeIndex build_operator_index_;
    const QueryPlan::DAGNodeIndex join_operator_index_;
    const CatalogRelation *referenced_stored_build_relation_;
    const CatalogRelation *referenced_stored_probe_relation_;
    const physical::BloomFilterConfig &bloom_filter_config_;
    const std::vector<attribute_id> build_side_bloom_filter_ids_;
    const std::vector<attribute_id> probe_side_bloom_filter_ids_;
    const QueryContext::join_hash_table_id join_hash_table_id_;
    const std::size_t estimated_build_relation_cardinality_;
  };

  struct AggregateInfo {
    AggregateInfo(const QueryPlan::DAGNodeIndex aggregate_operator_index,
                  const physical::BloomFilterConfig &bloom_filter_config,
                  std::vector<attribute_id> &&bloom_filter_ids,
                  const QueryContext::aggregation_state_id aggregate_state_id)
        : aggregate_operator_index_(aggregate_operator_index),
          bloom_filter_config_(bloom_filter_config),
          bloom_filter_ids_(bloom_filter_ids),
          aggregate_state_id_(aggregate_state_id) {
    }

    const QueryPlan::DAGNodeIndex aggregate_operator_index_;
    const physical::BloomFilterConfig &bloom_filter_config_;
    const std::vector<attribute_id> bloom_filter_ids_;
    const QueryContext::aggregation_state_id aggregate_state_id_;
  };

  struct SelectInfo {
    SelectInfo(const QueryPlan::DAGNodeIndex select_operator_index,
               const physical::BloomFilterConfig &bloom_filter_config,
               std::vector<attribute_id> &&bloom_filter_ids,
               SelectOperator *select_operator)
        : select_operator_index_(select_operator_index),
          bloom_filter_config_(bloom_filter_config),
          bloom_filter_ids_(bloom_filter_ids),
          select_operator_(select_operator) {
    }

    const QueryPlan::DAGNodeIndex select_operator_index_;
    const physical::BloomFilterConfig &bloom_filter_config_;
    const std::vector<attribute_id> bloom_filter_ids_;
    SelectOperator *select_operator_;
  };

  /**
   * @brief Constructor.
   **/
  ExecutionHeuristics() {}

  /**
   * @brief Saves information about a hash join used within the execution plan
   *        for a query.
   *
   * @param build_operator_index Index of the build operator of the hash join.
   * @param join_operator_index Index of the join operator of the hash join.
   * @param build_relation_id Id of the relation on which hash table is being built.
   * @param probe_relation_id Id of the relation on which hash table is being probed.
   * @param build_attributes List of attributes on which hash table is being built.
   * @param probe_attributes List of attributes on which hash table is being probed.
   * @param join_hash_table_id Id of the hash table which refers to the actual hash
   *        table within the query context.
   **/
  inline void addHashJoinInfo(const QueryPlan::DAGNodeIndex build_operator_index,
                              const QueryPlan::DAGNodeIndex join_operator_index,
                              const CatalogRelation *referenced_stored_build_relation,
                              const CatalogRelation *referenced_stored_probe_relation,
                              const physical::BloomFilterConfig &bloom_filter_config,
                              std::vector<attribute_id> &&build_side_bloom_filter_ids,
                              std::vector<attribute_id> &&probe_side_bloom_filter_ids,
                              const QueryContext::join_hash_table_id join_hash_table_id,
                              const std::size_t estimated_build_relation_cardinality) {
    hash_joins_.emplace_back(build_operator_index,
                             join_operator_index,
                             referenced_stored_build_relation,
                             referenced_stored_probe_relation,
                             bloom_filter_config,
                             std::move(build_side_bloom_filter_ids),
                             std::move(probe_side_bloom_filter_ids),
                             join_hash_table_id,
                             estimated_build_relation_cardinality);
  }

  inline void addAggregateInfo(const QueryPlan::DAGNodeIndex aggregate_operator_index,
                               const physical::BloomFilterConfig &bloom_filter_config,
                               std::vector<attribute_id> &&bloom_filter_ids,
                               const QueryContext::aggregation_state_id aggregate_state_id) {
    aggregates_.emplace_back(aggregate_operator_index,
                             bloom_filter_config,
                             std::move(bloom_filter_ids),
                             aggregate_state_id);
  }

  inline void addSelectInfo(const QueryPlan::DAGNodeIndex select_operator_index,
                            const physical::BloomFilterConfig &bloom_filter_config,
                            std::vector<attribute_id> &&bloom_filter_ids,
                            SelectOperator *select_operator) {
    selects_.emplace_back(select_operator_index,
                          bloom_filter_config,
                          std::move(bloom_filter_ids),
                          select_operator);
  }

  /**
   * @brief Optimize the execution plan based on heuristics generated
   *        during physical plan to execution plan conversion.
   *
   * @param query_plan A mutable reference to the query execution plan.
   * @param query_context_proto A mutable reference to the protobuf representation
   *        of the query context.
   **/
  void optimizeExecutionPlan(QueryPlan *query_plan, serialization::QueryContext *query_context_proto);

  /**
   * @brief Set the properties of the bloom filter proto based on the statistics
   *        of the given relation.
   *
   * @param bloom_filter_proto A mutable reference to the bloom filter protobuf representation.
   * @param relation The catalog relation on which bloom filter is being built.
   **/
  void setBloomFilterProperties(serialization::BloomFilter *bloom_filter_proto,
                                const std::size_t cardinality);

 private:
  std::vector<HashJoinInfo> hash_joins_;
  std::vector<AggregateInfo> aggregates_;
  std::vector<SelectInfo> selects_;

  DISALLOW_COPY_AND_ASSIGN(ExecutionHeuristics);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_HEURISTICS_HPP_ */
