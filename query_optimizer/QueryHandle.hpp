/**
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>

#include "catalog/Catalog.pb.h"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryPlan.hpp"
#include "utility/Macros.hpp"

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
  /**
   * @brief Constructor.
   *
   * @param query_id The given query id.
   * @param cli_id The client id of the CLI which submits the query.
   * @param query_priority The priority of this query.
   */
  explicit QueryHandle(const std::size_t query_id,
                       const tmb::client_id cli_id,
                       const std::uint64_t query_priority = 1)
      : query_id_(query_id),
        cli_id_(cli_id),
        query_priority_(query_priority),
        query_plan_(new QueryPlan()),
        query_result_relation_(nullptr) {}

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

 private:
  const std::size_t query_id_;

  // The client id of the CLI which submits the query.
  const tmb::client_id cli_id_;

  const std::uint64_t query_priority_;

  std::unique_ptr<QueryPlan> query_plan_;

  serialization::QueryContext query_context_proto_;

  // TODO(quickstep-team): Use Catalog to support multiple databases.
  serialization::CatalogDatabase catalog_database_cache_proto_;

  // NOTE(zuyu): The relation gets created by the optimizer,
  //             and deleted by the Cli shell.
  const CatalogRelation *query_result_relation_;

  DISALLOW_COPY_AND_ASSIGN(QueryHandle);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_
