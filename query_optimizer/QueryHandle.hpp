/**
 *   Copyright 2015 Pivotal Software, Inc.
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
#include <memory>
#include <utility>

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
   * @param The given query id.
   */
  explicit QueryHandle(const std::size_t query_id)
      : query_id_(query_id),
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

  std::unique_ptr<QueryPlan> query_plan_;

  serialization::QueryContext query_context_proto_;

  // NOTE(zuyu): The relation gets created by the optimizer,
  //             and deleted by the Cli shell.
  const CatalogRelation *query_result_relation_;

  DISALLOW_COPY_AND_ASSIGN(QueryHandle);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_QUERY_HANDLE_HPP_
