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

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "catalog/Catalog.pb.h"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryPlan.hpp"
#include "utility/Macros.hpp"

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
   */
  explicit QueryHandle(const std::size_t query_id,
                       const std::uint64_t query_priority = 1)
      : query_id_(query_id),
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

  void setEntryTime() {
    entry_time_ = std::chrono::steady_clock::now();
  }

  void setAdmissionTime() {
    admission_time_ = std::chrono::steady_clock::now();
  }

  void setCompletionTime() {
    completion_time_ = std::chrono::steady_clock::now();
  }

  const std::chrono::steady_clock::time_point& getEntryTime() const {
    return entry_time_;
  }

  const std::chrono::steady_clock::time_point& getAdmissionTime() const {
    return admission_time_;
  }

  const std::chrono::steady_clock::time_point& getCompletionTime() const {
    return completion_time_;
  }

  const double getExecutionTimeMillis() const {
    return std::chrono::duration<double, std::milli>(completion_time_ - admission_time_)
               .count();
  }

  const double getWaitingTimeMillis() const {
    return std::chrono::duration<double, std::milli>(admission_time_ - entry_time_).count();
  }

 private:
  const std::size_t query_id_;
  const std::uint64_t query_priority_;

  std::unique_ptr<QueryPlan> query_plan_;

  serialization::QueryContext query_context_proto_;

  // TODO(quickstep-team): Use Catalog to support multiple databases.
  serialization::CatalogDatabase catalog_database_cache_proto_;

  // NOTE(zuyu): The relation gets created by the optimizer,
  //             and deleted by the Cli shell.
  const CatalogRelation *query_result_relation_;

  // Time when query entered the system.
  std::chrono::steady_clock::time_point entry_time_;
  // Time when query was admitted to the system.
  std::chrono::steady_clock::time_point admission_time_;
  // Time when query finished its execution.
  std::chrono::steady_clock::time_point completion_time_;

  DISALLOW_COPY_AND_ASSIGN(QueryHandle);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_
