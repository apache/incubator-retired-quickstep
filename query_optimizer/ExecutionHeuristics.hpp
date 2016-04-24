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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_HEURISTICS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_HEURISTICS_HPP_

#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryPlan.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

namespace execution_heuristics_internal {

/**
 * @brief A simple internal class that holds information about various
 *        hash joins within the execution plan for a query.
 **/
class HashJoinInfo {
 public:
  HashJoinInfo(QueryPlan::DAGNodeIndex build_operator_index,
               QueryPlan::DAGNodeIndex join_operator_index,
               const relation_id &build_relation_id,
               const relation_id &probe_relation_id,
               std::vector<attribute_id> &&build_attributes,
               std::vector<attribute_id> &&probe_attributes,
               const QueryContext::join_hash_table_id &join_hash_table_id)
    : build_operator_index_(build_operator_index),
      join_operator_index_(join_operator_index),
      build_relation_id_(build_relation_id),
      probe_relation_id_(probe_relation_id),
      build_attributes_(build_attributes),
      probe_attributes_(probe_attributes),
      join_hash_table_id_(join_hash_table_id) {
  }

  const QueryPlan::DAGNodeIndex build_operator_index_;
  const QueryPlan::DAGNodeIndex join_operator_index_;
  const relation_id build_relation_id_;
  const relation_id probe_relation_id_;
  const std::vector<attribute_id> build_attributes_;
  const std::vector<attribute_id> probe_attributes_;
  const QueryContext::join_hash_table_id join_hash_table_id_;
};

}  // namespace execution_heuristics_internal

typedef execution_heuristics_internal::HashJoinInfo HashJoinInfo;

/**
 * @brief Compiles certain heuristics as an execution plan is being
 *        converted to a physical plan and uses them to optimize
 *        the execution plan after it has been generated.
 **/
class ExecutionHeuristics {
 public:
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
  inline void addHashJoinInfo(QueryPlan::DAGNodeIndex build_operator_index,
                              QueryPlan::DAGNodeIndex join_operator_index,
                              const relation_id build_relation_id,
                              const relation_id probe_relation_id,
                              std::vector<attribute_id> *build_attributes,
                              std::vector<attribute_id> *probe_attributes,
                              const QueryContext::join_hash_table_id &join_hash_table_id) {
    hash_joins_.push_back(HashJoinInfo(build_operator_index,
                                       join_operator_index,
                                       build_relation_id,
                                       probe_relation_id,
                                       std::move(*build_attributes),
                                       std::move(*probe_attributes),
                                       join_hash_table_id));
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

 private:
  std::vector<HashJoinInfo> hash_joins_;

  DISALLOW_COPY_AND_ASSIGN(ExecutionHeuristics);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_HEURISTICS_HPP_ */
