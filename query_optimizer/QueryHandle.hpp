/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
   */
  QueryHandle()
      : query_plan_(new QueryPlan()),
        query_result_relation_(nullptr) {}

  ~QueryHandle() {}

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
