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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryOptimizerConfig.h"  // For QUICKSTEP_DISTRIBUTED.
#include "query_optimizer/QueryPlan.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

class CatalogRelation;

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief A handle to hold the query plan and resultant relation, if any.
 **/
class QueryHandle {
 public:
  // The info for generated queries during executing '\analyze'.
  struct AnalyzeQueryInfo {
    AnalyzeQueryInfo(const bool is_analyze_attribute_query_in,
                     const relation_id rel_id_in,
                     const std::size_t num_relations_in,
                     const attribute_id attr_id_in = kInvalidCatalogId,
                     const bool is_min_applicable_in = false,
                     const bool is_max_applicable_in = false)
      : is_analyze_attribute_query(is_analyze_attribute_query_in),
        rel_id(rel_id_in),
        num_relations(num_relations_in),
        attr_id(attr_id_in),
        is_min_applicable(is_min_applicable_in),
        is_max_applicable(is_max_applicable_in) {}

    const bool is_analyze_attribute_query;
    const relation_id rel_id;
    const std::size_t num_relations;

    // Only valid if 'is_analyze_attribute_query' is true.
    const attribute_id attr_id;
    const bool is_min_applicable;
    const bool is_max_applicable;
  };

  /**
   * @brief Constructor.
   *
   * @param query_id The given query id.
   * @param cli_id The client id of the CLI which submits the query.
   * @param query_priority The priority of this query.
   * @param analyze_query_info The info of this analyze query.
   */
  QueryHandle(const std::size_t query_id,
              const tmb::client_id cli_id,
              const std::uint64_t query_priority = 1,
              AnalyzeQueryInfo *analyze_query_info = nullptr)
      : query_id_(query_id),
        cli_id_(cli_id),
        query_priority_(query_priority),
        analyze_query_info_(analyze_query_info),
        query_plan_(new QueryPlan()),
        query_result_relation_(nullptr),
        mem_data_(nullptr, 0) {}

  ~QueryHandle() {}

  /**
   * @brief Get the query id.
   *
   * @return The query id.
   */
  std::size_t query_id() const {
    return query_id_;
  }

  /**
   * @brief Get the client id of the CLI which submits the query.
   */
  tmb::client_id getClientId() const {
    return cli_id_;
  }

  /**
   * @brief Get the query priority.
   **/
  const std::uint64_t query_priority() const {
    return query_priority_;
  }

  /**
   * @brief Get the query info for the command '\analyze'.
   **/
  const AnalyzeQueryInfo* analyze_query_info() const {
    return analyze_query_info_.get();
  }

  /**
   * @return The const query plan.
   */
  const QueryPlan& getQueryPlan() const {
    return *query_plan_;
  }

  /**
   * @return The mutable query plan.
   */
  QueryPlan* getQueryPlanMutable() {
    return query_plan_.get();
  }

  /**
   * @return The query context in the protobuf format.
   */
  const serialization::QueryContext& getQueryContextProto() const {
    return query_context_proto_;
  }

  /**
   * @return The mutable query context in the protobuf format.
   */
  serialization::QueryContext* getQueryContextProtoMutable() {
    return &query_context_proto_;
  }

  /**
    * @return The catalog database cache in the protobuf format.
    */
  const serialization::CatalogDatabase& getCatalogDatabaseCacheProto() const {
    return catalog_database_cache_proto_;
  }

  /**
   * @return The mutable catalog database cache in the protobuf format.
   */
  serialization::CatalogDatabase* getCatalogDatabaseCacheProtoMutable() {
    return &catalog_database_cache_proto_;
  }

  /**
   * @brief Get the query result relation.
   */
  const CatalogRelation* getQueryResultRelation() const {
    return query_result_relation_;
  }

  /**
   * @brief Set the query result relation.
   */
  void setQueryResultRelation(const CatalogRelation *relation) {
    query_result_relation_ = relation;
  }

  const StringPiece& getMemData() const {
    return mem_data_;
  }

  void setMemData(const StringPiece &mem_data) {
    mem_data_ = mem_data;
  }

#ifdef QUICKSTEP_DISTRIBUTED
  /**
   * @brief Whether the query will be executed in the single node.
   */
  bool is_single_node_query() const {
    return is_single_node_query_;
  }

  /**
   * @brief Set the query to be executed in the single node.
   */
  void set_is_single_node_query() {
    is_single_node_query_ = true;
  }
#endif  // QUICKSTEP_DISTRIBUTED

 private:
  const std::size_t query_id_;

  // The client id of the CLI which submits the query.
  const tmb::client_id cli_id_;

  const std::uint64_t query_priority_;
  std::unique_ptr<AnalyzeQueryInfo> analyze_query_info_;

  std::unique_ptr<QueryPlan> query_plan_;

  serialization::QueryContext query_context_proto_;

  // TODO(quickstep-team): Use Catalog to support multiple databases.
  serialization::CatalogDatabase catalog_database_cache_proto_;

  // NOTE(zuyu): The relation gets created by the optimizer,
  //             and deleted by the Cli shell.
  const CatalogRelation *query_result_relation_;

  StringPiece mem_data_;

#ifdef QUICKSTEP_DISTRIBUTED
  // Indicate whether the query should be executed on the default Shiftboss for
  // correctness purpose.
  // An example would be the insert query that might otherwise need block
  // invalidation among multiple StorageManagers. In this case, an insert query
  // has scheduled on node 0, and the block is in the buffer pool of node 0.
  // Another insert query on the same relation might be scheduled on another
  // node, say node 1, which will pull the block from node 0, and do the
  // insertion. Thus, two blocks with the same block id in two nodes
  // have different contents, which is incorrect.
  // One approach is to evict blocks cached in all other nodes for every
  // change. It, however, does not scale, and even worse, it will also affect
  // the performance of each select query.
  // Alternatively, we choose to mark the query as a single-node query to
  // modify blocks on the default node only. But if the changed block has also
  // cached in another node, this approach would still produce inconsistent
  // query result.
  bool is_single_node_query_ = false;
#endif  // QUICKSTEP_DISTRIBUTED

  DISALLOW_COPY_AND_ASSIGN(QueryHandle);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_
